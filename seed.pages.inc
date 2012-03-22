<?php

/**
 * @file
 * Menu Callbacks for seed module.
 */

define('SEED_TIME_FORMAT_LONG', 'l, F j, Y - H:i:s');

/**
 * Lists seed information in a table.
 */
function seed_list($form, &$form_state) {
  $header = array(
    array('data' => t('SID')),
    array('data' => t('TID')),
    array('data' => t('UID')),
    array('data' => t('Seed Timestamp')),
    array('data' => t('Hold Timestamp')),
    array('data' => t('Used Timestamp')),
    array('data' => t('Used')),
    array('data' => t('Status')),
  );

  $rows = array();
  $seeds = db_select('seed', 's')
    ->fields('s')
    ->execute()
    ->fetchAll();

  foreach ( $seeds as $seed ) {
    if ( $seed->tid && $term = taxonomy_term_load($seed->tid) ) {
      $seed->term = $term;
    }
    if ( $seed->uid && $user = user_load($seed->uid) ) {
      $seed->user = $user;
    }
    $rows[] = array(
      $seed->sid,
      isset($seed->term->name) ? $seed->term->name : t('Uncategorized'),
      isset($seed->user->name) ? l($seed->user->name, 'user/' . $seed->user->uid) : t('No user.'),
      format_date($seed->seed_timestamp, 'custom', SEED_TIME_FORMAT_LONG),
      $seed->hold_timestamp > 0 ? format_date($seed->hold_timestamp, 'custom', SEED_TIME_FORMAT_LONG) : 0,
      $seed->used_timestamp > 0 ? format_date($seed->used_timestamp, 'custom', SEED_TIME_FORMAT_LONG) : 0,
      $seed->used ? t('Yes') : t('No'),
      $seed->status ? t('Available') : t('Not available'),
    );
  }

  $form['seeds'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No seeds.'),
  );

  return $form;
}