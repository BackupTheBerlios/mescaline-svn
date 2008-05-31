<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("Table")) include("table.inc");
if (!class_exists("IDField")) include("idfield.inc");
if (!class_exists("ListField")) include("listfield.inc");
if (!class_exists("Reference")) include("reference.inc");

class EditQuery extends WizardStep {

	public $context;

	protected function caption() {

		return "<b>Step 5/4</b>: Edit the list fields SQL query.<br><br>";
	}

	protected function content() {

		// The dialog cannot be reached by back button!
		
		$contextfile = $_GET['context'] . ".wizard";
	
		$s = implode("", @file($contextfile));
		$this->context = unserialize($s);

		$database = $this->context->database;
	
		// Get fields from referenced table. 

		$database->connect();
		$tables = $database->tables();
		$table = $tables[$_GET['table']];
		$fields = $database->columns($table);

		// Create reference.

		$idfield = $fields[$_GET['idfield']];
		$displayfield = $fields[$_GET['displayfield']];
		$reference = new Reference($table, $idfield, $displayfield);
	
		// Get fields from main table.

		$fields = $database->columns($this->context->table);
		$database->disconnect();

		// Create new listfield.

		$oldfield = $fields[$_GET['field']];
		$newfield = new Listfield($oldfield->name, $reference, $oldfield->optional);

		// update query.

		$newfield->query = $newfield->buildQuery($database);

		// Replace old field with new listfield.

		$this->context->table->fields = $this->replaceElement($oldfield, $newfield, $fields);
	
		// Save the context

		$s = serialize($this->context);
		$fp = fopen($this->context->name . ".wizard", "w");
		fputs($fp, $s);
		fclose($fp);
	
		$dark = 1;
	
		$ret .= "<p><b>Step 5.4</b>: Edit the list fields SQL query" . ($error ? "<font color=\"red\"> (The custom query has errors).</font>" : "." ) . "</p>";
		$ret .= "<form action=\"./wizard.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
		$ret .= "<input type=\"hidden\" name=\"step\" value=\"5_5\">";
		$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $fieldindex . "\">";		 
		$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $tableindex . "\">";		
		$ret .= "<input type=\"hidden\" name=\"idfield\" value=\"" . $tableindex . "\">";
		$ret .= "<input type=\"hidden\" name=\"displayfield\" value=\"" . $tableindex . "\">";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">". $newfield->toString() . "</td>";
		$ret .= "<td><input name=\"query\" value=\"" . $newfield->query . "\"></td></tr>";
	
		$ret .= "</table><br>";
		$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";
	
		return $ret;
	}

	function replaceElement($old, $new, $array) {

		foreach ($array as $i => $element) if ($old == $element) $array[$i] = $new;

		return $array;
	}
}