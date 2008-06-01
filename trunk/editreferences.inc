<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("Database")) include("database.inc");
if (!class_exists("WebContext")) include("webcontext.inc");
if (!class_exists("Table")) include("table.inc");
if (!class_exists("Field")) include("field.inc");
if (!class_exists("IDField")) include("idfield.inc");
if (!class_exists("ListField")) include("listfield.inc");

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
		
		$contextfile = $_GET['context'] . ".wizard";
		$s = implode("", @file($contextfile));
		$this->context = unserialize($s);

		$this->errors = array();

		if (isset($_GET['reference'])) { // back button
	
			// Set the query to the context.

			$this->context->table->fields[$_GET['field']]->query = $_GET['query'];

			$action = $_GET['reference'];

			if ($action == "check") {
				
				$field = $this->context->table->fields[$_GET['field']];

				// When the query isn't working set the correspondent error field.

				if ($this->testQuery($this->context->database, $_GET['query']) == false) 
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
	
			$selection = array();
			foreach ($this->context->table->fields as $i => $field) if (isset($_GET[$i])) $selection[] = $field;
			$this->context->table->fields = $selection;
		}

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
			
			if (isset($this->errors[$i])) $string = "<font color=\"red\">" . $field->toString() . "</font>";
			else $string = $field->toString();

			if (($field instanceof IDField) == false) { 
				$ret .= "<tr nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><td>" . $string . "</td><td><a href=\"./wizard.php?context=" . $this->context->name . "&dialog=choosereferencetable&field=" . $i ."\"><img src=\"edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a>";
				if ($field instanceof ListField) $ret .= "<a href=\"./wizard.php?context=" . $this->context->name . "&dialog=editreferences&reference=remove&field=" . $i ."\"><img src=\"remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"remove\"></a>";
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

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./wizard.php?dialog=" . $this->previous->name . "&context=" . $this->context->name ."&back=true\"><img src=\"previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}
}

?>