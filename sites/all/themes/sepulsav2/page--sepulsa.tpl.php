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
  <!-- middle -->
<section class="banner">
    <img src="<?php print $theme_path; ?>/images/content/banner_home.jpg" alt="banner home" />
    <div class="caption">
        <h2>Praktis Isi Pulsa. Gratis Diskon Belanja.</h2>
        <p>Kini isi ulang pulsa lebih dari sekedar mudah, tapi menguntungkan!</p>
        <a href="">Gimana Caranya?</a>
    </div>
</section>
<section class="h_middle_top">
  <div class="wrapper">
    <div class="home_tab">
  <div class="nav_tab after_clear"> 
    <a href="" class="active" target="target_1">
        <span class="ico pulsa"></span>
        isi pulsa & bolt
    </a>
    <a href="" target="target_2">
        <span class="ico listrik"></span>
        bayar listrik
    </a>
    <?php if (module_exists('deposit_sepulsa') && user_is_logged_in()): ?> 
    <a href="" target="target_3">
        <span class="ico bolt"></span>
        deposit
    </a>
    <?php endif; ?>
  </div>

  <div class="content_tab">
    <div class="tab form style_1" id="target_1">
      <?php print render($page['content']); ?>
    </div>
    <div class="tab form style_1" id="target_2">
      <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
      <?php print render($token_reload_form); ?>
      <?php endif; ?>
      <span class="note">* Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</span>
    </div>
    <?php if (module_exists('deposit_sepulsa') && user_is_logged_in()): ?> 
    <div class="tab form style_1" id="target_3">
      <div style="padding:5px 5px 20px;"> Deposit akan menambah jumlah Sepulsa Kredit yang tersedia di akun kamu. Sepulsa Kredit dapat digunakan untuk melakukan transaksi.</div>
      <?php print render($deposit_sepulsa_form); ?>
      <span class="note">* Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</span>
    </div>
    <?php endif; ?>
  </div>

</div>

        <div class="voucher_vendor">
            <h3><span class="ico"></span>voucher VENDOR :</h3>
            <div class="slider">
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_1.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_2.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_3.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_4.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_5.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_6.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_1.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_2.png" alt="vendor 1" /></div>
                <div class="slide"><img src="<?php print $theme_path; ?>/images/content/voucher_vendor_3.png" alt="vendor 1" /></div>
            </div>
        </div>

    </div>
</section>

<section class="promo_banner">
    <div class="wrapper">
        <div class="slider">
            <div class="slide">
                <img src="<?php print $theme_path; ?>/images/content/promo_banner.jpg " alt="promo card banner 1" />
                <a href="" class="bt_std">CALL TO ACTION BUTTON</a>
            </div>
            <div class="slide">
                <img src="<?php print $theme_path; ?>/images/content/promo_banner.jpg " alt="promo card banner 1" />
                <a href="" class="bt_std">CALL TO ACTION BUTTON</a>
            </div>
        </div>
    </div>
</section>

<section class="gratis_voucher after_clear">
    <div class="wrapper after_clear">
        <div class="desc right">
            <h2>
                Satu kali isi ulang pulsa,<br/>
                gratis tiga voucher belanja.
            </h2>
            <h4>
                Kini isi ulang pulsa lebih dari sekedar mudah, tapi menguntungkan!
                <span class="border"></span>
            </h4>
            <p>
                Cuma di Sepulsa, isi ulang pulsa lebih dari sekedar kemudahan dan kecepatan, tapi juga menguntungkan. Karena tiap kali kamu melakukan isi ulang pulsa, kamu langsung mendapatkan voucher diskon di berbagai macam online shop terkemuka di Indonesia. Tidak hanya satu, tapi kamu bebas memilih tiga voucher diskon sekaligus.
                <br/><br/>
                Terus isi ulang pulsa kamu di sini dan nantikan terus kejutan lain yang akan kami berikan untuk kamu. Di Sepulsa, kini isi ulang pulsa menjadi menyenangkan.
            </p>
            <p>
                <b>Layanan Sepulsa bisa dinikmati via PC desktop, mobile web, android, dan iOS Anda.
                    Unduh aplikasinya di Sini:</b>
            </p>

            <a href=""><img src="<?php print $theme_path; ?>/images/content/apple_store.png" alt="app store"/></a>
            <a href=""><img src="<?php print $theme_path; ?>/images/content/google_store.png" alt="google play"/></a>
        </div>
    </div>
</section>





<!-- end of middle -->

  <section id="front_first" class="front_first">
    <?php print render($page['front_first']); ?>
  </section>

  <section id="front_second" class="front_second">
    <?php print render($page['front_second']); ?>
  </section>

  <section id="front_third" class="front_third">
    <?php print render($page['front_third']); ?>
  </section>

  <section id="front_fourth" class="front_fourth">
    <?php print render($page['front_fourth']); ?>
  </section>
