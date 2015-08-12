<?php
/**
 * @file
 * page.tpl.php
 *
 * @author dwi@sepulsa.com
 * @since July 23th 2015.
 */

global $base_url;
$theme_path = $base_url . '/' . path_to_theme();
?>
<section class="banner sblog">
    <img src="<?php print $theme_path; ?>/images/content/banner_blog.jpg" alt="blog" />
    <div class="caption">
        <h2><?php print t('Berbagi Info Menarik dan Tips Bermanfaat.'); ?></h2>
        <p><?php print t('Baca Info Menarik dan Tips Bermanfaat Seputar Pulsa di Sepulsa Blog.'); ?></p>
    </div>
</section>

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
