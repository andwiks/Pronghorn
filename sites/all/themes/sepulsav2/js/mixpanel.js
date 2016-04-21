Drupal.behaviors.myBehavior = {
    attach: function (context, settings) {
        jQuery('input[id|=edit-commerce-coupon-coupon-code]').bind('mousedown', function() {
            alert('OK');
        });
    }
};
