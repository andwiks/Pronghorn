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

$web_name = $fields['field_website']->content;
$web_link = $fields['field_website_link']->content; 
if (!isset($web_link)) {
  $web_link = 'http://'.$fields['field_website']->content; 
}
  
?>

<div class="post-image">
  <div class="image-container">
    <a href="#pop<?php print $fields['nid']->content; ?>" class="image pop<?php print $fields['nid']->content; ?>_open"><img alt="" src="<?php print $fields['field_coupon_product_image']->content; ?>"></a>
  </div>
</div>
<div class="post-content">
  <div class="post-date"><span><?php print $fields['title']->content; ?></span></div>
  <h4 class="entry-title"><a href="#"><?php //print $fields['title']->content; ?></a></h4>
  <p>
    <?php print $fields['field_description']->content; ?>.
    Syarat penggunaan <a href="#pop<?php print $fields['nid']->content; ?>" class="image pop<?php print $fields['nid']->content; ?>_open" style="color:#dc2c27">klik disini</a>.
    <br />
    <a href="<?php print $web_link; ?>" target="_blank"><?php print $web_name; ?></a>
  </p>
  <div class="post-action">
    <?php print $fields['field_product']->content; ?>
  </div>
</div>

<div id="pop<?php print $fields['nid']->content; ?>" class="well pop<?php print $fields['nid']->content; ?>_close" style="max-width:44em;">
  <p><img src="<?php print $fields['field_coupon_product_image']->content; ?>" width="100%"></p>
  <h2 style="margin-bottom:5px"><strong><?php print $fields['title']->content; ?></strong></h2>
  <p>
    <?php print $fields['field_description']->content; ?>.
  </p>
  <hr class="color-text">
  <p><strong><?php print t('Terms & Conditions'); ?></strong></p>  
  <?php if (isset($fields['field_coupon_tnc']->content)) print $fields['field_coupon_tnc']->content; else print "-"; ?>
</div>

<?php
//TO DO : make it neater
?>
<script>
jQuery(document).ready(function () {
  jQuery('#pop<?php print $fields['nid']->content; ?>').popup();
});
</script>