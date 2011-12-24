<?php

$plugin = array(
  'title' => t('Flat rate'),
  'description' => t('Calculate shipping quotes on a flat rate.'),
  'handler' => array(
    'class' => 'CommerceShippingFlatRate',
    'parent' => 'quote_base'
  ),
);
