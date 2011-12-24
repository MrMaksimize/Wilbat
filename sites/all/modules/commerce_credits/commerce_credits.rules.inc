<?php


/**
 * @file
 * Rules integration with commerce credits module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_credits_rules_action_info() {
  $items = array();
  $items['commerce_credits_update_on_checkout'] = array(
    'label' => t('Updates credits based on order'),
    'group' => t('Commerce Credits'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order containing the credits bought.'),
      ),
    ),
   'callbacks' => array(
      'execute' => 'commerce_credits_rules_action_update_on_checkout',
    ),
  );
  $items['commerce_credits_transfer'] = array(
    'label' => t('Transfer credits'),
    'group' => t('Commerce Credits'),
    'parameter' => array(
      'entity_from' => array(
        'type' => 'entity',
        'label' => t('Transfer from'),
        'description' => t('Entity to transfer from'),
        'wrapped' => TRUE,
      ),
      'entity_to' => array(
        'type' => 'entity',
        'label' => t('Transfer to'),
        'description' => t('Entity to transfer to'),
        'wrapped' => TRUE,
      ),
      'credit_type' => array(
        'type' => 'text',
        'label' => t('Credit type'),
        'description' => t('Credit type to be transfered.'),
        'options list' => 'commerce_credits_types_list',
      ),
      'number' => array(
        'type' => 'integer',
        'label' => t('Number of credits'),
        'description' => t('Number of credits that should be transfered.'),
      )
    ),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_action_transfer_credits'
    )
  );
  $items['commerce_credits_add'] = array(
    'label' => t('Add credits'),
    'group' => t('Commerce Credits'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('Add credits to entity'),
        'wrapped' => TRUE,
      ),
      'credit_type' => array(
        'type' => 'text',
        'label' => t('Credit type'),
        'description' => t('Credit type to be added.'),
        'options list' => 'commerce_credits_types_list',
      ),
      'number' => array(
        'type' => 'integer',
        'label' => t('Number of credits'),
        'description' => t('Number of credits that should be added.'),
      )
    ),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_action_add_credits'
    )
  );
  $items['commerce_credits_remove'] = array(
    'label' => t('Remove credits'),
    'group' => t('Commerce Credits'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('Remove credits from entity'),
        'wrapped' => TRUE,
      ),
      'credit_type' => array(
        'type' => 'text',
        'label' => t('Credit type'),
        'description' => t('Credit type to be removed.'),
        'options list' => 'commerce_credits_types_list',
      ),
      'number' => array(
        'type' => 'integer',
        'label' => t('Number of credits'),
        'description' => t('Number of credits that should be removed.'),
      )
    ),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_action_remove_credits'
    )
  );
  $items['commerce_credits_set_credits_expiration_date'] = array(
    'label' => t('Set credits expiration date'),
    'group' => t('Commerce Credits'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('Entity for which credits expiration date shall be set'),
        'wrapped' => TRUE,
      ),
      'credit_type' => array(
        'type' => 'text',
        'label' => t('Credit type'),
        'description' => t('Credit type for which expiration date shall be set.'),
        'options list' => 'commerce_credits_types_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_action_set_expiration_date'
    )
  );
  $items['commerce_credits_delete_chosen_credit'] = array(
    'label' => t('Delete chosen credit'),
    'group' => t('Commerce Credits'),
    'parameter' => array(
    ),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_action_delete_chosen_credit'
    )
  );

  return $items;
}

/**
 * Updates number of credits a user has based on his order
 */
function commerce_credits_rules_action_update_on_checkout($order) {
  $uid = $order->uid;
  $numbers = array();
  $commerce_line_items = $order->commerce_line_items[LANGUAGE_NONE];
  foreach ($commerce_line_items as $l) {
    $id = $l['line_item_id'];
    $line_item = commerce_line_item_load($id);
    if ($line_item->type == 'product') {
      $sku = $line_item->line_item_label;
      $product = commerce_product_load_by_sku($sku);
      if ($product->type == 'commerce_credits') {
        $number = $line_item->quantity * $product->commerce_credits_number[LANGUAGE_NONE][0]['value'];
        $credit_type = $product->commerce_credits_type[LANGUAGE_NONE][0]['value'];
        if (isset($numbers[$credit_type])) {
          $numbers[$credit_type] += $number;
        }
        else {
          $numbers[$credit_type] = $number;
        }
      }
    }
  }
  $user = entity_metadata_wrapper('user', user_load($uid));
  foreach ($numbers as $credit_type => $number) {
    commerce_credits_add_credits($user, $credit_type, $number);
  }
}

/**
 * Transfers credits from one entity to another
 */
function commerce_credits_rules_action_transfer_credits(EntityDrupalWrapper $from, EntityDrupalWrapper $to, $type, $number) {
  commerce_credits_credit_transfer_by_type($from, $to, $type, $number);
}

/**
 * Adds credits to an entity.
 */
function commerce_credits_rules_action_add_credits(EntityDrupalWrapper $entity, $credit_type, $number) {
  commerce_credits_add_credits($entity, $credit_type, $number);
}

/**
 * Removes credits from an entity.
 */
function commerce_credits_rules_action_remove_credits(EntityDrupalWrapper $entity, $credit_type, $number) {
  commerce_credits_remove_credits($entity, $credit_type, $number);
}

