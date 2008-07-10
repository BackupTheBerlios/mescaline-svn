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

		$contextfile = "./context/".$contextname.".context";
		$s = implode("", @file($contextfile));
		$context = unserialize($s);

		$id = $_GET['edit:' . $context->name];

		// Create webfield array.
		$webfields = $this->createWebFields($context);

		if (!$new) {

			// Get Row.
			$row = array();

			$row = $context->database->buildRow($context->table, $id);

			if ($row === false) return new WebError("SQL Error", $context->database->error(), "index.php");

			// Populate WebFields.
			foreach ($row as $i => $value) $webfields[$i]->value = $value;

			return new WebEditor($context, $webfields, $this->arguments, $id);
		}
		else return new WebEditor($context, $webfields, $this->arguments);
	}

	private function createTable($contextname) {

		$contextfile = "./context/".$contextname.".context";
		$s = implode("", @file($contextfile));
		$context = unserialize($s);

		$table = new WebTable($context, $this->arguments);

		$rows = $context->database->buildTable($context->table, $context->table->sort);

		if ($rows === false) return new WebError("SQL Error", $context->table->buildQuery($context->database), "index.php");

		// Create content array		.	
		$content = array();

		foreach ($rows as $i => $row) {

			// Create webfield array.
			$webfields = $this->createWebFields($context);

			// Populate webfield array.
			foreach ($context->table->fields as $i => $field) {

				if (!$context->table->isHidden($i)) {

					$entry = each($row);
					$webfields[$i]->value = $entry["value"];
				}
			}
			//foreach ($row as $i => $value) 

			// Add webfield array to content array.
			$content[] = $webfields;
		}
			
		$table->content = $content;
		return $table;
	}

	private function modifyData($contextname, $name, $value) {

		$contextfile = "./context/".$contextname.".context";
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
			return $error;
		}

		return true;
	}

	private function ajax($widgets) {

		print '<script type="text/javascript" src="dojo/dojo.js"></script>'
			. '<script type="text/javascript">'
				. 'dojo.require("dojo.dnd.Mover");'
				. 'dojo.require("dojo.dnd.Moveable");'
				. 'dojo.require("dojo.dnd.move");';

		foreach($widgets as $id => $widget) print "var " . $id . ";";

		print "var initDND = function(){";		

		foreach($widgets as $id => $widget) print $id . ' = new dojo.dnd.Moveable("' .  $id . '");';

		print "};dojo.addOnLoad(initDND);</script>";
	}

	public function run() {

		$this->arguments = array();

		$this->printHeader();
		foreach($_GET as $key => $value) { 

			$split = split(":", $key, 2);
			if (count($split) > 1) $this->arguments[] = new Argument($split[0], $split[1], $value);
		}

		$widgets = array();

		// Check if the contexts are present.

		foreach($this->arguments as $i => $argument) {

			$contextfile = "./context/".$argument->contextname.".context";

			if (!file_exists($contextfile)) {
				
				$widgets[$argument->contextname] = new WebError("Context Error", "Context \"" . $argument->contextname ."\" could not be found.", "index.php");
				unset($this->arguments[$i]);
			}
		}
		
		// Perform data operations.
		
		foreach($this->arguments as $i => $argument) {
			
			if (($argument->name == "update") || ($argument->name == "insert") || ($argument->name == "remove")) {

				unset($this->arguments[$i]);
				$ret = $this->modifyData($argument->contextname, $argument->name, $argument->value);
				if ($ret === true) $this->arguments[] = new Argument("show", $argument->contextname, "true");  
				else $widgets[$argument->contextname] = new WebError("SQL Error", $ret, "index.php");
			}
		}

		// Perform show and edit operations.

		foreach($this->arguments as $i => $argument) {

			if (($argument->name == "show") && ($argument->value == "true"))
				$widgets[$argument->contextname] = $this->createTable($argument->contextname);
			else if (($argument->name == "edit") || ($argument->name == "new"))
				$widgets[$argument->contextname] = $this->createEditor($argument->contextname, $argument->value, ($argument->name == "new"));
		}

		// ajax magic
		
		//$this->ajax($widgets);

		// show widgets

		ksort($widgets);
		foreach($widgets as $i => $widget) print $widget->HTML();

		$this->printFooter();
	}
}

$main = new Main();
$main->run();

?>