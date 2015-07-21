<?php

/**
 * @file
 * Template for invoiced orders.
 */

?>

<div class="invoice-invoiced">
  <img src="<?php print $content['invoice_logo']['#value']; ?>" height="77" />
  <hr/>

  <div class="invoice-header-date"><?php print render($content['invoice_header_date']); ?></div>
  <?php print render($content['order_number']); ?><br />
  <?php print render($content['order_id']); ?>
  
  <br /><br />

  <?php print render($content['commerce_line_items']); ?>
  <div class="order-total"><?php print render($content['commerce_order_total']); ?></div>
  
  <?php print render($content['invoice_text']); ?>

  <div class="invoice-footer"><?php print render($content['invoice_footer']); ?></div>
</div>
