<footer id="footer" class="style4">
  <div class="footer-bottom-area">
    <div class="container">
      <div class="row">
        <div class="col-sm-6">
          <nav class="secondary-menu">
            <ul class="nav nav-pills">
              <li class="dropdown menu-item-has-children">
                <?php print l('Home', '<front>'); ?>
              </li>
              <li class="dropdown menu-item-has-children">
                <?php print l('Tentang Kami', 'about'); ?>
              </li>
              <li class="dropdown menu-item-has-children">
                <?php print l('F.A.Q.', 'faq'); ?>
              </li>
              <li class="dropdown menu-item-has-children">
                <?php print l('Syarat Penggunaan', 'ga-terms-conditions'); ?>
              </li>
              <li class="dropdown menu-item-has-children">
                <?php print l('Blog', 'blog'); ?>
              </li>
            </ul>       
          </nav>
          <div class="follow-content_footer">
            <h4 class="follow-footer-label">Follow Sepulsa on:</h4>
            <ul class="follow-footer-icon">
              <li><a href="https://www.facebook.com/sepulsa" alt="Facebook Sepulsa" target="_blank"><i class="fa fa-facebook-square"></i></a></li>
              <li><a href="https://twitter.com/sepulsa_id" alt="Twitter Sepulsa" target="_blank"><i class="fa fa-twitter-square"></i></a></li>
            </ul>
          </div>
        </div>
        <div class="col-sm-6">
          <div class="keamanan">
            <p>Keamanan Berbelanja:</p>
            <img src="<?php print url(path_to_theme() . '/images/footer/secure-visa.jpg'); ?>" style="min-height:25px; max-height:40px">
            <img src="<?php print url(path_to_theme() . '/images/footer/secure-mc.jpg'); ?>" style="min-height:25px; max-height:40px">
            <a href="https://sealinfo.thawte.com/thawtesplash?form_file=fdf/thawtesplash.fdf&amp;dn=WWW.SEPULSA.COM&amp;lang=en" tabindex="-1" onmousedown="return v_mDown(event);" target="THAWTE_Splash">
              <img name="seal" src="<?php print url(path_to_theme() . '/images/footer/secure-thawte.jpg'); ?>"  oncontextmenu="return false;" alt="Click to Verify - This site has chosen a thawte SSL Certificate to improve Web site security" style="min-height:25px; max-height:40px">
            </a>
          </div>
          <div class="pembayaran">
            <p>Metode Pembayaran:</p>
            <img src="<?php print url(path_to_theme() . '/images/footer/payment-mc.jpg'); ?>" style="min-height:25px; max-height:40px">
            <img src="<?php print url(path_to_theme() . '/images/footer/payment-visa.jpg'); ?>" style="min-height:25px; max-height:40px">
          </div>
        </div>
        <div class="col-sm-12">
          <div class="copyright">
            <div class="c-sepulsa">&copy; <?php print date('Y'); ?> sepulsa</div>
          </div>
        </div>
      </div>
    </div>
  </div>
</footer>
