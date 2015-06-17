<script>
  jQuery(document).ready(function(){
	jQuery('#close_banner').click(function() {
	  jQuery('#mobile_banner').hide();
	});
  });
</script>
<div id="mobile_banner" style="position: absolute; top: -1px; width: 100%; background-color: #eee; z-index: 101; text-align: center;">
  <table style="text-align: center; width: 100%">
	<tr>
	  <td width="10%"><img width="50px" src="<?php print $content['image_path'] ?>" /></td>
	  <td width="50%" style="text-align: left; padding-left: 10px;">
		Sepulsa.com<br />
		PT. Sepulsa Technology<br />
		FREE - In Google Play</td>
	  <td width="40%"><a href="<?php print $content['apps_path'] ?>"><button class="">View</button></a>
		  <button id="close_banner" class="">Close</button></td>
	</tr>
  </table>
 </div>