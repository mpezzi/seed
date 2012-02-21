<?php

/**
 * @file
 * Provides seed service resources.
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
 * Determine whether the current user can access a seed resource.
 *
 * @param $op
 *   A operation type of view, update, create, delete, find, use and replant.
 * @param $args
 *   Resource arguments passed through from the original request.
 * @return
 *   Boolean TRUE if the current user has the requested permissions. 
 */
function _seed_resource_access($op = 'view', $args = array()) {
  return TRUE;
}