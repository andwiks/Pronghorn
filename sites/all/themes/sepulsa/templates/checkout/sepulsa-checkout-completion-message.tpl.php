<div class="checkout-completion-message">
  <div class="container">
    <div class="heading-box">
      <h2 class="box-title">Terimakasih, Transaksi Anda Sukses</h2>
      <br>

      <div class="tqbox">
        <?php if ($payment_method == 'bank_transfer'): ?>
          <p>Mohon lakukan pembayaran melalui internet/mobile banking atau melalui teller di bank pilihan kamu ke akun berikut dalam jangka waktu kurang dari 24 jam. Jika tidak pesanan kamu akan dibatalkan.</p>

          <p class="text-left well" style="display:inline-block">
            <?php print t('Banking institution'); ?> : <strong><?php print $payment_settings['details']['account_bank']; ?></strong>
            <br><?php print t('Account number'); ?> : <strong><?php print $payment_settings['details']['account_number']; ?></strong>
            <br><?php print t('Account owner'); ?> : <strong><?php print $payment_settings['details']['account_owner']; ?></strong>
            <br><?php print t('Branch office'); ?> : <?php print $payment_settings['details']['account_branch']; ?>
            <br>
            <?php if (!empty($payment_settings['policy'])): ?>
              <br><?php print $payment_settings['policy']; ?>
              <br>
            <?php endif; ?>
            <br>Keterangan transfer : Sepulsa Order <?php print $order->order_id; ?>
          </p>

          <p>Untuk tahap akhir proses pembayaran, silakan lakukan konfirmasi pembayaran dengan melakukan konfirmasi pembayaran <?php print l('disini', 'konfirmasi'); ?> agar transaksi kamu dapat langsung kami proses.</p>

          <p>Pulsa kamu akan terisi setelah pembayaran sukses dan dikonfirmasi.
        <?php elseif ($payment_method == 'commerce_veritrans'): ?>
          <p>Pulsa kamu akan terisi sebentar lagi.
        <?php endif; ?>

        <?php if ($authenticated): ?>
          Kamu bisa melihat status pulsa kamu atau histori transaksi di halaman <?php print l('pesanan', 'user/' . $user->uid . '/orders'); ?>. Cek juga kupon-kupon yang kamu dapatkan di halaman <?php print l('detil', 'user'); ?>, atau kamu bisa kembali ke <?php print l('halaman depan', '<front>'); ?>.</p>
        <?php else: ?>
          Jika ini adalah transaksi pertama kamu, silahkan cek email kamu untuk mendapatkan instruksi login. Setelah login, kamu bisa melihat status pulsa kamu di halaman <?php print l('akun saya', 'user'); ?>.</p>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>
