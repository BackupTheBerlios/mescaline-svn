<?php

require_once("context.inc");
require_once("webwidget.inc");
require_once("webfield.inc");
require_once("idfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");

class WebTable extends WebWidget
{
	public $content;
	public $context;

	function __construct($context, $arguments, $flags = 0)
	{

		$this->moveable = true;
		$this->context = $context;
		$this->content = null;
		$this->arguments = $arguments;

		$this->domid = $context->name;

		$this->disassembleFlags($flags);
	}

 	protected function assembleFlags($hidden, $sort) {
 
 		return $hidden | ($sort << count($this->context->table->fields));
 	}

 	protected function disassembleFlags($flags) {
 
		$this->context->table->sort = $flags >> count($this->context->table->fields);
		if ($this->context->table->sort == 0) $this->context->table->hidden = $flags;
		else $this->context->table->hidden = $flags % ($this->context->table->sort << count($this->context->table->fields));
	}

	protected function toggleHidden($i) {

		return $this->hidden ^ (1 << $i);
	}

	protected function isHidden($i) {

		return ($this->hidden >> $i) & 1;
	}

	protected function buildContent() {

		// build table

		$ret = "<table border=\"0\" cellspacing=\"0\">";

		// create header

		$ret .= "<tr class=\"darkline\">";
		foreach ($this->context->table->fields as $i => $field) {
	
			if (($field instanceof IDField) == false) {
				
 				$sortflags = $this->assembleFlags($this->context->table->hidden, $i);
 				$hideflags = $this->assembleFlags($this->context->table->toggleHidden($i), $this->context->table->sort);

				$ret .= "<td nowrap=\"nowrap\">";
					
				if (!$this->context->table->isHidden($i)) {
					
					$arguments = array(new ContextAttribute("flags", $this->domid, $sortflags));
					
					$ret .= "<a href=\"./index.php" . $this->compileHrefArguments($arguments) . "\">" . $field->name . "</a>";
				} 

				$arguments = array(new ContextAttribute("flags", $this->domid, $hideflags));

				$ret .= "<a " . (($this->context->table->isHidden($i)) ? "class=\"tooltip\"" : "")
					. "href=\"./index.php" . $this->compileHrefArguments($arguments) . "\">"
					. (($this->context->table->isHidden($i)) ? "<span>" . $field->name . "</span>" : "")
					. "<img src=\"./icons/" . (($this->context->table->isHidden($i)) ? "show" : "hide") 
					. ".png\" width=\"14\" height=\"14\" border=\"0\" style=\"vertical-align:middle\" alt=\"show\"></a></td>";
				}
		}

		// If allow new is set, paint new button.

		$ret .= "<td nowrap=\"nowrap\">";

		if ($this->context->allow_new) {

			$arguments = array(new ContextAttribute("new", $this->domid, "true"));

			$ret .= "<a href=\"./index.php" . $this->compileHrefArguments($arguments) . "\"><img src=\"./icons/new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a>";
		}

		$ret .= "</td></tr>";

		$dark = 0;

		foreach ($this->content as $i => $row) {

			// Get the id
			$id = reset($row)->value;

			// The last element are edit & delete links, which embedd the id to edit.

			$ret .= "<tr id=\"" . $this->context->name . ":" . $id ."\" class=\"" . ($dark ? "darkline" : "brightline") . "\">";

			foreach ($row as $i => $webfield) {

				if ($this->context->table->isHidden($i)) $ret .= "<td></td>";
				else $ret .= $webfield->renderTableField();
			}			

			// Paint the action links

			$ret .= "<td nowrap=\"nowrap\">";

			if ($this->context->allow_edit) { // edit is an http get method

				$arguments = array(new ContextAttribute("edit", $this->domid, $id));

				$ret .= "<a href=\"./index.php" . $this->compileHrefArguments($arguments) . "\">
				<img src=\"./icons/edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle;\" alt=\"edit\">
				</a>";
				
			}

			if ($this->context->allow_delete) { // delete is an http post method

				$arguments = array(new ContextAttribute("remove", $this->domid, $id));
	
				$ret .= "<form style=\"display: inline;\" action=\"./index.php\" method=\"post\">" . $this->compileFormArguments($arguments) . "<input onclick=\"return window.confirm('Do you want to delete the entry?')\" type=\"image\" src=\"./icons/remove.png\" alt=\"remove\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\"/></form>"; 
			}

			$ret .= "</td></tr>";

			$dark = $dark ^ 1;
		}

		$ret .= "</table>";

		return $ret;
	}
}

?>
