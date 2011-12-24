<?php

/**
 * Specialized relationship handler for associating commerce credits and their entity.
 */
class commerce_credits_handler_credits_relationship extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $this->definition['extra'][] = array(
      'field' => 'entity_type',
      'value' => $this->definition['entity'],
    );
    parent::query();
  }
}