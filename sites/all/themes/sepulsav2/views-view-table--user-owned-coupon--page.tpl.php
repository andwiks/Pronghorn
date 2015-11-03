<?php

/**
 * @file
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $caption: The caption for this table. May be empty.
 * - $header_classes: An array of header classes keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $classes: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * - $field_classes: An array of classes to apply to each field, indexed by
 *   field id, then row number. This matches the index in $rows.
 * @ingroup views_templates
 */
?>
<nav class="after_clear">
    <a href="<?php print url('user/voucher'); ?>" class="active"><?php print t('Online Store'); ?></a>
    <a href="<?php print url('user/voucher/offline'); ?>"><?php print t('Offline Store'); ?></a>
    <a href="<?php print url('user/voucher/redeemed'); ?>"><?php print t('Redeemed Today'); ?></a>
</nav>
<div class="list_voucher after_clear">
<?php foreach ($rows as $row_count => $row): ?>
    <div class="box_voucher">
        <div class="image"><img src="<?php print $row['field_coupon_product_image']; ?>" alt="voucher" /></div>
        <span class="date"><?php print $row['field_owned_coupon_expiry']; ?></span>
        <a href="" class="detail"><?php print t('lihat detail'); ?> ></a>
        <div class="data_popup">
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
