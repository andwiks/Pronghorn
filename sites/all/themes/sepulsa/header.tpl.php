<?php
/**
 * @file
 * header.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;

?>
<header id="header">
  <div class="container">
    <div class="header-inner">
      <div class="branding">
        <a href="<?php print $base_url; ?>">
          <img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/logo@2x.png" alt="" width="225">
        </a>
      </div>
      <nav id="nav">
        <ul class="header-top-nav">
          <li class="visible-mobile">
            <a href="#mobile-nav-wrapper" data-toggle="collapse"><i class="fa fa-bars has-circle"></i></a>
          </li>
        </ul>
        <ul id="main-nav" class="hidden-mobile">
          <li class="menu-item-has-children">
            <a href="<?php print $base_url; ?>">Home</a>
          </li>
          <?php if ($user->uid > 0) { ?>
          <li class="menu-item-has-children">
            <a href="#">Kupon</a>
          </li>
          <li class="menu-item-has-children">
            <a href="<?php print $base_url."/user/logout"; ?>">Logout</a>
          </li>
          <?php } else { ?>
          <li class="mini-cart menu-item-has-children">
            <a href="<?php print $base_url."/user"; ?>">Login</a>
            <?php if ($page['user_login']) { ?>
            <div class="sub-nav cart-content">
              <div style="text-align:center">
                <?php print render($page['user_login']); ?>
              </div>
            </div>
            <?php } ?>
          </li>
          <li class="mini-cart menu-item-has-children">
            <a href="<?php print $base_url."/user/register"; ?>">Sign Up</a>
            <?php if ($page['user_signup']) { ?>
            <div class="sub-nav cart-content">
              <div style="text-align:center">
                <?php print render($page['user_signup']); ?>
              </div>
            </div>
            <?php } ?>
          </li>
          <?php } ?>

        </ul>
      </nav>
      </div>
  </div>

  <div class="mobile-nav-wrapper collapse visible-mobile" id="mobile-nav-wrapper">
    <ul class="mobile-nav">
      <li class="menu-item">
        <a href="index.html">Home</a>
      </li>
      <li class="menu-item">
        <a href="#">Kupon</a>
      </li>
      <li class="menu-item">
        <a href="#">Login</a>
      </li>
      <li class="menu-item">
        <a href="#">Sign Up</a>
      </li>
    </ul>
  </div>
</header>


<div class="page-title-container">
  <div class="page-title"></div>
  <ul class="breadcrumbs">
    <li><a href="index.html">Home</a></li>
  </ul>
</div>