<?php
/**
 * @file
 * main view renderer
 *
 * @category view
 * @package   WebEngage
 * @link     http://www.webengage.com/
 */
?>

<div class="wrap Configuration">
  <?php
    // Rendering message/error-message as a wp-message if any.
    if(isset($message) && strlen($message) > 0):
   ?>
    <div id="message" class="updated mag_message_box" style="font-size:16px;">
      <p class="mag_message_box_text"><?php echo $message; ?></p>
    </div>
  <?php elseif (isset($error_message) && strlen($error_message) > 0): ?>
    <div id="message" class="weerror mag_message_box">
      <p class="mag_message_box_text"><?php echo $error_message; ?></p>
    </div>
  <?php elseif ($m_widget_status != '' && $m_widget_status === 'PENDING'): ?>
    <?php 
      $discard_url = $main_url . "&weAction=discardMessage" . "&webengage_license_code=" . $m_license_code_old;
    ?>
    <div id="message" class="weerror mag_message_box">
      <p class="mag_message_box_text">Your WebEngage plugin is not active currently because the email associated with your account needs to be verified.
        We have sent you the activation email already, please follow the instructions therein. In case you haven't received the email, <a class="resend-email-link" href="#">click here to resend the activation email</a>.<span class="discard-msg">[ <a onclick="return confirm('Do you really wish to discard this message?');" href="<?php echo $discard_url ?>" title="Discard this message">discard</a> ]</span></p>
    </div>
  <?php endif; ?>
  <div class="webengage-container">
    <div class="webengage-logo-container">
      <span />
    </div>

    <?php if (!$m_license_code_old || $m_license_code_old === ''): ?>
      <div class="webengage-form-container">
        <div class="webengage-form inline-forms">
          <h3>New to WebEngage? Create your account</h3>
          <hr/>
          <?php 
            $src_url = $secure_webengage_host . "/thirdparty/signup.html?" . "next=" . urlencode($next_url) .
                        "&em=" . urlencode($email) . "&domain=" . urlencode($domain_name) .
                        "&activationUrl=" . urlencode($main_url . "&weAction=activate") .
                        "&channel=drupal";
          ?>
          <div id="webengage-loading-info">Loading, please wait ...</div>
          <iframe src="<?php echo $src_url; ?>" class="webengage-iframe signup-iframe" id="we-signup-iframe" marginheight="0" frameborder="0" style="height:0;background-color:transparent;" allowTransparency="true"></iframe>
        </div>

        <div class="webengage-form inline-forms">
          <h3>Already a Webengage user? Add license code for <?php echo $domain_name ?></h3>
          <hr>
          <p>Copy the license code for <?php echo $domain_name; ?> from your <a target="_blank" class="external_icon" href="<?php echo $webengage_host ?>/publisher/dashboard">WebEngage Dashboard</a> and paste it here.</p>
          <?php echo drupal_render($update_form); ?>
        </div>

        <br class="clear"/>
      </div>
    <?php else: ?>
      <div class="webengage-form-container">
        <?php echo drupal_render($update_form); ?>
        Or,  <a class="form-submit" href="<?php echo $main_url ?>&weAction=reset" onclick="return confirm('Do you wish to change your email or domain name for this plugin?');">Reset the license code</a>
      </div>
      
      <div class="webengage-instructions">
        Your WebEngage widget is installed. What's next?
        <ol>
          <li>
            Manage this widget from your <a href="http://webengage.com/publisher/dashboard">WebEngage dashboard</a>. List of things you can do -
            <ul style="padding-left:30px;">
              <li style="margin-top:6px;"><b>Feedback</b>: Create your feedback tab with custom text and colors. Choose your language for the widget. Manage your categories and fields for feedback.</li>
              <li><b>Notifications</b>: Create your own short notification. Add filters (targeting rules) to show this notification only to a specific set of audience. Drive traffic. Increase conversion. Get real-time reports.</li>
              <li><b>Surveys</b>: Create surveys and add targeting rules for them. Modify CSS to customize the survey skin to match your site's look and feel. View realtime analytics & reports.</li>
            </ul>
          </li>
          <li>The widget is not appearing on your site even after saving the license code? This is most likely beacause you haven't verified your email yet. <a class="resend-email-link" href="#">Resend the activation email</a>.</li>
          <li>Can't access your dashboard? Try <a href="http://webengage.com/user/account.html?action=viewForgot">forgot password</a>. If nothing works, please <a href="http://webengage.com/contact">get in touch</a> with us.</li>
          <li>Choose from a range of <a href="http://webengage.com/pricing">pricing plans</a> that fits your requirement. Uprades or downgrades done in the dashboard reflect automatically on your site without changing anything in the plugin.</li>
        </ol>
      </div>
      <script>
        window.onload = function() {
          var resendLinks = document.getElementsByTagName('a');
          for(var i = 0; i < resendLinks.length; i++) {
                var resendLink = resendLinks[i];
                if(resendLink.className === 'resend-email-link') {
              resendLink.onclick = function() {
                    var newFrame = document.createElement("iframe");
                newFrame.style.height = "0px";
                newFrame.setAttribute("marginheight", "0");
                newFrame.setAttribute("frameborder", "0");
                newFrame.setAttribute("src", "<?php echo $resend_email_url; ?>");
                document.body.appendChild(newFrame);
                return false;
              }
                }
          }
            }
      </script>
      <br class="clear"/>
    <?php endif; ?>
  </div>
  <script type="text/javascript">
    if (document.getElementById('we-signup-iframe')) {
      var resizeIframe = function (height) {
        document.getElementById('we-signup-iframe').style.height = (parseInt(height) + 60) + "px";
      };
    
      if (typeof window['addEventListener'] !== 'undefined' && typeof window['postMessage'] !== 'undefined') {
        window.addEventListener("message", function (e) {
          if (e.origin.search("<?php echo $webengage_host_name; ?>") < 0) {
            return;
          }
          resizeIframe(e.data);
        }, false);
      }
      
      document.getElementById('we-signup-iframe').onload = function () {
        if (typeof window['addEventListener'] === 'undefined' || typeof window['postMessage'] === 'undefined') {
          document.getElementById('we-signup-iframe').style.height = "450px";
        }
        setTimeout(function () {
          if (document.getElementById('webengage-loading-info')) {
      document.getElementById('webengage-loading-info').style.display = 'none';
          }
        }, 500);
      };
    }
  </script>
</div>
