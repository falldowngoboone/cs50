<h1>Sell Shares</h1>
<form action="sell.php" method="post">
    <?php if (count($stocks) > 0) { ?>
    <fieldset>
        <div class="form-group">
            <select name="symbol">
                <option disabled selected>Select a stock</option>
                <?php foreach($stocks as $stock): ?>
                <option value="<?= $stock['symbol'] ?>"><?= $stock['symbol'] ?></option>
                <?php endforeach ?>
            </select>
        </div>
        <div class="form-group">
            <input class="form-control" name="shares" placeholder="Number of Shares" type="number"/>
        </div>
        <div class="form-group">
            <button type="submit" class="btn btn-default">Sell Shares</button>
        </div>
    </fieldset>
    <?php } else { ?>
    <p>You don't have any stock to sell. <a href="/buy.php">Buy some.</a>
    <?php } ?>
</form>
