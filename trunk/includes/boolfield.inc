<?php

require_once("field.inc");

class BoolField extends Field {

	function toString() {

		return $this->name . " (Bool)";
	}

	function formatValue($value) {

		if ($value == "") return "";
		else if ($value == "0") return "No";
		else return "Yes";
	}
}

?>