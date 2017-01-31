<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // serve up all the stocks currently owned by the user
        $stocks = query("SELECT symbol FROM portfolio WHERE id = ?", $_SESSION["id"]);
        
        // render form
        render("sell_form.php", ["title" => "Sell Shares", "stocks" => $stocks]);
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
        
        // find out if and how many shares the user owns of this particular stock
        $rows = query("SELECT * FROM portfolio WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);

        // if stock is owned by the user
        if (count($rows) == 1)
        {
            $row = $rows[0]; // first (and only) row
            $stock = lookup($_POST["symbol"]);
            $amount = $_POST["shares"] * $stock["price"];
            
            // validate the posted shares to sell
            if ($_POST["shares"] > $row["shares"])
            {
                // posted shares is greater than shares owned, invalid amount
                apologize("You have entered an amount of shares greater than you own");
            }
            else if ($_POST["shares"] == $row["shares"]) // posted equals amount owned
            {
                // delete the entire row in the database and update user balance
                query("START TRANSACTION");
                query("DELETE FROM portfolio WHERE id = ? AND symbol = ?", 
                    $_SESSION["id"], $_POST["symbol"]);
                query("UPDATE users SET cash = cash + ? WHERE id = ?", 
                    $amount, $_SESSION["id"]);
                query("COMMIT");
            }
            else // posted shares is less than shares owned
            {
                // update stock shares in the portfolio and user balance
                query("START TRANSACTION");
                query("UPDATE portfolio SET shares = shares - ? WHERE id = ? AND symbol = ?", 
                    $_POST["shares"], $_SESSION["id"], $_POST["symbol"]);
                query("UPDATE users SET cash = cash + ? WHERE id = ?", 
                    $amount, $_SESSION["id"]);
                query("COMMIT");
            }
            
            // set transaction history
            query("INSERT INTO history (id, type, symbol, shares, price) VALUES(?, ?, ?, ?, ?)",
                $_SESSION["id"], "sell", $_POST["symbol"], $_POST["shares"], $stock["price"]);
                    
            // render sell confirmation page, passing in symbol and amount
            render("sell_confirm.php", ["symbol" => $_POST["symbol"], "amount" => $amount]);
        }

    }

?>
