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
<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July 23th 2015.
 */

global $base_url;

?>
<div id="page-wrapper">

  <section id="content">
    <div class="container">
      <div id="main">
          <?php print render($tabs2); ?>
          <?php print $messages; ?>
          <?php print render($page['help']); ?>
          <div class="clearfix">
            <?php print render($page['content']); ?>
          </div>
      </div>
    </div>
  </section>


</div>
