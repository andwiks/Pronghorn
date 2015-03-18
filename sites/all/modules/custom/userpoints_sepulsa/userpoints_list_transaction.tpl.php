<?php //print "<pre>".print_r($content, true)."</pre>"; ?>
<?php if ($content['rows'] != []) { ?>
    <h3>Sepulsa Kredit</h3>
    <table class="table style1">
        <thead>
            <tr>
                <?php //foreach($content['header'] as $header) { ?>
                <th>Sepulsa Kredit</th>
                <th>Date</th>
                <th>Description</th>
                <th>Status</th>
                <?php //} ?>
            </tr>
        </thead>
        <tbody>
            <?php 
            if (isset($content['rows'])) {
            foreach($content['rows'] as $rows) { ?>
            <tr>
                <?php //foreach($rows['data'] as $rows_value) { ?>
                <td><?php print number_format($rows['data'][0]['data'],0,",",".") ?></td>
                <td><?php print $rows['data'][1]['data'] ?></td>
                <td><?php print $rows['data'][2]['data'] ?></td>
                <td><?php print $rows['data'][3]['data'] ?></td>
                <?php //} ?>
            </tr>
            <?php } 
            }?>
        </tbody>
    </table>
    Total Sepulsa Kredit : Rp.<?php print $content['totalamount'] ?>
<?php } ?>