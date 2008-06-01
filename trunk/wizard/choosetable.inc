<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("Database")) include("../database.inc");
if (!class_exists("WebContext")) include("../webcontext.inc");
if (!class_exists("Table")) include("../table.inc");

class ChooseTable extends WizardStep
{

	public $context;

	protected function caption() {

		return "<b>Step 2</b>: Choose a table.<br><br>";
	}

	public function content() {

		// check whether we have a context yet, if not create one. 

		if (isset($_GET['context'])) {

			$contextfile =  $_GET['context'] . ".wizard";
			$s = implode("", @file($contextfile));
			$this->context = unserialize($s);
		} else {

			$contextname = $_GET['contextname'];
			$databasename = $_GET['databasename'];
			$hostname = $_GET['hostname'];
			$user = $_GET['user'];
			$password = $_GET['password'];
	
			$database = new Database($databasename, $hostname, $user, $password);
			$this->context = new WebContext($contextname, null, $database); 
		
			$s = serialize($this->context);
			$fp = fopen($this->context->name . ".wizard", "w");
			fputs($fp, $s);
			fclose($fp);
		}

		// Get table list.

		$database = $this->context->database;

		$database->connect();
		$tables = $database->tables();
		$database->disconnect();

		// If there is no table set in the context yet, set the first one.

		if ($this->context->table == null) $this->context->table = reset($tables);

		$dark = 1;
	
		$ret .= $this->caption();
		$ret .= "<form action=\"./wizard.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";	
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		
		foreach ($tables as $i => $table) { 

			// If current table is the one in the context, select the entry.

			$selected = ($this->context->table->name == $table->name) ? " checked=\"checked\"" : "";

			$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\"" . $selected . " name=\"table\" value=\"" . $i . "\">" . $table->toString() . "</td></tr>";
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

		if ($this->previous) return "<a id=\"previous\"href=\"./wizard.php?dialog=" . $this->previous->name . "&context=" . $this->context->name ."&back=true\"><img src=\"../icons/previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}
}

?>