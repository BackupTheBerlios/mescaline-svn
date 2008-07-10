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

	function __construct($context, $arguments)
	{

		$this->context = $context;
		$this->content = $null;
		$this->arguments = $arguments;

		$this->disassembleFlags();
	}

 	protected function assembleFlags($hidden, $sort) {
 
 		return $hidden | ($sort << count($this->context->table->fields));
 	}

 	protected function disassembleFlags() {
 
		foreach ($this->arguments as $i => $argument) {

			if (($argument->contextname == $this->context->name) && ($argument->name == "flags")) {
 
				$flags = $argument->value;
				$this->context->table->sort = $flags >> count($this->context->table->fields);
				if ($this->context->table->sort == 0) $this->context->table->hidden = $flags;
				else $this->context->table->hidden = $flags % ($this->context->table->sort << count($this->context->table->fields));
			}
		}
	}

	protected function toggleHidden($i) {

		return $this->hidden ^ (1 << $i);
	}

	protected function isHidden($i) {

		return ($this->hidden >> $i) & 1;
	}

	protected function buildContent() {

		// build table

		$ret = "<table border=0 cellspacing=0>";

		// create header

		$ret .= "<tr class=\"darkline\">";
		foreach ($this->context->table->fields as $i => $field) {
	
			if (($field instanceof IDField) == false) {

				// Remove flags argument, than put new flags argument for sorting and hiding into the link.

				$stripped = $this->removeArgument($this->arguments, "flags", $this->context->name);
				
				$sortflags = $this->assembleFlags($this->context->table->hidden, $i);
				$sortarguments = $this->addArgument($stripped, "flags", $this->context->name, $sortflags);
				$hideflags = $this->assembleFlags($this->context->table->toggleHidden($i), $this->context->table->sort);
				$hidearguments = $this->addArgument($stripped, "flags", $this->context->name, $hideflags);

				$ret .= "<td nowrap=\"nowrap\">
					<a href=\"./index.php"
					. $this->assembleArguments($sortarguments)
					. "\">" 
					. ((!$this->context->table->isHidden($i)) ? $field->name : "")
					. "</a><a href=\"./index.php"
					. $this->assembleArguments($hidearguments)
					. "\"><img src=\"./icons/" . (($this->context->table->isHidden($i)) ? "show" : "hide") . ".png\" width=\"14\" height=\"14\" border=\"0\" style=\"vertical-align:middle\" alt=\"show\"></a>
					</td>";
				}
		}

		// iIf allow new is set, paint new button.
	
		$stripped = $this->removeArgument($this->arguments, "show", $this->context->name);
		$newarguments = $this->addArgument($stripped, "new", $this->context->name, "true");

		$ret .= "<td>" 
			. ($this->context->allownew ? 
				"<a href=\"./index.php" 
					. $this->assembleArguments($newarguments)
					. "\"><img src=\"./icons/new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a>" : ""
				) 
			. "</td></tr>";

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

			// Remove the show argument for the edit command.

			$stripped = $this->removeArgument($this->arguments, "show", $this->context->name);

			$editarguments = $this->addArgument($stripped, "edit", $this->context->name, $id);

			$removearguments = $this->addArgument($this->arguments, "remove", $this->context->name, $id);

			$ret .= "<td nowrap=\"nowrap\">
				<a href=\"./index.php"
					. $this->assembleArguments($editarguments)
					. "\">
					<img src=\"./icons/edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\">
				</a>
				<a onclick=\"return window.confirm('Do you want to delete the entry?')\" href=\"./index.php"
					. $this->assembleArguments($removearguments)
					. "\">
					<img src=\"./icons/remove.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\">
				</a>
				</td></tr>";
			$dark = $dark ^ 1;
		}

		$ret .= "</table>";

		return $ret;
	}
}

?>
