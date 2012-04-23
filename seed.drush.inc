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
        'tid' => dt('Category ID'),
        'name' => dt('Seed Name'),
        // @TODO: Limit generated seeds for daytime only.
      ),
      'aliases' => array('sgr'),
    ),
  );
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
 */
function drush_seed_generate_random($start, $end, $window = 86400) {
  if ( !is_numeric($start) ) {
    $start = strtotime($start);
  }

  if ( !is_numeric($end) ) {
    $end = strtotime($end);
  }

  if ( !is_numeric($window) ) {
    $window = strtotime($window);
  }

  // Calculate the number of seeds required.
  $seeds = floor( ( $end - $start ) / $window );

  // Show information about seeds being generated.
  drush_log(t('Generating seeds ...'), 'success');
  drush_log(t('Starts: @start', array('@start' => format_date($start, 'long'))), 'success');
  drush_log(t('Ends: @end', array('@end' => format_date($end, 'long'))), 'success');
  drush_log(t('Window: @window', array('@window' => format_interval($window))), 'success');

  for ( $i = 0; $i < $seeds; $i++ ) {
    $min = $start + ( $window * $i );
    $max = $start + ( $window * $i ) + $window;
    $seed_timestamp = rand($min, $max);

    // Set seed name.
    if ( drush_get_option('name') ) {
      $name = drush_get_option('name');
    }
    else {
      $name = 'generated';
    }

    // Set seed tid.
    if ( drush_get_option('tid') ) {
      $tid = drush_get_option('tid');
    }
    else {
      $tid = NULL;
    }

    // Create and save seed.
    $seed = (object) array(
      'tid' => $tid,
      'seed_timestamp' => $seed_timestamp,
      'name' => $name . '_' . ( $i + 1 ),
      'status' => 1,
    );

    seed_save($seed);
  }

  drush_log(format_plural($i, 'Successfully generated 1 seed.', 'Successfully generated @count seeds.'), 'success');
}