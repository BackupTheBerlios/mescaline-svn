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
			. "#container { float: left; position: relative; left: 50%; margin-top: 1em; }"
			. "#inner { padding: 3px; position: relative; left: -50%; border: 1px solid black; }"
			. "</style>"
			. "</head><body><div id=\"container\">";
		print $this->buildTable();
		print "</div></body></html>";
	}

	protected function buildTable() {

		$ret = $this->context->database->buildTable($this->context->table, $this->sort);

		if ($ret == false) {

			return "ERROR: table could not be build from query: " . $this->context->table->buildQuery($this->context->database);
		}

		$ret = "<div id=\"inner\"><table border=0 cellspacing=0>";

		// create header

		$skipid = 1;
		$ret .= "<tr class=\"darkline\">";
		foreach ($this->context->table->fields as $i => $value) ($skipid ? $skipid = 0 : $ret .= "<td nowrap=\"nowrap\"><a href=\"./index.php?context=" . $this->context->name . "&sort=" . $value->name . "\">" . $value->name . "</a></td>");
		$ret .= "<td><a href=\"./index.php?context=" . $this->context->name . "&new=true\"><img src=\"./icons/new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a></td></tr>";

		$dark = 0;

		foreach ($this->context->table->rows as $i => $row) {

			// The first field of each line (the id) is skipped. The last element is an edit link, which embedds the id to edit.

			$skipid = 1;
			$ret .= "<tr class=\"" . ($dark ? "darkline" : "brightline") . "\">";
			foreach ($row as $i => $value)
				if (($this->context->table->fields[$i] instanceof IDField) == false)
					$ret .= "<td nowrap=\"nowrap\">"
						. $this->context->table->fields[$i]->formatValue($value)
						. "</td>";

			// Get the id
			$value = reset($row);

			$ret .= "<td nowrap=\"nowrap\"><a href=\"./index.php?context=" . $this->context->name . "&edit=true&id=" . $value . "\"><img src=\"./icons/edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a><a onclick=\"return window.confirm('Do you want to delete the entry?')\" href=\"./index.php?context=" . $this->context->name . "&remove=true&id=" . $value . "\"><img src=\"./icons/remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a></td></tr>";
			$dark = $dark ^ 1;
		}

		$ret .= "</table></div>";

		return $ret;
	}
}

?>