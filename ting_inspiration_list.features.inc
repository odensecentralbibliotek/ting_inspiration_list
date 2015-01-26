<?php

/**
 * @file
 * ting_inspiration_list.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ting_inspiration_list_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ting_inspiration_list_node_info() {
  $items = array(
    'ding_inspiration_list' => array(
      'name' => t('Inspiration List'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_image_default_styles().
 */
function ting_inspiration_list_image_default_styles() {
  $styles = array();

  // Exported image style: ting_inspiration_list_cover.
  $styles['ting_inspiration_list_cover'] = array(
    'name' => 'ting_inspiration_list_cover',
    'label' => 'ting_inspiration_list_cover',
    'effects' => array(
      1 => array(
        'label' => 'Skalér og beskær',
        'help' => 'Skalering og beskæring bevarer billedets højde-bredde-forhold og beskærer derefter den største dimension. Det er nyttigt til at generere kvadratiske thumbnails uden at forvrænge billedet.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 140,
          'height' => 203,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
