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

	protected function footer() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"../icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	function printHTML() {
	
		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html> 
				<head>
					<title>" . $this->name . "</title>
					<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
					<link rel=stylesheet type=\"text/css\" href=\"../css/default.css\">
				</head>
				<body>
					<div id=\"container\">
						<div id=\"inner\" align=\"center\">"
									. $this->content()
									. "<div id=\"spacer\"><hr size=\"1\" color=\"black\"></div>"
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