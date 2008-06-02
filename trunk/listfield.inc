<?php

if (!class_exists("Reference")) include("reference.inc");
if (!class_exists("Field")) include("field.inc");

class ListField extends Field {
	
	public $reference;
	public $query;

	function __construct($name, $reference, $optional=false) {

		parent::__construct($name, $optional);
		$this->reference = $reference;
	}

	function buildQuery($database) {

		return "SELECT `" 
			. mysql_real_escape_string($this->reference->table->name) . "`.`" 
			. mysql_real_escape_string($this->reference->idfield->name) . "`,`" 
			. mysql_real_escape_string($this->reference->table->name) . "`.`" 
			. mysql_real_escape_string($this->reference->displayfield->name) ."` FROM `" 
			. mysql_real_escape_string($database->name) . "`.`" 
			. mysql_real_escape_string($this->reference->table->name) . "`";
	}

	function toString() {

		return $this->name . " => " . $this->reference->table->name . "." . $this->reference->displayfield->name . " (SQL)";
	}
}

