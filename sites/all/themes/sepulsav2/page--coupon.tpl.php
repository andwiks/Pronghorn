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
        <div class="c_left">
        <?php print render($page['right']); ?>
        </div>
        <div class="c_right">
            <div class="grid_promo_bank">
                <?php print render($page['promo']); ?>
            </div>
            <div class="atau"><span><?php print t('atau'); ?></span></div>
            <a href="<?php print url("checkout"); ?>" class="bt_next" ><?php print t('LANJUT KE PEMBAYARAN'); ?></a>
        </div>
        <div class="clear"></div>

        <div class="wrapper_2">
            <h2><?php print t('PILIH VOUCHER YANG KAMU SUKA'); ?></h2>
            <h5>
                <?php print t('Kamu bisa memilih 3 Voucher yang kamu suka.'); ?>
                <span class="border"></span>
            </h5>
            <div class="filter after_clear">
                <div class="row">
                    <label>Sort by :</label>
                    <div class="input">
                        <select name="voucher_sort">
                            <option value="defualt">--Default--</option>
                            <option value="latest">--Latest Voucher--</option>
                        </select>
                    </div>
                </div>
                <div class="row">
                    <label>Filter :</label>
                    <div class="input">
                        <select name="voucher_filter">
                            <option value="All">--All--</option>
                            <option value="Lifestyle">--Lifestyle--</option>
                            <option value="Food">--Food--</option>
                            <option value="Fashion">--Fashion--</option>
                            <option value="Travel">--Travel--</option>
                            <option value="E-Commerce">--E-Commerce--</option>
                            <option value="Grocery">--Grocery--</option>
                            <option value="Kids">--Kids--</option>
                        </select>
                    </div>
                </div>
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
