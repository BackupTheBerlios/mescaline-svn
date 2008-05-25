<?php

include("table.inc");
include("idfield.inc");
include("listfield.inc");
include("boolfield.inc");
include("webform.inc");
include("webtable.inc");
if (!class_exists("Database")) include("database.inc");

// Build fields

$fields = array();
$fields[] = new IDField("MachineID");
$fields[] = new Field("Hostname");
$fields[] = new Field("IP");
$fields[] = new Field("MAC", true);
$fields[] = new ListField("Architecture", "SELECT `architecture`.`ArchitectureID`, `architecture`.`ArchitectureName` FROM `machines`.`architecture`");
$fields[] = new ListField("OS", "SELECT `os`.`OSID`, `os`.`OSName` FROM `machines`.`os`");

// Build table

$table = new Table("machines", "SELECT `machines`.`MachineID`, `machines`.`Hostname`, `machines`.`IP`, `machines`.`MAC`, `architecture`.`ArchitectureName` AS `Architecture`, `os`.`OSName` FROM `machines`.`machines` AS `machines`, `machines`.`architecture` AS `architecture`, `machines`.`os` AS `os`, `machines`.`ostype` AS `ostype` WHERE ( `machines`.`Architecture` = `architecture`.`ArchitectureID` AND `machines`.`OS` = `os`.`OSID` AND `os`.`OSType` = `ostype`.`OSTypeID` ) ORDER BY `machines`.`Hostname`");

$table->fields = $fields;

if (isset($_GET['remove'])) {
	
	$database = new Database();
	$database->connect();

	$database->remove($table, $_GET['id']);
	$database->disconnect();

	header("Location: ./machines.php");

} else if (isset($_GET['update'])) {
	
	$database = new Database();
	$database->connect();
	$options = array();

	// Extract options from parameters.

	foreach ($_GET as $option => $value) {

		if (strncmp($option, "field:", 6) == 0) $options[substr($option,6)] = $value;
	}
	$database->update($table, $options);
	$database->disconnect();

	header("Location: ./machines.php");

} else if (isset($_GET['insert'])) {
	
	$database = new Database();
	$database->connect();
	$options = array();

	// Extract options from parameters.

	foreach ($_GET as $option => $value) {

		if (strncmp($option, "field:", 6) == 0) $options[substr($option,6)] = $value;
	}
	$database->insert($table, $options);
	$database->disconnect();

	header("Location: ./machines.php");

} else if (isset($_GET['edit'])) {

	// Show form

	$webform = new WebForm($table, $_GET['id']);
	$webform->printHTML();

} else if (isset($_GET['new'])) {

	// Show form

	$webform = new WebForm($table);
	$webform->printHTML();

} else {

	$webtable = new WebTable($table);
	$webtable->printHTML();
}

?>