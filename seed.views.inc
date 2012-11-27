<?php

/**
 * Implements hook_views_data().
 */
function seed_views_data() {
  $data['seed']['table']['group'] = t('Seed');
  $data['seed']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Seed'),
    'help' => t('Seed data.'),
    // 'access query tag' => 'user_access',
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

  $data['seed']['seed_timestamp_end'] = array(
    'title' => t('Seed Timestamp End'),
    'help' => t('Seed Timestamp End'),
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

  $data['seed']['hold_timestamp_end'] = array(
    'title' => t('Hold Timestamp End'),
    'help' => t('Hold Timestamp End'),
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

  $data['seed']['replant'] = array(
    'title' => t('Replantable'),
    'help' => t('Is the seed replantable?'),
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

  return $data;
}

/**
 * Implements hook_views_default_views().
 */
function seed_views_default_views() {
  return array(
    'admin_seed_overview' => seed_views_default_views_admin(),
  );
}

/**
 * Returns an Administrative Seeds View.
 */
function seed_views_default_views_admin() {
  $view = new view();
  $view->name = 'admin_seed_overview';
  $view->description = 'Provides an overview of seeds.';
  $view->tag = 'default';
  $view->base_table = 'seed';
  $view->human_name = 'Seed Overview';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Seeds';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access site reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'sid' => 'sid',
    'name_1' => 'name_1',
    'name_2' => 'name_2',
    'name' => 'name',
    'seed_timestamp' => 'seed_timestamp',
    'seed_timestamp_end' => 'seed_timestamp_end',
    'hold_timestamp' => 'hold_timestamp',
    'hold_timestamp_end' => 'hold_timestamp_end',
    'replant' => 'replant',
    'used_timestamp' => 'used_timestamp',
    'used_token' => 'used_token',
    'used' => 'used',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'sid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'seed_timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'seed_timestamp_end' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hold_timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hold_timestamp_end' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'replant' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'used_timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'used_token' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'used' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'Currently no seeds available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Seed: Tid */
  $handler->display->display_options['relationships']['tid']['id'] = 'tid';
  $handler->display->display_options['relationships']['tid']['table'] = 'seed';
  $handler->display->display_options['relationships']['tid']['field'] = 'tid';
  /* Relationship: Seed: Uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'seed';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Seed: Sid */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'seed';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name_1']['label'] = 'User';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
  $handler->display->display_options['fields']['name_2']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_2']['field'] = 'name';
  $handler->display->display_options['fields']['name_2']['relationship'] = 'tid';
  $handler->display->display_options['fields']['name_2']['label'] = 'Term';
  /* Field: Seed: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'seed';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Seed: Seed Timestamp */
  $handler->display->display_options['fields']['seed_timestamp']['id'] = 'seed_timestamp';
  $handler->display->display_options['fields']['seed_timestamp']['table'] = 'seed';
  $handler->display->display_options['fields']['seed_timestamp']['field'] = 'seed_timestamp';
  $handler->display->display_options['fields']['seed_timestamp']['date_format'] = 'long';
  /* Field: Seed: Seed Timestamp End */
  $handler->display->display_options['fields']['seed_timestamp_end']['id'] = 'seed_timestamp_end';
  $handler->display->display_options['fields']['seed_timestamp_end']['table'] = 'seed';
  $handler->display->display_options['fields']['seed_timestamp_end']['field'] = 'seed_timestamp_end';
  $handler->display->display_options['fields']['seed_timestamp_end']['date_format'] = 'long';
  /* Field: Seed: Hold Timestamp */
  $handler->display->display_options['fields']['hold_timestamp']['id'] = 'hold_timestamp';
  $handler->display->display_options['fields']['hold_timestamp']['table'] = 'seed';
  $handler->display->display_options['fields']['hold_timestamp']['field'] = 'hold_timestamp';
  $handler->display->display_options['fields']['hold_timestamp']['date_format'] = 'long';
  /* Field: Seed: Hold Timestamp End */
  $handler->display->display_options['fields']['hold_timestamp_end']['id'] = 'hold_timestamp_end';
  $handler->display->display_options['fields']['hold_timestamp_end']['table'] = 'seed';
  $handler->display->display_options['fields']['hold_timestamp_end']['field'] = 'hold_timestamp_end';
  $handler->display->display_options['fields']['hold_timestamp_end']['date_format'] = 'long';
  /* Field: Seed: Replantable */
  $handler->display->display_options['fields']['replant']['id'] = 'replant';
  $handler->display->display_options['fields']['replant']['table'] = 'seed';
  $handler->display->display_options['fields']['replant']['field'] = 'replant';
  $handler->display->display_options['fields']['replant']['not'] = 0;
  /* Field: Seed: Used Timestamp */
  $handler->display->display_options['fields']['used_timestamp']['id'] = 'used_timestamp';
  $handler->display->display_options['fields']['used_timestamp']['table'] = 'seed';
  $handler->display->display_options['fields']['used_timestamp']['field'] = 'used_timestamp';
  $handler->display->display_options['fields']['used_timestamp']['date_format'] = 'long';
  /* Field: Seed: Token */
  $handler->display->display_options['fields']['used_token']['id'] = 'used_token';
  $handler->display->display_options['fields']['used_token']['table'] = 'seed';
  $handler->display->display_options['fields']['used_token']['field'] = 'used_token';
  /* Field: Seed: Used */
  $handler->display->display_options['fields']['used']['id'] = 'used';
  $handler->display->display_options['fields']['used']['table'] = 'seed';
  $handler->display->display_options['fields']['used']['field'] = 'used';
  $handler->display->display_options['fields']['used']['not'] = 0;
  /* Field: Seed: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'seed';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Available';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Seed: Seed Timestamp */
  $handler->display->display_options['sorts']['seed_timestamp']['id'] = 'seed_timestamp';
  $handler->display->display_options['sorts']['seed_timestamp']['table'] = 'seed';
  $handler->display->display_options['sorts']['seed_timestamp']['field'] = 'seed_timestamp';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/seeds';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Seeds';
  $handler->display->display_options['menu']['description'] = 'View a list of seeds currently stored in the system.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['admin_seed_overview'] = array(
    t('Master'),
    t('Seeds'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Currently no seeds available.'),
    t('Term ID'),
    t('User ID'),
    t('Sid'),
    t('.'),
    t(','),
    t('User'),
    t('Term'),
    t('Name'),
    t('Seed Timestamp'),
    t('Seed Timestamp End'),
    t('Hold Timestamp'),
    t('Hold Timestamp End'),
    t('Replantable'),
    t('Used Timestamp'),
    t('Token'),
    t('Used'),
    t('Available'),
    t('Page'),
  );

  return $view;
}