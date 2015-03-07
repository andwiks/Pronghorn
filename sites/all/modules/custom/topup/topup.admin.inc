<?php
/**
 * @file
 * topup.admin.inc
 *
 * @todo: enable disable code.
 *
 * @author andre4s_y
 */

/**
 * Callback Functions.
 */

/**
 * Function topup_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function topup_settings_form($form = array(), $form_state = array()) {
  // Define line item type options.
  $options = array();
  // Get all line item type.
  $line_item_types = commerce_line_item_types();
  // Looping for each line item type.
  foreach ($line_item_types as $line_item_key => $line_item_info) {
    // Only for line item product.
    if (isset($line_item_info['product']) && !empty($line_item_info['product'])) {
      $options[$line_item_key] = $line_item_info['name'];
    }
  }

  // Get line item type default value.
  $line_item_type = variable_get('topup_lineitem_type', 'mobile_prepaid');

  // Fieldset line item.
  $form['line_item'] = array(
    '#type' => 'fieldset',
    '#title' => t('Line item'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Build line item type select.
  $form['line_item']['topup_lineitem_type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => $options,
    '#default_value' => $line_item_type,
    '#description' => t('Choose line item type which provide top up information.'),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'topup_line_item_fields',
      'wrapper' => 'topup_line_item_fields',
    ),
  );

  // Build line item field list: based on line item type above.
  $form['line_item']['topup_field_phone_number'] = array(
    '#type' => 'select',
    '#title' => t('Phone number field'),
    '#options' => _topup_get_lineitem_fields($line_item_type),
    '#default_value' => variable_get('topup_field_phone_number', 'field_phone_number'),
    '#description' => t('Choose line item field which provide top up phone number.'),
    '#required' => TRUE,
    '#prefix' => '<div id="topup_line_item_fields">',
    '#suffix' => '</div>',
  );

  // Define product fields options.
  $options = array();
  // Get product field info.
  $product = field_info_instances('commerce_product');
  // Looping for each product fields.
  foreach ($product as $bundle => $field_info) {
    // Looping foreach field.
    foreach ($field_info as $field_info_key => $field_info_data) {
      $option[$field_info_key] = $field_info_data['label'];
    }
    $options[$bundle] = $option;
  }

  // Product fieldset.
  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Build product field list: topup provider.
  $form['product']['topup_field_provider'] = array(
    '#type' => 'select',
    '#title' => t('Provider field'),
    '#options' => $options,
    '#default_value' => variable_get('topup_field_provider', 'field_api_provider'),
    '#description' => t('Choose product field which provide top up provider.'),
    '#required' => TRUE,
  );
  // Build product field list: topup code.
  $form['product']['topup_field_code'] = array(
    '#type' => 'select',
    '#title' => t('Code field'),
    '#options' => $options,
    '#default_value' => variable_get('topup_field_code', 'field_package_code'),
    '#description' => t('Choose product field which provide top up code.'),
    '#required' => TRUE,
  );

  // Queue try number.
  $form['topup_try'] = array(
    '#type' => 'textfield',
    '#title' => t('Try'),
    '#description' => t('Maximum number system try to queue topup request.'),
    '#default_value' => variable_get('topup_try', 3),
    '#size' => 2,
    '#maxlength' => 2,
    '#required' => TRUE,
  );

  // Add validate callback.
  $form['#validate'][] = 'topup_settings_form_validate';
  // Add submit callback.
  $form['#submit'][] = 'topup_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Function topup_settings_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function topup_settings_form_validate($form, &$form_state) {
  // Get all properties of commerce_product entity.
  $properties = entity_get_all_property_info('commerce_product');
  // Validate topup_field_provider: must available.
  if (!isset($properties[$form_state['values']['topup_field_provider']])) {
    form_set_error('topup_field_provider', t('You must create field %field at commerce product first.', array(
      '%field' => $form_state['values']['topup_field_provider'],
    )));
  }
  // Validate topup_field_code: must available.
  if (!isset($properties[$form_state['values']['topup_field_code']])) {
    form_set_error('topup_field_code', t('You must create field %field at commerce product first.', array(
      '%field' => $form_state['values']['topup_field_code'],
    )));
  }
  else {
    // Validate for update field allowed value settings.
    // Get field old info.
    $info = field_info_field($form_state['values']['topup_field_code']);
    // Compare old allowed value with new allowed value.
    $lost_keys = array_diff(array_keys($info['settings']['allowed_values']), array_keys(_topup_get_code_options()));
    if (!empty($lost_keys)) {
      form_set_error('topup_field_code', t('Unable to update field %field because values %values currently in use.', array(
        '%field' => $form_state['values']['topup_field_code'],
        '%values' => implode(', ', $lost_keys),
      )));
    }
  }
}

/**
 * Function topup_settings_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function topup_settings_form_submit($form, &$form_state) {
  // Sanitize topup try.
  $form_state['values']['topup_try'] = intval($form_state['values']['topup_try']);

  // Get all properties of commerce_line_item entity.
  $properties = entity_get_all_property_info('commerce_line_item');
  // Check field_topup_status.
  if (!isset($properties['field_topup_status'])) {
    // Create field: field_topup_status.
    $field = array(
      'field_name' => 'field_topup_status',
      'type' => 'list_text',
      'settings' => array(
        'allowed_values' => array(
          TOPUP_SUCCESS => topup_translate_status(TOPUP_SUCCESS),
          TOPUP_PENDING => topup_translate_status(TOPUP_PENDING),
          TOPUP_FAILED => topup_translate_status(TOPUP_FAILED),
          TOPUP_INQUEUE => topup_translate_status(TOPUP_INQUEUE),
          TOPUP_CANCELED => topup_translate_status(TOPUP_CANCELED),
          TOPUP_ERROR => topup_translate_status(TOPUP_ERROR),
          TOPUP_NONE => topup_translate_status(TOPUP_NONE),
        ),
      ),
    );
    field_create_field($field);
    // Create an instance and attach it to the commerce product entity.
    $instance = array(
      'field_name' => 'field_topup_status',
      'entity_type' => 'commerce_line_item',
      'bundle' => $form_state['values']['topup_lineitem_type'],
      'label' => 'Topup Status',
      'required' => FALSE,
      'default_value' => array(
        array(
          'value' => TOPUP_NONE,
        ),
      ),
    );
    field_create_instance($instance);
  }
  // Check field_topup_data.
  if (!isset($properties['field_topup_data'])) {
    // Create field: field_topup_data.
    $field = array(
      'field_name' => 'field_topup_data',
      'type' => 'text',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    );
    field_create_field($field);
    // Create an instance and attach it to the commerce product entity.
    $instance = array(
      'field_name' => 'field_topup_data',
      'entity_type' => 'commerce_line_item',
      'bundle' => $form_state['values']['topup_lineitem_type'],
      'label' => 'Topup Data',
      'required' => FALSE,
      'default_value' => array(
        array(
          'value' => '',
        ),
      ),
      'display' => array(
        'default' => array(
          'label' => 'Topup Data Formatter',
          'type' => 'topup_data_formatter',
        ),
      ),
    );
    field_create_instance($instance);
  }

  // Get all properties of commerce_product entity.
  $properties = entity_get_all_property_info('commerce_product');
  // Get all topup providers.
  $providers = topup_providers();

  // Check field topup_field_provider.
  if (isset($properties[$form_state['values']['topup_field_provider']])) {
    // Get field info.
    $info = field_info_field($form_state['values']['topup_field_provider']);
    // Update field allowed value settings.
    $allowed_values = array();
    foreach ($providers as $provider_id => $provider_data) {
      $allowed_values[$provider_id] = $provider_data['title'] . ' (' . $provider_id . ')';
    }
    $info['settings']['allowed_values'] = $allowed_values;
    // Save the field.
    field_update_field($info);
  }

  // Check field topup_field_code.
  if (isset($properties[$form_state['values']['topup_field_code']])) {
    // Get field info.
    $info = field_info_field($form_state['values']['topup_field_code']);
    // Update field allowed value settings.
    $info['settings']['allowed_values'] = _topup_get_code_options();
    // Save the field.
    field_update_field($info);
    // Update field instance.
    $instance = array(
      'field_name' => $form_state['values']['topup_field_code'],
      'description' => t('Pattern: provider id [package code] code title (price).'),
      'entity_type' => 'commerce_product',
      'bundle' => $info['bundles']['commerce_product'][0],
      'label' => t('Package Code'),
    );
    field_update_instance($instance);
  }
}

/**
 * Function topup_line_item_fields().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array for topup field number.
 */
