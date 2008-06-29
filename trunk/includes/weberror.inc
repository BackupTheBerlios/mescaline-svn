<?php

require_once("webwidget.inc");

class WebError extends WebWidget {

	public $caption;
	public $link;
	public $error;

	function __construct($caption, $error, $link) {

		$this->caption = $caption;
		$this->error = $error;
		$this->link = $link;
	}

	protected function buildContent() {
	
		return "<table border=0 cellpadding=0 cellspacing=0><tr><td>" . $this->caption . "</td></tr>"
			. "<tr><td><br></td></tr>"
			. "<tr><td>" . "<i>" . $this->error . "</i>" . "</td></tr>"
			. "<tr><td><hr size=\"1\" color=\"black\"></td></tr>"
			. "<tr><td align=\"right\"><a href=\"" . $this->link . "\"><img src=\"./icons/ok.png\" style=\"vertical-align:middle\" alt=\"confirm\" border=\"0\" alt=\"ok\"></a></td></tr>
			</table>";
	}
}