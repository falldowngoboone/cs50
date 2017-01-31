<?php
    // configuration
    require("../includes/config.php");

    // query history table for all transactions belonging to id
    $rows = query("SELECT * FROM history WHERE id = ? ORDER BY time DESC", $_SESSION["id"]);
    
    // for each row, push to $transactions array an array of values
    $transactions = [];
    
    foreach($rows as $row)
    {
        $transactions[] = $row;
    }
    
    render("history_table.php", ["title" => "Transaction History", "transactions" => $transactions]);
    
?>
