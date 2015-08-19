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
    <img src="<?php print $theme_path; ?>/images/content/banner_home.jpg" alt="Praktis Isi Pulsa. Gratis Voucher Belanja" />
    <div class="caption" id="top_page">
        <h1 style="font-family: 'robotoregular';font-size: 36px;margin: 0 0 15px 0;"><?php print t('Praktis Isi Pulsa. Gratis Voucher Belanja.'); ?></h1>
        <p><?php print t('Kini isi ulang pulsa lebih dari sekedar mudah, tapi menguntungkan!'); ?></p>
        <a href="#easy_step"><?php print t('Gimana Caranya?'); ?></a>
    </div>
</section>
<section class="h_middle_top">
  <div class="wrapper">
    <div class="home_tab">
      <div class="nav_tab after_clear"> 
        <table class="tab-pulsa">
          <tr>
            <td>
              <a href="" class="active" target="target_1">
                  <span class="ico pulsa"></span>
                  <?php print t('isi pulsa & bolt'); ?>
              </a>
            </td>
            <td>
              <a href="" target="target_2">
                  <span class="ico listrik"></span>
                  <?php print t('bayar listrik'); ?>
              </a>
            </td>
          </tr>
        </table>
      </div>

      <div class="content_tab">
        <div class="tab form style_1" id="target_1">
          <?php print render($messages); ?>
          <?php print render($page['content']); ?>

          <div class="topup-notes"> 
              <ul>
                <li><?php print t('Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.'); ?></li>
                <li><?php print t('Hanya bisa mengisi pulsa ke nomer dan nominal yang sama satu kali dalam sehari.'); ?></li>
              </ul>
          </div>
        </div>
        <div class="tab form style_1" id="target_2">
          <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
          <?php print render($token_reload_form); ?>
          <?php endif; ?>
          
          <div class="topup-notes topup-notes-2"> 
              <ul>
                <li><?php print t('Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.'); ?></li>
                <li><?php print t('Hanya bisa mengisi pulsa ke nomer dan nominal yang sama satu kali dalam sehari.'); ?></li>
              </ul>
          </div>
        </div>
      </div>

    </div>


  </div>
</section>

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
