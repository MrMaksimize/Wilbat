<?php

/**
 * @file
 * FeedsUserProcessor class.
 */

/**
 * Feeds processor plugin. Create users from feed items.
 */
class FeedsCommerceCouponProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'commerce_coupon';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Coupons');
    return $info;
  }

  /**
   * Creates a new user account in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $coupon = new stdClass();
    $account->uid = 0;
    $account->status = $this->config['status'];
    return $account;
  }

  /**
   * Loads an existing user.
   */
  protected function entityLoad(FeedsSource $source, $uid) {
    return user_load($uid);
  }

  /**
   * Validates a user account.
   */
  protected function entityValidate($account) {
    if (empty($account->name) || empty($account->mail) || !valid_email_address($account->mail)) {
      throw new FeedsValidationException(t('User name missing or email not valid.'));
    }
  }

  /**
   * Save a user account.
   */
  protected function entitySave($account) {
    if ($this->config['defuse_mail']) {
      $account->mail = $account->mail . '_test';
    }
    user_save($account, (array) $account);
    if ($account->uid && !empty($account->openid)) {
      $authmap = array(
        'uid' => $account->uid,
        'module' => 'openid',
        'authname' => $account->openid,
      );
      if (SAVED_UPDATED != drupal_write_record('authmap', $authmap, array('uid', 'module'))) {
        drupal_write_record('authmap', $authmap);
      }
    }
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($uids) {
    foreach ($uids as $uid) {
      user_delete($uid);
    }
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'type' => '',
      'is_active' => 1,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['is_active'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Select whether users should be imported active or blocked.'),
      '#options' => array(0 => t('Blocked'), 1 => t('Active')),
      '#default_value' => $this->config['is_active'],
    );

    $types = commerce_coupon_get_types();

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#description' => t('Every coupon is set to this type.'),
      '#default_value' => $this->config['type'],
      '#options' => $types,
    );

    return $form;
  }

  /**
   * Override setTargetElement to operate on a target item that is a node.
   */
  public function setTargetElement(FeedsSource $source, $target_coupon, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $coupon_user->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      default:
        parent::setTargetElement($source, $coupon_user, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'commerce_coupon', 'commerce_coupon');

    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($coupon_id = parent::existingEntityId($source, $result)) {
      return $coupon_id;
    }

    // Iterate through all unique targets and try to find a coupon for the
    // target's value.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'code':
          $query = new EntityFieldQuery();
          $query->entityCondition('entity_type', 'commerce_coupon')
            ->fieldCondition('commerce_coupon_code', 'value', $value, '=');


          $coupon = $query->execute();

          if ($coupon == NULL || !isset($coupon['commerce_coupon']) || !is_array($coupon['commerce_coupon'])) {
            return NULL;
          }

          $coupon = reset($coupon['commerce_coupon']);

          if (isset($coupon) && isset($coupon->coupon_id)) {
            return $coupon->coupon_id;
          }
          break;
      }
    }
    return 0;
  }
}
