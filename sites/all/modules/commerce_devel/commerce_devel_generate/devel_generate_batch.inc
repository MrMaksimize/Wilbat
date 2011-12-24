<?php

/**
 * Devel Generate batch handling functions using the BatchAPI
 */

/**
 * Functions called from FAPI:
 */

function commerce_devel_generate_batch_products($form_state) {
  $operations = array();

  // Setup the batch operations and save the variables.
  $operations[] = array('commerce_devel_generate_batch_products_pre_product', array($form_state['values']));

  // add the kill operation
  if ($form_state['values']['kill_products']) {
    $operations[] = array('commerce_devel_generate_batch_products_kill', array());
  }

  // add the operations to create the products
  for ($num = 0; $num < $form_state['values']['num_products']; $num ++) {
    $operations[] = array('commerce_devel_generate_batch_products_add_product', array());
  }

  // start the batch
  $batch = array(
    'title' => t('Generating Products'),
    'operations' => $operations,
    'finished' => 'commerce_devel_generate_batch_finished',
    'file' => drupal_get_path('module', 'commerce_devel_generate') . '/devel_generate_batch.inc',
  );
  batch_set($batch);
}

/**
 * Create Product Batch Functions:
 */

function commerce_devel_generate_batch_products_kill(&$context) {
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  commerce_devel_generate_products_kill($context['results']);
}

function commerce_devel_generate_batch_products_pre_product($vars, &$context) {
  $context['results'] = $vars;
  $context['results']['num_products'] = 0;
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  commerce_devel_generate_products_pre_product($context['results']);
}

function commerce_devel_generate_batch_products_add_product(&$context) {
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  module_load_include('inc', 'devel_generate', 'devel_generate');
  commerce_devel_generate_product_add_product($context['results']);
  $context['results']['num_product_ids'] ++;
}

function commerce_devel_generate_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t('Finished @num_product_ids products created successfully.', array('@num_product_ids' => $results['num_product_ids']));
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

