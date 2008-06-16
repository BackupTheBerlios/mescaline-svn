<?php

require_once("webfield.inc");
require_once("boolfield.inc");

class WebBoolField extends WebField
{

	function renderFormField($hash) {

		$ret = "<td><select name=\"field:" . $hash . "\" >";	

		// If the field is optional, add a blank field on the top.

		if ($this->field->optional) $ret .= "<option " . (($this->value == "") ? "selected=\"selected\" " : "") . "value=\"\">NULL</<option>";
		$ret .= "<option " . (($this->value == "1") ? "selected=\"selected\" " : "") . "value=\"1\">Yes</option>";
		$ret .= "<option " . (($this->value == "0") ? "selected=\"selected\" " : "") . "value=\"0\">No</option>";

		$ret = $ret . "</select></td>";

		return $ret;
	}

	function formatValue($value) {

		if ($value == "") return "";
		else if ($value == "0") return "No";
		else return "Yes";
	}
}

?>