<script>
  jQuery(document).ready(function(){
	jQuery('#close_banner').click(function() {
	  jQuery('#mobile_banner').hide();
	});
  });
</script>
<div id="mobile_banner" style="position: absolute; top: -1px; width: 100%; background-color: #eee; z-index: 101; text-align: center;">
  <table style="text-align: center; margin-left: 30%; width: 50%">
	<tr>
	  <td width="50px"><img width="50px" src="<?php print $content['image_path'] ?>" /></td>
	  <td width="30%">
		Sepulsa.com<br />
		PT. Sepulsa Technology<br />
		FREE - In Google Play</td>
	  <td><a href="<?php print $content['apps_path'] ?>"><button class="btn style1">View</button></a>
		  <button id="close_banner" class="btn style1">Close</button></td>
	</tr>
  </table>
 </div>