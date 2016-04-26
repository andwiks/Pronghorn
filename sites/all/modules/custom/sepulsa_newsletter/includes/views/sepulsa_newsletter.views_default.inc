<?php
/**
 * @file
 * sepulsa_newsletter.views_default.inc
 *
 * Default Views for report
 */

/**
 * Implements hook_views_default_views().
 */
function sepulsa_newsletter_views_default_views() {
  $view = new view();
  $view->name = 'mailchimp_subscribers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Mailchimp Subscribers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Mailchimp Subscribers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: User: Subscribe Newsletter */
  $handler->display->display_options['fields']['field_newsletter_subscribers']['id'] = 'field_newsletter_subscribers';
  $handler->display->display_options['fields']['field_newsletter_subscribers']['table'] = 'field_data_field_newsletter_subscribers';
  $handler->display->display_options['fields']['field_newsletter_subscribers']['field'] = 'field_newsletter_subscribers';
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Subscribe Newsletter (field_newsletter_subscribers) */
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['id'] = 'field_newsletter_subscribers_value';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['table'] = 'field_data_field_newsletter_subscribers';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['field'] = 'field_newsletter_subscribers_value';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['operator_id'] = 'field_newsletter_subscribers_value_op';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['label'] = 'Subscribe Newsletter';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['description'] = '0 = Unsubscriber / 1 = Subscriber';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['operator'] = 'field_newsletter_subscribers_value_op';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['identifier'] = 'field_newsletter_subscribers_value';
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_newsletter_subscribers_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
    19 => 0,
    20 => 0,
    21 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
  );

  /* Display: Mailchimp Subscribers */
  $handler = $view->new_display('page', 'Mailchimp Subscribers', 'page');
  $handler->display->display_options['path'] = 'admin/reports/newsletter/mailchimp-subscribers';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Mailchimp Subscribers';
  $handler->display->display_options['menu']['description'] = 'Mailchimp Subscribers.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Mailchimp Subscriber Export */
  $handler = $view->new_display('views_data_export', 'Mailchimp Subscriber Export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'admin/reports/newsletter/mailchimp-subscriber-export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['mailchimp_subscribers'] = array(
    t('Master'),
    t('Mailchimp Subscribers'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Uid'),
    t('Name'),
    t('E-mail'),
    t('Subscribe Newsletter'),
    t('0 = Unsubscriber / 1 = Subscriber'),
    t('Mailchimp Subscriber Export'),
  );

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  return $views;
}
