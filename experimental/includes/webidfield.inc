<?php

require_once("webfield.inc");
require_once("idfield.inc");

class WebIDField extends WebField
{
	function renderTableField() {
	}

	function renderTableHeader($contextname) {
		
		return "";
	}

	function renderFormName() {

		return "";
	}

	function renderFormField($contextname, $hash) {

		return "<input type=\"hidden\" name=\"ptr:" . $hash . "\" value=\"" . $this->value ."\">";
	}
}

?>