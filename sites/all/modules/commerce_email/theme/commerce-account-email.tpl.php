<?php
/**
 * @file
 * Default implementation of the commerce order email template.
 *
 * Available variables:
 * - $site: site_name & site_url
 * - $user: user object.
 * - $login: one time login url
 * - $language: language code of the mail.
 */
?>
<?php print $user->name; ?>,

<p>A site administrator at <?php print $site['site_name']; ?> has created an account for you. You may now log in by clicking this link or copying and pasting it to your browser:</p>

<p><?php print $login; ?></p>

<p>This link can only be used once to log in and will lead you to a page where you can set your password.</p>

<p>After setting your password, you will be able to log in at <?php print $site['site_url']; ?>/user in the future using:</p>

<p>username:  <?php print $user->name; ?></p>

<p>password: Your password</p>

-- <?php print $site['site_name']; ?> team

<p style="font-weight: bold;">Commerce Email - ACCOUNT TEMPLATE</p>