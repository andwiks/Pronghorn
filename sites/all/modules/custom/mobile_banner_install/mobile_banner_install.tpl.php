<script>
  jQuery(document).ready(function(){
	jQuery('#close_banner').click(function() {
	  jQuery('#mobile_banner').hide();
	});
  });
</script>
<div id="mobile_banner" style="position: absolute; top: -1px; width: 100%; background-color: #eee; z-index: 101; text-align: center; padding-bottom: 10px;">
  <table style="text-align: center; width: 100%">
	<tr>
	  <td width="10%"><img width="50px" src="<?php if (isset($content['image_path'])) print $content['image_path'] ?>" /></td>
	  <td width="30%" style="text-align: left; padding-left: 10px;">
		<?php if (isset($content['apps_desc'])) print $content['apps_desc']  ?>
	  </td>
	  <td width="50%"><a href="<?php if (isset($content['apps_path'])) print $content['apps_path'] ?>"><button class="btn">View</button></a>
		  <button id="close_banner" class="btn">Close</button></td>
	</tr>
  </table>
 </div>