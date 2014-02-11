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

    //Todo remove false from caching
    $cid = _ting_inspiration_list_get_cid_from_nid($node->nid);
    $cache = cache_get($cid, 'cache_ting_inspiration_list');
    if (false && ($cache->expire > REQUEST_TIME)) {
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
 * Enable admin settings page.
 */
function ting_inspiration_list_covers_content_type_edit_form($form, &$form_state) {
  return $form;
}



