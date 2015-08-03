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
    <table class="tab-isi-pulsa">
      <tr>
        <td>
          <a href="" class="active" target="target_1">
              <span class="ico pulsa"></span>
              isi pulsa & bolt
          </a>
        </td>
        <td>
          <a href="" target="target_2">
              <span class="ico listrik"></span>
              bayar listrik
          </a>
        </td>
      </tr>
    </table>
  </div>

  <div class="content_tab">
    <div class="tab form style_1" id="target_1">
      <?php print render($page['content']); ?>
      <div class="topup-notes"> 
          <ul>
            <li>Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</li>
            <li>Hanya bisa mengisi pulsa ke nomer dan nominal yang sama satu kali dalam sehari.</li>
          </ul>
      </div>
    </div>
    <div class="tab form style_1" id="target_2">
      <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
      <?php print render($token_reload_form); ?>
      <?php endif; ?>
      <span class="note">* Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.</span>
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
