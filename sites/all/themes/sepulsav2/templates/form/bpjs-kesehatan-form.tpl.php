<div class="form-bpjs">
  <div class="form-field">
    <?php print render($form['field_phone_number']); ?>
   </div>

  <?php foreach (element_children($form['line_items']) as $delta => $child): ?>
    <div class="form-field">
      <?php print render($form['line_items'][$child]); ?>

      <?php if ($delta == count(element_children($form['line_items'])) - 1): ?>
        <?php print render($form['actions']['new']); ?>
      <?php endif; ?>
    </div>
  <?php endforeach; ?>

  <div class="form-actions form-wrapper" id="edit-actions">
    <?php print render($form['actions']['charge']); ?>
    <?php print render($form['actions']['submit']); ?>
  </div>

  <?php print drupal_render_children($form); ?>
</div>
