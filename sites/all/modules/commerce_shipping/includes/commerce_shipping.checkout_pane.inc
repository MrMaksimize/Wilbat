<?php

/**
 * @file
 * Callback functions for the shipping module's checkout panes.
 */


/**
 * Checkout pane callback: returns the shipping service pane's settings form.
 */
function commerce_shipping_pane_settings_form($checkout_pane) {
  $form = array();

  $form['commerce_shipping_pane_require_service'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require a shipping service at all times, preventing checkout if none are available.'),
    '#default_value' => variable_get('commerce_shipping_pane_require_service', FALSE),
  );

  return $form;
}

/**
 * Checkout pane callback: builds a shipping quote selection form.
 */
function commerce_shipping_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // TODO: integrate with Commerce Physical Product to ensure an order contains
  // physical products before attempting to quote shipping rates.

  // Collect shipping rates for the order.
  commerce_shipping_collect_rates($order);

  // Generate an array of shipping service rate options.
  $options = commerce_shipping_service_rate_options($order);

  // If at least one shipping option is available...
  if (!empty($options)) {
    // Store the shipping methods in the form for validation purposes.
    $pane_form['shipping_rates'] = array(
      '#type' => 'value',
      '#value' => $order->shipping_rates,
    );

    // Add a radios element to let the customer select a shipping service.
    $pane_form['shipping_service'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#ajax' => array(
        'callback' => 'commerce_shipping_pane_service_details_refresh',
        'wrapper' => 'commerce-shipping-service-details',
      ),
    );

    // Find the default shipping method using either the pre-selected value
    // stored as a line item on the order or the first available method.
    $pane_values = !empty($form_state['values']) ? $form_state['values'][$checkout_pane['pane_id']] : array();
    $pane_values['service_details'] = array();

    // First check for a shipping service selection in the pane values.
    if (!empty($pane_values['shipping_service']) && !empty($options[$pane_values['shipping_service']])) {
      $default_value = $pane_values['shipping_service'];
    }
    else {
      // Then look for one in a line item on the order already.
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

      foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
        if ($line_item_wrapper->type->value() == 'shipping' && !empty($options[$line_item_wrapper->commerce_shipping_service->value()])) {
          $default_value = $line_item_wrapper->commerce_shipping_service->value();

          // If the service details value is empty, see if there are any values
          // in the line item to merge into the array as defaults.
          if (empty($pane_values['service_details'])) {
            $pane_values['service_details'] = $line_item_wrapper->value()->data['service_details'];
          }

          break;
        }
      }
    }

    // If we still don't have a default value, use the first available.
    if (empty($default_value)) {
      $default_value = key($options);
    }

    // Set the default value for the shipping method radios.
    $pane_form['shipping_service']['#default_value'] = $default_value;
    $shipping_service = commerce_shipping_service_load($default_value);

    $pane_form['service_details'] = array('#type' => 'container');

    // If the service specifies a details form callback...
    if ($callback = commerce_shipping_service_callback($shipping_service, 'details_form')) {
      // Look for a form array from the callback.
      $details_form = $callback($pane_form, $pane_values, $checkout_pane, $order, $shipping_service);

      // If a details form array was actually returned...
      if (!empty($details_form)) {
        // Add it to the form now.
        $pane_form['service_details'] += $details_form;
      }
    }

    $pane_form['service_details']['#prefix'] = '<div id="commerce-shipping-service-details">';
    $pane_form['service_details']['#suffix'] = '</div>';
  }
  elseif (variable_get('commerce_shipping_pane_require_service', FALSE)) {
    $pane_form['message'] = array(
      '#markup' => '<div>' . t('Unfortunately we could not calculate any valid shipping rates for your order, and we require shipping service selection to complete checkout.') . '<br /><strong>' . t('Please contact us to resolve any issues with your order.') . '</strong></div>',
    );

    $pane_form['shipping_rates'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
  }

  return $pane_form;
}

/**
 * Returns the shipping details element for display via AJAX.
 */
function commerce_shipping_pane_service_details_refresh($form, $form_state) {
  return $form['commerce_shipping']['service_details'];
}

/**
 * Checkout pane callback: validate the shipping service selection and details.
 */
