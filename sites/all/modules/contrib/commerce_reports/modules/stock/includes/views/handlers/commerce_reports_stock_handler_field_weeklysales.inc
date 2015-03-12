<?php

/**
 * Stock lifetime
 */
class commerce_reports_stock_handler_field_weeklysales extends views_handler_field_custom {

  function render($values) {
    $sku = $values->commerce_product_sku;
    return _commerce_reports_stock_calculate_dataset($sku, 'weeklysales');
  }
}
