/**
 * Autocomplete form.
 */

;(function($) {
  Drupal.behaviors.sepulsa = {
    attach: function (context, settings) {
       $("#edit-phone", context).on('keyup blur', function(event) {
    	  phone = new RegExp("^0\\d{3}");
    	  number = $(this).val();
    	  prefix = phone.exec(number);
    	  if (prefix != null) {
    		  for (operator in settings.sepulsa.prefix) {
    			  if (settings.sepulsa.prefix[operator].indexOf(prefix[0]) != -1) {
    				  for (cardtype in settings.sepulsa.cardtype[operator]) {
    					  $("#edit-card-type").val(cardtype);
    					  break;
    				  }
    				  $("#edit-operator").val(operator);
    				  break;
    			  }
    		  }
    	  }
    	  else {
    		  $("#edit-operator, #edit-card-type").val(0);
    		  $('#edit-card-type option:gt(0)').remove();
    	  }
      });
    }
  }
})(jQuery);
