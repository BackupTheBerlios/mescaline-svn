<?php

if (!class_exists("Field")) include("field.inc");

class IDField extends Field {

	function toString() {

		return $this->name . " (ID Field)";	
	}
}

?>