<?php
	ini_set('include_path', '../includes/');
	require_once("database.inc");
	require_once("table.inc");
	require_once("context.inc");
	require_once("whereclause.inc");
	require_once("wherecombination.inc");

	$FILENAME = "reserve_machine.context";

	$s = implode("", @file($FILENAME));
	$context = unserialize($s);

	$machinestest = new WhereClause("`machines`.`Test` = '1'");
	$machinesdev = new WhereClause("`machines`.`Development` = '1'");
	//$context->table->whereclause = new WhereClause("`machines`.`Test` = '1'");
	$context->table->whereclause = new WhereCombination($machinestest, "OR", $machinesdev);

	$s = serialize($context);
	$fp = fopen($FILENAME, "w");
	fputs($fp, $s);
	fclose($fp);

?>