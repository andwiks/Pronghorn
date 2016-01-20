<?php
/**
 * @file
 * topup_handler_field_line_item_operation.inc
 */

class kraken_api_views_handler_kraken_data extends views_handler_field_serialized {

  function render($values) {
    $value = $values->{$this->field_alias};

    if ($this->options['format'] == 'unserialized') {
      return check_plain(print_r(unserialize($value)['data'], TRUE));
    }
    elseif ($this->options['format'] == 'key' && !empty($this->options['key'])) {
      $value = (array) unserialize($value);
      if (isset($value['data'][$this->options['key']])) {
        return check_plain($value['data'][$this->options['key']]);
      }
      else {
        return FALSE;
      }
    }

    return $value['data'];
  }
}