function commerce_shipping_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];

  // Only attempt validation if we actually had shipping services on the form.
  if (!empty($form[$pane_id]) && !empty($form_state['values'][$pane_id])) {
    $pane_form = $form[$pane_id];
    $pane_values = $form_state['values'][$pane_id];

    // Initialize the extra details if necessary.
    if (empty($pane_values['service_details'])) {
      $pane_values['service_details'] = array();
    }

    // Only attempt validation if there were shipping services available.
    if ($pane_values['shipping_rates']) {
      // If the selected shipping service was changed...
      if ($pane_values['shipping_service'] != $pane_form['shipping_service']['#default_value']) {
        // And the newly selected service has a valid details form callback...
        if ($shipping_service = commerce_shipping_service_load($pane_values['shipping_service'])) {
          if (commerce_shipping_service_callback($shipping_service, 'details_form')) {
            // Fail validation so the form is rebuilt to include the shipping
            // service specific form elements.
            return FALSE;
          }
        }
      }

      // Allow the shipping service to validate the service details.
      $shipping_service = commerce_shipping_service_load($pane_values['shipping_service']);

      if ($callback = commerce_shipping_service_callback($shipping_service, 'details_form_validate')) {
        $result = $callback($pane_form['service_details'], $pane_values['service_details'], $shipping_service, $order, array($checkout_pane['pane_id'], 'service_details'));

        // To prevent payment method validation routines from having to return TRUE
        // explicitly, only return FALSE if it was specifically returned.  Otherwise
        // default to TRUE.
        return $result === FALSE ? FALSE : TRUE;
      }
    }
    elseif (variable_get('commerce_shipping_pane_require_service', FALSE)) {
      // But fail validation if no rates were returned and we require selection.
      drupal_set_message(t('You cannot continue checkout without selecting a valid shipping service. Please contact us if an error is preventing you from seeing valid shipping rates for your order.'), 'error');
      return FALSE;
    }
  }

  // Nothing to validate.
  return TRUE;
}

/**
 * Checkout pane callback: submit the shipping checkout pane.
 */
function commerce_shipping_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];

  // Only attempt validation if we actually had payment methods on the form.
  if (!empty($form[$pane_id]) && !empty($form_state['values'][$pane_id])) {
    $pane_form = $form[$pane_id];
    $pane_values = $form_state['values'][$pane_id];

    // Initialize the extra details if necessary.
    if (empty($pane_values['service_details'])) {
      $pane_values['service_details'] = array();
    }

    // Only submit if there were shipping services available.
    if (!empty($pane_values['shipping_rates'])) {
      $shipping_service = commerce_shipping_service_load($pane_values['shipping_service']);

      // Delete any existing shipping line items from the order.
      commerce_shipping_delete_shipping_line_items($order, TRUE);

      // Extract the unit price from the calculated rate.
      $rate_line_item = $pane_values['shipping_rates'][$pane_values['shipping_service']];
      $rate_line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $rate_line_item);
      $unit_price = $rate_line_item_wrapper->commerce_unit_price->value();

      // Create a new shipping line item with the calculated rate from the form.
      $line_item = commerce_shipping_line_item_new($pane_values['shipping_service'], $unit_price, $order->order_id, $rate_line_item->data, $rate_line_item->type);

      // Add the service details to the line item's data array and the order.
      $line_item->data['service_details'] = $pane_values['service_details'];

      // Allow the details form submit handler to make any necessary updates to
      // the line item before adding it to the order.
      if ($callback = commerce_shipping_service_callback($shipping_service, 'details_form_submit')) {
        $callback($pane_form['service_details'], $pane_values['service_details'], $line_item);
      }

      // Save and add the line item to the order.
      commerce_shipping_add_shipping_line_item($line_item, $order, TRUE);
    }
  }
}

/**
 * Checkout pane callback: show the selected shipping service on the review pane.
 */
function commerce_shipping_pane_review($form, $form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Loop over all the line items on the order.
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    // If the current line item is a shipping line item...
    if ($line_item_wrapper->type->value() == 'shipping') {
      // Return its label and formatted total price.
      $total_price = $line_item_wrapper->commerce_total->value();
      $rate = commerce_currency_format($total_price['amount'], $total_price['currency_code'], $line_item_wrapper->value());
      return t('@service: @rate', array('@service' => $line_item_wrapper->line_item_label->value(), '@rate' => $rate));
    }
  }
}
