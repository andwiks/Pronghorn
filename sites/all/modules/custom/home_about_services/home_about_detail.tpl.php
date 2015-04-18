<?php //print "<pre>".print_r($content, true)."</pre>"; ?>
<style>
.front_first, .front_second, .front_third {
    margin-bottom: 40px;
}
.front_first h3, .front_second h3, .front_third h3 {
    border-bottom: 1px dashed #ddd;
    color: #666;
    font-weight: bold;
    padding-bottom: 15px;
    text-align: center;
    text-transform: uppercase;
}
.front_first p, .front_second p, .front_third p {
    font-size: 14px;
    text-align: center;
}
.front-img-desc {
    height: 260px;
    overflow: hidden;
}
.front-img-desc img {
    position: relative;
    top: -40px;
}
.post-content {
    background: none repeat scroll 0 0 #f5f5f5;
    padding: 20px;
}
.post-image {
    border: 1px solid #f5f5f5;
    margin-bottom: 5px;
}
.post-image img {
    margin: 0 auto;
}
.front_second .img-thumbnail {
    display: block;
    margin: 15px auto;
}
#block-block-5 {
    background: none repeat scroll 0 0 #efefef;
    border-bottom: 1px solid #dedede;
    border-top: 1px solid #dedede;
    margin-top: 40px;
    padding-bottom: 40px;
}
.front-how-to {
    text-align: center;
}
.front-how-to img {
    margin-bottom: 5px;
}

</style>
    <div class="container">
        <div class="row">
          <div class="col-sm-6">
            <div class="post-content"><?php print $content['home_about_text'] ?>
            </div>
          </div>

          <div class="col-sm-6">
            <div class="row">
                <?php
                foreach($content['merchants'] as $merchant) {
                    $field_merchant_image = $merchant->field_field_merchant_image[0]['raw']['uri'];
                    $image_url = file_create_url($field_merchant_image);
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