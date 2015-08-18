<?php $count = 1; ?>
<table class="table style1 grid_akun order_transaksi">
	<thead>
		<tr>
			<th class="views-field views-field-order-id">Email</th>
			<th class="views-field views-field-order-id">Date Registered</th>
			<th class="views-field views-field-order-id">Date Transaction</th>
			<th class="views-field views-field-order-id">Credit Earned</th>
		</tr>
	</thead>
	<tbody>
			<?php if (isset($content['referral_list'])) { ?>
				<?php foreach ($content['referral_list'] as $data) { ?>
				<tr class="<?php print ($count%2 == 0) ? "" : "odd" ?>">
					<td><?php print $data['email'] ?></td>
					<td><?php print $data['date_registered'] ?></td>
					<td><?php print $data['date_txn'] ?></td>
					<td><?php print (empty($data['points']))? '-' : $data['points'] ?></td>
				</tr>
				<?php $count = $count + 1; ?>
			<?php } ?>
		<?php } ?>
	</tbody>
</table>