<?php

/**
 * @file
 * Form constructors and page callback for the Commerce BPC display node module.
 */

/**
 * Menu callback: Display list of node types or redirect to node type form.
 *
 * Path: admin/commerce/products/add-bulk/PRODUCT_TYPE/display/BULK_CREATION_ID`
 *
 * @param $product_type
 *   The type of the created products.
 * @param $bulk_creation_id
 *   The temporary id of the bulk creation set. Used to find the array of
 *   created product IDs in the $_SESSION array.
 *
 * @see commerce_bpc_menu()
 */
function bpc_display_select_node_type($product_type, $bulk_creation_id) {

  // If a node type is specified in the settings, we use that.
  $node_type = commerce_bpc_setting('display', 'auto_content_type', $product_type);

  if ($node_type == '_none') {
    $node_type = NULL;
  }

  // Else, we get a list of node types. If there is only one that has a
  // product reference field, we redirect to that one.
  if (empty($node_type)) {
    $node_types = bpc_display_get_node_types($product_type);
    if (count($node_types) == 1) {
      $node_type = reset($node_types);
    }
  }

  // If either of the cases above yielded a single node type, we redirect to
  // that.
  if (!empty($node_type)) {
    // Redirect to the node/add form
    drupal_goto('node/add/' . str_replace('_', '-', $node_type), array('query' => array('bulk_creation_id' => $bulk_creation_id)));
  }
  else {
    return drupal_get_form('bpc_display_select_node_type_form', $product_type, $bulk_creation_id);
  }
}

/**
 * Form constructor for the node type select form.
 *
 * @see bpc_display_select_node_type()
 * @see bpc_display_select_node_type_form_submit()
 */
function bpc_display_select_node_type_form($form, &$form_state, $product_type, $bulk_creation_id) {

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Bulk Product Creation Form - Choose Display Node type'),
  );

  // Store the product type for later use
  $form['product_type'] = array(
    '#type' => 'value',
    '#value' => $product_type,
  );
  $form['bulk_creation_id'] = array(
    '#type' => 'value',
    '#value' => $bulk_creation_id,
  );

  $form['node_type'] = array(
    '#type' => 'radios',
    '#title' => t('Display Node'),
    '#options' => bpc_display_get_node_types($product_type),
    '#required' => TRUE,
    '#description' => t("Select which Display Node Type to use.
      Note that the Node Type must have a Product Reference Field with multiple values which can accept products of the chosen type to be considered as a candidate."),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create node'),
    '#validate' => array(),
  );

  return $form;
}

/**
 * Form submission handler for bpc_display_select_node_type_form().
 */
function bpc_display_select_node_type_form_submit($form, &$form_state) {

  $node_type = str_replace('_', '-', $form_state['values']['node_type']);
  $path = 'node/add/' . $node_type;
  $options = array(
    'query' => array(
      'bulk_creation_id' => $form_state['values']['bulk_creation_id']
    ),
  );

  drupal_goto($path, $options);
}

