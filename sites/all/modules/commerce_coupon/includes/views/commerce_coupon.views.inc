<?php

/**
 * @file
 * Coupon views alter
 *
 */

/**
 * Alter the views data to enable some additional features for coupons in views
 */
function commerce_coupon_views_data_alter(&$data) {
  if (isset($data['commerce_coupon'])) {
    $data['commerce_coupon']['operations'] = array(
      'field' => array(
        'title' => t('Operations links'),
        'help' => t('Display all the available operations links for the coupon.'),
        'handler' => 'commerce_coupon_handler_field_coupon_operations',
      ),
    );
  }
}
