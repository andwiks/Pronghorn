<?php

/**
 * @file
 */

class TopupEntityController extends EntityAPIController {

  public function create(array $values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );
    return parent::create($values);
  }
}
