<?php

require_once("boolfield.inc");
require_once("field.inc");
require_once("table.inc");

class Database {

	public $link;
	public $name;
	public $hostname;
	public $user;
	public $password;

	function __construct($name, $hostname, $user, $password)
	{
		$this->name = $name;
		$this->hostname = $hostname;
		$this->user = $user;
		$this->password = $password;
	}
	
	function connect() {
		
		$this->link = mysql_connect($this->hostname,$this->user,$this->password);
		if ($this->link == false) return false;
		if (mysql_select_db($this->name, $this->link) == false) {
		
			mysql_close($this->link);	
			return false;
		}
	
		return true;
	}

	function disconnect() {

		return mysql_close($this->link);
	}

	function columns($table) {

		$query = "SHOW COLUMNS FROM " . "`" 
			. mysql_real_escape_string($this->name) . "`.`" 
			. mysql_real_escape_string($table->name) . "`";

		$result = mysql_query($query, $this->link);

		$fields = array();

		while($row = mysql_fetch_row($result)) {
	
			if ($row[1] == "tinyint(1)") $fields[] = new BoolField($row[0], ($row[2] == "YES"));
			else $fields[] = new Field($row[0], ($row[2] == "YES"));
		}

		return $fields;	
	}

	function tables() {

		$query = "SHOW TABLES FROM `" . 
			mysql_real_escape_string($this->name) . "`";

		$result = mysql_query($query, $this->link);

		$tables = array();

		while($row = mysql_fetch_row($result)) $tables[] = new Table($row[0], $this);	


		return $tables;
	}

	function update($table, $valuepairs) {

		$query = "UPDATE `" 
			. mysql_real_escape_string($table->name) . "` SET ";

		foreach ($valuepairs as $i => $pair) {

			$col = mysql_real_escape_string($pair[0]); 
			$value = (($pair[1] == "null") ? "NULL" : "'" . mysql_real_escape_string($pair[1]) . "'");
			$query .= "`" 
				. $col . "` = " . $value . ",";
		}
		// Remove trailing comma.

	  $query = substr($query , 0, strlen($query) - 1);

		$pair = reset($valuepairs);

		$query .= " WHERE `" 
			. mysql_real_escape_string($pair[0]) . "` = '" 
			. mysql_real_escape_string($pair[1]) . "'";

		mysql_query($query, $this->link) or die(mysql_error($this->link));
	}

	function insert($table, $valuepairs) {

		$query = "INSERT INTO `" 
			. mysql_real_escape_string($table->name) . "` (";

		// Skip ID field.

		$skipid = true;

		foreach ($valuepairs as $i => $pair) {

			if (!$skipid) $query .= "`" 
				. mysql_real_escape_string($pair[0]) . "`,";
			else $skipid = false;
		}		

		// Remove trailing comma.

	 	$query = substr($query , 0, strlen($query) - 1);

		$query .= ") VALUES (";

		$skipid = true;

		foreach ($valuepairs as $i => $pair) {
		
			if (!$skipid) $query .= "'" . mysql_real_escape_string($pair[1]) . "',";
			else $skipid = false;
		}		

		// Remove trailing comma.

	  $query = substr($query , 0, strlen($query) - 1);

		$query .= ")";

		mysql_query($query, $this->link) or die(mysql_error($this->link));
	}

	function remove($table, $id) {

		$value = reset($table->fields);

		$query = "DELETE FROM `" 
			. mysql_real_escape_string($table->name) . "` WHERE `" 
			. mysql_real_escape_string($value->name) . "` = '" . $id . "'";

		mysql_query($query, $this->link) or die(mysql_error($this->link));
	}

	function buildRow($table, $id) {

		$this->buildTable($table, null, $id);
	}

	function buildTable($table, $sort = null, $id = null)
	{
		$result = $this->connect();

		if ($result == false) {

			echo mysql_error();
			return false;
		}

		$result = mysql_query($table->buildQuery($this, $sort, $id), $this->link);

		if ($result == false) {

			$this->disconnect();
			echo mysql_error();
			return false;
		}

		$rows = array();
		while($row = mysql_fetch_row($result)) $rows[] = $row;

		$table->rows = $rows;

		return true;
	}
	
	function buildListField($listfield) {		
		
		$result = $this->connect();

		if ($result == false) {

			echo mysql_error();
			return false;
		}

		$result = mysql_query($listfield->query, $this->link);

		if ($result == false) {

			$this->disconnect();
			echo mysql_error();
			return false;
		}

		$listfield->list = array();
		while($row = mysql_fetch_row($result)) $listfield->list[] = $row;

		return true;
	}
}

?>