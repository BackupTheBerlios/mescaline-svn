<?php

require_once("attribute.inc");

class ContextAttribute extends Attribute {

	public $contextname;

	function __construct($name, $contextname, $value) {

		parent::__construct($name, $value);

		$this->contextname = $contextname;
	}

	//public function key() { // TODO: still need that?

	//	return $this->name . ":" . $this->contextname;
	//}
}