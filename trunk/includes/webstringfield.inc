<?php

require_once("webfield.inc");
require_once("stringfield.inc");

class WebStringField extends WebField
{

	function renderFormField($hash) {

		return "<td><input name=\"field:" 
			. $hash 
			. "\" value=\"" 
			. $this->value 
			. "\" size=\""
			. ((($this->field->length <= 20) || (strlen($this->value) == 0)) ? $this->field->length : "20")
			. "\" maxlength=\""
			. $this->field->length
			."\"></td>";
	}
}

?>