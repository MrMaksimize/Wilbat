<?php

/**
 * Field handler to translate a credit group into its readable form.
 */
class commerce_credits_handler_field_group extends views_handler_field {
  function render($values) {
    $type = $this->get_value($values);
    $value = commerce_credits_group_get_name($type);

    return $value;
  }
}
