<?php

require_once("webeditor.inc");

class SingleWebEditor extends WebEditor
{

	function __construct($context, $webfields, $id = -1) {

		$this->moveable = true;
		$this->context = $context;
		$this->webfields = $webfields;
		$this->id = $id;
		$this->domid = $context->name;
	} 

	protected function buildHandle() {}
}

?>