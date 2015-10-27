<?php

function hybridauth_facebook_twitter_login_services_process($data){
    global $user;
    
    $return = array();
    //$data['access_token']
    //$data['access_secret']
    //$data['provider'] Facebook, Twitter
    //$data['source']
    //$data['email']
    
    $return["email_request"] = 0;
    
    //access token validation
    if(!isset($data['access_token']) || empty($data['access_token'])){
        $return["return"] = 0;
        $return["message"] = t("Token anda tidak valid");
        return $return;
    }
    
    //provider validation
    if(isset($data['provider']) && !empty($data['provider']) && strtolower($data['provider']) == 'facebook'){
        $data['provider'] = "Facebook";
    }
    elseif(isset($data['provider']) && !empty($data['provider']) && strtolower($data['provider']) == 'twitter'){
        $data['provider'] = "Twitter";
    }
    else{
        $return["return"] = 0;
        $return["message"] = t("Provider login anda tidak valid");
        return $return;
    }
    
    if($data['provider'] == "Twitter"){
        //access token validation
        if(!isset($data['access_secret']) || empty($data['access_secret'])){
            $return["return"] = 0;
            $return["message"] = t("Secret token anda tidak valid");
            return $return;
        }
        
        module_load_include("php", "hybridauth_facebook_twitter_login_services", "TwitterAPIExchange");

        /** Set access tokens here - see: https://dev.twitter.com/apps/ **/
        $settings = array(
            'oauth_access_token' => $data['access_token'],
            'oauth_access_token_secret' => $data['access_secret'],
            'consumer_key' => variable_get("hybridauth_provider_Twitter_keys_key","tm3JggaFEi93Kq3ufC5pPclBO"),
            'consumer_secret' => variable_get("hybridauth_provider_Twitter_keys_secret","Xv3poUULrnJkf4R25Bk2RfMY90gKIzUtoZv2DsdODJC32kguDa")
        );
        
        $url = 'https://api.twitter.com/1.1/account/verify_credentials.json';
        $requestMethod = 'GET';
        $twitter = new TwitterAPIExchange($settings);
        $response = json_decode($twitter->setGetfield($getfield)->buildOauth($url, $requestMethod)->performRequest());     
        
        //define the container
        $hybriauth_user_profile = new stdClass();
        $hybriauth_user_profile->user = new stdClass();
        $hybriauth_user_profile->user->profile = new stdClass();
        
        # store the user profile.  
        $hybriauth_user_profile->user->profile->identifier = (property_exists($response, 'id')) ? $response->id : "";
        $hybriauth_user_profile->user->profile->displayName = (property_exists($response, 'screen_name')) ? $response->screen_name : "";
        $hybriauth_user_profile->user->profile->description = (property_exists($response, 'description')) ? $response->description : "";
        $hybriauth_user_profile->user->profile->firstName = (property_exists($response, 'name')) ? $response->name : "";
        $hybriauth_user_profile->user->profile->photoURL = (property_exists($response, 'profile_image_url')) ? (str_replace('_normal', '', $response->profile_image_url)) : "";
        $hybriauth_user_profile->user->profile->profileURL = (property_exists($response, 'screen_name')) ? ("http://twitter.com/" . $response->screen_name) : "";
        $hybriauth_user_profile->user->profile->webSiteURL = (property_exists($response, 'url')) ? $response->url : "";
        $hybriauth_user_profile->user->profile->region = (property_exists($response, 'location')) ? $response->location : "";
        if(isset($data['email']) && !empty($data['email'])){
            $hybriauth_user_profile->user->profile->email = $data['email'];
        }
        else{
            $hybriauth_user_profile->user->profile->email = "";
        }
        
        $user_profile = (array)$hybriauth_user_profile->user->profile;
        $user_profile["provider"] = $data['provider'];
        
        $identity = _hybridauth_identity_load($user_profile);
        
        if (!$identity) {
            if(isset($user_profile['email']) && !empty($user_profile['email'])) {
                $check_email_exist = user_load_by_mail($user_profile['email']);
                if($check_email_exist){
                    $return["return"] = 0;
                    $return["message"] = t("Email yg anda input telah digunakan oleh account lain");
                    return $return;
                }
            }
            else{
                $return["return"] = 0;
                $return["email_request"] = 1;
                $return["message"] = t("Tolong masukan email");
                return $return;
            }
        }
    }
    elseif($data['provider'] == "Facebook"){
        $user_details = "https://graph.facebook.com/me?access_token=" .$data['access_token'];

        $response = file_get_contents($user_details);
        $response = json_decode($response,true);

        $return["return"] = 0;

        //convert data response facebook to hybridauth profile format
        //define the container
        $hybriauth_user_profile = new stdClass();
        $hybriauth_user_profile->user = new stdClass();
        $hybriauth_user_profile->user->profile = new stdClass();

        # store the user profile.
        $hybriauth_user_profile->user->profile->identifier = (array_key_exists('id', $response)) ? $response['id'] : "";
        $hybriauth_user_profile->user->profile->username = (array_key_exists('username', $response)) ? $response['username'] : "";
        $hybriauth_user_profile->user->profile->displayName = (array_key_exists('name', $response)) ? $response['name'] : "";
        $hybriauth_user_profile->user->profile->firstName = (array_key_exists('first_name', $response)) ? $response['first_name'] : "";
        $hybriauth_user_profile->user->profile->lastName = (array_key_exists('last_name', $response)) ? $response['last_name'] : "";
        $hybriauth_user_profile->user->profile->photoURL = "https://graph.facebook.com/" . $hybriauth_user_profile->user->profile->identifier . "/picture?width=150&height=150";
        $hybriauth_user_profile->user->profile->coverInfoURL = "https://graph.facebook.com/" . $hybriauth_user_profile->user->profile->identifier . "?fields=cover&access_token=" . $data['access_token'];
        $hybriauth_user_profile->user->profile->profileURL = (array_key_exists('link', $response)) ? $response['link'] : "";
        $hybriauth_user_profile->user->profile->webSiteURL = (array_key_exists('website', $response)) ? $response['website'] : "";
        $hybriauth_user_profile->user->profile->gender = (array_key_exists('gender', $response)) ? $response['gender'] : "";
        $hybriauth_user_profile->user->profile->language = (array_key_exists('locale', $response)) ? $response['locale'] : "";
        $hybriauth_user_profile->user->profile->description = (array_key_exists('about', $response)) ? $response['about'] : "";
        $hybriauth_user_profile->user->profile->email = (array_key_exists('email', $response)) ? $response['email'] : "";
        $hybriauth_user_profile->user->profile->emailVerified = (array_key_exists('email', $response)) ? $response['email'] : "";
        $hybriauth_user_profile->user->profile->region = (array_key_exists("location", $response) && array_key_exists("name", $response['location'])) ? $response['location']["name"] : "";

        $user_profile = (array)$hybriauth_user_profile->user->profile;
        $user_profile["provider"] = $data['provider'];
        
        $identity = _hybridauth_identity_load($user_profile);
    }
    
    // User is already logged in, tries to add new identity.
    if (user_is_logged_in()) {
        // Identity is already registered.
        if ($identity) {
          // Registered to this user.
          if ($user->uid == $identity['uid']) {
            $return["message"] = t("Sudah terdaftar");
          }
          // Registered to another user.
          else {
            $return["message"] = t("Credential anda sudah dipakai di account lain");
          }
        }
        // Identity is not registered - add it to the logged in user.
        elseif($user_profile["provider"] != "Twitter") {
          _hybridauth_identity_save($user_profile);
          $return["return"] = 0;
          $return["message"] = t("Data anda telah ditambahkan");
        }
    }

    if ($identity) {
        // Check if user is blocked.
        if ($account = _hybridauth_user_is_blocked_by_uid($identity['uid'])) {
            $return["return"] = 0;
            $return["message"] = t('Username %name belum belum diaktivasi atau sedang diblock.', array('%name' => $account->name));
        }
        // Check for email verification timestamp.
        elseif (!_hybridauth_user_login_access_by_uid($identity['uid'])) {
            $return["return"] = 0;
            $data = unserialize($identity['data']);
            $return["message"] = t('You need to verify your e-mail address - !email. A welcome message with further instructions has been sent to your e-mail address.', array('!email' => $data['email']));
            _hybridauth_mail_notify('hybridauth_email_verification', user_load($identity['uid']));
        }
        else {
            $form_state['uid'] = $identity['uid'];
            user_login_submit(array(), $form_state);
            _hybridauth_invoke_hooks('hybridauth_user_login', $user, $user_profile);
        }
    }
    // Handle duplicate email addresses.
    elseif (variable_get('hybridauth_duplicate_emails', 1) && !empty($data['email']) && $account = user_load_by_mail($data['email'])) {
        // Add identity to existing account, only if emailVerified.
        if (variable_get('hybridauth_duplicate_emails', 1) == 2 && $data['email'] == $data['emailVerified']) {
            if (!$identity) {
                _hybridauth_identity_save($user_profile, $account->uid);
            }
            _hybridauth_invoke_hooks('hybridauth_identity_added', $account, $user_profile);
            $form_state['uid'] = $account->uid;
            user_login_submit(array(), $form_state);
            _hybridauth_invoke_hooks('hybridauth_user_login', $user, $user_profile);
            $return["return"] = 1;
            $return["message"] = t("Identitas diri anda telah di update");
            $return["token"] = drupal_get_token('services');
            return $return;
        }
        // Block registration - if (variable_get('hybridauth_duplicate_emails', 1) == 1) or
        // (variable_get('hybridauth_duplicate_emails', 1) == 2 && $data['email'] != $data['emailVerified'])
        else {
            if (!empty($account->data['hybridauth'])) {
                $providers = hybridauth_providers_list();
                $return["return"] = 0;
                $return["message"] = t("Kamu mencoba login dengan email yang sudah terdaftar, mohon login menggunakan %provider.",
                array('%provider' => $providers[$account->data['hybridauth']['provider']]));
            }
            else {
                $return["return"] = 0;
                $return["message"] = t('Kamu mencoba login dengan email yang sudah terdaftar, apakah kamu mau reset password? - <a href="@password">request new password</a>.',
                array('@password' => url('user/password')));
            }
        }
    }
    // Check if other modules want to block this registration.
    elseif ($message = _hybridauth_registration_block($user_profile)) {
      // Destroy the session with the blocked authorized identity.
      session_destroy();
      if (is_string($message)) {
        $return["return"] = 0;
        $return["message"] = $message;
      }
    }
    // Create new user account.
    else {
      // Visitors can create accounts.
      if ((variable_get('hybridauth_register', 0) == 0 && variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL))
        || variable_get('hybridauth_register', 0) == 1 || variable_get('hybridauth_register', 0) == 2) {
        _hybridauth_invoke_hooks('hybridauth_user_preinsert', $user, $user_profile);

        // As we have already checked for the additional info we can unset the plaintext $data['pass'] here.
        if (isset($user_profile['pass'])) {
          $user_password = $user_profile['pass'];
          unset($user_profile['pass']);
        }

        // TODO: remove this global if possible.
        global $_hybridauth_data;
        $_hybridauth_data = $user_profile;
        // Register this new user.
        $name = _hybridauth_make_username($user_profile);
        $userinfo = array(
          'name' => $name,
          'pass' => empty($user_password) ? user_password() : $user_password,
          'init' => $name,
          'status' => 1,
          'access' => REQUEST_TIME,
          'mail' => $user_profile['email'],
          'data' => array('hybridauth' => $user_profile),
        );
        // Invoke hook_hybridauth_userinfo_alter().
        drupal_alter('hybridauth_userinfo', $userinfo, $user_profile);

        $admin_approval_required = FALSE;
        // Admin approval is required.
        if ((variable_get('hybridauth_register', 0) == 0 && variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL) == USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL)
          || variable_get('hybridauth_register', 0) == 2) {
          $userinfo['status'] = 0;
          $admin_approval_required = TRUE;
        }
        
        $account_check = user_load_by_mail($userinfo["mail"]);
        if(!$account_check){
            return $userinfo;
            //$account = user_save(drupal_anonymous_user(), $userinfo);
        }
        else{
            $account = $account_check;
        }
        
        // Terminate if an error occurred during user_save().
        if (!$account) {
            $return["return"] = 0;
            $return["message"] = t("Penyimpanan akun anda gagal");
            return $return;
        }
        _hybridauth_invoke_hooks('hybridauth_user_insert', $account, $user_profile);
        if (!$identity) {
            _hybridauth_identity_save($user_profile, $account->uid);
        }
        _hybridauth_invoke_hooks('hybridauth_identity_added', $account, $user_profile);
        
        $user_save_trigger = FALSE;
        $user_email_verify_trigger = FALSE;
        $user_login_trigger = TRUE;
        
        // Admin approval is required.
        if ($admin_approval_required) {
          $user_login_trigger = FALSE;
          _user_mail_notify('register_pending_approval', $account);
          $return["return"] = 0;
          $return["message"] = t("Terima Kasih telah mendaftar ke situs kami, akun anda sekarang sedang menunggu verifikasi");
        }
        // Email verification is required.
        elseif (!empty($user_profile['email']) && $user_profile['email'] != $user_profile['emailVerified']
          && ((!variable_get('hybridauth_email_verification', 0) && variable_get('user_email_verification', TRUE)) || variable_get('hybridauth_email_verification', 0) == 1)) {
          $user_login_trigger = FALSE;
          // Dries birthday timestamp, Nov 19, 1978 = 280281600 :).
          $edit['login'] = 280281600;
          $user_save_trigger = TRUE;
          $user_email_verify_trigger = TRUE;
        }

        if ($user_save_trigger) {
          // Hack to remove one notice from Legal module.
          if (module_exists('legal')) {
            $edit['legal_accept'] = NULL;
          }
          $account = user_save($account, $edit);
        }
        if ($user_email_verify_trigger) {
          _hybridauth_mail_notify('hybridauth_email_verification', $account);
          $return["return"] = 0;
          $return["message"] = t("Terima Kasih telah mendaftar ke situs kami, mohon verifikasi akun anda melalui email yg telah kami kirimkan");
        }
        // Log user in.
        if ($user_login_trigger) {
          $form_state['uid'] = $account->uid;
          user_login_submit(array(), $form_state);
          _hybridauth_invoke_hooks('hybridauth_user_login', $user, $user_profile);
          $return["return"] = 1;
          $return["token"] = drupal_get_token('services');
          return $return;
        }
      }
      // Visitors can't register accounts through HybridAuth.
      elseif (variable_get('hybridauth_register', 0) == 3) {
        $return["return"] = 0;
        $return["message"] = t("Registrasi akun tidak diperbolehkan");
        return $return;
      }
      // Only admin can create accounts.
      else {
        $return["return"] = 0;
        $return["message"] = t("Mohon kontak kami untuk mendaftar akun baru");
        return $return;
      }
    }
    
    if($user->uid > 0){
       $return["return"] = 1;
       $return["token"] = drupal_get_token('services');
    }
    return $return;
}