<?php

session_start();

ini_set('include_path', './includes/');

require_once("mescaline.inc");

$mescaline = new Mescaline();
$mescaline->init();
print $mescaline->buildHTML();

?>
