<html>
<head>
</head>
<body>
<section class="banner">
	<?php
	global $base_url;
	if (module_exists('cdn') && cdn_status_is_enabled()){
	    $theme_path = file_create_url('public://' . '/' . path_to_theme());
	}else{
	    $theme_path = $base_url . '/' . path_to_theme();
	}
	?>
	<img src="<?php print $theme_path; ?>/images/content/banner_home.jpg" alt="banner home">
	<div class="caption">
		<img src="<?php print $theme_path; ?>/images/material/logo.png" alt="sepulsa">

	<?php print $content; ?>

	</div>
</section>

<style>
	section.banner .caption {
		position: absolute;
		text-align: center;
		width: 100%;
		top: 20%;
		color: #FFF;
	}
</style>

</body>
</html>
