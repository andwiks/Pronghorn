/**
 * Autocomplete form.
 */

;(function($) {
  Drupal.behaviors.sepulsa = {
    attach: function (context, settings) {
      var operatorid = 0;
      function sepulsa_form_inactive() {
        $("#edit-operator").val(0);
        $("#edit-card-type option").remove();
        $(".topup-notes").hide();
        $("#edit-add-cart").hide();
        $("#edit-add-charge").hide();   
        $("#edit-operator").hide();
        $("#sepulsa-autocomplete-card-type").hide();
        $("#edit-packet option").remove();
        $("#sepulsa-autocomplete-packets").hide();
        $("form#sepulsa-phone-form input[name=op]").toggleClass("inactive", true).attr("disabled", "disabled");
        $("#operator-sepulsa").css("display", "none");
      }
      
      function sepulsa_form_packet(oid, pid, ps) {
        $("#edit-packet option").remove();
        price = 0;
        select = false;
        sid = 0;
        for (p in settings.sepulsa[oid].packet[pid]) {
          $("#edit-packet").append($("<option>", {"value":settings.sepulsa[oid].packet[pid][p].id}).text(settings.sepulsa[oid].packet[pid][p].title));
          if (parseInt(settings.sepulsa[oid].packet[pid][p].price) > price) {
            price = parseInt(settings.sepulsa[oid].packet[pid][p].price);
            sid = settings.sepulsa[oid].packet[pid][p].id;
          }
          if (ps != null && ps == settings.sepulsa[oid].packet[pid][p].id) {
            select = true;
          }
        }
        $("#edit-packet").val(select ? ps : sid);
      }
      
      function sepulsa_check_number_prefix() {
        phone = new RegExp("\\d{4}");
        if ($("#edit-phone").val() == null || $("#edit-phone").val() == "") {
            number = $("#edit-existing-phone").val();
        }
        else {
            number = $("#edit-phone").val();
        }
        prefix = phone.exec(number);
          
        if (prefix != null) {
          for (operator in settings.sepulsa) {
            if (settings.sepulsa[operator].prefix.indexOf(prefix[0]) != -1) {
              operatorid = operator;
              if ($("#edit-operator").val() != settings.sepulsa[operator].operator) {
                $("#edit-operator").val(settings.sepulsa[operator].operator);
                $("#operator-sepulsa").css("display", "inline");
                $("#edit-operator").css("display", "inline");
              }
              counter = 0;
              selected = $("#edit-card-type").val();
              $("#edit-card-type option").remove();
              for (cardtype in settings.sepulsa[operator].cardtype) {
                counter++;
                $("#edit-card-type").append($("<option>", {"value":cardtype}).text(settings.sepulsa[operator].cardtype[cardtype]));
                if (counter == 1) {
                  $("#edit-card-type").val(cardtype);
                }
              }
              if (selected != null && settings.sepulsa[operator].cardtype[selected] != undefined) {
                $("#edit-card-type").val(selected);
              }
              $("#sepulsa-autocomplete-card-type").toggle(Boolean(counter > 1));
              sepulsa_form_packet(operator, $("#edit-card-type").val(), $("#edit-packet").val());
              if ($("#edit-packet option").size() > 0) {
                $(".topup-notes").show();
                $("#edit-add-cart").show();
                $("#edit-add-charge").show();
                $("#edit-operator").show();
                $("#sepulsa-autocomplete-packets").show();
                $("form#sepulsa-phone-form input[name=op]").toggleClass("inactive", false).removeAttr("disabled");
              }
              return;
            }
          }
        }
        sepulsa_form_inactive(); 
      }
      
      $(document).ready(function() {
        $("form#sepulsa-phone-form input[name=op]").attr("disabled", "disabled");
        $("#edit-phone").trigger("blur").focus().val($("#edit-phone").val());
        if (!($("#edit-existing-phone").val() == null || $("#edit-existing-phone").val() == "" || $("#edit-existing-phone").val() == "0")) {
            existing_number = $("#edit-existing-phone").val();
            $("#phone-sepulsa").css("display", "inline");
            $("#edit-phone").css("display", "none");
            $("#edit-phone").val(existing_number);
            $("#operator-sepulsa").css("display", "inline");
            sepulsa_check_number_prefix();
        }
      });
      
      $("#edit-phone", context).on("keyup blur", function(event) {
        //phone = new RegExp("^0\\d{3}");
        sepulsa_check_number_prefix();
      });
       
      $("#edit-card-type", context).on("change", function (event) {
        if (operatorid > 0) {
          sepulsa_form_packet(operatorid, $(this).val(), $("#edit-packet").val());
        }
        else {
          sepulsa_form_inactive();
        }
      });
      
      $("#edit-existing-phone", context).on("change", function (event) {
        if ($("#edit-existing-phone").val() == "0") {
            $("#edit-existing-phone").css("display", "none");
            $("#edit-phone").css("display", "inline");
            $("#phone-sepulsa").css("display", "inline");
            $("#edit-operator").css("display", "none");
            $("#edit-phone").val('');
            sepulsa_check_number_prefix();
        } else {
            existing_number = $("#edit-existing-phone").val();
            $("#phone-sepulsa").css("display", "inline");
            $("#edit-phone").css("display", "none");
            $("#edit-phone").val(existing_number);
            sepulsa_check_number_prefix();
        }
      });
    }
  }
})(jQuery);
