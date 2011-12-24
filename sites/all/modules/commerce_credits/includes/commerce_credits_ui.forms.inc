<?php

/**
 * @file
 * Forms for creating / editing and deleting credits.
 */


/**
 * Form callback: create or edit a credit group.
 *
 * @param $group
 *   The group array to edit or for a create form an empty group
 *     array with properties instantiated but not populated.
 */
function commerce_credits_ui_group_form($form, &$form_state, $group) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_credits_ui') . '/includes/commerce_credits_ui.forms.inc';

  // Store the initial groupin the form state.
  $form_state['group'] = $group;

  $form['group'] = array(
    '#tree' => TRUE,
  );

  $form['group']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $group['name'],
    '#description' => t('The human-readable name of this credit group. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 32,
  );

  if (empty($group['type'])) {
    $form['group']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $group['type'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_credits_group_load',
        'source' => array('group', 'name'),
      ),
      '#description' => t('The machine-readable name of this credit group. This name must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }

  $form['group']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this credit group. The text will be displayed on the <em>Add new content</em> page.'),
    '#default_value' => $group['description'],
    '#rows' => 3,
  );

  /*if (module_exists('entity_translation')) {
    $form['product_type']['multilingual'] = array(
      '#type' => 'radios',
      '#title' => t('Multilingual support'),
      '#description' => t('If <em>Entity translation</em> is enabled it will be possible to provide a different version of the same product for each available language.') . '<br />' . t('Existing products will not be affected by changing this option.'),
      '#options' => array(
        0 => t('Disabled'),
        ENTITY_TRANSLATION_ENABLED => t('Enabled via <em>Entity translation</em>'),
      ),
      '#default_value' => variable_get('language_product_type_' . $product_type['type'], 0),
    );
  }*/

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save credit group'),
    '#submit' => $submit + array('commerce_credits_ui_group_form_submit'),
  );

  if (!empty($form_state['group']['type'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete credit group'),
      '#suffix' => l('Cancel', 'admin/commerce/credits/groups'),
      '#submit' => $submit + array('commerce_credits_ui_group_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['save_continue'] = array(
      '#type' => 'submit',
      '#value' => t('Save and add fields'),
      '#suffix' => l('Cancel', 'admin/commerce/credits/groups'),
      '#submit' => $submit + array('commerce_credits_ui_group_form_submit'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'commerce_credits_ui_group_form_validate';

  return $form;
}

/**
 * Validation callback for commerce_credits_ui_group_form().
 */
function commerce_credits_ui_group_form_validate($form, &$form_state) {
  $group = $form_state['group'];

  // If saving a new credit group, ensure it has a unique machine name.
  if (empty($group['type'])) {
    if (!commerce_credits_ui_validate_group_unique($form_state['values']['group']['type'])) {
      form_set_error('group][type', t('The machine name specified is already in use.'));
    }
  }
}

/**
 * Form submit handler: save a credit group.
 */
function commerce_credits_ui_group_form_submit($form, &$form_state) {
  $group = $form_state['group'];
  $updated = !empty($group['type']);

  // If a type is set, we should still check to see if a row for the type exists
  // in the database; this is done to accomodate types defined by Features.
  if ($updated) {
    $updated = db_query('SELECT 1 FROM {commerce_credits_groups} WHERE type = :type', array(':type' => $group['type']))->fetchField();
  }

  foreach ($form_state['values']['group'] as $key => $value) {
    $group[$key] = $value;
  }

  // Write the product type to the database.
  $group['is_new'] = !$updated;
  commerce_credits_ui_group_save($group);

  // Set the multingual value for the product type if entity translation is enabled.
  /*if (module_exists('entity_translation')) {
    variable_set('language_product_type_' . $product_type['type'], $product_type['multilingual']);
  }*/

  // Redirect based on the button clicked.
  drupal_set_message(t('Credit group saved.'));

  if ($form_state['triggering_element']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/credits/groups/' . strtr($group['type'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/credits/groups';
  }
}

/**
 * Submit callback for delete button on commerce_credits_ui_group_form().
 *
 */
function commerce_credits_ui_group_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/credits/groups/' . strtr($form_state['group']['type'], '_', '-') . '/delete';
}

/**
 * Form callback: confirmation form for deleting a credit group.
 *
 * @param $group
 *   The group array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_credits_ui_group_delete_form($form, &$form_state, $group) {
  $form_state['group'] = $group;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_credits_ui') . '/includes/commerce_credits_ui.forms.inc';

  $form['#submit'][] = 'commerce_credits_ui_group_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name credit group?', array('%name' => $group['name'])),
    'admin/commerce/credits/groups',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_credits_ui_group_delete_form().
 */
function commerce_credits_ui_group_delete_form_submit($form, &$form_state) {
  $group = $form_state['group'];

  commerce_credits_ui_group_delete($group['type']);

  drupal_set_message(t('The credit group %name has been deleted.', array('%name' => $group['name'])));
  watchdog('commerce_product', 'Deleted credit group %name.', array('%name' => $group['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/credits/groups';
}

/**
 * Form callback: create or edit a credit type.
 *
 * @param $type
 *   The credit type object to edit or for a create form an empty credit type object
 *     with only a credit group defined.
 */
function commerce_credits_ui_credit_type_form($form, &$form_state, $type) {

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_credits_ui') . '/includes/commerce_credits_ui.forms.inc';
  
    // Add the field related form elements.
  $form_state['commerce_credits_type'] = $type;
  field_attach_form('commerce_credits_type', $type, $form, $form_state);

  // Add the default field elements.

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $type->label,
    '#description' => t('The human-readable name of this credit type. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 32,
  );

  if (empty($type->name)) {
    $form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $type->name,
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_credits_type_load_by_name',
        'source' => array('label')
      ),
      '#description' => t('The machine-readable name of this credit type. This name must contain only lowercase letters, numbers, and underscores, it must be unique.'),
    );
  }
  
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $type->description,
    '#maxlength' => 255,
  );
  
  $form['duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#default_value' => $type->duration,
    '#maxlength' => 255,
    '#description' => t('Duration of this credit type in minutes. Set to -1 for unlimited.')
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save credit type'),
  );
  return $form;
}
