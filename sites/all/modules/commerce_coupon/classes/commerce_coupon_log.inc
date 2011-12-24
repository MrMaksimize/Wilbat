<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

class CommerceCouponLog extends Entity {

  public $log_id;
  public $coupon_id;
  public $uid;
  public $order_id;
  public $is_accepted;
  public $is_processed;
  public $created;
  public $changed;


  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_coupon_log');
  }
}
