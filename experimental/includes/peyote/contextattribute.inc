<?php

require_once("attribute.inc");

class ContextAttribute extends Attribute {

	public $contextname;

	function __construct($name, $contextname, $value) {

		parent::__construct($name, $value);

		$this->contextname = $contextname;
	}
}

?>