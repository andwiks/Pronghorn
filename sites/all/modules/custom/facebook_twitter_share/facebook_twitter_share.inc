<?php

function facebook_twitter_share_get_share($data = null){
    //sample array to put into this queue
    //$data['order_id'] = 1002310;
    //$data['source'] = "android";
    
    $return = array();
    $return["return"] = 0;
    
    if(!isset($data['order_id']) || empty($data['order_id'])){
        $data['order_id'] = "";
    }
    
    $return["result"] = facebook_twitter_share_set_return($data['order_id']);
    
    if(count($return["result"]) > 0){
        $return["return"] = 1;
    }
    
    return $return;
}
