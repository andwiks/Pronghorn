<tfoot>
  <?php foreach ($components as $component): ?>
    <tr class="cart_item">
      <td class="product-price" colspan="3">
        <strong><?php print $component['title']; ?></strong>
      </td>
      <td class="product-subtotal">
        <span class="amount"><strong><?php print $component['formatted_price']; ?></strong></span>
      </td>
    </tr>
  <?php endforeach; ?>
</tfoot>
