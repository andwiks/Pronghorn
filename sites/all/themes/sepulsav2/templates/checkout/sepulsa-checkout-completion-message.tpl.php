<?php
$theme_url = url(path_to_theme());
if (module_exists('facebook_twitter_share')) {
    $order_id = arg(1);
    $returned_facebook_twitter_share = facebook_twitter_share_set_return($order_id);
}
?>
<section class="transaction">
    <div class="wrapper_3 thanks_page after_clear ">
        <h2>Transaksi Anda Sukses</h2>
        <h5>Terima kasih telah mempercayakan Sepulsa sebagai dealer pulsa kamu!</h5>
        <?php if (module_exists('facebook_twitter_share') && count($returned_facebook_twitter_share) > 0) { ?>
        <div class="facebook_twitter_share_container">
            <div class="facebook_twitter_share_description">
                <?php echo $returned_facebook_twitter_share["info"]; ?>
            </div>
            <div class="facebook_twitter_share_icon">
                <?php if(isset($returned_facebook_twitter_share["facebook"]) && !empty($returned_facebook_twitter_share["facebook"])){ ?>
                <a href="<?php echo $returned_facebook_twitter_share["facebook"]; ?>" target="_blank"><image src="<?php echo $theme_url; ?>/images/material/sr_scfb.png" alt="facebook share" /></a>
                <?php } ?>
                <?php if(isset($returned_facebook_twitter_share["twitter"]) && !empty($returned_facebook_twitter_share["twitter"])){ ?>
                <a href="<?php echo $returned_facebook_twitter_share["twitter"]; ?>" target="_blank"><image src="<?php echo $theme_url; ?>/images/material/sr_sctw.png" alt="twitter share" /></a>
                <?php } ?>
            </div>
        </div>
        <?php } ?>
        <span class="border"></span>

        <?php
        //if bank transfer
        /*
        <div class="c_left">
            <h4>
                <img src="<?php print url(path_to_theme()); ?>/images/material/bank_transfer_info.png" alt="img bank" />
                BANK TRANSFER INFO
            </h4>
            <div class="checkout_complete">
            	<?php print $message; ?>
        	</div>
        </div>
        <div class="c_right">
            <h4>
                <img src="<?php print url(path_to_theme()); ?>/images/material/informasi_penting.png" alt="img info" />
                INFORMASI PENTING
            </h4>
            <p>
                Mohon lakukan pembayaran melalui internet/mobile banking atau melalui teller di bank pilihan kamu ke akun berikut dalam jangka waktu <span>kurang dari 24 jam</span>. Jika tidak pesanan kamu akan dibatalkan.
                <br/><br/>
                Untuk tahap akhir proses pembayaran, silakan lakukan konfirmasi pembayaran dengan melakukan konfirmasi pembayaran <a href="<?php print url('konfirmasi'); ?>">disini</a> agar transaksi kamu dapat langsung kami proses.
                <br/><br/>
                Pulsa kamu akan terisi setelah pembayaran sukses dan dikonfirmasi.
                <br/><br/>
                Jika ini adalah transaksi pertama kamu, silahkan cek email kamu untuk mendapatkan instruksi login. Setelah login, kamu bisa melihat status pulsa kamu di halaman <a href="<?php print url('user'); ?>">akun saya</a>.
            </p>
        </div>

    	<?php
        //if not bank transfer
        /*
        	<div class="cc_thankyou" style="width:100%">
        		print $message;
        	</div>
    	*/
        ?>
		
		<?php print $message; ?>

    </div>
</section>
