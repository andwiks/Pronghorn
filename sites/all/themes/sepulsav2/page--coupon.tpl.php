<?php
/**
 * @file
 * page.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;

?>
<section class="std_content">
<?php print render($title_suffix); ?>
<?php print $messages; ?>
    <div class="wrapper_2 voucher ">
        <?php print render($page['right']); ?> 
        <div class="c_right">
            <div class="grid_promo_bank">
                <?php print render($page['promo']); ?>
            </div>
            <div class="atau"><span>atau</span></div>
            <a href="" class="bt_next" >LANJUT KE PEMBAYARAN</a>
        </div>
        <div class="clear"></div>

        <div class="wrapper_2">
            <h2>PILIH VOUCHER YANG KAMU SUKA</h2>
            <h5>
                Saat ini kamu memilih untuk melakukan pembayaran dengan kartu kredit BCA,<br/>
                kamu bisa memilih tambahan 2 Voucher Diskon Special.
                <span class="border"></span>
            </h5>
        </div> 
        <div class="wrapper_3">
            <div class="slider_voucher">
                <div class="slider">
                    
                </div>
            </div>
            <div class="list_voucher after_clear">
                <?php print render($page['content']); ?>
            </div>
            <a href="#" class="back_top">
                <img src="images/material/go_top.png" alt="" />
            </a>
        </div>

    </div>


</section>
