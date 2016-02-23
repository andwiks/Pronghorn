<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July 23th 2015.
 */
?>
<section class="h_middle_top">
  <div class="wrapper">
    <div class="home_tab after_clear" style="top:auto;margin-top:75px">
      <div class="content_tab" style="border-top:none">
        <div class="form style_1">
          <?php print $messages; ?>
          <?php if ($title): ?>
            <h2><?php print $title ?></h2>
          <?php endif; ?>
          <?php print render($page['content']); ?>

          <div class="topup-notes">
            <ul>
              <li><?php print t('Segala bentuk informasi yang anda masukan akan kami jaga kerahasiaannya.'); ?></li>
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
