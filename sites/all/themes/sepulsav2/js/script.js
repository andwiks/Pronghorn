(function($) {

  Drupal.behaviors.sepulsav2Theme = {
    attach: function (context, settings) {
      if (typeof settings.zclip != 'undefined') {
        $(settings.zclip.selector).zclip({
          path: settings.zclip.path,
          copy: $(settings.zclip.target).val(),
          afterCopy: function() {
            alert(settings.zclip.message);
          }
        });
      }
      $('#voucher_filter').change(function () {
        var filter = $(this).val();
        if (filter == 'All') {
            $('.box_voucher').show();
        } else {
            $('.box_voucher').hide();
            $('.' + filter).show();
        }
      });
    }
  }
})(jQuery);
