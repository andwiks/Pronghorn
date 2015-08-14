<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July 23th 2015.
 */
?>

<section class="std_content">
  <div class="wrapper_2 akun after_clear">
    <?php if (!empty($page['sidebar_first'])): ?>
      <div class="c_left">
        <?php print render($page['sidebar_first']); ?>
      </div>
    <?php endif; ?>

    <div class="c_right">
      <?php if ($title): ?>
        <h2><?php print decode_entities($title); ?></h2>
      <?php endif; ?>

      <?php print $messages; ?>
      <?php $hybridauth = $page['content']['system_main']['hybridauth']; ?>
      <?php unset($page['content']['system_main']['hybridauth']); ?>
      <?php print render($page['content']); ?>
      <?php print render($hybridauth); ?>
    </div>
  </div>
</section>
