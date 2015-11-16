<!-- page.tpl.php -->
<?php
/**
 * @file
 * page.tpl.php
 *
 * @author roy@sepulsa.com
 * @since Nov 16 2015.
 * 
 */
?>

<section class="std_content">
  <div class="wrapper_2 other after_clear">
    <?php if (!empty($page['sidebar_first'])): ?>
      <div class="c_left">
        <?php print render($page['sidebar_first']); ?>
      </div>
    <?php endif; ?>

    <div class="c_right">
      <?php print $messages; ?>
      <?php if ($title): ?>
        <h2><?php print $title ?></h2>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </div>
  </div>
</section>