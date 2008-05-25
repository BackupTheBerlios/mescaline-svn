<?php

if (!class_exists("Field")) include("field.inc");

class BoolField extends Field {

	function toString() {

		return $this->name . " (Bool)";	
	}	
}

?>