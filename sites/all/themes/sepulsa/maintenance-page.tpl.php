<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 *
 * @ingroup themeable
 */

global $base_url;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
    
  <link rel="shortcut icon" href="<?php print $base_url . '/' . path_to_theme(); ?>/images/favicon.png" />

  <!-- Meta Tags -->
  <meta charset="utf-8">
  <meta name="description" content="Miracle | Responsive Multi-Purpose HTML5 Template">
  <meta name="author" content="SoapTheme">

  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <!-- Theme Styles -->
  <?php print $styles; ?>
  <link href='https://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700' rel='stylesheet' type='text/css'>
  <link href='https://fonts.googleapis.com/css?family=Open+Sans+Condensed:300,300italic' rel='stylesheet' type='text/css'>
  <link href='https://fonts.googleapis.com/css?family=Playfair+Display:400,400italic' rel='stylesheet' type='text/css'>
  <link href='https://fonts.googleapis.com/css?family=Dosis:400,300,500,600,700' rel='stylesheet' type='text/css'>

  <!-- CSS for IE -->
  <!--[if lte IE 9]>
      <link rel="stylesheet" type="text/css" href="<?php print $base_url . '/' . path_to_theme(); ?>/css/ie.css" />
  <![endif]-->


  <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!--[if lt IE 9]>
    <script type='text/javascript' src="https://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
    <script type='text/javascript' src="https://cdnjs.cloudflare.com/ajax/libs/respond.js/1.4.2/respond.js"></script>
  <![endif]-->

  <!-- Javascript -->
  <?php print $scripts; ?>
</head>
<body class="coming-soon-page no-sticky-menu">
  <div id="page-wrapper">
    <section id="content">
      <div class="container">
        <div id="main">
          <div style="margin:40px auto 0; width:90%"><img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/logo.png"></div>
          <?php print $content; ?>
        </div>
      </div>
    </section>
  </div>

  <!-- Javascript -->
  <script type="text/javascript">
    function cacluateLaunchTime() {
      var launchDateStr = "2015/02/21 21:00:00"; // timezone must be UTC + 0
      var launchDate = new Date(launchDateStr);
      launchDate.setTime( launchDate.getTime() - launchDate.getTimezoneOffset()*60*1000 );

      var currentDate = new Date();
      var diff = new Date(launchDate.getTime() - currentDate.getTime());

      if (diff > 0) {
        var milliseconds, seconds, minutes, hours, days;
        diff = Math.abs(diff);
        diff = (diff - (milliseconds = diff % 1000)) / 1000;
        diff = (diff - (seconds = diff % 60)) / 60;
        diff = (diff - (minutes = diff % 60)) / 60;
        days = (diff - (hours = diff % 24)) / 24;
        sjq(".clock .days").html((days + "").lpad("0", 2));
        sjq(".clock .hours").html((hours + "").lpad("0", 2));
        sjq(".clock .minutes").html((minutes + "").lpad("0", 2));
        sjq(".clock .seconds").html((seconds + "").lpad("0", 2));
      }
    }
    cacluateLaunchTime();
    var calcLaunchTimeInterval = setInterval(cacluateLaunchTime, 1000);
  </script>

</body>
</html>
