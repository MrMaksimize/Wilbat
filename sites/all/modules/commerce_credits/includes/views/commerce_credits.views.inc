<?php

/**
 * Export Drupal Commerce credits to Views.
 */

function commerce_credits_views_data_alter(&$data) {
  $data['commerce_credits_types']['type']['title'] = t('Group');
  $data['commerce_credits_types']['type']['help'] = t('The human-readable name of the group of this credit type');
  $data['commerce_credits_types']['type']['field']['handler'] = 'commerce_credits_handler_field_group';
  $data['commerce_credits_types']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the credit type.'),
      'handler' => 'commerce_credits_handler_field_type_operations',
    ),
  );
  $data['commerce_credits']['expiration_date']['field']['handler'] = 'views_handler_field_date';

  // Generate the relations to all possible entity types.
  foreach (entity_get_info() as $entity_type => $info) {
    if (empty($info['configuration'])) {
      $data['commerce_credits'][$info['base table'] . '_rel'] = array(
        'group' => t('Commerce credits'),
        'title' => t('Referenced @entity', array('@entity' => $info['label'])),
        'help' => t('The associated entity for this credit.'),
        'relationship' => array(
          // Pass the entity to the handler.
          'entity' => $entity_type,
          'handler' => 'commerce_credits_handler_entity_relationship',
          'label' => t('Entity'),
          'base' => $info['base table'],
          'base field' => $info['entity keys']['id'],
          'relationship field' => 'entity_id',
        ),
      );
      // Add relations from the entity to its credits.
      $data[$info['base table']]['commerce_credits_rel'] = array(
        'group' => t('Commerce credits'),
        'title' => t('Referenced commerce credits'),
        'help' => t('The associated commerce credits entity for the @entity.', array('@entity' => strtolower($info['label']))),
        'relationship' => array(
          // Pass the entity to the handler.
          'entity' => $entity_type,
          'handler' => 'commerce_credits_handler_credits_relationship',
          'label' => t('Commerce credits entity'),
          'base' => 'commerce_credits',
          'base field' => 'entity_id',
          'relationship field' => $info['entity keys']['id'],
        ),
      );
    }
  }
}
