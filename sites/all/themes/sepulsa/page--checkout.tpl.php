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
          <?php print render($page['content']); ?>
        </div>
        
      </div>
    </div>
  </section>
        
  <?php include "footer.tpl.php"; ?>
    
</div>