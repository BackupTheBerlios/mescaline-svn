<?php

session_start();
//print session_id() . "<br>";

ini_set('include_path', './includes/');


require_once("argument.inc");
require_once("contextattribute.inc");
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

	private $arguments;

	private function printHeader() { 

		print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html>
			<head>
			<title>mescaline: table view</title>
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
			<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
			</head>
			<body onload=\"onLoad();\">
			<script type=\"text/javascript\" src=\"js/layout.js\"></script>";			
	}

	private function printFooter() {

		print "</body></html>";
	}

	private function printContextbar() {

		print "<div class=\"contextbar\"><table><tr>";
		if ($handle = opendir('./context')) {

			while (false !== ($file = readdir($handle))) {

				if (substr($file, -8) == ".context") {

					$contextname = substr($file, 0, -8);

					if ($_SESSION["show"][$contextname] != "true") {

						print "<td><a href=\"./index.php?show:" . $contextname . "=true\">" . $contextname . "</a></td>";
					}
				}
			}
		}
		closedir($handle);
		print "</tr></table></div>";
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

		//$id = $_GET['edit:' . $context->name];

		// Create webfield array.
		$webfields = $this->createWebFields($context);

		if (!$new) {

			// Get Row.
			$row = array();

			$row = $context->database->buildRow($context->table, $id);

			if ($row === false) return new WebError("SQL Error", $context->name, $context->database->error(), "./index.php");

			// Populate WebFields.
			foreach ($row as $i => $value) $webfields[$i]->value = $value;

			return new WebEditor($context, $webfields, $id);
		}
		else return new WebEditor($context, $webfields);
	}

	private function createTable($contextname) {

		$contextfile = "./context/".$contextname.".context";
		$s = implode("", @file($contextfile));
		$context = unserialize($s);

		$table = new WebTable($context, $this->arguments, isset($_SESSION["flags"][$contextname]) ? $_SESSION["flags"][$contextname] : 0);

		$rows = $context->database->buildTable($context->table, $context->table->sort);

		if ($rows === false) return new WebError("SQL Error", $context->name, $context->database->lastError(), "index.php" . WebWidget::assembleArguments($this->arguments));

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

	private function dojo($widgets) {

		print "<script type=\"text/javascript\" src=\"js/dojo.js\"></script>
			<script type=\"text/javascript\">
			dojo.require(\"dojo.dnd.Mover\");
			dojo.require(\"dojo.dnd.Moveable\");
			dojo.require(\"dojo.dnd.move\");
	
			var initDND = function(){		

				var widgets = new Array(" . sizeof($widgets) . ");";
				$i = 0;
				foreach($widgets as $id => $widget) print "widgets[" . $i++ . "] = \"" . $id . "\";";
				print "makeMoveable(widgets);
				
			};
			dojo.addOnLoad(initDND);
		</script>";
	}

	private function layoutJS($widgets) {

		// dojo magic

		$this->dojo($widgets);

		// show widgets

		ksort($widgets);
		foreach($widgets as $i => $widget) print $widget->HTML();

		// Find out layouted and custom widgets

		$positionArguments = array();
		$layoutWidgets = array();

 		foreach($widgets as $id => $widget) {

			if (!isset($_SESSION["pos"][$id])) $layoutWidgets[$id] = $widget;
			else $positionArguments[$id] = new ContextAttribute($id, "pos", $_SESSION["pos"][$id]);
		}

		print "<script type=\"text/javascript\">
			function onLoad() {

				var layoutWidgets = new Array(" . sizeof($layoutWidgets) . ");";
				$i = 0;
				foreach($layoutWidgets as $id => $widget) print "layoutWidgets[" . $i++ . "] = \"" . $id . "\";";
				print "layout(layoutWidgets);";
				foreach($positionArguments as $id => $argument) {

					$split = split("x", $argument->value, 2);
					$x = $split[0];
					$y = $split[1];

					print "place(\"" . $id . "\"," . $x . "," . $y . ");";
				}
			print "}
		</script>";
	}

	private function layoutNoJS($widgets) {

		print "<div align=\"center\"><table><tr>";

		// show widgets

		ksort($widgets);
		foreach($widgets as $i => $widget) {
		
			$widget->moveable = false;
			print "<td style=\"vertical-align:top\">" . $widget->HTML() . "</td>";
		}

		print "</tr></table></div>";
	}

	private function setContextAttribute($contextname, $key, $value) {

		if (!isset($_SESSION[$key])) $_SESSION[$key] = array();
		$_SESSION[$key][$contextname] = $value;

// 		print 'set $_SESSION['.$key.']['.$contextname.'] = '. $value . '<br>';
	}

	private function setArgument($key, $value) {

		if (!isset($_SESSION[$key])) $_SESSION[$key] = array();
		$_SESSION[$key]["global"] = $value;

// 		print 'set $_SESSION['.$key.'][global] = '. $value . '<br>';
	}

	public function run() {

		$this->arguments = array();
		$widgets = array();

		if ($_GET["new_session"] == "true") {

			session_unset();
			unset($_GET["new_session"]);
		}

		foreach($_GET as $key => $value) { 

			$split = split(":", $key, 2); 

			if ($split[0] == "pos") {

				$contextname = $split[1];
// 				print "contextattribute (SESSION): " . $split[0] . "," . $split[1] . "," . $value ."<br>";

				$this->setContextAttribute($contextname, $split[0], $value);
			}
			else if ($split[0] == "flags") {

				$contextname = $split[1];
// 				print "contextattribute (SESSION): " . $split[0] . "," . $split[1] . "," . $value ."<br>";

				$this->setContextAttribute($contextname, $split[0], $value);
			}
			else if (($split[0] == "show") || ($split[0] == "edit") || ($split[0] == "new")) {

				$contextname = $split[1];
// 				print "contextattribute (SESSION): " . $split[0] . "," . $split[1] . "," . $value ."<br>";

				$this->setContextAttribute($contextname, $split[0], $value);
			}
			else if ($key == "js") {

// 				print "contextattribute (SESSION): " . $key . "," . $value ."<br>";

				$this->setArgument($key, $value);
			}
 			else if (count($split) > 1) {
					
// 				print "contextattribute: " .$split[0] . "," . $split[1] . "," . $value ."<br>";
				$this->arguments[] = new ContextAttribute($split[0], $split[1], $value);
			}
		}

		// Check if the contexts are present.

		foreach($this->arguments as $i => $argument) {

			if ($argument instanceof ContextAttribute) {
		
				$contextfile = "./context/".$argument->contextname.".context";

				if (!file_exists($contextfile)) {
					
					unset($this->arguments[$i]);
					$widgets[$argument->contextname] = new WebError("Context Error", $argument->contextname, "Context \"" . $argument->contextname ."\" could not be found.", "index.php" . WebWidget::assembleArguments($this->arguments));
				}
			}
		}
		
		// Perform data operations.
		
		foreach($this->arguments as $i => $argument) {
			
			if (($argument->name == "update") || ($argument->name == "insert") || ($argument->name == "remove")) {

				unset($this->arguments[$i]);
				$ret = $this->modifyData($argument->contextname, $argument->name, $argument->value);
				if ($ret === true) {

					if ($argument->name == "update") unset($_SESSION["edit"][$argument->contextname]);
					else if ($argument->name == "insert") unset($_SESSION["new"][$argument->contextname]);
					$_SESSION["show"][$argument->contextname] = "true";  
				}
				else $widgets[$argument->contextname] = new WebError("SQL Error", $argument->contextname, $ret, "./index.php");
			}
		}

		// Perform show and edit operations.

// 		foreach($this->arguments as $i => $argument) {
// 
// 			if ($argument->name == "show") {
// 
// 				if ($argument->value == "true") $widgets[$argument->contextname] = $this->createTable($argument->contextname);
// 				else unset($this->arguments[$i]);
// 			}
// 			else if (($argument->name == "edit") || ($argument->name == "new"))
// 				$widgets[$argument->contextname] = $this->createEditor($argument->contextname, $argument->value, ($argument->name == "new"));
// 		}

		print "<div style=\"position:absolute;right:0px;top:0px;text-align:right;\">";
		foreach ($_SESSION as $key => $value) foreach ($value as $key2 => $value2) print $key . ":" . $key2 . "=" . $value2 . "<br>";
		print "</div>";

		foreach ($_SESSION as $key => $array) {

			if ($key == "show") foreach ($array as $contextname => $value) {

				if ($value == "true") $widgets[$contextname] = $this->createTable($contextname);
				else unset($_SESSION["show"][$contextname]);
			}
			else if (($key == "edit") || ($key == "new")) foreach ($array as $contextname => $value) {

				$widgets[$contextname] = $this->createEditor($contextname, $value, ($key == "new"));
			}
		}

		$this->printHeader();

		// Check js.

		if (!isset($_SESSION["js"])) $jsEnabled = "unknown";
		else $jsEnabled = $_SESSION["js"]["global"];

		// If js argument is not set, refresh the page with js argument via js.

		if ($jsEnabled == "unknown") {

			$this->arguments[] = new Argument("js", "true");

			print "<script type=\"text/javascript\">
			window.location.replace(\"./index.php" . WebWidget::assembleArguments($this->arguments) . "\");
			</script>";

			$this->layoutNoJS($widgets);

		} else if ($jsEnabled == "true") $this->layoutJS($widgets);
		else $this->layoutNoJS($widgets);

		$this->printContextbar();

		$this->printFooter();	
	}
}

$main = new Main();
$main->run();

?>