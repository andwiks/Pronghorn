<h2>Your Referral Link</h2>
<p><input id='referral_text' class='referral_text' size='50' readonly='true' type='text' value='<?php print $content['referral_link'] ?>'></p> 
<script type="text/javascript" src="<?php print $content['path_to_theme'] ?>/js/zclip/jquery.zclip.js"></script>
<script type="text/javascript">
  (function ($) {
      $(document).ready(function () {
          $("#copy-link-wrap").zclip({
              path: "<?php print $content['path_to_theme'] ?>/js/zclip/ZeroClipboard.swf",
              copy: $("#toclip").val(),
              afterCopy: function () {
                  alert("Your Referral link has been copied");
              }
          });
      });
  })(jQuery)
</script>
<style>
	.referral_text { background-color: #eee; border: 1px solid #ccc; height: 40px; line-height: 40px; padding: 0 15px; box-shadow: inset 0px 0px 10px rgba(0,0,0,0.1); margin: 10px 0; }
	.share_referral { margin-bottom: 20px; }
	.share_referral span { float: left; margin-right: 5px; font-weight: 700; text-transform: uppercase; margin-top: 10px; }
	.share_referral a { float: left; border: 1px solid #ccc; background: #f9f9f9; padding: 8px 20px; border-radius: 8px; margin-left: 10px; font-size: 14px;}

</style>
<div class="share_referral clearfix"><span>Share in:</span>
	<a href="http://www.facebook.com/sharer.php?u=<?php print $content['referral_link'] ?>" target="_blank"><i class="fa fa-facebook"></i> Facebook</a> 
	<a href="http://twitter.com/share?url=<?php print $content['referral_link'] ?>&text=TEXT&hashtags=sepulsa" target="_blank""><i class="fa fa-twitter"></i> Twitter</a> 
	<a href="#" id="copy-link-wrap">
		<i class="fa fa-envelope"></i> Copy
		<input name="toclip" id="toclip" type="hidden" value="<?php print $content['referral_link'] ?>">
	</a>
</div>
<div>&nbsp;</div>
<h2>Your Referral List</h2>
<table class="table style1">
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
				<tr>
					<td><?php print $data['email'] ?></td>
					<td><?php print $data['date_registered'] ?></td>
					<td><?php print $data['date_txn'] ?></td>
					<td><?php print (empty($data['points']))? '-' : $data['points'] ?></td>
				</tr>
			<?php } ?>
		<?php } ?>
	</tbody>
</table>