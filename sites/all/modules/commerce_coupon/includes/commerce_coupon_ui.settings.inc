<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

function commerce_coupon_ui_settings_form($form, &$form_state) {
  $form['commerce_coupon_default_code_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon Code Length'),
    '#default_value' => variable_get('commerce_coupon_default_code_size', 8),
    '#description' => t('Set the coupon code length for auto generated codes.'),
  );

  return system_settings_form($form);
}
