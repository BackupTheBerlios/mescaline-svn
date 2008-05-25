<?php

if (!class_exists("Table")) include("table.inc");
if (!class_exists("Field")) include("field.inc");

class WebTable
{
  
	public $context;
	public $sort;

	function __construct($context, $sort = null)
	{

		$this->context = $context;
		$this->sort = $sort;
	} 

	function printHTML() {
	
		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"
			. "<html><head><title>table</title>"
			. "<style type=\"text/css\">"
			. ".brightline { background-color:#FFFFFF; }"
			. ".darkline { background-color:#DDDDDD; }"
			. "</style>"
			. "</head><body><div id=\"tables\" align=\"center\">";
		print $this->buildTable();
		print "</div></body></html>";
	}

	protected function buildTable() {

		$ret = $this->context->database->buildTable($this->context->table, $this->sort);
	
		if ($ret == false) {
	
			return "ERROR: table could not be build from query: " . $this->context->table->buildQuery($this->context->database);
		}

		$ret = "<div id=\"machinetable\"><table border=0 cellspacing=0>";
		
		// create header

		$skipid = 1;
		$ret .= "<tr class=\"darkline\">";
		foreach ($this->context->table->fields as $i => $value) ($skipid ? $skipid = 0 : $ret .= "<td nowrap=\"nowrap\"><a href=\"./index.php?context=" . $this->context->name . "&sort=" . $value->name . "\">" . $value->name . "</a></td>");
		$ret .= "<td><a href=\"./index.php?context=" . $this->context->name . "&new=true\"><img src=\"new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a></td></tr>";
	
		$dark = 0;
	
		foreach ($this->context->table->rows as $i => $row) {
	
			// The first field of each line (the id) is skipped. The last element is an edit link, which embedds the id to edit.
	
			$skipid = 1;
			$ret .= "<tr class=\"" . ($dark ? "darkline" : "brightline") . "\">";
			foreach ($row as $i => $value) {

				if ($this->context->table->fields[$i] instanceof BoolField) {

					if ($value == "") $ret .= "<td nowrap=\"nowrap\"></td>";
					else if ($value == "0") $ret .= "<td nowrap=\"nowrap\">No</td>";
					else $ret .= "<td nowrap=\"nowrap\">Yes</td>";
				}
				else ($skipid ? $skipid = 0 : $ret .= "<td nowrap=\"nowrap\">" . $value . "</td>");
			}		
	
			// Get the id
			$value = reset($row);

			$ret .= "<td nowrap=\"nowrap\"><a href=\"./index.php?context=" . $this->context->name . "&edit=true&id=" . $value . "\"><img src=\"edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a><a onclick=\"return window.confirm('Do you want to delete the entry?')\" href=\"./index.php?context=" . $this->context->name . "&remove=true&id=" . $value . "\"><img src=\"remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a></td></tr>";
			$dark = $dark ^ 1;
		}
		
		$ret .= "</table></div>";
		
		return $ret;
	}
}

?>