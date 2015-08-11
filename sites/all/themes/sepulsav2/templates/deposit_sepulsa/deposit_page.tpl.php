<p>
    Kamu bisa melihat sisa kredit, catatan pengisian pulsa,<br/>
    dan melakukan isi ulang deposit kamu di sini.
</p>
<div class="kredit">
    <span>Sisa kredit kamu</span>
    <?php print number_format($credit, 0, ",", ".") ?> IDR
</div>
<div class="nav_tab after_clear"> 
    <a href="" target="target_1" class="active">TOP UP Sepulsa Credit</a>
    <a href="" target="target_2" >Transaksi Sebelumnya</a>
</div>
<div class="content_tab">

    <div class="tab" id="target_1">
        <p>Untuk mempercepat dan memudahkan proses pengisian pulsa, TOP UP dan gunakan sepulsa kredit kamu.</p>
        <div class="form dompet">
            <?php print $deposit_form; ?>
        </div>
    </div>

    <div class="tab" id="target_2">
        <p>Berikut ini adalah rekam jejak penggunaan & top up sepulsa kredit yang pernah kamu lakukan</p>
        <?php print $history; ?>        
        
    </div>
</div>
