<?php

/**
 * @file
 * Contains functions for the user interface of commerce credits module.
 * 
 * This file contains function for building user interface and administering
 * user credits balance.   
 */

function commerce_credits_choose_form($form, &$form_state, $group) {
  // Credits of this user
  global $user;
  $entity = entity_metadata_wrapper('user', $user);
  $credits = commerce_credits_entity_credits($entity);
  
  $form = array();
  if (empty($credits)) {
    $form['empty'] = array(
      '#markup' => t('You need to buy credits')
    );
  }
  else {
    $header = array(t('Name'), t('Description'), t('Number'));
    $rows = array();
    $types = commerce_credits_types();
    foreach ($credits as $credit_type => $credit_number) {
      $type = $types[$credit_type];
      if ($group == 'all' || $type->type == $group) {
        $rows[$type->name] = array(
          $type->label,
          $type->description,
          $credit_number
        );
      }
    }
    $form['credit_choice'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#multiple' => FALSE
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Choose')
  );
  return $form;
}

function commerce_credits_choose_form_submit($form, &$form_state) {
  // Save credit type in session
  $credit_type = $form_state['input']['credit_choice'];
  $_SESSION[COMMERCE_CREDITS_SESSION_VARIABLE] = $credit_type;
  module_invoke_all('commerce_credits_after_credit_choice', $credit_type);
  rules_invoke_event('commerce_credits_after_choice', $credit_type);
}
