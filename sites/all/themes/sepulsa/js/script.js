(function($) {

  Drupal.behaviors.sepulsaTheme = {
    attach: function (context, settings) {
      $('form[id^="commerce-cart-add-to-cart-form"] :input[name="op"]').on('state:disabled', function(e) {
        $(this).toggleClass('inactive', e.value);
      });

      // checkbox
      $('.checkbox input[type="checkbox"], .radio input[type="radio"]').each(function() {
        if ($(this).is(':checked')) {
          $(this).closest('.checkbox').addClass('checked');
          $(this).closest('.radio').addClass('checked');
        }
      });
      $('.checkbox input[type="checkbox"]').bind('change', function() {
        if ($(this).is(':checked')) {
          $(this).closest('.checkbox').addClass('checked');
        }
        else {
          $(this).closest('.checkbox').removeClass('checked');
        }
      });

      $(':input[name="commerce_payment[payment_details][veritrans][tokens]"]', context).on('change', function () {
        $('select.selector').next('span.selector').remove();
        $('select.selector').removeClass('hasCustomSelect');
        $('select.selector').removeAttr('style');
        $('select.selector').customSelect();

        $(this).siblings('.description').toggle(!!parseInt($(this).val()));
      });
    }
  }

})(jQuery);
