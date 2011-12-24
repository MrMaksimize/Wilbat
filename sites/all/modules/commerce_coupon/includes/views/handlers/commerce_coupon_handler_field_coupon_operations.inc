<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

/**
 * Field handler to present an order's operations links.
 */
class commerce_coupon_handler_field_coupon_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['coupon_id'] = 'coupon_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $coupon_id = $this->get_value($values, 'coupon_id');
    $links = menu_contextual_links('commerce-coupon', 'admin/commerce/coupons/manage', array($coupon_id));

    //return print_r($links, 1);

    if (!empty($links)) {
      //drupal_add_css(drupal_get_path('module', 'commerce_coupon') . '/theme/commerce_order_views.css');

      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
