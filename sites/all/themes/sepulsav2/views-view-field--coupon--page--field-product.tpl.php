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
if (isset($row->field_commerce_price[0]['raw']['amount']) && empty($row->field_commerce_price[0]['raw']['amount'])) :
  $row->field_field_product[0]['rendered']['submit']['#value'] = t('Take Voucher');
  $row->field_field_product[0]['rendered']['submit']['#attached']['js'][0]['data']['ajax']['edit-submit']['submit']['_triggering_element_value'] = t('Take Voucher');
else :
  $row->field_field_product[0]['rendered']['submit']['#value'] = t('Buy Voucher');
  $row->field_field_product[0]['rendered']['submit']['#attached']['js'][0]['data']['ajax']['edit-submit']['submit']['_triggering_element_value'] = t('Buy Voucher');
endif;
?>
<?php print drupal_render($row->field_field_product[0]['rendered']); ?>
