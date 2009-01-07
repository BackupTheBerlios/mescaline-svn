<?php

if (!class_exists("IDField")) include("../idfield.inc");
if (!class_exists("ListField")) include("../listfield.inc");
if (!class_exists("Reference")) include("../reference.inc");
if (!class_exists("Table")) include("../table.inc");
if (!class_exists("WizardStep")) include("wizardstep.inc");

class EditQuery extends WizardStep {

	public $context;

	protected function caption() {

		return "<b>Step 5/4</b>: Edit the list fields SQL query.<br><br>";
	}

	protected function content() {

		// The dialog cannot be reached by back button!
		
		$s = implode("", @file($this->wizardFilename()));
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
	
		// Get fields from context

		$fields = $this->context->table->fields;
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
		$fp = fopen($this->wizardFilename(), "w");
		fputs($fp, $s);
		fclose($fp);
	
		$dark = 1;
	
		$ret = $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $_GET['field'] . "\">";		 
		$ret .= "<input type=\"hidden\" name=\"reference\" value=\"check\">";		
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">". $newfield->toString() . "</td>";
		$ret .= "<td><input name=\"query\" value=\"" . $newfield->query . "\"></td></tr>";
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"../icons/next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}

	protected function previousButton() {

		// Send back table parameter

		if ($this->previous) return "<a id=\"previous\"href=\"./index.php" 
			. "?dialog=" . $this->previous->name 
			. "&context=" . $this->context->name
			. "&field=" . $_GET['field'] 
			. "&table=" . $_GET['table']
			. "&idfield=" . $_GET['idfield'] 
			. "&displayfield=" . $_GET['displayfield']
			. "&back=true\"><img src=\"../icons/previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}

	function replaceElement($old, $new, $array) {

		foreach ($array as $i => $element) if ($old == $element) $array[$i] = $new;

		return $array;
	}
}