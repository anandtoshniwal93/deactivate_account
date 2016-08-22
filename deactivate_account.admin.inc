<?php

/**
 * @file
 * Contains configuration settings for disable my account module.
 */

/**
 * Callback to admin/config/disable-my-account.
 */
function deactivate_account_config_form($form, &$form_state) {
  $form = array();
  $form['#tree'] = TRUE;
  $deactivate_account_total_options = variable_get('deactivate_account_total_options');
  if (empty($deactivate_account_total_options)) {
    $deactivate_account_total_options = 1;
  }

  $form['deactivate_account_time_option'] = array(
    '#type' => 'fieldset',
    '#title' => t('Time options to deactivate account'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="names-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  // Build the fieldset with the proper number of time options.
  // variable $total_options determine the number of textfields to build.
  $test = variable_get('deactivate_account_time_option');
  for ($i = 0; $i < $deactivate_account_total_options; $i++) {
    $form['deactivate_account_time_option']['name']['deactivate_account_time_' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Time'),
      '#default_value' => $test['name']['deactivate_account_time_' . $i],
    );
  }
  $form['deactivate_account_time_option']['add_option'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('deactivate_account_add_one_option'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
      'callback' => 'deactivate_account_add_more_callback',
      'wrapper' => 'names-fieldset-wrapper',
    ),
  );
  if ($deactivate_account_total_options > 1) {
    $form['deactivate_account_time_option']['remove_option'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('deactivate_account_remove_one_option'),
      '#ajax' => array(
        'callback' => 'deactivate_account_add_more_callback',
        'wrapper' => 'names-fieldset-wrapper',
      ),
    );
  }

  // Checkbox for disabling decativated user comments.
  $form['deactivate_account_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable all comments of deactivated user.'),
    '#description' => t('If checked then all the comments posted by the user will be hidden till the time period account gets active.'),
    '#default_value' => variable_get('deactivate_account_comments'),

  );

  // Checkbox for disabling decativated user comments.
  $form['deactivate_account_nodes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable all nodes of deactivated user.'),
    '#description' => t('If checked then all the nodes published by the user will be hidden till the time period account gets active.'),
    '#default_value' => variable_get('deactivate_account_nodes'),
  );

  // Checkbox for making local tab on user page or link.
  $form['deactivate_account_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create tab on user page'),
    '#description' => t('If checked then deactivation form will be built under user profile as local tab else it will be menu link as specified by the admin.'),
    '#default_value' => variable_get('deactivate_account_tab'),
  );

  $form['deactivate_account_path_container'] = array(
    '#type' => 'container',
    '#states' => array(
      "visible" => array(
        "input[name='deactivate_account_tab']" => array("checked" => FALSE),
      ),
    ),
  );

  $menu = variable_get('deactivate_account_path_container');
  $form['deactivate_account_path_container']['deactivate_account_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Deactivate form link'),
    '#description' => t('deactivate form link @base_url/user/', array('@base_url' => $GLOBALS['base_url'])),
    '#default_value' => $menu['deactivate_account_path'],
    '#prefix' => '',
    '#suffix' => '',
    '#validate' => array('deactivate_account_config_form_validate'),
  );

  $form['deactivate_account_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect path'),
    '#description' => t('user to entered path after deactivating account @base_url/', array('@base_url' => $GLOBALS['base_url'])),
    '#required' => TRUE,
    '#default_value' => variable_get('deactivate_account_redirect'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'deactivate_account_clear_cache';
  return $form;
}

/**
 * Clears all cache.
 */
function deactivate_account_clear_cache($form, &$form_state) {

  drupal_flush_all_caches();
}

/**
 * Function to add one textbox.
 */
function deactivate_account_add_one_option($form, &$form_state) {
  $time_options = variable_get('deactivate_account_total_options');
  $time_options++;
  variable_set('deactivate_account_total_options', $time_options);
  $form_state['rebuild'] = TRUE;
}

/**
 * Function to return form with existing form fields.
 */
function deactivate_account_add_more_callback($form, $form_state) {
  return $form['deactivate_account_time_option'];
}

/**
 * Function to remove one textbox.
 */
function deactivate_account_remove_one_option($form, &$form_state) {
  $time_options = variable_get('deactivate_account_total_options');
  if ($time_options > 1) {
    $time_options--;
    variable_set('deactivate_account_total_options', $time_options);
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Function to check wheather deactivate form link field is required or not.
 */
function deactivate_account_config_form_validate($form, &$form_state) {

  if ($form_state['values']['deactivate_account_tab'] == 0 && empty(trim($form_state['values']['deactivate_account_path_container']['deactivate_account_path']))) {
    form_set_error('deactivate_account_tab', t('Deactivate form link field is required'));
  }
}