function topup_line_item_fields($form, &$form_state) {
  // Get input line item type.
  $type = (isset($form_state['values']['topup_lineitem_type']) && !empty($form_state['values']['topup_lineitem_type'])) ? $form_state['values']['topup_lineitem_type'] : 'product';

  // Replace field options.
  $form['topup_field_phone_number']['#options'] = _topup_get_lineitem_fields($type);

  // Return topup field phone number form.
  return $form['topup_field_phone_number'];
}

/**
 * Function topup_provider_page().
 */
function topup_provider_page() {
  // Define default content.
  $content = array();
  // Get all topup providers.
  $providers = topup_providers();

  // Show providers table.
  // Operations: enable/disable, configure.
  if (!empty($providers)) {
    foreach ($providers as $id => $provider) {
      $provider_link = 'admin/commerce/config/topup/provider/' . $id;
       // Generate operations link.
      $links = array();
      // Check provider status.
      if (!empty($provider['status'])) {
        $links[] = array(
          'title' => t('Disable'),
          'href' => $provider_link . '/disable',
        );
      }
      else {
        $links[] = array(
          'title' => t('Enable'),
          'href' => $provider_link . '/enable',
        );
      }

      // Check providers by module base.
      $configurable_providers = variable_get('topup_providers_' . $provider['base'], array());
      if (!empty($configurable_providers) && isset($configurable_providers[$id])) {
        $links[] = array(
          'title' => t('Delete'),
          'href' => $provider_link . '/delete',
        );
      }


      // Add provider configure link.
      $links[] = array(
        'title' => t('Configure'),
        'href' => $provider_link,
      );
      // Check ctools module.
      if (module_exists('ctools')) {
        // Include ctools dropdown.theme.
        ctools_include('dropbutton.theme');
        // Generate ctools dropdown.
        $operations = theme_links__ctools_dropbutton(array(
          'class' => FALSE,
          'image' => FALSE,
          'title' => FALSE,
          'attributes' => array(),
          'links' => $links,
        ));
      }
      else {
        $operations = theme_links(array(
          'links' => $links,
          'attributes' => array(),
          'heading' => array(),
        ));
      }
      // Create tabular rows.
      $rows[] = array(
        'data' => array(
          'title' => $provider['title'] . '<br />(' . $id . '|' . $provider['base'] . ')',
          'description' => $provider['description'],
          'endpoint' => (isset($provider['settings']['endpoint'])) ? $provider['settings']['endpoint'] : '-',
          'operations' => $operations,
        ),
      );
    }
    // Put our providers into a themed table. See theme_table() for details.
    $content['providers_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(t('Title'), t('Description'), t('Endpoint'), t('Operations')),
    );
  }
  else {
    // There were no provider. Tell the user.
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No top up provider currently available.'),
    );
  }
  return $content;
}

