<?php

/**
 * @file
 *
 * Contains form builder function for the inspiration list admin page.
 */

/**
 * Form builder for 'admin/config/ting/term-list' route.
 */
function ting_inspiration_list_admin_form($form, &$form_state) {
 	// Cache settings
	$form['cache_settings'] = array(
		'#type' => 'fieldset',
		'#title' => t('Cache settings'),
		'#tree' => FALSE,
	);
	$options = array();
	for ($i = 7; $i <= 14; $i++) {
		$options[$i] = t('@days dage', array('@days' => $i));
	}
	$form['cache_settings']['ting_inspiration_list_cache_lifetime'] = array(
		'#type' => 'select',
		'#title' => t('Cache lifetime'),
		'#description' => t('Select the amount of time term lists should be cached'),
		'#default_value' => variable_get('ting_inspiration_list_cache_lifetime', 
			TING_INSPIRATION_LIST_DEFAULT_CACHE_LIFETIME),	
		'#options' => $options,
	);
	$options = array();
	for ($i = 1; $i <= 12; $i++) {
		$amount = $i * 5;
		$options[$amount] = $amount;
	}
	$form['cache_settings']['ting_term_list_cache_amount'] = array(
		'#type' => 'select',
		'#title' => t('Cache amount'),
		'#description' => t('Select the max amount of term lists to cache on nightly cron jobs'),
		'#default_value' => variable_get('ting_inspiration_list_cache_amount',
			TING_INSPIRATION_LIST_DEFAULT_CACHE_AMOUNT),
		'#options' => $options,
	);

	// Cache history table
	$header = array(
		array('data' => t('ID'), 'field' => 'n.nid'),
		array('data' => t('Title'), 'field' => 'n.title'),
		array('data' => t('Time'), 'field' => 'chttl.time'),
	);
	$query = db_select('node', 'n')->extend('TableSort');
	$query->join('cache_history_ting_inspiration_list', 'chtil', 'n.nid = chtil.nid');
	$query->addField('n', 'nid', 'nid');
	$query->addField('n', 'title', 'title');
	$query->addField('chtil', 'time', 'time');
	$query->orderBy('time', 'DESC');
	$result = $query->orderByHeader($header)->execute();
	$rows = array();
	foreach ($result as $row) {
    $row->time = date(DATE_FORMAT_DATETIME, $row->time);
		$rows[] = array('data' => (array) $row);
	}
	$table = array(
		'#theme' => 'table',
		'#header' => $header,
		'#rows' => $rows,	
	);
	$form['cache_history'] = array(
		'#type' => 'fieldset',
		'#title' => t('Cache history'),
		'#tree' => FALSE,
	);
	$form['cache_history']['cache_history_table'] = $table;
	return system_settings_form($form);
}