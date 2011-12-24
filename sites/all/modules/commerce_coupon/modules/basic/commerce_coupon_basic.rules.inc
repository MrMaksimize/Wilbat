<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_coupon_basic_rules_action_info() {
  $actions = array();


  $actions['commerce_coupon_basic_apply_to_item_line'] = array(
    'label' => t('Apply a percentage coupon to a line item'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
      'coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Coupon'),
      ),
    ),
    'base' => 'commerce_coupon_basic_apply_percentage_coupon_to_item_line',
    'group' => t('Commerce Coupon'),
  );

  return $actions;
}


function commerce_coupon_basic_apply_percentage_coupon_to_item_line($line_item, $coupon) {

  $amounts = &drupal_static(__FUNCTION__);

  if (!($coupon instanceof EntityMetadataWrapper)) {
    $coupon = entity_metadata_wrapper('commerce_coupon', $coupon);
  }

  $fields = $coupon->getPropertyInfo();
  if (isset($fields['commerce_coupon_percent_amount'])
      && $coupon->commerce_coupon_percent_amount->value() > 0
      ) {
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

    $rate = $coupon->commerce_coupon_percent_amount->value();
    if ($rate > 1) {
      // Ensure that the rate is never bigger then 100%
      $rate = $rate / 100;
    }

    // Calculate the total granted amount for this line item
    $amount = $line_item_wrapper->commerce_total->amount->value() * $rate * -1;

    // Save the amount for later use:
    $amounts[$line_item->order_id][$coupon->coupon_id->value()][$line_item->line_item_id] = $amount;

    // Sum up all granted amounts for this coupon on this order:
    $total_amount = 0;
    foreach ($amounts[$line_item->order_id][$coupon->coupon_id->value()] as $line_amount) {
      $total_amount += $line_amount;
    }

    // Load the coupon log for updating the granted amount
    $log = commerce_coupon_log_load_by_order_and_coupon($line_item->order_id, $coupon->coupon_id->value());
    $commerce_coupon_log = entity_metadata_wrapper('commerce_coupon_log', $log);

    // Set the unit price on the line item object.
    $commerce_coupon_log->commerce_granted_amount->amount = -1 * $total_amount;
    $commerce_coupon_log->commerce_granted_amount->currency_code = $line_item_wrapper->commerce_total->currency_code->value();


    // Add the base price to the components array.
    if (!commerce_price_component_load($commerce_coupon_log->commerce_granted_amount->value(), 'commerce_coupon_' . $coupon->type->value())) {
      $commerce_coupon_log->commerce_granted_amount->data = commerce_price_component_add(
        $commerce_coupon_log->commerce_granted_amount->value(),
        'commerce_coupon_' . $coupon->type->value(),
        $commerce_coupon_log->commerce_granted_amount->value(),
        TRUE,
        FALSE
      );
    }

    commerce_coupon_log_save($commerce_coupon_log->value());
  }
}

