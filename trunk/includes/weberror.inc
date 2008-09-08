<?php

require_once("webwidget.inc");

class WebError extends WebWidget {

	public $caption;
	public $link;
	public $error;

	function __construct($caption, $domid, $error, $link) {

		$this->moveable = true;
		$this->caption = $caption;
		$this->domid = $domid;
		$this->error = $error;
		$this->link = $link;
	}

	protected function buildContent() {
	
		$ret = "";

		$ret .= "<table id=\"" . $this->domid . "\" class=\"webwidget\" border=0 cellpadding=0 cellspacing=0>
			<tr><td>" . $this->caption . "</td></tr>";

		$ret .=	"<tr><td colspan=\"2\"><br></td></tr>"
			. "<tr><td colspan=\"2\">" . "<i>" . $this->error . "</i>" . "</td></tr>"
			. "<tr><td colspan=\"2\"><hr size=\"1\" color=\"black\"></td></tr>"
			. "<tr><td colspan=\"2\" align=\"right\"><a href=\"" . $this->link . "\"><img src=\"./icons/ok.png\" style=\"vertical-align:middle\" alt=\"confirm\" border=\"0\" alt=\"ok\"></a></td></tr>
			</table>";

		return $ret;
	}
}
