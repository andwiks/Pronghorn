/**
 * Automatically submit payment data to get Veritrans Token.
 */

;(function($) {
  Drupal.behaviors.commerceVeritrans = {
    attach: function (context, settings) {   
      function toggleCCForm(toggle) {
        $("#edit-commerce-payment-payment-details-veritrans-credit-card").toggleClass("collapsed", toggle).toggle(!toggle);
        $("#edit-commerce-payment-payment-details-veritrans-code2").toggle(toggle).prev("label").toggle(toggle);
      }
      
      function openDialog(url) {
        $.fancybox({
          href: url,
          type: "iframe",
          autoSize: false,
          width: 700,
          height: 500,
          closeBtn: false,
          modal: true
        });
      }
      
      function closeDialog() {
        $.fancybox.close();
      }
      
      function callback(response) {
        if (response.redirect_url) {
          openDialog(response.redirect_url);
        }
        else {
          if (settings.vt_secure) {
            closeDialog();
          }
          $("#response_data").val($.param(response, true));
          if (response.status_code == "200") {          
            $("#token_id").val(response.token_id);
            $("#error_message").val("");
          }
          else {
            $("#error_message").val(response.status_message);
          }
          $("form#commerce-checkout-form-checkout", context).submit();
        }
      }
      
      $.ajax({
        url: settings.vt_js,
        dataType: "script",
        cache: true
      }).done(function (script, textStatus) {
        Veritrans.url = settings.vt_url + '/token';
        Veritrans.client_key = settings.vt_client;
        $("#edit-commerce-payment-payment-details-veritrans-credit-card-phone-other").hide();
        if ($("#edit-commerce-payment-payment-details-veritrans-tokens").is("select")) {
          toggleCCForm($("#edit-commerce-payment-payment-details-veritrans-tokens").val() != "0");
        }
        if ($("#edit-commerce-payment-payment-details-veritrans-credit-card-phone").is("select")
          && $("#edit-commerce-payment-payment-details-veritrans-credit-card-phone").val() == "0"
        ) {
          $("#edit-commerce-payment-payment-details-veritrans-credit-card-phone-other").show();
        }
      });
                  
      var card = function () {
        if (settings.vt_type == "twoclick"
          && $("#edit-commerce-payment-payment-details-veritrans-tokens").is("select")
          && $("#edit-commerce-payment-payment-details-veritrans-tokens").val() != "0"
        ) {
          return {
            "card_cvv": $("#edit-commerce-payment-payment-details-veritrans-code2").val(),
            "token_id" : $("#edit-commerce-payment-payment-details-veritrans-tokens").val(),
            "two_click" : true,
            "secure": settings.vt_secure,
            "bank": settings.vt_bank,
            "gross_amount": settings.vt_amount
          }
        } 
        return {
          "card_number": $("#edit-commerce-payment-payment-details-veritrans-credit-card-number").val(),
          "card_exp_month": $("#edit-commerce-payment-payment-details-veritrans-credit-card-exp-month").val(),
          "card_exp_year": $("#edit-commerce-payment-payment-details-veritrans-credit-card-exp-year").val(),
          "card_cvv": $("#edit-commerce-payment-payment-details-veritrans-credit-card-code").val(),
          "secure": settings.vt_secure,
          "bank": settings.vt_bank,
          "gross_amount": settings.vt_amount
        }
      };
                   
      $("#edit-continue", context).on("click", function (event) {
        if (settings.vt_type == "oneclick" 
          && $("#edit-commerce-payment-payment-details-veritrans-tokens").is("select")
          && $("#edit-commerce-payment-payment-details-veritrans-tokens").val() != "0"
        ) {
          $("#token_id").val($("#edit-commerce-payment-payment-details-veritrans-tokens").val());
          return true;
        }
        event.preventDefault();
        if (typeof Veritrans === "undefined") {
          alert("Unable to connect to payment server! Please check your internet connection and reload the page.");
          return false;
        }
        Veritrans.token(card, callback);
        return false;
      });  
            
      $("#edit-commerce-payment-payment-details-veritrans-credit-card-code, #edit-commerce-payment-payment-details-veritrans-code2", context).on("focus", function (event) {
        $(this).removeAttr("readonly").off(event);
      });
      
      $("select#edit-commerce-payment-payment-details-veritrans-credit-card-phone", context).on("change", function () {
        $("#edit-commerce-payment-payment-details-veritrans-credit-card-phone-other").toggle(($(this).val() == "0"));
      });
      
      $("#edit-commerce-payment-payment-details-veritrans-tokens", context).on("change", function () {
        toggleCCForm(($(this).val() != "0"));
      });
    }
  }
})(jQuery);
