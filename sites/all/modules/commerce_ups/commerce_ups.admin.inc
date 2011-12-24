<?php

// $Id$

/**
 * @file
 * Handles admin settings page for Commerce UPS module.
 */

/* hook_settings_form() */
function commerce_ups_settings_form($form, &$form_state) {
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('UPS API credentials'),
    '#collapsible' => TRUE,
    '#description' => t('In order to obtain shipping rate estimates, you must have an account with UPS. You can apply for UPS API credentials at '. l('UPS.com', 'https://www.ups.com/upsdeveloperkit', array('attributes' => array('target'=>'_blank')))),
  );

  $encrypted = variable_get('commerce_ups_encrypt', FALSE);
  $ups_accountid = $encrypted ? commerce_ups_decrypt(variable_get('commerce_ups_account_id', '')) : variable_get('commerce_ups_account_id', '');
  $ups_userid = $encrypted ? commerce_ups_decrypt(variable_get('commerce_ups_user_id', '')) : variable_get('commerce_ups_user_id', '');
  $ups_accesskey = $encrypted ? commerce_ups_decrypt(variable_get('commerce_ups_access_key', '')) : variable_get('commerce_ups_access_key', '');

  $form['api']['commerce_ups_account_id'] = array(
    '#type' => "textfield",
    '#title' => t("Account ID"),
    '#default_value' => $ups_accountid,
    '#required' => !variable_get('commerce_ups_account_id', FALSE),
  );
  $form['api']['commerce_ups_user_id'] = array(
    '#type' => "textfield",
    '#title' => t("User ID"),
    '#default_value' => $ups_userid,
    '#required' => !variable_get('commerce_ups_user_id', FALSE),
  );
  $form['api']['commerce_ups_password'] = array(
    '#type' => "password",
    '#title' => t("Password"),
    '#required' => !variable_get('commerce_ups_password', FALSE),
    '#description' => t('Please leave blank if you do not want to update your password at this time.'),
  );
  $form['api']['commerce_ups_access_key'] = array(
    '#type' => "textfield",
    '#title' => t("Access Key"),
    '#default_value' => $ups_accesskey,
    '#required' => !variable_get('commerce_ups_access_key', FALSE),
  );
  $form['api']['encryption'] = array(
    '#type' => 'item',
    '#title' => 'Encryption',
    'status' => array(
      '#type' => 'item',
      '#title' => FALSE,
    ),
  );
  if (commerce_ups_encryption_available(array('display_all' => TRUE))) {
    $form['api']['encryption']['status']['#markup'] =
      'Encryption is available and configured properly.';
    $form['api']['encryption']['commerce_ups_encrypt'] = array(
      '#type' => 'checkbox',
      '#title' => 'Encrypt UPS credentials (HIGHLY RECOMMENDED)',
      '#description' => 'Note: enabling this setting will not encrypt existing credentials.',
      '#default_value' => TRUE,
    );
  }
  else {
    $form['api']['encryption']['status']['#markup'] = '<span class="error">'. l('AES','http://drupal.org/project/aes', array('attributes' => array('target' => '_blank'))) .'is not installed - your login credentials will not be encrypted.</span>';
  }

  $form['origin'] = array(
    '#type' => "fieldset",
    '#title' => t("Ship From Address"),
    '#collapsible' => true
  );
  $form['origin']['commerce_ups_company_name'] = array(
    '#type' => "textfield",
    '#title' => t("Company Name"),
    '#value' => variable_get('commerce_ups_company_name')
  );
  $form['origin']['commerce_ups_address_line_1'] = array(
    '#type' => 'textfield',
    '#title' => t("Address"),
    '#value' => variable_get('commerce_ups_address_line_1')
  );
  $form['origin']['commerce_ups_address_line_2'] = array(
    '#type' => 'textfield',
    '#title' => t("Address (Additional)"),
    '#value' => variable_get('commerce_ups_address_line_2')
  );
  $form['origin']['commerce_ups_city'] = array(
    '#type' => 'textfield',
    '#title' => t("City"),
    '#value' => variable_get('commerce_ups_city')
  );
  $form['origin']['commerce_ups_state'] = array(
    '#type' => 'select',
    '#title' => t("State"),
    '#value' => variable_get('commerce_ups_state'),
    '#options' => array(
      '' => t('Please Select'),
      'AL' => t('Alabama'),
      'AK' => t('Alaska'),
      'AZ' => t('Arizona'),
      'AR' => t('Arkansas'),
      'CA' => t('California'),
      'CO' => t('Colorado'),
      'CT' => t('Connecticut'),
      'DE' => t('Delaware'),
      'DC' => t('District of Columbia'),
      'FL' => t('Florida'),
      'GA' => t('Georgia'),
      'HI' => t('Hawaii'),
      'ID' => t('Idaho'),
      'IL' => t('Illinois'),
      'IN' => t('Indiana'),
      'IA' => t('Iowa'),
      'KS' => t('Kansas'),
      'KY' => t('Kentucky'),
      'LA' => t('Louisiana'),
      'ME' => t('Maine'),
      'MD' => t('Maryland'),
      'MA' => t('Massachusetts'),
      'MI' => t('Michigan'),
      'MN' => t('Minnesota'),
      'MS' => t('Mississippi'),
      'MO' => t('Missouri'),
      'MY' => t('Montana'),
      'NE' => t('Nebraska'),
      'NV' => t('Nevada'),
      'NH' => t('New Hampshire'),
      'NJ' => t('New Jersey'),
      'NM' => t('New Mexico'),
      'NY' => t('New York'),
      'NC' => t('North Carolina'),
      'ND' => t('North Dakota'),
      'OH' => t('Ohio'),
      'OK' => t('Oklahoma'),
      'OR' => t('Oregon'),
      'PA' => t('Pennsylvania'),
      'RI' => t('Rhode Island'),
      'SC' => t('South Carolina'),
      'SD' => t('South Dakota'),
      'TN' => t('Tennessee'),
      'TX' => t('Texas'),
      'UT' => t('Utah'),
      'VT' => t('Vermont'),
      'VA' => t('Virginia'),
      'WA' => t('Washington'),
      'WV' => t('West Virginia'),
      'WI' => t('Wisconsin'),
      'WY' => t('Wyoming'),
    )
  );
  $form['origin']['commerce_ups_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t("Postal Code"),
    '#size' => 5,
    '#value' => variable_get('commerce_ups_postal_code')
  );
  $form['origin']['commerce_ups_country_code'] = array(
    '#type' => 'select',
    '#title' => t("Country"),
    '#value' => variable_get('commerce_ups_country_code'),
    '#options' => array(
      '' => t('Please Select'),
      'US' => t('United States of America'),
    )
  );
  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t("Enable UPS Shipping Services"),
    '#collapsible' => TRUE
  );

  foreach (_commerce_ups_service_list() as $key => $service) {
    $array_options[$key] = $service['title'];
  }
  $form['services']['commerce_ups_services'] = array(
    '#type' => 'checkboxes',
    '#options' => $array_options,
    '#default_value' => variable_get("commerce_ups_services", array())
  );
  $form['packaging'] = array(
    '#type' => 'fieldset',
    '#title' => t("Enable UPS Packaging Types"),
    '#collapsible' => FALSE
  );
  $form['packaging']['commerce_ups_packaging_explanation'] = array(
    '#type' => 'markup',
    '#markup' => 'Only "Customer supplied packaging" is currently available. Please enter the default package size below.',
  );
  $form['packaging']['commerce_ups_packaging'] = array(
    '#type' => 'hidden',
    '#value' => '02',
  );
  /*
  $form['packaging']['commerce_ups_packaging'] = array(
    '#type' => 'checkboxes',
    '#options' => _commerce_ups_packaging_types(),
    '#default_value' => variable_get("commerce_ups_packaging", array())
  );
  */
  // Fields for default package size (inches)
  $form['default_package_size'] = array(
    '#type' => 'fieldset',
    '#title' => t("Default package size (inches)"),
    '#collapsible' => FALSE,
    '#description' => 'UPS requires a package size when determining estimates.',
  );
  $form['default_package_size']['commerce_ups_default_package_size_length'] = array(
    '#type' => 'textfield',
    '#title' => t("Length"),
    '#size' => 5,
    '#required' => TRUE,
    '#value' => variable_get('commerce_ups_default_package_size_length'),
  );
  $form['default_package_size']['commerce_ups_default_package_size_width'] = array(
    '#type' => 'textfield',
    '#title' => t("Width"),
    '#size' => 5,
    '#required' => TRUE,
    '#value' => variable_get('commerce_ups_default_package_size_width'),
  );
  $form['default_package_size']['commerce_ups_default_package_size_height'] = array(
    '#type' => 'textfield',
    '#title' => t("Height"),
    '#size' => 5,
    '#required' => TRUE,
    '#value' => variable_get('commerce_ups_default_package_size_height'),
  );
  $form['default_package_size']['details'] = array(
    '#markup' => 'The package size is used to determine the number of packages necessary to
      create a UPS shipping cost estimate. <strong>If products do not have physical dimensions and
      weights associated with them, the estimates will not be accurate.</strong> The logic implemented works as:
      <ul>
      <li>Assume each order requires at least one package.</li>
      <li>Use the combined volume of all products in an order to determine the number of packages.</li>
      </ul>
      This is a simple calculation that can get close to actual shipping costs. More
      complex logic involving multiple package sizes, weights, and void space can be implemented via
      custom modules.',
  );
  $form['commerce_ups_pick_up_schedule'] = array(
    "#type" => "select",
    "#title" => t("Pick-up Schedule"),
    "#options" => _commerce_ups_pickup_types(),
    "#value" => variable_get("commerce_ups_pick_up_schedule")
  );
  $form['commerce_ups_show_logo'] = array(
    "#type" => "checkbox",
    "#title" => t("Show UPS Logo on Shipping Page"),
    "#default_value" => variable_get("commerce_ups_show_logo", 0),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/* hook_settings_form_validate() */
function commerce_ups_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  // If the Password field is empty, then they're not trying to update it and we should ignore it.
  if (empty($values['commerce_ups_password'])) {
    unset($form_state['values']['commerce_ups_password']);
    return;
  }

  /* If we are setting or resetting values, test the connection.
  $ups_connection = ups_api_function_here();
  if (is_good_connection($ups_connection)) {
    drupal_set_message(t('Connection established. UPS credentials updated.'));
  }
  else {
    drupal_set_message(
      t('Resetting UPS credentials failed.'), 'error');
    form_set_error('commerce_ups_account_id', t('Unable to connect to UPS. Please check your credentials.'));
    form_set_error('commerce_ups_user_id');
    form_set_error('commerce_ups_access_key');
    form_set_error('commerce_ups_password');
  }
  */
}

