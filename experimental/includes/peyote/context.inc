<?php

require_once("version.inc");

class Context {

	public $name;
	public $database;
	public $version;

	function __construct($name)
	{
		global $version;
		$this->version = $version;
		$this->name = $name;
	}
}

?>