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
		$this->arguments = $arguments;
		$this->domid = $context->name;
	} 

	protected function buildContent() {

		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
		$ret .= "<form action=\"./index.php#" . $this->context->name . ":" . $this->id . "\" method=\"post\">";
		$ret .= "<input type=\"hidden\" name=\"" . (($this->id == -1) ? "insert" : "update") . ":" . $this->domid .  "\" value=\"" .  $this->id . "\">";

		// Walk through the fields.

		foreach ($this->webfields as $i => $webfield) {

			// The field encoding is quite complicated, since php will transform html form variable names with special chars. Thus there is a hash which references to the correct field name.

			$hash = md5($webfield->field->name);
			$ret .= "<input type=\"hidden\" name=\"" . $hash . ":" . $this->context->name . "\" value=\"" . $webfield->field->name . "\">";

			$ret .= "<tr>";

			$ret .= $webfield->renderFormName();
			$ret .= $webfield->renderFormField($this->context->name, $hash);
			
			$ret .= "</tr>";
		}

		$ret .= "</td></tr><tr><td colspan=\"2\"><hr size=\"1\" color=\"black\"></td></tr>
		  <tr><td style=\"text-align:right;\" colspan=\"2\">
		  <input type=\"image\" src=\"./icons/ok.png\" alt=\"ok\" width=\"20\" height=\"20\" border=\"0\" />
		  </td></tr>
		</form></table>";

		return $ret;
	}
}

?>