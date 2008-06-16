<?php

require_once("webfield.inc");
require_once("idfield.inc");

class WebIDField extends WebField
{
	function renderTableField() {

		return "<a name=\"" . $this->value . "\"></a>";
	}

	function renderTableHeader($contextname) {
		
		return "";
	}

	function renderFormName() {

		return "";
	}

	function renderFormField($hash) {

		return "<input type=\"hidden\" name=\"field:" . $hash . "\" value=\"" . $this->value ."\">";
	}
}

?>