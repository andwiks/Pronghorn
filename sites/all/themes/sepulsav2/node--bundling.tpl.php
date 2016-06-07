<?php
/**
 * @file
 * node--bundling.tpl.php
 *
 * @author andreas@sepulsa.com
 */

// Get current theme path.
$theme_path = drupal_get_path('theme', $GLOBALS['theme']);

// Include bundling css.
drupal_add_css($theme_path . '/css/bundling.css');

// Include bundling js.
drupal_add_js('
  (function ($) {
    $(document).ready(function () {
      $("input[type=text]").each(function() {
        $(this).parents("form").find(":submit.input-btn").prop("disabled", this.value == "" ? true : false);
      });
      $("input[type=text]").change(function(){
        $(this).parents("form").find(":submit.input-btn").prop("disabled", this.value == "" ? true : false);
      });
      $("input[type=text]").keyup(function(){
        $(this).parents("form").find(":submit.input-btn").prop("disabled", this.value == "" ? true : false);
      });
    });
  }(jQuery));
', 'inline');

?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <div class="field field-name-body field-type-text-with-summary field-label-hidden">
      <div class="field-items">
        <div class="field-item even">
          <div class="w3-row row">
            <div class="w3-col l12 m12 s12 header">
              <?php print render($content['field_main_banner']); ?>
            </div>
          </div>

          <div class="product-bundling">
            <div class="w3-row row wrapper">
              <?php if (!empty($messages)) : ?>
              <div class="w3-col l12 m12 s12">
                <div class="custom-alert">
                  <?php print $messages; ?>
                </div>
              </div>
              <?php endif; ?>

              <?php foreach ($content['field_product']['#items'] as $product) : ?>
              <?php
                $data = commerce_product_load($product['product_id']);
                $description = field_view_field('commerce_product', $data, 'field_bundling_description', 'default');
                $bundling_banner = field_view_field('commerce_product', $data, 'field_bundling_banner', 'default');

                // Check status, expiry date, and stock.
                if ($data->status && REQUEST_TIME <= $data->field_expiry_date[LANGUAGE_NONE][0]['value']
                  && $data->commerce_stock[LANGUAGE_NONE][0]['value'] > 0
                  && sepulsa_bundle_voucher_stock_status($data)
                ) :
                  $voucher = commerce_product_load($data->field_bundling_voucher[LANGUAGE_NONE][0]['target_id']);
                  // Get fresh data from content field_product.
                  $product_render = $content['field_product'];
                  $product_render[0]['product_id']['#options'] = array(
                    $product['product_id'] => $content['field_product'][0]['product_id']['#options'][$product['product_id']],
                  );
                  $product_render[0]['product_id']['#default_value'] = $product['product_id'];
                  if (isset($content['field_product'][0]['error_data']['#value']['product_id']) && $content['field_product'][0]['error_data']['#value']['product_id'] == $product['product_id']) :
                    $product_render[0]['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#value'] = $content['field_product'][0]['error_data']['#value']['field_phone_number'];
                    $product_render[0]['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'error';
                  endif;
                  $product_render[0]['line_item_fields']['field_phone_number'][LANGUAGE_NONE][0]['value']['#attributes']['class'][] = 'bundling-' . $product['product_id'];
              ?>
              <div class="w3-col l3 m3 s12">
                <div class="product-bundling-area">
                  <?php if (isset($data->field_bundling_banner) && empty($data->field_bundling_banner)) :?>
                  <div class="product-bundling-title">
                    <h1><?php print $content['field_product'][0]['product_id']['#options'][$product['product_id']]; ?></h1>
                    <p><?php print commerce_currency_format($data->commerce_price[LANGUAGE_NONE][0]['amount'], $data->commerce_price[LANGUAGE_NONE][0]['currency_code'], $data); ?></p>
                  </div>
                  <?php else: ?>
                  <div class="product-bundling-title">
                    <?php print render($bundling_banner); ?>
                  </div>
                  <?php endif; ?>
                  <div class="product-bundling-box">
                    <?php if (isset($description[0]) && !empty($description[0])) : ?>
                    <p><?php print render($description[0]); ?></p>
                    <?php endif; ?>
                    <?php print render($product_render); ?>
                  </div>
                </div>
              </div>
              <?php endif; ?>
              <?php endforeach; ?>
            </div>

            <?php if (!empty($content['field_coupon_tnc'])) : ?>
            <div class="w3-row row wrapper">
              <div class="product-bundling-tnc">
                <div class="product-bundling-box">
                  <div class="w3-col l3 m3 s12">&nbsp;</div>
                  <div class="w3-col l6 m6 s12">
                    <?php print render($content['field_coupon_tnc']); ?>
                  </div>
                  <div class="w3-col l3 m3 s12">&nbsp;</div>
                  <div style="clear:both;">&nbsp;</div>
                </div>
              </div>
            </div>
            <?php endif; ?>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>
