<?php

class Argument { // TODO: remove.

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