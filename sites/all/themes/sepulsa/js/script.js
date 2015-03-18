(function($) {

  Drupal.behaviors.sepulsaTheme = {
    attach: function (context, settings) {
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
    }
  }

})(jQuery);
