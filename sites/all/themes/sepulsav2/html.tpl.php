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
<?php include('inc_header.php'); ?>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
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
                    isi pulsa
                </a>
                <a href="" target="target_2">
                    <span class="ico bolt"></span>
                    isi bolt
                </a>
                <a href="" target="target_3">
                    <span class="ico listrik"></span>
                    bayar listrik
                </a>
            </div>

            <div class="content_tab">
                <div class="tab form style_1" id="target_1">
                    <form action="price.php" >
                        <div class="row">
                            <input type="text" value="" value-placeholder="masukan nomer handphone (eq : 08122344567)" id="nomor-hp">
                            <input type="button" class="bt toogle_step">
                        </div>
                        <div class="next_step">
                            <div class="row saldo">
                                <div class="left">
                                    XL
                                </div>
                                <div class="right">
                                    <select>
                                        <option>IDR   100.000</option>
                                    </select>
                                </div>
                            </div>
                            <div class="row bt_wrap">
                                <div class="left"> 
                                    <span class="note">* Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</span>
                                    <span class="note">* Hanya bisa mengisi pulsa ke nomer dan nominal yang sama satu kali dalam sehari.</span>
                                </div>
                                <div class="right">
                                    <input type="submit" value="PROSES" class="bt_std" />
                                </div>                        
                            </div>
                        </div>
                    </form> 
                </div>
                <div class="tab form style_1" id="target_2">
                    <form action="" >
                        <input type="text" value="" value-placeholder="masukan nomer handphone (eq : 08122344567)">
                        <input type="submit" class="bt">
                    </form> 
                    <span class="note">* Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</span>
                </div>
                <div class="tab form style_1" id="target_3">
                    <form action="" >
                        <input type="text" value="" value-placeholder="masukan nomer handphone (eq : 08122344567)">
                        <input type="submit" class="bt">
                    </form> 
                    <span class="note">* Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</span>
                </div>
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
<section class="newsletter">
    <div class="wrapper after_clear">
        <div class="left">
            <h3>info updates terbaru</h3>
            <h5>dapatkan ON GOING VOUCHER dan discount updates</h5>
        </div>
        <div class="right form style_1 ">            
            <div>
                <input type="text" value-placeholder="masukan alamat e-mail kamu disini" />
                <input type="submit" class="bt" />
            </div>
        </div>
    </div>
    <div>

    </div>
</section>

<!-- end of middle -->
<?php
include('inc_footer.php');
include('inc_popup.php');
?>
