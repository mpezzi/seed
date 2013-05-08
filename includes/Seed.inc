<?php

/**
 * @file
 * Provides seed functionality for seed.module
 */

/**
 * Class Seed.
 */
class Seed {

  /**
   * Creates an instance of a seed.
   */
  public function __construct($data) {
    $this->data = $data;
  }

  /**
   * Get a data field.
   */
  public function __get($property) {
    if (isset($this->data->$property)) {
      return $this->data->$property;
    }
  }

  /**
   * Set a data field.
   */
  public function __set($property, $value) {
    if ($property == 'data') {
      $this->$property = $value;
    }
    else {
      $this->data->$property = $value;
    }
  }

  public function getData() {
    return $this->data;
  }

  /**
   * Save a seed.
   */
  public function save() {
    if (!empty($this->data->sid)) {
      drupal_write_record('seed', $this->data, 'sid');
    }
    else {
      drupal_write_record('seed', $this->data);
    }
    return $this;
  }

  /**
   * Delete a seed.
   *
   * @return
   *   If the record delete failed, returns FALSE.
   *   If it succeeded, returns SAVED_DELETED.
   */
  public function delete() {
    $transaction = db_transaction();
    try {
      db_delete('seed')
        ->condition('sid', $this->data->sid)
        ->execute();
      cache_clear_all();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('seed', $e);
      throw $e;
    }
    return $this;
  }

  /**
   * Drop a seed.
   */
  public function drop() {
    $account = user_load($this->data->uid);

    if ($this->data->replant) {
      $this->data->status = 1;
      $this->data->hold_timestamp = 0;
      $this->data->hold_timestamp_end = 0;
      $this->data->uid = 0;
    }
    else {
      $this->data->used = 0;
      $this->data->status = 0;
      $this->data->uid = 0;
    }

    $this->save();

    watchdog('seed', 'Seed @seed was dropped by @user.', array(
      '@seed' => $this->data->name,
      '@user' => $account->name,
    ), WATCHDOG_NOTICE, l('user', 'user/' . $account->uid));

    return $this;
  }

  /**
   * Hold a seed.
   */
  public function hold($uid, $time = NULL) {
    if (is_null($time)) {
      $time = time();
    }

    $account = user_load($uid);

    $this->data->uid = $uid;
    $this->data->hold_timestamp = $time;
    $this->data->hold_timestamp_end = $time + $this->data->hold_life;
    $this->data->status = 0;
    $this->save();

    watchdog('seed', 'Seed @seed is held by @user.', array(
      '@seed' => $this->data->name,
      '@user' => $account->name,
    ), WATCHDOG_NOTICE, l('user', 'user/' . $account->uid));

    return $this;
  }

  /**
   * Consume a seed.
   */
  public function consume($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }

    // Only held seeds can be consumed.
    if ($this->data->uid == 0) {
      return $this;
    }

    $account = user_load($this->data->uid);

    $this->data->used = TRUE;
    $this->data->used_timestamp = $time;
    $this->data->used_token = $this->consume_token();
    $this->save();

    watchdog('seed', 'Seed @seed was consumed by @user.', array(
      '@seed' => $this->data->name,
      '@user' => $account->name,
    ), WATCHDOG_NOTICE, l('user', 'user/' . $account->uid));

    return $this;
  }

  /**
   * Replant a seed.
   */
  public function replant() {
    $this->data->uid = 0;
    $this->data->status = 1;
    $this->data->hold_timestamp = 0;
    $this->data->hold_timestamp_end = 0;
    $this->save();

    watchdog('seed', 'Seed @seed was replanted.', array(
      '@seed' => $this->data->name,
    ), WATCHDOG_NOTICE);

    return $this;
  }

  /**
   * Pluck a seed.
   */
  public function pluck($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }

    $this->data->uid = 0;
    $this->data->status = 0;
    $this->data->used = 1;
    $this->data->used_timestamp = $time;
    $this->save();

    watchdog('seed', 'Seed @seed was plucked.', array(
      '@seed' => $this->data->name,
    ), WATCHDOG_NOTICE);

    return $this;
  }

  /**
   * Determine whether a seed is held.
   *
   * @return (bool)
   *   TRUE if the seed is currently held.
   */
  public function isHeld() {
    return ($this->data->uid != 0);
  }

  /**
   * Return a seed consume token.
   *
   * @return (string)
   *   A unique token for the consumed seed.
   */
  protected function consume_token() {
    $data = array(
      $this->data->sid,
      $this->data->uid,
      $this->data->used_timestamp,
    );
    return substr(md5(implode('-', $data)), 0, 16);
  }

}