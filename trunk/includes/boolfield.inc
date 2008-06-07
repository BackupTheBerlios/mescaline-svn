<?php

require_once("field.inc");

class BoolField extends Field {

	function toString() {

		return $this->name . " (Bool)";
	}
}

?>