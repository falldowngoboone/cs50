<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // render form
        render("buy_form.php", ["title" => "Buy Shares"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a stock symbol.");
        }
        else if (!preg_match("/^\d+$/", $_POST["shares"]))
        {
            apologize("You must provide a non-negative whole number of shares.");
        }
        
        // lookup the posted symbol
        $stock = lookup($_POST["symbol"]);
        
        // validate new entry
        if ($stock === false)
        {
            apologize("Either service is down or you may have submitted a bad symbol.");
        }
        
        else
        {
            // select the user's info
            $rows = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
            $row = $rows[0];
            
            // cache the amount
            $amount = $_POST["shares"] * $stock["price"];
            
            // uppercase the symbol
            $symbol = strtoupper($_POST["symbol"]);
            
            // validate if user's balance can cover the transaction
            if ($amount > $row["cash"])
            {
                // posted shares is greater than shares owned, invalid amount
                apologize("You don't have enough money for the transaction.");
            }
            else // posted shares is less than shares owned
            {
                // update stock shares in the portfolio and user balance
                query("START TRANSACTION");
                query("INSERT INTO portfolio (id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)",
                    $_SESSION["id"], $symbol, $_POST["shares"]);
                query("UPDATE users SET cash = cash - ? WHERE id = ?", 
                    $amount, $_SESSION["id"]);
                query("COMMIT");
                
                // set transaction history
                query("INSERT INTO history (id, type, symbol, shares, price) VALUES(?, ?, ?, ?, ?)",
                    $_SESSION["id"], "buy", $symbol, $_POST["shares"], $stock["price"]);
            }
            
            // render sell confirmation page, passing in symbol and amount
            render("buy_confirm.php", ["symbol" => $_POST["symbol"], "amount" => $amount]);
        }

    }

?>
