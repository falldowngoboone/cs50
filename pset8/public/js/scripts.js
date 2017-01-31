/**
 * scripts.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Global JavaScript.
 */

// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 42.3770, lng: -71.1256}, // Cambridge, Massachusetts
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place, timeout)
{
    // set latitude and longitude, create marker
    var latLng = new google.maps.LatLng(place.latitude, place.longitude);
    var marker;
    
    var image = {
        url: 'img/news.png',
        // This marker is 22 pixels wide by 22 pixels tall.
        size: new google.maps.Size(22, 22),
        // The origin for this image is 0,0.
        origin: new google.maps.Point(0,0),
        // The anchor for this image is the base of the flagpole at 0,32.
        anchor: new google.maps.Point(11, 31)
    };
    
    // add the event listener
    window.setTimeout(function() {
    
        // explicitly set width of label because: centering
        var labelWidth = 500;
        
        marker = new MarkerWithLabel({
            map: map,
            animation: google.maps.Animation.DROP,
            position: latLng,
            labelContent: place.place_name + ", " + place.admin_code1,
            labelAnchor: new google.maps.Point((labelWidth / 2), 0),
            labelClass: "label", // the CSS class for the label
            labelStyle: {opacity: 0.75, width: labelWidth + 'px'},
            icon: image
        });
        
        google.maps.event.addDomListener(marker, 'click', function() {
            showInfo(marker, getInfo(place.postal_code, marker));
        });
        
        markers.push(marker);
    }, timeout);
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {
        update();
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // remove markers whilst dragging
    google.maps.event.addListener(map, "dragstart", function() {
        removeMarkers();
    });

    // configure typeahead
    // https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
    $("#q").typeahead({
        autoselect: true,
        highlight: false,
        minLength: 2
    },
    {
        source: search,
        templates: {
            empty: "no places found yet",
            suggestion: _.template("<p><%- place_name %>, <%- admin_name1 %> <span class=\"small\"><%- postal_code %></span></p>")
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // ensure coordinates are numbers
        var latitude = (_.isNumber(suggestion.latitude)) ? suggestion.latitude : parseFloat(suggestion.latitude);
        var longitude = (_.isNumber(suggestion.longitude)) ? suggestion.longitude : parseFloat(suggestion.longitude);

        // set map's center
        map.setCenter({lat: latitude, lng: longitude});

        // update UI
        update(suggestion.postal_code);
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        hideInfo();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Hides info window.
 */
function hideInfo()
{
    info.close();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    // for each marker, remove it from the map
    markers.forEach(function(marker) {
        marker.setMap(null);
    });
    
    // set the matches array length to 0 because: performance™
    markers.length = 0;
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, cb)
{
    // get places matching query (asynchronously)
    var parameters = {
        geo: query
    };
    $.getJSON("search.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // call typeahead's callback with search results (i.e., places)
        cb(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) === "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='img/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Gathers the content at geo (zip code) and sets the content for the info window.
 */
function getInfo(geo, marker)
{
    var content = '';
    
    $.getJSON('articles.php', {geo: geo})
    .done(function(data, textStatus, jqXHR) {
        
        if (data.length == 0)
        {
        
            // if no data, let the user know
            content = '<p>Sorry, slow news day.</p>';
            showInfo(marker, content);
        }
        else
        {
            
            // initialize the content list
            content = '<ul id="infoList" style="margin: 10px 0 5px;">';
            
            // populate content
            data.forEach(function(article) {
                content += '<li class="infoItem"><a href="' + article.link + '">';
                content += article.title;
                content += '</a></li>';
            });
            
            content += '</ul>';

            // trigger info.open()
            showInfo(marker, content);
            
            // a bit of stylistic animation
            $('#infoList').hide().fadeIn('slow');
            $('.infoItem').hover(function() {
                var $highlight = $('<div class="infoHighlight" />'),
                    width = $(this).find('a').width();
                $(this).append($highlight);
                $highlight.animate({width: width}, 'fast');
            }, function() {
                $(this).find('.infoHighlight')
                .stop()
                .animate({width: 0}, 'slow', function() {
                    $(this).remove();
                });
            });
        }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

         // log error to browser's console
         console.log(errorThrown.toString());
     });
}

/**
 * Updates UI's markers.
 * [selection] - optional postal_code to ensure selected items are displayed
 */
function update(selection) 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng(),
        select: selection // passing in the selection's zip code
    };
    $.getJSON("update.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // remove old markers from map
        removeMarkers();

        // add new markers to map
        for (var i = 0; i < data.length; i++)
        {
            addMarker(data[i], i * 200);
        }
     })
     .fail(function(jqXHR, textStatus, errorThrown) {

         // log error to browser's console
         console.log(errorThrown.toString());
     });
};
