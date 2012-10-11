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

  foreach ( $seeds as $seed ) {
    $form[$seed->sid]['#seed'] = $seed;
    $form[$seed->sid]['name'] = array('#markup' => check_plain($seed->name));
    $form[$seed->sid]['seed_timestamp'] = array('#markup' => $seed->seed_timestamp ? format_date($seed->seed_timestamp) : t('None'));
    $form[$seed->sid]['hold_timestamp'] = array('#markup' => $seed->hold_timestamp ? format_date($seed->hold_timestamp) : t('None'));
    $form[$seed->sid]['used_timestamp'] = array('#markup' => $seed->used_timestamp ? format_date($seed->used_timestamp) : t('None'));
    $form[$seed->sid]['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => "admin");
    $form[$seed->sid]['delete'] = array('#type' => 'link', '#title' => t('delete'), '#href' => "admin");
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
      $row[] = drupal_render($seed['edit']);
      $row[] = drupal_render($seed['delete']);
      $rows[] = $row;
    }
  }

  $header = array(t('Seed name'), t('Seed Timestamp'), t('Hold Timestamp'), t('Used Timestamp'));
  $header[] = array('data' => t('Operations'), 'colspan' => '2');

  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No seeds available. <a href="@link">Add seed</a>.', array('@link' => url('admin/structure/taxonomy/seed/add'))))) . drupal_render_children($form);
}