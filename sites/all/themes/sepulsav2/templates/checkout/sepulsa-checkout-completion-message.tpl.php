<?php
$theme_url = url(path_to_theme());
if (module_exists('facebook_twitter_share')) {
    $order_id = arg(1);
    $returned_facebook_twitter_share = facebook_twitter_share_set_return($order_id);
}
?>
<link rel="stylesheet" href="<?php print base_path() ?>sites/all/themes/sepulsav2/templates/checkout/nps/jquery-labelauty.css">
<script src="<?php print base_path() ?>sites/all/themes/sepulsav2/templates/checkout/nps/jquery-labelauty.js"></script>
<link rel="stylesheet" href="<?php print base_path() ?>sites/all/themes/sepulsav2/templates/checkout/nps/nps_style.css">
<script src="<?php print base_path() ?>sites/all/themes/sepulsav2/templates/checkout/nps/nps_script.js"></script>

<section class="transaction">
    <div class="wrapper_3 thanks_page after_clear ">
        <h2>TERIMA KASIH ATAS TRANSAKSI ANDA</h2>
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
        <div class="nps">
          <p class="question">Seberapa ingin anda merekomendasikan website ini kepada teman anda?</p>
          <table class="nps-rate">
          <tr>
            <td colspan="5" style="text-align:left;">
              <span class="title title-bad">Tidak Ingin</span>
            </td>
            <td colspan="5" style="text-align:right;">
              <span class="title title-good">Sangat Ingin</span>
            </td>
          </tr>
          <tr class="outer">
            <td>
              <input type="radio" name="nps" value="1">
              <span class="inner">1</span>
            </td>
            <td>
              <input type="radio" name="nps" value="2">
              <span class="inner">2</span>
            </td>
            <td>
              <input type="radio" name="nps" value="3">
              <span class="inner">3</span>
            </td>
            <td>
              <input type="radio" name="nps" value="4">
              <span class="inner">4</span>
            </td>
            <td>
              <input type="radio" name="nps" value="5">
              <span class="inner">5</span>
            </td>
            <td>
              <input type="radio" name="nps" value="6">
              <span class="inner">6</span>
            </td>
            <td>
              <input type="radio" name="nps" value="7">
              <span class="inner">7</span>
            </td>
            <td>
              <input type="radio" name="nps" value="8">
              <span class="inner">8</span>
            </td>
            <td>
              <input type="radio" name="nps" value="9">
              <span class="inner">9</span>
            </td>
            <td>
              <input type="radio" name="nps" value="10">
              <span class="inner">10</span>
            </td>
          </tr>
        </table>
        <button name="nps-submit" type="button">Vote</button>
        </div>

		<?php print $message; ?>

    </div>
</section>
