<div class="wrap_popup" id="login" >
    <div class="box_popup login">
        <a href="" class="close"><img src="<?php print $theme_path; ?>/images/material/close_pop.png" alt="close" /></a>
        <div class="wrapper after_clear">
            <h2><?php print t('Log In dengan akun Sepulsa.'); ?></h2>
            <h6><?php print t('Selamat datang! Masukan password anda untuk masuk ke akun.'); ?></h6>
            <span class="border"></span>
            <div class="left form">
                <?php $elements = drupal_get_form('user_login_block'); ?>
                <?php $hybridauth = isset($elements['hybridauth']) ? $elements['hybridauth'] : ""; ?>
                <?php unset($elements['hybridauth']); ?>
                <?php print drupal_render($elements); ?>
            </div>
            <div class="left spar">
                atau
            </div>
            <div class="right">
              <?php print drupal_render($hybridauth); ?>
              <p>
                  <?php print t('Belum punya akun Sepulsa?'); ?>
                  <a href="<?php print url('user/register'); ?>"><?php print t('Klik disini'); ?></a> <?php print t('untuk buat akun.'); ?>
              </p>
            </div>
        </div>
    </div>
</div>
