<?php

require_once("webtable.inc");

class SingleWebTable extends WebTable
{

	function __construct($context, $arguments, $flags = 0)
	{
		parent::__construct($context, $arguments, $flags);
	}

	protected function buildHandle() {}
}

?>