<?php

require_once("webfield.inc");
require_once("stringfield.inc");

class WebStringField extends WebField
{

	function renderFormField($contextname, $hash) {

		return "<td><input name=\"field" 
			. $hash . ":"
			. $contextname 
			. "\" value=\"" 
			. $this->value 
			. "\" size=\""
			. ((($this->field->length > 20) || (strlen($this->value) == 0)) ? "20" : $this->field->length)
			. "\" maxlength=\""
			. $this->field->length
			."\"></td>";
	}
}

?>