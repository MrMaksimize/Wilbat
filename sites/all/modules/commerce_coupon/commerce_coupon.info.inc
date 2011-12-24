<?php

/**
 * @file
 * Provides metadata for the coupon entity.
 */

/**
 * Implements hook_entity_property_info() of entity module
 * @see http://drupal.org/node/1021466
 */
function commerce_coupon_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_order properties.
  $properties = &$info['commerce_coupon']['properties'];

  $properties['coupon_id'] = array(
    'type' => 'integer',
    'label' => t('Coupon ID', array(), array('context' => 'a drupal commerce coupon')),
    'description' => t('The internal numeric ID of the coupon.'),
    'schema field' => 'coupon_id',
  );

  $properties['type'] = array(
    'type' => 'commerce_coupon_type',
    'label' => t('Type'),
    'description' => t('The human readable name of the coupon type.'),
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_coupon_type_options_list',
    'required' => FALSE,
    'schema field' => 'type',
  );

  // Add meta-data about the basic commerce_order properties.
  $properties = &$info['commerce_coupon_log']['properties'];

  $properties['log_id'] = array(
    'type' => 'integer',
    'label' => t('Log ID', array(), array('context' => 'a drupal commerce coupon')),
    'description' => t('The internal numeric ID of the coupon.'),
    'schema field' => 'log_id',
  );

  $properties['coupon'] = array(
    'type' => 'commerce_coupon',
    'label' => t('Coupon', array(), array('context' => 'a drupal commerce coupon')),
    'description' => t('The associated coupon.'),
    'schema field' => 'coupon_id',
  );

  $properties['user'] = array(
    'type' => 'user',
    'label' => t('User'),
    'description' => t('The user whose redeemed the coupon.'),
    'schema field' => 'uid',
  );

  $properties['order'] = array(
    'type' => 'commerce_order',
    'label' => t('Order'),
    'description' => t('The order on which this coupon log relies on.'),
    'schema field' => 'order_id',
  );

  $properties['is_accepted'] = array(
    'type' => 'integer',
    'options list' => 'entity_metadata_status_options_list',
    'label' => t('Is Accepted'),
    'description' => t('Indicates if this coupon is accepted in this context (order / coupon).'),
    'schema field' => 'is_accepted',
  );

  $properties['is_processed'] = array(
    'type' => 'boolean',
    'label' => t('Is Processed'),
    'description' => t('If this coupon log is processed, this means it is final / the order is complete. It can not be removed any more.'),
    'schema field' => 'is_processed',
  );

  $properties['created'] = array(
    'type' => 'date',
    'label' => t('Creation date'),
    'description' => t('When the log entry was created.'),
    'schema field' => 'created',
  );

  $properties['changed'] = array(
    'type' => 'date',
    'label' => t('Changed date'),
    'description' => t('When the log entry was last updated.'),
    'schema field' => 'changed',
  );

  return $info;
}
