<?php 
$count = 1;
$form = drupal_get_form('admin_daily_referral_report_form');
print drupal_render($form);

if (isset($_GET['sd']) && isset($_GET['ed']))
	$download = l('Download CSV', 'admin/people/referral/daily_referral_report/csv', array("query" => array("sd" => $_GET['sd'], "ed" => $_GET['ed'])));
else
	$download = l('Download CSV', 'admin/people/referral/daily_referral_report/csv');
?>
<div style="float: right;"><?php print $download ?></div>
<table>
	<thead>
		<tr>
			<th>Date</th>
			<th>Total Referral Order</th>
			<th>Total Revenue</th>
			<th>Average</th>
		</tr>
	</thead>
	<tbody>
			<?php if (isset($content)) { ?>
				<?php foreach ($content as $data) { ?>
				<tr class="<?php print ($count%2 == 0) ? "even" : "odd" ?>">
					<td><?php print $data['order_date'] ?></td>
					<td><?php print $data['total_order'] ?></td>
					<td><?php print $data['total_amount'] ?></td>
					<td><?php print $data['order_amount_avg'] ?></td>
				</tr>
				<?php $count = $count + 1; ?>
			<?php } ?>
		<?php } ?>
	</tbody>
</table>
<?php print theme('pager') ?>