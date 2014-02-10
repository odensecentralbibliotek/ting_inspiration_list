  <?php
/**
 * @file
 * This plugin generates a search result based on a new materials node. It's
 * mainly used to override the default page for this node type (node/%node).
 */

$plugin = array(
  'title' => t('Ting Inspiration list'),
  'description' => t('Show a list of new materials'),
  'single' => TRUE,
  'edit form' => 'ting_inspiration_list_content_type_edit_form',
  'render callback' => 'ting_inspiration_list_content_type_render',
  'category' => t('Ting'),
  'required context' => array(new ctools_context_required(t('Inspiration List'), 'node')),
  'render last' => TRUE,
);

/**
 * Render the ting new materials results results amount block.
 */
function ting_inspiration_list_content_type_render($subtype, $conf, $panel_args, $context) {
  // Define the return block.
  $block = new stdClass();


  // Check if context exists.
  if (isset($context[0]->data) && $context[0]->data->type == 'ting_inspiration_list') {
//    // Get the node.
//    $node = $context[0]->data;
//
//    // As we are using a pager, we need to find the current page.
//    $pager_page = pager_find_page();
//
//    // Get number of results.
//    $items = field_get_items('node', $node, 'field_ting_inspiration_list_items');
//    $num_per_page = empty($items[0]['value']) ? 10 : $items[0]['value'];
//
//    // Build search query based on node fields.
//   $query = ting_inspiration_list_build_search_query($node);
//
//    // Search the data well with the query and pager page.
//    $search_result = ting_inspiration_list_do_search($query, $pager_page, $num_per_page);
//
//    // Check if the search return any results.
//    $results = NULL;
//    if ($search_result->numTotalObjects > 0) {
//      // Now that we have a result set, initialize the pager.
//      pager_default_initialize($search_result->numTotalObjects, $num_per_page);
//
//      // Create legend with "Show y-x of result(s)", but only if pager is active
//      // for the current result.
//      if ($search_result->numTotalObjects > $num_per_page) {
//        $from = ($pager_page * $num_per_page) + 1;
//        $to = $from + ($search_result->numTotalCollections - 1);
//        $total = $search_result->numTotalObjects;
//        $legend = format_plural($total, 'Show %from-%to of 1 result', 'Show %from-%to of <span class="placeholder">@count</span> results', array(
//          '%from' => $from,
//          '%to' => $to,
//        ));
//      }
//
//      // We want to build the result as a normal search result. So we have to
//      // build the collections as search results "objects".
//      $results = array();
//      foreach ($search_result->collections as $collection) {
//        // Build the collection.
//        $build = ting_collection_view($collection, 'teaser');
//
//        // Get link to link the result back to the collection.
//        $uri = entity_uri('ting_collection', $collection);
//
//        // Add the result to an array for the "ting_search_results" theme
//        // function.
//        $results[] = array(
//          'link' => url($uri['path'], $uri['options']),
//          'title' => $collection->title,
//          'snippet' => drupal_render($build),
//        );
//      }
//    }
//
//    // Set default page result.
//    $content = array(
//      '#theme' => 'ting_inspiration_list',
//      '#title' => $node->title,
//      '#availability_legend' => ding_availability_render_legend(),
//      '#legend' => isset($legend) ? $legend : NULL,
//    );
//
//    // If any search result have been found add them to the array.
//    if (!is_null($results)) {
//      $content['#results'] = array(
//        '#theme' => 'ting_search_results',
//        '#results' => $results,
//      );
//    }
    
    $node = $context[0]->data;
   // Build search query based on node fields.
    $query = ting_inspiration_list_build_search_query($node);

    // Search the data well with the query and pager page.
    //$search_result = ting_inspiration_list_do_search($query, $pager_page, $num_per_page);
    $search_results = ting_inspiration_list_get_raw_result($query, 500);
    $ids = ting_inspiration_list_process_search_results($search_results);
    $availablity_info = ting_inspiration_list_get_availability_info($ids);
    $ranked_ids = ting_inspiration_list_process_availability($availablity_info, $ids);
    $list_query = ting_inspiration_list_make_list_query($ranked_ids, 0, 10);

    $list_search_result = ting_inspiration_list_do_search($list_query, 0, 10);
      $results = array();
      foreach ($list_search_result->collections as $collection) {
        // Build the collection.
        $build = ting_collection_view($collection, 'teaser');

        // Get link to link the result back to the collection.
        $uri = entity_uri('ting_collection', $collection);

        // Add the result to an array for the "ting_search_results" theme
        // function.
        $results[] = array(
          'link' => url($uri['path'], $uri['options']),
          'title' => $collection->title,
          'snippet' => drupal_render($build),
        );
      }
      //    // Set default page result.
    $content = array(
      '#theme' => 'ting_new_materials',
      '#title' => $node->title,
      '#availability_legend' => ding_availability_render_legend(),
      '#legend' => isset($legend) ? $legend : NULL,
    );

    // If any search result have been found add them to the array.
    if (!is_null($results)) {
      $content['#results'] = array(
        '#theme' => 'ting_search_results',
        '#results' => $results,
      );
    }
    
    // Set block content.
    $block->content = $content;
  }

  return $block;
}

/**
 * Enable admin settings page.
 */
function ting_inspiration_list_content_type_edit_form($form, &$form_state) {
  return $form;
}
