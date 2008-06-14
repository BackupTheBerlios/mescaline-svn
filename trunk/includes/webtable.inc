<?php

require_once("context.inc");
require_once("webfield.inc");
require_once("webidfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");

class WebTable
{
	public $content;
	public $context;
	public $hidden;
	public $sort;

	function __construct($context, $content)
	{

		$this->context = $context;
		$this->content = $content;

		if (!isset($_GET['hidden'])) $this->hidden = 0;
		else $this->hidden = $_GET['hidden'];		
	}

	protected function CSS() {

		return "<style type=\"text/css\">
			#link { background-color: white; }
			#footer { text-align: center; padding: 6px; }
			table { background-color: white; padding: 3px; margin: 0px; border: 1px solid black; }
			body { background: lightgray; }
			a { color : #3c3; font-family : verdana, arial, helvetica, sans-serif; text-decoration : none; }
			a:link { color : #3c3; }
			a:visited { color : #3c3; }
			a:hover { text-decoration : underline; }
			.brightline { background-color: white; }
			.darkline { background-color: lightgray; }
		</style>";
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
					<title>table</title>"
					. $this->CSS()
				. "</head>
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
		foreach (reset($this->content) as $i => $webfield) {
	
			if (($webfield instanceof WebIDField) == false) {
	
			$ret .= "<td nowrap=\"nowrap\">"
			. "<a href=\"./index.php?context="
			. $this->context->name
			. "&sort=" . $webfield->field->name 
			. "&hidden=" . $this->hidden			
			. "\">" 
			. ((!$this->isHidden($i)) ? $webfield->field->name : "") 
			. "</a>"		
			. "<a href=\"./index.php?context="
			. $this->context->name;
			if (isset($_GET['sort'])) $ret .= "&sort=" . $_GET['sort'];
			$ret .= "&hidden=" . $this->toggleHidden($i) 
			. "\">"	
			. (($this->isHidden($i)) ? "<img src=\"./icons/show.png\" width=\"14\" height=\"14\" border=\"0\" style=\"vertical-align:middle\" alt=\"show\">" : "<img src=\"./icons/hide.png\" width=\"14\" height=\"14\" style=\"vertical-align:middle\" border=\"0\" alt=\"hide\">") 
			. "</a>" 
			. "</td>";
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

			// The last element are edit & delete links, which embedd the id to edit.

			$ret .= "<tr class=\"" . ($dark ? "darkline" : "brightline") . "\">";
			foreach ($row as $i => $webfield) {

				if ($this->isHidden($i)) $ret .= "<td></td>";
				else $ret .= $webfield->renderTableField();
			}			

			// Get the id
			$id = reset($row)->value;

			$ret .= "<td nowrap=\"nowrap\"><a href=\"./index.php?context=" . $this->context->name . "&edit=true&id=" . $id . "\"><img src=\"./icons/edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a><a onclick=\"return window.confirm('Do you want to delete the entry?')\" href=\"./index.php?context=" . $this->context->name . "&remove=true&id=" . $id . "\"><img src=\"./icons/remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a></td></tr>";
			$dark = $dark ^ 1;
		}

		$ret .= "</table></div>";

		return $ret;
	}
}

?>
