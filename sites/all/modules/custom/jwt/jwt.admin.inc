<?php
/**
 * @file
 * jwt.admin.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function jwt_settings().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   System setting form.
 */
function jwt_settings($form = array(), &$form_state = array()) {
  // Algorithm selection.
  $form['jwt_alg'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Active Algorithm'),
    '#options' => array(
      'HS256' => t('HS256'),
      'HS384' => t('HS384'),
      'HS512' => t('HS512'),
      'RS256' => t('RS256'),
      'RS384' => t('RS384'),
      'RS512' => t('RS512'),
      'ES256' => t('ES256'),
      'ES384' => t('ES384'),
      'ES512' => t('ES512'),
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('jwt_alg', array('HS256')),
    '#description' => t('Choose active algorithm.'),
  );

  return system_settings_form($form);
}

/**
 * Function jwt_config_form().
 *
 * Form constructor for the configuration edit form.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $config
 *   (optional) Object representing the configuration, when editing an
 *   existing configuration.
 *
 * @return array
 *   Form array.
 */
function jwt_config_form($form, &$form_state, $config = NULL) {
  // Define default form array.
  $form = array();

  // Retrieve all active jwt algorithms.
  $algs = variable_get('jwt_alg', array('HS256'));
  // Get filtered algorithms.
  $algs = array_filter($algs);

  // Define label.
  $form['label'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($config->label) ? $config->label : '',
    '#description' => t('The human-readable name of the configuration.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($config->name) ? $config->name : '',
    '#maxlength' => 32,
    '#disabled' => isset($config->name),
    '#machine_name' => array(
      'exists' => 'jwt_config_load_by_name',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for the configuration. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($config->description) ? $config->description : '',
    '#description' => t('A short description of the configuration.'),
  );

  $form['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
  );
  $form['general_settings']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($config->enabled) ? $config->enabled : '',
    '#description' => t('If checked, this configuration will be available.'),
  );

  $form['general_settings']['alg'] = array(
    '#type' => 'select',
    '#title' => t('Algorithm'),
    '#description' => t('Select algorithm for this configuration.'),
    '#required' => TRUE,
    '#options' => $algs,
    '#default_value' => isset($config->alg) ? $config->alg : 'HS256',
  );

  // Define whether algoritma need secret or public/private key.
  $secret = isset($config->alg) && in_array($config->alg, JWT_HMAC);
  // Define hmac secret options.
  $secret_options = array();
  foreach (JWT_HMAC as $alg) {
    $secret_options[] = array('value' => $alg);
  }

  $form['secret'] = array(
    '#type' => 'fieldset',
    '#title' => t('Secret settings'),
    '#collapsible' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="alg"]' => $secret_options,
      ),
    ),
    '#tree' => TRUE,
  );
  $form['secret']['secret_method'] = array(
    '#type' => 'radios',
    '#title' => t('Method'),
    '#required' => TRUE,
    '#options' => array(
      'input' => t('Direct input.'),
      'file' => t('Using file.'),
    ),
    '#required' => TRUE,
    '#default_value' => (isset($config->method) && $secret) ? $config->method : 'input',
  );
  $form['secret']['input'] = array(
    '#type' => 'fieldset',
    '#title' => t('Direct Input'),
    '#states' => array(
      'visible' => array(
        ':input[name="secret[secret_method]"]' => array('value' => 'input'),
      ),
    ),
  );
  $form['secret']['input']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Direct Input'),
    '#title_display' => 'invisible',
    '#default_value' => (isset($config->public_key) && $secret && isset($config->method) && $config->method == 'input') ? $config->public_key : '',
  );
  $form['secret']['file'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Location'),
    '#states' => array(
      'visible' => array(
        ':input[name="secret[secret_method]"]' => array('value' => 'file'),
      ),
    ),
  );
  $form['secret']['file']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('File Location'),
    '#title_display' => 'invisible',
    '#default_value' => (isset($config->public_key) && $secret && isset($config->method) && $config->method == 'file') ? preg_replace('#^file\:\/\/#', '', $config->public_key) : '',
    '#field_prefix' => 'file://',
    '#description' => t('Use full path.'),
  );

  $form['key'] = array(
    '#type' => 'fieldset',
    '#title' => t('Public / Private key settings'),
    '#collapsible' => TRUE,
    '#states' => array(
      'invisible' => array(
        ':input[name="alg"]' => $secret_options,
      ),
    ),
    '#tree' => TRUE,
  );
  $form['key']['key_method'] = array(
    '#type' => 'radios',
    '#title' => t('Method'),
    '#required' => TRUE,
    '#options' => array(
      'input' => t('Direct input.'),
      'file' => t('Using file.'),
    ),
    '#required' => TRUE,
    '#default_value' => (isset($config->method) && !$secret) ? $config->method : 'input',
  );
  $form['key']['input'] = array(
    '#type' => 'fieldset',
    '#title' => t('Direct Input'),
    '#states' => array(
      'visible' => array(
        ':input[name="key[key_method]"]' => array('value' => 'input'),
      ),
    ),
  );
  $form['key']['input']['public'] = array(
    '#type' => 'textarea',
    '#title' => t('Public Key'),
    '#default_value' => (isset($config->public_key) && !$secret && isset($config->method) && $config->method == 'input') ? $config->public_key : '',
    '#rows' => (isset($config->public_key) && !$secret && isset($config->method) && $config->method == 'input') ? substr_count($config->public_key, "\n") + 1 : 5,
  );
  $form['key']['input']['private'] = array(
    '#type' => 'textarea',
    '#title' => t('Private Key'),
    '#default_value' => (isset($config->private_key) && !$secret && isset($config->method) && $config->method == 'input') ? $config->private_key : '',
    '#rows' => (isset($config->private_key) && !empty($config->private_key) && !$secret && isset($config->method) && $config->method == 'input') ? substr_count($config->private_key, "\n") + 1 : 5,
  );
  $form['key']['file'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Location'),
    '#states' => array(
      'visible' => array(
        ':input[name="key[key_method]"]' => array('value' => 'file'),
      ),
    ),
  );
  $form['key']['file']['public'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key'),
    '#default_value' => (isset($config->public_key) && !$secret && isset($config->method) && $config->method == 'file') ? preg_replace('#^file\:\/\/#', '', $config->public_key) : '',
    '#field_prefix' => 'file://',
    '#description' => t('Use full path.'),
  );
  $form['key']['file']['private'] = array(
    '#type' => 'textfield',
    '#title' => t('Private Key'),
    '#default_value' => (isset($config->private_key) && !empty($config->private_key) && !$secret && isset($config->method) && $config->method == 'file') ? preg_replace('#^file\:\/\/#', '', $config->private_key) : '',
    '#field_prefix' => 'file://',
    '#description' => t('Use full path.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('jwt_config_form_submit'),
  );

  // Check if configuration is loaded.
  if (isset($config->name)) {
    $form['config'] = array(
      '#type' => 'value',
      '#value' => $config,
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete configuration'),
      '#submit' => array('jwt_config_form_delete_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  $form['#validate'][] = 'jwt_config_form_validate';

  return $form;
}

/**
 * Function jwt_config_form_validate().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_config_form_validate($form, &$form_state) {
  // Check if algorithm is using HMAC.
  if (in_array($form_state['values']['alg'], JWT_HMAC)) {
    // Check the secret method.
    if (empty($form_state['values']['secret'][$form_state['values']['secret']['secret_method']]['secret'])) {
      form_set_error('secret][' . $form_state['values']['secret']['secret_method'] . '][secret', t('You need to fill the secret key.'));
    }
    // Check the secret file if using method file.
    if ($form_state['values']['secret']['secret_method'] == 'file'
      && !file_exists('file://' . $form_state['values']['secret'][$form_state['values']['secret']['secret_method']]['secret'])
    ) {
      form_set_error('secret][file][secret', t('The secret file can not be found by system.'));
    }
  }
  else {
    // RSA and ECDA algoritm in used.
    if (empty($form_state['values']['key'][$form_state['values']['key']['key_method']]['public'])) {
      form_set_error('key][' . $form_state['values']['key']['key_method'] . '][public', t('At least you need to fill the public key in order to use JWT verify.'));
    }
    // Check the file if using method file.
    if ($form_state['values']['key']['key_method'] == 'file') {
      // Check public key file.
      if (!file_exists('file://' . $form_state['values']['key']['file']['public'])) {
        form_set_error('key][file][public', t('The public key file can not be found by system.'));
      }
      // Check private key file.
      if (!empty($form_state['values']['key']['file']['private'])
        && !file_exists('file://' . $form_state['values']['key']['file']['private'])
      ) {
        form_set_error('key][file][private', t('The private key file can not be found by system.'));
      }
    }
    // @todo: Check public key validity.
  }
}

/**
 * Function jwt_config_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_config_form_submit($form, &$form_state) {
  // Check configuration algorithm.
  if (in_array($form_state['values']['alg'], JWT_HMAC)) {
    // Add prefix file://
    if ($form_state['values']['secret']['secret_method'] == 'file') {
      $form_state['values']['secret']['file']['secret'] = 'file://' . $form_state['values']['secret']['file']['secret'];
    }
    // HMAC save: Create new jwt_config.
    if (!isset($form_state['values']['config'])) {
      $entity = entity_get_controller('jwt_config')->create(array(
        'name' => $form_state['values']['name'],
        'label' => $form_state['values']['label'],
        'description' => $form_state['values']['description'],
        'alg' => $form_state['values']['alg'],
        'method' => $form_state['values']['secret']['secret_method'],
        'public_key' => $form_state['values']['secret'][$form_state['values']['secret']['secret_method']]['secret'],
        'private_key' => $form_state['values']['secret'][$form_state['values']['secret']['secret_method']]['secret'],
        'enabled' => $form_state['values']['enabled'],
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
      ));
    }
    else {
      // Load default configuration.
      $entity = $form_state['values']['config'];
      $entity->name = $form_state['values']['name'];
      $entity->label = $form_state['values']['label'];
      $entity->description = $form_state['values']['description'];
      $entity->alg = $form_state['values']['alg'];
      $entity->method = $form_state['values']['secret']['secret_method'];
      $entity->public_key = $form_state['values']['secret'][$form_state['values']['secret']['secret_method']]['secret'];
      $entity->private_key = $form_state['values']['secret'][$form_state['values']['secret']['secret_method']]['secret'];
      $entity->enabled = $form_state['values']['enabled'];
      $entity->changed = REQUEST_TIME;
    }
    entity_save('jwt_config', $entity);
  }
  else {
    // Check whether key method if input.
    if ($form_state['values']['key']['key_method'] == 'input') {
      // Trim for accidental whitespaces.
      $form_state['values']['key']['input']['public'] = trim($form_state['values']['key']['input']['public']);
      // Check whether need wordwrap.
      if (preg_match('#[^\v]{65,}\v#', $form_state['values']['key']['input']['public'])) {
        // Wordwrap it in order to meed the certificate standard.
        $form_state['values']['key']['input']['public'] = wordwrap($form_state['values']['key']['input']['public'], 64, "\n", TRUE);
      }
      // Check whitespace between - chars.
      $form_state['values']['key']['input']['public'] = preg_replace('#\h*([\-]+)\h*#', '$1', $form_state['values']['key']['input']['public']);
      if (!empty($form_state['values']['key']['input']['private'])) {
        $form_state['values']['key']['input']['private'] = trim($form_state['values']['key']['input']['private']);
        if (preg_match('#[^\v]{65,}\v#', $form_state['values']['key']['input']['private'])) {
          $form_state['values']['key']['input']['private'] = wordwrap($form_state['values']['key']['input']['private'], 64, "\n", TRUE);
        }
        $form_state['values']['key']['input']['private'] = preg_replace('#\h*([\-]+)\h*#', '$1', $form_state['values']['key']['input']['private']);
      }
    }
    else {
      // Add prefix file://.
      $form_state['values']['key']['file']['public'] = 'file://' . $form_state['values']['key']['file']['public'];
      if (!empty($form_state['values']['key']['file']['private'])) {
        $form_state['values']['key']['file']['private'] = 'file://' . $form_state['values']['key']['file']['private'];
      }
    }
    // RSA and ECDSA save: create new jwt_config.
    if (!isset($form_state['values']['config'])) {
      $entity = entity_get_controller('jwt_config')->create(array(
        'name' => $form_state['values']['name'],
        'label' => $form_state['values']['label'],
        'description' => $form_state['values']['description'],
        'alg' => $form_state['values']['alg'],
        'method' => $form_state['values']['key']['key_method'],
        'public_key' => $form_state['values']['key'][$form_state['values']['key']['key_method']]['public'],
        'private_key' => $form_state['values']['key'][$form_state['values']['key']['key_method']]['private'],
        'enabled' => $form_state['values']['enabled'],
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
      ));
    }
    else {
      // Load default configuration.
      $entity = $form_state['values']['config'];
      $entity->name = $form_state['values']['name'];
      $entity->label = $form_state['values']['label'];
      $entity->description = $form_state['values']['description'];
      $entity->alg = $form_state['values']['alg'];
      $entity->method = $form_state['values']['key']['key_method'];
      $entity->public_key = $form_state['values']['key'][$form_state['values']['key']['key_method']]['public'];
      $entity->private_key = $form_state['values']['key'][$form_state['values']['key']['key_method']]['private'];
      $entity->enabled = $form_state['values']['enabled'];
      $entity->changed = REQUEST_TIME;
    }
    entity_save('jwt_config', $entity);
  }
  // Save jwt config entity to cache.
  if (isset($entity->cid)) {
    cache_set('jwt:config:' . $entity->cid, $entity, 'cache');
  }
  // Show message to admin.
  drupal_set_message('JWT Configuration has been saved.');
  // Redirect to list page.
  $form_state['redirect'] = JWT_CONFIG_MENU_PATH;
}

/**
 * Function jwt_config_form_delete_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_config_form_delete_submit($form, &$form_state) {
  // Redirect to delete page.
  if (isset($form['config']['#value']->cid)) {
    $form_state['redirect'] = JWT_CONFIG_MENU_PATH . '/delete/' . $form['config']['#value']->cid;
  }
}

/**
 * Function jwt_config_delete_confirm().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $config
 *   JWT object.
 *
 * @return array
 *   Form array.
 */
function jwt_config_delete_confirm($form, &$form_state, $config = NULL) {
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $config->cid,
  );
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $config->label,
  );

  $message = t('Are you sure you want to delete the configuration %label?', array('%label' => $config->label));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, filter_xss_admin($message), JWT_CONFIG_MENU_PATH, filter_xss_admin($caption), t('Delete'));
}

