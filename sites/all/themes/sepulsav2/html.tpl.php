<?php
/**
 * @file
 * html.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July, 23th 2015.
 */
global $base_url;
$theme_path = $base_url . '/' . path_to_theme();
$page_state = "home";
?>
<!DOCTYPE html>
<!--[if IE 8]>          <html class="ie ie8"> <![endif]-->
<!--[if IE 9]>          <html class="ie ie9"> <![endif]-->
<!--[if gt IE 9]><!-->  <html> <!--<![endif]-->
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>

  <meta charset="utf-8">
  <meta name="author" content="PT. Sepulsa Teknologi Indonesia">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no"/>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <!--[if lt IE 7]>
      <p class="chromeframe">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> or <a href="http://www.google.com/chromeframe/?redirect=true">activate Google Chrome Frame</a> to improve your experience.</p>
  <![endif]-->

  <!-- header -->

  <script>
    function buynow() {
      document.getElementById('nomor-hp').focus();
    }
  </script>
<?php include('inc_header.php'); ?>
<?php print $page_top; ?>
<?php print $page; ?>
<?php print $page_bottom; ?>
  <!--Footer -->
    <footer>
        <div class="wrapper after_clear">
            <div class="left">
              <?php print render($footer_sub_left); ?>

            </div>
            <div class="right">
              <?php print render($footer_sub_right); ?>

            </div>
        </div>
        <div class="bottom">
          <?php print render($footer_second); ?>

        </div>
    </footer>
  <!--end of Footer -->
  </body>
</html>
<?php include('inc_popup.php'); ?>
?>
