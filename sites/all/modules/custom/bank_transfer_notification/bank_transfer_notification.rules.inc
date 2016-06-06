<?php
/**
 * @file
 * bank_transfer_notification.rules.inc
 */

/**
 * Implements hook_rules_event_info().
 */
function bank_transfer_notification_rules_event_info() {
  $events['bank_transfer_notification_queue_email'] = array(
    'label' => t('When queue order payment filled'),
    'group' => t('Order Payment Remainder (Bank Transfer)'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Completed order', array(), array('context' => 'a drupal commerce order')),
      ),
    ),
  );

  return $events;
}
