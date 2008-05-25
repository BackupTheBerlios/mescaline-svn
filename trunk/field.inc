<?php

if (!class_exists("Table")) include("table.inc");

class Field {
	
	public $name;
	public $optional;

	function __construct($name, $optional=false) {

		$this->name = $name;
		$this->optional = $optional;
	}

	function toString() {

		return $this->name . " (Default)";	
	}
}

?>