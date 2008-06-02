<?php

require_once("version.inc");
require_once("table.inc");
require_once("idfield.inc");
require_once("listfield.inc");
require_once("boolfield.inc");
require_once("webform.inc");
require_once("webtable.inc");
require_once("database.inc");

class WebContext {

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

	private function extractFields() {

	// Extract options from parameters. The information arrives in the format: "<hash>=$name ... field:<hash>=$value".

		$options = array();

		foreach ($_GET as $option => $value) {

			if (strncmp($option, "field:", 6) == 0) {

				$name = $_GET[substr($option,6)];
				$options[] = array($name, $value);
			}
		}
		return $options;
	}

	function start() {

		if (isset($_GET['remove'])) {
	
			$this->database->connect();
			$this->database->remove($this->table, $_GET['id']);
			$this->database->disconnect();
		
			header("Location: ./index.php?context=" . $this->name);
		
		} else if (isset($_GET['update'])) {
			
			$this->database->connect();	
			$this->database->update($this->table, $this->extractFields());
			$this->database->disconnect();
		
			header("Location: ./index.php?context=" . $this->name);
		
		} else if (isset($_GET['insert'])) {
			
			$this->database->connect();
			$this->database->insert($this->table, $this->extractFields());
			$this->database->disconnect();
		
			header("Location: ./index.php?context=" . $this->name);
		
		} else if (isset($_GET['edit'])) {
		
			// Show form
		
			$webform = new WebForm($this, $_GET['id']);
			$webform->printHTML();
		
		} else if (isset($_GET['new'])) {
		
			// Show form
		
			$webform = new WebForm($this);
			$webform->printHTML();
		
		} else {
		
			if (isset($_GET['sort'])) $webtable = new WebTable($this, $_GET['sort']);
			else $webtable = new WebTable($this);
			$webtable->printHTML();
		}
	}
}

?>