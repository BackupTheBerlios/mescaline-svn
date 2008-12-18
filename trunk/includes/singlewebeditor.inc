<?php

require_once("webeditor.inc");

class SingleWebEditor extends WebEditor
{

	function __construct($context, $webfields, $id = -1) {

		parent::__construct($context, $webfields, $id);
	} 

	protected function buildHandle() {}
}

?>