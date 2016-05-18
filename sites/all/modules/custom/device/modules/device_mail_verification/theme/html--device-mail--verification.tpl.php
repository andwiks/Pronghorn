<?php
/**
 * @file
 * html--device-mail--verification.tpl.php
 *
 * @author andreas@sepulsa.com
 */
?>
<!DOCTYPE html>
<!--[if IE 8]>          <html class="ie ie8"> <![endif]-->
<!--[if IE 9]>          <html class="ie ie9"> <![endif]-->
<!--[if gt IE 9]><!-->
<html xmlns="http://www.w3.org/1999/xhtml"
      xmlns:og="http://ogp.me/ns#"
      xmlns:fb="http://www.facebook.com/2008/fbml">
<!--<![endif]-->
<head>
  <?php print $head; ?>
  <link rel="dns-prefetch" href="//ajax.googleapis.com" />
  <link rel="dns-prefetch" href="//www.google-analytics.com" />
  <link rel="dns-prefetch" href="//chat.freshdesk.com" />
  <link rel="dns-prefetch" href="//cdn.mxpnl.com" />
  <title><?php print $head_title; ?></title>
  <meta name="author" content="PT. Sepulsa Teknologi Indonesia">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no"/>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?> blue"<?php print $attributes; ?>>
  <!--[if lt IE 7]>
      <p class="chromeframe">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> or <a href="http://www.google.com/chromeframe/?redirect=true">activate Google Chrome Frame</a> to improve your experience.</p>
  <![endif]-->
  <!-- header -->
	<?php print $page; ?>
  <!--Footer -->
  </body>
</html>