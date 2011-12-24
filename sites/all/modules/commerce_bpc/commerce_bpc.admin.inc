<?php
/**
 * @file
 * Page generation callbacks the Commerce Bulk Product Creation settings
 * pages.
 */

/**
 * Form constructor for the commerce_bpc settings pages.
 *
 * Paths:
 * - admin/commerce/config/commerce_bpc
 * - admin/commerce/config/commerce_bpc/patterns
 * - admin/commerce/products/types/PRODUCT_TYPE/commerce_bpc
 * - admin/commerce/products/types/PRODUCT_TYPE/commerce_bpc/patterns
 *
 * @see commerce_bpc_menu()
 * @see commetce_bpc_settings_form()
 * @ingroups forms
 */
function commerce_bpc_pattern_settings_form($form, &$form_state, $type = NULL) {
  $form['patterns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Patterns'),
    '#description' => t('Specify the patterns used for the generation of SKUs and titles during bulk creation.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
    '#tree' => FALSE,
  );

  $form['patterns']['sku_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU Pattern'),
    '#description' => t('Enter the pattern that should be used to generate SKUs. You should use the patterns below.'),
    '#size' => 160,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'sku_pattern', $type),
  );
  $form['patterns']['sku_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for combination field values in SKU'),
    '#description' => t('This string will be used to separate the values of various combination field. Thus if there are two combination fields "size" and "color", and for a particular combination, those have the values "M" and "red", respectively, with a separator value of "-", the string returned by [bulk_defaults:sku_combination_values] will be "M-red".'),
    '#size' => 3,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'sku_separator', $type),
  );

  $form['patterns']['title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Title pattern'),
    '#description' => t('Enter the pattern that should be used to generate titles. You should use the patterns below.'),
    '#size' => 160,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'title_pattern', $type),
  );

  $form['patterns']['title_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for combination field labels in titles'),
    '#description' => t('This string will be used to separate the value-labels of various combination fields. Thus if there are two combination fields "size" and "color", and for a particular combination, those have the values "Medium" and "red", respectively, with a separator value of ", ", the string returned by [commerce_bpc:default_title_combination_values] will be "Medium, red".'),
    '#size' => 3,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'title_separator', $type),
  );
  $bpc_tokens = commerce_bpc_token_info();


  $form['tokens'] = _commerce_bpc_show_tokens('bulk_defaults', $bpc_tokens['tokens']['bulk_defaults']);
  if ($type) {
    $product_type_tokens = commerce_bpc_token_info_by_product_type($type);
    $form['product_tokens'] = _commerce_bpc_show_tokens('bulk_product', $product_type_tokens);
  }


  return commerce_bpc_settings_form($form, 'default', $type);
}

