<div class="col-md-7">
  <?php print render($form['cart_contents']); ?>
  <?php print render($form['commerce_coupon']); ?>
</div>

<div class="col-md-5">
  <?php print drupal_render_children($form); ?>
</div>
