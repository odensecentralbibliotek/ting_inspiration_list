<?php
/**
 * @file
 * ting_inspiration_list.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function ting_inspiration_list_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-ding_inspiration_list-body'
  $field_instances['node-ding_inspiration_list-body'] = array(
    'bundle' => 'ding_inspiration_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
      'teaser_highlight' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-ding_inspiration_list-field_inspiration_raw_count'
  $field_instances['node-ding_inspiration_list-field_inspiration_raw_count'] = array(
    'bundle' => 'ding_inspiration_list',
    'default_value' => array(
      0 => array(
        'value' => 200,
      ),
    ),
    'deleted' => 0,
    'description' => 'Number of objects to get from the datawell. Bigger number gives a better result but has a performance hit. ',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser_highlight' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_inspiration_raw_count',
    'label' => 'Inspiration list raw count',
    'required' => 0,
    'settings' => array(
      'max' => 500,
      'min' => 100,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-ding_inspiration_list-field_inspiration_row_count'
  $field_instances['node-ding_inspiration_list-field_inspiration_row_count'] = array(
    'bundle' => 'ding_inspiration_list',
    'default_value' => array(
      0 => array(
        'value' => 3,
      ),
    ),
    'deleted' => 0,
    'description' => 'Number of rows of materials to show en the page',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser_highlight' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_inspiration_row_count',
    'label' => 'Inspiration list row count',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-ding_inspiration_list-field_inspiration_search'
  $field_instances['node-ding_inspiration_list-field_inspiration_search'] = array(
    'bundle' => 'ding_inspiration_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The search query to use when searching the datawell.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser_highlight' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_inspiration_search',
    'label' => 'Inspiration list search query',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 7,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Inspiration list raw count');
  t('Inspiration list row count');
  t('Inspiration list search query');
  t('Number of objects to get from the datawell. Bigger number gives a better result but has a performance hit. ');
  t('Number of rows of materials to show en the page');
  t('The search query to use when searching the datawell.');

  return $field_instances;
}
