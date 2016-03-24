<?php
/**
 * @file
 * views-view-field--coupon--page--field-product.tpl.php
 *
 * @author andreas@sepulsa.com
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
// Check stock first.
try {
  if (isset($row->_field_data['commerce_product_field_data_field_product_product_id']['entity'])) :
    $entity = $row->_field_data['commerce_product_field_data_field_product_product_id']['entity'];
    $wrapper = entity_metadata_wrapper('commerce_product', $entity);
    $stock = $wrapper->commerce_stock->value();
  endif;
}
catch (Exception $e) {
  $stock = FALSE;
}
// Only if has stock or unable to get stock.
if ($stock > 0 || $stock === FALSE) :
  if (isset($row->field_commerce_price[0]['raw']['amount']) && empty($row->field_commerce_price[0]['raw']['amount'])) :
    $row->field_field_product[0]['rendered']['submit']['#value'] = t('Take Voucher');
    $row->field_field_product[0]['rendered']['submit']['#attached']['js'][0]['data']['ajax']['edit-submit']['submit']['_triggering_element_value'] = t('Take Voucher');
  else :
    $float_number = floatval($row->field_commerce_price[0]['raw']['amount']);
    $button = t('Rp @price', array(
      '@price' => number_format($float_number, 0, ',', '.'),
    ));
    $row->field_field_product[0]['rendered']['submit']['#attributes']['style'][] = 'text-transform: none;';
    $row->field_field_product[0]['rendered']['submit']['#value'] = $button;
    $row->field_field_product[0]['rendered']['submit']['#attached']['js'][0]['data']['ajax']['edit-submit']['submit']['_triggering_element_value'] = $button;
  endif;
endif;
print drupal_render($row->field_field_product[0]['rendered']); ?>
