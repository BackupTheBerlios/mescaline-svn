<?php

require_once("listfield.inc");
require_once("whereclause.inc");
require_once("wherecombination.inc");

class Table {
	
	public $name;
	public $fields;	
	public $hidden;
	public $sort;
	public $whereclause;

	function __construct($name) {

		$this->name = $name;
		$this->hidden = 0;
		$this->sort = 0;
		$this->whereclause = null;
	}

	public function toggleHidden($i) {

		return $this->hidden ^ (1 << $i);
	}

	public function isHidden($i) {

		return ($this->hidden >> $i) & 1;
	}

	public function buildQuery($database, $sort = null, $id = null) {

		// Create lists of reference fields and used tables.

		$referencefields = array();
		$tables = array();
		$tables[] = $this;
		
		$tablecount = 1;

		foreach($this->fields as $i => $field) if ($field instanceof ListField) {
		
			$tables[] = $field->reference->table;
			$referencefields[$i] = $field; // set to $i because of the isHidden function below.
			$tablecount++;
		}
		
		// Create query
		
		$query = "SELECT ";
		
		$tablecount = 1;

		foreach($this->fields as $i => $field) { 

			if (!$this->isHidden($i)) {

				if ($field instanceof ListField) {
					
					$query .= "`referencetable" 
						. mysql_real_escape_string($tablecount) .  "`.`" 
						. mysql_real_escape_string($field->reference->displayfield->name) . "` AS `" 
						. mysql_real_escape_string($field->name) . "`,";
					$tablecount++;
				} else $query .= "`" 
					. mysql_real_escape_string($this->name) .  "`.`" 
					. mysql_real_escape_string($field->name) . "`,";
			}
		}

		// Remove trailing comma.
		
		$query = substr($query , 0, strlen($query) - 1);
		
		$query .= " FROM `" 
			. mysql_real_escape_string($database->name) . "`.`" 
			. mysql_real_escape_string($this->name) . "` AS `" 
			. mysql_real_escape_string($this->name) . "`";
		
		$tablecount = 1;

		foreach($referencefields as $i => $field) {

			if (!$this->isHidden($i)) {

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
		}

 		if ($id != null) $query .= " WHERE (`" 
 			. mysql_real_escape_string($this->name) .  "`.`" 
 			. mysql_real_escape_string($this->fields[0]->name) . "` = '" 
 			. mysql_real_escape_string($id) ."')";
		else if ($this->whereclause != null) $query .= " WHERE (" . $this->whereclause->toString() . ")";

		if ($sort != null) $query .= " ORDER BY `" 
			. mysql_real_escape_string($this->name) .  "`.`" 
			. mysql_real_escape_string($this->fields[$sort]->name) . "`";

		return $query;
	}

	function toString() {

		return $this->name;	
	}
}

?>