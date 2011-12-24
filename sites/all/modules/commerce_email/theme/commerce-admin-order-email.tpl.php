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
<p>A customer at <?php print $site['site_name']; ?> has just placed an order.</p>

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

<p>You can view the complete order at: <?php print $site['site_url']; ?>/admin/commerce/orders/<?php print $order->order_id; ?></p>

<p style="font-weight: bold;">Commerce Email - DEFAULT ADMIN ORDER TEMPLATE</p>