<?php

require_once("field.inc");

class WebField
{
	public $field;
	public $value;

	function __construct($field) {

		$this->field = $field;
	}

	function renderTableField() {

		return "<td nowrap=\"nowrap\">"
			. $this->formatValue($this->value)
			. "</td>";
	}

	function renderFormField($hash) {

		return "<td><input name=\"field:" . $hash . "\" value=\"" . $this->value ."\"></td>";
	}

	function renderFormName() {

		return "<td nowrap=\"nowrap\">"
			. $this->field->name . ($this->field->optional ? "" : "<font color=\"red\">*</font>") 
			. "</td>";
	}

	protected function formatValue($value) {

		return $value;
	}
}

?>