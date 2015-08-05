<?php
/**
 * @file
 * header.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url, $user;

?>
    <header>
        <div class="wrapper">
            <a href="javascript:;" onclick="buynow()" class="belipulsa">Beli Pulsa</a>

            <div class="soc_link">
                <a href="https://www.facebook.com/sepulsa" class="fb">facebook</a>
                <a href="https://twitter.com/sepulsa_id" class="tw">twitter</a>
            </div>
            <div class="logo">
                <a href="index.php">
                    <img src="<?php print $theme_path; ?>/images/material/logo.png" alt="sepulsa" />
                </a>
            </div>
            <div class="right_link">

                <?php if ($user->uid > 0) { ?>
                    <div class="box myacc">
                        <a href="" >
                            <span class="ico"></span>
                            My Acc
                        </a>
                        <div class="box_drop">
                            <div class="arr"></div>
                            <!--<div class="box_credit after_clear">
                                <div class="left">SEPULSA CREDIT</div>
                                <div class="right">IDR 2.000.000</div>
                            </div>-->
                            <a href="<?php print $base_url."/user/".$user->uid; ?>">AKUN</a>
                            <a href="<?php print $base_url."/konfirmasi"; ?>">KONFIRMASI PEMBAYARAN</a>
                            <a href="<?php print $base_url."/user/logout"; ?>">LOG OUT</a>
                        </div>
                    </div>
                    <div class="nav_mobile">
                        <a href="" class="toggle">toogle</a>
                        <div class="box_drop">
                            <!--<div class="box_credit after_clear">
                                <div class="left">SEPULSA CREDIT</div>
                                <div class="right">IDR 2.000.000</div>
                            </div>-->
                            <a href="<?php print $base_url."/user/".$user->uid; ?>" >Informasi Akun</a>
                            <a href="order_transaksi.php">Order Transaksi</a>
                            <a href="info_kartu.php">Info Kartu</a>
                            <a href="dompet_saya.php">Dompet Saya</a>
                            <a href="akun_voucher.php">Voucher Diskon</a>
                            <a href="<?php print $base_url."/konfirmasi"; ?>">Konfirmasi Pembayaran</a>
                            <a href="<?php print $base_url."/user/logout"; ?>">LOG OUT</a>
                            <div class="m_lang">
                                <a href="">English</a>
                            </div>
                        </div>
                    </div>
                <?php } else { ?>
                    <div class="nav_mobile">
                        <a href="" class="toggle">toogle</a>
                        <div class="box_drop">
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
