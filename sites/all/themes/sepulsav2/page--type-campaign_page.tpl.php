<!--page--type-campaign_page.tpl.php -->
<?php
/**
 * @file
 * page--type-campaign_page.tpl.php
 *
 * @author roy@sepulsa.com
 * @since Nov 16 2015
 */
?>

<section class="std_content">
  <div class="wrapper_2 other after_clear">
    <?php print $messages; ?>
    <?php if ($title): ?>
      <h2><?php print $title ?></h2>
    <?php endif; ?>

    <?php print render($page['content']); ?>
  </div>
</section>
