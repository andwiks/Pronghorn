<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */

global $base_url;
global $user;

/* TO DO : move this to module */
$order = commerce_cart_order_load($user->uid);
$order_wrapper = entity_metadata_wrapper('commerce_order', $order);
$line_items = $order_wrapper->commerce_line_items;
$total = commerce_line_items_total($line_items);
?>
<div class="pricing-table style1">
  <?php if ($rows): ?>
    <div class="pricing-table-content" style="text-align:left; padding-left:20px">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <div class="pricing-table-header">
    <h4 class="pricing-type">TOTAL = IDR <?php print number_format($total['amount'], 0, ".", ","); ?></h4>
  </div>
  
  <div class="pricing-table-footer">
    <a href="<?php print $base_url."/checkout"; ?>" class="btn style4">Proses</a>
  </div>
  
</div>