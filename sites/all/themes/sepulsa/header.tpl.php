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
          <li class="menu-item-has-children">
            <a href="#">Kupon</a>
          </li>
          <li class="mini-cart menu-item-has-children">
            <a href="#">Login</a>
            <div class="sub-nav cart-content">
              <form>
                <div class="form-group">
                  <input type="text" class="input-text full-width" placeholder="Email Address">
                </div>
                <div class="form-group">
                  <input type="password" class="input-text full-width" placeholder="Password">
                </div>
                <div class="form-group" style="text-align:center">
                  <button type="submit" class="btn style1">Login</button>
                </div>
                <div class="form-group">
                  <div style="text-align:center">
                    <a href="#">lupa password</a>
                  </div>
                </div>
              </form>
            </div>
          </li>
          <li class="mini-cart menu-item-has-children">
            <a href="#">Sign Up</a>
            <div class="sub-nav cart-content">
              <form>
                <div class="form-group">
                  <input type="text" class="input-text full-width" placeholder="Email Address">
                </div>
                <div class="form-group" style="text-align:center">
                  <button type="submit" class="btn style1">Sign Up</button>
                </div>
              </form>
            </div>
          </li>

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