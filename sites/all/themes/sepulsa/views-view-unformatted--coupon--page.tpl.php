<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($rows as $id => $row): ?>
  <div id="node-<?php print $view->result[$id]->nid; ?>" class="col-sm-6 col-md-3<?php print isset($row_classes[$id]) ? ' ' . $row_classes[$id] : ''; ?>">
    <article class="post post-grid">
      <?php print $row; ?>
    </article>
  </div>
<?php endforeach; ?>
