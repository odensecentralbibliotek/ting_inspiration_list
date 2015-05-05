<?php

/**
 * @file
 * This plugin generates a inspiration list on a Inspiration list node. It's
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
);

/**
 * Render the inspiration list block.
 */
function ting_inspiration_list_covers_content_type_render($subtype, $conf, $panel_args, $context) {
  // Define the return block.
  $block = new stdClass();
  // Check if context exists.
  if (isset($context[0]->data)) {

    $node = $context[0]->data;
    $well_query = field_get_items('node', $node, 'field_inspiration_search');
    
    if (!isset($well_query) || empty($well_query[0]['value'])) {
      return $block;
    }
    
    $cid = _ting_inspiration_list_get_cid_from_nid($node->nid);
    $cache = cache_get($cid, 'cache_ting_inspiration_list');
    if ($cache && ($cache->expire > REQUEST_TIME)) {
      $cached_results = $cache->data;
    } else {
      $cached_results = ting_inspiration_list_get_inspration_list_covers($node);
    }
    // Set block content.
    drupal_add_js(drupal_get_path('module', 'ting_covers') . '/js/ting-covers.js');
    $block->content = ting_inspiration_list_render_cover($cached_results, $node);
  }

  return $block;
}


/**
 * Enable admin settings page.
 */
function ting_inspiration_list_covers_content_type_edit_form($form, &$form_state) {
  return $form;
}



