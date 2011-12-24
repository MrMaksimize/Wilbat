<?php

/**
 * Filter by product type.
 */
class commerce_credits_handler_filter_group extends views_handler_filter_in_operator {
  // Display a list of credit groups in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Credit group');
      $this->value_options = commerce_credits_group_get_name();
    }
  }
}
