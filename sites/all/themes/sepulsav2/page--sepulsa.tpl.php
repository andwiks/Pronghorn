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

    <section id="content">
    <div class="container">
      <div id="main">
        <?php print $messages; ?>

        <div class="row">
          <div class="col-sm-6 col-md-8">
            <div class="tab-container full-width style1 box">
              <ul class="tabs clearfix">
                <li<?php if($active_tab == 'topup') print ' class="active"';?>><a href="#topup" data-toggle="tab">Isi Pulsa &amp; Bolt</a></li>
                <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
                  <li<?php if($active_tab == 'token_reload') print ' class="active"';?>><a href="#token-reload" data-toggle="tab">Listrik</a></li>
                <?php endif; ?>
				<?php if (module_exists('deposit_sepulsa') && user_is_logged_in()): ?>  
                  <li<?php if($active_tab == 'deposit_sepulsa') print ' class="active"';?>><a href="#deposit-sepulsa" data-toggle="tab">Deposit</a></li>
				<?php endif; ?>
              </ul>
              <div id="topup" class="tab-content<?php if($active_tab == 'topup') print ' in active';?>">
                <div class="tab-pane">
                  <?php print render($page['content']); ?>
                </div>
                <div style="color:red; margin-top:10px;">* Hanya bisa mengisi pulsa ke nomer dan nominal yang sama satu kali dalam sehari.</div>
              </div>
              <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
                <div id="token-reload" class="tab-content<?php if($active_tab == 'token_reload') print ' in active';?>">
                  <div class="tab-pane">
                    <?php print render($token_reload_form); ?>
                  </div>
                </div>
              <?php endif; ?>
			  <?php if (module_exists('deposit_sepulsa') && user_is_logged_in()): ?> 
                <div id="deposit-sepulsa" class="tab-content<?php if($active_tab == 'deposit_sepulsa') print ' in active';?>">
                  <div class="tab-pane">
                    <div style="padding:5px 5px 20px;"> Deposit akan menambah jumlah Sepulsa Kredit yang tersedia di akun kamu. Sepulsa Kredit dapat digunakan untuk melakukan transaksi.</div>
                    <?php print render($deposit_sepulsa_form); ?>
                  </div>
                </div>
              <?php endif; ?>
            </div>
          </div>

          <div class="col-sm-6 col-md-4">
            <?php print render($page['banner']); ?>
          </div>
        </div>
      </div>
    </div>
  </section>

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
                <img src="images/content/promo_banner.jpg " alt="promo card banner 1" />
                <a href="" class="bt_std">CALL TO ACTION BUTTON</a>
            </div>
            <div class="slide">
                <img src="images/content/promo_banner.jpg " alt="promo card banner 1" />
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

<section class="easy_step">
    <div class="wrapper after_clear">
        <h2>semudah apa isi pulsa dengan sepulsa?</h2>
        <h4>
            Kemudahan Sepulsa membuat kamu bisa isi pulsa kapan saja dan di mana saja.
            <span class="border"></span>
        </h4>
        <div class="box_step">
            <div class="image"><img src="<?php print $theme_path; ?>/images/content/easy_step-1.png" alt="step1 " /></div>
            <div class="label">1</div>
            <p>Masukkan nomor HP dan nominal pulsa yang kamu inginkan.</p>
        </div>
        <div class="box_step">
            <div class="image"><img src="<?php print $theme_path; ?>/images/content/easy_step-2.png" alt="step1 " /></div>
            <div class="label">2</div>
            <p>Kamu bebas memilih 3 voucher diskon belanja yang menarik sesukamu. Gratis!</p>
        </div>
        <div class="box_step">
            <div class="image"><img src="<?php print $theme_path; ?>/images/content/easy_step-3.png" alt="step1 " /></div>
            <div class="label">3</div>
            <p>Pilih metode pembayaran mudah yang kamu inginkan.</p>
        </div>
        <div class="box_step">
            <div class="image"><img src="<?php print $theme_path; ?>/images/content/easy_step-4.png" alt="step1 " /></div>
            <div class="label">4</div>
            <p>Pulsamu segera terisi 5-10 menit setelah pembayaran berhasil dan terverifikasi.Praktis!</p>
        </div>
        <div class="clear"></div>
        <a href="" class="bt_std">Beli pulsa sekarang</a>
    </div>
</section>

<section class="h_keuntungan">
    <div class="wrapper_2 after_clear">
        <h2>KEUNTUNGan bertansaksi di sepulsa</h2>
        <h4>4 fakta yang membuat Sepulsa aman, nyaman digunakan, dan terpercaya.<span class="border"></span></h4>


        <div class="box">
            <div class="image" style="background: url(<?php print $theme_path; ?>/images/content/keuntungan_img_1.png) no-repeat ">
            </div>
            <h5>Mudah di Mana Saja</h5>
            <p>Di manapun lokasi kamu berada, tak perlu mencari mesin ATM ataupun repot dengan mobile banking. Dengan Sepulsa, transaksi kamu jadi lebih mudah dan cepat.</p>
        </div>
        <div class="box">
            <div class="image" style="background: url(<?php print $theme_path; ?>/images/content/keuntungan_img_2.png) no-repeat ">
            </div>
            <h5>Praktis Kapan saja</h5>
            <p>Dengan Sepulsa, kamu bebas bertransaksi setiap saat kamu butuh. Pagi, siang, sore, hingga tengah malam, dalam segala situasi dan kondisi.</p>
        </div>
        <div class="box">
            <div class="image" style="background: url(<?php print $theme_path; ?>/images/content/keuntungan_img_3.png) no-repeat ">
            </div>
            <h5>voucher diskon</h5>
            <p>Kamu bisa memilih voucher diskon di berbagai macam e-commerce website di Indonesia. Bukan hanya satu, kamu bisa memilih sampai TIGA voucher diskon setiap transaksi.</p>
        </div>
        <div class="box">
            <div class="image" style="background: url(<?php print $theme_path; ?>/images/content/keuntungan_img_4.png) no-repeat ">
            </div>
            <h5>transaksi aman</h5>
            <p>Kami menggunakan 3D Secure untuk memastikan keamanan dari setiap transaksi dengan menambahkan autentikasi dari pemilik kartu ketika proses pembelian.</p>
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
