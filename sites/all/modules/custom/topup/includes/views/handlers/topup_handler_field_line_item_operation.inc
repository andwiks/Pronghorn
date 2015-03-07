<?php
/**
 * @file
 * topup_handler_field_line_item_operation.inc
 *
 * @desc
 * Field handler to present a button todo line item topup operation.
 */

/**
 * Field handler to present a button todo line item topup operation.
 */
class topup_handler_field_line_item_operation extends views_handler_field {

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

    // Set form container for all of this button.
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    // At this point, the query has already been run, so we can access the
    // results in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_id => $row) {
      // Get line item ID.
      $line_item_id = $this->get_value($row, 'line_item_id');
      // Create tree for this operation.
      $form[$this->options['id']][$row_id] = array(
        '#tree' => TRUE,
      );
      // Select operations.
      $form[$this->options['id']][$row_id]['operation'] = array(
        '#type' => 'select',
        '#options' => array(
          0 => t('- Operation -'),
          'cancel' => t('Cancel'),
          'query' => t('Query'),
          'reset' => t('Reset'),
          'requeue_query' => t('Requeue: Query'),
          'requeue_process' => t('Requeue: Process'),
          'update_failed' => t('Update: Failed'),
          'update_success' => t('Update: Success'),
        ),
        '#title' => t('Topup Operation'),
        '#title_display' => 'invisible',
        '#default_value' => 0,
      );
      $form[$this->options['id']][$row_id]['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Go'),
        '#name' => 'topup-line-item-' . $row_id,
        '#line_item_id' => $line_item_id,
      );
    }
  }

  /**
   * Form Validate.
   */
  function views_form_validate($form, &$form_state) {
    // Looping for each row inside form container.
    foreach (element_children($form[$this->options['id']]) as $row_id) {
      // Check for form state order: prevent form submit error.
      if (!isset($form_state['order'])) {
        $line_item_id = $form[$this->options['id']][$row_id]['submit']['#line_item_id'];
        // Add form state order with empty object.
        $form_state['order'] = new stdClass();
        // Get views argument.
        $view = $form_state['build_info']['args'][0];
        // Get line item wrapper.
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item_id);
        // Define form state order id.
        $form_state['order']->order_id = (isset($view->argument['order_id'])) ? $view->argument['order_id'] : $line_item_wrapper->order_id->value();
      }
      // Check element which trigger submission.
      if ($form_state['triggering_element']['#name'] == 'topup-line-item-' . $row_id) {
        // Check for no operation.
        if (empty($form_state['values']['topup_operation'][$row_id]['operation'])) {
          form_set_error($this->options['id'] . '][' . $row_id, t('No topup operation selected.'));
        }
        return;
      }
    }
  }

  /**
   * Form Submit.
   */
  function views_form_submit($form, &$form_state) {
    // Looping for each row inside form container.
    foreach (element_children($form[$this->options['id']]) as $row_id) {
      // Check which button trigger the submit.
      if ($form_state['triggering_element']['#name'] == 'topup-line-item-' . $row_id) {
        // Get line item id.
        $line_item_id = $form[$this->options['id']][$row_id]['submit']['#line_item_id'];
        // Get topup operation.
        $topup_operation = $form_state['values']['topup_operation'][$row_id]['operation'];
        // Do something based on topup operation.
        switch ($topup_operation) {
          case 'requeue_query':
          case 'requeue_process':
            // Define requeue operation.
            $requeue_operation = str_replace('requeue_', '', $topup_operation);
            // Call topup requeue.
            topup_requeue($line_item_id, $requeue_operation);
            // Load line item wrapper.
            $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item_id);
            // Get topup field code.
            $field_topup_code = variable_get('topup_field_code', 'field_package_code');
            // Get topup code.
            $topup_code = $line_item_wrapper->commerce_product->$field_topup_code->value();
            // Show message to user.
            drupal_set_message(t('Line item (@id)  %phone [@code] has been requeued for %op.', array(
              '@id' => $line_item_id,
              '%phone' => $line_item_wrapper->field_phone_number->value(),
              '@code' => (!empty($topup_code)) ? $topup_code : '-',
              '%op' => $requeue_operation,
            )));
            break;

          case 'update_failed':
            // Update topup status.
            topup_update_status($line_item_id, TOPUP_FAILED);
            // Show message to user.
            drupal_set_message(t('Line item (@id) status has been updated to @status.', array(
              '@id' => $line_item_id,
              '@status' => str_replace('update_', '', $topup_operation),
            )));
            break;

          case 'update_success':
            // Update topup status.
            topup_update_status($line_item_id, TOPUP_SUCCESS);
            // Show message to user.
            drupal_set_message(t('Line item (@id) status has been updated to @status.', array(
              '@id' => $line_item_id,
              '@status' => str_replace('update_', '', $topup_operation),
            )));
            break;

          case 'query':
            // Call to topup query.
            $query = topup_query($line_item_id);
            // Show message to user.
            drupal_set_message(t('Line item (@id)  %op result: <pre>!result</pre>', array(
              '@id' => $line_item_id,
              '%op' => 'query',
              '!result' => print_r($query, TRUE),
            )));
            break;

          case 'reset':
            // Update topup status.
            topup_update_status($line_item_id, TOPUP_NONE);
            // Delete all topup data.
            topup_delete_topup_data($line_item_id);
            // Show message to user.
            drupal_set_message(t('Line item (@id) topup has been reset.', array(
              '@id' => $line_item_id,
            )));
            break;

          case 'cancel':
            // Update topup status.
            topup_update_status($line_item_id, TOPUP_CANCELED);
            // Get topup cancel list.
            $cancel_list = variable_get('topup_cancel_list', array());
            // Get topup quantity.
            $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item_id);
            $quantity = intval($line_item_wrapper->quantity->value());
            for ($i = 0; $i < $quantity; $i++) {
              $cancel_list[$line_item_id][$i] = TRUE;
            }
            // Update topup cancel list.
            variable_set('topup_cancel_list', $cancel_list);
            // Show message to user.
            drupal_set_message(t('Line item (@id) topup has been canceled. Queue process also canceled unless it is running.', array(
              '@id' => $line_item_id,
            )));
            break;

          default:
            drupal_set_message(t('Topup operation %op is not supported.', array(
              '%op' => $form_state['values']['topup_operation'][$row_id]['operation'],
            )), 'error');
            break;
        }
        return;
      }
    }
  }
}
