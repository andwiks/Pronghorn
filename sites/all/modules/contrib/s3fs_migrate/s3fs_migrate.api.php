<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup s3fs_migrate_hooks S3 File System Migrate hooks
 * Hooks that can be implemented by other modules to extend S3 File System Migrate.
 */

/**
 * Allows modules to alter the destination URI when migrating a file to S3.
 * 
 * @param array $data
 *   Associative array of URL settings:
 *     - 'dest_uri': (string) The destination URI, this is the only alterable element.
 *     - 'orig_uri': (string) The original URI as recorded in the file_managed table.
 *     - 'fid': (int) The File ID of the file being migrated.
 */
function hook_s3fs_migrate_destination_uri_alter(&$data) {
  // An example of what you might want to do with this hook.
  $data['uri_dest'] = transliteration_clean_filename($data['uri_dest']);
}
