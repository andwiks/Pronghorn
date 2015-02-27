<?php
/**
 * @file
 * topup_handler_field_line_item_requeue.inc
 *
 * @desc
 * Field handler to present a button to requeue line item topup.
 */

/**
 * Field handler to present a button to requeue line item topup.
 */
class topup_handler_field_line_item_requeue extends views_handler_field {

  /**
   * (non-PHPdoc)
   * @see views_handler_field::construct()
   */
  function construct() {
    parent::construct();
    $this->additional_fields['line_item_id'] = 'line_item_id';
  }

  /**
   * (non-PHPdoc)
   * @see views_handler_field::render()
   */
  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Returns the form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }
    // Set form container for all of this button: topup_requeue.
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    // At this point, the query has already been run, so we can access the
    // results in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_id => $row) {
      $form[$this->options['id']][$row_id] = array(
        '#type' => 'submit',
        '#value' => t('Requeue'),
        '#name' => 'requeue-line-item-' . $row_id,
        '#line_item_id' => $row->line_item_id,
      );
    }
  }


  // @todo: multiple submit button will get fucked up.
  function views_form_submit($form, &$form_state) {
    // Looping for each row inside form container.
    foreach (element_children($form[$this->options['id']]) as $row_id) {
      // Check for the removal of an item.
      if ($form_state['triggering_element']['#name'] == 'requeue-line-item-' . $row_id) {
        $line_item_id = $form[$this->options['id']][$row_id]['#line_item_id'];
        // Call topup requeue.
        topup_requeue($line_item_id);
        // Load line item wrapper.
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item_id);
        // Get topup field code.
        $field_topup_code = variable_get('topup_field_code', 'field_package_code');
        // Get topup code.
        $topup_code = $line_item_wrapper->commerce_product->$field_topup_code->value();
        // Show message to user.
        drupal_set_message(t('Line item (@id)  %phone [@code] has been requeued.', array(
          '@id' => $line_item_id,
          '%phone' => $line_item_wrapper->field_phone_number->value(),
          '@code' => (!empty($topup_code)) ? $topup_code : '-',
        )));
      }
    }
  }
}
