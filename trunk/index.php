<?php

include "version.inc";

if (!class_exists("WebContext")) include("webcontext.inc");

if (isset($_GET['context'])) {

	$contextfile = $_GET['context'] . ".context";

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