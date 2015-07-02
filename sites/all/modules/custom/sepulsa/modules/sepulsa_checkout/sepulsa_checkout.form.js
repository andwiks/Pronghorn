(function($) {
  $(document).ready(function() {
    if($("body.not-logged-in").length){
      $("form#commerce-checkout-form-checkout input[name=op]").attr("disabled", "disabled");
    }

    $("#edit-account-login-mail").on("input propertychange paste", function (event) {
      alert();
      if ($("#edit-account-login-mail").val() > "0") {
        $("form#commerce-checkout-form-checkout input[name=op]").toggleClass("inactive", false).removeAttr("disabled");
      } else {
        $("form#commerce-checkout-form-checkout input[name=op]").attr("disabled", "disabled");
      }
    });
  });
})(jQuery);
