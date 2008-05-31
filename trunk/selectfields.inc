<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("Database")) include("database.inc");
if (!class_exists("WebContext")) include("webcontext.inc");
if (!class_exists("Table")) include("table.inc");
if (!class_exists("Field")) include("field.inc");
if (!class_exists("IDField")) include("idfield.inc");

class SelectFields extends WizardStep {

	public $context;
	public $selection;

	protected function caption() {

		return "<b>Step 4</b>: Select the needed columns.<br><br>";
	}

	public function content() {

		// There has to be a context yet.

		$contextfile = $_GET['context'] . ".wizard";
		$s = implode("", @file($contextfile));
		$this->context = unserialize($s);

		// Create Selection array

		$this->selection = array();
	
		// Either the idfield is set in the context yet (back button) or the "idfield" parameter is set. 
		
		if (isset($_GET['idfield'])) {

			// Replace selected field with an IDField object.

			$fields = $this->context->table->fields;
			$field = $this->context->table->fields[$_GET['idfield']];
			$idfield = new IDField($field->name);
			$this->context->table->fields = $this->replaceElement($field, $idfield , $fields);

			// Put ID field in selection.

			$this->selection[] = $idfield;

		} else { // back button 

			// Get selection.

			$this->selection = $this->context->table->fields;

			// Get all fields and set them to context.

			$database = $this->context->database;
			$database->connect();
			$this->context->table->fields = $database->columns($this->context->table);
			$database->disconnect();

			// Find id field in selection and add it to context fields.

			foreach ($this->selection as $i => $field) if ($field instanceof IDField) $this->context->table->fields[$i] = $field;
		}

		// Write context to disk

		$s = serialize($this->context);
		$fp = fopen($this->context->name . ".wizard", "w");
		fputs($fp, $s);
		fclose($fp);

		$dark = 1;
	
		$ret .= $this->caption();
		$ret .= "<form action=\"./wizard.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
	
		foreach ($this->context->table->fields as $i => $field) { 
			
			// If the field exists in the selection array, the box is checked.

			$selected = (in_array($field, $this->selection)) ? " checked=\"checked\"" : "";

			// Disabled HTML checkboxes send no value obviously, so we have to do it manually.

			if ($field instanceof IDField) $ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input disabled=\"disabled\"" . $selected . " type=\"checkbox\"><input type=\"hidden\" name=\"" . $i . "\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
			else $ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"checkbox\" name=\"" . $i . "\" value=\"" . $i . "\"" . $selected . ">" . $field->toString() . "</td></tr>";
			$dark = $dark ^ 1;
		}
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./wizard.php?dialog=" . $this->previous->name . "&back=true&context=" . $this->context->name ."\"><img src=\"previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}

	private function replaceElement($old, $new, $array) {

		foreach ($array as $i => $element) if ($old == $element) $array[$i] = $new;

		return $array;
	}

}

?>