<?php

    // configuration
    require("../includes/config.php"); 
    
    // acquire the user's balance
    $user = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    
    $username = $user[0]["username"];
    $cash = $user[0]["cash"];
    
    // query the user's stocks in portfolio table
    $rows = query("SELECT * FROM portfolio WHERE id = ?", $_SESSION["id"]);
    
    $positions = [];
    $portfolio = 0;
    // for each stock, lookup() the symbol and push the result to $positions
    foreach ($rows as $row)
    {
        $stock = lookup($row["symbol"]);
        if ($stock !== false)
        {
            $positions[] = [
                "name" => $stock["name"],
                "price" => $stock["price"],
                "shares" => $row["shares"],
                "symbol" => $row["symbol"],
            ];
            
            $portfolio = $portfolio + ($stock["price"] * $row["shares"]);
        }
    }

    // render portfolio
    render("portfolio.php", [
        "title" => "Portfolio",
        "positions" => $positions,
        "username" => $username,
        "cash" => $cash,
        "portfolio" => $portfolio
    ]);

?>
