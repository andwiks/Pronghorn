<?php
/**
 * @file
 * Template file for theming a given documentation version.
 *
 * A given documentation version contains the following nested elements:
 * - Resources, defined by Services. E.g., user, node, etc.
 * - Method bundles. E.g., operations, actions, targeted actions.
 * - Methods. E.g., create, update, index, etc.
 *
 * Available custom variables:
 * - $resources: An array of resources for this documentation version.
 * - $description
 * - $table_of_contents
 */
?>
<!-- services-documentation-version -->
<div class="services-documentation-version row">
  <?php if ($description): ?>
    <div class="services-version-description col-sm-12">
      <?php print render($description); ?>
    </div>
  <?php endif; ?>

  <?php if ($table_of_contents): ?>
    <div class="services-documentation-toc col-sm-4 affix-top" data-spy="affix" data-offset-top="200" data-offset-bottom="150">
      <h2 class="toc-title">Resources</h2>
      <?php print render($table_of_contents); ?>
    </div>
  <?php endif; ?>

  <?php if ($resources): ?>
    <div class="services-documentation-resources col-sm-8 pull-right">
      <?php print render($resources); ?>
    </div>
  <?php endif; ?>
</div>
<!-- /services-documentation-version -->
