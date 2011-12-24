<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

/**
 * Payment pane: form callback.
 */
function commerce_coupon_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // Store the payment methods in the form for validation purposes.
  $pane_form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon Code'),
    '#description' => t('Enter here your coupon code.'),
  );

  // Extract the View and display keys from the cart contents pane setting.
  list($view_id, $display_id) = explode('|', variable_get('commerce_coupon_review_pane_view', 'commerce_coupon_review_pane|default'));
  if (!empty($view_id) && !empty($display_id)) {
    $pane_form['redeemed_coupons'] = array(
      '#type' => 'markup',
      '#markup' => commerce_embed_view($view_id, $display_id, array($order->order_id)),
    );
  }

  return $pane_form;
}

function commerce_coupon_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  //@todo I think global function should start and end with an underscore
  global $commerce_coupon_validation_error_message;

  // Check if it is empty
  if (empty($form_state['values']['commerce_coupon']['coupon_code'])) {
    return TRUE;
  }

  // Valdiate the coupon code
  if (commerce_coupon_code_is_valid($form_state['values']['commerce_coupon']['coupon_code'], $order)) {
    return TRUE;
  }
  else {
    if (isset($commerce_coupon_validation_error_message)) {
      $message = $commerce_coupon_validation_error_message;
    }
    else {
      $message = t('Your coupon code is not valid.');
    }

    form_set_error('commerce_coupon][coupon_code', $message);
    return FALSE;
  }

}


function commerce_coupon_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $code = $form_state['values']['commerce_coupon']['coupon_code'];

  $coupon = commerce_coupon_load_by_code($code);
  commerce_coupon_redeem_coupon($coupon, $order);
}



/**
 * Implements the callback for the checkout pane review form
 */
function commerce_coupon_pane_review($form, $form_state, $checkout_pane, $order) {

  // Extract the View and display keys from the cart contents pane setting.
  list($view_id, $display_id) = explode('|', variable_get('commerce_coupon_review_pane_view', 'commerce_coupon_review_pane|default'));

  return commerce_embed_view($view_id, $display_id, array($order->order_id));
}



/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function commerce_coupon_pane_settings_form($checkout_pane) {
  $form = array();

  // Build an options array of Views available for the cart contents pane.
  $options = array();

   // Generate an option list from all user defined and module defined views.
   foreach (views_get_all_views() as $view_id => $view_value) {
    // Only include line item Views.
    if ($view_value->base_table == 'commerce_coupon_log') {
       foreach ($view_value->display as $display_id => $display_value) {
        $options[check_plain($view_id)][$view_id . '|' . $display_id] = check_plain($display_value->display_title);
      }
     }
   }

  $form['commerce_coupon_review_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Coupons Review Pane View'),
    '#description' => t('Specify the View to use in the review pane to display the coupons.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_coupon_review_pane_view', 'commerce_coupon_review_pane|default'),
  );

  $form['commerce_coupon_checkout_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Coupons Checkout Pane View'),
    '#description' => t('Specify the View to use in the checkout pane under the coupon form. Leave empty to do not display anything.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_coupon_checkout_pane_view', 'commerce_coupon_review_pane|default'),
  );

  return $form;
}

