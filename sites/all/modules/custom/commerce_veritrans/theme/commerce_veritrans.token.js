/**
 * Automatically submit payment data to get Veritrans Token.
 */

;(function($) {
  Drupal.behaviors.commerceVeritrans = {
    attach: function (context, settings) {

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
        $form = $('form#commerce-checkout-form-checkout');
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

          $form.submit();
        }
      }

      $.ajax({
        url: settings.vt_js,
        dataType: "script",
        cache: true
      }).done(function (script, textStatus) {
        Veritrans.url = settings.vt_url + '/token';
        Veritrans.client_key = settings.vt_client;
      });

      var card = function () {
        if (settings.vt_type == "twoclick"
          && $(':input[name="commerce_payment[payment_details][veritrans][tokens]"]').is('select')
          && $(':input[name="commerce_payment[payment_details][veritrans][tokens]"]').val() != '0'
        ) {
          return {
            "card_cvv": $(':input[name="commerce_payment[payment_details][veritrans][code2]"]').val(),
            "token_id" : $(':input[name="commerce_payment[payment_details][veritrans][tokens]"]').val(),
            "two_click" : true,
            "secure": settings.vt_secure,
            "bank": settings.vt_bank,
            "gross_amount": settings.vt_amount
          }
        }
        return {
          "card_number": $(':input[name="commerce_payment[payment_details][veritrans][credit_card][number]"]').val(),
          "card_exp_month": $(':input[name="commerce_payment[payment_details][veritrans][credit_card][exp_month]"]').val(),
          "card_exp_year": $(':input[name="commerce_payment[payment_details][veritrans][credit_card][exp_year]"]').val(),
          "card_cvv": $(':input[name="commerce_payment[payment_details][veritrans][credit_card][code]"]').val(),
          "secure": settings.vt_secure,
          "bank": settings.vt_bank,
          "gross_amount": settings.vt_amount
        }
      };

      $("#edit-continue", context).on("click", function (event) {
        /**
         * Check only if commerce_veritrans is selected as payment method
         */
        if ($(':input[name="commerce_payment[payment_method]"][value^="commerce_veritrans"]').is(':checked')) {
          if (settings.vt_type == "oneclick"
            && $(':input[name="commerce_payment[payment_details][veritrans][tokens]"]').is('select')
            && $(':input[name="commerce_payment[payment_details][veritrans][tokens]"]').val() != '0'
          ) {
            $('#token_id').val($(':input[name="commerce_payment[payment_details][veritrans][tokens]"]').val());
            return true;
          }
          event.preventDefault();
          if (typeof Veritrans === "undefined") {
            alert("Unable to connect to payment server! Please check your internet connection and reload the page.");
            return false;
          }
          Veritrans.token(card, callback);
          return false;
        };
      });

      $(':input[name="commerce_payment[payment_details][veritrans][credit_card][code]"], :input[name="commerce_payment[payment_details][veritrans][code2]"]', context).on('focus', function (event) {
        $(this).removeAttr('readonly').off(event);
      });


    }
  }
})(jQuery);
