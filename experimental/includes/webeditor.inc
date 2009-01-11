<?php

class WebEditor extends WebWidget
{
	public $id;
	public $webfields;

	function __construct($context, $webfields, $id) {

		parent::__construct($context);

		$this->webfields = $webfields;
		$this->id = $id;
	}

	protected function buildContent() {

		// The field encoding is quite complicated, since php will transform html form variable names with special chars. Thus there is an md5 hash which references to the correct field name.

		$ret = "<form style=\"display: inline;\" action=\"./index.php" . (($this->id != -1) ? "#" . $this->context->name . ":" . $this->id : "") . "\" method=\"post\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";

		// create the hidden fields

		$values = array();
		$values[($this->id == "new") ? "insert" : "update"] = $this->id;
		foreach ($this->webfields as $i => $webfield) $values[md5($webfield->field->name)] = $webfield->field->name;

		$ret .= $this->compileMultipleFormArguments($values);
		
		// Walk through the fields and print them.

		foreach ($this->webfields as $i => $webfield) $ret .= "<tr>" . $webfield->renderFormName() . $webfield->renderFormField($this->context->name, md5($webfield->field->name), ($this->id == "new")) . "</tr>";

		$ret .= "</td></tr><tr><td colspan=\"2\"><hr size=\"1\" color=\"black\"></td></tr>
		  <tr><td style=\"text-align:right;\" colspan=\"2\">
		  <input type=\"image\" src=\"./icons/ok.png\" alt=\"ok\" width=\"20\" height=\"20\" border=\"0\" />
		  </td></tr>
		</table></form>";

		return $ret;
	}
}

?>