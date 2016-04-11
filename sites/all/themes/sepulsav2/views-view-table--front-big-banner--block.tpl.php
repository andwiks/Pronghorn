<section class="promo_banner">
    <div class="wrapper">
        <div class="slider">
        <?php foreach ($rows as $row_count => $row): ?>
            <a href="<?php print $row['field_link_target']  ?>">
            <div class="slide">
                <?php print $row['field_image_banner']; ?>
                <div class="slide_content">
                    <?php print $row['field_link_title']; ?>
                </div>
                
            </div>
            </a>
        <?php endforeach; ?>
        </div>
    </div>
</section>

