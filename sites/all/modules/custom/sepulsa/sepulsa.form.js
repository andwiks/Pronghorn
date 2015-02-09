/**
 * Autocomplete form.
 * @todo: check condition: We apologize, your phone number is not supported by our system.
 */

;(function($) {
  Drupal.behaviors.sepulsa = {
    attach: function (context, settings) {
       var operatorid = 0;
       $("#edit-phone", context).on('keyup blur', function(event) {
    	  $("#edit-card-type option").remove();
    	  phone = new RegExp("^0\\d{3}");
    	  number = $(this).val();
    	  prefix = phone.exec(number);
    	  
    	  if (prefix != null) {
    		  for (operator in settings.sepulsa) {
    			  if (settings.sepulsa[operator].prefix.indexOf(prefix[0]) != -1) {
    				  operatorid = operator;
    				  $("#edit-operator").val(settings.sepulsa[operator].operator);
    				  counter = 0;
    				  for (cardtype in settings.sepulsa[operator].cardtype) {
    					  counter++;
    					  $("#edit-card-type").append($("<option>", {"value":cardtype}).text(settings.sepulsa[operator].cardtype[cardtype]));
    					  if (counter == 1) {
    						  $("#edit-card-type").val(cardtype);
    				  	  }
    				  }
    				  if (counter > 1) {
    					  $("#sepulsa-autocomplete-card-type").show();
    				  }
    				  
    				  $("#edit-packet option").remove();
    				  for (packet in settings.sepulsa[operator].packet[$("#edit-card-type").val()]) {
    					  $("#edit-packet").append($("<option>", {"value":packet}).text(settings.sepulsa[operator].packet[$("#edit-card-type").val()][packet]));
    					  $("#edit-packet").val(packet );
    				  }
    				  if ($('#edit-packet option').size() > 0) {
	    				  $("#sepulsa-autocomplete-packets").show();
	    				  $("#sepulsa-autocomplete-operation").show();
    				  }
    				  break;
    			  }
    		  }
    	  }
    	  else {
    		  $("#edit-operator").val(0);
    		  $("#edit-card-type option:gt(0)").remove();
    		  $("#sepulsa-autocomplete-card-type").hide();
    		  $("#sepulsa-autocomplete-packets option:gt(0)").remove();
    		  $("#sepulsa-autocomplete-packets").hide();
    		  $("#sepulsa-autocomplete-operation").hide();
    	  }
      });
       
      $("#edit-card-type", context).on('change', function (event) {
    	  $("#edit-packet option").remove();
    	  
    	  for (packet in settings.sepulsa[operatorid].packet[$(this).val()]) {
			  $("#edit-packet").append($("<option>", {"value":packet}).text(settings.sepulsa[operatorid].packet[$(this).val()][packet]));
			  $("#edit-packet").val(packet );
		  }
      });
    }
  }
})(jQuery);
