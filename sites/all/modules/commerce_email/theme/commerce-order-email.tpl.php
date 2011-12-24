<?php

/**
 * @file
 * Default implementation of the commerce order email template.
 *
 * Available variables:
 * - $site: site_name & site_url
 * - $order: order object.
 * - $order->order_items: array of order items
 * - $customer_profile: customer order name and address.
 * - $language: language code of the mail.
 */
?>
<?php if (!empty($customer_profile['first_name'])) { ?>
    <p>
        Dear <?php print $customer_profile['first_name']; ?> <?php if (!empty($customer_profile['last_name'])) print $customer_profile['last_name']; ?>
    </p>
<?php } ?>

<p>Thanks for your order <?php print $order->order_number; ?> at <?php print $site['site_name']; ?>.</p>

<table>
  <thead>
    <tr>
      <?php foreach ($order->order_items['header'] as $label): ?>
        <th <?php if (!empty($label['style'])) print 'style="'. implode(" ",$label['style']) .'" '; ?>>
          <?php print $label['data']; ?>
        </th>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($order->order_items['rows'] as $count => $row): ?>
      <tr>
        <?php foreach ($row['data'] as $content): ?>
          <td>
            <?php print $content['data']; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>

<p>If this is your first order with us, you will receive a separate e-mail with login instructions.</p>

<p>You can view your order history with us at any time by logging into our website at: <?php print $site['site_url']; ?>/user</p>

<p>You can find the status of your current order at: <?php print $site['site_url']; ?>/user/<?php print $order->uid; ?>/orders/<?php print $order->order_number; ?></p>

<p>Please contact us if you have any questions about your order.</p>

<p style="font-weight: bold;">Commerce Email - DEFAULT TEMPLATE</p>