<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<div class="row accordion deactive">
  <?php if (!empty($title)): ?>
    <div class="label"><span class="ico">+</span> <?php print $title; ?></div>
  <?php endif; ?>
  <div class="content">
  <?php foreach ($rows as $id => $row): ?>
    <p>
      <?php print $row; ?>
    </p>
  <?php endforeach; ?>
  </div>
</div>
