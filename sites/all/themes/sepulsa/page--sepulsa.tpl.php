<?php
/**
 * @file
 * page.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;

?>
<div id="page-wrapper">
  <?php include "header.tpl.php"; ?>
        
  <section id="content">
    <div class="container">
      <div id="main">
        <?php print $messages; ?>

        <div class="row">
          <div class="col-sm-6 col-md-8">
            <div class="tab-container full-width style1 box">
              <ul class="tabs clearfix">
                <li class="active"><a href="#tab1-1" data-toggle="tab">Isi Pulsa</a></li>
              </ul>
              <div id="tab1-1" class="tab-content in active">
                <div class="tab-pane">
                  <?php print render($page['content']); ?>
                </div>
              </div>
            </div>                        
          </div>
              
          <div class="col-sm-6 col-md-4">
            <div class="post-slider style2 owl-carousel box">
              <a href="http://placehold.it/770x415" class="soap-mfp-popup">
                <img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/banner1.jpg" alt="">
                <div class="slide-text">
                  <h4 class="slide-title">Judul iklan nol satu dua</h4>
                  <span class="meta-info">line kedua hey ho</span>
                </div>
              </a>
              <a href="http://placehold.it/770x415" class="soap-mfp-popup">
                <img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/banner2.jpg" alt="">
                <div class="slide-text">
                  <h4 class="slide-title">Hand-crafted Accuracy & Pixel Perfection</h4>
                  <span class="meta-info">In Fashion  .  12 Nov, 2014</span>
                </div>
              </a>
              <a href="http://placehold.it/770x415" class="soap-mfp-popup">
                <img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/banner3.jpg" alt="">
                <div class="slide-text">
                  <h4 class="slide-title">Hand-crafted Accuracy & Pixel Perfection</h4>
                  <span class="meta-info">In Fashion  .  12 Nov, 2014</span>
                </div>
              </a>
            </div>
          </div>
        </div>
      </div>
    </div>
  </section>
        
  <footer id="footer" class="style4">
    <div class="footer-bottom-area" style="border-top:solid 1px #ddd">
      <div class="container">
        <div class="copyright-area">
          <nav class="secondary-menu">
            <ul class="nav nav-pills">
              <li class="dropdown menu-item-has-children">
                <a href="index.html">Home</a>
              </li>
              <li class="dropdown menu-item-has-children">
                <a href="#">Tentang Kami</a>
              </li>
              <li class="dropdown menu-item-has-children">
                <a href="#">F.A.Q.</a>
              </li>
              <li class="dropdown menu-item-has-children">
                <a href="#">Syarat Penggunaan</a>
              </li>
              <li class="dropdown menu-item-has-children">
                <a href="#">Kontak Kami</a>
              </li>
          </ul>
          </nav>
          <div class="copyright">&copy; 2014 sepulsa</div>
        </div>
      </div>
    </div>
  </footer>
    
</div>