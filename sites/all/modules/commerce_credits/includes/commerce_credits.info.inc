<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

class CommerceCreditsEntityMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    unset($properties['credit_type']);
    $properties['commerce_credits_type'] = array(
      'label' => t("Credit type"),
      'type' => 'commerce_credits_type',
      'description' => t("The associated credit type."),
      'required' => TRUE,
      'getter callback' => 'entity_property_getter_method',
      'schema field' => 'credit_type',
    );

    return $info;
  }
}