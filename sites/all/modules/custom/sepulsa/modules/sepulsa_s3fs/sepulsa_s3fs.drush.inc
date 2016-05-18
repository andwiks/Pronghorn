<?php

/**
 * @file
 * Defines a drush command that refreshes the S3 metadata cache.
 */

/**
 * Implements hook_drush_command().
 */
function sepulsa_s3fs_drush_command() {
  $items = array();
  $items['sepulsa-s3fs-copy-local'] = array(
    'description' => dt('Copy local themes files from your public and/or private file system(s) into your S3 bucket, if s3fs is configured to take them over.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('sepulsa-s3fs-cl'),
    'callback' => 'drush_sepulsa_s3fs_copy_local',
  );
  return $items;
}

/**
 * Implement command drush sepulsa s3fs
 */
function drush_sepulsa_s3fs_copy_local()
{
  _s3fs_copy_file_themes_to_s3();
  watchdog('sepulsa_s3fs_copy_local', 'drush command: drush_sepulsa_s3fs_copy_local run!', array(
    // '!device_account_id' => $device_uid,
  ), WATCHDOG_INFO);
}
