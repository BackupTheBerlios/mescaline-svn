<?php

require_once("field.inc");

class StringField extends Field {

	public $length;

	function __construct($name, $length, $optional=false) {

		parent::__construct($name, $optional);
		$this->length = $length;
	}

	function toString() {

		return $this->name . " (String)";
	}
}

?>