/**
 * Function topup_provider_add_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function topup_provider_add_form($form = array(), $form_state = array()) {
  // Machine name: provider id.
  $form['provider_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine Provider ID'),
    '#required' => TRUE,
    '#default_value' => '',
    '#description' => t('The provider id can only consist of lowercase letters, underscores, and numbers.'),
    '#attributes' => array(
      'placeholder' => t('provider_id'),
    ),
  );
  // Provider Title.
  $form['provider_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Provider Title'),
    '#required' => TRUE,
    '#default_value' => '',
    '#description' => t('The provider title can only consist of letters, numbers, and space.'),
    '#attributes' => array(
      'placeholder' => t('provider title'),
    ),
  );
  // Description.
  $form['provider_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => TRUE,
    '#default_value' => '',
    '#attributes' => array(
      'placeholder' => t('description'),
    ),
  );
  // Get module base.
  $options = array();
  foreach (module_implements('topup_provider_info') as $module) {
    $info = system_get_info('module', $module);
    $options[$module] = $info['name'];
  }
  // Base module.
  $form['module'] = array(
    '#type' => 'select',
    '#title' => t('Provider module base'),
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('Choose base provider base module.'),
  );

  // Submit button: create.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  return $form;
}

/**
 * Function topup_provider_add_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function topup_provider_add_form_validate($form, &$form_state) {
  // Validate machine name.
  if (isset($form_state['values']['provider_id']) && !preg_match('#^[a-z\_0-9]+$#', $form_state['values']['provider_id'])) {
    form_set_error('provider_id', t('Provider ID can only consist of lowercase letters, underscores, and numbers.'));
  }
  else {
    // Machine name must be unique for each module base integration.
    $module_providers = variable_get('topup_providers_' . $form_state['values']['module'], array());
    if (in_array($form_state['values']['provider_id'], $module_providers)) {
      form_set_error('provider_id', t('Provider ID has been defined before.'));
    }
  }

  // Validate provider title.
  if (isset($form_state['values']['provider_title']) && !preg_match('#^[a-z\s0-9]+$#i', $form_state['values']['provider_title'])) {
    form_set_error('provider_id', t('Provider ID can only consist of lowercase letters, underscores, and numbers.'));
  }
}

/**
 * Function topup_provider_add_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function topup_provider_add_form_submit($form, &$form_state) {
  // Get current module providers.
  $module_providers = variable_get('topup_providers_' . $form_state['values']['module'], array());
  // Save provider configuration.
  $module_providers[$form_state['values']['provider_id']] = array(
    'title' => $form_state['values']['provider_title'],
    'description' => $form_state['values']['provider_desc'],
  );
  // Set variables.
  variable_set('topup_providers_' . $form_state['values']['module'], $module_providers);
  variable_set('topup_providers_title_' . $form_state['values']['provider_id'], $form_state['values']['provider_title']);
  variable_set('topup_providers_desc_' . $form_state['values']['provider_id'], $form_state['values']['provider_desc']);
  variable_set('topup_providers_status_' . $form_state['values']['provider_id'], FALSE);
  variable_set('topup_providers_settings_' . $form_state['values']['provider_id'], array());

  // Show success message.
  drupal_set_message(t('A new provider has been setup successfully.'));
  // Clear all caches.
  drupal_flush_all_caches();
  // Redirect to new provider page.
  drupal_goto('admin/commerce/config/topup/provider/' . $form_state['values']['provider_id']);
}

/**
 * Function topup_provider_status_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function topup_provider_status_form($form = array(), $form_state = array()) {
  $provider = arg(5);
  $operation = arg(6);

  // Get all topup providers.
  $providers = topup_providers();

  if (isset($providers[$provider])
    && in_array($operation, array('enable', 'disable', 'delete'))
  ) {

    if ($operation == 'delete') {
      $configurable_providers = variable_get('topup_providers_' . $providers[$provider]['base'], array());
      if (empty($configurable_providers) || !isset($configurable_providers[$provider])) {
        // Provider is not available.
        drupal_set_message(t('Topup provider %provider can be be deleted because this is module implementations.', array(
          '%provider' => $provider,
        )), 'error');
        // Redirect to provider list.
        drupal_goto('admin/commerce/config/topup/provider');
      }
    }

    $form['info'] = array(
      '#markup' => t('Are you sure to @operation provider %provider ?<br />', array(
        '@operation' => $operation,
        '%provider' => $providers[$provider]['title'],
      )),
    );
    $form['operation'] = array(
      '#type' => 'value',
      '#value' => $operation,
    );
    $form['provider'] = array(
      '#type' => 'value',
      '#value' => $provider,
    );
    $form['provider_title'] = array(
      '#type' => 'value',
      '#value' => $providers[$provider]['title'],
    );
    $form['provider_base'] = array(
      '#type' => 'value',
      '#value' => $providers[$provider]['base'],
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Yes'),
    );

    $form['cancel'] = array(
      '#markup' => l(t('cancel'), 'admin/commerce/config/topup/provider'),
    );

    return $form;
  }
  else {
    // Provider is not available.
    drupal_set_message(t('Topup provider %provider is not available or not configurable.', array(
      '%provider' => $provider,
    )), 'error');
    // Redirect to provider list.
    drupal_goto('admin/commerce/config/topup/provider');
  }
}

/**
 * Function topup_provider_status_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function topup_provider_status_form_submit($form, &$form_state) {
  // Get operation.
  switch ($form_state['values']['operation']) {
    case 'enable':
      variable_set('topup_providers_status_' . $form_state['values']['provider'], TRUE);
      break;

    case 'disable':
      variable_set('topup_providers_status_' . $form_state['values']['provider'], FALSE);
      break;

    case 'delete':
      $module_providers = variable_get('topup_providers_' . $form_state['values']['provider_base'], array());
      unset($module_providers[$form_state['values']['provider']]);
      variable_set('topup_providers_' . $form_state['values']['provider_base'], $module_providers);
      variable_del('topup_providers_title_' . $form_state['values']['provider']);
      variable_del('topup_providers_desc_' . $form_state['values']['provider']);
      variable_del('topup_providers_status_' . $form_state['values']['provider']);
      variable_del('topup_providers_settings_' . $form_state['values']['provider']);
      break;
  }
  drupal_set_message(t('Provider %provider has been @operatord.', array(
    '%provider' => $form_state['values']['provider_title'],
    '@operator' => $form_state['values']['operation'],
  )));
  // Redirect to provider list page.
  $form_state['redirect'] = 'admin/commerce/config/topup/provider';
}

/**
 * Function topup_provider_settings_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form array.
 */
