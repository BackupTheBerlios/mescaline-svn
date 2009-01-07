<?php

require_once("argument.inc");

class ContextAttribute extends Argument {

	public $contextname;

	function __construct($name, $contextname, $value) {

		$this->name = $name;
		$this->contextname = $contextname;
		$this->value = $value;
	}

	public function key() {

		return $this->name . ":" . $this->contextname;
	}
}