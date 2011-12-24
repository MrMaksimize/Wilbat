<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

class CommerceCoupon extends Entity {
  public $is_active = TRUE;
  public $type;
  
  public function __construct($values = array()) {
    // Allow passing in the type entity.
    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->type;
    }
    parent::__construct($values, 'commerce_coupon');
  }
}

