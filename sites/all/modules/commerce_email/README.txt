Commerce Email for Drupal Commerce

- Adds a configuration page for the order email
- Email content can be entered into textarea/wysiwyg (if available) form
- Email content can also be set to be loaded from a template file:
commerce-order-email.tpl.php
- Allows the insertion of the order items into the email, token addition:
[commerce-email:order-items]
- Adds different language versions of each email

N.B. After install you will need to disable the existing checkout rules for
sending the order and account email, otherwise it will send out 2 emails.