<?php

/**
 * @file
 * Base Seed class
 */

class SeedAPI {

  protected function __construct() {}

  /**
   * Load a seed.
   */
  public static function load($sid) {
    $data = db_select('seed', 's')
      ->fields('s')
      ->condition('s.sid', $sid)
      ->execute()
      ->fetchObject();
    return $data ? new Seed($data) : FALSE;
  }

  /**
   * Find a seed.
   */
  public static function find(array $params = array()) {
    $params += array(
      'tid' => NULL,
      'hold' => TRUE,
      'time' => time(),
      'account' => $GLOBALS['user'],
    );

    // Clean out any expired seeds.
    static::clean($params['time']);

    // 1. Find a seed current held by the user.
    $seed = static::findHeld($params);

    // 2. If no seed was found, look for a new unheld seed.
    if ($seed === FALSE) {
      $seed = static::findAny($params);
    }

    // 3. If no seed was found, look for a new seed with limits.
    if ($seed === FALSE) {
      $seed = static::findAnyLimited($params);
    }

    // 4. If a seed was found, update some information on it.
    if ($seed !== FALSE) {
      if ($params['hold'] && !$seed->isHeld()) {
        $seed->hold($params['account']->uid, $params['time']);
      }
      return $seed;
    }

    return FALSE;
  }

  /**
   * Find any available seed.
   */
  public static function findAny(array $params = array()) {
    $query = db_select('seed', 's')
      ->fields('s', array())
      ->condition('s.status', 1)
      ->condition('s.seed_timestamp', $params['time'], '<')
      ->condition('s.seed_timestamp_end', 0)
      ->orderBy('s.seed_timestamp')
      ->range(0, 1);

    if ( !is_null($params['tid']) ) {
      $query->condition('s.tid', $params['tid']);
    }

    $data = $query->execute()->fetchObject();

    return $data ? new Seed($data) : FALSE;
  }

  /**
   * Find any available seed.
   */
  public static function findAnyLimited(array $params = array()) {
    $query = db_select('seed', 's')
      ->fields('s', array())
      ->condition('s.status', 1)
      ->condition('s.seed_timestamp', $params['time'], '<')
      ->condition('s.seed_timestamp_end', 0, '<>')
      ->condition('s.seed_timestamp_end', $params['time'], '>')
      ->orderBy('s.seed_timestamp')
      ->range(0, 1);

    if ( !is_null($params['tid']) ) {
      $query->condition('s.tid', $params['tid']);
    }

    $data = $query->execute()->fetchObject();

    return $data ? new Seed($data) : FALSE;
  }

  /**
   * Find a held seed.
   */
  public static function findHeld(array $params = array()) {
    $query = db_select('seed', 's')
      ->fields('s', array())
      ->condition('s.uid', $params['account']->uid)
      ->condition('s.used', 0)
      ->condition('s.status', 0)
      ->orderBy('s.seed_timestamp')
      ->range(0, 1);

    if ( !is_null($params['tid']) ) {
      $query->condition('s.tid', $params['tid']);
    }

    $data = $query->execute()->fetchObject();

    return $data ? new Seed($data) : FALSE;
  }

  /**
   * Clean seeds.
   */
  public static function clean($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }

    if (variable_get('seed_replant', TRUE)) {
      static::replant($time);
    }

    if (variable_get('seed_pluck', TRUE)) {
      static::pluck($time);
    }
  }

  /**
   * Replant seeds.
   */
  public static function replant($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }

    $seeds = db_select('seed', 's')
      ->fields('s', array())
      ->condition('hold_timestamp_end', $time, '<')
      ->condition('replant', 1)
      ->condition('used', 0)
      ->condition('status', 0)
      ->execute()
      ->fetchAll();

    foreach ($seeds as $data) {
      module_invoke_all('seed_replant', $data);
      $seed = new Seed($data);
      $seed->replant();
    }

    if (count($seeds) > 0) {
      watchdog('seed', 'Replanted @count expired seeds.', array(
        '@count' => count($seeds),
      ), WATCHDOG_NOTICE);
    }
  }

  /**
   * Pluck seeds.
   */
  public static function pluck($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }

    // 1. Pluck expired seeds.
    $seeds = db_select('seed', 's')
      ->fields('s', array())
      ->condition('seed_timestamp_end', 0, '<>')
      ->condition('seed_timestamp_end', $time, '<')
      ->condition('replant', 0)
      ->condition('used', 0)
      ->condition('status', 1)
      ->execute()
      ->fetchAll();

    foreach ($seeds as $data) {
      module_invoke_all('seed_pluck', $data);
      $seed = new Seed($data);
      $seed->pluck($time);
    }

    if (count($seeds) > 0) {
      watchdog('seed', 'Plucked @count expired seeds.', array(
        '@count' => count($seeds),
      ), WATCHDOG_NOTICE);
    }

    // 2. Pluck expired held seeds.
    $seeds = db_select('seed', 's')
      ->fields('s', array())
      ->condition('hold_timestamp_end', $time, '<')
      ->condition('replant', 0)
      ->condition('used', 0)
      ->condition('status', 0)
      ->execute()
      ->fetchAll();

    foreach ($seeds as $data) {
      module_invoke_all('seed_pluck', $data);
      $seed = new Seed($data);
      $seed->pluck($time);
    }

    if (count($seeds) > 0) {
      watchdog('seed', 'Plucked @count expired seeds.', array(
        '@count' => count($seeds),
      ), WATCHDOG_NOTICE);
    }
  }

  /**
   * Truncate seed table.
   */
  public static function clear() {
    db_truncate('seed')->execute();
  }
}