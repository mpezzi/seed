<?php

/**
 * @file
 * Provides seed resources for services module.
 */


/**
 * Returns the results of seed_load() for the specified seed.
 *
 * @param $sid
 *   A Seed ID.
 * @return
 *   Seed object or FALSE if not found.
 */
function _seed_resource_retrieve($sid) {
  $seed = seed_load($sid);

  if ( $seed ) {
    // @TODO: Sanitize the seed element?
  }

  return $seed;
}

/**
 * Creates a new seed based on submitted values.
 *
 * @param $seed
 *   Object representing the attributes of a seed.
 * @return
 *   Seed object containing the new sid.
 */
function _seed_resource_create($seed) {
  if ( !isset($seed['seed_timestamp']) ) {
    return services_error(t('Seed timestamp is required.'));
  }

  if ( !isset($seed['name']) ) {
    return services_error(t('Seed name parameter is required.'));
  }

  $seed = (object) $seed;
  seed_save($seed);

  return $seed;
}

/**
 * Updates a seed based on submitted values.
 *
 * @param $sid
 *   A Seed ID.
 * @param $seed
 *   Object representing the attributes of a seed.
 */
function _seed_resource_update($sid, $update_values) {
  if ( $seed = seed_load($sid) ) {
    $update_values = (object) $update_values;
    $update_values->sid = $seed->sid;
    seed_save($update_values);
  }

  return $update_values;
}

/**
 * Delete a seed given its sid.
 *
 * @param $sid
 *   A Seed ID.
 */
function _seed_resource_delete($sid) {
  seed_delete($sid);
  return TRUE;
}

/**
 * Return an array of optionally paged sids based on a set of criteria.
 */
function _seed_resource_index() {
  $result = db_select('seed', 's')
    ->fields('s')
    ->orderBy('seed_timestamp')
    ->execute();

  $items = array();
  foreach ( $result as $seed ) {
    $items[] = $seed;
  }

  return $items;
}

/**
 * Find a seed.
 *
 * @param $tid
 *   (optional) A Category ID.
 * @param $hold
 *   Whether to hold the seed for the user.
 * @return
 *   A seed object or FALSE.
 */
function _seed_resource_find($tid = NULL, $hold = TRUE) {
  return seed_find($tid, $hold);
}

/**
 * Use a seed.
 *
 * @param $sid
 *   A Seed ID.
 * @return
 *   A Seed Object or FALSE if unable to use.
 */
function _seed_resource_use($sid) {
  return seed_use($sid);
}

/**
 * Replant seeds that have not expired.
 */
function _seed_resource_replant() {
  seed_replant();
}

/**
 * Determine whether the current user can access a seed resource.
 *
 * @param $op
 *   A operation type of view, update, create, delete, find, use and replant.
 * @param $args
 *   Resource arguments passed through from the original request.
 * @return
 *   Boolean TRUE if the current user has the requested permissions. 
 */
function _seed_resource_access($op = 'retrieve', $args = array()) {
  if ( $op == 'create' || $op == 'retrieve' || $op == 'update' || $op == 'delete' || $op == 'index' ) {
    return user_access('administer seeds');
  }

  if ( $op == 'find' || $op == 'use' || $op == 'replant' ) {
    return user_access('use seeds');
  }
}