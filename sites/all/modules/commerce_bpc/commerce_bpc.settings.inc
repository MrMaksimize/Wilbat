<?php
/**
 * @file
 * Helper functions for settings-related functionality.
 */
/**
 * Returns default values for all commerce_bpc settings.
 *
 * @return
 *   An array keyed by settings-group labels, whose values are arrays whose
 *   key/value pairs specify default values for the settings in this group.
 */
function commerce_bpc_setting_defaults() {
  $settings = module_invoke_all('commerce_bpc_setting_defaults');

  // Defaults for patterns.
  $settings['default'] = array(
    'sku_pattern' => '[bulk_defaults:entered_sku]--[bulk_defaults:combination_values]',
    'sku_separator' => '-',
    'title_pattern' => '[bulk_defaults:entered_title] ([bulk_defaults:combination_labels])',
    'title_separator' => ', ',
  );

  return $settings;
}
/**
 * Retrieves a value form the variable table.
 *
 * @param $group
 *   The settings group of the setting to be retrieved.
 * @param $varname
 *  The name of the variable to retrieve.
 * @param $type
 *   (optional) if set to the machine name of a product type, the function
 *   will check if a product type specific version of the value exists (i.e.
 *   a variable with the name '$varname . '_' . $product_type).
 *
 * @return
 *  If $product_type is set and a product type specific value exists, it
 *  it will be returned. Otherwise, the non-specific value will be returned.
 *  failing that, a default value will be returned.
 */
function commerce_bpc_setting($group, $varname, $type = NULL) {
  // We short-circuit the look-up of defaults and global settings for 'override'
  // settings, for in this case, they do not make sense.
  if ($varname == 'override') {
    return variable_get('commerce_bpc_' . $group . '_override_' . $type, FALSE);
  }
  $defaults = commerce_bpc_setting_defaults();
  $value = isset($defaults[$group][$varname]) ? $defaults[$group][$varname] : NULL;
  $varname = 'commerce_bpc_' . $group . '_' . $varname;
  $value = variable_get($varname, $value);
  if (!empty($type) && commerce_bpc_setting($group, 'override', $type)) {
    $value = variable_get($varname . '_' . $type, $value);
  }
  return $value;
}
/**
 * Sets a variable value.
 *
 * @param $group
 *   String specifying the settings group this variable belongs to.
 * @param $varname
 *   The name of the variable.
 * @param $value
 *   The value of the variable.
 * @param $type
 *   (optional) The product type for which the value is being set. Pass NULL in
 *   order to set the global value.
 */
function commerce_bpc_setting_set($group, $varname, $value, $type = NULL) {
  $varname = 'commerce_bpc_' . $group . '_' . $varname;
  if (!empty($type)) {
    $varname .= '_' . $type;
  }
  variable_set($varname, $value);
}
/**
 * BPC-specific wrapper for system_settings_form.
 */
function commerce_bpc_settings_form($form, $settings_group, $product_type = NULL) {
  $form['settings_group'] = array(
    '#type' => 'value',
    '#value' => $settings_group,
  );

  if (!empty($product_type)) {
    $form['product_type'] = array(
      '#type' => 'value',
      '#value' => $product_type
    );
    // For product-type specific forms, we add a checkbox for overriding,
    // and hide all form elements if it is unchecked.
    foreach ($form as $key => $element) {
      $form[$key]['#states']['visible'][':input[name="override"]'] = array('checked' => TRUE);
    }
    $types = commerce_product_types();
    $form['override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override settings for type %label', array('%label' => $types[$product_type]['name'])),
      '#default_value' => commerce_bpc_setting($settings_group, 'override', $product_type),
      '#weight' => -1,
    );
  }
  $form = system_settings_form($form);
  // Override the submit handler, so we can do pre- and suffixing.
  $form['#submit'] = array('commerce_bpc_settings_form_submit');
  return $form;
}
function commerce_bpc_settings_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $fv = &$form_state['values'];

  $group = $fv['settings_group'];
  unset($fv['settings_group']);

  $type = isset($fv['product_type']) ? $fv['product_type'] : NULL;
  unset($fv['product_type']);

  foreach ($fv as $key => $value) {
    commerce_bpc_setting_set($group, $key, $value, $type);
  }
  drupal_set_message(t('The configuration options have been saved.'));

}