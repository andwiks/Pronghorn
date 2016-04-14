<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<div class="row accordion active">
  <?php if (!empty($title)): ?>
    <div class="label"><?php print $title; ?> <span class="ico"></span></div>
  <?php endif; ?>
  <div class="content">
  <?php foreach ($rows as $id => $row): ?>
    <p>
      <?php print $row; ?>
    </p>
  <?php endforeach; ?>
  </div>
</div>
