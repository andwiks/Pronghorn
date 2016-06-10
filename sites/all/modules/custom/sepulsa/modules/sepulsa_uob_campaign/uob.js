(function($) {
  Drupal.behaviors.sepulsa_uob_campaign = {
    attach: function (context, settings) {
      console.log(settings.sepulsa_uob_campaign.uobvars);

      if(settings.sepulsa_uob_campaign.uobvars.anonym==true){
        openPop(".wrap_popup#login");
        $('#login .box_popup .close').hide();
        $('#login #user-login-form a[href="/user/password"]').attr("href", settings.sepulsa_uob_campaign.uobvars.forgot_destination);
        $('#login a[href="/user/register"]').attr("href", settings.sepulsa_uob_campaign.uobvars.register_destination);
      }
    }
  };

  $(document).ready(function () {
    $('#edit-submitted-no-hp').prop('required', false);
  });



})(jQuery);
