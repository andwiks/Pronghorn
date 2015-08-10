<section class="transaction">
    <div class="wrapper_2 after_clear">
		<div class="col-md-7 c_left">
		  <h4><strong>Detail Transaksi</strong></h4>
		  <?php print render($form['cart_contents']); ?>

		  <?php print render($form['commerce_coupon']); ?>
		</div>

		<div class="col-md-5 c_right">
		  <?php print drupal_render_children($form); ?>
		</div>
	</div>
</section>