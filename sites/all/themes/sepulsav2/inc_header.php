<?php
/**
 * @file
 * header.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url, $user, $language;
?>
    <header>
        <div class="wrapper">
            <div class="soc_link">
                <a href="https://www.facebook.com/sepulsa" class="fb">facebook</a>
                <a href="https://twitter.com/sepulsa_id" class="tw">twitter</a>
            </div>
            <div class="logo">
                <a href="<?php print url(); ?>">
                    <img src="<?php print $theme_path; ?>/images/material/logo.png" alt="sepulsa" />
                </a>
            </div>
            <div class="right_link">

                <?php if ($user->uid > 0) { ?>
                    <div class="box myacc">
                        <a href="" >
                            <span class="ico"></span>
                            <?php print t('Akunku'); ?>
                        </a>
                        <div class="box_drop">
                            <div class="arr"></div>
                            <div class="box_credit after_clear">
                                <div class="left"><?php print t('SEPULSA CREDIT'); ?></div>
                                <div class="right"><?php print number_format(userpoints_get_current_points($user->uid, 'all'), 0, ",", "."); ?> IDR</div>
                            </div>
                            <a href="<?php print url('user/' . $user->uid); ?>"><?php print t('AKUN'); ?></a>
                            <a href="<?php print url("konfirmasi"); ?>"><?php print t('KONFIRMASI PEMBAYARAN'); ?></a>
                            <a href="<?php print url("user/logout"); ?>"><?php print t('KELUAR'); ?></a>
                        </div>
                    </div>
                    <div class="nav_mobile">
                        <a href="" class="toggle">toogle</a>
                        <div class="box_drop">
                            <div class="box_credit after_clear">
                                <div class="left"><?php print t('SEPULSA CREDIT'); ?></div>
                                <div class="right"><?php print commerce_currency_format(userpoints_get_current_points($user->uid, 'all'), 'IDR'); ?></div>
                            </div>
                            <a href="<?php print url('user/' . $user->uid); ?>" ><?php print t('Informasi Akun'); ?></a>
                            <a href="<?php print url('user/orders'); ?>"><?php print t('Order Transaksi'); ?></a>
                            <a href="<?php print url('user/token'); ?>"><?php print t('Info Kartu'); ?></a>
                            <a href="<?php print url('user/wallet'); ?>"><?php print t('Dompet Saya'); ?></a>
<<<<<<< HEAD
                            <a href="<?php print url('user/orders'); ?>"><?php print t('Voucher Diskon'); ?></a>
=======
                            <a href="<?php print url("user/voucher"); ?>"><?php print t('Voucher Diskon'); ?></a>
>>>>>>> 16081325d48f6f3164ea8505fe39e30605a83ce9
                            <a href="<?php print url("konfirmasi"); ?>"><?php print t('Konfirmasi Pembayaran'); ?></a>
                            <a href="<?php print url("user/logout"); ?>"><?php print t('KELUAR'); ?></a>
                            <div class="m_lang">
                                <a href="">English</a>
                                <a href="">Indonesia</a>
                            </div>
                        </div>
                    </div>
                <?php } else { ?>
                    <div class="nav_mobile">
                        <a href="" class="toggle">toogle</a>
                        <div class="box_drop">
                            <a href="<?php print url("user/login"); ?>" class="login">LOGIN</a>
                            <div class="m_lang">
                                <a href="">English</a>
                                <a href="">Indonesia</a>
                            </div>
                        </div>
                    </div>
                    <div class="box login">
                        <a href="<?php print url("user/login"); ?>" >
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
                     <?php if ($language->language == 'id') { ?>
                        ID
                      <?php } else { ?>
                        EN
                      <?php } ?>
                    </a>
                    <div class="box_drop">
                        <div class="arr"></div>
                      <?php if ($language->language == 'id') : ?>
                        <a href="?language=en">EN</a>
                      <?php else: ?>
                        <a href="?language=id">ID</a>
                      <?php endif; ?>
                    </div>
                </div>
            </div>

        </div>
    </header>
