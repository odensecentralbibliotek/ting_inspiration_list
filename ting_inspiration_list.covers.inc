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
file_put_contents("/home/quickstart/work/debug/debuginsp10.txt", print_r($covers_ids, TRUE), FILE_APPEND);
  $result_ids = array();
  $missing_images_local_ids = array();

  foreach ($covers_ids as $cover_id) {
    // Determine if the local id is a known negative.
    if (cache_get('ting_covers:' . $cover_id->localId, FALSE)) {
      $cover_id->hasImage = FALSE;
      break;
    }

    $path = ting_covers_object_path($cover_id->localId);
    if (file_exists($path)) {
      $cover_id->hasImage = TRUE;
    } else {
      $missing_images_local_ids[] = (object) array(
          'localIdentifier' => $cover_id->localId,
          'libraryCode' => $cover_id->ownerId,
      );
    }
  }
  $service = new AdditionalInformationService(variable_get('addi_wsdl_url'), variable_get('addi_username'), variable_get('addi_group'), variable_get('addi_password'));
  // Local ids = Faust numbers. Library object identifiers can be confusing.
  $additional_informations = $service->getByLocalIdentifier($missing_images_local_ids);
  foreach ($missing_images_local_ids as $local_id) {
    if (isset($additional_informations[$local_id->localIdentifier]) && $ai = $additional_informations[$local_id->localIdentifier]) {
      if ($ai->detailUrl  || $ai->thumbnailUrl) {
        $covers_ids[$local_id->localIdentifier]->hasImage = TRUE;
      } else {
        $covers_ids[$local_id->localIdentifier]->hasImage = FALSE;
      }
    }
  }
  return $covers_ids;
}


?>
