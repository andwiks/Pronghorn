<html>
<head>
</head>
<body>
	<section class="banner">
		<img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/content/banner_home.jpg" alt="banner home">
		<div class="caption">
			<img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/material/logo.png" alt="sepulsa">

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
