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
        
        
        <div class="row">
          <div class="col-sm-9">

            <?php print render($title_suffix); ?>
            <?php print $messages; ?>
            <?php print render($page['content']); ?>
            
          </div>
          
            
          <div class="col-sm-6 col-sm-3">
            <?php print render($page['right']); ?>
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