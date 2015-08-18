<?php 
$form = drupal_get_form('admin_referral_list_form');
print drupal_render($form);
//drupal_set_message("<pre>".print_r($content, true)."</pre>");
?>
<?php $count = 1; ?>
<p>
Referred By: <?php print (empty($content['referred_by'])) ? " - " : $content['referred_by'] ?>
</p>
<table>
	<thead>
		<tr>
			<th>Email</th>
			<th>Date Registered</th>
			<th>Date Transaction</th>
			<th>Credit Earned</th>
		</tr>
	</thead>
	<tbody>
			<?php if (isset($content['referral_list'])) { ?>
				<?php foreach ($content['referral_list'] as $data) { ?>
				<tr class="<?php print ($count%2 == 0) ? "even" : "odd" ?>">
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