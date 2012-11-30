<?php

/**
 * @file
 * Provides Drush functions.
 */


/**
 * Implementation of hook_drush_command().
 */
function seed_drush_command() {
  return array(
    'seed-clear-all' => array(
      'description' => 'Clear all seeds.',
      'drupal dependencies' => array('seed'),
      'aliases' => array('scc'),
    ),
    'seed-generate-random' => array(
      'description' => 'Generate randomized seed times.',
      'drupal dependencies' => array('seed'),
      'arguments' => array(
        'start' => 'Start time.',
        'end' => 'End time.',
        'window' => 'Window to randomize between.',
      ),
      'options' => array(
        'name' => dt('Seed Name'),
        'tid' => dt('Category ID'),
        'time' => dt('Time'),
        'seed_life' => dt('Seed Life'),
        'hold_life' => dt('Hold Life'),
        'replant' => dt('Replant'),
        'status' => dt('Status'),
        // @TODO: Limit generated seeds for daytime only.
      ),
      'aliases' => array('sgr'),
    ),
    'seed-generate-test-replant' => array(
      'description' => 'Generates test results',
      'drupal dependencies' => array('seed'),
      'arguments' => array(
        'num_seeds' => 'Number of seeds.',
        'offset' => 'Offset.',
      ),
    ),
    'seed-generate-test-replant-with-limit' => array(
      'description' => 'Generates test results',
      'drupal dependencies' => array('seed'),
      'arguments' => array(
        'num_seeds' => 'Number of seeds.',
        'offset' => 'Offset.',
      ),
    ),
    'seed-generate-test-nonreplant' => array(
      'description' => 'Generates test results',
      'drupal dependencies' => array('seed'),
      'arguments' => array(
        'num_seeds' => 'Number of seeds.',
        'offset' => 'Offset.',
      ),
    ),
  );
}

function drush_seed_generate_test_replant($num_seeds = 10, $offset = 0, $window = 20, $tid = NULL) {
  // Create seeds a set amount appart.
  for ( $i = 0; $i < $num_seeds; $i++ ) {
    $seed = (object) array(
      'tid' => $tid,
      'name' => 'seedtest' . '_' . ( $i + 1 ),
      'seed_timestamp' => ( $i * $window ) + $offset,
      'seed_timestamp_end' => 0,
      'hold_life' => 10,
      'replant' => TRUE,
      'status' => TRUE,
    );
    seed_save($seed);
  }
}

function drush_seed_generate_test_replant_with_limit($num_seeds = 10, $offset = 0, $window = 20, $tid = NULL) {
  // Create seeds a set amount appart.
  for ( $i = 0; $i < $num_seeds; $i++ ) {
    $seed = (object) array(
      'tid' => $tid,
      'name' => 'seedtest' . '_' . ( $i + 1 ),
      'seed_timestamp' => ( $i * $window ) + $offset,
      'seed_timestamp_end' => ( $i * $window ) + $offset + 10,
      'hold_life' => 10,
      'replant' => TRUE,
      'status' => TRUE,
    );
    seed_save($seed);
  }
}

function drush_seed_generate_test_nonreplant($num_seeds = 10, $offset = 0, $window = 20, $tid = NULL) {
  // Create seeds a set amount appart.
  for ( $i = 0; $i < $num_seeds; $i++ ) {
    $seed = (object) array(
      'tid' => $tid,
      'name' => 'seedtest' . '_' . ( $i + 1 ),
      'seed_timestamp' => ( $i * $window ) + $offset,
      'seed_timestamp_end' => ( $i * $window ) + $offset + 10,
      'hold_life' => 10,
      'replant' => FALSE,
      'status' => TRUE,
    );
    seed_save($seed);
  }
}

/**
 * Command callback. Clear all seeds.
 */
function drush_seed_clear_all() {
  if ( drush_confirm(dt('Are you sure you want to clear all the seeds?')) ) {
    seed_clear_all();
  }
  else {
    drush_die('Aborting.');
  }
}

/**
 * Command callback. Generate randomized seed times.
 *
 * @param $start
 *   A period start time, unix timestamp or english textual datetime description.
 * @param $end
 *   A period end time, unix timestamp or english textual datetime description.
 * @param $window
 *   A window duration, unix timestamp or english textual datetime description. Defaults to 24 hours.
 */
function drush_seed_generate_random($start, $end, $window = 86400) {
  $name = drush_get_option('name', 'seed');
  $tid = drush_get_option('tid', NULL);
  $time = drush_get_option('time', time());
  $seed_life = drush_get_option('seed_life', 0);
  $hold_life = drush_get_option('hold_life', 600);
  $replant = drush_get_option('replant', 1);
  $status = drush_get_option('status', 1);

  if ( !is_numeric($start) ) {
    $start = strtotime($start);
  }

  if ( !is_numeric($end) ) {
    $end = strtotime($end);
  }

  if ( !is_numeric($window) ) {
    $window = strtotime($window) - $time;
  }

  // Calculate the number of seeds required.
  $seeds = floor( ( $end - $start ) / $window );

  // Show information about seeds being generated.
  drush_log(t('Generating seeds ...'), 'success');
  drush_log(t('Starts: @start', array('@start' => format_date($start, 'long'))), 'success');
  drush_log(t('Ends: @end', array('@end' => format_date($end, 'long'))), 'success');
  drush_log(t('Window: @window', array('@window' => format_interval($window))), 'success');

  if ( $seed_life > 0 ) {
    drush_log(t('Seed Life: @life', array('@life' => format_interval($seed_life))), 'success');
  }

  if ( $hold_life > 0 ) {
    drush_log(t('Hold Life: @life', array('@life' => format_interval($hold_life))), 'success');
  }

  $seed_generate_status = seed_generate_random(array(
    'start' => $start,
    'end' => $end,
    'window' => $window,
    'seed_life' => $seed_life,
    'hold_life' => $hold_life,
    'name' => $name,
    'tid' => $tid,
    'time' => $time,
    'replant' => $replant,
    'status' => $status,
  ));

  if ( $seed_generate_status ) {
    drush_log(format_plural($seeds, 'Successfully generated 1 seed.', 'Successfully generated @count seeds.'), 'success');
  }
  else {
    drush_log(format_plural($seeds, 'Error generating 1 seed.', 'Error generating @count seeds.'), 'error');
  }
  
}