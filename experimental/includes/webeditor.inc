<?php

require_once("webwidget.inc");

class WebEditor extends WebWidget
{
	public $id;
	public $context;
	public $webfields;

	function __construct($context, $webfields, $id = -1) {

		$this->moveable = true;
		$this->context = $context;
		$this->webfields = $webfields;
		$this->id = $id;
		$this->domid = $context->name;
	} 

	protected function buildContent() {

		// The field encoding is quite complicated, since php will transform html form variable names with special chars. Thus there is an md5 hash which references to the correct field name.

		$ret = "<form style=\"display: inline;\" action=\"./index.php" . (($this->id != -1) ? "#" . $this->context->name . ":" . $this->id : "") . "\" method=\"post\">";
		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";

		// create the hidden fields

		$arguments = array(new ContextAttribute(($this->id == -1) ? "insert" : "update", $this->domid, $this->id));
		foreach ($this->webfields as $i => $webfield) $arguments[] = new ContextAttribute(md5($webfield->field->name), $this->context->name, $webfield->field->name);
		$ret .= $this->compileFormArguments($arguments);

		// Walk through the fields and print them.

		foreach ($this->webfields as $i => $webfield) $ret .= "<tr>" . $webfield->renderFormName() . $webfield->renderFormField($this->context->name, md5($webfield->field->name), ($this->id == -1)) . "</tr>";

		$ret .= "</td></tr><tr><td colspan=\"2\"><hr size=\"1\" color=\"black\"></td></tr>
		  <tr><td style=\"text-align:right;\" colspan=\"2\">
		  <input type=\"image\" src=\"./icons/ok.png\" alt=\"ok\" width=\"20\" height=\"20\" border=\"0\" />
		  </td></tr>
		</table></form>";

		return $ret;
	}
}

?>