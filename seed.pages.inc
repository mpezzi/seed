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
    $rows[] = array(
      $seed->sid,
      isset($seed->term->name) ? $seed->term->name : t('Uncategorized'),
      $seed->uid,
      format_date($seed->seed_timestamp, 'custom', SEED_TIME_FORMAT_LONG),
      $seed->hold_timestamp > 0 ? format_date($seed->hold_timestamp, 'custom', SEED_TIME_FORMAT_LONG) : 0,
      $seed->used_timestamp > 0 ? format_date($seed->used_timestamp, 'custom', SEED_TIME_FORMAT_LONG) : 0,
      $seed->used ? 'Yes' : 'No',
      $seed->status,
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