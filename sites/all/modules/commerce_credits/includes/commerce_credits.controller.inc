<?php

/**
 * @file
 * The controller for the commerce credit entity.
 */


class CommerceCreditsEntityController extends EntityAPIController {

  /**
   * Create a default credit entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return
   *   A credit object with all default fields initialized.
   */
  public function create(array $values = array()) {
    return (object) ($values + array(
      'cid' => '',
      'is_new' => TRUE,
      'entity_type' => '',
      'entity_id' => '',
      'credit_type' => '',
      'expiration_date' => NULL,
      'credits' => 0,
    ));
  }

  /**
   * Helper function that creates a new credits entity that references to
   * another entity, e.g. the user.
   *
   * @param $ref_entity_id
   *  The entity id of the referenced entity.
   * @param $ref_entity_type
   *  The entity type of the referenced entity.
   * @param $credit_type
   *  The credit type name for which the credit entity will be loaded.
   *
   * @return
   *  The credit entity if it exists, else FALSE.
   */
  public function createByEntity($ref_entity_id, $ref_entity_type, $credit_type) {
    $values = array(
      'entity_id' => $ref_entity_id,
      'entity_type' => $ref_entity_type,
      'credit_type' => $credit_type,
    );
    return $this->create($values);
  }

  /**
   * Helper function that loads the credit entity that another entity
   * references, e.g. the user.
   *
   * @param $ref_entity_id
   *  The entity id of the referenced entity.
   * @param $ref_entity_type
   *  The entity type of the referenced entity.
   * @param $credit_type
   *  The credit type name for which the credit entity will be loaded. If empty
   *  all credit entities for the associated entity will be loaded.
   * @param $reset
   *  Reset static cache.
   *
   * @return
   *  An array of credit entities, or FALSE if non found.
   */
  public function loadByEntity($ref_entity_id, $ref_entity_type, $credit_type = '', $reset = FALSE) {
    if ($reset) {
      $this->resetCache();
    }
    $conditions = array(
      'entity_id' => $ref_entity_id,
      'entity_type' => $ref_entity_type,
    );
    if (!empty($credit_type)) {
       $conditions['credit_type'] = $credit_type;
    }
    return $this->load(FALSE, $conditions);
  }

  /**
   * Adds credits.
   *
   * @param $entity
   *  The credits entity to which the credits will be added.
   * @param $credits
   *  The amount of credits to be added.
   *
   * @return
   *  The credits entity.
   */
  public function addCredits($entity, $credits) {
    $entity->credits = $entity->credits + $credits;
    return $this->save($entity);
  }

  /**
   * Remove credits.
   *
   * In case the resulting credits number is lower than 0, the entity will be
   * removed from the database.
   *
   * @param $entity
   *  The credits entity from which the credits will be removed.
   * @param $credits
   *  The amount of credits to be removed.
   *
   * @return
   *  The credits entity, if not deleted, else FALSE.
   */
  public function removeCredits($entity, $credits) {
    $entity->credits = $entity->credits - $credits;
    if ($entity->credits <= 0) {
      $this->delete(array($entity->cid));
      return FALSE;
    }
    else {
      return $this->save($entity);
    }
  }

  /**
   * Transfer credits from one entity to another
   * 
   * @param $from_entity
   *  The wrapped entity from which the credits will be taken.
   * @param $to_entity
   *  To wrapped entity to which the credits will be transferred.
   * @param $credit_type
   *  To id of the credit type name.
   * @param $credits
   *  The number of credits to transfer, use -1 to transfer all credits.
   *
   * @return
   *  The credits entity to which the credits have been transferred in case
   *  everything went well, else FALSE.
   */
  public function transferCredits(EntityDrupalWrapper $from_entity, EntityDrupalWrapper $to_entity, $credit_type, $credits = 1) {
    // Make sure cid exists.
    $from_credits = $this->loadByEntity($from_entity->getIdentifier(), $from_entity->type(), $credit_type);
    if ($from_credits) {
      $from_credits = reset($from_credits);
      $credits = ($credits == -1) ? $from_credits->credits : $credits;
      if ($from_credits->credits < $credits) {
        // @todo maybe throw exception.
        return FALSE;
      }
      $to_credits = $this->loadByEntity($to_entity->getIdentifier(), $to_entity->type(), $from_credits->credit_type);
      // Create a new credits entity if it doesn't yet exist.
      if (!$to_credits) {
        $to_credits = $this->createByEntity($to_entity->getIdentifier(), $to_entity->type(), $from_credits->credit_type);
      }
      else {
        $to_credits = reset($to_credits);
      }
      $this->addCredits($to_credits, $credits);
      $this->removeCredits($from_credits, $credits);
      return $to_credits;
    }
    return FALSE;
  }
}
