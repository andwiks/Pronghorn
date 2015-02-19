/**
 * Automatically submit payment data to get Veritrans Token.
 */

;(function($) {
  Drupal.behaviors.commerceVeritrans = {
    attach: function (context, settings) {
      if (typeof Veritrans == "undefined") {
    	Veritrans = {};
      }
      Veritrans.url = settings.vt_url + '/token';
      Veritrans.client_key = settings.vt_client;
      
      var card = function () {
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
    
      function callback(response) {
        if (response.redirect_url) {
          openDialog(response.redirect_url);
        }
        else {
          if (settings.vt_secure) {
        	closeDialog();
          }
          if (response.status_code == "200") {
        	$("#token_id").val(response.token_id);
        	$("#error_message").val("");
          }
          else {
        	$("#error_message").val(response.status_message);
          }
          $("form").submit();
        }
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
      
      $("#edit-continue", context).on("click", function (event) {
    	if (settings.vt_type == "oneclick" 
    	  && $("#edit-commerce-payment-payment-details-veritrans-tokens").is("select")
    	  && $("#edit-commerce-payment-payment-details-veritrans-tokens").val() != "0"
    	) {
          $("#token_id").val($("#edit-commerce-payment-payment-details-veritrans-tokens").val());
          return true;
    	}
    	event.preventDefault();
        Veritrans.token(card, callback);     
        return false;
      });  
      
      $("#edit-commerce-payment-payment-details-veritrans-tokens").on("change ready", function () {
   		$("#edit-commerce-payment-payment-details-veritrans-credit-card").toggleClass("collapsed", ($(this).val() != "0"));
      });
    }
  }
})(jQuery);
