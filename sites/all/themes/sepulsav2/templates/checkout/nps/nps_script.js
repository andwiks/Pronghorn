(function($) { 
  $(function() {
    $(":radio").labelauty();
    $(":radio").change(function(){
      var $nps = $(this).val();
      var $radios = $('#edit-submitted-nps');
      if($radios.is(':checked') === false) {
        $("#edit-submitted-nps-"+$nps).prop("checked", true)
      }
    });
    $("button[name='nps-submit']").click(function(){
      $('.webform-client-form').submit();
    })
  });
})(jQuery);