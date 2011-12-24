<?php

/**
 * @file
 * Coupon rules
 *
 * This file contains...
 */

function commerce_coupon_rules_event_info() {
  $events = array();

  $events['commerce_coupon_validate'] = array(
    'label' => t('Validate a coupon'),
    'group' => t('Commerce Coupon'),
    'variables' => array(
      'coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('commerce coupon to validate')
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  $events['commerce_coupon_redeem'] = array(
    'label' => t('Redeem a coupon'),
    'group' => t('Commerce Coupon'),
    'variables' => array(
      'coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('commerce coupon to redeem')
      ),
      'coupon_log' => array(
        'type' => 'commerce_coupon_log',
        'label' => t('commerce coupon log')
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_coupon_rules_action_info() {
  $actions = array();

  $actions['commerce_coupon_action_is_valid_coupon'] = array(
    'label' => t('Set coupon as valid'),
    'parameter' => array(
      'override' => array(
        'type' => 'boolean',
        'label' => t('Override'),
        'description' => t('Normaly the validation result is concatenated by an AND operator. Here you can bypass this and set the value to true.'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_is_valid_coupon',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_is_valid_coupon',
    ),
  );

  $actions['commerce_coupon_action_is_invalid_coupon'] = array(
    'label' => t('Set coupon as invalid'),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_is_invalid_coupon',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_is_invalid_coupon',
    ),
  );

  $actions['commerce_coupon_action_set_granted_amount'] = array(
    'label' => t('Set granted coupon amount'),
    'parameter' => array(
      'commerce_coupon_log' => array(
        'type' => 'commerce_coupon_log',
        'label' => t('Commerce Coupon Log'),
      ),
      'amount' => array(
        'type' => 'decimal',
        'label' => t('Granted Amount'),
      ),
      'currency_code' => array(
        'type' => 'text',
        'label' => t('Currency Code'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_is_invalid_coupon',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_set_granted_amount',
    ),
  );

  $actions['commerce_coupon_action_create_coupon_line_item'] = array(
    'label' => t('Create coupon line item'),
    'parameter' => array(
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Commerce Coupon'),
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('commerce order'),
      ),
      'amount' => array(
        'type' => 'decimal',
        'label' => t('coupon amount'),
      ),
      'currency_code' => array(
        'type' => 'text',
        'label' => t('coupon amount currency code'),
      ),
    ),
    'provides' => array(
      'commerce_coupon_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('commerce coupon line item'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_create_coupon_line_item',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_create_coupon_line_item',
    ),
      );


  $actions['commerce_coupon_action_get_coupon_uses'] = array(
    'label' => t('Get the redemption number of a coupon'),
    'parameter' => array(
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Commerce Coupon'),
      ),

    ),
    'group' => t('Commerce Coupon'),
    'provides' => array(
      'number_of_uses' => array(
        'type' => 'integer',
        'label' => t('number of uses'),
      ),
    ),
  );


  return $actions;
}


function commerce_coupon_action_is_valid_coupon($override = FALSE) {
  $validation_results = &drupal_static('commerce_coupon_action_validation_results');

  if (isset($validation_results) && $override == FALSE) {
    $validation_results = $validation_results && TRUE;
  }
  else {
    $validation_results[$commerce_coupon->coupon_id] = TRUE;
  }

}

function commerce_coupon_action_is_invalid_coupon() {
  $validation_results = &drupal_static('commerce_coupon_action_validation_results');
  $validation_results = FALSE;
}

function commerce_coupon_action_set_granted_amount($commerce_coupon_log, $amount, $currency_code) {
  if (!($commerce_coupon_log instanceof EntityMetadataWrapper)) {
    $commerce_coupon_log = entity_metadata_wrapper('commerce_coupon_log', $commerce_coupon_log);
  }

  $coupon = commerce_coupon_load($commerce_coupon_log->coupon_id->value());

  // Set the unit price on the line item object.
  $commerce_coupon_log->commerce_granted_amount->amount = $amount;
  $commerce_coupon_log->commerce_granted_amount->currency_code = $currency_code;


  // Add the base price to the components array.
  if (!commerce_price_component_load($commerce_coupon_log->commerce_granted_amount->value(), 'commerce_coupon_' . $coupon->type)) {
    $commerce_coupon_log->commerce_granted_amount->data = commerce_price_component_add(
      $commerce_coupon_log->commerce_granted_amount->value(),
      'commerce_coupon_' . $coupon->type,
      $commerce_coupon_log->commerce_granted_amount->value(),
      TRUE,
      FALSE
    );
  }

  commerce_coupon_log_save($commerce_coupon_log->value());


}

function commerce_coupon_action_create_coupon_line_item($commerce_coupon, $commerce_order, $amount, $currency_code) {

  if (!($commerce_order instanceof EntityMetadataWrapper)) {
    $commerce_order = entity_metadata_wrapper('commerce_order', $commerce_order);
  }


  $line_item = commerce_coupon_line_item_new($commerce_coupon, $commerce_order->order_id->value());

  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

  // Set the unit price on the line item object.
  $line_item_wrapper->commerce_unit_price->amount = $amount * -1;
  $line_item_wrapper->commerce_unit_price->currency_code = $currency_code;


  // Add the base price to the components array.
  if (!commerce_price_component_load($line_item_wrapper->commerce_unit_price->value(), 'commerce_coupon_' . $commerce_coupon->type)) {
    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
      $line_item_wrapper->commerce_unit_price->value(),
      'commerce_coupon_' . $commerce_coupon->type,
      $line_item_wrapper->commerce_unit_price->value(),
      TRUE,
      FALSE
    );
  }

  commerce_line_item_save($line_item);

  $commerce_order->commerce_line_items[] = $line_item;
  $commerce_order->save();

  return array('commerce_coupon_line_item' => $line_item);
}


function commerce_coupon_action_get_coupon_uses($commerce_coupon) {
  return array('number_of_uses' => commerce_coupon_get_number_of_uses($commerce_coupon->coupon_id));
}
