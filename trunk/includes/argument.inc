<?php

class Argument {

	public $name;
	public $value;

	function __construct($name, $contextname, $value) {

		$this->name = $name;
		$this->contextname = $contextname;
		$this->value = $value;
	}

}

?>