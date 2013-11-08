<?php

/**
 * @file
 * A specific handler for ID
 */

$plugin = array(
  'title' => t('Address form (ID add-on)'),
  'format callback' => 'addressfield_format_address_id_generate',
  'type' => 'address',
  'weight' => -80,
);

function addressfield_format_address_id_generate(&$format, $address, $context = array()) {
  if ($address['country'] == 'ID' && $context['mode'] == 'form') {
    $result = db_select("shipping_jne_reg","j")->fields("j",array("state"))->execute();
    $state = array();
    while($record = $result->fetch()) {
      $state[$record->state]=$record->state;
    }
    $format['locality_block']['administrative_area'] = array_merge(
      $format['locality_block']['administrative_area'],
      array(
        '#title' => t('State'),
        '#required' => true,
        '#options' => $state,
        '#default_value' => 'DKI Jakarta',
        '#weight' => 3,
        '#size' => 30,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );
    $format['locality_block']['administrative_area']['#wrapper_id'] = $format['#wrapper_id'];
    $format['locality_block']['administrative_area']['#process'][] = 'ajax_process_form';
    $format['locality_block']['administrative_area']['#process'][] = 'addressfield_format_address_id_state_process';
    $format['locality_block']['administrative_area']['#ajax'] = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
      'method' => 'replace',
    );
    //City
    $city = addressfield_format_address_id_get_city($address['administrative_area']!='' ? $address['administrative_area'] : 'Nanggroe Aceh Darusalam');
    $format['locality_block']['locality'] = array_merge(
      $format['locality_block']['locality'],
      array(
        '#title' => t('Town/City'),
        '#weight' => 45,
        '#options' => $city,
      )
    );
    // Postal code
    $format['locality_block']['postal_code'] = array_merge(
      $format['locality_block']['postal_code'],
      array(
        '#title' => t('Kode Pos'),
        '#weight' => 60,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );
  } else {
    if (isset($format['locality_block']['administrative_area'])) {
      // Cancel the AJAX for forms we don't control.
      $format['locality_block']['administrative_area']['#ajax'] = array();
    }
  }
}

function addressfield_format_address_id_state_process($element) {
  $element['#limit_validation_errors'] = array($element['#parents']);

  return $element;
}

function addressfield_format_address_id_get_city($state){
  $result = db_select("shipping_jne_reg","j")->fields("j",array("city"))->condition('state',$state,'=')->execute();
  $city = array();
  while($record = $result->fetch()) {
    $city[$record->city]=$record->city;
  }
  return $city;
}