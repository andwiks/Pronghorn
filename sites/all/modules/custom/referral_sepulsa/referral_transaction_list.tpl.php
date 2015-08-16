<?php $count = 1; ?>
<table>
	<thead>
		<tr>
			<th>Email</th>
			<th>Order ID</th>
			<th>Order Total</th>
			<th>Date Transaction</th>
			<th>Referrer Email</th>
		</tr>
	</thead>
	<tbody>
			<?php if (isset($content)) { ?>
				<?php foreach ($content as $data) { ?>
				<tr class="<?php print ($count%2 == 0) ? "even" : "odd" ?>">
					<td><?php print $data['mail'] ?></td>
					<td><?php print $data['order_id'] ?></td>
					<td><?php print $data['order_total'] ?></td>
					<td><?php print $data['order_date'] ?></td>
					<td><?php print $data['referrer_mail'] ?></td>
				</tr>
				<?php $count = $count + 1; ?>
			<?php } ?>
		<?php } ?>
	</tbody>
</table>