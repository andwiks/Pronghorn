<section class="transaction">
    <div class="wrapper_3 thanks_page after_clear ">
        <h2>Transaksi Anda Sukses</h2>
        <h5>Terima kasih telah mempercayakan Sepulsa sebagai dealer pulsa kamu!</h5>
        <span class="border"></span>

        <?php
        //if bank transfer
        ?>
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

    </div>
</section>
