<section class="promo_banner">
    <div class="wrapper">
        <div class="slider">
        <?php foreach ($rows as $row_count => $row): ?>            
            <div class="slide">
                <a href="<?php print $row['field_link_target']  ?>">
                <?php print $row['field_image_banner']; ?>
                </a>
                <div class="slide_content">
                    <?php print $row['field_link_title']; ?>
                </div>                
            </div>
        <?php endforeach; ?>
        </div>
    </div>
</section>

