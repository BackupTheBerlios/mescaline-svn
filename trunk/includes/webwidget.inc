<?php

require_once("contextattribute.inc");

class WebWidget {

	public $arguments;
	public $domid;
	public $moveable;
	public $hidden;

	public function HTML() {
	
		return "<div class=\"webwidget\" id=\"" . $this->domid . "\" >"
			. $this->buildHandle() 
			. "<div class=\"webwidget_content\">" . $this->buildContent() . "</div>"
			. "</div>";
	}

	protected function buildHandle() {

		return "<div class=\"webwidget_bar\">
			  <div id=\"" . $this->domid . "_handle\">" . $this->domid . "</div>
			  <div class=\"webwidget_icon\"><a href=\"./index.php?show:" . $this->domid . "=false\">
				<img src=\"./icons/close.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"close\"/>
			      </a>
			  </div>
			</div>";
	}

	protected function buildFooter() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle;\" src=\"./icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	public function assembleArguments($arguments) {

		$ret = "";

		foreach($arguments as $i => $argument) {
			
			if ($ret == "") $ret = "?" . $argument->key() . "=" . $argument->value;
			else $ret .= "&" . $argument->key() . "=" . $argument->value;
		}
		
		return $ret;
	}

	protected function buildContent() {}

	public function assembleArgumentsPost($arguments) {

		$ret = "";

		foreach ($arguments as $i => $argument)
			$ret .= "<input type=\"hidden\" name=\"" . $argument->key() . "\" value=\"" . $argument->value . "\">";
	
		return $ret;
	}

// 	protected function removeArgument($arguments, $name, $contextname) {
// 
// 		$arguments;
// 
// 		foreach ($arguments as $i => $argument) {
// 
// 			if (($argument->contextname == $contextname) && ($argument->name == $name)) unset($arguments[$i]);
// 		}
// 
// 		return $arguments;
// 	}

	protected function addArgument($arguments, $name, $contextname, $value) {

		$arguments;

		$arguments[] = new ContextAttribute($name, $contextname, $value);

		return $arguments;
	}
}

?>