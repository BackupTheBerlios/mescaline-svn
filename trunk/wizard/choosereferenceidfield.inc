<?php

if (!class_exists("ChooseIDField")) include("chooseidfield.inc");

class ChooseReferenceIDField extends ChooseIDField {

	public $table;

	protected function caption() {

		return "<b>Step 5/2</b>: Select the referenced tables id field.<br><br>";
	}

	public function content() {

		// There has to be a context yet, and table has to be set.

		$s = implode("", @file($this->wizardFilename()));
		$this->context = unserialize($s);

		$database = $this->context->database;

		// Get tables and fields

		$database->connect();
		$tables = $database->tables();
		$this->table = $tables[$_GET['table']];
		$fields = $database->columns($this->table);
		$database->disconnect();

		// Either the idfield parameter ist set (back button) or not. 

		if (isset($_GET['idfield'])) $this->idfield = $fields[$_GET['idfield']];
		else $this->idfield = reset($fields);

		$dark = 1;

		$ret .= $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $_GET['field'] . "\">";
		$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $_GET['table'] . "\">";
		
		
		foreach ($fields as $i => $field) { 
			
			// If current field is the one in the context, select the entry.

			$selected = ($field == $this->idfield) ? " checked=\"checked\"" : "";

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

		// Send back table parameter

		if ($this->previous) return "<a id=\"previous\"href=\"./index.php?dialog=" . $this->previous->name 
			. "&context=" . $this->context->name 
			. "&field=" . $_GET['field']
			. "&table=" . $_GET['table'] 
			. "&back=true\"><img src=\"../icons/previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}

}

?>