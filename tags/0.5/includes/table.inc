<?php

require_once("listfield.inc");

class Table {
	
	public $name;
	public $fields;	

	function __construct($name) {

		$this->name = $name;
	}

	function buildQuery($database, $sort = null, $id = null) {

		// Create lists of reference fields and used tables.

		$referencefields = array();
		$tables = array();
		$tables[] = $this;
		
		$tablecount = 1;

		foreach($this->fields as $i => $field) if ($field instanceof ListField) {
		
			$tables[] = $field->reference->table;
			$referencefields[] = $field;
			$tablecount++;
		}
		
		// Create query
		
		$query = "SELECT ";
		
		$tablecount = 1;

		foreach($this->fields as $i => $field) { 

			if ($field instanceof ListField) {
				
				$query .= "`referencetable" 
					. mysql_real_escape_string($tablecount) .  "`.`" 
					. mysql_real_escape_string($field->reference->displayfield->name) . "` AS `" 
					. mysql_real_escape_string($field->name) . "`,";
				$tablecount++;
			}
			else $query .= "`" 
				. mysql_real_escape_string($this->name) .  "`.`" 
				. mysql_real_escape_string($field->name) . "`,";
		}

		// Remove trailing comma.
		
		$query = substr($query , 0, strlen($query) - 1);
		
		$query .= " FROM `" 
			. mysql_real_escape_string($database->name) . "`.`" 
			. mysql_real_escape_string($this->name) . "` AS `" 
			. mysql_real_escape_string($this->name) . "`";
		
		$tablecount = 1;

		foreach($referencefields as $i => $field) {
		
			$query .= " LEFT JOIN `" 
				. mysql_real_escape_string($database->name) . "`.`" 
				. mysql_real_escape_string($field->reference->table->name) . "` AS `referencetable"
				. mysql_real_escape_string($tablecount) . "` ON ( `" 
				. mysql_real_escape_string($this->name) . "`.`" 
				. mysql_real_escape_string($field->name) . "` = `referencetable" 
				. mysql_real_escape_string($tablecount) . "`.`" 
				. mysql_real_escape_string($field->reference->idfield->name) . "`)";	

			$tablecount++;
		}

		if ($id != null) $query .= " WHERE (`" 
			. mysql_real_escape_string($this->name) .  "`.`" 
			. mysql_real_escape_string($this->fields[0]->name) . "` = '" 
			. mysql_real_escape_string($id) ."')";

		if ($sort != null) $query .= " ORDER BY `" 
			. mysql_real_escape_string($sort) . "`";

		return $query;
	}

	function toString() {

		return $this->name;	
	}
}

?>