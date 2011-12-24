<?php

/**
 * Views for the default credits UI.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_credits_ui_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'credits';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_credits';
  $view->human_name = 'Credits';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Credits';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce credits';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Commerce Credit: Credit type credit_type */
  $handler->display->display_options['relationships']['commerce_credits_type']['id'] = 'commerce_credits_type';
  $handler->display->display_options['relationships']['commerce_credits_type']['table'] = 'commerce_credits';
  $handler->display->display_options['relationships']['commerce_credits_type']['field'] = 'commerce_credits_type';
  $handler->display->display_options['relationships']['commerce_credits_type']['required'] = 1;
  /* Field: Commerce Credit: Commerce credit ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'commerce_credits';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  /* Field: Commerce Credit Type: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'commerce_credits_types';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'commerce_credits_type';
  $handler->display->display_options['fields']['label']['label'] = 'Credit type';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = 1;
  /* Field: Commerce Credit: Expiration_date */
  $handler->display->display_options['fields']['expiration_date']['id'] = 'expiration_date';
  $handler->display->display_options['fields']['expiration_date']['table'] = 'commerce_credits';
  $handler->display->display_options['fields']['expiration_date']['field'] = 'expiration_date';
  $handler->display->display_options['fields']['expiration_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['expiration_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['expiration_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['expiration_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['expiration_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['expiration_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['expiration_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['expiration_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['expiration_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['expiration_date']['date_format'] = 'long';
  /* Field: Commerce Credit: Credits */
  $handler->display->display_options['fields']['credits']['id'] = 'credits';
  $handler->display->display_options['fields']['credits']['table'] = 'commerce_credits';
  $handler->display->display_options['fields']['credits']['field'] = 'credits';
  $handler->display->display_options['fields']['credits']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['external'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['credits']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['credits']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['html'] = 0;
  $handler->display->display_options['fields']['credits']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['credits']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['credits']['hide_empty'] = 0;
  $handler->display->display_options['fields']['credits']['empty_zero'] = 0;
  $handler->display->display_options['fields']['credits']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['credits']['format_plural'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/credits/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Credits';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Credits';
  $handler->display->display_options['tab_options']['description'] = 'Manage commerce credits and their types';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $views['credits'] = $view;

  $view = new view;
  $view->name = 'credits_user_balance';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_credits';
  $view->human_name = 'Credits user balance';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Your credits balance';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'You do not have any credit.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Commerce Credit: Credit type credit_type */
  $handler->display->display_options['relationships']['commerce_credits_type']['id'] = 'commerce_credits_type';
  $handler->display->display_options['relationships']['commerce_credits_type']['table'] = 'commerce_credits';
  $handler->display->display_options['relationships']['commerce_credits_type']['field'] = 'commerce_credits_type';
  $handler->display->display_options['relationships']['commerce_credits_type']['required'] = 1;
  /* Field: Commerce Credit Type: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'commerce_credits_types';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'commerce_credits_type';
  $handler->display->display_options['fields']['label']['label'] = 'Name';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label']['empty_zero'] = 0;
  $handler->display->display_options['fields']['label']['hide_alter_empty'] = 1;
  /* Field: Commerce Credit Type: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'commerce_credits_types';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['relationship'] = 'commerce_credits_type';
  /* Field: Commerce Credit: Credits */
  $handler->display->display_options['fields']['credits']['id'] = 'credits';
  $handler->display->display_options['fields']['credits']['table'] = 'commerce_credits';
  $handler->display->display_options['fields']['credits']['field'] = 'credits';
  $handler->display->display_options['fields']['credits']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['external'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['credits']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['credits']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['credits']['alter']['html'] = 0;
  $handler->display->display_options['fields']['credits']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['credits']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['credits']['hide_empty'] = 0;
  $handler->display->display_options['fields']['credits']['empty_zero'] = 0;
  $handler->display->display_options['fields']['credits']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['credits']['format_plural'] = 0;
  /* Contextual filter: Commerce Credit: Entity_id */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'commerce_credits';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['entity_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['entity_id']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['entity_id']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['entity_id']['validate_options']['restrict_roles'] = '1';
  $handler->display->display_options['arguments']['entity_id']['validate_options']['roles'] = array(
    3 => '3',
  );
  $handler->display->display_options['arguments']['entity_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['entity_id']['not'] = 0;
  /* Filter criterion: Commerce Credit: Entity_type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'commerce_credits';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = 'user';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/credits-user-balance';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Credits';
  $handler->display->display_options['menu']['weight'] = '20';

  $views['credits_user_balance'] = $view;

  return $views;
}
