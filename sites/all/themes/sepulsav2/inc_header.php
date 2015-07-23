<body class="<?php print ($page_state == "home" || $page_state == "home_acc") ? 'home ' : ''; print $classes; ?>"<?php print $attributes; ?>>
    <!--[if lt IE 7]>
        <p class="chromeframe">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> or <a href="http://www.google.com/chromeframe/?redirect=true">activate Google Chrome Frame</a> to improve your experience.</p>
    <![endif]--> 

    <!-- header -->
    
    <script>
      function buynow() {
        document.getElementById('nomor-hp').focus();
      }
    </script>
    <header>
        <div class="wrapper">
            <a href="javascript:;" onclick="buynow()" class="belipulsa">Beli Pulsa</a>

            <div class="soc_link">
                <a href="" class="fb">facebook</a>
                <a href="" class="tw">twitter</a>
            </div>
            <div class="logo">
                <a href="index.php">
                    <img src="<?php print $theme_path; ?>/images/material/logo.png" alt="sepulsa" />
                </a>
            </div>
            <div class="right_link">

                <?php if ($page == "home_acc" || $page == "member_area") { ?>
                    <div class="box myacc">
                        <a href="" >
                            <span class="ico"></span>
                            My Acc
                        </a>
                        <div class="box_drop">
                            <div class="arr"></div>
                            <div class="box_credit after_clear">
                                <div class="left">SEPULSA CREDIT</div>
                                <div class="right">IDR 2.000.000</div>
                            </div>
                            <a href="informasi_akun.php">AKUN</a>
                            <a href="konfirmasi_pembayaran.php">KONFIRMASI PEMBAYARAN</a>
                            <a href="">LOG OUT</a>
                        </div>
                    </div>
                    <div class="nav_mobile">
                        <a href="" class="toggle">toogle</a>
                        <div class="box_drop">
                            <div class="box_credit after_clear">
                                <div class="left">SEPULSA CREDIT</div>
                                <div class="right">IDR 2.000.000</div>
                            </div>
                            <a href="informasi_akun.php" >Informasi Akun</a>
                            <a href="order_transaksi.php">Order Transaksi</a>
                            <a href="info_kartu.php">Info Kartu</a>
                            <a href="dompet_saya.php">Dompet Saya</a>
                            <a href="akun_voucher.php">Voucher Diskon</a>
                            <a href="konfirmasi_pembayaran.php">Konfirmasi Pembayaran</a>
                            <a href="">LOG OUT</a>
                            <div class="m_lang">
                                <a href="">English</a>
                            </div>
                        </div>
                    </div>
                <?php } else { ?>  
                    <div class="nav_mobile">
                        <a href="" class="toggle">toogle</a>
                        <div class="box_drop">
                            <div class="box_credit after_clear">
                                <div class="left">SEPULSA CREDIT</div>
                                <div class="right">IDR 2.000.000</div>
                            </div>
                            <a href="" class="login">LOGIN</a>
                            <div class="m_lang">
                                <a href="">English</a>
                            </div>
                        </div>
                    </div>
                    <div class="box login">
                        <a href="" >
                            <span class="ico"></span>
                            Log In
                        </a>

                    </div>
                    <?php
                }
                ?>
                <div class="box lang">
                    <a href="" >
                        <span class="ico"></span>
                        IN
                    </a>  
                    <div class="box_drop">
                        <div class="arr"></div>
                        <a href="">EN</a>
                        <a href="">IN</a>
                    </div>
                </div>
            </div>

        </div>
    </header>
    <!-- end of header --udah >
