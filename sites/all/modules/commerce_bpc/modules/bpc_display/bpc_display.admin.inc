<?php

/**
 * @file
 *
 * Adminstration pages for the Commerce BPC display node module.
 */

/**
 * Form constructor for the commerce_bpc display node settings page.
 *
 * Paths:
 * - admin/commerce/config/commerce_bpc/display_nodes
 * - admin/commerce/products/types/PRODUCT_TYPE/commerce_bpc/display_nodes
 *
 * @see bpc_display_menu()
 * @see bpc_display_settings_validate()
 * @see commerce_bpc_settings_form()
 * @ingroups forms
 */
function bpc_display_settings_form($form, &$form_state, $type = NULL) {

  $form = array();

  $form['creation_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display node creation'),
    '#description' => t('How should the creation of display nodes be handled?'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
  );

  $form['creation_type']['creation_method'] = array(
    '#type' => 'radios',
    '#title' => t('Display node creation'),
    '#title_display' => 'invisible',
    '#description' => t('<strong>Note:</strong> The rest of the settings available on this page depends on what you select here.'),
    '#options' => array(
      'wizard' => t('Provide a "Save and create display" link on the Bulk creation form that takes the user to a pre-populated node creation form.'),
      'auto' => t('Silently create a display node automatically that references all the created products.'),
      'onetoone' => t('Create a display node automatically for every product created.'),
      'none' => t('Do not include any display node functionality.'),
    ),
    '#default_value' => commerce_bpc_setting('display', 'creation_method', $type),
  );


  $form['auto_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for created display nodes'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_bpc_display_creation_method"]' => array('value' => 'none'),
      ),
    ),
  );
  $required_if_auto = array(
    'optional' => array(
      ':input[name="creation_method"]' => array('value' => 'wizard'),
    )
  );
  $node_types = bpc_display_get_node_types();
  if (!empty($node_types)) {
    $form['auto_settings']['auto_content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type of the created display node'),
      '#description' => t('The setting <strong>--Allow user to select node type--</strong> is only valid for manual node creation.'),
      '#multiple' => FALSE,
      '#options' => array('_none' => t('--Allow user to select node type--')) + $node_types,
      '#default_value' => commerce_bpc_setting('display', 'auto_content_type', $type),
      '#states' => $required_if_auto,
    );
  }
  else {
    $form['auto_settings']['auto_content'] = array(
      '#markup' => '<p><strong>' . t('In order to auto-generate display nodes for all product types, you must create at least one content type that has a product reference field that can reference all product types. Visit the <a href="@content_type_path">content type page</a> to add such a field to a content type.',
      array(
        '@content_type_path' => '/admin/structure/types',
      )
      ) . '</strong></p>',
    );
    $form['auto_settings']['no_content_types_available'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
  }
  $form['auto_settings']['auto_node_title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern for the title of the created display node'),
    '#description' => t('You can use the token %title_fragment_token for the title fragment entered by the user on the bulk creation form.', array(
           '%title_fragment_token' => '[bulk_defaults:entered_title]'
    )),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('display', 'auto_node_title_pattern', $type),
    '#states' => $required_if_auto,
  );

  $tokens = token_info();
  $product_tokens = $tokens['tokens']['commerce-product'];
  $form['auto_settings']['tokens'] = _commerce_bpc_show_tokens('commerce-product', $product_tokens);
  // Only show if we create one display per product.
  $form['auto_settings']['tokens']['#states'] = array(
    'visible' => array(
      ':input[name="display_creation_method"]' => array('value' => 'onetoone'),
      ),

  );

  $form['auto_settings']['auto_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('After successful bulk creation, send the user to ...'),
    '#options' => array(
      'product listing' => t('... the list of products.'),
      'display node'    => t('... the newly created display node.'),
      'custom'          => t('... a custom location.'),
    ),
    '#default_value' => commerce_bpc_setting('display', 'auto_redirect', $type),
  );
  $form['auto_settings']['redirection_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom redirect path'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
    '#states' => array(
      'visible' => array(
        ':input[name="auto_redirect"]' => array('value' => 'custom'),
      ),
    ),
  );


$form['auto_settings']['redirection_settings']['auto_redirect_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom redirect path'),
    '#size' => 80,
    '#maxlength' => 255,
    '#title_display' => 'invisible',
    '#description' => 'Enter any valid drupal path.',
    '#default_value' => commerce_bpc_setting('display', 'auto_redirect_custom', $type),
    '#states' => array(
      'visible' => array(
        ':input[name="display_auto_redirect"]' => array('value' => 'custom'),
      ),
      'required' => array(
        ':input[name="display_auto_redirect"]' => array('value' => 'custom'),
      ),
    ),
  );

  return commerce_bpc_settings_form($form, 'display', $type);
}
/**
 * Form validation handler for bpc_display_settings_form().
 */
function bpc_display_settings_form_validate($form, &$form_state) {
  $fv = &$form_state['values'];
  switch ($fv['creation_method']) {
    case 'auto':
    case 'onetoone':
      // Notify the user if there is no content type available.
      if (!empty($fv['no_content_types_available'])) {
        form_set_error('auto_content_type', 'You cannot activate auto-creation of display nodes if there are no appropriate display node types. See below for details.');
      }
      // Make sure all the required fields have values.
      if (empty($fv['auto_content_type']) || $fv['auto_content_type'] == '_none') {
        form_set_error('auto_content_type', 'You have to specify a content type to automatically create display nodes.');
      }
      if (empty($fv['auto_node_title_pattern'])) {
        form_set_error('auto_node_title_pattern', 'For auto-creation of display nodes to work, you have to specify a pattern for the node title.');
      }
      if ($fv['auto_redirect'] == 'custom') {
        if (empty($fv['auto_redirect_custom'])) {
          form_set_error('auto_redirect', 'Please specify a custom redirect location.');
        }
        elseif (!drupal_valid_path($fv['auto_redirect_custom'])) {
          form_set_error('auto_redirect_custom', 'Please specify a valid path as custom redirect location');
        }
      }
      break;
  }
}
