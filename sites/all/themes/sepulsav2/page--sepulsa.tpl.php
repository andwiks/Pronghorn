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

// Get sepulsa settings.
$settings = variable_get('sepulsa_settings', array());
?>
  <!-- middle -->
<section class="banner">
<?php print render($page['banner']); ?>
</section>
<section class="h_middle_top">
  <div class="wrapper">
    <div class="home_tab">
      <div class="nav_tab after_clear">
        <a href="" class="active" target="target_1">
          <span class="ico pulsa"></span>
          <?php print t('isi pulsa'); ?>
        </a>
        <a href="" target="target_2">
          <span class="ico listrik"></span>
          <?php print t('token listrik'); ?>
        </a>
        <?php if (module_exists('biznet') && user_access('view any commerce_product entity of bundle biznet')): ?>
          <a href="" target="target_3">
            <span class="ico bolt"></span>
            <?php print t('biznet wifi'); ?>
          </a>
        <?php endif; ?>
        <?php if (module_exists('multifinance') && user_access('view any commerce_product entity of bundle multifinance')): ?>
          <a href="" target="target_4">
            <span class="ico finance"></span>
            <?php print t('multifinance'); ?>
          </a>
        <?php endif; ?>
      </div>

      <div class="content_tab">
        <div class="tab form style_1" id="target_1">
          <?php print render($messages); ?>
          <?php print render($page['content']); ?>

          <div class="topup-notes<?php print (isset($settings['multipaid_product']) && !empty($settings['multipaid_product'])) ? ' topup-notes-multi' : ''; ?>">
              <ul>
                <li><?php print t('Segala bentuk informasi yang anda masukkan akan kami jaga kerahasiaannya.'); ?></li>
              </ul>
          </div>
        </div>
        <?php if (module_exists('pln_prepaid') && user_access('view any commerce_product entity of bundle pln_prepaid')): ?>
          <div class="tab form style_1" id="target_2">
            <?php print render($messages); ?>
            <?php print render($pln_prepaid_form); ?>
            <div class="topup-notes topup-notes-2<?php print (isset($settings['multipaid_product']) && !empty($settings['multipaid_product'])) ? ' topup-notes-multi' : ''; ?>">
                <ul>
                  <li><?php print t('Segala bentuk informasi yang anda masukkan akan kami jaga kerahasiaannya.'); ?></li>
                </ul>
            </div>
          </div>
        <?php elseif (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
          <div class="tab form style_1" id="target_2">
            <?php print render($messages); ?>
            <?php print render($token_reload_form); ?>
            <div class="topup-notes topup-notes-2<?php print (isset($settings['multipaid_product']) && !empty($settings['multipaid_product'])) ? ' topup-notes-multi' : ''; ?>">
                <ul>
                  <li><?php print t('Segala bentuk informasi yang anda masukkan akan kami jaga kerahasiaannya.'); ?></li>
                </ul>
            </div>
          </div>
        <?php endif; ?>
        <?php if (module_exists('biznet') && user_access('view any commerce_product entity of bundle biznet')): ?>
          <div class="tab form style_1" id="target_3">
            <?php print render($messages); ?>
            <?php print render($biznet_form); ?>

            <div class="topup-notes topup-notes-3<?php print (isset($settings['multipaid_product']) && !empty($settings['multipaid_product'])) ? ' topup-notes-multi' : ''; ?>">
                <ul>
                  <li><?php print t('Segala bentuk informasi yang anda masukkan akan kami jaga kerahasiaannya.'); ?></li>
                </ul>
            </div>
          </div>
        <?php endif; ?>
        <?php if (module_exists('multifinance') && user_access('view any commerce_product entity of bundle multifinance')): ?>
          <div class="tab form style_1" id="target_4">
            <?php print render($messages); ?>
            <?php print render($multifinance_form); ?>

            <div class="topup-notes topup-notes-3<?php print (isset($settings['multipaid_product']) && !empty($settings['multipaid_product'])) ? ' topup-notes-multi' : ''; ?>">
                <ul>
                  <li><?php print t('Segala bentuk informasi yang anda masukkan akan kami jaga kerahasiaannya.'); ?></li>
                </ul>
            </div>
          </div>
        <?php endif; ?>
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
