<section class="promo_banner">
    <div class="wrapper">
        <div class="slider">
        <?php foreach ($rows as $row_count => $row): ?>
            <div class="slide">
                <?php print $row['field_image_banner']; ?>
                <a href="<?php print $row['field_link_target']; ?>" class="bt_std"><?php print $row['field_link_title']; ?></a>
            </div>
        <?php endforeach; ?>
        </div>
    </div>
</section>
