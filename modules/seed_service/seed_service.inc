<?php

/**
 * @file
 * Callbacks and access callbacks for seed services integration.
 */


/**
 * Access callback for seeds.
 */
function seed_service_access($string = NULL) {
  return TRUE;
}

/**
 * Sanitize seed results.
 */
function seed_service_seed_sanitize($seed) {
  unset($seed->uid);
  unset($seed->used_timestamp);
  unset($seed->hold_timestamp);
  unset($seed->seed_timestamp);

  return $seed;
}

/**
 * Returns the results of a seed_load() for the specified seed.
 *
 * @param $sid
 *   SID of the seed we want to return.
 * @return
 *   Seed object or FALSE if not found.
 *
 * @see seed_load()
 */
function seed_service_retrieve($sid) {
  $seed = seed_load($sid);
  $seed = seed_service_seed_sanitize($seed);

  return $seed;
}

/**
 * Return an array of optionally paged sids based on a set of criteria.
 */
function seed_service_index($tid = NULL) {
  $seed = seed_find($tid);
  $seed = seed_service_seed_sanitize($seed);

  return $seed;
}

/**
 * Return an array of optionally paged sids based on a set of criteria.
 */
function seed_service_find($tid = NULL) {
  $seed = seed_find($tid);
  $seed = seed_service_seed_sanitize($seed);

  return $seed;
}
