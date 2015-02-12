/**
 * Automatically submit payment data to get Veritrans Token.
 */

;(function($) {
  Drupal.behaviors.commerceVeritrans = {
    attach: function (context, settings) {
      Veritrans.url = settings.vt_url+'/token';
      Veritrans.client_key = settings.vt_client;
      
      var card = function () {
        return {
          "card_number": $("#edit-commerce-payment-payment-details-veritrans-credit-card-number").val(),
          "card_exp_month": $("#edit-commerce-payment-payment-details-veritrans-credit-card-exp-month").val(),
          "card_exp_year": $("#edit-commerce-payment-payment-details-veritrans-credit-card-exp-year").val(),
          "card_cvv": $("#edit-commerce-payment-payment-details-veritrans-credit-card-code").val(),
          "secure": true,
          "gross_amount": settings.vt_amount
        }
      };
	
	  function callback(response) {
	    console.log(response);
	    // @todo: can not invoke 3DS.
	    if (response.redirect_url) {
	      openDialog(response.redirect_url);
	    }
	    else if (response.status_code == "200") {     
	      closeDialog();
	      $("#token_id").val(response.token_id);
	      $("#commerce-checkout-form-checkout").submit();
	    }
	    else {
	    	// @todo: improve this error message.
	    	// Example: error with cvv.
	    	// Example: network error.
	      alert(response.status_message);
	      closeDialog();
	      $("#edit-continue").removeAttr("disabled", "disabled");
	      return false;
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
	      event.preventDefault();
	      $(this).attr("disabled", "disabled");
	      Veritrans.token(card, callback);
	      return false;
	  });
    }
  }
})(jQuery);
