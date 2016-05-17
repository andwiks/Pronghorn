<?php
global $base_url;
$theme_path = $base_url . '/' . path_to_theme();
if (module_exists('cdn') && cdn_status_is_enabled()){
    $theme_path = file_create_url('public://' . '/' . path_to_theme());
}
?>
<nav class="after_clear">
    <a href="<?php print url('user/voucher'); ?>"><?php print t('Online Store'); ?></a>
    <a href="<?php print url('user/voucher/offline'); ?>"><?php print t('Offline Store'); ?></a>
    <a href="<?php print url('user/voucher/redeemed'); ?>" class="active"><?php print t('Redeemed Today'); ?></a>
</nav>
<div class="list_voucher after_clear">
<?php foreach ($rows as $row_count => $row): ?>
    <div class="box_voucher offline">
        <div class="image"><img src="<?php print $row['field_coupon_product_image']; ?>" alt="voucher" /></div>
        <span class="date"><?php print $row['field_owned_coupon_expiry']; ?></span>
        <a href="" class="detail"><?php print t('lihat detail'); ?> ></a>
        <div class="data_popup" id="<?php print $row['nid']; ?>">
            <div class="img"><?php print $row['field_coupon_product_image']; ?></div>
            <div class="desc">
                <h3><?php print $row['title']; ?></h3>
                <h4><?php print $row['field_detail_simple_coupon']; ?></h4>
                <h5><?php print t('Syarat & Ketentuan'); ?></h5>
                <div class="tnc_desc"><?php print $row['field_coupon_tnc']; ?></div>
            </div>
        </div>
    </div>
<?php endforeach; ?>
</div>
