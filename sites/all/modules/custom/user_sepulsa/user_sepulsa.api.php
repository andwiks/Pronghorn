<?php
/**
 * @file
 * user_sepulsa.api.php
 *
 * @author ananto@sepulsa.com
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implement this hook to generate a username for user_sepulsa module.
 *
 * Other modules may implement hook_email_registration_name($edit, $account)
 * to generate a username (return a string to be used as the username, NULL
 * to have email_registration generate it).
 *
 * @param array $edit
 *   The array of form values submitted by the user.
 * @param object $account
 *   The user object on which the operation is being performed.
 *
 * @return string
 *   A string defining a generated username.
 */
function hook_user_sepulsa_name($edit, $account) {
  // Your hook implementation should ensure that the resulting string
  // works as a username. You can use email_registration_cleanup_username($name)
  // to clean up the name.
  return _user_sepulsa_cleanup_username('u' . $account->uid);
}

/**
 * @} End of "addtogroup hooks".
 */
