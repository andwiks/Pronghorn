<?php

/**
 * @file
 * Kupon Rules code: actions.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function kupon_rules_action_info() {
  $actions = array(
    'kupon_create_owned_simple_coupon' => array(
      'label' => t('Create Owned Simple Coupon'), // Name displayed to admins
      'group' => t('Sepulsa'), // Used for grouping actions in select lists
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_line_item',
          'label' => t('Commerce Line Item'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('User to say hello to'),
        ),
      ),
    ),
  );
  
  return $actions;
}

/**
 * The action function for 'kupon_create_owned_simple_coupon'.
 */
function kupon_create_owned_simple_coupon($line_item, $user) {
  if ($line_item->type == "coupon") {
    //drupal_set_message("<pre>".print_r($line_item, true)."</pre>");
    $node_coupon = node_load($line_item->data['context']['entity']['entity_id']);
    $product = commerce_product_load($line_item->data['context']['product_ids'][0]);
    $expiry = field_get_items('commerce_product', $product, 'field_expiry_date');
    
    if (isset($node_coupon->field_coupon_type[LANGUAGE_NONE]) && $node_coupon->field_coupon_type[LANGUAGE_NONE][0]['value'] == 'unique') {
        //$unique_nid = db_query_range("SELECT nid FROM {node} WHERE uid = 1 AND type = 'owned_unique_coupon'", 0, 1)->fetchField();
        $query = "SELECT nid FROM {node} a, {field_data_field_simple_coupon} b "
                . "WHERE a.nid=b.entity_id AND uid = 1 AND type = 'owned_unique_coupon' AND field_simple_coupon_target_id=".$line_item->data['context']['entity']['entity_id'];
        $unique_nid = db_query_range($query, 0, 1)->fetchField();
        if ($unique_nid != null) {
            $node = node_load($unique_nid);
            $node->language = LANGUAGE_NONE;
            $node->uid = $user->uid;
            $node->field_order_id[$node->language][] = array(
                'value' => $line_item->order_id,
            );
            $node->field_owned_coupon_expiry[$node->language][0]['value'] = $expiry[0]['value'];
            node_save($node);
        } else {
            drupal_set_message($node_coupon->title.' is out of stock', 'error');
        }
    } else {
        $node = new stdClass();
        $node->title = $node_coupon->title;
        $node->type = "owned_simple_coupon";
        $node->language = LANGUAGE_NONE;

        $node->field_simple_coupon[$node->language][] = array(
          'target_id' => $node_coupon->nid,
          'target_type' => 'node',
        );
        $node->field_owned_coupon_expiry[$node->language][] = array(
          'value' => $expiry[0]['value'],
          'timezone' => date_default_timezone(),
          'timezone_db' => date_default_timezone(),
          'date_type' => 'datestamp',
        );
        $node->field_order_id[$node->language][] = array(
          'value' => $line_item->order_id,
        );
        $node->uid = $user->uid;
        $node = node_submit($node); // Prepare node for saving
        node_save($node);
    }
    //drupal_set_message( "Node with nid " . $node->nid . " saved!\n");
  }
}
