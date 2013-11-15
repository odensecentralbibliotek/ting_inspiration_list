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
    $number_of_raw_results = 500;
    $node = $context[0]->data;

    if ($cache = ting_inspiration_list_check_cache($node)) {
      $content = $cache->data;
      file_put_contents("/home/quickstart/work/debug/debuggenrecache3.txt", print_r("Ramt inspiration 3" , TRUE), FILE_APPEND);
    } else {
      $alma_query = field_get_items('node', $node, 'field_ddelibra_inspiration_list');
      if (!empty($alma_query[0]['value'])) {
        //Use alma to get results
        $query = $alma_query[0]['value'];
        $ids = ting_inspiration_list_search_alma_query($query, $number_of_raw_results);
      } else {
        //Use the datawell to get results
        // Build search query based on node fields.
        $query = ting_inspiration_list_build_search_query($node);
        $raw_search_results = ting_inspiration_list_get_raw_result($query, $number_of_raw_results);
        $ids = ting_inspiration_list_process_search_results($raw_search_results);
      }


      $availablity_info = ting_inspiration_list_get_availability_info($ids);
      $ranked_ids = ting_inspiration_list_process_availability($availablity_info, $ids);
      $list_query = ting_inspiration_list_make_list_query($ranked_ids, 0, 20);

      //file_put_contents("/home/quickstart/work/debug/debuggenre.txt", print_r($ranked_ids , TRUE), FILE_APPEND);
      $search_result = ting_inspiration_list_do_search($list_query, 0, 20);
      // Check if the search return any results.
      $results = NULL;
      if ($search_result->numTotalObjects > 0) {
        $cover_ids = array();
        foreach ($search_result->collections as $collection) {
          $cover_ids[] = array(
            'local_id' => $collection->primary_object->localId,
            'owner_id' => $collection->primary_object->ownerId
          );
        }
        $cover_result_ids = ting_inspiration_list_check_covers($cover_ids);

        $number_of_objects = 15;
        $number_found = 0;
        $results = array();

        foreach ($search_result->collections as $collection) {
          // If there is a image
          if (isset($cover_result_ids[$collection->primary_object->localId]) && $cover_result_ids[$collection->primary_object->localId]) {
            // Build the collection.
            //$build = ting_collection_view($collection, 'teaser');
            // Get link to link the result back to the collection.
            $uri = entity_uri('ting_collection', $collection);

            // Add the result to an array for the "ting_search_results" theme
            // function.      
            $image = theme('ting_object_cover', array('elements' => array('#object' => $collection->primary_object, '#image_style' => 'ting_new_materials_cover')));
            //$snippet = drupal_render($build);
            $options = array(
              'html' => TRUE,
              'attributes' => array('title' => $collection->title),
            );
            $results[] = array(
              'snippet' => l($image, $uri['path'], $options)
            );
            $number_found++;
            if ($number_found >= $number_of_objects) {
              break;
            }
          }
        }
      }

      // Set default page result.
      $content = array(
        '#theme' => 'ting_new_materials',
        '#title' => $node->title,
        '#availability_legend' => NULL,
        '#legend' => isset($legend) ? $legend : NULL,
      );

      // If any search result have been found add them to the array.
      if (!is_null($results)) {
        $content['#results'] = array(
          '#theme' => 'covers_search_results',
          '#results' => $results,
        );
      }
      ting_inspiration_list_set_cached_lists($query, 'cover15', $content);
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

function ting_inspiration_list_check_covers($covers_ids) {
  $result_ids = array();
  $missing_images_local_ids = array();

  foreach ($covers_ids as $cover_id) {
    // Determine if the local id is a known negative.
    if (cache_get('ting_covers:' . $cover_id, FALSE)) {
      $result_ids[$cover_id['local_id']] = FALSE;
      break;
    }

    $path = ting_covers_object_path($cover_id['local_id']);
    if (file_exists($path)) {
      $result_ids[$cover_id['local_id']] = TRUE;
    } else {
      $missing_images_local_ids[] = (object) array(
          'localIdentifier' => $cover_id['local_id'],
          'libraryCode' => $cover_id['owner_id'],
      );
    }
  }
  $service = new AdditionalInformationService(variable_get('addi_wsdl_url'), variable_get('addi_username'), variable_get('addi_group'), variable_get('addi_password'));
  // Local ids = Faust numbers. Library object identifiers can be confusing.
  $additional_informations = $service->getByLocalIdentifier($missing_images_local_ids);
  foreach ($missing_images_local_ids as $local_id) {
    if (isset($additional_informations[$local_id->localIdentifier]) && $ai = $additional_informations[$local_id->localIdentifier]) {
      if ($ai->detailUrl) {
        $result_ids[$local_id->localIdentifier] = TRUE;
        //ting_inspiration_list_fetch_image(ting_covers_object_path($local_id->localIdentifier), $ai->detailUrll);
      } else if ($ai->thumbnailUrl) {
        $result_ids[$local_id->localIdentifier] = TRUE;
        //ting_inspiration_list_fetch_image(ting_covers_object_path($local_id->localIdentifier), $ai->thumbnailUrl);
      } else {
        $result_ids[$local_id->localIdentifier] = FALSE;
      }
    }
  }
  return $result_ids;
}

/**
 * Helper function to fetch and save a cover image file.
 *
 * @see image_style_create_derivative()
 *
 * @param string $filename
 *    File name, including its path within Drupal's file folder.
 * @param string $image_url
 *    URL for the source image file.
 * @return mixed
 *    A file object or FALSE on error.
 */
function ting_inspiration_list_fetch_image($filename, $image_url) {
  $result = ting_inspiration_list_get_by_curl ($image_url); //To get by proxy on develplatform //drupal_http_request($image_url);

  //Bail if the HTTP request failed
//  if ($result->code != 200) {
//    return FALSE;
//  }

  // Get the folder for the final location of this preset...
  $directory = dirname($filename);
  // Build the destination folder tree if it doesn't already exist.
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    watchdog('ting_covers', 'Failed to create directory: %directory', array('%directory' => $directory), WATCHDOG_ERROR);
    return FALSE;
  }

  return file_unmanaged_save_data($result, $filename, FILE_EXISTS_REPLACE);
}

 
//Developmentplatform has proxy
function ting_inspiration_list_get_by_curl ($url) {
$agent = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.8) Gecko/2009032609 Firefox/3.0.8';
$curl_handle=curl_init();
curl_setopt($curl_handle, CURLOPT_PROXY, '172.18.0.40:8080');
curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($curl_handle, CURLOPT_USERAGENT, $agent);
curl_setopt($curl_handle,CURLOPT_URL, $url);

return curl_exec($curl_handle);
}

