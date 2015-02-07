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

/*TO DO : Move this to module*/
global $user;
$order = commerce_cart_order_load($user->uid);
$wrapper = entity_metadata_wrapper('commerce_order', $order);
$order_total = $wrapper->commerce_order_total->value();

?>
<table class="shop_table cart box-sm" <?php print $attributes; ?>>
   <?php if (!empty($title) || !empty($caption)) : ?>
     <caption><?php print $caption . $title; ?></caption>
  <?php endif; ?>
  <?php if (!empty($header)) : ?>
    <thead>
      <tr>
        <?php foreach ($header as $field => $label): ?>
          <th <?php if ($header_classes[$field]) { print 'class="'. $header_classes[$field] . '" '; } ?>>
            <strong><?php print $label; ?></strong>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>
  <?php endif; ?>
  <tbody>
    <?php foreach ($rows as $row_count => $row): ?>
      <tr class="cart_item" style="border: 1px solid #ddd;">
        <?php foreach ($row as $field => $content): ?>
          <td <?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
      
    <tr class="cart_item" style="border: 1px solid #ddd; background:#f7f7f7">
        <td class="product-name">&nbsp;</td>
        <td class="product-price">&nbsp;</td>
        <td class="product-price">
            <strong>TOTAL:</strong>
        </td>
        <td class="product-subtotal">
            <span class="amount"><strong><?php print commerce_currency_format($order_total['amount'], $order_total['currency_code']); ?></strong></span>
        </td>
    </tr>
  </tbody>
</table>