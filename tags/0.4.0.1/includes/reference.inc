<?php

require_once("field.inc");

class Reference {

	public $idfield;
	public $displayfield;
	public $table;

	function __construct($table, $idfield, $displayfield) {

		$this->idfield = $idfield;
		$this->displayfield = $displayfield;
		$this->table = $table;
	}
}