<?php

/**
 * Implements hook_views_data().
 */
function seed_views_data() {
  $data['seed']['table']['group'] = t('Seed');
  $data['seed']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('Seed'),
    'help' => t('Seed data.'),
    'access query tag' => 'user_access',
  );

  $data['seed']['sid'] = array(
    'title' => t('Sid'),
    'help' => t('Seed ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['seed']['tid'] = array(
    'title' => t('Tid'),
    'help' => t('Category ID'),
    'relationship' => array(
      'base' => 'taxonomy_term_data',
      'field' => 'tid',
      'handler' => 'views_handler_relationship',
      'label' => t('Term ID'),
    ),
  );

  $data['seed']['uid'] = array(
    'title' => t('Uid'),
    'help' => t('User currently holding seed'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User ID'),
    ),
  );

  $data['seed']['name'] = array(
    'title' => t('Name'),
    'help' => t('Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['seed']['seed_timestamp'] = array(
    'title' => t('Seed Timestamp'),
    'help' => t('Seed Timestamp'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['seed']['hold_timestamp'] = array(
    'title' => t('Hold Timestamp'),
    'help' => t('Hold Timestamp'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['seed']['used_timestamp'] = array(
    'title' => t('Used Timestamp'),
    'help' => t('Used Timestamp'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['seed']['used_token'] = array(
    'title' => t('Token'),
    'help' => t('Token'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['seed']['used'] = array(
    'title' => t('Used'),
    'help' => t('Has the seed been used?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Used'),
      'type' => 'yes-no',
    ),
  );

  $data['seed']['status'] = array(
    'title' => t('Status'),
    'help' => t('Seed Status'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}

