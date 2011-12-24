<?php

/**
 * Field handler to present a credit type operations links.
 */
class commerce_credits_handler_field_type_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['cid'] = 'cid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $cid = $this->get_value($values, 'cid');

    $links = menu_contextual_links('commerce-credits-ui-type', 'admin/commerce/credits/types', array($cid));

    if (!empty($links)) {

      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
