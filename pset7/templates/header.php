<!DOCTYPE html>

<html lang="en">

    <head>
        <meta charset="UTF-8">
        <link href="/css/bootstrap.min.css" rel="stylesheet"/>
        <link href="/css/bootstrap-theme.min.css" rel="stylesheet"/>
        <link href="/css/styles.css" rel="stylesheet"/>

        <?php if (isset($title)): ?>
            <title>C$50 Finance: <?= htmlspecialchars($title) ?></title>
        <?php else: ?>
            <title>C$50 Finance</title>
        <?php endif ?>

        <script src="/js/jquery-1.11.1.min.js"></script>
        <script src="/js/bootstrap.min.js"></script>
        <script src="/js/scripts.js"></script>

    </head>

    <body>

        <div class="container">

            <div id="top">
                <a href="/"><img alt="C$50 Finance" src="/img/logo.gif"/></a>
                <?php 
                
                // hide nav for login and register pages
                if (!in_array($_SERVER["PHP_SELF"], ["/login.php", "/logout.php", "/register.php"])){ ?>
                   
                <nav role="navigation">
                    <ul class="nav nav-tabs nav-justified">
                        <li><a href="/quote.php">Quote</a></li>
                        <li><a href="/buy.php">Buy</a></li>
                        <li><a href="/sell.php">Sell</a></li>
                        <li><a href="/history.php">History</a></li>
                        <li><a href="/profile.php">Profile</a></li>
                        <li><a href="/logout.php">Log Out</a></li>
                    </ul>
                </nav>     
                
                <?php } ?>
            </div>

            <div id="middle">
