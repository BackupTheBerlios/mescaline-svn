<?php

require_once("table.inc");
require_once("field.inc");
require_once("idfield.inc");
require_once("listfield.inc");
require_once("wizardstep.inc");
require_once("database.inc");

class EditReferences extends WizardStep {

	public $context;
	public $errors;

	function testQuery($database, $query) {
	
		$database->connect();
		$result = mysql_query($query, $database->link);
		$database->disconnect();
	
		if ($result == false) return false;
		else return true;
	}

	protected function caption() {

		return "<b>Step 5</b>: Edit fields which reference other tables.<br><br>";
	}

	public function content() {
		
		$s = implode("", @file($this->wizardFilename()));
		$this->context = unserialize($s);

		$this->errors = array();

		if (isset($_GET['back'])) {

		} else if (isset($_GET['reference'])) { 
	
			// Set the query to the context.

			$query = $_GET['query'];

			$this->context->table->fields[$_GET['field']]->query = $query;

			$action = $_GET['reference'];

			if ($action == "check") {
				
				$field = $this->context->table->fields[$_GET['field']];

				// When the query isn't working set the correspondent error field.

				if ($this->testQuery($this->context->database, $query) == false) 
					$this->errors[$_GET['field']] = true;
			} else if ($action == "remove") {

				// Get fields;

				$database = $this->context->database;
				$database->connect();
				$fields = $database->columns($this->context->table);
				$database->disconnect();

				$newfield = $fields[$_GET['field']];
				$oldfield = $this->context->table->fields[$_GET['field']];
			
				$this->context->table->fields = replaceElement($oldfield, $newfield, $this->context->table->fields);		
			}
		} else {

			// Put fields which were into selection array, if a non-optional field was deselected
			// disable new entry functionality.
	
			$selection = array();
			$allownew = true;

			foreach ($this->context->table->fields as $i => $field) { 
				if (isset($_GET[$i])) $selection[] = $field;
				else if ($field->optional == false) $allownew = false;
			}
			$this->context->table->fields = $selection;
			$this->context->allow_new = $allownew;
		}

		// Write context to disk.

		$s = serialize($this->context);
		$fp = fopen($this->wizardFilename(), "w");
		fputs($fp, $s);
		fclose($fp);

		$dark = 1;
	
		$ret = $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
	
		foreach ($this->context->table->fields as $i => $field) { 
			
			if (isset($this->errors[$i])) $string = "<font color=\"red\">" . $field->toString() . "</font>";
			else $string = $field->toString();

			if (($field instanceof IDField) == false) { 
				$ret .= "<tr nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><td>" . $string . "</td><td><a href=\"./index.php?context=" . $this->context->name . "&dialog=choosereferencetable&field=" . $i ."\"><img src=\"../icons/edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a>";
				if ($field instanceof ListField) $ret .= "<a href=\"./index.php?context=" . $this->context->name . "&dialog=editreferences&reference=remove&field=" . $i ."\"><img src=\"../icons/remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"remove\"></a>";
				$ret .= "</td></tr>";
				$dark = $dark ^ 1;
			}
		}
	
		$ret .= "</table>";

		if (sizeof($this->errors) > 0) $ret .= "<br><font color=\"red\">*</font> SQL query has errors.";

		return $ret;
	}

	function replaceElement($old, $new, $array) {

		foreach ($array as $i => $element) if ($old == $element) $array[$i] = $new;

		return $array;
	}

	protected function nextButton() {

		if (sizeof($this->errors) > 0) return "";

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"../icons/next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./index.php?dialog=" . $this->previous->name . "&context=" . $this->context->name ."&back=true\"><img src=\"../icons/previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}
}

?>
