<?php
/**
 * @file
 * html.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;
?>
<!DOCTYPE html>
<!--[if IE 8]>          <html class="ie ie8"> <![endif]-->
<!--[if IE 9]>          <html class="ie ie9"> <![endif]-->
<!--[if gt IE 9]><!-->  <html> <!--<![endif]-->
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>

  <meta charset="utf-8">
  <meta name="description" content="PT. Sepulsa Teknologi Indonesia | Isi Ulang Pulsa Gratis">
  <meta name="author" content="PT. Sepulsa Teknologi Indonesia">

  <meta name="viewport" content="width=device-width, initial-scale=1.0">

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

  <?php print $scripts; ?>
  <!-- Facebook Conversion Code for Checkout - Successful transaction -->
  <script>(function() {
    var _fbq = window._fbq || (window._fbq = []);
    if (!_fbq.loaded) {
      var fbds = document.createElement('script');
      fbds.async = true;
      fbds.src = '//connect.facebook.net/en_US/fbds.js';
      var s = document.getElementsByTagName('script')[0];
      s.parentNode.insertBefore(fbds, s);
      _fbq.loaded = true;
    }
  })();
  window._fbq = window._fbq || [];
  window._fbq.push(['track', '6026439927327', {'value':'0.00','currency':'IDR'}]);
  </script>
  <noscript><img height="1" width="1" alt="" style="display:none" src="https://www.facebook.com/tr?ev=6026439927327&amp;cd[value]=0.00&amp;cd[currency]=IDR&amp;noscript=1" /></noscript>
</head>

<body class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  <!-- Thawte SSL Seal -->
  <div id="thawteseal" style="display:none;text-align:center;" title="Click to Verify - This site chose Thawte SSL for secure e-commerce and confidential communications.">
    <div><script type="text/javascript" src="https://seal.thawte.com/getthawteseal?host_name=www.sepulsa.com&amp;size=S&amp;lang=en"></script></div>
  </div>
</body>
</html>
