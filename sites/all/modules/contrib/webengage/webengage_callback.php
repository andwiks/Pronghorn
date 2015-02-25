<?php
/**
 * @file
 * callback view
 *
 * @category view
 * @package   WebEngage
 * @link     http://www.webengage.com/
 */

 /**
  * Implements hook_preprocess_HOOK().
  */
function webengage_preprocess_webengage_callback(&$vars) {
  module_load_include("php", "webengage", "webengage_constants");

  $vars['wlc'] = urldecode(isset($_REQUEST['webengage_license_code']) ? htmlspecialchars($_REQUEST['webengage_license_code'], ENT_COMPAT, 'UTF-8') : "");
  $vars['vm'] = urldecode(isset($_REQUEST['verification_message']) ? htmlspecialchars($_REQUEST['verification_message'], ENT_COMPAT, 'UTF-8') : "");
  $vars['wwa'] = urldecode(isset($_REQUEST['webengage_widget_status']) ? htmlspecialchars($_REQUEST['webengage_widget_status'], ENT_COMPAT, 'UTF-8') : "");
  $vars['option'] = urldecode(isset($_REQUEST['option']) ? htmlspecialchars($_REQUEST['option'], ENT_COMPAT, 'UTF-8') : NULL);
}

 /**
  * Callback for callback path.
  *
  * @return string
  *   the theme block
  */
function webengage_do_action_callback() {
  return theme('webengage_callback',
    array(
      'wlc' => NULL,
      'vm' => NULL,
      'wwa' => NULL,
      'option' => NULL,
    )
  );
}
