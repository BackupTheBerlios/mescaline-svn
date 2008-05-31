<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("Database")) include("database.inc");
if (!class_exists("WebContext")) include("webcontext.inc");
if (!class_exists("Table")) include("table.inc");
if (!class_exists("Field")) include("field.inc");
if (!class_exists("IDField")) include("idfield.inc");

class EditReferences extends WizardStep {

	public $context;

	protected function caption() {

		return "<b>Step 5</b>: Edit fields which reference other tables.<br><br>";
	}

	public function content() {
		
		$contextfile = $_GET['context'] . ".wizard";
		$s = implode("", @file($contextfile));
		$this->context = unserialize($s);

		if (isset($_GET['back'])) {

		} else {
	
			$selection = array();
			foreach ($this->context->table->fields as $i => $field) if (isset($_GET[$i])) $selection[] = $field;
			$this->context->table->fields = $selection;
		
			$s = serialize($this->context);
			$fp = fopen($this->context->name . ".wizard", "w");
			fputs($fp, $s);
			fclose($fp);
		}

		$dark = 1;
	
		$ret .= $this->caption();
		$ret .= "<form action=\"./wizard.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
	
		foreach ($this->context->table->fields as $i => $field) { 
			
			if (($field instanceof IDField) == false) { 
				$ret .= "<tr nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><td>" . $field->toString() . "</td><td><a href=\"./wizard.php?context=" . $this->context->name . "&dialog=choosereferencetable&field=" . $i ."\"><img src=\"edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a></td></tr>";
				$dark = $dark ^ 1;
			}
		}
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./wizard.php?dialog=" . $this->previous->name . "&context=" . $this->context->name ."&back=true\"><img src=\"previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}
}

?>