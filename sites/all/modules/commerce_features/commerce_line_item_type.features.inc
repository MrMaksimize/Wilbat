<?php

/**
 * Implements hook_features_export().
 */
function commerce_line_item_type_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $map = features_get_default_map('commerce_line_item_type');
  // Add commerce and features as dependencies,
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['commerce_features'] = 'commerce_features';
  $export['dependencies']['commerce_line_item_ui'] = 'commerce_line_item_ui';

  $info = commerce_line_item_types();

  foreach ($data as $type) {
    // Add module dependencies
    if (isset($info[$type]['module'])) {
      $export['dependencies'][$info[$type]['module']] = $info[$type]['module'];
    }
    $export['features']['commerce_line_item_type'][$type] = $type;

    // Fetch fields of the profile type and add them as dependency
    $fields = field_info_instances('commerce_line_item', $type);
    foreach ($fields as $name => $field) {
      $pipe['field'][] = "commerce_line_item-{$field['bundle']}-{$field['field_name']}";
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function commerce_line_item_type_features_export_options() {
  $feature_types = array();
  $line_item_types = commerce_line_item_types();
  if (!empty($line_item_types)) {
    foreach ($line_item_types as $type) {
      $feature_types[$type['type']] = $type['name'];
    }
  }
  return $feature_types;
}

/**
 * Implements hook_features_export_render().
 */
function commerce_line_item_type_features_export_render($module, $data) {
  $exportable_data = array();
  $info = commerce_line_item_types();

  // Filter out all line item types not coming from UI.
  foreach ($data as $type) {
   if (isset($info[$type]) && $line_item_type = $info[$type]) {
     if (isset($line_item_type['module']) && $line_item_type['module'] == 'commerce_custom_line_items') {
      $exportable_data[$type] = $type;
     }
   }
  }

  if (!empty($exportable_data)) {
    $output = array();
    $output[] = '  $items = array(';
    foreach ($exportable_data as $type) {
      if (isset($info[$type]) && $line_item_type = $info[$type]) {
        $output[] = "    '{$type}' => " . features_var_export($line_item_type) . ",";
      }
    }
    $output[] = '  );';
    $output[] = '  return $items;';
    $output = implode("\n", $output);
    return array('commerce_line_item_default_types' => $output);
  }

  return array();
}

/**
 * Implements hook_features_revert().
 */
function commerce_line_item_type_features_revert($module) {
  // Get default line item types
  if (module_hook($module, 'commerce_line_item_default_types')) {
    $default_types = module_invoke($module, 'commerce_line_item_default_types');
    $existing_types = commerce_line_item_types();
    foreach ($default_types as $type) {
      // Add / or update
      if (!isset($existing_types[$type['type']])) {
        $type['is_new'] = TRUE;
      }
      // Use UI function because it provides already the logic we need
      module_invoke('commerce_custom_line_items', 'commerce_line_item_type_save', $type);
    }
  }
  else {
    drupal_set_message(t('Could not load default line item types.'), 'error');
    return FALSE;
  }

  // Re-Cache
  drupal_static_reset('commerce_line_item_types');

  menu_rebuild();
  return TRUE;
}

/**
 * Implements hook_features_rebuild().
 */
function commerce_line_item_type_features_rebuild($module) {
  return commerce_line_item_type_features_revert($module);
}

