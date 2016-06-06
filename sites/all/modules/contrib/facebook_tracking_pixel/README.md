Facebook Tracking Pixel
================================================================================

This module is meant to offer flexibility in adding Facebook tracking pixels to
your website. The difference in this module and other modules that allow you to
add codes to your page is that this is specifically targeted to Facebook
tracking pixels. You have the ability to add more than one tracking pixel per
page. This module also takes the code from Facebook as it is supplied by them
and optimizes it for loading on pages so it can be aggregated, compressed, and
pushed via a CDN if you wish.

Travis-CI Build status:
[![BuildStatus](https://travis-ci.org/taz77/drupal-facebook_tracking_pixel.svg?branch=7.x-1.x)](https://travis-ci.org/taz77/drupal-facebook_tracking_pixel)

Basic Setup
================================================================================

First step to the module is adding a base tracking code. The base ID is 
available in your Facebook Account under Manage Ads->Tools->Pixels. On the right
hand column you will see your Account name and underneath that is the ID number.
This is your base code ID number.

Add your code to the UI (admin/config/system/facebook_tracking_pixel/base_codes).

This module allows you to manage multiple base tracking codes on your site. The
arrangment of the codes in the UI is the order in which they are added to the
site. Facebook recommends that you do not use more than three pixels on any
given page. Beyond that, the pixels may not execute.

When adding a base tracking pixel you can choose to have the pixel show over the
entire site or not, this is accomplished via the "global" setting.

Tracking By Path
================================================================================

The main feature of this module that allows it to have ultimate flexibility is
the ability to attach tracking to pages on your site on a path by path basis.
This allows for direct and finely grained tracking of events.

In the path portion of the module (admin/config/system/facebook_tracking_pixel/path),
you can choose events to add to your site, the path they act on, and which base
code to use for the event.

Tracking User Registrations
================================================================================

You may choose to track user registrations on your site. You can enable user
registration tracking by going to the Track User Registration page 
(admin/config/system/facebook_tracking_pixel/user_registration) and enable the
tracking. You can then select what base code to track against. This only tracks
against a single base code. This code works off of special Drupal hooks to fire
when a user is added from any location on the entire site.

Purge
================================================================================

Provided for troubleshooting and "when all else fails" scenarios, there is a
purge function (admin/config/system/facebook_tracking_pixel/purge). By clicking
Purge All every code on the site and every path tracking event will be erased.
This removes everything from the database and deletes all files that have been
created. It will return the module to a state of initial installation.

You have been warned!

Commerce Tracking
================================================================================

Not finished... 
admin/config/system/facebook_tracking_pixel/commercetracking

Developers - Hooks provided
================================================================================

There is one hook alter and one hook provided. The hook allows the ability to
provide additional tracking events that can be incorporated into a site. The 
additional events will then be available in the path admin UI. The name of the
provided hook is HOOK_facebook_tracking_pixel(). You may define additional
events by providing an array of event information. An example:

    mymodule_facebook_tracking_pixel(){
      $events = [];
      $events['pageview'] = [
        'name' => t('Key Page View'),
        'code' => 'fbq(\'track\', \'ViewContent\');',
      ];
      return $events;
    }
The array is keyed with the machine name of the event and each item contains an
array with the human readable name and the actual JS code to be used.

As a function of the path tracking. You may alter the paths being used by the
module by executing a `HOOK_facebook_tracking_pixel_alter()`. See
`facebook_tracking_pixel_facebook_tracking_pixel_alter()` in the file
*facebook_tracking_pixel.module* as an example of how to use this alter. This
module uses it's own alter to match paths.