<?php
/**
 * @file
 * page--checkout.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since February 8th 2015.
 */

global $base_url;

?>
<div id="page-wrapper">
  <?php include "header.tpl.php"; ?>
        
  <section id="content">
    <div class="container">
      <div id="main">    
        <div class="woocommerce">

          <?php print render($title_suffix); ?>

          <?php print $messages; ?>
          
          <div class="row">
            <div class="col-md-7">
              <h4><strong>Detail Transaksi</strong></h4>
              <?php print render($page['content']['system_main']['cart_contents'] ); ?>
            </div>
            <div class="col-md-5">
              <?php print render($page['content']['system_main']['account'] ); ?>
              <?php print render($page['content']['system_main']['commerce_coupon'] ); ?>
              <?php print render($page['content']['system_main']['commerce_payment'] ); ?>           
              <?php print render($page['content']['system_main']['buttons'] ); ?>
            </div>
          </div>
        </div>        
      </div>
    </div>
  </section>
        
  <?php include "footer.tpl.php"; ?>
    
</div>