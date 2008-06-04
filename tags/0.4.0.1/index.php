<?php

ini_set('include_path', './includes/');

require_once("version.inc");
require_once("webcontext.inc");

if (isset($_GET['context'])) {

	$contextfile = "./context/" . $_GET['context'] . ".context";

	if (!file_exists($contextfile)) {

		print "ERROR: context \"" . $_GET['context'] ."\" could not be found.";
		exit(1);
	}

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	if ($webcontext->version != $version) {

		print "ERROR: context is version " . $webcontext->version . ", while the app needs version " . $version . ".";
		exit(1);
	}

	$webcontext->start();

} else {

	print "ERROR: script has to be called with the option \"context\".";
	exit(1);
}

?>