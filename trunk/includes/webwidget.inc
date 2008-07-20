<?php

require_once("argument.inc");

class WebWidget {

	public $arguments;
	public $domid;

	public function HTML() {
	
		return "<div style=\"float:left;padding:6px;\"><div id=\"" . $this->domid . "\" >" . $this->buildContent() . $this->buildFooter() . "</div></div>";
	}

	protected function buildFooter() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle\" src=\"./icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	public function assembleArguments($arguments) {

		$ret = "";

		foreach($arguments as $i => $argument) {
			
			if ($ret == "") $ret = "?" . $argument->name . ":" . $argument->contextname . "=" . $argument->value;
			else $ret .= "&" . $argument->name . ":" . $argument->contextname . "=" . $argument->value;
		}
		
		return $ret;
	}

	protected function buildContent() {}

	public function assembleArgumentsPost($arguments) {

		$ret = "";

		foreach ($arguments as $i => $argument) { 
			$ret .= "<input type=\"hidden\" 
				name=\"" . $argument->name . ":" . $argument->contextname 
				. "\" value=\"" . $argument->value . "\">";
		}
	
		return $ret;
	}

	protected function removeArgument($arguments, $name, $contextname) {

		$arguments;

		foreach ($arguments as $i => $argument) {

			if (($argument->contextname == $contextname) && ($argument->name == $name)) unset($arguments[$i]);
		}

		return $arguments;
	}

	protected function addArgument($arguments, $name, $contextname, $value) {

		$arguments;

		$arguments[] = new Argument($name, $contextname, $value);

		return $arguments;
	}
}

?>