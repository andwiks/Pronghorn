<?php
/**
 * @file
 * request processor
 *
 * @category helper
 * @package   WebEngage
 * @link     http://www.webengage.com/
 */

 /**
  * Main request processor.
  */
function webengage_process_webengage_options() {
  global $base_root;
  $redirect_url = "";
  $main_url = $base_root . base_path() . '?q=admin/webengage/action/main&noheader=true';

  if (isset($_REQUEST['weAction'])) {
    if ($_REQUEST['weAction'] === 'wp-save') {
      $message = webengage_update_webengage_options();
      $redirect_url = $main_url . '&' . $message[0] . "=" . urlencode($message[1]);
    }
    elseif ($_REQUEST['weAction'] === 'reset') {
      $message = webengage_reset_webengage_options();
      $redirect_url = $main_url . '&' . $message[0] . "=" . urlencode($message[1]);
    }
    elseif ($_REQUEST['weAction'] === 'activate') {
      $message = webengage_activate_we_widget();
      $redirect_url = $main_url . '&' . $message[0] . "=" . urlencode($message[1]);
    }
    elseif ($_REQUEST['weAction'] === 'discardMessage') {
      webengage_discard_status_message();
      $redirect_url = $main_url;
    }

    if (strlen($redirect_url) > 0) {
      drupal_goto($redirect_url);
    }
  }
}

 /**
  * Discard message processor.
  */
function webengage_discard_status_message() {
  webengageSetWidgetStatus('');
}

 /**
  * Resetting processor.
  */
function webengage_reset_webengage_options() {
  webengageSetLicenseCode('');
  webengageSetWidgetStatus('');
  return array("message", "Your WebEngage options are deleted. You can signup for a new account.");
}

 /**
  * Update processor.
  */
function webengage_update_webengage_options() {
  $wlc = isset($_REQUEST['webengage_license_code']) ? htmlspecialchars($_REQUEST['webengage_license_code'], ENT_COMPAT, 'UTF-8') : "";
  $vm = isset($_REQUEST['verification_message']) ? htmlspecialchars($_REQUEST['verification_message'], ENT_COMPAT, 'UTF-8') : "";
  $wws = isset($_REQUEST['webengage_widget_status']) ? htmlspecialchars($_REQUEST['webengage_widget_status'], ENT_COMPAT, 'UTF-8') : "ACTIVE";
  if (!empty($wlc)) {
    webengageSetLicenseCode(trim($wlc));
    webengageSetWidgetStatus($wws);
    $msg = !empty($vm) ? $vm : "Your WebEngage widget license code has been updated.";
    return array("message", $msg);
  }
  else {
    return array("error-message", "Please add a license code.");
  }
}

 /**
  * Activate processor.
  */
function webengage_activate_we_widget() {
  $wlc = isset($_REQUEST['webengage_license_code']) ? htmlspecialchars($_REQUEST['webengage_license_code'], ENT_COMPAT, 'UTF-8') : "";
  $old_value = webengageGetLicenseCode();
  $wws = isset($_REQUEST['webengage_widget_status']) ? htmlspecialchars($_REQUEST['webengage_widget_status'], ENT_COMPAT, 'UTF-8') : "ACTIVE";
  if ($wlc === $old_value) {
    webengageSetWidgetStatus($wws);
    $msg = "Your plugin installation is complete. You can do further customizations from your WebEngage dashboard.";
    return array("message", $msg);
  }
  else {
    $msg = "Unauthorized plugin activation request";
    return array("error-message", $msg);
  }
}
