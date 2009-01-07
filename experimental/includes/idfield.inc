<?php

require_once("field.inc");

class IDField extends Field {

	function toString() {

		return $this->name . " (ID Field)";	
	}
}

?>