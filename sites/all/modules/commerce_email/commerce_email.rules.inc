<?php

/**
 * @file
 * Defines rules for order email functionality.
 */

 /**
 * Implements hook_rules_action_info().
 */
function commerce_email_rules_action_info() {
  $actions = array();

  $actions['commerce_email_order_email'] = array(
    'label' => t('Send order email (HTML)'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      )
    ),
    'group' => t('Commerce Email'),
    'callbacks' => array(
      'execute' => 'commerce_email_order_email_send',
    ),
  );
  
  $actions['commerce_email_account_email'] = array(
    'label' => t('Send account e-mail (HTML)'),
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('Account'),
      )
    ),
    'group' => t('Commerce Email'),
    'callbacks' => array(
      'execute' => 'commerce_email_account_email_send',
    ),
  );
  
  return $actions;
}

/**
 * Rules action: Send the users order as an HTML email
 */
function commerce_email_order_email_send($order) {

  global $language, $base_url;
  $site_email = variable_get('site_mail', '');
  $commerce_email_template = variable_get('commerce_email_order_template_' . $language->language, 0);
  $site = array(
      'site_name' => variable_get('site_name', "Commerce Email (default)"),
      'site_url' => $base_url
  );
  
  // Load email content
  list($subject, $content) = commerce_email_load('order');
  
  $message = array(
    'id' => 'commerce_email_order',
    'module' => 'commerce_email',
    'key' => 'order',
    'to' => $order->mail,
    'from' => $site_email,
    'subject' => token_replace($subject, array('commerce-order' => $order)),
    'body' => array(''),
    'headers' => array(
      'Content-Type' => 'text/html; charset=UTF-8; format=flowed',
      'From' => $site_email,
      'Sender' => $site_email,
      'Return-Path' => $site_email
    )
  );
  
  // Order and Customer details
  $customer_profile = commerce_email_customer_profile($order);
  commerce_email_customer_name($order, $customer_profile);
  
  if ($commerce_email_template == 1) {
    $order->order_items = commerce_email_order_items($order, FALSE);
    $message['body'] = theme('commerce_order_email', array('site' => $site, 'order' => $order, 'customer_profile' => $customer_profile, 'language' => $language->language));
  }
  else {
    $order->order_items = commerce_email_order_items($order);
    $message['body'] = token_replace($content, array('commerce-order' => $order, 'commerce-email' => $order, 'commerce-customer-profile' => $customer_profile), array('sanitize' => FALSE));
  }

  //Send user order email
  commerce_email_mailsystem_send('order', $message);
  
  // Send admin order email
  $admin_email = variable_get('admin_email', array());
  if (!empty($admin_email['enabled'])) {
    
    $commerce_email_template = variable_get('commerce_email_admin_order_template_' . $language->language, 0);
    
    // Load email content
    list($subject, $content) = commerce_email_load('admin_order');
    $recipient = (empty($admin_email['email_address'])) ? $site_email : $admin_email['email_address'];
    
    $message = array(
      'id' => 'commerce_email_admin_order',
      'module' => 'commerce_email',
      'key' => 'admin_order',
      'to' => $recipient,
      'from' => $site_email,
      'subject' => token_replace($subject, array('commerce-order' => $order)),
      'body' => array(''),
      'headers' => array(
        'Content-Type' => 'text/html; charset=UTF-8; format=flowed',
        'From' => $site_email,
        'Sender' => $site_email,
        'Return-Path' => $site_email
      )
    );
    
    if ($commerce_email_template == 1) {
      $order->order_items = commerce_email_order_items($order, FALSE);
      $message['body'] = theme('commerce_admin_order_email', array('site' => $site, 'order' => $order, 'customer_profile' => $customer_profile, 'language' => $language->language));
    }
    else {
      $order->order_items = commerce_email_order_items($order);
      $message['body'] = token_replace($content, array('commerce-order' => $order, 'commerce-email' => $order, 'commerce-customer-profile' => $customer_profile), array('sanitize' => FALSE));
    }
    
    // Send admin a copy of the user order email
    commerce_email_mailsystem_send('admin_order', $message);   
  }
}

function commerce_email_account_email_send($account) {
  
  global $language;
  $site_email = variable_get('site_mail', '');
  $commerce_email_template = variable_get('commerce_email_account_template_' . $language->language, 0);
  $variables = array('user' => $account);
  
  // Load email content
  list($subject, $content) = commerce_email_load('account');
  
  $message = array(
    'id' => 'commerce_email_account',
    'module' => 'commerce_email',
    'key' => 'email_account',
    'to' => $account->mail,
    'from' => $site_email,
    'subject' => token_replace($subject, $variables, array('language' => $language, 'callback' => 'user_mail_tokens', 'sanitize' => FALSE)),
    'body' => array(''),
    'headers' => array(
      'Content-Type' => 'text/html; charset=UTF-8; format=flowed',
      'From' => $site_email,
      'Sender' => $site_email,
      'Return-Path' => $site_email
    )
  );

  if ($commerce_email_template==1) {
    global $base_url;
    $login = user_pass_reset_url($variables['user']);
    $site = array(
      'site_name' => variable_get('site_name', "Commerce Email (default)"),
      'site_url' => $base_url
    );
    
    $message['body'] = theme('commerce_account_email', array('site' => $site, 'user' => $variables['user'], 'login' => $login, 'language' => $language->language));
  }
  else {
    $message['body'] = token_replace($content, $variables, array('language' => $language, 'callback' => 'user_mail_tokens', 'sanitize' => FALSE));
  }

  // Send user an account email
  commerce_email_mailsystem_send('account', $message);
}
