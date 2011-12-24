<?php

/**
 * Specialized relationship handler for associating commerce credits and their entity.
 */
class commerce_credits_handler_entity_relationship extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    parent::query();
    $table_alias = $this->ensure_my_table();
    $this->query->add_where(0, $table_alias . '.entity_type', $this->definition['entity']);
  }
}