function topup_provider_settings_form($form = array(), $form_state = array()) {
  // Get provider based on url argument.
  $provider = arg(5);
  // Get all topup providers.
  $providers = topup_providers();
  // Check whether provider is available.
  if (isset($providers[$provider])) {
    $form['provider_id'] = array(
      '#type' => 'item',
      '#title' => 'Provider ID',
      '#title_display' => 'before',
      '#markup' => $provider,
    );
    // Provider Title.
    $form['provider_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Provider Title'),
      '#required' => TRUE,
      '#default_value' => $providers[$provider]['title'],
      '#description' => t('The provider title can only consist of letters, numbers, and space.'),
      '#attributes' => array(
        'placeholder' => t('provider title'),
      ),
    );
    // Description.
    $form['provider_desc'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#required' => TRUE,
      '#default_value' => $providers[$provider]['description'],
      '#attributes' => array(
        'placeholder' => t('description'),
      ),
    );
    // Get setting form callback.
    $settings_form = $providers[$provider]['base'] . '_settings_form';
    // Check whether settings form is available.
    if (function_exists($settings_form) && is_callable($settings_form)) {
      // Get settings form.
      $form['settings'] = $settings_form($providers[$provider]['settings']);
      $form['settings']['#tree'] = TRUE;
      $form['settings']['#type'] = 'fieldset';
      $form['settings']['#title'] = t('Settings');
      $form['settings']['#collapsible'] = TRUE;
      $form['settings']['#collapse'] = FALSE;
    }
    global $base_url;
    // Callback URL.
    $form['callback_url'] = array(
      '#type' => 'item',
      '#title' => t('Callback URL'),
      '#markup' => $base_url . '/topup/' . $provider . '/callback',
    );

    // Add internal form value.
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $provider,
    );

    // Add submit button.
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    // Check provider status.
    if (!empty($providers[$provider]['status'])) {
      // Disable button.
      $form['status'] = array(
        '#type' => 'submit',
        '#value' => t('Disable'),
      );
    }
    else {
      // Enable button.
      $form['status'] = array(
        '#type' => 'submit',
        '#value' => t('Enable'),
      );
    }

    // Delete button.
    $configurable_providers = variable_get('topup_providers_' . $providers[$provider]['base'], array());
    if (!empty($configurable_providers) && isset($configurable_providers[$provider])) {
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
      );
    }

    // Add submit callback.
    $form['#submit'][] = 'topup_provider_settings_form_submit';
    // Return form.
    return $form;
  }
  // Provider is not available.
  drupal_set_message(t('Topup provider %provider is not available or not configurable.', array(
    '%provider' => $provider,
  )), 'error');
  // Redirect to provider list.
  drupal_goto('admin/commerce/config/topup/provider');
}

