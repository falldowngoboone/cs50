<?php
    // configuration
    require("../includes/config.php");
    
    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("quote_form.php", ["title" => "Get a Quote"]);
    }
    
    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate form
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a symbol.");
        }
        
        // lookup the posted symbol
        $stock = lookup($_POST["symbol"]);
        
        // validate new entry
        if ($stock === false)
        {
            apologize("Either service is down or you may have submitted a bad symbol.");
        }

        // render the page
        else
        {
            render("quote_display.php", 
                ["stock" => $stock,
                "symbol" => $stock["symbol"], 
                "name" => $stock["name"], 
                "price" => $stock["price"]]);
        }
    }
    
?>
