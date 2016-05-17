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
if (module_exists('cdn') && cdn_status_is_enabled()){
    $theme_path = file_create_url('public://' . '/' . path_to_theme());
}
?>
<section class="banner sblog">
    <img src="<?php print $theme_path; ?>/images/content/banner_blog.jpg" alt="sepulsa blog" />
    <div class="caption">
        <h1><?php print t('Berbagi Info Menarik dan Tips Bermanfaat.'); ?></h1>
        <p><?php print t('Baca Info Menarik dan Tips Bermanfaat Seputar Pulsa di Sepulsa Blog.'); ?></p>
    </div>
</section>

<section id="frameBlog">
    <div class="wrapper">

        <div id="inner" class="clearfix">
            <div id="left_column">
                <?php print render($title_prefix); ?>
                <?php if ($title): ?>
                    <?php
                      /*
                      <h1><?php print $title ?></h1>
                      */
                    ?>
                <?php endif; ?>

                <?php print $messages; ?>

                <?php print render($page['content']); ?>

                <?php print render($title_suffix); ?>
            </div>
            <?php if (!empty($page['sidebar_first'])): ?>
            <div id="right_column">
               <?php print render($page['sidebar_first']); ?>
            </div>
            <?php endif; ?>
        </div>

    </div>
</section>
