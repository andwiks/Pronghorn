Description
-----------

This is a contribution to Drupal Commerce. This module adds the possibility to
send the order data to the Google Analytics service.

Requirements
------------

This module requires:

  - Google Analytics module: https://www.drupal.org/project/google_analytics
  - Ga Push module: https://www.drupal.org/project/ga_push

Features
--------

Rules implementation which let you control when to send the analytics code to
google analytics.

Usage
-----

GA Push server-side methods are recommended to use with this module:
  - Classic Analytics: PHP-GA library (see ga_push README.txt).
  - Universal Analytics: UTMP-PHP.

You can configure it on:
  - ga_push settings (global): /admin/config/system/ga-push
  - rules settings (this module): /admin/config/workflow/rules/reaction/manage/commerce_google_analytics_rule_ga
