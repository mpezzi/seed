<?php

/**
 * @file
 * Provides seed functionality.
 */


/**
 * Clear all seeds.
 */
function seed_clear_all() {
  db_truncate('seed')->execute();
  drupal_set_message(t('Cleared all seeds'));
}

/**
 * Generate random seeds.
 */
function seed_generate_random($params = array()) {
  $params += array(
    'start' => 0,
    'end' => 0,
    'window' => 86400,
    'seed_life' => 0,
    'hold_life' => 0,
    'name' => 'seed',
    'tid' => NULL,
    'time' => time(),
    'replant' => 0,
    'status' => 1,
  );

  // Parse english textual datetime description arguments to unix timestamps.
  foreach ( $params as $key => $value ) {
    if ( in_array($key, array('start', 'end')) ) {
      if ( !is_numeric($params[$key]) ) {
        $params[$key] = strtotime($params[$key], $params['time']);
      }
    }
  }

  // Parse english textual datetime description arguments to unix timestamps.
  // we also need to subtract the time for string arguments.
  foreach ( $params as $key => $value ) {
    if ( in_array($key, array('window', 'seed_life', 'hold_life')) ) {
      if ( !is_numeric($params[$key]) ) {
        $params[$key] = strtotime($params[$key], $params['time']) - $params['time'];
      }
    }
  }

  // Determine the number of seeds required.
  $num_seeds = floor( ( $params['end'] - $params['start'] ) / $params['window'] );

  // Create each seed and keep track of any errors in $status.
  $status = array();
  for ( $i = 0; $i < $num_seeds; $i++ ) {
    $min = $params['start'] + ( $params['window'] * $i );
    $max = $params['start'] + ( $params['window'] * $i ) + $params['window'];
    $seed_timestamp = rand($min, $max);

    $seed = (object) array(
      'tid' => $params['tid'],
      'name' => $params['name'] . '_' . ( $i + 1 ),
      'seed_timestamp' => $seed_timestamp,
      'seed_timestamp_end' => $params['seed_life'] ? $seed_timestamp + $params['seed_life'] : 0,
      'hold_life' => $params['hold_life'],
      'replant' => $params['replant'],
      'status' => $params['status'],
    );

    $status[] = seed_save($seed);
  }

  // Determine if any of the seeds we saved had an issue.
  return !array_search(FALSE, $status, TRUE);
}

/**
 * Find a seed.
 *
 * @param $tid
 *   A term id.
 * @param $hold
 *   Whether to hold the seed for the user.
 * @return
 *   A seed object, or FALSE if the seed is not found.
 */
function seed_find($params = array()) {
  //dpm(__FUNCTION__);

  $params += array(
    'tid' => NULL,
    'hold' => TRUE,
    'time' => time(),
    'account' => $GLOBALS['user'],
  );

  if ( is_null($params['account']) ) {
    $params['account'] = $GLOBALS['user'];
  }

  // If account doesn't have access, stop now.
  if ( !seed_find_access($params['account']) ) {
    return FALSE;
  }

  // @TODO: Perhaps this should run on cron?
  // 1. Cultivate (or clean) out any old or stale seeds.
  seed_cultivate($params['time']);

  // 2. Find a seed currently held by the user.
  $seed = seed_find_held($params);
  //$seed = FALSE;

  // 3. If no seed was found, look for a new seed without any limits.
  if ( $seed === FALSE ) {
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

    $seed = $query->execute()->fetchObject();
  }

  // 4. If no seed was found, look for a new seed with limits.
  if ( $seed === FALSE ) {
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

    $seed = $query->execute()->fetchObject();
  }

  // 5. If a seed was found, update some information on it.
  if ( $seed !== FALSE ) {

    // The first time the seed is found, send a watchdog notice.
    if ( $params['hold'] && $seed->uid == 0 ) {
      watchdog('seed', 'Seed @seedname was found by @username.', array('@seedname' => $seed->name, '@username' => $params['account']->name), WATCHDOG_NOTICE, l('user', 'user/' . $params['account']->uid));

      // Update seed to be marked as held.
      $seed->hold_timestamp = $params['time'];
      $seed->hold_timestamp_end = $params['time'] + $seed->hold_life;
      $seed->uid = $params['account']->uid;
      $seed->status = 0;
      seed_save($seed);
    }

    return $seed;
  }

  // 6. If we got this far, there wasn't a seed available.
  return FALSE;
}

/**
 * Returns a seed held by a user.
 */
