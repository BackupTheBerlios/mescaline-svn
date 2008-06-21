<?php

require_once("reference.inc");
require_once("field.inc");

class ListField extends Field {
	
	public $reference;
	public $query;

	function __construct($name, $reference, $optional=false) {

		parent::__construct($name, $optional);
		$this->reference = $reference;
	}

	function buildQuery($database) {

		return "SELECT `" 
			. $this->reference->table->name . "`.`"
			. $this->reference->idfield->name . "`,`" 
			. $this->reference->table->name . "`.`" 
			. $this->reference->displayfield->name . "` FROM `" 
			. $database->name . "`.`" 
			. $this->reference->table->name . "` ORDER BY `"
			. $this->reference->table->name . "`.`" 
			. $this->reference->displayfield->name . "`";
	}

	function toString() {

		return $this->name . " => " . $this->reference->table->name . "." . $this->reference->displayfield->name . " (SQL)";
	}
}

