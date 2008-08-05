<?php

require_once("table.inc");
require_once("field.inc");
require_once("idfield.inc");
require_once("wizardstep.inc");
require_once("database.inc");

class ChooseIDField extends WizardStep {

	public $context;
	public $idfield;

	protected function caption() {

		return "<b>Step 3</b>: Choose the tables ID field.<br><br>";
	}

	public function content() {

		// There has to be a context yet.

		$s = implode("", @file($this->wizardFilename()));
		$this->context = unserialize($s);

		$database = $this->context->database;

		// Either the table is set in the context yet (back button) or the "table" parameter is set. 
		
		if (isset($_GET['table'])) {

			// Get tables and fields.

			$database->connect();
			$tables = $database->tables();
			$table = $tables[$_GET['table']];
			$fields = $database->columns($table);
			$database->disconnect();

			// Set table to context.

			$this->context->table = $table;

			// Make first field the IDField.

			reset($fields);
			$this->idfield = key($fields);

		} else {

			// There has to be an IDField in context yet.

			$fields = $this->context->table->fields;

			// Set idfield from context.

			foreach ($fields as $i => $field) if ($field instanceof IDField) $this->idfield = $i;
		}
	
		// Get (fresh) fields.

		$database->connect();
		$fields = $database->columns($this->context->table);
		$database->disconnect();

		// Put fields to context (overwrite old).

		$this->context->table->fields = $fields;

		// Write context.

		$s = serialize($this->context);
		$fp = fopen($this->wizardFilename(), "w");
		fputs($fp, $s);
		fclose($fp);

		$dark = 1;

		$ret .= $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		
		foreach ($fields as $i => $field) { 
			
			// If current field is the one in the context, select the entry.

			$selected = ($i == $this->idfield) ? " checked=\"checked\"" : "";

			$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\"" . $selected . " name=\"idfield\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
			$dark = $dark ^ 1;
		}
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"../icons/next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./index.php?dialog=" . $this->previous->name . "&context=" . $this->context->name ."&back=true\"><img src=\"../icons/previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}
}

?>
