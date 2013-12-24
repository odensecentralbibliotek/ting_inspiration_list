<?php

/**
 * @file
 * This plugin generates a search result based on a new materials node. It's
 * mainly used to override the default page for this node type (node/%node).
 */
$plugin = array(
  'title' => t('Ting Inspiration Cover list'),
  'description' => t('Show a list of relevant materials'),
  'single' => TRUE,
  'edit form' => 'ting_inspiration_list_covers_content_type_edit_form',
  'render callback' => 'ting_inspiration_list_covers_content_type_render',
  'category' => t('Ting'),
  'required context' => array(new ctools_context_required(t('Inspiration List'), 'node')),
  'render last' => TRUE,
);

/**
 * Render the ting new materials results results amount block.
 */
function ting_inspiration_list_covers_content_type_render($subtype, $conf, $panel_args, $context) {
  // Define the return block.
  $block = new stdClass();
  // Check if context exists.
  if (isset($context[0]->data) && ($context[0]->data->type == 'ting_inspiration_list' || $context[0]->data->type == 'genre_page')) {

    $node = $context[0]->data;

    if (false && $cache = ting_inspiration_list_check_cache($node)) {
      $content = $cache->data;
    } else {
      $content = ting_inspiration_list_get_inspration_list_covers($node);
    }

    // Set block content.
    $block->content = $content;
  }

  return $block;
}


/**
 * Check cache
 */
function ting_inspiration_list_check_cache($node) {
  $alma_query = field_get_items('node', $node, 'field_ddelibra_inspiration_list');
  if (!empty($alma_query[0]['value'])) {
    $query = $alma_query[0]['value'];
  } else {
    $query = ting_inspiration_list_build_search_query($node);
  }
  return ting_inspiration_list_get_cached_lists($query, 'cover15');;
}

/**
 * Enable admin settings page.
 */
function ting_inspiration_list_covers_content_type_edit_form($form, &$form_state) {
  return $form;
}



