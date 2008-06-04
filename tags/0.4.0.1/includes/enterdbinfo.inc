<?php

require_once("wizardstep.inc");

class EnterDBInfo extends WizardStep
{

	protected function caption() {

		return "<b>Step 1</b>: Enter the database configuration.<br><br>";
	}

	protected function content() {
	
		// At back button, load context, fill in the values, and delete context file.

		if (isset($_GET['context'])) {

			$s = implode("", @file($this->wizardFilename()));
			$context = unserialize($s);

			$databasename = $context->database->name;
			$hostname = $context->database->hostname;
			$user = $context->database->user;
			$password = $context->database->password;
			$contextname = $context->name;

			unlink($contextfile);
		}

		$dark = 1;
	
		$ret .= $this->caption();
		$ret .= "<form action=\"./index.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Context:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"contextname\" value=\"" . $contextname ."\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Database:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"databasename\" value=\"" .  $databasename . "\"></td></tr>";	
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Hostname:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"hostname\" value=\"" . $hostname . "\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">User:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"user\" value=\"" . $user . "\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Password:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input type=\"password\" name=\"password\" value=\"" . $password ."\"></td></tr>";
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"../icons/next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}
}

?>