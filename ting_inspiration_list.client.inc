<?php
/**
 * @file
 * Wrapper functions for Ting client.
 */

/**
 * Performs a query against the Data Well and returns the objects.
 *
 * @param string @query
 *  The query string.
 * @param $page int
 *  The page number.
 * @return TingClientSearchResult
 *  The search result.
 *
 */
function ting_inspiration_list_do_parallel_search($query, $num_results = 200) {
  // Load the ting client.
  module_load_include('client.inc', 'ting');

  // Build search options and ensure that they are sorted corretly.
  $options = array();
  $options['enrich'] = TRUE;
  $options['collectionType'] = 'manifestation';

  $objects_per_request = 50;
  $page = 1;
  $number_of_objects_retrieved = 0;
  $search_requests = array();
  do {
    $search_requests[] = ting_inspiration_list_build_request($query, $page, $objects_per_request, $options);
    $page++;
    $number_of_objects_retrieved += $objects_per_request;
  } while ($number_of_objects_retrieved < $num_results);
  try {   
    $search_result = ting_inspiration_list_execute($search_requests);
  } catch (TingClientException $e) {
    watchdog('ting_inspiration_list', 'Failed to get objects from the Data well: “@message”', array('@message' => $e->getMessage(), WATCHDOG_ERROR));
  }
  return $search_result;
}

function ting_inspiration_list_build_request($query, $page = 1, $results_per_page = 50, $options = array()) {
  $request = ting_get_request_factory()->getSearchRequest();
  $request->setQuery($query);
  if ($agency = variable_get('ting_agency', FALSE)) {
    $request->setAgency($agency);
  }

  $request->setStart($results_per_page * ($page - 1) + 1);
  $request->setNumResults($results_per_page);

  if (isset($options['collectionType'])) {
    $request->setCollectionType($options['collectionType']);
  }
  $request->setAllObjects(isset($options['allObjects']) ? $options['allObjects'] : FALSE);

  // Set search profile, if applicable.
  $profile = variable_get('ting_search_profile', '');
  if (!empty($profile) && method_exists($request, 'setProfile')) {
    $request->setProfile($profile);
  }


  // Apply custom ranking if enabled.
//  if (variable_get('ting_ranking_custom', FALSE)) {
//    $fields = array();
//    foreach (variable_get('ting_ranking_fields', array()) as $field) {
//      $fields[] = array(
//        'fieldName' => $field['field_name'],
//        'fieldType' => $field['field_type'],
//        'weight' => $field['weight'],
//      );
//    }

//    if (!empty($fields)) {
//      // Add the default anyIndex boosts.
//      $fields[] = array(
//        'fieldName' => 'cql.anyIndexes',
//        'fieldType' => 'phrase',
//        'weight' => 1,
//      );
//
//      $fields[] = array(
//        'fieldName' => 'cql.anyIndexes',
//        'fieldType' => 'word',
//        'weight' => 1,
//      );
//
//      $request->userDefinedRanking = array('tieValue' => 0.1, 'rankField' => $fields);
//    }
 // }
  // Otherwise, use the ranking setting.
//  else {
//    $request->setRank((isset($options['rank']) && $options['rank']) ? $options['rank'] : 'rank_general');
//  }

  // Apply custom boosts if any.
//  $boosts = variable_get('ting_boost_fields', array());
//
//  if ($boosts) {
//    $uboosts = array();
//    foreach ($boosts as $boost_field) {
//      $uboosts[] = array(
//        'fieldName' => $boost_field['field_name'],
//        'fieldValue' => $boost_field['field_value'],
//        'weight' => $boost_field['weight'],
//      );
//    }
//    $request->userDefinedBoost = array('boostField' => $uboosts);
//  }

  return $request;
}

/**
 * Perform a request against Ting and perform error handling if necessary.
 *
 * @param object $request
 *   The request.
 *
 * @return mixed
 *   Result of the request or false if an error occurs.
 */
function ting_inspiration_list_execute($requests) {
  try {
    timer_start('ting');
    $request = ting_get_request_factory()->getSearchRequest();;
    $request->requests = $requests;
    $res = ting_inspiration_list_get_client()->execute($request);
    timer_stop('ting');
    $response = array();
    foreach ($res as $res_object) {
      $response[] = $request->parseResponse($res_object);
    }   
    return $response;
  }
  catch (TingClientException $e) {
    timer_stop('ting');
    watchdog('ting inspiration client', 'Error performing request: ' . $e->getMessage(), NULL, WATCHDOG_ERROR, 'http://' . $_SERVER["HTTP_HOST"] . request_uri());
    return FALSE;
  }
}

/**
 * Retrieves an initialized Ting client with appropriate request adapter and logger
 *
 * @return TingClient
 */
function ting_inspiration_list_get_client() {
  static $client;
  if (!isset($client)) {
    $logger = (variable_get('ting_enable_logging', FALSE)) ? new TingClientDrupalWatchDogLogger(ting_get_request_factory()) : new TingClientVoidLogger();
    $client = new TingClient(new TingClientMultiRequestAdapter(), $logger);
  }

  return $client;
}