<style>
  .myButton {
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #7892c2), color-stop(1, #476e9e));
	background:-moz-linear-gradient(top, #7892c2 5%, #476e9e 100%);
	background:-webkit-linear-gradient(top, #7892c2 5%, #476e9e 100%);
	background:-o-linear-gradient(top, #7892c2 5%, #476e9e 100%);
	background:-ms-linear-gradient(top, #7892c2 5%, #476e9e 100%);
	background:linear-gradient(to bottom, #7892c2 5%, #476e9e 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#7892c2', endColorstr='#476e9e',GradientType=0);
	background-color:#7892c2;
	-moz-border-radius:4px;
	-webkit-border-radius:4px;
	border-radius:4px;
	border:1px solid #4e6096;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:12px;
	padding:8px 12px;
	text-decoration:none;
	text-shadow:1px 1px 0px #283966;
  }
  .myButton:hover {
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #476e9e), color-stop(1, #7892c2));
	background:-moz-linear-gradient(top, #476e9e 5%, #7892c2 100%);
	background:-webkit-linear-gradient(top, #476e9e 5%, #7892c2 100%);
	background:-o-linear-gradient(top, #476e9e 5%, #7892c2 100%);
	background:-ms-linear-gradient(top, #476e9e 5%, #7892c2 100%);
	background:linear-gradient(to bottom, #476e9e 5%, #7892c2 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#476e9e', endColorstr='#7892c2',GradientType=0);
	background-color:#476e9e;
  }
  .myButton:active {
	position:relative;
	top:1px;
  }
</style>
<script>
  jQuery(document).ready(function(){
	jQuery('#close_banner').click(function() {
	  jQuery('#mobile_banner').hide();
	});
  });
</script>
<div id="mobile_banner" style="position: absolute; top: -1px; width: 100%; background-color: #393939; z-index: 101; text-align: center;">
  <table style="text-align: center; width: 100%">
	<tr>
	  <td width="20%"><img style="padding: 10px;" width="90px" src="<?php if (isset($content['image_path'])) print $content['image_path'] ?>" /></td>
	  <td width="45%" style="text-align: left; padding-left: 10px; color: #fff;">
		<?php if (isset($content['apps_desc'])) print $content['apps_desc']  ?>
	  </td>
	  <td width="35%"><a href="<?php if (isset($content['apps_path'])) print $content['apps_path'] ?>"><button class="myButton">View</button></a>
		  <button id="close_banner" class="myButton">Close</button></td>
	</tr>
  </table>
 </div>
