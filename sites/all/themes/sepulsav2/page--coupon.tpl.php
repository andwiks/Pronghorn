<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July, 23th 2015.
 */

global $base_url;
$theme_path = $base_url . '/' . path_to_theme();
if (module_exists('cdn') && cdn_status_is_enabled()){
    $theme_path = file_create_url('public://' . '/' . path_to_theme());
}
?>

<?php drupal_add_js('https://code.jquery.com/ui/1.11.4/jquery-ui.js', array(
  'type' => 'external',
  'scope' => 'footer',
  'group' => JS_THEME,
  'every_page' => FALSE,
  'weight' => -1,
)); ?>
<section class="std_content">
<?php print render($title_suffix); ?>
    <div class="wrapper_2 voucher ">
      <?php print $messages; ?>
        <div class="c_left custom-voucher">
        <?php print render($page['right']); ?>
        </div>
        <?php
        /*
        <div class="c_right">
            <div class="grid_promo_bank">
                <?php print render($page['promo']); ?>
            </div>
            <div class="atau"><!--<span><?php print t('atau'); ?></span>--></div>
            <a href="<?php print url("checkout"); ?>" class="bt_next" ><?php print t('LANJUT KE PEMBAYARAN'); ?></a>
        </div>
        */
        ?>
        <div class="clear"></div>

        <div class="wrapper_2">
            <h2><?php print t('PILIH VOUCHER YANG KAMU SUKA'); ?></h2>
            <h5>
                <?php print t('Kamu bisa memilih 3 Voucher yang kamu suka.'); ?>
                <span class="border"></span>
            </h5>
            <div class="filter after_clear" style="border-bottom:0px; margin:0px;">
                <!--
                <div class="row">
                    <label><?php print t('Filter :'); ?></label>
                    <div class="input">
                        <select name="voucher_filter" id="voucher_filter">
                            <option value="All">--<?php print t('All'); ?>--</option>
                            <option value="Lifestyle">--<?php print t('Lifestyle'); ?>--</option>
                            <option value="Food">--<?php print t('Food'); ?>--</option>
                            <option value="Fashion">--<?php print t('Fashion'); ?>--</option>
                            <option value="Travel">--<?php print t('Travel'); ?>--</option>
                            <option value="E-Commerce">--<?php print t('E-Commerce'); ?>--</option>
                            <option value="Grocery">--<?php print t('Grocery'); ?>--</option>
                            <option value="Kids">--<?php print t('Kids'); ?>--</option>
                        </select>
                    </div>
                </div>
                -->
            </div>
        </div>

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
