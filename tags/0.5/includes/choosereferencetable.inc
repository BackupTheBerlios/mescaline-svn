<?php

require_once("choosetable.inc");

class ChooseReferenceTable extends ChooseTable {

	public $table;

	protected function caption() {

		return "<b>Step 5/1</b>: Choose referenced table.<br><br>";
	}

	public function content() {

		// There has to be a context yet.

		$s = implode("", @file($this->wizardFilename()));
		$this->context = unserialize($s);

		// Get database.

		$database = $this->context->database;

		// Get tables.

		$database->connect();
		$tables = $database->tables();
		$database->disconnect();

		// Either the table parameter ist set (back button) or not. 

		if (isset($_GET['table'])) $this->table = $tables[$_GET['table']];
		else $this->table = reset($tables);

		$dark = 1;
	
		$ret .= $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $_GET['field'] . "\">";		

		foreach ($tables as $i => $table) { 

			// If current table is the one in the context, select the entry.

			$selected = ($table == $this->table) ? " checked=\"checked\"" : "";

			$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\"" . $selected . " name=\"table\" value=\"" . $i . "\">" . $table->toString() . "</td></tr>";
			$dark = $dark ^ 1;
		}
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./index.php?dialog=" . $this->previous->name . "&context=" . $this->context->name ."&back=true\"><img src=\"../icons/previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}
}

?>