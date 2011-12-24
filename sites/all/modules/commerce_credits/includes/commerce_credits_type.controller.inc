<?php

/**
 * @file
 * The controller for the credit type entity containing the CRUD operations.
 */

class CommerceCreditsTypeEntityController extends EntityAPIControllerExportable {
  
  /**
   * Create a default credit type.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   A credit type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    return (object) ($values + array(
      'cid' => '',
      'is_new' => TRUE,
      'label' => '',
      'name' => '',
      'description' => '',
      'duration' => 0,
    ));
  }
}
