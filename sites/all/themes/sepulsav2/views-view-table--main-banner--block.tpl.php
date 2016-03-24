<?php foreach ($rows as $row_count => $row): ?>
<?php print isset($row['field_main_banner']) ? $row['field_main_banner'] : ''; ?>
<?php print $row['body']; ?>
<?php endforeach; ?>
