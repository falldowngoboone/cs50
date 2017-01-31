<h1>Transaction History</h1>
<?php if (count($transactions) === 0) { ?>

    <p>You currently have no transaction history.</p>
    
<?php } else { ?>
<table class="table table-striped">
    <thead>
        <tr>
            <th>Type</th>
            <th>Time</th>
            <th>Symbol</th>
            <th class="text-right">Number</th>
            <th class="text-right">Price per share</th>
            <th class="text-right">Total</th>
        </tr>
    </thead>
    <tbody>
    <?php foreach($transactions as $trans): ?>
        <?php $buy = $trans["type"] === "buy" ?>
        <tr>
            <td><?= strtoupper($trans["type"]) ?></td>
            <td><?= date("D, M d g:i:s a T", strtotime($trans["time"])) ?></td>
            <td><?= $trans["symbol"] ?></td>
            <td class="text-right"><?= $trans["shares"] ?></td>
            <td class="text-right">$<?= $trans["price"] ?></td>
            <td class="text-right">
                <?php if ($buy) { ?>(<?php } ?>$<?= number_format($trans["shares"] * $trans["price"], 4, ".", ",") ?><?php if ($buy) { ?>)<?php } ?>
            </td>
        </tr>
    <?php endforeach ?>
    </tbody>
</table>
<?php } ?>
