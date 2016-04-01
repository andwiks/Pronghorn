<?php

/**
 * @file
 * Hooks provided by the entity API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * React to an email address requiring verification being updated.
 *
 * Please note: This hook is called AFTER any action set in the
 * settings.
 *
 * $current_mail is a database row containing:
 * - did
 * - email
 * - status
 * -- 0 = Unverified
 * -- 1 = Verified
 * - new_flag
 * -- 0 = Merge User
 * -- 1 = New User
 * - hash (hash expected, created when verification mail was sent)
 * - created
 *
 *
 * $status can be one of:
 *
 * - FALSE
 * When something went wrong - this should NOT happen. Possible reasons:
 * -- Hash not found
 * -- Original entity not found (this should not happen, because the
 *    hash should also be deleted then)
 *
 * - timeout
 * Hash called after timeout reached. Might also be set by cron.
 *
 * - confirmed
 * Hash called within time.
 *
 * - already_confirmed
 * Confirmation link is called again, after for an address already
 * confirmed.
 *
 * - already_timeout
 * Confirmation link is clicked again, after for an address already set
 * timeout. *
 */
function device_mail_verification_update($current_mail, $status) {
}

/**
 * @} End of "addtogroup hooks".
 */
