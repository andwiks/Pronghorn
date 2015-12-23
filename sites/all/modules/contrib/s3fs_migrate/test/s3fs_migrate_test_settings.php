<?php

$conf['awssdk2_access_key'] = '1HH6CT6NNGWHRSG80EG2';
$conf['awssdk2_secret_key'] = 'CwKTSwqHc+FA9gB9koVT2hQ0eMogRnSfRr7sYStp';
$conf['s3fs_bucket'] = str_replace('.', '-', CO_DOMAIN_PRIMARY). '-74yz8tf2'; //Add salt to increase chance of unique bucket name.
$conf['s3fs_region'] = 'ap-southeast-2'; //Sydney
