<?php
/**
 * @file
 * header.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url, $user;

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
            <a href="<?php print $base_url."/user/".$user->uid; ?>"><?php print t('My Account'); ?></a>
          </li>
          <li class="menu-item-has-children">
            <a href="<?php print $base_url."/user/logout"; ?>"><?php print t('Logout'); ?></a>
          </li>
          <?php } else { ?>
          <li class="mini-cart menu-item-has-children">
            <a href="<?php print $base_url."/user"; ?>"><?php print t('Log In'); ?></a>
            <?php if ($page['user_login']) { ?>
            <div class="sub-nav cart-content">
              <div style="text-align:center">
                <?php print render($page['user_login']); ?>
              </div>
            </div>
            <?php } ?>
          </li>
          <li class="mini-cart menu-item-has-children">
            <a href="<?php print $base_url."/user/register"; ?>"><?php print t('Sign Up'); ?></a>
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
        <a href="<?php print $base_url; ?>">Home</a>
      </li>
      <?php if ($user->uid > 0) { ?>
        <li class="menu-item">
          <a href="<?php print $base_url."/user/".$user->uid; ?>"><?php print t('My Account'); ?></a>
        </li>
        <li class="menu-item">
          <a href="<?php print $base_url."/user/logout"; ?>"><?php print t('Logout'); ?></a>
        </li>
      <?php } else { ?>
        <li class="menu-item">
          <a href="<?php print $base_url."/user"; ?>"><?php print t('Log In'); ?></a>
        </li>
        <li class="menu-item">
          <a href="<?php print $base_url."/user/register"; ?>"><?php print t('Sign Up'); ?></a>
        </li>
      <?php } ?>
    </ul>
  </div>
</header>


<div class="page-title-container">
  <div class="page-title"></div>
</div>

<section class="follow-top">
  <div class="container">
    <div class="row">
      <div class="col-sm-12">
        <div class="follow-content">
          <h4 class="follow-label">
            Follow Sepulsa on:
          </h4>
          <ul class="follow-icon">
            <li><a href="https://www.facebook.com/sepulsa" alt="Facebook Sepulsa" target="_blank"><i class="fa fa-facebook-square"></i></a></li>
            <li><a href="https://twitter.com/sepulsa_id" alt="Twitter Sepulsa" target="_blank"><i class="fa fa-twitter-square"></i></a></li>
          </ul>
        </div>
      </div>
    </div>
  </div>
</section>