/**
 * Function jwt_config_delete_confirm_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_config_delete_confirm_submit($form, &$form_state) {
  // Delete jwt config cache.
  cache_clear_all('jwt:config:' . $form_state['values']['cid'], 'cache');
  // Delete jwt config entity.
  entity_delete('jwt_config', $form_state['values']['cid']);
  // Notify the admin.
  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The configuration %label has been deleted.', $t_args));
  watchdog('jwt', 'Deleted JWT configuration %label.', $t_args, WATCHDOG_NOTICE);
  // Redirect to list page.
  $form_state['redirect'] = JWT_CONFIG_MENU_PATH;
}

/**
 * Function jwt_configs_list().
 *
 * @return array
 *   Page build array contain table of jwt config.
 */
function jwt_configs_list() {
  // Define table header.
  $header = array(
    t('Name'),
    t('Algorithm'),
    t('Method'),
    t('Updated'),
    t('Created'),
    t('Status'),
    t('Operations'),
  );
  // Define row container.
  $rows = array();

  // Query to all jwt config.
  $query = db_select('jwt_config', 'jc')->extend('TableSort');
  $query->fields('jc', array(
    'cid', 'name', 'label', 'description', 'alg', 'method', 'enabled',
    'created', 'changed',
  ));
  $result = $query->orderByHeader($header)->execute();

  // Looping for each config.
  foreach ($result as $config) {
    // Define operation link.
    $links = array(
      array(
        'title' => t('Edit'),
        'href' => JWT_CONFIG_MENU_PATH . '/edit/' . $config->cid,
      ),
      array(
        'title' => t('Delete'),
        'href' => JWT_CONFIG_MENU_PATH . '/delete/' . $config->cid,
      ),
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
    // Beautify config name.
    $name = check_plain($config->label);
    $name .= ' <small>' . t('(Machine name: @name)', array('@name' => $config->name)) . '</small>';
    $name .= '<div class="description">' . filter_xss_admin($config->description) . '</div>';
    // Append row to the rows container.
    // If unable to get voucher status: redeem or available, show error row.
    $rows[] = array(
      'data' => array(
        $name, $config->alg, $config->method,
        format_date($config->changed, 'short'),
        format_date($config->created, 'short'),
        ($config->enabled ? t('enabled') : t('disabled')),
        $operations,
      ),
    );
  }

  // Attach the pager table.
  $build['jwt_config_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no jwt config found. <a href="@link">Add a configuration</a>.', array(
      '@link' => url(JWT_CONFIG_MENU_PATH . '/add'),
    )),
  );

  return $build;
}
