<?php //print "<pre>".print_r($content, true)."</pre>"; ?>
    <div class="container">
        <div class="row">
          <div class="col-sm-6">
            <div class="post-content"><?php print $content['home_about_text'] ?>
            </div>
          </div>

          <div class="col-sm-6">
            <div class="row">
                <?php
                foreach($content['nodes'] as $node) {
                    $field_merchant_image = field_get_items('node', $node, 'field_merchant_image');
                    $image_url = file_create_url($field_merchant_image[0]['uri']);
                ?>
                    <div class="col-sm-6">
                        <div class="post-image">
                          <img src="<?php print $image_url ?>" alt="" class="img-responsive" />
                        </div>
                    </div>
                <?php } ?>
            </div>
          </div>

        </div>
    </div>