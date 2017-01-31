<?php
    // configuration
    require("../includes/config.php");
    
    $rows = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    $account = $rows[0];
    
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        if (isset($_GET["n"]) && $_GET["n"] === "pw")
        {
            render("change_pw.php", ["title" => "Account Profile"]);
        }
        else
        {
            render("account_profile.php", ["title" => "Account Profile", "account" => $account]);
        }
    }
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate form data
        if (empty($_POST["current_pw"]))
        {
            apologize("You must provide your current password.");
        }
        else if (crypt($_POST["current_pw"], $account["hash"]) != $account["hash"])
        {
            apologize("Your current password was incorrect.");
        }
        else if (empty($_POST["new_pw"]))
        {
            apologize("You must provide a new password.");
        }
        else if (empty($_POST["confirm_pw"]))
        {
            apologize("You must confirm your new password");
        }
        else if ($_POST["new_pw"] != $_POST["confirm_pw"])
        {
            apologize("Your password confirmation does not match.");
        }
        else
        {
            // update hash for current user
            query("UPDATE users SET hash = ? WHERE id = ?", crypt($_POST["new_pw"]), $_SESSION["id"]);
            redirect("/profile.php");
        }
    }
    
?>
