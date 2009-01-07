<?php

require_once("weberror.inc");

class SingleWebError extends WebError
{

	function __construct($caption, $domid, $error, $argument = null) {

		parent::__construct($caption, $domid, $error, $argument);
	}

	protected function buildHandle() {}
}

?>