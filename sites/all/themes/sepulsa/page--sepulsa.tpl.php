<?php
/**
 * @file
 * page.tpl.php
 *
 * @author ananto@sepulsa.com
 * @since January 27th 2015.
 */

global $base_url;

?>
<div id="page-wrapper">
  <?php include "header.tpl.php"; ?>

  <section id="content">
    <div class="container">
      <div id="main">
        <?php print $messages; ?>

        <div class="row">
          <div class="col-sm-6 col-md-8">
            <div class="tab-container full-width style1 box">
              <ul class="tabs clearfix">
                <li<?php if($active_tab == 'topup') print ' class="active"';?>><a href="#topup" data-toggle="tab">Isi Pulsa</a></li>
                <li<?php if($active_tab == 'token_reload') print ' class="active"';?>><a href="#token-reload" data-toggle="tab">Listrik</a></li>
              </ul>
              <div id="topup" class="tab-content<?php if($active_tab == 'topup') print ' in active';?>">
                <div class="tab-pane">
                  <?php print render($page['content']); ?>
                </div>
              </div>
              <div id="token-reload" class="tab-content<?php if($active_tab == 'token_reload') print ' in active';?>">
                <div class="tab-pane">
                  <?php print render($token_reload_form); ?>
                </div>
              </div>
            </div>
          </div>

          <div class="col-sm-6 col-md-4">
            <?php print render($page['banner']); ?>
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

  <?php include "footer.tpl.php"; ?>

</div>
