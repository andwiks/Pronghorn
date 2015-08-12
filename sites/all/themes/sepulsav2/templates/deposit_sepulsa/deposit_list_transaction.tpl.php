<?php //print "<pre>".print_r($content, true)."</pre>"; ?>
<?php if ($content['rows'] != []) { ?>
    <table class="grid_akun dompet">
        <thead>
            <tr>
                <th><?php print t('Tanggal'); ?></th>
                <th><?php print t('Sepulsa Kredit'); ?></th>
                <th><?php print t('Deskripsi'); ?></th> 
            </tr>                    
        </thead>
        <tbody>
             <?php
            if (isset($content['rows'])) {
            foreach($content['rows'] as $rows) { ?>
            <tr>
                <?php //foreach($rows['data'] as $rows_value) { ?>
                <td><?php print $rows['data'][2]['data'] ?></td>
                <td><?php print number_format($rows['data'][0]['data'],0,",",".") ?></td>
                <td><?php print $rows['data'][3]['data'] ?></td>
                <?php //} ?>
            </tr>
            <?php }
            }?>
        </tbody>
    </table>

    <?php
        /*
        <strong>Total Sepulsa Kredit : Rp.<?php print $content['totalamount'] ?></strong>
        */
    ?>
<?php } ?>
