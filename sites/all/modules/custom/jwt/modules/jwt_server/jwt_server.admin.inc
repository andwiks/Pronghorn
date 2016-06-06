<?php
/**
 * @file
 * jwt_server.admin.inc
 *
 * @author andre4s_y
 */

/**
 * Function jwt_server_list().
 *
 * @return array
 *   Form array.
 */
function jwt_server_list() {
  // Define table header.
  $header = array(
    array('data' => t('ID'), 'field' => 'js.sid'),
    t('Client Name'),
    t('URL'),
    t('Passkey'),
    array('data' => t('Updated'), 'field' => 'js.changed'),
    array('data' => t('Created'), 'field' => 'js.created'),
    array('data' => t('Status'), 'field' => 'js.active'),
    t('Operations'),
  );
  // Define row container.
  $rows = array();

  // Query to all jwt server.
  $query = db_select('jwt_server', 'js')->extend('TableSort');
  $query->fields('js', array(
    'sid', 'name', 'label', 'url', 'passkey', 'active', 'created', 'changed',
  ));
  $result = $query->orderByHeader($header)->execute();

  // Looping for each server.
  foreach ($result as $server) {
    // Define operation link.
    $links = array(
      array(
        'title' => ($server->active ? t('Disable') : t('Enable')),
        'href' => JWT_SERVER_CONFIG_MENU_PATH . '/' . $server->sid . '/' . ($server->active ? 'disable' : 'enable'),
      ),
      array(
        'title' => t('Edit'),
        'href' => JWT_SERVER_CONFIG_MENU_PATH . '/' . $server->sid,
      ),
      array(
        'title' => t('Delete'),
        'href' => JWT_SERVER_CONFIG_MENU_PATH . '/' . $server->sid . '/delete',
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
    // Beautify server name.
    $name = check_plain($server->label);
    $name .= ' <small>' . t('(Machine name: @name)', array('@name' => $server->name)) . '</small>';
    // Append row to the rows container.
    // If unable to get voucher status: redeem or available, show error row.
    $rows[] = array(
      'data' => array(
        $server->sid, $name, $server->url, $server->passkey,
        format_date($server->changed, 'short'),
        format_date($server->created, 'short'),
        ($server->active ? t('enabled') : t('disabled')),
        $operations,
      ),
    );
  }

  // Attach the pager table.
  $build['jwt_server_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no jwt server configuration found. <a href="@link">Add a configuration</a>.', array(
      '@link' => url(JWT_SERVER_CONFIG_MENU_PATH . '/add'),
    )),
  );
  return $build;
}

/**
 * Function jwt_server_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $server
 *   JWT server object.
 *
 * @return array
 *   Form array.
 */
function jwt_server_form($form = array(), $form_state = array(), $server = NULL) {
  // Define label.
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($server->label) ? $server->label : '',
    '#description' => t('The human-readable name of the client configuration.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($server->name) ? $server->name : '',
    '#maxlength' => 32,
    '#disabled' => isset($server->name),
    '#machine_name' => array(
      'exists' => 'jwt_server_load_by_name',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for the client configuration. It must only contain lowercase letters, numbers, and underscores.'),
  );

  // Define configuration is active.
  $form['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($server->active) ? $server->active : '',
    '#description' => t('If checked, this configuration will be available.'),
  );

  // Client passkey.
  $form['passkey'] = array(
    '#title' => t('Passkey'),
    '#type' => 'textfield',
    '#default_value' => isset($server->passkey) ? $server->passkey : '',
    '#description' => t('Server passkey.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Client URL.
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Client URL'),
    '#default_value' => (isset($server->url)) ? $server->url : '',
    '#maxlength' => 128,
    '#description' => t('Please input full url: https:// or http://.'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('jwt_server_form_submit'),
  );

  // Check if configuration is loaded.
  if (isset($server->name)) {
    $form['server'] = array(
      '#type' => 'value',
      '#value' => $server,
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete configuration'),
      '#submit' => array('jwt_server_form_delete_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * Function jwt_server_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_server_form_submit($form, &$form_state) {
  // Create new jwt_server.
  if (!isset($form_state['values']['server'])) {
    $entity = entity_get_controller('jwt_server')->create(array(
      'name' => $form_state['values']['name'],
      'label' => $form_state['values']['label'],
      'url' => $form_state['values']['url'],
      'passkey' => $form_state['values']['passkey'],
      'active' => $form_state['values']['active'],
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    ));
  }
  else {
    // Load default configuration.
    $entity = $form_state['values']['server'];
    $entity->name = $form_state['values']['name'];
    $entity->label = $form_state['values']['label'];
    $entity->url = $form_state['values']['url'];
    $entity->passkey = $form_state['values']['passkey'];
    $entity->active = $form_state['values']['active'];
    $entity->changed = REQUEST_TIME;
  }
  entity_save('jwt_server', $entity);
  // Save jwt server config entity to cache.
  if (isset($entity->sid)) {
    cache_set('jwt:server:' . $entity->sid, $entity, 'cache');
  }
  // Show message to admin.
  drupal_set_message('JWT Server Configuration has been saved.');
  // Redirect to list page.
  $form_state['redirect'] = JWT_SERVER_CONFIG_MENU_PATH;
}

/**
 * Function jwt_server_form_delete_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_server_form_delete_submit($form, &$form_state) {
  // Redirect to delete page.
  if (isset($form['server']['#value']->sid)) {
    $form_state['redirect'] = JWT_SERVER_CONFIG_MENU_PATH . '/' . $form['server']['#value']->sid . '/delete';
  }
}

/**
 * Function jwt_server_delete_form().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param array $server
 *   Server configuration.
 *
 * @return array
 *   Form array.
 */
function jwt_server_delete_form($form = array(), $form_state = array(), $server = NULL) {
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $server->sid,
  );
  $form['label'] = array(
    '#type' => 'value',
    '#value' => $server->label,
  );

  $message = t('Are you sure you want to delete the server configuration %label?', array('%label' => $server->label));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, filter_xss_admin($message), JWT_SERVER_CONFIG_MENU_PATH, filter_xss_admin($caption), t('Delete'));
}

/**
 * Function jwt_server_delete_form_submit().
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function jwt_server_delete_form_submit($form, &$form_state) {
  // Delete jwt server config cache.
  cache_clear_all('jwt:server:' . $form_state['values']['sid'], 'cache');
  // Delete jwt config entity.
  entity_delete('jwt_server', $form_state['values']['sid']);
  // Notify the admin.
  $t_args = array('%label' => $form_state['values']['label']);
  drupal_set_message(t('The JWT server configuration %label has been deleted.', $t_args));
  watchdog('jwt_server', 'Deleted JWT Server configuration %label.', $t_args, WATCHDOG_NOTICE);
  // Redirect to list page.
  $form_state['redirect'] = JWT_SERVER_CONFIG_MENU_PATH;
}

/**
 * Function jwt_server_enable().
 *
 * @param object $server
 *   JWT server object.
 */
function jwt_server_enable($server = NULL) {
  if (!empty($server) && isset($server->sid)) {
    $server->active = TRUE;
    entity_save('jwt_server', $server);
    cache_set('jwt:server:' . $server->sid, $server, 'cache');
    drupal_set_message(t('JWT Server Configuration %label has been enabled.', array(
      '%label' => $server->label,
    )));
    drupal_goto(JWT_SERVER_CONFIG_MENU_PATH);
  }
}

/**
 * Function jwt_server_disable().
 *
 * @param object $server
 *   JWT server object.
 */
function jwt_server_disable($server = NULL) {
  if (!empty($server) && isset($server->sid)) {
    $server->active = FALSE;
    entity_save('jwt_server', $server);
    cache_set('jwt:server:' . $server->sid, $server, 'cache');
    drupal_set_message(t('JWT Server Configuration %label has been disabled.', array(
      '%label' => $server->label,
    )));
    drupal_goto(JWT_SERVER_CONFIG_MENU_PATH);
  }
}