/**
 * Set credits expiration date
 */
function commerce_credits_rules_action_set_expiration_date(EntityDrupalWrapper $entity, $type) {
  commerce_credits_credit_set_entity_expiration_date($entity, $type);
}

function commerce_credits_rules_action_delete_chosen_credit() {
  unset($_SESSION[COMMERCE_CREDITS_SESSION_VARIABLE]);
}

/**
 * Implements hook_rules_condition_info()
 */
function commerce_credits_rules_condition_info() {
  $items = array();
  $items['commerce_credits_entity_has_credits'] = array(
    'label' => t('Entity has credits'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('Specify the entity for which to evaluate the condition.'),
        'wrapped' => TRUE,
      ),
      'credit_type' => array(
        'type' => 'text',
        'label' => t('Credit type'),
        'description' => t('Which credit type this entity should have for the condition to be true.'),
        'options list' => 'commerce_credits_types_list',
      ),
      'credit_number' => array(
        'type' => 'integer',
        'label' => t('Number of credits'),
        'description' => t('Number of credits the entity should have for this condition to be true.'),
      )
    ),
    'group' => t('Commerce Credits'),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_condition_entity_has_credits'
    )
  );
  $items['commerce_credits_entity_has_credits_of_group'] = array(
    'label' => t('Entity has credits of group'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('Specify the entity for which to evaluate the condition.'),
        'wrapped' => TRUE,
      ),
      'credit_group' => array(
        'type' => 'text',
        'label' => t('Credit group'),
        'description' => t('Which credit group this entity should have for the condition to be true.'),
        'options list' => 'commerce_credits_groups_list',
      ),
      'credit_number' => array(
        'type' => 'integer',
        'label' => t('Number of credits'),
        'description' => t('Number of credits the entity should have for this condition to be true.'),
      )
    ),
    'group' => t('Commerce Credits'),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_condition_entity_has_credits_of_group'
    )
  );
  $items['commerce_credits_is_of_type'] = array(
    'label' => t('Credit is of type'),
    'parameter' => array(
      'credit_type_to_check' => array(
        'type' => 'text',
        'label' => t('Credit to check'),
        'description' => t('Specify the credit for which to evaluate the condition.'),
        'wrapped' => TRUE,
      ),
      'credit_type' => array(
        'type' => 'text',
        'label' => t('Credit type'),
        'description' => t('Which credit type this credit should have for the condition to be true.'),
        'options list' => 'commerce_credits_types_list',
      ),
    ),
    'group' => t('Commerce Credits'),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_condition_credit_is_of_type'
    )
  );
  $items['commerce_credits_is_of_group'] = array(
    'label' => t('Credit is of group'),
    'parameter' => array(
      'credit_type_to_check' => array(
        'type' => 'text',
        'label' => t('Credit to check'),
        'description' => t('Specify the credit for which to evaluate the condition.'),
      ),
      'credit_group' => array(
        'type' => 'text',
        'label' => t('Credit group'),
        'description' => t('Which credit group this credit should belong to for the condition to be true.'),
        'options list' => 'commerce_credits_groups_list',
      ),
    ),
    'group' => t('Commerce Credits'),
    'callbacks' => array(
      'execute' => 'commerce_credits_rules_condition_credit_is_of_group'
    )
  );
  return $items;
}

function commerce_credits_rules_condition_entity_has_credits(EntityDrupalWrapper $entity, $credit_type, $credit_number) {
  $credits = commerce_credits_credit_load_by_entity($entity, $credit_type);
  if ($credits) {
    $credits = reset($credits);
    if (count($credits->credits) >= $credit_number) {
      return TRUE;
    }
  }
  else {
    return FALSE;
  }
}

function commerce_credits_rules_condition_entity_has_credits_of_group(EntityDrupalWrapper $entity, $credit_group, $credit_number) {
  return commerce_credits_entity_has_credits_of_group($entity, $credit_group, $credit_number);
}

function commerce_credits_rules_condition_credit_is_of_type($credit_type_to_check, $credit_type) {
  return ($credit_type_to_check == $credit_type);
}

function commerce_credits_rules_condition_credit_is_of_group($credit_type_to_check, $credit_group) {
  $types = commerce_credits_types();
  $type = $types[$credit_type_to_check];
  return ($type->type == $credit_group);
}

/**
 * Implements hook_rules_event_info()
 */
function commerce_credits_rules_event_info() {
  $items = array();
  $items['commerce_credits_expire'] = array(
    'label' => t('When credits expire'),
    'variables' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Associated entity'),
        'wrapped' => TRUE,
      ),
      'credit' => array(
        'type' => 'commerce_credits',
        'label' => t('Credit'),
        'description' => t('The credit entity that expires.'),
      ),
      'credit_type' => array(
        'type' => 'commerce_credits_type',
        'label' => t('Credit type'),
      ),
    ),
    'group' => t('Commerce Credit'),
  );
  $items['commerce_credits_after_choice'] = array(
    'label' => t('After credit choice'),
    'variables' => array(
      'credit_type' => array(
        'type' => 'commerce_credits_type',
        'label' => t('Credit type'),
      ),
    ),
    'group' => t('Commerce Credit'),
  );
  return $items;
}