/**
 * Function topup_provider_settings_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function topup_provider_settings_form_submit($form, &$form_state) {
  // Check input settings.
  if (isset($form_state['values']['settings']) && !empty($form_state['values']['settings'])
    && isset($form_state['values']['id']) && !empty($form_state['values']['id'])
  ) {
    // Get provider based on url argument.
    $provider = arg(5);
    // Get all topup providers.
    $providers = topup_providers();
    // Update provider title if necessary.
    if ($form_state['values']['provider_title'] != $providers[$provider]['title']) {
      variable_set('topup_providers_title_' . $provider, $form_state['values']['provider_title']);
    }
    // Update provider description if necessary.
    if ($form_state['values']['provider_desc'] != $providers[$provider]['description']) {
      variable_set('topup_providers_desc_' . $provider, $form_state['values']['provider_desc']);
    }
    // Store it inside variables.
    variable_set('topup_providers_settings_' . $form_state['values']['id'], $form_state['values']['settings']);
  }
  // Get form operation.
  $op = $form_state['values']['op'];
  switch ($op) {
    case t('Save'):
      drupal_set_message('Top up provider settings have been saved.');
      $form_state['redirect'] = 'admin/commerce/config/topup/provider';
      break;

    case t('Enable'):
      $form_state['redirect'] = 'admin/commerce/config/topup/provider/' . $form_state['values']['id'] . '/enable';
      break;

    case t('Disable'):
      $form_state['redirect'] = 'admin/commerce/config/topup/provider/' . $form_state['values']['id'] . '/disable';
      break;

    case t('Delete'):
      $form_state['redirect'] = 'admin/commerce/config/topup/provider/' . $form_state['values']['id'] . '/delete';
      break;
  }
}

/**
 * Private Functions.
 */

/**
 * Function _topup_get_lineitem_fields().
 *
 * @param string $type
 *   Line item type/bundle name.
 *
 * @return array
 *   Array field info for form options.
 */
function _topup_get_lineitem_fields($type = 'product') {
  // Define default options.
  $options = array();
  // Get packet options.
  $fields = field_info_instances('commerce_line_item', $type);
  // Looping for each line item field.
  foreach ($fields as $field_key => $field_info) {
    $options[$field_key] = $field_info['label'];
  }
  return $options;
}

/**
 * Function _topup_get_code_options().
 *
 * @return array
 *   Codes array, suitable for allowed value at field setting.
 */
function _topup_get_code_options() {
  // Get all topup provider codes.
  $codes = topup_provider_get_all_codes();
  // Define default options.
  $options = array();
  // Looping for each codes.
  foreach ($codes as $provider_id => $code_data) {
    foreach ($code_data as $code => $code_info) {
      $options[$code] = $provider_id . ' [' . $code . '] ' . $code_info['title'] . ' (' . $code_info['price'] . ')';
    }
  }
  // Return options.
  return $options;
}
