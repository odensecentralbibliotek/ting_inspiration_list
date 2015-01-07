<?php

/**
 * Duplicate code from ting_covers module. Ting covers module doesn't provide a api we can use.
 * 
 * @param array $covers_ids
 *  Ids of the materials to fetch covers for.
 * @param array
 *   True if there is a cover image.
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


?>
