<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July 23th 2015.
 */
?>
<?php if ($node->type != 'blog'): ?>
<section class="std_content">
  <div class="wrapper_2 other after_clear">
    <?php if (!empty($page['sidebar_first'])): ?>
      <div class="c_left">
        <?php print render($page['sidebar_first']); ?>
      </div>
    <?php endif; ?>

    <div class="c_right">
      <?php if ($title): ?>
        <h2><?php print $title ?></h2>
      <?php endif; ?>

      <?php print $messages; ?>
      <?php print render($page['content']); ?>
    </div>
  </div>
</section>
<?php else: ?>
<section id="frameBlog">
    <div class="wrapper">
        
        <div id="inner" class="clearfix">
            <div id="left_column">                               
                <?php if ($title): ?>
                  <h2><?php print $title ?></h2>
                <?php endif; ?>

                <?php print $messages; ?>
                <?php print render($page['content']); ?>
                
            </div>
            <?php if (!empty($page['sidebar_first'])): ?>
            <div id="right_column">
               <?php print render($page['sidebar_first']); ?>
            </div> 
            <?php endif; ?>           
        </div>
        
    </div>
</section>
<?php endif; ?>
