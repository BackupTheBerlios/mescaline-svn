<?php

require_once("webtable.inc");

class SingleWebTable extends WebTable
{

	function __construct($context, $arguments, $flags = 0)
	{

		$this->moveable = true;
		$this->context = $context;
		$this->content = null;
		$this->arguments = $arguments;

		$this->domid = $context->name;

		$this->disassembleFlags($flags);
	}

	protected function buildHandle() {}
}

?>