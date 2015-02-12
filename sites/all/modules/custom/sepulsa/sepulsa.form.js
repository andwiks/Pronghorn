/**
 * Autocomplete form.
 * @todo: check condition: We apologize, your phone number is not supported by our system.
 */

;(function($) {
  Drupal.behaviors.sepulsa = {
    attach: function (context, settings) {
      var operatorid = 0;
      function sepulsa_form_inactive() {
    	$("#edit-operator").val(0);
    	$("#edit-card-type option").remove();
        $("#sepulsa-autocomplete-card-type").hide();
        $("#edit-packet option").remove();
        $("#sepulsa-autocomplete-packets").hide();
        $("input[name=op]").toggleClass("inactive", true);
      }
      function sepulsa_form_packet(oid, pid) {
    	$("#edit-packet option").remove();
        for (packet in settings.sepulsa[oid].packet[pid]) {
          $("#edit-packet").append($("<option>", {"value":packet}).text(settings.sepulsa[oid].packet[pid][packet])).val(packet);
        }
      }
      
      $(document).ready(function() {
    	$("#edit-phone").trigger("blur").focus().val($("#edit-phone").val());
      });
      
      $("#edit-phone", context).on('keyup blur', function(event) {
        phone = new RegExp("^0\\d{3}");
        number = $(this).val();
        prefix = phone.exec(number);
          
        if (prefix != null) {
          for (operator in settings.sepulsa) {
            if (settings.sepulsa[operator].prefix.indexOf(prefix[0]) != -1) {
              operatorid = operator;
              if ($("#edit-operator").val() != settings.sepulsa[operator].operator) {
            	$("#edit-operator").val(settings.sepulsa[operator].operator);
              }
              $("#edit-card-type option").remove();
              counter = 0;
              for (cardtype in settings.sepulsa[operator].cardtype) {
                counter++;
                $("#edit-card-type").append($("<option>", {"value":cardtype}).text(settings.sepulsa[operator].cardtype[cardtype]));
                if (counter == 1) {
                  $("#edit-card-type").val(cardtype);
                }
              }
              $("#sepulsa-autocomplete-card-type").toggle(Boolean(counter > 1));
              sepulsa_form_packet(operator, $("#edit-card-type").val());
              if ($('#edit-packet option').size() > 0) {
                $("#sepulsa-autocomplete-packets").show();
                $("input[name=op]").toggleClass("inactive", false);
              }
              return;
            }
          }
        }
        sepulsa_form_inactive();
      });
       
      $("#edit-card-type", context).on('change', function (event) {
        if (operatorid > 0) {
          sepulsa_form_packet(operatorid, $(this).val());
        }
        else {
          sepulsa_form_inactive();
        }
      });
    }
  }
})(jQuery);
