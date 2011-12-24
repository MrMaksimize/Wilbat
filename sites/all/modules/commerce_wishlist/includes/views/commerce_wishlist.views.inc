<?php

/**
 * Export Drupal Commerce products to Views.
 */

/**
 * Implements hook_views_data()
 */
function commerce_wishlist_views_data() {
  $data = array();

  $data['commerce_wishlist']['table']['group']  = t('Commerce Wishlist');

  $data['commerce_wishlist']['table']['base'] = array(
    'field' => 'wishlist_id',
    'title' => t('Commerce Wishlist'),
    'help' => t('TODO:'),
    'entity type' => 'commerce_product',
    'access query tag' => 'commerce_product_access',
  );

  // Expose the product ID.
  $data['commerce_wishlist']['product_id'] = array(
    'title' => t('Product ID'),
    'help' => t('The unique internal identifier of the product.'),
    'field' => array(
      'handler' => 'commerce_product_handler_field_product',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Product'),
      'help' => t("Relate this wishlist to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_product',
      'base field' => 'product_id',
      'field' => 'product_id',
      'label' => t('Wishlist product display'),
    ),
  );

  // Expose the product type.
  $data['commerce_wishlist']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The human-readable name of the type of the product.'),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Product display (node)'),
      'help' => t("Relate this wishlist to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'field' => 'nid',
      'label' => t('Wishlist product display'),
    ),
  );

  // Expose the creator uid.
  $data['commerce_wishlist']['uid'] = array(
    'title' => t('Uid'),
    'help' => t("The owner's user ID."),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // display this field in the summary
    ),
    'filter' => array(
      'title' => t('Owner'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Owner'),
      'help' => t("Relate this wishlist to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Wishlist owner'),
    ),
  );

  // Expose links to operate on the product.
  $data['commerce_wishlist']['delete_wishlist'] = array(
    'field' => array(
      'title' => t('Delete'),
      'help' => t('Provide a simple link to delete the product from the wishlist.'),
      'handler' => 'commerce_wishlist_handler_field_product_link_delete',
    ),
  );

  return $data;
}

