<?php global $base_url; ?>
<?php
  drupal_add_css(
    '.post-image { margin-bottom: 0; } @media(min-width:768px){ .blog-posts .post-grid .post-content { height: 150px; } } .popup-box { max-width: 44em; } .popupbox-close { position: absolute; top: 0; right: 0; } .popup-close { width: 40px; background-color: #f5f5f5; border-radius: 50%; } .popup-box h2 { margin-bottom: 5px } .popup-image img { width: 100% }',
    array(
      'group' => CSS_THEME,
      'type' => 'inline',
      'media' => 'screen'
    )
  );
?>

<div class="view-coupon-box">
  <div class="blog-posts row">
    <?php foreach ($rows as $row_count => $row): ?>
      <?php $web_name = $row['field_website']; ?>
      <?php $web_link = $row['field_website_link']; ?> 
      <?php if (!isset($web_link)): ?>
      <?php $web_link = 'http://'.$row['field_website']; ?>
      <?php endif;
      ?>
      <div class="col-sm-3 col-xs-12">
        <article class="post post-grid">
          <div class="post-image">
            <div class="image-container">
              <a href="#pop<?php print $row['nid']; ?>" class="image pop<?php print $row['nid']; ?>_open"><?php print $row['field_coupon_product_image']; ?></a>
            </div>
          </div>
          <div class="post-content">
            <div class="post-date"><span><?php print $row['title']; ?></span></div>
            <?php print ($row['field_description']); ?>
            <div class="post-link">Syarat penggunaan <a href="#pop<?php print $row['nid']; ?>" class="image pop<?php print $row['nid']; ?>_open" style="color:#dc2c27">klik disini</a></div>
          </div>
          <div id="pop<?php print $row['nid']; ?>" class="well popup-box">
            <a href="#" class="popupbox-close pop<?php print $row['nid']; ?>_close"><img src="<?php print $base_url . '/' . path_to_theme(); ?>/images/ico-close.png" class="popup-close"></a>
            <p class="popup-image"><?php print ($row['field_coupon_product_image']); ?></p>
            <h2><strong><?php print $row['title']; ?></strong></h2>
            <a href="<?php print $web_link; ?>" target="_blank"><?php print $web_name; ?></a>
            <p><?php print ($row['field_description']); ?>.</p>
            <hr class="color-text">
            <p><strong><?php print t('Terms & Conditions'); ?></strong></p>  
            <?php if (isset($row['field_coupon_tnc'])) print ($row['field_coupon_tnc']); else print "-"; ?>
          </div>
        </article>
          
      </div>
      <?php
      drupal_add_js('jQuery(document).ready(function () { jQuery("#pop'.$row['nid'].'").popup(); });',
        array('type' => 'inline', 'scope' => 'footer', 'weight' => 5)
      );
      ?>
    <?php endforeach; ?>
  </div>
</div>