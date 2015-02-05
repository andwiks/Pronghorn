<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($rows as $id => $row): ?>
  <div class="col-sm-6 col-md-3">
    <article class="post post-grid">
      <?php print $row; ?>
    </article>
  </div>
<?php endforeach; ?>