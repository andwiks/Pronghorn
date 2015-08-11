<?php
/**
 * @file
 * page.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;
$theme_path = $base_url . '/' . path_to_theme();
?>

<?php drupal_add_js('http://code.jquery.com/ui/1.11.4/jquery-ui.js', array(
  'type' => 'external',
  'scope' => 'footer',
  'group' => JS_THEME,
  'every_page' => FALSE,
  'weight' => -1,
)); ?>
<section class="std_content">
<?php print render($title_suffix); ?>
<?php print $messages; ?>
    <div class="wrapper_2 voucher ">
        <div class="c_left">
        <?php print render($page['right']); ?> 
        </div>
        <div class="c_right">
            <div class="grid_promo_bank">
                <?php print render($page['promo']); ?>
            </div>
            <div class="atau"><span>atau</span></div>
            <a href="<?php print url("checkout"); ?>" class="bt_next" >LANJUT KE PEMBAYARAN</a>
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

        <div class="coupon-separate" style="height:15px; clear:both; display:block;margin-top: 15px;border-top: 1px dashed #999;">&nbsp;</div>

        <div class="wrapper_3">
            <div class="list_voucher after_clear">
                <?php print render($page['content']); ?>
            </div>
            <a href="#" class="back_top">
                <img src="<?php print $theme_path; ?>/images/material/go_top.png" alt="" />
            </a>
        </div>

    </div>


</section>

<script>
/*$( document ).ready(function() {

$( document ).click(function() {
  $( ".grid_voucher_order" ).effect( "shake" );
});
 
});*/
</script>
