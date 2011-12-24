<?php

/**
 * @file
 * Default rule configurations for Commerce Email.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_email_default_rules_configuration() {
  
  // Store the customer profile entity info for use in default rules.
  $customer_profile_entity_info = entity_get_info('commerce_customer_profile');
  
  $rules = array();
  
  // Add a reaction rule to send order e-mail upon checkout completion.
  $rule = rules_reaction_rule();

  $rule->label = t('Send an order notification e-mail (HTML)');
  $rule->active = TRUE;
  
  $rule
    ->event('commerce_checkout_complete')
    ->action('commerce_email_order_email', array(
      'commerce_order:select' => 'commerce-order', 
    ));
    
  $rule->weight = 9;

  $rules['commerce_checkout_order_email_html'] = $rule;
  
  /**
   * Add rule for sending html email for account
   */
  $rule = rules_reaction_rule();

  $rule->label = t('Create a new account for an anonymous order (HTML)');
  $rule->active = TRUE;

  $rule
    ->event('commerce_checkout_complete')
    ->condition('data_is', array(
      'data:select' => 'commerce-order:uid',
      'op' => '==',
      'value' => '0',
    ))
    ->condition(rules_condition('entity_exists', array(
      'type' => 'user',
      'property' => 'mail',
      'value:select' => 'commerce-order:mail',
    ))->negate())
    ->condition('data_is', array(
      'data:select' => 'commerce-order:type',
      'op' => '==',
      'value' => 'commerce_order',
    ))
    ->action('entity_create', array(
      'type' => 'user',
      'param_name:select' => 'commerce-order:mail',
      'param_mail:select' => 'commerce-order:mail',
      'entity_created:label' => t('Created account'),
      'entity_created:var' => 'account_created',
    ))
    ->action('data_set', array(
      'data:select' => 'account-created:status',
      'value' => 1,
    ))
    ->action('entity_save', array(
      'data:select' => 'account-created',
      'immediate' => 1,
    ))
    ->action('entity_query', array(
      'type' => 'user',
      'property' => 'mail',
      'value:select' => 'commerce-order:mail',
      'limit' => 1,
      'entity_fetched:label' => t('Fetched account'),
      'entity_fetched:var' => 'account_fetched',
    ));

  // Build a loop that send the account notification e-mail and updates the
  // order and customer profile uids with the uid from the fetched user account.
  $loop = rules_loop(array(
    'list:select' => 'account-fetched',
    'item:var' => 'list_item',
    'item:label' => t('Current list item'),
    'item:type' => 'user',
  ))
    ->action('commerce_email_account_email', array(
      'account:select' => 'list-item'
    ))
    ->action('data_set', array(
      'data:select' => 'commerce-order:uid',
      'value:select' => 'list-item:uid',
    ));

  // Accommodate any profile types referenced by the order.
  foreach ($customer_profile_entity_info['bundles'] as $type => $data) {
    $loop
      ->action('data_set', array(
        'data:select' => 'commerce-order:' . strtr('commerce-customer-' . $type, '_', '-') . ':uid',
        'value:select' => 'list-item:uid',
      ));
  }

  // Add the loop to the rule as an action.
  $rule->action($loop);

  // Adjust the weight so this rule executes after the one checking for a pre-
  // existing user account.
  $rule->weight = 2;

  $rules['commerce_email_new_account'] = $rule;  

  return $rules;
}