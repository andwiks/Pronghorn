<table class="table style1">
    <thead>
        <tr>
            <th>User</th>
            <th>Date Join</th>
        </tr>
    </thead>
    <tbody>
        <?php foreach ($result as $data) { ?>
        <tr>
            <td><?php print l($data->mail, "user/$data->uid") ?></td>
            <td><?php print format_date($data->created, 'custom', REFERRAL_DATE_FORMAT) ?></td>
        </tr>
        <?php } ?>
    </tbody>
</table>