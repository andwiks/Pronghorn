<?php
/**
 * @file
 * device.info.inc
 *
 * @author andreas@sepulsa.com
 */

/**
 * Implements hook_entity_property_info().
 */
function device_entity_property_info() {
  $info = array();

  // Add meta-data about the device properties.
  $device = &$info['device']['properties'];
  // Add properties for device.did.
  $device['did'] = array(
    'entity views field' => TRUE,
  );
  // Add properties for device.imei.
  $device['imei'] = array(
    'entity views field' => TRUE,
    'required' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device.phone.
  $device['phone'] = array(
    'entity views field' => TRUE,
    'required' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device.label.
  $device['label'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device.created.
  $device['created'] = array(
    'entity views field' => TRUE,
    'required' => TRUE,
  );
  // Add properties for device.access.
  $device['access'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device.status.
  $device['status'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  // Add properties for device.uid.
  $device['uid'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device.data.
  $device['data'] = array(
    'label' => t('Data'),
    'description' => t('Device data.'),
    'type' => 'struct',
    'schema field' => 'data',
    'setter callback' => 'entity_property_verbatim_set',
    'auto creation' => TRUE,
  );

  // Add meta-data about the device verification properties.
  $verification = &$info['device_verification']['properties'];
  // Add properties for device_verification.dv_id.
  $verification['dv_id'] = array(
    'entity views field' => TRUE,
  );
  // Add properties for device_verification.did.
  $verification['did'] = array(
    'entity views field' => TRUE,
    'required' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device_verification.phone.
  $verification['phone'] = array(
    'entity views field' => TRUE,
    'required' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );
  // Add properties for device_verification.created.
  $verification['created'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  // Add properties for device_verification.timeout.
  $verification['timeout'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  // Add properties for device_verification.status.
  $verification['status'] = array(
    'entity views field' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
  );
  // Add properties for device_verification.hash.
  $verification['hash'] = array(
    'entity views field' => TRUE,
    'required' => TRUE,
    'setter callback' => 'entity_property_verbatim_set',
  );

  // Return property info array.
  return $info;
}

/**
 * Implements hook_entity_property_info_alter().
 */
function device_entity_property_info_alter(&$info) {
  $info['device']['properties']['created']['type'] = 'date';
  $info['device']['properties']['access']['type'] = 'date';
  $info['device_verification']['properties']['created']['type'] = 'date';
  $info['device_verification']['properties']['timeout']['type'] = 'date';
}
