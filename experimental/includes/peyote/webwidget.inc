<?php

require_once("contextattribute.inc");

class WebWidget {

	public $domid;
	public $context;

	function __construct($context) {

		$this->context = $context;
		$this->domid = $context->name;
	}

	public function HTML() {
	
		return "<div class=\"webwidget\" id=\"" . $this->domid . "\" >"
			. $this->buildHandle() 
			. "<div class=\"webwidget_content\">" . $this->buildContent() . "</div>"
			. "</div>";
	}

	protected function buildHandle() {

		return "<div class=\"webwidget_bar\">
			  <div id=\"" . $this->domid . "_handle\">" . $this->domid . "</div>
			  <div class=\"webwidget_icon\"><a href=\"./index.php" . $this->compileHrefArgument("setVisible", "false") . "\">
				<img src=\"./icons/close.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"close\"/>
			      </a>
			  </div>
			</div>";
	}

	protected function buildFooter() {

		return "<div id=\"footer\"><a href=\"http://mescaline.berlios.de\"><img id=\"link\" width=\"100\" height=\"21\" border=\"0\" style=\"vertical-align:middle;\" src=\"./icons/footer.png\"/ alt=\"mescaline\"/></a></div>";
	}

	protected function buildContent() {}

	public function compileFormArgument($name, $value) {

		return "<input type=\"hidden\" name=\"" . session_name() . "\" value=\"" . session_id() . "\"/><input type=\"hidden\" name=\"" . $name . "\" value=\"" . $this->domid . "," . $value . "\"/>";
	}

	public function compileMultipleFormArguments($values) {

		$ret = "<input type=\"hidden\" name=\"" . session_name() . "\" value=\"" . session_id() . "\"/>";

		foreach ($values as $name => $value) $ret .= "<input type=\"hidden\" name=\"" . $name . "\" value=\"" . $this->domid . "," . $value . "\"/>";
		
		return $ret;
	}

	public function compileHrefArgument($name, $value) {

		return "?" . session_name() . "=" . session_id() . "&" . $name . "=" . $this->domid . "," . $value;
	}
}

?>