<?php

/**
 * @file
 * Provides administration settings for seed module.
 */


/**
 * Returns a Seed administrative configuration form.
 */
function seed_admin_settings($form, &$form_state) {
  $form['seed_ignored_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Ignore finding seeds for the following roles'),
    '#description' => t('These roles will not be able to find seeds.'),
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('seed_ignored_roles', array()),
  );

  return system_settings_form($form);
}
