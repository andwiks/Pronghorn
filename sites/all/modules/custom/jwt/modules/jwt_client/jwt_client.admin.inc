<?php
/**
 * @file
 * jwt_client.admin.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Callback Functions.
 */

/**
 * Function jwt_client_list().
 *
 * @return array
 *   Form array.
 */
function jwt_client_list() {
  // Define table header.
  $header = array(
    array('data' => t('ID'), 'field' => 'jc.cid'),
    t('Name'),
    t('Endpoint'),
    t('Passkey'),
    array('data' => t('Updated'), 'field' => 'jc.changed'),
    array('data' => t('Created'), 'field' => 'jc.created'),
    array('data' => t('Status'), 'field' => 'jc.active'),
    t('Operations'),
  );
  // Define row container.
  $rows = array();

  // Query to all jwt client.
  $query = db_select('jwt_client', 'jc')->extend('TableSort');
  $query->fields('jc', array(
    'cid', 'name', 'label', 'endpoint', 'passkey', 'active', 'created',
    'changed',
  ));
  $result = $query->orderByHeader($header)->execute();

  // Looping for each client.
  foreach ($result as $client) {
    // Define operation link.
    $links = array(
      array(
        'title' => ($client->active ? t('Disable') : t('Enable')),
        'href' => JWT_CLIENT_CONFIG_MENU_PATH . '/' . $client->cid . '/' . ($client->active ? 'disable' : 'enable'),
      ),
      array(
        'title' => t('Edit'),
        'href' => JWT_CLIENT_CONFIG_MENU_PATH . '/' . $client->cid,
      ),
      array(
        'title' => t('Delete'),
        'href' => JWT_CLIENT_CONFIG_MENU_PATH . '/' . $client->cid . '/delete',
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
    // Beautify client name.
    $name = check_plain($client->label);
    $name .= ' <small>' . t('(Machine name: @name)', array('@name' => $client->name)) . '</small>';
    // Append row to the rows container.
    // If unable to get voucher status: redeem or available, show error row.
    $rows[] = array(
      'data' => array(
        $client->cid, $name, l($client->endpoint, 'admin/structure/services/list/' . $client->endpoint . '/resources', array(
          'attributes' => array('target' => '_blank'),
        )), $client->passkey, format_date($client->changed, 'short'),
        format_date($client->created, 'short'),
        ($client->active ? t('enabled') : t('disabled')),
        $operations,
      ),
    );
  }

  // Attach the pager table.
  $build['jwt_client_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no jwt client configuration found. <a href="@link">Add a configuration</a>.', array(
      '@link' => url(JWT_CLIENT_CONFIG_MENU_PATH . '/add'),
    )),
  );
  return $build;
}

/**
 * Function jwt_client_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $client
 *   JWT client object.
 *
 * @return array
 *   Form array.
 */
function jwt_client_form($form = array(), $form_state = array(), $client = NULL) {
  // Define label.
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($client->label) ? $client->label : '',
    '#description' => t('The human-readable name of the client configuration.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($client->name) ? $client->name : '',
    '#maxlength' => 32,
    '#disabled' => isset($client->name),
    '#machine_name' => array(
      'exists' => 'jwt_client_load_by_name',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for the client configuration. It must only contain lowercase letters, numbers, and underscores.'),
  );

  // Define configuration is active.
  $form['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($client->active) ? $client->active : '',
    '#description' => t('If checked, this configuration will be available.'),
  );

  // Client passkey.
  $form['passkey'] = array(
    '#title' => t('Passkey'),
    '#type' => 'textfield',
    '#default_value' => isset($client->passkey) ? $client->passkey : '',
    '#description' => t('Server passkey.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Get all possible endpoints.
  $endpoints = services_endpoint_load_all();
  // Endpoint select.
  $form['endpoint'] = array(
    '#type' => 'select',
    '#title' => t('Endpoint'),
    '#options' => drupal_map_assoc(array_keys($endpoints)),
    '#default_value' => isset($client->endpoint) ? $client->endpoint : '',
    '#description' => t('Set client configuration attached to this endpoint. Check services !link.', array(
      '!link' => l(t('settings'), 'admin/structure/services', array(
        'attributes' => array('target' => '_blank'),
      )),
    )),
    '#required' => TRUE,
  );

  // Only if client endpoint is available.
  if (isset($client->endpoint) && isset($endpoints[$client->endpoint])) {
    // Get all resources data from this endpoint.
    $resources_data = services_get_resources($client->endpoint);
    // Get default data.
    $resources = isset($client->resources) ? $client->resources : array();
    // Define tableselect header.
    $header = array(
      'resource' => array('data' => t('Resource')),
      'operation' => array('data' => t('Operation / Action')),
      'description' => array('data' => t('Description')),
    );
    // Set resource as tree.
    $form['resources'] = array(
      '#tree' => TRUE,
    );
    // Define default options.
    $options = array();
    // Service endpoint resource checboxes.
    foreach ($endpoints[$client->endpoint]->resources as $key => $data) {
      foreach ($data as $op => $res) {
        foreach ($res as $name => $setting) {
          // Set options for this resource.
          $options[$name] = array(
            'resource' => $key,
            'operation' => $name,
            'description' => $resources_data[$key][$op][$name]['help'],
          );
        }
      }
      // Set form table select for each resource.
      $form['resources'][$key] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#default_value' => isset($resources[$key]) ? $resources[$key] : array(),
        '#empty' => t('No content available.'),
      );
      $options = array();
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('jwt_client_form_submit'),
  );

  // Check if configuration is loaded.
  if (isset($client->name)) {
    $form['client'] = array(
      '#type' => 'value',
      '#value' => $client,
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete configuration'),
      '#submit' => array('jwt_client_form_delete_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * Function jwt_client_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_client_form_submit($form, &$form_state) {
  // Create new jwt_client.
  if (!isset($form_state['values']['client'])) {
    $entity = entity_get_controller('jwt_client')->create(array(
      'name' => $form_state['values']['name'],
      'label' => $form_state['values']['label'],
      'endpoint' => $form_state['values']['endpoint'],
      'resources' => '',
      'passkey' => $form_state['values']['passkey'],
      'active' => $form_state['values']['active'],
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    ));
  }
  else {
    // Load default configuration.
    $entity = $form_state['values']['client'];
    $entity->name = $form_state['values']['name'];
    $entity->label = $form_state['values']['label'];
    $entity->endpoint = $form_state['values']['endpoint'];
    $entity->resources = serialize($form_state['values']['resources']);
    $entity->passkey = $form_state['values']['passkey'];
    $entity->active = $form_state['values']['active'];
    $entity->changed = REQUEST_TIME;
  }
  entity_save('jwt_client', $entity);
  // Save jwt client config entity to cache.
  if (isset($entity->cid)) {
    $entity->resources = !empty($entity->resources) ? unserialize($entity->resources) : array();
    cache_set('jwt:client:' . $entity->cid, $entity, 'cache');
  }
  // Show message to admin.
  drupal_set_message('JWT Client Configuration has been saved.');
  // Redirect to list page.
  $form_state['redirect'] = JWT_CLIENT_CONFIG_MENU_PATH;
}

/**
 * Function jwt_client_form_delete_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_client_form_delete_submit($form, &$form_state) {
  // Redirect to delete page.
  if (isset($form['client']['#value']->cid)) {
    $form_state['redirect'] = JWT_CLIENT_CONFIG_MENU_PATH . '/' . $form['client']['#value']->cid . '/delete';
  }
}

/**
 * Function jwt_client_delete_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param array $client
 *   Client configuration.
 *
 * @return array
 *   Form array.
 */
function jwt_client_delete_form($form = array(), $form_state = array(), $client = NULL) {
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $client->cid,
  );
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $client->label,
  );

  $message = t('Are you sure you want to delete the client configuration %label?', array('%label' => $client->label));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, filter_xss_admin($message), JWT_CLIENT_CONFIG_MENU_PATH, filter_xss_admin($caption), t('Delete'));
}

/**
 * Function jwt_client_delete_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_client_delete_form_submit($form, &$form_state) {
  // Delete jwt client config cache.
  cache_clear_all('jwt:client:' . $form_state['values']['cid'], 'cache');
  // Delete jwt config entity.
  entity_delete('jwt_client', $form_state['values']['cid']);
  // Notify the admin.
  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The JWT client configuration %label has been deleted.', $t_args));
  watchdog('jwt_client', 'Deleted JWT Client configuration %label.', $t_args, WATCHDOG_NOTICE);
  // Redirect to list page.
  $form_state['redirect'] = JWT_CLIENT_CONFIG_MENU_PATH;
}

/**
 * Function jwt_client_enable().
 *
 * @param object $client
 *   JWT client object.
 */
function jwt_client_enable($client = NULL) {
  if (!empty($client) && isset($client->cid)) {
    $client->active = TRUE;
    entity_save('jwt_client', $client);
    cache_set('jwt:client:' . $client->cid, $client, 'cache');
    drupal_set_message(t('JWT Client Configuration %label has been enabled.', array(
      '%label' => $client->label,
    )));
    drupal_goto(JWT_CLIENT_CONFIG_MENU_PATH);
  }
}

/**
 * Function jwt_client_disable().
 *
 * @param object $client
 *   JWT client object.
 */
function jwt_client_disable($client = NULL) {
  if (!empty($client) && isset($client->cid)) {
    $client->active = FALSE;
    entity_save('jwt_client', $client);
    cache_set('jwt:client:' . $client->cid, $client, 'cache');
    drupal_set_message(t('JWT Client Configuration %label has been disabled.', array(
      '%label' => $client->label,
    )));
    drupal_goto(JWT_CLIENT_CONFIG_MENU_PATH);
  }
}
