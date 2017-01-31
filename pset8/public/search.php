<?php

    require(__DIR__ . "/../includes/config.php");

    // validate request and data
    if ($_SERVER["REQUEST_METHOD"] == "GET" && isset($_GET["geo"]))
    {
        // remove commas from keywords
        $keywords = str_replace(",", "", $_GET["geo"]) . "*";

        // search the database
        $places = query("SELECT * FROM places
            WHERE MATCH (country_code, postal_code, place_name, admin_name1, admin_code1)
            AGAINST (? IN BOOLEAN MODE)
            ORDER BY MATCH (place_name, admin_name1, admin_code1)
            AGAINST (? IN BOOLEAN MODE) DESC",$keywords, $keywords);
        
        // output places as JSON (pretty-printed for debugging convenience)
        header("Content-type: application/json");
        print(json_encode($places, JSON_PRETTY_PRINT));
    }

?>
