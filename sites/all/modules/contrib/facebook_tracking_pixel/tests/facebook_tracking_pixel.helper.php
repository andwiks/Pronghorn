<?php

/**
 * @file
 * Contains tests for the Facebook Tracking Pixel module.
 */

namespace facebookTrackingPixel;

class FacebookTrackingPixelTestHelper {
  public function enable_tracking_all_roles() {
    // Turn on tracking for roles.
    variable_set('facebook_tracking_pixel_roles_administrator', 1);
    variable_set('facebook_tracking_pixel_roles_anonymous_user', 1);
    variable_set('facebook_tracking_pixel_roles_authenticated_user', 1);
  }
}

