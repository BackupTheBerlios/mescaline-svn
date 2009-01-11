<?php

class WebError extends WebWidget {

	public $error;

	function __construct($context, $error, $value, $name) {

		parent::__construct($context);
		$this->error = $error;
	}

	protected function buildContent() {
	
		$ret = "";

		$ret .= "<table id=\"" . $this->domid . "\" class=\"webwidget\" border=0 cellpadding=0 cellspacing=0>
			<tr><td>" . $this->caption . "</td></tr>";

		$ret .=	"<tr><td colspan=\"2\"><br></td></tr>"
			. "<tr><td colspan=\"2\">" . "<i>" . $this->error . "</i>" . "</td></tr>"
			. "<tr><td colspan=\"2\"><hr size=\"1\" color=\"black\"></td></tr>"
			. "<tr><td colspan=\"2\" align=\"right\"><a href=\"./index.php" . $this->compileHrefArguments($this->arguments) . "\"><img src=\"./icons/ok.png\" style=\"vertical-align:middle\" alt=\"confirm\" border=\"0\" alt=\"ok\"></a></td></tr>
			</table>";

		return $ret;
	}
}
