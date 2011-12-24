<?php

/**
 * Field handler to present a link to delete a product for wishlist.
 */
class commerce_wishlist_handler_field_product_link_delete extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['wishlist_id'] = 'wishlist_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Ensure the user has access to delete this product.
    $wishlist_id = $this->get_value($values, 'wishlist_id');
    $uid = $this->get_value($values, 'uid');

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    return l($text, 'user/'. $uid .'/wishlist/delete/'. $wishlist_id, array('query' => drupal_get_destination()));
  }
}
