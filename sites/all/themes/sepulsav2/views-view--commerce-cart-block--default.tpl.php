<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */

global $base_url;
global $user;

/* TO DO : move this to module */
$order = commerce_cart_order_load($user->uid);
$order_wrapper = entity_metadata_wrapper('commerce_order', $order);
$line_items = $order_wrapper->commerce_line_items;
$total = commerce_line_items_total($line_items);
?>


    <div class="grid_voucher_order">
        <table>
            <thead>
                <tr>
                    <th>Order</th>
                    <th colspan="2">Harga</th>
                </tr>
            </thead>
            <?php if ($rows): ?>
            <tbody>
            <?php print $rows; ?>
            </tbody>
            <?php elseif ($empty): ?>
            <tbody>
                <tr>
                    <td colspan="3">
                        <div class="view-empty">
                          <?php print $empty; ?>
                        </div>
                    </td>
                </tr>
            </tbody>
            <?php endif; ?>
            <tfoot>
                <tr>
                    <th>TOTAL PAYMENT</th>
                    <th colspan="2">
                        <?php print number_format($total['amount'], 0, ".", ","); ?> IDR
                        <a href="<?php print url("checkout"); ?>" class="add_bt">Proses</a>
                    </th>                            
                </tr>
            </tfoot>
        </table>
        <p>
            * Pemilihan Voucher diskon tidak berlaku <b>kelipatan</b>. Maksimal 3 Voucher Diskon tiap kali transaksi.<br/>
            * Check Bank Promo untuk mendapatkan lebih banyak voucher diskon!
        </p>
    </div>

