<div class="wrap_popup" id="login" >
    <div class="box_popup login">
        <a href="" class="close"><img src="<?php print $theme_path; ?>/images/material/close_pop.png" alt="close" /></a>
        <div class="wrapper after_clear">
            <h2>Log In dengan akun Sepulsa.</h2>
            <h6>Selamat datang! Masukan password anda untuk masuk ke akun.</h6>
            <span class="border"></span>
            <div class="left form">
                <?php $elements = drupal_get_form('user_login_block'); ?>
                <?php print drupal_render($elements); ?>
            </div>
            <div class="left spar">
                atau
            </div>
            <div class="right">
                <a href=""><img src="<?php print $theme_path; ?>/images/content/fb_login.jpg" alt="facebook login" /></a>
                <a href=""><img src="<?php print $theme_path; ?>/images/content/tw_login.jpg" alt="facebook login" /></a>

                <p>
                    Belum punya akun Sepulsa?
                    <a href="">Click disini</a> untuk buat akun.
                </p>
            </div>
        </div>
    </div>
</div>
<div class="wrap_popup voucher" id="detail_voucher">
    <div class="box_popup">
        <a href="" class="close style_1"><img src="<?php print $theme_path; ?>/images/material/close_popup.png" alt="close" /></a>
        <div class="content_pop">
            <h3></h3>
            <div class="image">
            </div>
            <h4></h4>

            <div class="redeem">
                <h6>REEDEM Voucher kamu</h6>
                <span>
                    Kode akan dicheck dan diisi oleh pihak merchant yang tertera di kupon.
                    Tunjukan halaman ini, dan reedem voucher kamu.
                </span>
                <div class="form">
                    <form>
                        <div class="row"><input type="text" placeholder="Masukan 14 digit kode reedem"/></div>                        
                        <div class="row">
                            <input type="submit" value="REDEEM VOUCHER" class="bt bt_std" />
                        </div>
                    </form>
                </div>
            </div>

            <h5></h5>
            <p>
            </p>
        </div>
        <div class="nav_pop">
            <a href="" class="prev"><</a> 
            <a href="" class="next">></a> 
        </div>
    </div>
</div>
