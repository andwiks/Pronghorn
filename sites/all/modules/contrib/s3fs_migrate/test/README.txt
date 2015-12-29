These tests require your Amazon S3 credentials to be included in a file called
s3fs_migrate_test_settings.php that is located in the directory above the 
Drupal root directory, for example if Drupal is installed at 
/home/myname/public_html 
then the config file should be located at 
/home/myname/s3fs_migrate_test_settings.php

The file should contain the lines:

<?php
$s3fs_config['awssdk2_access_key'] = 'YOUR ACCESS KEY';
$s3fs_config['awssdk2_secret_key'] = 'YOUR SECRET KEY';
