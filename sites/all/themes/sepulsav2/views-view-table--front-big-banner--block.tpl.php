<section class="promo_banner">
    <div class="wrapper">
        <div class="slider">
        <?php foreach ($rows as $row_count => $row): ?>
            <div class="slide">
                <?php print $row['field_image_banner']; ?>
                <div class="slide_content">
                    <?php print $row['field_link_title']; ?>
                </div>
                
            </div>
        <?php endforeach; ?>
        </div>
    </div>
</section>

