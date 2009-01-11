<?php

require_once("peyote/context.inc");
require_once("table.inc");
require_once("database.inc");

class MySQLContext extends Context {

	public $table;
	public $database;
	public $allow_new;
	public $allow_edit;
	public $allow_delete;

	function __construct($name, $table, $database)
	{
		parent::__construct($name);

		$this->table = $table;
		$this->database = $database;
		$this->allow_new = true;
		$this->allow_edit = true;
		$this->allow_delete = true;
	}
}

?>