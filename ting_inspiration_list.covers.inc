<?php

/**
 * Duplicate code from ting_covers module. Ting covers module doesn't provide a api we can use.
 */
function ting_inspiration_list_check_covers($covers_ids) {
  $result_ids = array();
  $missing_images_local_ids = array();

  foreach ($covers_ids as $cover_id) {
    // Determine if the local id is a known negative.
    if (cache_get('ting_covers:' . $cover_id['local_id'], FALSE)) {
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
      if ($ai->detailUrl  || $ai->thumbnailUrl) {
        $result_ids[$local_id->localIdentifier] = TRUE;
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
curl_setopt($curl_handle, CURLOPT_PROXY, '172.18.0.30:8080');
curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($curl_handle, CURLOPT_USERAGENT, $agent);
curl_setopt($curl_handle,CURLOPT_URL, $url);

return curl_exec($curl_handle);
}



?>
