<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July 23th 2015.
 */
?>
<?php if ($node->type == 'campaign_page'): ?>
<section class="std_content">
  <div class="wrapper_2 other after_clear">
    <?php print $messages; ?>
    <?php if ($title): ?>
      <h2><?php print $title ?></h2>
    <?php endif; ?>

    <?php print render($page['content']); ?>
  </div>
</section>
<?php elseif ($node->type == 'blog') : ?>

<div id="fb-root"></div>
<script>(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/sdk.js#xfbml=1&version=v2.5&appId=1391916274445735";
  fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));</script>

<section id="frameBlog">
    <div class="wrapper">
        
        <div id="inner" class="clearfix">
            <div id="left_column">      
                <?php print render($title_prefix); ?>
                <?php print $messages; ?>
                
                <?php if ($title): ?>
                  <h1><?php print $title ?></h1>
                <?php endif; ?>
                
                <?php print render($title_suffix); ?>
                <?php if ($tabs): ?>
                  <div class="tabs">
                      <?php print render($tabs); ?>
                  </div>
                <?php endif; ?>

                <?php if ($action_links): ?>
                  <ul class="action-links">
                    <?php print render($action_links); ?>
                  </ul>
                <?php endif; ?>

                <?php print render($page['content']); ?>

                <div style="float:left; margin-right:10px;">
                  <div class="fb-share-button" data-href="<?php $_GET['q']?>"  data-layout="button_count"></div>
                </div>

                <div style="float:left;">
                  <a href="https://twitter.com/share" class="twitter-share-button" data-via="sepulsa_id">Tweet</a>
                  <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>
                </div>
            </div>
            <?php if (!empty($page['sidebar_first'])): ?>
            <div id="right_column">
               <?php print render($page['sidebar_first']); ?>
            </div> 
            <?php endif; ?>           
        </div>
        
    </div>
</section>
<?php else: ?>
<section class="std_content">
  <div class="wrapper_2 other after_clear">
    <?php if (!empty($page['sidebar_first'])): ?>
      <div class="c_left">
        <?php print render($page['sidebar_first']); ?>
      </div>
    <?php endif; ?>

    <div class="c_right">
      <?php print $messages; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h2><?php print $title ?></h2>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </div>
  </div>
</section>
<?php endif; ?>