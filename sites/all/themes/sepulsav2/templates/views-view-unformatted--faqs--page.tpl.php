<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */

// Expand first row.
if($classes_array[0]){
  $class_active='active';
}
?>
<div class="row accordion <?php print $class_active ?> ">
  <?php if (!empty($title)): ?>
    <div class="label"><?php print $title; ?> <span class="ico"></span></div>
  <?php endif; ?>
  <div class="content">
  <?php foreach ($rows as $id => $row): ?>
      <?php print $row; ?>
  <?php endforeach; ?>
  </div>
</div>
