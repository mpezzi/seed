<?php

/**
 * @file
 * Administrative page callbacks for the seed module.
 */


/**
 * Form builder to list and manage seed terms.
 */
function seed_ui_overview_terms($form) {
  $terms = taxonomy_get_tree(seed_get_vid());
  $form['#tree'] = TRUE;

  foreach ( $terms as $term ) {
    $form[$term->tid]['#term'] = $term;
    $form[$term->tid]['name'] = array('#markup' => check_plain($term->name));
    $form[$term->tid]['list'] = array('#type' => 'link', '#title' => t('list seeds'), '#href' => "admin/structure/seed/$term->tid");
    $form[$term->tid]['add'] = array('#type' => 'link', '#title' => t('add seeds'), '#href' => "admin/structure/seed/$term->tid/add");
  }

  return $form;
}

/**
 * Returns HTML for the terms ovewview for seeds.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see seed_ui_overview_terms()
 * @ingroup themeable
 */
function theme_seed_ui_overview_terms($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach ( element_children($form) as $key ) {
    if ( isset($form[$key]['name']) ) {
      $term = &$form[$key];

      $row = array();
      $row[] = drupal_render($term['name']);
      $row[] = drupal_render($term['list']);
      $row[] = drupal_render($term['add']);
      $rows[] = $row;
    }
  }

  $header = array(t('Term name'));
  $header[] = array('data' => t('Operations'), 'colspan' => '2');

  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No terms available. <a href="@link">Add term</a>.', array('@link' => url('admin/structure/taxonomy/seed/add'))))) . drupal_render_children($form);
}

/**
 * Form builder to list and manage seeds.
 */
function seed_ui_overview_seeds($form, &$form_state, $term) {
  $seeds = seed_load_by_tid($term->tid);

  $form['#term'] = $term;

  foreach ( $seeds as $seed ) {
    $form[$seed->sid]['#seed'] = $seed;
    $form[$seed->sid]['name'] = array('#markup' => check_plain($seed->name));
    $form[$seed->sid]['seed_timestamp'] = array('#markup' => $seed->seed_timestamp ? format_date($seed->seed_timestamp) : t('None'));
    $form[$seed->sid]['hold_timestamp'] = array('#markup' => $seed->hold_timestamp ? format_date($seed->hold_timestamp) : t('None'));
    $form[$seed->sid]['used_timestamp'] = array('#markup' => $seed->used_timestamp ? format_date($seed->used_timestamp) : t('None'));
    $form[$seed->sid]['view'] = array('#type' => 'link', '#title' => t('view'), '#href' => "seed/$seed->sid");
    $form[$seed->sid]['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => "seed/$seed->sid/edit");
    $form[$seed->sid]['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => "seed/$seed->sid/delete");
  }

  return $form;
}

/**
 * Returns HTML for the seeds ovewview for seeds.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see seed_ui_overview_seeds()
 * @ingroup themeable
 */
function theme_seed_ui_overview_seeds($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach ( element_children($form) as $key ) {
    if ( isset($form[$key]['name']) ) {
      $seed = &$form[$key];

      $row = array();
      $row[] = drupal_render($seed['name']);
      $row[] = drupal_render($seed['seed_timestamp']);
      $row[] = drupal_render($seed['hold_timestamp']);
      $row[] = drupal_render($seed['used_timestamp']);
      $row[] = drupal_render($seed['view']);
      $row[] = drupal_render($seed['edit']);
      $row[] = drupal_render($seed['delete']);
      $rows[] = $row;
    }
  }

  $header = array(t('Seed name'), t('Seed Timestamp'), t('Hold Timestamp'), t('Used Timestamp'));
  $header[] = array('data' => t('Operations'), 'colspan' => '3');

  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No seeds available. <a href="@link">Add seed</a>.', array('@link' => url('admin/structure/seed/' . $form['#term']->tid . '/add'))))) . drupal_render_children($form);
}

/**
 * Form function for the seed edit form.
 *
 * @ingroup forms
 * @see seed_ui_form_term_submit()
 */
function seed_ui_form_seed($form, &$form_state, $edit = array(), $term = NULL) {
  if ( !isset($form_state['seed']) ) {
    $seed = is_object($edit) ? $edit : (object) $edit;
    if ( !isset($term) && isset($seed->tid) ) {
      $term = taxonomy_term_load($seed->tid);
    }
    $defaults = array(
      'tid' => $term->tid,
      'uid' => NULL,
      'name' => '',
      'seed_timestamp' => NULL,
      'hold_timestamp' => NULL,
      'used_timestamp' => NULL,
      'used_token' => NULL,
      'used' => 0,
      'status' => 1,
    );
    foreach ( $defaults as $key => $value ) {
      if ( !isset($seed->$key) ) {
        $seed->$key = $value;
      }
    }
    $form_state['seed'] = $seed;
  }

  $form['#seed'] = (array) $seed;
  $form['#term'] = $term;

  $form['sid'] = array(
    '#type' => 'value',
    '#value' => isset($seed->sid) ? $seed->sid : NULL,
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $seed->uid,
  );
  $form['tid'] = array(
    '#type' => 'value',
    '#value' => $seed->tid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $seed->name,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  $form['seed_timestamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Seed Timestamp'),
    '#default_value' => $seed->seed_timestamp,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -1,
  );
  $form['hold_timestamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Hold Timestamp'),
    '#default_value' => $seed->hold_timestamp,
    '#maxlength' => 255,
    '#weight' => 0,
  );
  $form['used_timestamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Used Timestamp'),
    '#default_value' => $seed->used_timestamp,
    '#maxlength' => 255,
    '#weight' => 0,
  );
  $form['used_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Used Token'),
    '#default_value' => $seed->used_token,
    '#maxlength' => 255,
    '#weight' => 1,
  );
  $form['used'] = array(
    '#type' => 'checkbox',
    '#title' => t('Used'),
    '#default_value' => $seed->used,
    '#weight' => 5,
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status'),
    '#default_value' => $seed->status,
    '#weight' => 6,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 5,
    ),
  );

  if ( isset($seed->sid) ) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 10,
    );
  }

  return $form;
}

/**
 * Submit handler to insert or update a seed.
 */
function seed_ui_form_seed_submit($form, &$form_state) {
  $seed = (object) $form_state['values'];

  $status = seed_save($seed);
  switch ( $status ) {
    case SAVED_NEW:
      drupal_set_message(t('Created new seed %name', array('%name' => $seed->name)));
      watchdog('seed', 'Created new seed %name', array('%name' => $seed->name), WATCHDOG_NOTICE);
      break;
    case SAVED_UPDATED:
      drupal_set_message(t('Updated new seed %name', array('%name' => $seed->name)));
      watchdog('seed', 'Updated new seed %name', array('%name' => $seed->name), WATCHDOG_NOTICE);
      break;
  }
}

/**
 * Title callback for seed pages.
 *
 * @param $seed
 *   A seed object.
 * @return
 *   The seed name to be used as the page title.
 */
function seed_ui_title($seed) {
  return $seed->name;
}

/**
 * Menu callback; displays seed information.
 *
 * @param $seed
 *   A seed object.
 * @return
 *   The page content.
 */
function seed_ui_page($seed) {
  drupal_set_title($seed->name);

  $build = array();
  $build['debug'] = array(
    '#markup' => '<pre>' . print_r($seed, 1) . '</pre>',
  );

  return $build;
}