function seed_held($params = array()) {
  $params += array(
    'tid' => NULL,
    'hold' => TRUE,
    'time' => time(),
    'account' => $GLOBALS['user'],
  );

  if ( is_null($params['account']) ) {
    $params['account'] = $GLOBALS['user'];
  }

  // If account doesn't have access, stop now.
  if ( !seed_find_access($params['account']) ) {
    return FALSE;
  }

  // @TODO: Perhaps this should run on cron?
  // 1. Cultivate (or clean) out any old or stale seeds.
  seed_cultivate($params['time']);

  return seed_find_held($params);
}

/**
 * Cultivate (or clean) expired seeds.
 */
function seed_cultivate($time = NULL) {
  //dpm(__FUNCTION__);

  // Replant held seeds that have expired.
  if ( variable_get('seed_replant', TRUE) ) {
    seed_replant($time);
  }

  // Cull held seeds that have expired.
  if ( variable_get('seed_pluck', TRUE) ) {
    seed_pluck($time);
  }
}

/**
 * Replant seeds so they can be found again.
 */
function seed_replant($time = NULL) {
  if ( is_null($time) ) {
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

  foreach ( $seeds as $seed ) {
    // Invoke hook_seed_replant().
    module_invoke_all('seed_replant', $seed);

    $seed->status = 1;
    $seed->uid = 0;
    $seed->hold_timestamp = 0;
    $seed->hold_timestamp_end = 0;
    seed_save($seed);
  }

  if ( count($seeds) > 0 ) {
    watchdog('seed', 'Replanted @seeds expired seeds.', array('@seeds' => count($seeds)), WATCHDOG_NOTICE);
  }
}

/**
 * Pluck expired seeds so the can't be found again.
 */
function seed_pluck($time = NULL) {
  if ( is_null($time) ) {
    $time = time();
  }

  $seeds = db_select('seed', 's')
    ->fields('s', array())
    ->condition('hold_timestamp_end', $time, '<')
    ->condition('replant', 0)
    ->condition('used', 0)
    ->condition('status', 0)
    ->execute()
    ->fetchAll();

  foreach ( $seeds as $seed ) {
    // Invoke hook_seed_pluck().
    module_invoke_all('seed_pluck', $seed);

    $seed->status = 0;
    $seed->uid = 0;
    $seed->used = 1;
    $seed->used_timestamp = $time;
    seed_save($seed);
  }

  if ( count($seeds) > 0 ) {
    watchdog('seed', 'Plucked @seeds expired seeds.', array('@seeds' => count($seeds)), WATCHDOG_NOTICE);
  }
}

/**
 * Find a seed currently being held by a user.
 *
 * @param $account
 *   A user object.
 * @return
 *   TRUE if a seed is found, FALSE otherwise.
 */
function seed_find_held($params) {
  //dpm(__FUNCTION__);

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

  return $query->execute()->fetchObject();
}



/**
 * Returns weather the account has access to find a seed.
 *
 * @param $account
 *   A user object.
 * @return
 *   TRUE if the account has access, otherwise FALSE
 */
function seed_find_access($account) {
  // Ensure user account has permission to use seeds.
  if ( !user_access('use seeds', $account) ) {
    return FALSE;
  }

  // Does the user have a role that is to be ignored?
  foreach ( variable_get('seed_ignored_roles', array()) as $id ) {
    if ( $id != 0 && array_key_exists($id, $account->roles) ) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Drop a seed.
 */
function seed_drop($sid = NULL) {
  global $user;

  if ( is_null($sid) ) {
    $seed = seed_held();
  }
  else {
    $seed = seed_load($sid, TRUE);
  }

  if ( $seed ) {
    $account = user_load($seed->uid);

    if ( $seed->replant ) {
      $seed->status = 1;
      $seed->hold_timestamp = 0;
      $seed->hold_timestamp_end = 0;
      $seed->uid = 0;
    }
    else {
      $seed->used = 0;
      $seed->status = 0;
      $seed->uid = 0;
    }

    seed_save($seed);

    watchdog('seed', 'Seed @seedname was dropped by @username.', array('@seedname' => $seed->name, '@username' => $account->name), WATCHDOG_NOTICE, l('user', 'user/' . $account->uid));

    return $seed;
  }

  // Something went wrong.
  watchdog('seed', 'unable to drop seed @sid for user @username', array('@sid' => $sid, '@username' => $user->name), WATCHDOG_CRITICAL, l('user', 'user/' . $user->uid));

  return FALSE;
}

/**
 * Use a held seeds and prevent it from being found in the future.
 *
 * @param $sid
 *   A seed id.
 * @return
 *   A seed object or FALSE if unable to use.
 */
function seed_use($sid = NULL, $time = NULL) {
  global $user;

  if ( is_null($sid) ) {
    $seed = seed_held();
  }
  else {
    $seed = seed_load($sid, TRUE);
  }

  if ( is_null($time) ) {
    $time = time();
  }

  if ( $seed ) {
    $account = user_load($seed->uid);

    if ( user_access('use seeds', $account) ) {
      $seed->used = TRUE;
      $seed->used_timestamp = $time;
      $seed->used_token = seed_use_token($seed);
      seed_save($seed);

      // Invoke hook_seed_use().
      module_invoke_all('seed_use', $seed);

      watchdog('seed', 'Successfully used seed @seedname for @username', array('@seedname' => $seed->name, '@username' => $account->name), WATCHDOG_NOTICE, l('user', 'user/' . $account->uid));

      return $seed;
    }
  }

  // Something went wrong.
  watchdog('seed', 'unable to use seed @sid for user @username', array('@sid' => $sid, '@username' => $user->name), WATCHDOG_CRITICAL, l('user', 'user/' . $user->uid));

  return FALSE;
}

/**
 * Returns a seed token.
 */
function seed_use_token($seed) {
  return substr(md5($seed->sid . '-' . $seed->uid . '-' . $seed->used_timestamp), 0, 16);
}

/**
 * Load a seed.
 *
 * @param $sid
 *   A Seed ID.
 * @param $is_held
 *   Only return a seed that is currently being held.
 * @return
 *   A Seed Object.
 */
function seed_load($sid, $is_held = FALSE) {
  $query = db_select('seed', 's')
    ->fields('s')
    ->condition('s.sid', $sid);

  // If looking for held seeds, add extra conditions.
  if ( $is_held ) {
    $query
      ->condition('s.used', 0)
      ->condition('s.status', 0);
  }

  return $query->execute()->fetchObject();
}

/**
 * Load all seeds for a taxonomy term.
 *
 * @param $tid
 *   A Term ID.
 * @return
 *   An array of Seed Objects.
 */
function seed_load_by_tid($tid) {
  $result = db_select('seed', 's')
    ->fields('s')
    ->condition('s.tid', $tid)
    ->execute()
    ->fetchAll();

  return $result;
}

/**
 * Saves a seed.
 */
function seed_save($seed) {
  $seed->is_new = empty($seed->sid);

  if ( !$seed->is_new ) {
    $status = drupal_write_record('seed', $seed, 'sid');
  }
  else {
    $status = drupal_write_record('seed', $seed);
  }

  return $status;
}

/**
 * Delete a seed.
 */
function seed_delete($sid) {
  if ( $seed = seed_load($sid) ) {
    $transaction = db_transaction();
    try {
      db_delete('seed')
        ->condition('sid', $seed->sid)
        ->execute();

      cache_clear_all();
      seed_static_reset();

      return SAVED_DELETED;
    }
    catch ( Exception $e ) {
      $transaction->rollback();
      watchlog_exception('seed', $e);
      throw $e;
    }
  }

  return FALSE;
}

/**
 * Clear all static cache variables for seeds.
 */
function seed_static_reset($ids = NULL) {
  drupal_static_reset('seed_load');
}

/**
 * Return the Vocabulary ID (vid) used by seed for categorization.
 */
function seed_get_vid() {
  if ( !module_exists('taxonomy') ) {
    return FALSE;
  }

  $vid = variable_get(SEED_CATEGORY_DEFAULT_VID, '');
  if ( empty($vid) || !taxonomy_vocabulary_load($vid) ) {
    $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE module = 'seed'")->fetchField();
    if ( !$vid ) {
      drupal_set_message(t('Created Seed vocabulary'));

      $vocab = (object) array(
        'name' => SEED_CATEGORY_NAME,
        'description' => t('Automatically created by the seed module.'),
        'machine_name' => 'seed',
        'multiple' => FALSE,
        'required' => FALSE,
        'hierarchy' => TRUE,
        'relations' => FALSE,
        'module' => 'seed',
      );

      taxonomy_vocabulary_save($vocab);
      $vid = $vocab->vid;
      variable_set(SEED_CATEGORY_DEFAULT_VID, $vid);
    }
  }

  if ( !is_numeric($vid) ) {
    watchdog('seed', 'Seed module was unable to select or create a vocabulary.', array(), WATCHDOG_ERROR);
  }

  return $vid;
}