/*
 * Implements hook_form_submit().
 */
function commerce_ups_settings_form_submit($form, &$form_state) {
  // Encrypt the UPS API credentials if available
  if ($form_state['input']['commerce_ups_encrypt']) {
    $form_state['input']['commerce_ups_user_id'] = commerce_ups_encrypt($form_state['input']['commerce_ups_user_id']);
    $form_state['input']['commerce_ups_account_id'] = commerce_ups_encrypt($form_state['input']['commerce_ups_account_id']);
    $form_state['input']['commerce_ups_access_key'] = commerce_ups_encrypt($form_state['input']['commerce_ups_access_key']);
  }
  
  if (empty($form_state['input']['commerce_ups_password'])) {
    unset($form_state['input']['commerce_ups_password']);
  }
  else {
    $form_state['input']['commerce_ups_password']= commerce_ups_encrypt($form_state['input']['commerce_ups_password']);
  }

  $fields = array(
    'commerce_ups_account_id',
    'commerce_ups_user_id',
    'commerce_ups_password',
    'commerce_ups_access_key',
    'commerce_ups_encrypt',
    'commerce_ups_services',
    'commerce_ups_company_name',
    'commerce_ups_address_line_1',
    'commerce_ups_address_line_2',
    'commerce_ups_city',
    'commerce_ups_state',
    'commerce_ups_postal_code',
    'commerce_ups_country_code',
    'commerce_ups_packaging',
    'commerce_ups_pick_up_schedule',
    'commerce_ups_show_logo',
    'commerce_ups_default_package_size_length',
    'commerce_ups_default_package_size_width',
    'commerce_ups_default_package_size_height',
  );

  foreach ($fields as $key) {
    if (array_key_exists($key, $form_state['input'])) {
      $value = $form_state['input'][$key];
      variable_set($key, $value);
    }
  }

  drupal_set_message(t('The UPS configuration options have been saved.'));
}
