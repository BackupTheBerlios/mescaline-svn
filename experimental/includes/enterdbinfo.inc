<?php

require_once("wizardstep.inc");

class EnterDBInfo extends WizardStep
{
	private $databasename;
	private $hostname;
	private $user;
	private $password;
	private $contextname;

	protected function caption() {

		return "<b>Step 1</b>: Enter the database configuration.<br><br>";
	}

	protected function content() {
	
		// At back button, load context, fill in the values, and delete context file.

		if (isset($_GET['context'])) {

			$s = implode("", @file($this->wizardFilename()));
			$context = unserialize($s);

			$this->databasename = $context->database->name;
			$this->hostname = $context->database->hostname;
			$this->user = $context->database->user;
			$this->password = $context->database->password;
			$this->contextname = $context->name;

			unlink($this->wizardFilename());
		}
	
		$ret = $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		
		$ret .= "<tr><td nowrap=\"nowrap\">Context:</td>";
		$ret .= "<td><input name=\"contextname\" value=\"" . $this->contextname ."\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\">Database:</td>";
		$ret .= "<td><input name=\"databasename\" value=\"" .  $this->databasename . "\"></td></tr>";	
	
		$ret .= "<tr><td nowrap=\"nowrap\">Hostname:</td>";
		$ret .= "<td><input name=\"hostname\" value=\"" . $this->hostname . "\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\">User:</td>";
		$ret .= "<td><input name=\"user\" value=\"" . $this->user . "\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\">Password:</td>";
		$ret .= "<td><input type=\"password\" name=\"password\" value=\"" . $this->password ."\"></td></tr>";
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"../icons/next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}
}

?>