<?php

require_once("version.inc");
require_once("table.inc");
require_once("database.inc");

class Context {

	public $table;
	public $name;
	public $database;
	public $version;
	public $allow_new;
	public $allow_edit;
	public $allow_delete;

	function __construct($name, $table, $database)
	{
		global $version;
		$this->version = $version;

		$this->name = $name;
		$this->table = $table;
		$this->database = $database;
		$this->allow_new = true;
		$this->allow_edit = true;
		$this->allow_delete = true;
	}
}

?>