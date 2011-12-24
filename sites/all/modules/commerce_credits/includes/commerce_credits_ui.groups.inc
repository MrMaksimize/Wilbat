<?php

/**
 * @file
 */


/**
 * Menu callback: display an overview of available groups.
 */
function commerce_credits_ui_groups_overview() {
  //drupal_add_css(drupal_get_path('module', 'commerce_product_ui') . '/theme/commerce_product_ui.types.css');

  $header = array(
    t('Name'),
    t('Operations'),
  );

  $rows = array();

  // Loop through all defined groups.
  foreach (commerce_credits_groups() as $group_name => $group) {
    // Build the operation links for the current credit group.
    $links = menu_contextual_links('commerce-credits-groups', 'admin/commerce/credits/groups', array(strtr($group_name, array('_' => '-'))));

    // Add the group's row to the table's rows array.
    $rows[] = array(
      theme('credit_group_admin_overview', array('group' => $group)),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no groups are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new credit group.
    $rows[] = array(
      array(
        'data' => t('There are no credit groups yet. <a href="@link">Add credit group</a>.', array('@link' => url('admin/commerce/credits/groups/add'))),
        'colspan' => 2,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Builds an overview of a credit group for display to an administrator.
 *
 * @param $variables
 *   An array of variables used to generate the display; by default includes the
 *     type key with a value of the credit group array.
 *
 * @ingroup themeable
 */
function theme_credit_group_admin_overview($variables) {
  $group = $variables['group'];

  $output = check_plain($group['name']);
  $output .= ' <small> (Machine name: ' . check_plain($group['type']) . ')</small>';
  $output .= '<div class="description">' . filter_xss_admin($group['description']) . '</div>';

  return $output;
}

/**
 * Form callback wrapper: create or edit a credit group.
 *
 * @param $group
 *   The machine-name of the credit group being created or edited by this form
 *     or a full credit group array.
 *
 */
function commerce_credits_ui_group_form_wrapper($group) {
  if (is_array($group)) {
    $credit_group = $group;
  }
  else {
    $credit_group = commerce_credits_group_load($group);
  }

  // Add the breadcrumb for the form's location.
  //commerce_product_ui_set_breadcrumb(TRUE);

  // Return a message if the product type is not governed by Product UI.
  if (!empty($credit_group['type']) && $credit_group['module'] != 'commerce_credits_ui') {
    return t('This credit group cannot be edited, because it is not defined by the Commerce Credits UI module.');
  }

  // Include the forms file from the Product module.
  module_load_include('inc', 'commerce_credits_ui', 'includes/commerce_credits_ui.forms');

  return drupal_get_form('commerce_credits_ui_group_form', $credit_group);
}

/**
 * Form callback wrapper: confirmation form for deleting a credit group.
 *
 * @param $group_name
 *   The machine-name of the credit group being deleted
 *
 * @see commerce_product_product_type_delete_form()
 */
function commerce_credits_ui_group_delete_form_wrapper($group_name) {
  if (is_array($group_name)) {
    $group = $group_name;
  }
  else {
    $group = commerce_credits_group_load($group_name);
  }

  // Add the breadcrumb for the form's location.
  //commerce_product_ui_set_breadcrumb(TRUE);

  // Return a message if the credit group is not governed by Credits UI.
  if ($group['module'] != 'commerce_credits_ui') {
    return t('This credit group cannot be deleted, because it is not defined by the Commerce Credits UI module.');
  }

  // TODO: Don't allow deletion of product types that have credits already.
  /*if (($count = db_query("SELECT product_id FROM {commerce_product} WHERE type = :product_type", array(':product_type' => $product_type['type']))->rowCount()) > 0) {
    drupal_set_title(t('Cannot delete the %name product type', array('%name' => $product_type['name'])), PASS_THROUGH);
    return format_plural($count,
      'There is 1 product of this type. It cannot be deleted.',
      'There are @count products of this type. It cannot be deleted.'
    );
  }*/

  // Include the forms file from the Credits module.
  module_load_include('inc', 'commerce_credits_ui', 'includes/commerce_credits_ui.forms');

  return drupal_get_form('commerce_credits_ui_group_delete_form', $group);
}
