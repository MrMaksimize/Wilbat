<?php

function commerce_price_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_commerce_price_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _commerce_price_devel_generate($object, $field, $instance, $bundle);
  }
}

function _commerce_price_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();
  $min = 0;
  $max = 50000;
  // @TODO: Make min, max, currency code configurable.
  $object_field['currency_code'] = 'USD';
  $object_field['amount'] = rand($min, $max);
  return $object_field;
}
