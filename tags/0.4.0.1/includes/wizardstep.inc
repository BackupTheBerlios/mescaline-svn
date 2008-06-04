<?php

class WizardStep {

	public $previous;
	public $next;
	public $name;

	function __construct($name, $previous=false, $next=false) {

		$this->name = $name;
		$this->previous = $previous;
		$this->next = $next;
	}

	protected function wizardFilename() {

		return "../context/" . $_GET['context'] . ".wizard";
	}

	protected function CSS() {

		return "<style type=\"text/css\">
			#container { float: left; position: relative; left: 50%; margin-top: 1em; }
			#inner { position: relative; left: -50%; padding-left: 3px; padding-right: 3px; padding-top: 3px; padding-bottom: 1em; border: 1px solid black; }
			#previous, #next { position: absolute; bottom: 0; padding-left: 3px; padding-right: 3px; padding-bottom: 3px; }
			#previous { left: 0; }
			#next { right: 0; }
			#spacer { height: 10px }
			#footer { clear: both; text-align: center; padding: 6px; }
			a { color : #3c3; font-family : verdana, arial, helvetica, sans-serif; text-decoration : none; }
			a:link { color : #3c3; }
			a:visited { color : #3c3; }
			a:hover { text-decoration : underline; }
			.brightline { background-color:#FFFFFF; }
			.darkline { background-color:#DDDDDD; }
		</style>";
	}

	protected function footer() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"../icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	function printHTML() {
	
		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html> 
				<head>
					<title>" . $this->name . "</title>"
					. $this->CSS()
				. "</head>
				<body>
					<div id=\"container\">
						<div id=\"inner\" align=\"center\">"
									. $this->content()
									. "<div id=\"spacer\"></div>"
									. $this->previousButton() . $this->nextButton()
						. "</div>
					</div>"
					. $this->footer()
				. "</body>
			</html>";
	}

	protected function content() {
		
		$this->caption();
	}

	protected function caption() {

		return "";
	}

	protected function previousButton() {

		if ($this->previous) return "<a id=\"previous\"href=\"./index.php?dialog=" . $this->previous->name . "\"><img src=\"../previous.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"previous\"></a>";
		return "";
	}

	protected function nextButton() {

		if ($this->next) return "<a id=\"next\"href=\"./index.php?dialog=" . $this->next->name . "\"><img src=\"../icons/next.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"next\"></a>";
		return "";
	}
}