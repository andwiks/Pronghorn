<?php
/**
 * @file
 * commerce-price-formatted-components.tpl.php
 */

// Define array to store component temporary.
$sorted = array();
// Get number of component.
$max = count($components) - 1;
// Define index for start positive amount and end negative amount.
$start = $end = 1;
// Re-sorting order total components.
foreach ($components as $key => $component) :
  // Sort by component key.
  switch ($key) :
    case 'base_price':
      // This is subtotal, must be set first.
      $sorted[0] = $component;
      break;

    case 'commerce_price_formatted_amount':
      // This is order total, must be set last.
      $sorted[$max] = $component;
      break;

    default:
      // Check the amount first: negative or positive.
      if (isset($component['price']['amount'])
        && $component['price']['amount'] > 0
      ) :
        // Positive amount: component store first.
        $sorted[$start] = $component;
        $start++;
      else:
        // Negative amount: component store last.
        $sorted[$max - $end] = $component;
        $end++;
      endif;
      break;
  endswitch;
endforeach;
// Sort by key.
ksort($sorted);
// Store it back as components.
$components = $sorted;
?>
<tfoot>
  <?php foreach ($components as $component): ?>
    <tr class="cart_item">
      <td class="product-price">
        <strong><?php print $component['title']; ?></strong>
      </td>
      <td class="product-subtotal">
        <span class="amount"><strong><?php print $component['formatted_price']; ?></strong></span>
      </td>
    </tr>
  <?php endforeach; ?>
</tfoot>
