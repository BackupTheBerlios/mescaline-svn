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

	protected function CSS() {

		return "<style type=\"text/css\">
			#container { float: left; position: relative; left: 50%; margin-top: 1em; }
			#inner { padding: 3px; position: relative; left: -50%; border: 1px solid black; }
			#footer { clear: both; text-align: center; padding: 6px; }
			a { color : #3c3; font-family : verdana, arial, helvetica, sans-serif; text-decoration : none; }
			a:link { color : #3c3; }
			a:visited { color : #3c3; }
			a:hover { text-decoration : underline; }
			.brightline { background-color:#FFFFFF; }
			.darkline { background-color:#DDDDDD; }
		</style>";
	}

	function printHTML() {
	
		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"
			. "<html><head><title>form</title>"
			. $this->CSS()
			. "</head><body><div id=\"container\"><div id=\"inner\" align=\"center\">";
		print $this->buildForm();
		print "</div></div>" . $this->footer() . "</body></html>";
	}

	protected function footer() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"./icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	protected function buildForm() {

		//if ($this->id != -1) {
		
			//$this->webcontext->context->database->buildRow($this->webcontext->context->table, $this->id);
			//$row = $this->webcontext->context->table->rows[0];
		//}

		$ret .= "<form action=\"./index.php\">";

		// Set the insert or update variable, and put table and context name in form.

		$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $this->context->name . "\">";
		$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $this->context->table->name . "\">";

		if ($this->id != -1) $ret .= "<input type=\"hidden\" name=\"update\" value=\"true\">";
		else $ret .= "<input type=\"hidden\" name=\"insert\" value=\"true\">";

		$ret .= "<table border=0 cellpadding=0 cellspacing=0>";

		// Walk through the fields.

		$dark = 1;

		foreach ($this->webfields as $i => $webfield) {

			// The field encoding is quite complicated, since php will transform html form variable names with special chars. Thus there is a hash which references to the correct field name.

			$hash = md5($webfield->field->name);
			$ret .= "<input type=\"hidden\" name=\"" . $hash . "\" value=\"" . $webfield->field->name . "\">";

			$ret .= "<tr>";

			$ret .= $webfield->renderFormName($dark);
			$ret .= $webfield->renderFormField($hash);

			$dark = $dark ^ 1;
			
			/*

			// When in update mode get the value of the current field in the row. In insert mode set the value to empty.

			//if ($this->id != -1) $value = $row[$i];
			//else $value = "";

			$ret .= "<td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">" . $field->name . ((($field->optional) || ($field instanceof IDField)) ? "" : "<font color=\"red\">*</font>") . "</td><td>";
			$dark = $dark ^ 1;

			// The field encoding is quite complicated, since php will transform html form variable names with special chars. Thus there is a hash which references to the correct field name.

			$hash = md5($field->name);
			$ret .= "<input type=\"hidden\" name=\"" . $hash . "\" value=\"" . $field->name . "\">";

			if ($field instanceof BoolField) {

				$ret .= "<select name=\"field:" . $hash . "\" >";	

				// If the field is optional, add a blank field on the top.

				if ($field->optional) $ret .= "<option " . (($value == "") ? "selected=\"selected\" " : "") . "value=\"null\">NULL</<option>";
				$ret .= "<option " . (($value == "1") ? "selected=\"selected\" " : "") . "value=\"1\">Yes</option>";
				$ret .= "<option " . (($value == "0") ? "selected=\"selected\" " : "") . "value=\"0\">No</option>";

				$ret = $ret . "</select>";
			
			} else if ($field instanceof ListField) {

				// Build list of listfield

				$this->webcontext->context->database->buildListField($field);

				$ret .= "<select name=\"field:" . $hash . "\" >";

				// If the field is optional, add a blank field on the top.

				if ($field->optional) {

					$selected = ($value == "") ? "selected=\"selected\" " : "";
					$ret .= "<option " . $selected . "value=\"null\">NULL</<option>";
				}

				// Make the rows ID field the value of the option and display the name. Select the first entry, when creating new entries.

				if ($this->id != -1) {

					foreach ($field->list as $i => $listrow) {
	
						$selected = ($value == $listrow[1]) ? "selected=\"selected\" " : "";			
						$ret .= "<option " . $selected . "value=\"" . $listrow[0] . "\">" . $listrow[1] . "</option>";
					}
				
				} else {

 					$selectfirst = true;

					foreach ($field->list as $i => $listrow) {
	
						$selected = ($selectfirst) ? "selected=\"selected\" " : "";			
						$ret .= "<option " . $selected . "value=\"" . $listrow[0] . "\">" . $listrow[1] . "</option>";
						$selectfirst = false;
					}				
				}

				$ret = $ret . "</select>";

			} else if ($field instanceof IDField) {

				$ret .= "<input size=\"5\" disabled=\"disabled\" value=\"" . $value ."\"><input type=\"hidden\" name=\"field:" . $hash . "\" value=\"" . $value ."\">";	
	
			} else {

				$ret .= "<input name=\"field:" . $hash . "\" value=\"" . $value ."\">";
			} 

			$ret .= "</td></tr>";

			*/
			$ret .= "</tr>";
		}

		$ret .= "</td></tr></table><br><input type=\"image\" src=\"./icons/ok.png\" alt=\"ok\"></form>";

		return $ret;
	}
}

?>