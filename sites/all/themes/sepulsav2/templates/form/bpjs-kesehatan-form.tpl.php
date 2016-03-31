<div class="form-bpjs">
  <div class="form-field">
    <?php print render($form['field_phone_number']); ?>
   </div>

  <div class="form-field">
    <label><?php print t('Customer Number'); ?></label>
  </div>

  <?php foreach (element_children($form['line_items']) as $child): ?>
    <div class="form-field">
      <?php print render($form['line_items'][$child]); ?>
    </div>
  <?php endforeach; ?>

  <div class="form-actions form-wrapper" id="edit-actions">
    <?php print render($form['actions']['new']); ?>

    <div style="float:right">
      <?php print render($form['actions']['charge']); ?>
    </div>
    <div style="float:right">
      <?php print render($form['actions']['submit']); ?>
    </div>
  </div>

  <?php print drupal_render_children($form); ?>
</div>
