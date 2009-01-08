<?php

require_once("webfield.inc");
require_once("listfield.inc");
require_once("database.inc");

class WebListField extends WebField
{

	function __construct($field, $database) {

		$this->field = $field;
		$this->database = $database;
	}

// Build list of listfield

	function renderFormField($contextname, $hash, $new = false) {

		$list = $this->database->buildListField($this->field);
	
		$ret = "<td><select name=\"ptr:" . $hash . "\" >";
	
		// If the field is optional, add a blank field on the top.
	
		if ($this->field->optional) {
	
			$selected = ($this->value == "") ? "selected=\"selected\" " : "";
			$ret .= "<option " . $selected . "value=\"\">NULL</<option>";
		}
	
		// Make the rows ID field the value of the option and display the name. Select the first entry, when creating new entries.
	
		if (!$new) {
	
			foreach ($list as $i => $listrow) {
	
				$selected = ($this->value == $listrow[1]) ? "selected=\"selected\" " : "";			
				$ret .= "<option " . $selected . "value=\"" . $listrow[0] . "\">" . $listrow[1] . "</option>";
			}
		} else {
	
			$selectfirst = true;
	
			foreach ($list as $i => $listrow) {
	
				$selected = ($selectfirst) ? "selected=\"selected\" " : "";			
				$ret .= "<option " . $selected . "value=\"" . $listrow[0] . "\">" . $listrow[1] . "</option>";
				$selectfirst = false;
			}				
		}
	
		$ret = $ret . "</select></td>";

		return $ret;
	}

}

?>