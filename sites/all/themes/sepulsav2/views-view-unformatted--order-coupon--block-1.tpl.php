<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($rows as $id => $row): ?>
  <div class="w3-col s12 m4">
    <div class="shortcode-banner" style="margin: 10px 15px;">
      <?php print $row; ?>
    </div>
  </div>  
<?php endforeach; ?>
