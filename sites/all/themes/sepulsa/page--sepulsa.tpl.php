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
                <li<?php if($active_tab == 'topup') print ' class="active"';?>><a href="#topup" data-toggle="tab">Isi Pulsa &amp; Bolt</a></li>
                <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
                  <li<?php if($active_tab == 'token_reload') print ' class="active"';?>><a href="#token-reload" data-toggle="tab">Listrik</a></li>
                <?php endif; ?>
				<?php if (module_exists('deposit_sepulsa') && user_is_logged_in()): ?>  
                  <li<?php if($active_tab == 'deposit_sepulsa') print ' class="active"';?>><a href="#deposit-sepulsa" data-toggle="tab">Deposit</a></li>
				<?php endif; ?>
              </ul>
              <div id="topup" class="tab-content<?php if($active_tab == 'topup') print ' in active';?>">
                <div class="tab-pane">
                  <?php print render($page['content']); ?>
                </div>
                <div style="color:red; margin-top:10px;">* Hanya bisa mengisi pulsa ke nomer dan nominal yang sama satu kali dalam sehari.</div>
              </div>
              <?php if (module_exists('token_reload') && user_access('view any commerce_product entity of bundle electricity')): ?>
                <div id="token-reload" class="tab-content<?php if($active_tab == 'token_reload') print ' in active';?>">
                  <div class="tab-pane">
                    <?php print render($token_reload_form); ?>
                  </div>
                </div>
              <?php endif; ?>
			  <?php if (module_exists('deposit_sepulsa') && user_is_logged_in()): ?> 
                <div id="deposit-sepulsa" class="tab-content<?php if($active_tab == 'deposit_sepulsa') print ' in active';?>">
                  <div class="tab-pane">
                    <?php print render($deposit_sepulsa_form); ?>
                  </div>
                </div>
              <?php endif; ?>
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
