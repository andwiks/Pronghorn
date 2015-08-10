(function ($) {

/**
 * Command to provide select_coupon animation.
 */
Drupal.ajax.prototype.commands.select_coupon = function (ajax, response, status) {
  var cart = $('.grid_voucher_order table');
  var imgToDrag = $(response.selector + ' .box_voucher');

  if (imgToDrag) {
    var imgClone = imgToDrag.clone()

    .offset({
      top: imgToDrag.offset().top,
      left: imgToDrag.offset().left
    })

    .css({
      'opacity': '0.5',
      'position': 'absolute',
      'height': '150px',
      'width': '150px',
      'z-index': '100'
    })

    .appendTo($('body')).animate({
      'top': cart.offset().top + 10,
      'left': cart.offset().left + 10,
      'width': 75,
      'height': 75
    }, {
      'duration': 1000,
      'easing': 'easeInOutExpo',
      'done': function() {
        $('.grid_voucher_order').effect('shake', {
          times: 2
        }, 200);
      }
    });

    imgClone.animate({
      'width': 0,
      'height': 0
    }, function() {
      $(this).detach();
    });
  }
}

})(jQuery);
