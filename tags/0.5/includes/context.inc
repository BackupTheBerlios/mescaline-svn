<?php

require_once("version.inc");
require_once("table.inc");
require_once("database.inc");

class Context {

	public $table;
	public $name;
	public $database;
	public $version;
	public $allownew;

	function __construct($name, $table, $database)
	{
		global $version;
		$this->version = $version;

		$this->name = $name;
		$this->table = $table;
		$this->database = $database;
		$allownew = false;
	}
}

?>