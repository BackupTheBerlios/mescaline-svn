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
			  <div class=\"webwidget_icon\"><a href=\"./index.php" . $this->compileHrefArguments(array(new ContextAttribute("show", $this->domid, "false"))) . "\">
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

	protected function compileFormArguments($arguments) {

		$ret = "<input type=\"hidden\" name=\"" . session_name() . "\" value=\"" . session_id() . "\"/>";

		foreach ($arguments as $i => $argument) {

			$ret .= "<input type=\"hidden\" name=\"";
			
			if ($argument instanceof ContextAttribute) $ret .= $argument->name . ":" . $argument->contextname;
			else $ret .= $argument->name;

			$ret .= "\" value=\"" . $argument->value . "\"/>";
		}
		
		return $ret;	
	}

	protected function compileHrefArguments($arguments) {

		$ret = "?" . session_name() . "=" . session_id();

		foreach ($arguments as $i => $argument) {

			$ret .= "&";
			
			if ($argument instanceof ContextAttribute) 
				$ret .= $argument->name . ":". $argument->contextname . "=" . $argument->value;
			else $ret .= $argument->name . "=" . $argument->value;
		}
		
		return $ret;	
	}
}

?>