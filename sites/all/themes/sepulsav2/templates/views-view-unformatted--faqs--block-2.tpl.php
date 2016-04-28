<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
  // kpr(get_defined_vars());
$rowclass = 'deactive';
if ($node = menu_get_object()) {
 if ($node->field_faq_category['und'][0]['taxonomy_term']->name == $title ) {
  $rowclass = 'active';
 }

}

  foreach ($rows as $id => $row){

  }

?>
<div class="row accordion <?php print $rowclass ?>">
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
