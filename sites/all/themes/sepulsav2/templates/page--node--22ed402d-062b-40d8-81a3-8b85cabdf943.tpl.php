<?php
/**
 * @file
 * Template page.tpl.php.
 *
 * @author azul@sepulsa.com
 *
 * @since June 9th 2016.
 */
?>


<section class="std_content">

  <div class="wrapper_2 other after_clear uob">
    <?php print $messages; ?>
  </div>

  <div class="wrapper_2 other after_clear uob-campaign">
    <?php if (!empty($page['sidebar_first'])): ?>
      <div class="w3-col l6 m6 s12">
        <div class="uob-snk">
          <?php print render($page['sidebar_first']); ?>
        </div>
      </div>
    <?php endif; ?>

    <div class="w3-col l6 m6 s12">
      <div class="uob-form">
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
        <?php endif; ?>
        <?php print render($page['content']); ?>
      </div>
    </div>
  </div>
</section>
