<?php
/**
 * @file
 * helper
 *
 * @category helper
 * @package   WebEngage
 * @link     http://www.webengage.com/
 */

 /**
  * Returns the main script.
  *
  * @param string $license_code
  *   The signedup license code
  *
  * @return string
  *   The integration script
  */
function webengage_get_webengage_script($license_code) {
  return "
    <!-- Added via WebEngage Drupal Plugin 2.0.2 -->
    <script id=\"_webengage_script_tag\" type=\"text/javascript\">
    var _weq = _weq || {};
    _weq['webengage.licenseCode'] = '" . $license_code . "';
    _weq['webengage.widgetVersion'] = \"4.0\";

    (function(d){
      var _we = d.createElement('script');
      _we.type = 'text/javascript';
      _we.async = true;
      _we.src = (d.location.protocol == 'https:' ? \"https://ssl.widgets.webengage.com\" : \"http://cdn.widgets.webengage.com\") + \"/js/widget/webengage-min-v-4.0.js\";
      var _sNode = d.getElementById('_webengage_script_tag');
      _sNode.parentNode.insertBefore(_we, _sNode);
    })(document);
  </script>
  ";
}

 /**
  * Saves the license code.
  *
  * @param string $license_code
  *   The signedup license code
  *
  * @return int
  *   the new row id
  */
function webengageSetLicenseCode($license_code) {
  $nid = NULL;

  try{
    $nid = db_update('webengage_settings')->fields(array(
      'option_value' => $license_code,
    ))
    ->condition('option_key', 'license_code')
    ->execute();
  }
  catch (PDOException $e) {
    drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
  }

  return $nid;
}

 /**
  * Gets the license code.
  *
  * @return string
  *   the license code
  */
function webengageGetLicenseCode() {
  $license_code = NULL;

  try{
    $result = db_select('webengage_settings', 'w')
      ->fields('w', array('option_value'))
      ->condition('option_key', 'license_code')
      ->range(0, 1)
      ->execute()
      ->fetchAssoc();

    $license_code = $result['option_value'];
  }
  catch(Exception $e) {
    drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
  }

  return $license_code;
}

 /**
  * Saves the widget status.
  *
  * @param string $widget_status
  *   The new widget status
  *
  * @return int
  *   the new row id
  */
function webengageSetWidgetStatus($widget_status) {
  $nid = NULL;

  try{
    $nid = db_update('webengage_settings')
      ->fields(array(
        'option_value' => $widget_status,
      ))
      ->condition('option_key', 'widget_status')
      ->execute();
  }
  catch (PDOException $e) {
    drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
  }

  return $nid;
}

 /**
  * Gets the widget status.
  *
  * @return int
  *   the new row id
  */
function webengageGetWidgetStatus() {
  $widget_status = NULL;

  try{
    $result = db_select('webengage_settings', 'w')
    ->fields('w', array('option_value'))
    ->condition('option_key', 'widget_status')
    ->range(0, 1)
    ->execute()
    ->fetchAssoc();

    $widget_status = $result['option_value'];
  }
  catch(Exception $e) {
    drupal_set_message(t('Error: %message', array('%message' => $e->getMessage())), 'error');
  }

  return $widget_status;
}
