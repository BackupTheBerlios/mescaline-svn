<?php

require_once("context.inc");
require_once("webfield.inc");
require_once("idfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");

class WebTable
{
	public $content;
	public $context;
	public $hidden;

	function __construct($context, $content)
	{

		$this->context = $context;
		$this->content = $content;

		if (!isset($_GET['hidden'])) $this->hidden = 0;
		else $this->hidden = $_GET['hidden'];		
	}

	protected function footer() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"./icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	protected function toggleHidden($i) {

		return $this->hidden ^ (1 << $i);
	}

	protected function isHidden($i) {

		return ($this->hidden >> $i) & 1;
	}

	function printHTML() {

		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html>
				<head>
					<title>mescaline: table view</title>
					<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
					<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
				</head>
				<body>";
	
		print $this->buildTable();
		print $this->footer();
		print "</body></html>";
	}

	protected function buildTable() {

      // build table

		$ret = "<div align=\"center\"><table border=0 cellspacing=0>";

		// create header

		$ret .= "<tr class=\"darkline\">";
		foreach ($this->context->table->fields as $i => $field) {
	
			if (($field instanceof IDField) == false) {
	
				$ret .= "<td nowrap=\"nowrap\"><a href=\"./index.php?context="
					. $this->context->name
					. "&sort=" . $field->name 
					. "&hidden=" . $this->hidden			
					. "\">" 
					. ((!$this->isHidden($i)) ? $field->name : "")
					. "</a><a href=\"./index.php?context="
					. $this->context->name
					. ((isset($_GET['sort'])) ? ("&sort=" . $_GET['sort']) : "")
					. "&hidden=" . $this->toggleHidden($i) 
					. "\"><img src=\"./icons/" . (($this->isHidden($i)) ? "show" : "hide") . ".png\" width=\"14\" height=\"14\" border=\"0\" style=\"vertical-align:middle\" alt=\"show\"></a></td>";
				}
		}

		// if allow new is set, paint new button.
	
		$ret .= "<td>" 
			. ($this->context->allownew ? 
				"<a href=\"./index.php?context=" 
				. $this->context->name 
				. "&new=true\"><img src=\"./icons/new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a>" : ""
				) 
			. "</td></tr>";

		$dark = 0;

		foreach ($this->content as $i => $row) {

			// Get the id
			$id = reset($row)->value;

			// The last element are edit & delete links, which embedd the id to edit.

			$ret .= "<tr id=\"" . $id ."\" class=\"" . ($dark ? "darkline" : "brightline") . "\">";
			foreach ($row as $i => $webfield) {

				if ($this->isHidden($i)) $ret .= "<td></td>";
				else $ret .= $webfield->renderTableField();
			}			

        // paint the action links

			$ret .= "<td nowrap=\"nowrap\">
				<a href=\"./index.php?context=" 
					. $this->context->name . "&edit=true&id=" . $id
					. ((isset($_GET['sort'])) ? "&sort=" . $_GET['sort'] : "")
					. ((isset($_GET['hidden'])) ? "&hidden=" . $_GET['hidden'] : "")
					. "\">
					<img src=\"./icons/edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\">
				</a>
				<a onclick=\"return window.confirm('Do you want to delete the entry?')\" href=\"./index.php?context=" . $this->context->name . "&remove=true&id=" . $id
					. ((isset($_GET['sort'])) ? "&sort=" . $_GET['sort'] : "")
					. ((isset($_GET['hidden'])) ? "&hidden=" . $_GET['hidden'] : "")
					. "\">
					<img src=\"./icons/remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\">
				</a>
				</td></tr>";
			$dark = $dark ^ 1;
		}

		$ret .= "</table></div>";

		return $ret;
	}
}

?>
