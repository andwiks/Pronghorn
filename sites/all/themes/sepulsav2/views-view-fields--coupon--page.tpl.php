<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */

global $base_url;
$web_name = $fields['field_website']->content;
$web_link = $fields['field_website_link']->content; 
if (!isset($web_link)) {
  $web_link = 'http://'.$fields['field_website']->content; 
}
  
?>
<div class="box_voucher">
    <div class="image"><img alt="" src="<?php print $fields['field_coupon_product_image']->content; ?>"></div>
    <h4><?php print $fields['title']->content; ?></h4>
    <p class="voucher-description"><?php print $fields['field_description']->content; ?>.</p>
    <a href="" class="detail">Lihat detail voucher</a>
    <?php print $fields['field_product']->content; ?>
    <div class="data_popup">
        <div class="img"><?php print $fields['field_coupon_product_image']->content; ?></div>
        <div class="desc">
            <h3><?php print $fields['title']->content; ?></h3>
            <h4><?php print $fields['field_description']->content; ?>.</h4>
            <h5><?php print t('Terms & Conditions'); ?></h5>
            <p><?php if (isset($fields['field_coupon_tnc']->content)) print $fields['field_coupon_tnc']->content; else print "-"; ?>
            </p>
        </div>
    </div>
</div>
