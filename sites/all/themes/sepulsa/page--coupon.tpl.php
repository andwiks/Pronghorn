<?php
/**
 * @file
 * page.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;

?>
<div id="page-wrapper">
  <?php include "header.tpl.php"; ?>
        
  <section id="content">
    <div class="container">
      <div id="main">
        
        
        <div class="row">
          <div class="col-sm-9">

            <?php print render($title_suffix); ?>
            <?php print $messages; ?>
            <?php print render($page['content']); ?>
            
          </div>
          
            
          <div class="col-sm-6 col-sm-3">
            <?php print render($page['right']); ?>
          </div>
          
        
        </div>
      </div>
    </div>
  </section>
        
  <?php include "footer.tpl.php"; ?>
    
</div>