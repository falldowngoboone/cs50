<div>
    <h1><?= $username ?>'s Portfolio</h1>
    <table class="table table-striped">
        <thead>
            <tr>
                <th>Stock Symbol</th>
                <th>Shares Owned</th>
                <th class="text-right">Price per Share</th>
            </tr>
        </thead>
        <tbody>
        <!-- $position is the return array of arrays from index.php lookups -->
        <?php foreach ($positions as $position): ?>
            <tr>
                <td><?= $position["symbol"] ?></td>
                <td><?= $position["shares"] ?></td>
                <td class="text-right">$<?= $position["price"] ?></td>
            </tr>
        <?php endforeach ?>
        <tbody>
        <tfoot>
            <tr>
                <td></td>
                <td class="text-right"><strong>Portfolio total:</strong></td>
                <td class="text-right"><strong>$<?= number_format(($portfolio), 4, ".", ",") ?></strong></td>
            </tr>
            <tr>
                <td></td>
                <td class="text-right"><strong>Current account balance:</strong></td>
                <td class="text-right"><strong>$<?= number_format(($cash), 4, ".", ",") ?></strong></td>
            </tr>
            <tr>
                <td></td>
                <td class="text-right"><strong>Total worth:</strong></td>
                <td class="text-right"><strong>$<?= number_format(($portfolio + $cash), 4, ".", ",") ?></strong></td>
            </tr>
        </tfoot>
    </table>

</div>
<div>
    <a href="logout.php">Log Out</a>
</div>
