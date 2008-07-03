<?php

require_once("boolfield.inc");
require_once("field.inc");
require_once("stringfield.inc");
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
			else if (strncmp($row[1], "varchar", 7) == 0) {

				$length = sscanf($row[1],"varchar(%d)");
				$fields[] = new StringField($row[0], $length[0], ($row[2] == "YES"));
			}
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

		if ($this->connect() == false) return false;

		$query = "UPDATE `" 
			. mysql_real_escape_string($table->name) . "` SET ";

		foreach ($valuepairs as $i => $pair) {

			$col = mysql_real_escape_string($pair[0]); 
			$value = (($pair[1] == "") ? "NULL" : "'" . mysql_real_escape_string($pair[1]) . "'");
			$query .= "`" . $col . "` = " . $value . ",";
		}
		// Remove trailing comma.

		$query = substr($query , 0, strlen($query) - 1);

		$pair = reset($valuepairs);

		$query .= " WHERE `" 
			. mysql_real_escape_string($pair[0]) . "` = '" 
			. mysql_real_escape_string($pair[1]) . "'";

		$ret = mysql_query($query, $this->link);
		if ($ret == false) return false;

		$this->disconnect();

		return $ret;
	}

	function error() {

		return mysql_error($this->link);
	}

	function insert($table, $valuepairs) {

		if ($this->connect() == false) return false;

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
		
			if (!$skipid) $query .= (($pair[1] == "") ? "NULL," : "'" . mysql_real_escape_string($pair[1]) . "',");
			else $skipid = false;
		}		

		// Remove trailing comma.

		$query = substr($query , 0, strlen($query) - 1);

		$query .= ")";

		$ret = mysql_query($query, $this->link);
		if ($ret == false) return false;

		$this->disconnect();

		return $ret;
	}

	function remove($table, $id) {

		if ($this->connect() == false) return false;

		$value = reset($table->fields);

		$query = "DELETE FROM `" 
			. mysql_real_escape_string($table->name) . "` WHERE `" 
			. mysql_real_escape_string($value->name) . "` = '" . $id . "'";

		$ret = mysql_query($query, $this->link);
		if ($ret == false) return false;

		$this->disconnect();

		return $ret;
	}

	function buildRow($table, $id) {

		$rows = $this->buildTable($table, null, $id);
		return $rows[0];
	}

	function buildTable($table, $sort = null, $id = null)
	{
		if ($this->connect() == false) return false;

		$result = mysql_query($table->buildQuery($this, $sort, $id), $this->link);

		if ($result == false) {

			print mysql_error();
			$this->disconnect();
			return false;
		}

		$rows = array();
		while($row = mysql_fetch_row($result)) $rows[] = $row;

		return $rows;
	}

	function buildListField($listfield) {		

		if ($this->connect() == false) return false;

		$result = mysql_query($listfield->query, $this->link);

		if ($result == false) {

			$this->disconnect();
			return false;
		}

		$list = array();
		while($row = mysql_fetch_row($result)) $list[] = $row;

		return $list;
	}
}

?>