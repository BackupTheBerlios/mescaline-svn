<?php

class Attribute {

	public $name;
	public $value;

	function __construct($name, $value) {

		$this->name = $name;
		$this->value = $value;
	}

	public function key() {

		return $this->name;
	}
}

?>