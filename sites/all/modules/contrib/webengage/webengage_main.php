<?php
/**
 * @file
 * main callback view
 *
 * @category view
 * @package   WebEngage
 * @link     http://www.webengage.com/
 */

 /**
  * Implements hook_preprocess_HOOK().
  */
function webengage_preprocess_webengage_main(&$vars) {
  global $user, $base_root;

  module_load_include("php", "webengage", "webengage_constants");
  module_load_include("php", "webengage", "helper");
  module_load_include("php", "webengage", "process_options");
  webengage_process_webengage_options();

  $vars['m_license_code_old'] = webengageGetLicenseCode();
  $vars['m_widget_status'] = webengageGetWidgetStatus();
  $vars['message'] = urldecode(isset($_REQUEST['message']) ? $_REQUEST['message'] : "");
  $vars['error_message'] = urldecode(isset($_REQUEST['error-message']) ? $_REQUEST['error-message'] : "");
  $vars['update_form'] = drupal_get_form('webengage_form_save_license_code');

  $vars['email'] = variable_get('site_mail', ini_get('sendmail_from'));
  $vars['user_full_name'] = $user->name;

  $vars['main_url'] = $base_root . base_path() . '?q=' . PATH_MAIN . '';
  $vars['next_url'] = $base_root . base_path() . '?q=' . PATH_CALLBACK . '&noheader=true';
  //$vars['resize_url'] = $base_root . base_path() . '?q=' . PATH_RESIZE . '&noheader=true';
  $vars['activation_url'] = $vars['main_url'] . "&weAction=activate";

  if(isset($_SERVER['HTTP_HOST'])) {
    $vars['domain_name'] = $_SERVER['HTTP_HOST'];
  }
  else {
    $vars['domain_name'] = $_SERVER['SERVER_NAME'];
  }

  $vars['webengage_host_name'] = "webengage.com";
  $vars['webengage_host'] = 'http://' . $vars['webengage_host_name'];
  $vars['secure_webengage_host'] = 'https://' . $vars['webengage_host_name'];
  $vars['resend_email_url'] = $vars['webengage_host'] . "/thirdparty/signup.html?action=resendVerificationEmail&licenseCode=" . urlencode($vars['m_license_code_old']) . "&next=" . urlencode($vars['next_url']) .
        "&activationUrl=" . urlencode($vars['activation_url']) . "&channel=drupal";
}

 /**
  * Callback for main path.
  *
  * @return string
  *   the theme block
  */
function webengage_do_action_main() {
  return theme('webengage_main',
    array(
      'm_license_code_old' => NULL,
      'm_widget_status' => NULL,
      'message' => NULL,
      'error_message' => NULL,
      'update_form' => NULL,
    )
  );
}
