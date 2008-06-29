<?php

ini_set('include_path', './includes/');

require_once("argument.inc");
require_once("version.inc");
require_once("context.inc");

require_once("webidfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");
require_once("webstringfield.inc");

require_once("webtable.inc");
require_once("webeditor.inc");
require_once("weberror.inc");

class Main {

	private function printHeader() { 

		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html>
			<head>
			<title>mescaline: table view</title>
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
			<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
			</head>
			<body>";
	}

	private function printFooter() {

		print "</body></html>";
	}

	private function createWebFields($context) {

		$webfields = array();

		foreach ($context->table->fields as $i => $field) {

			if ($field instanceof IDField) $webfields[$i] = new WebIDField($field);
			else if ($field instanceof ListField) $webfields[$i] = new WebListField($field, $context->database);
			else if ($field instanceof BoolField) $webfields[$i] = new WebBoolField($field);
			else if ($field instanceof StringField) $webfields[$i] = new WebStringField($field);
			else $webfields[$i] = new WebField($field);
		} 

		return $webfields;
	}

	private function extractFieldArguments($contextname) {

		// Extract options from parameters. The information arrives in the format: "<hash>:$contextname=$name ... field<hash>:contextname=$value".

		$options = array();

		foreach ($this->arguments as $i => $argument) {

			if ((strncmp($argument->name, "field", 5) == 0) && ($argument->contextname == $contextname)) {
		
				$hash = substr($argument->name,5);
				foreach ($this->arguments as $j => $argument2) {
				
					if ($argument2->name == $hash) {

						$options[] = array($argument2->value, $argument->value);
						unset($this->arguments[$j]);
					} 
				}
				unset($this->arguments[$i]);
			}
		}

		return $options;
	}

	function createEditor($contextname, $id, $new) {
			
		// Check whether config file exists

		$contextfile = "./context/".$contextname.".context";

		if (!file_exists($contextfile)) return new WebError("Context Error", "Context \"" . $contextname ."\" could not be found.", "index.php");

		$s = implode("", @file($contextfile));
		$context = unserialize($s);

		$id = $_GET['edit:' . $context->name];

		// Create webfield array.
		$webfields = $this->createWebFields($context);

		if (!$new) {

			// Get Row.
			$row = array();

			$return = $context->database->buildRow($context->table, $row, $id);

			if (!$return) return new WebError("SQL Error", $context->database->error(), "index.php");

			// Populate WebFields.
			foreach ($row as $i => $value) $webfields[$i]->value = $value;

			return new WebEditor($context, $webfields, $this->arguments, $id);
		}
		else return new WebEditor($context, $webfields, $this->arguments);
	}

	private function createTable($contextname) {

		// Check whether config file exists

		$contextfile = "./context/".$contextname.".context";

		if (!file_exists($contextfile)) return new WebError("Context Error", "Context \"" . $contextname ."\" could not be found.", "index.php");

		$s = implode("", @file($contextfile));
		$context = unserialize($s);

		$table = new WebTable($context, $this->arguments);

		$rows = array();
		$result = $context->database->buildTable($context->table, $rows, $table->sort);

		if ($result == false) return new WebError("SQL Error", $context->table->buildQuery($context->database), "index.php");

		// Create content array		.	
		$content = array();

		foreach ($rows as $i => $row) {

			// Create webfield array.
			$webfields = $this->createWebFields($context);

			// Populate webfield array.
			foreach ($row as $i => $value) $webfields[$i]->value = $value;

			// Add webfield array to content array.
			$content[] = $webfields;
		}
			
		$table->content = $content;
		return $table;
	}

	private function modifyData($contextname, $name, $value) {

		// Check whether config file exists

		$contextfile = "./context/".$contextname.".context";

		if (!file_exists($contextfile)) return new WebError("Context Error", "Context \"" . $contextname ."\" could not be found.", "index.php");

		$s = implode("", @file($contextfile));
		$context = unserialize($s);

		$success = true;
		if (($name == "update") || ($name == "insert")) {

			$options = $this->extractFieldArguments($contextname);

			if ($name == "update") $success = $context->database->update($context->table, $options);
			else $success = $context->database->insert($context->table, $options);
		}
		else $success = $context->database->remove($context->table, $value);
		
		if (!$success) {
		
			$error = $context->database->error();
			$context->database->disconnect();
			return new WebError("SQL Error", $error, "index.php");
		}

		return $this->createTable($contextname);
	}

	public function run() {

		$this->arguments = array();

		$this->printHeader();
		foreach($_GET as $key => $value) { 

			$split = split(":", $key, 2);
			if (count($split) > 1) $this->arguments[] = new Argument($split[0], $split[1], $value);
		}

		$widgets = array();

		foreach($this->arguments as $i => $argument) {
			
			if (($argument->name == "update") || ($argument->name == "insert") || ($argument->name == "remove")) {

				unset($this->arguments[$i]);
				$this->arguments[] = new Argument("show", $argument->contextname, "true");	
				$widgets[$argument->contextname] = $this->modifyData($argument->contextname, $argument->name, $argument->value);
			} else if (($argument->name == "show") && ($argument->value == "true"))
				$widgets[$argument->contextname] = $this->createTable($argument->contextname);
			else if (($argument->name == "edit") || ($argument->name == "new"))
				$widgets[$argument->contextname] = $this->createEditor($argument->contextname, $argument->value, ($argument->name == "new"));
		}

		// show widgets

		foreach($widgets as $i => $widget) {

			print $widget->HTML();
		}

		$this->printFooter();
	}
}

$main = new Main();
$main->run();

?>