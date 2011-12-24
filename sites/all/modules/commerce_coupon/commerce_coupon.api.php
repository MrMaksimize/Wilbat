<?php
/**
 * @file
 * Hooks provided by Commerce Coupon module.
 */

/**
 * @defgroup commerce_coupon_api_hooks Commerce Coupon Hooks
 * @{
 */

/**
 * This hook called when a coupon type deleted.
 * 
 * @param string $type Coupon type.
 */
function hook_commerce_coupon_type_delete($type) {
  // Delete all coupons of this type.
  if ($pids = array_keys(commerce_coupon_load_multiple(FALSE, array('type' => $type->type)))) {
    commerce_coupon_delete_multiple($pids);
  }
  // Rebuild the menu as any (user-category) menu items should be gone now.
  menu_rebuild();
}

/**
 * ???
 * @see commerce_coupon_type_configure().
 */
function commerce_coupon_type_configure($bundle, $reset) {
}
