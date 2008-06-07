<?php

require_once("field.inc");

class WebField
{
	public $field;
	public $value;

	function __construct($field) {

		$this->field = $field;
	}

	function renderTableHeader($contextname) {

		return "<td nowrap=\"nowrap\"><a href=\"./index.php?context="
			. $contextname
			. "&sort=" . $this->field->name . "\">" 
			. $this->field->name . "</a></td>";
	}

	function renderTableField() {

		return "<td nowrap=\"nowrap\">"
			. $this->formatValue($this->value)
			. "</td>";
	}

	function renderFormField($hash) {

		return "<td><input name=\"field:" . $hash . "\" value=\"" . $this->value ."\"></td>";
	}

	function renderFormName($dark) {

		return "<td nowrap=\"nowrap\" class=\"" 
			. ($dark ? "darkline" : "brightline") . "\">"
			. $this->field->name . ($this->field->optional ? "" : "<font color=\"red\">*</font>") 
			. "</td>";
	}

	protected function formatValue($value) {

		return $value;
	}
}

?>