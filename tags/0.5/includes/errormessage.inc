<?php

class ErrorMessage {

	public $caption;
	public $link;
	public $error;

	function __construct($caption, $error, $link) {

		$this->caption = $caption;
		$this->error = $error;
		$this->link = $link;
	}

	protected function footer() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	function printHTML() {
	
		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html> 
				<head>
					<title>" . $this->name . "</title>
					<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
					<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
				</head>
				<body>
					<div id=\"container\">
						<div id=\"inner\" align=\"center\">"
									. "<b>" . $this->caption . "</b>"
									. "<br><br>"
									. "<i>" . $this->error . "</i>"
									. "<div id=\"spacer\"><hr size=\"1\" color=\"black\"></div>"
									. "<a id=\"confirm\" href=\"" . $this->link . "\"><img src=\"./icons/ok.png\" style=\"vertical-align:middle\" alt=\"confirm\" border=\"0\" alt=\"ok\"></a>"
						. "</div>
					</div>"
					. $this->footer()
				. "</body>
			</html>";
	}
}