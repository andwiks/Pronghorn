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
      <?php if (!empty($page['sidebar_first'])): ?>
        <div class="sidebar col-sm-4 col-sm-3">
          <?php print render($page['sidebar_first']); ?>
        </div>
      <?php endif; ?>
      <div id="main"<?php if (!empty($page['sidebar_first'])) print ' class="col-sm-8 col-md-9"'; ?>>
        <?php print render($page['header']); ?>

        <?php if ($tabs['#primary']): ?><div id="tabs-wrapper" class="tab-container full-width style1 box"><?php endif; ?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1<?php print $tabs ? ' class="with-tabs"' : '' ?>><?php print $title ?></h1>
        <?php endif; ?>

        <?php print render($title_suffix); ?>
        <?php if ($tabs['#primary']) { ?>
          <?php print render($tabs); ?>
          <div class="tab-content in active">
            <?php print render($tabs2); ?>
            <?php print $messages; ?>
            <?php print render($page['help']); ?>
            <?php if (arg(0) == 'user' && arg(1) > 0) { ?>
              <?php print render($page['content']); ?>
            <?php } else { ?>
              <div class="heading-box"><?php print render($page['content']); ?></div>
            <?php } ?>
          </div>
        </div>
        <?php } else { ?>
          <?php print render($tabs2); ?>
          <?php print $messages; ?>
          <?php print render($page['help']); ?>

          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <div class="clearfix">
            <?php print render($page['content']); ?>
          </div>
        <?php } ?>

      </div>
    </div>
  </section>

  <?php include "footer.tpl.php"; ?>

</div>
