<!--page--type-blog.tpl.php -->
<?php
/**
 * @file
 * page--type-blog.tpl.php
 *
 * @author roy@sepulsa.com
 * @since Nov 16 2015
 */
?>
<section id="frameBlog">
  <div class="wrapper">
    <div id="inner" class="clearfix">
      <div id="left_column">      
        <?php print $messages; ?>                         
        <?php if ($title): ?>
         <h2><?php print $title ?></h2>
        <?php endif; ?>
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