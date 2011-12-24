<?php

/**
 * Flat rate commerce shipping quote plugin.
 */
class CommerceShippingFlatRate extends CommerceShippingQuote {
  public function settings_form(&$form, $rules_settings) {
    $currencies = commerce_currencies(TRUE);

    $form['shipping_price'] = array(
      '#type' => 'textfield',
      '#title' => t('Shipping rate (@code)', array('@code' => variable_get('commerce_default_currency', 'USD'))),
      '#description' => t('Configure what the rate should be.'),
      '#default_value' => is_array($rules_settings) && isset($rules_settings['shipping_price']) ? $rules_settings['shipping_price'] : 42,
      '#element_validate' => array('rules_ui_element_decimal_validate'),
      '#weight' => 0,
    );

    if (count($currencies) > 1) {
      $form['shipping_price']['#title'] = t('Default rate');
      $form['shipping_price']['#description'] = t("Note multi currency on a single order doesn't work well with commerce yet without custom coding.");
      $form['shipping_rates'] = array(
        '#type' => 'fieldset',
        '#title' => t('Shipping rates'),
        '#collapsed' => TRUE,
        '#weight' => 10,
      );

      foreach ($currencies as $currency_code => $currency) {
        $form['shipping_rates'][$currency_code][$currency_code] = array(
          '#type' => 'textfield',
          '#title' => t('Shipping rate (@code)', array('@code' => $currency_code)),
          '#description' => t('Configure what the rate should be.'),
          '#default_value' => is_array($rules_settings) && isset($rules_settings['shipping_rates'][$currency_code]) ? $rules_settings['shipping_rates'][$currency_code][$currency_code] : 42,
          '#element_validate' => array('rules_ui_element_decimal_validate'),
        );
      }
    }

    $form['rate_type'] = array(
      '#type' => 'select',
      '#title' => t('Rate type'),
      '#description' => t('Select what should be counted when calculating the shipping quote.'),
      '#default_value' => is_array($rules_settings) && isset($rules_settings['rate_type']) ? $rules_settings['rate_type'] : 'product',
      '#options' => array(
        'product' => t('Product'),
        'line_item' => t('Product types (line items)'),
        'order' => t('Order'),
      ),
      '#weight' => 100,
    );

    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Line item label'),
      '#default_value' => is_array($rules_settings) && isset($rules_settings['label']) ? $rules_settings['label'] : t('Flat rate shipping'),
      '#weight' => 101,
    );

    // Add tax-related elements if needed.
    $this->tax_form($form, $rules_settings);
  }

  private function tax_form(&$form, $rules_settings) {
    if (!module_exists('commerce_tax') || count($this->tax_inclusive_types()) == 0) {
      return;
    }

    $options = array('_none' => t('- None -')) + $this->tax_inclusive_types();
    $form['include_tax'] = array(
      '#type' => 'select',
      '#title' => t('Include tax in this rate'),
      '#description' => t('Saving rates tax inclusive will bypass later calculations for the specified tax.'),
      '#options' => count($options) == 1 ? reset($options) : $options,
      '#default_value' => is_array($rules_settings) && isset($rules_settings['include_tax']) ? $rules_settings['include_tax'] : '_none',
      '#required' => FALSE,
      '#weight' => 1,
    );

    $currencies = commerce_currencies(TRUE);
    if (count($currencies) > 1) {
      foreach ($currencies as $currency_code => $currency) {
        $form['shipping_rates'][$currency_code]['include_tax'] = array(
          '#type' => 'select',
          '#title' => t('Include tax in this rate'),
          '#description' => t('Saving rates tax inclusive will bypass later calculations for the specified tax.'),
          '#options' => count($options) == 1 ? reset($options) : $options,
          '#default_value' => is_array($rules_settings) && isset($rules_settings['shipping_rates'][$currency_code]['include_tax']) ? $rules_settings['shipping_rates'][$currency_code]['include_tax'] : '_none',
          '#required' => FALSE,
        );
      }
    }
  }

  /**
   * @todo Make this a function in commerce_tax.module.
   * Originally taken from commerce_tax_field_attach_form().
   */
  private function tax_inclusive_types() {
    // Build an array of tax types that are display inclusive.
    $inclusive_types = array();

    foreach (commerce_tax_types() as $name => $tax_type) {
      if ($tax_type['display_inclusive']) {
        $inclusive_types[$name] = $tax_type['title'];
      }
    }

    // Build an options array of tax rates of these types.
    $options = array();
    foreach (commerce_tax_rates() as $name => $tax_rate) {
      if (in_array($tax_rate['type'], array_keys($inclusive_types))) {
        $options[$inclusive_types[$tax_rate['type']]][$name] = t('Including @title', array('@title' => $tax_rate['title']));
      }
    }
    return $options;
  }

  public function calculate_quote($currency_code, $form_values = array(), $order = NULL, $pane_form = NULL, $pane_values = NULL) {
    if (empty($order)) {
      $order = $this->order;
    }
    $settings = $this->settings;
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    if (isset($settings['shipping_rates'][$currency_code]) && $settings['shipping_rates'][$currency_code]) {
      $amount = $settings['shipping_rates'][$currency_code][$currency_code];
      $tax_type = !empty($settings['shipping_rates'][$currency_code]['include_tax']) ? $settings['shipping_rates'][$currency_code]['include_tax'] : '_none';
    }
    else {
      $amount = $settings['shipping_price'];
      $tax_type = !empty($settings['include_tax']) ? $settings['include_tax'] : '_none';
    }

    $amount = commerce_currency_decimal_to_amount($amount, $currency_code);
    if ($tax_type != '_none' && module_exists('commerce_tax') && $tax_rate = commerce_tax_rate_load($tax_type)) {
      // Create base price component
      $price = array(
        'amount' => $amount,
        'currency_code' => $currency_code,
        'data' => array(),
      );
      // Add quote component part.
      $quote = $price;
      $quote['amount'] = $price['amount'] / (1 + $tax_rate['rate']);
      $price['data'] = commerce_price_component_add(
        $price,
        'quote',
        $quote,
        TRUE,
        FALSE
      );
      // Add tax component part.
      $tax = $quote;
      $tax['amount'] = $price['amount'] - $quote['amount'];
      $price['data'] = commerce_price_component_add(
        $price,
        $tax_rate['price_component'],
        $tax,
        TRUE,
        FALSE
      );
    }

    $quantity = 0;
    foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
      if ($line_item_wrapper->type->value() == 'product') {
        if ($settings['rate_type'] == 'product') {
          $quantity += $line_item_wrapper->quantity->value();
        }
        elseif ($settings['rate_type'] == 'line_item') {
          $quantity += 1;
        }
        elseif ($settings['rate_type'] == 'order') {
          $quantity = 1;
        }
      }
    }
    $line_item_data = array(
      'label' => $settings['label'],
      'quantity' => $quantity,
    );
    if (!empty($price)) {
      $line_item_data['price'] = $price;
    }
    else {
      $line_item_data['amount'] = $amount;
    }
    $shipping_line_items = array();
    $shipping_line_items[] = $line_item_data;
    return $shipping_line_items;
  }
}
