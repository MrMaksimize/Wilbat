<?php

/**
 * UI controller.
 */
class CommerceCreditsTypeUIController extends EntityDefaultUIController {

  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    unset($items[$this->path . '/add']);
    foreach (commerce_credits_groups() as $group_name => $group) {
      $items[$this->path . '/add/' . strtr($group_name, array('_' => '-'))] = array(
        'title' => 'Add @name type',
        'title arguments' => array('@name' => $group['name']),
        'description' => $group['description'],
        'page callback' => 'entity_ui_get_form',
        'page arguments' => array($this->entityType, NULL, 'add'),
        'access callback' => 'entity_access',
        'access arguments' => array('create', $this->entityType),
        'type' => MENU_LOCAL_ACTION,
        'file' => $this->entityInfo['admin ui']['file'],
        'file path' => isset($this->entityInfo['admin ui']['file path']) ? $this->entityInfo['admin ui']['file path'] : drupal_get_path('module', $this->entityInfo['module']),
        'weight' => -1,
      );
    }
    return $items;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    // Remove the manage fields links.
    // @todo caused by 'bundle of' key, maybe can be fixed in Entity API.
    unset($row[3]);
    unset($row[4]);
    return $row;
  }
}

/**
 * Generates the commerce credits type editing form.
 */
function commerce_credits_type_form($form, &$form_state, $credit_type, $op = 'edit') {
  // arg(5) contains the group type name.
  if ($op == 'add') {
    $credit_type->type = arg(5);
  }
  if ($op == 'clone') {
    $credit_type->label .= ' (cloned)';
  }
  module_load_include('inc', 'commerce_credits_ui', 'includes/commerce_credits_ui.forms');
  return commerce_credits_ui_credit_type_form($form, $form_state, $credit_type);
}

/**
 * Form API validation callback for the type form.
 */
function commerce_credits_type_formm_validate($form, &$form_state) {
  // Make sure duration is a positive integer
  $duration = $form_state['values']['duration'];
  if (!is_numeric($duration) || $duration_int < -1) {
    form_set_error('duration', t('You must enter a positive integer or -1 for the duration.'));
  }
}

/**
 * Form API submit callback for the type form.
 */
function commerce_credits_type_form_submit(&$form, &$form_state) {
  $credit_type = entity_ui_form_submit_build_entity($form, $form_state);
  commerce_credits_type_save($credit_type);
  drupal_set_message(t('Credit type saved.'));
  $form_state['redirect'] = 'admin/commerce/credits/types';
}
