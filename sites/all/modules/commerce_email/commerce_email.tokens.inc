<?php

/**
 * @file
 * Defines additional tokens for order email.
 */

/**
 * Implements hook_token_info().
 */
function commerce_email_token_info() {
  $type = array(
    'name' => t('Orders Email', array(), array('context' => 'a drupal commerce order')),
    'description' => t('Tokens related to individual orders.'),
    'needs-data' => 'commerce-order',
  );

  // Tokens for orders.
  $order = array();
  
  $order['customer-name'] = array(
    'name' => t('Name from Customer Profile'),
    'description' => t('Full name of customer'),
  );
  
  $order['order-items'] = array(
    'name' => t('Order Items'),
    'description' => t('A table containing order items.'),
  );

  return array(
    'types' => array('commerce-email' => $type),
    'tokens' => array('commerce-email' => $order),
  );
}

/**
 * Implements hook_tokens().
 */
function commerce_email_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'commerce-email' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'order-items':
          $replacements[$original] = $order->order_items;
          break;
        case 'customer-name':
          $replacements[$original] = $order->customer_name;
          break;
      }
    }
  }
  return $replacements;
}