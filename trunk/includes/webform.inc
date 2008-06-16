<?php

require_once("webidfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");

class WebForm 
{
	public $id;
	public $context;
	public $webfields;

	function __construct($context, $webfields, $id = -1) {

		$this->context = $context;
		$this->webfields = $webfields;
		$this->id = $id;
	} 

	function printHTML() {
	
		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"
			. "<html><head>
					<title>mescaline: editor</title>
					<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
					<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
				</head>
				<body>";
		print $this->buildForm();
		print $this->footer() . "</body></html>";
	}

	protected function footer() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"./icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	protected function buildForm() {

		$ret .= "<div align=\"center\"><form action=\"./index.php\">";

		// Set the insert or update variable, and put table and context name in form.

		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";
		$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $this->context->table->name . "\">";

		// To return to sorted lists, set sort and hidden if they are present.

		$ret .= ((isset($_GET['sort'])) ? "<input type=\"hidden\" name=\"sort\" value=\"" . $_GET['sort'] . "\">" : "");
		$ret .= ((isset($_GET['hidden'])) ? "<input type=\"hidden\" name=\"hidden\" value=\"" . $_GET['hidden'] . "\">" : "");

		if ($this->id != -1) $ret .= "<input type=\"hidden\" name=\"update\" value=\"true\">";
		else $ret .= "<input type=\"hidden\" name=\"insert\" value=\"true\">";

		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";

		// Walk through the fields.

		foreach ($this->webfields as $i => $webfield) {

			// The field encoding is quite complicated, since php will transform html form variable names with special chars. Thus there is a hash which references to the correct field name.

			$hash = md5($webfield->field->name);
			$ret .= "<input type=\"hidden\" name=\"" . $hash . "\" value=\"" . $webfield->field->name . "\">";

			$ret .= "<tr>";

			$ret .= $webfield->renderFormName();
			$ret .= $webfield->renderFormField($hash);
			
			$ret .= "</tr>";
		}

		$ret .= "</td></tr><tr><td colspan=\"2\"><hr size=\"1\" color=\"black\"></td></tr><tr><td align=\"right\" colspan=\"2\"><input type=\"image\" src=\"./icons/ok.png\" alt=\"ok\"></td></tr></table></form></div>";

		return $ret;
	}
}

?>