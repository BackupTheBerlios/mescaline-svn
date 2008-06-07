<?php

require_once("webfield.inc");
require_once("idfield.inc");

class WebIDField extends WebField
{
	function renderTableField() {

		return "";
	}

	function renderTableHeader($contextname) {
		
		return "";
	}

	function renderFormName($dark) {

		return "";
	}

	function renderFormField($hash) {

		return "<input type=\"hidden\" name=\"field:" . $hash . "\" value=\"" . $this->value ."\">";
	}
}

?>