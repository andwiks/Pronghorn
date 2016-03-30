<?php kpr($variables); ?>
<div class="form-bpjs">
  <div class="form-field">
    <label>Nomor Telepon</label>
    <input type="text"></input>
   </div>

  <div class="form-field">
    <label><?php print t('Customer Number'); ?></label>
  </div>

  <?php foreach (element_children($form['line_items']) as $child): ?>
    <div class="form-field">
      <?php print render($form['line_items'][$child]); ?>
    </div>
  <?php endforeach; ?>

  <?php print drupal_render_children($form); ?>
</div>
