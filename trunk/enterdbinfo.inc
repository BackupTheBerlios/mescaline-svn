<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");

class EnterDBInfo extends WizardStep
{

	protected function caption() {

		return "<b>Step 1</b>: Enter the database configuration.<br><br>";
	}

	protected function content() {
	
		$dark = 1;
	
		$ret .= $this->caption();
		$ret .= "<form action=\"./wizard.php\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<input type=\"hidden\" name=\"dialog\" value=\"" . $this->next->name . "\">";
		
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Context:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"contextname\" value=\"\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Database:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"databasename\" value=\"\"></td></tr>";	
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Hostname:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"hostname\" value=\"\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">User:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"user\" value=\"\"></td></tr>";
	
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Password:</td>";
		$dark = $dark ^ 1;
		$ret .= "<td><input name=\"password\" value=\"\"></td></tr>";
	
		$ret .= "</table>";
	
		return $ret;
	}

	protected function nextButton() {

		if ($this->next) return "<input id=\"next\" type=\"image\" src=\"next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></form>";
		return "";
	}
}

?>