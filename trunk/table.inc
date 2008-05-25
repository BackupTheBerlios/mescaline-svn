<?php

if (!class_exists("ListField")) include("listfield.inc");

class Table {
	
	public $name;
	public $fields;	
	public $rows;

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
				
				$query .= "`referencetable" . $tablecount .  "`.`" . $field->reference->displayfield->name . "` AS `" . $field->name . "`,";
				$tablecount++;
			}
			else $query .= "`" . $this->name .  "`.`" . $field->name . "`,";
		}

		// Remove trailing comma.
		
		$query = substr($query , 0, strlen($query) - 1);
		
		$query .= " FROM `" . $database->name . "`.`" . $this->name . "` AS `" . $this->name . "`";
		
		$tablecount = 1;

		foreach($referencefields as $i => $field) {
		
			$query .= " LEFT JOIN `" . $database->name . "`.`" . $field->reference->table->name . "` AS `referencetable" . $tablecount . "` ON ( `" . $this->name . "`.`" . $field->name . "` = `referencetable" . $tablecount . "`.`" . $field->reference->idfield->name . "`)";	

			$tablecount++;
		}

		if ($id != null) $query .= " WHERE (`" . $this->name .  "`.`" . $this->fields[0]->name . "` = '" . $id ."')";

		if ($sort != null) $query .= " ORDER BY `" . $sort . "`";

		return $query;
	}

	function toString() {

		return $this->name;	
	}
}

?>