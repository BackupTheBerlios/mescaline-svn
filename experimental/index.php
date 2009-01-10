<?php

session_start();

ini_set('include_path', './includes/');

require_once("attribute.inc");
require_once("contextattribute.inc");
require_once("version.inc");
require_once("context.inc");

require_once("webidfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");
require_once("webstringfield.inc");

require_once("singlewebeditor.inc");
require_once("singlewebtable.inc");
require_once("singleweberror.inc");
require_once("webtable.inc");
require_once("webeditor.inc");
require_once("weberror.inc");

class Main {

	private $contexts; // TODO: check class members.
	private $arguments;
	private $multiple;
	private $create_contexts;

	private function buildHeader() { 

		return "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html>
			<head>
			<title>mescaline: table view</title>
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
			<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
			</head>
			<body onload=\"onLoad();\"><script type=\"text/javascript\" src=\"js/layout.js\"></script>";
	}

	private function buildFooter() {

		return "</body></html>";
	}

	private function printContextbar() {

		print "<div class=\"contextbar\">" . ($this->create_contexts ? "<div class=\"contextbar_item\">
		  <a href=\"./wizard/index.php\">
		  <img src=\"./icons/new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a></div>" : "" );
		if ($handle = opendir('./context')) {

			$contextarray = array();

			while (false !== ($file = readdir($handle))) {

				if (substr($file, -8) == ".context") {

					$contextname = substr($file, 0, -8);
					if ((!isset($_SESSION["show"][$contextname])) || ($_SESSION["show"][$contextname] != "true")) $contextarray[] = $contextname;

				}
			}

			foreach($contextarray as $i => $contextname) {
			
				print "<div class=\"contextbar_item\"><a href=\"./index.php?" . session_name() . "=" . session_id() . "&show:" . $contextname . "=true\">" . $contextname . "</a></div>";
				 if (end($contextarray) != $contextname) print "<div class=\"contextbar_item\">|</div>";	
			}
		}
		closedir($handle);
		print "</div>";
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

// 	private function extractFieldArguments($contextname) {
// 
// 		// Extract options from parameters. The information arrives in the format: "<hash>:$contextname=$name ... field<hash>:contextname=$value".
// 
// 		$options = array();
// 
// 		foreach ($this->arguments as $i => $argument) {
// 
// 			if ((strncmp($argument->name, "field", 5) == 0) && ($argument->contextname == $contextname)) {
// 		
// 				$hash = substr($argument->name,5);
// 				foreach ($this->arguments as $j => $argument2) {
// 				
// 					if ($argument2->name == $hash) {
// 
// 						$options[] = array($argument2->value, $argument->value);
// 						unset($this->arguments[$j]);
// 					} 
// 				}
// 				unset($this->arguments[$i]);
// 			}
// 		}
// 
// 		return $options;
// 	}

	private function extractFieldArguments($context) {

		// Extract options from parameters. The information arrives in the format: "<hash>=$contextname,$name ... ptr:<hash>=$value". This is complicated but mandatory, because the contextname cannot be included in form values (they would appear in the fields).
	
		$options = array();

		foreach ($_POST as $key => $value) {

			// Find an argument with a "ptr:" prefix as key.

			if (strncmp($key, "ptr:", 4) == 0) {
			
				$hash = substr($key,4);

				// Now find the fitting argument.

				foreach ($_POST as $key2 => $value2) if ($key2 == $hash) {

					$splitValue2 = split(",", $value2, 2);
					$contextName = $splitValue2[0];
					$name = $splitValue2[1];

					// When the name fits the context add a pair in options.

					if ($contextName == $context->name) $options[] = array($name, $value);
				}
			}
		}

		return $options;
	}

	private function loadContext($contextname) {

		global $version;

		if ($this->multiple == true) $filename = "./context/".$contextname.".context";
		else $filename = "./context";

		$s = implode("", @file($filename));
		$context = unserialize($s); // TODO: Proper error when file missing.

		if (($context->version != $version) || ($context == FALSE)) return null;
		else return $context;
	}

	function createError($caption, $domid, $error) {

		if ($this->multiple) return new WebError($caption, $domid, $error);
		else return new SingleWebError($caption, $domid, $error);
	}

	function createEditor($context, $id) {

		// Create webfield array.
		$webfields = $this->createWebFields($context);

		if ($id != "new") {

			// Get Row.
			$row = array();

			$row = $context->database->buildRow($context->table, $id);

			// Populate WebFields.
			foreach ($row as $i => $value) $webfields[$i]->value = $value;
		}
		
		return new WebEditor($context, $webfields, $id);
		//if ($this->multiple) return new WebEditor($context, $webfields, $id);
		//else return new SingleWebEditor($context, $webfields, $id);
	}

	private function createTable($context, $flags = 0) {

		$table = new WebTable($context, $this->arguments, $flags);
		//if ($this->multiple) $table = new WebTable($context, $this->arguments, $flags);
		//else $table = new SingleWebTable($context, $this->arguments, $flags);

		$rows = $context->database->buildTable($context->table, $context->table->sort);

		if ($rows === false) return $this->createError("SQL Error", $context->name, $context->database->lastError());

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

		$context = $this->loadContext($contextname);

		if ($context == null) return $this->createError("ContextError", $contextname, "Could not load context. Possible cause could be file corruption or wrong version.");

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
				foreach($widgets as $key => $widget) print "widgets[" . $i++ . "] = \"" . $widget->domid . "\";";
				print "makeMoveable(\"" . session_name() . "=" . session_id() . "\", widgets);		
			};
			dojo.addOnLoad(initDND);
		</script>";
	}

	private function layoutJS($widgets) {

		$html = "";

		// dojo magic

		$this->dojo($widgets);

		// show widgets

		ksort($widgets);
		foreach($widgets as $i => $widget) $html .= $widget->HTML();

		// Find out layouted and custom widgets

		$positionArguments = array();
		$layoutWidgets = array();

 		foreach($widgets as $key => $widget) {

			$layoutWidgets[$widget->domid] = $widget;
			//if (!isset($_SESSION["pos"][$id])) $layoutWidgets[$id] = $widget;
			//else $positionArguments[$id] = new ContextAttribute($id, "pos", $_SESSION["pos"][$id]);
		}

		$html .= "<script type=\"text/javascript\"> function onLoad() { ";

		$i = 0;
		foreach($widgets as $key => $widget) {

			$position = $this->getContextAttribute($this->getContextByName($widget->domid), "position");

			if ($position != null) {

				$split = split("x", $position, 2);
				$x = $split[0];
				$y = $split[1];
				$html .= "place(\"" . $widget->domid . "\"," . $x . "," . $y . ");";
			}
			else $html .= "place(\"" . $widget->domid . "\",0,0);";
		}
				//var layoutWidgets = new Array(" . sizeof($layoutWidgets) . ");";
				//$i = 0;
				//foreach($layoutWidgets as $id => $widget) $html .= "layoutWidgets[" . $i++ . "] = \"" . $id . "\";";
				//$html .= "layout(layoutWidgets);";
				/*foreach($positionArguments as $id => $argument) {

					$split = split("x", $argument->value, 2);
					$x = $split[0];
					$y = $split[1];

					$html .= "place(\"" . $id . "\"," . $x . "," . $y . ");";
				}*/
		$html .= "} </script>";

		return $html;
	}

	private function layoutNoJS($widgets) {

		$html = "";

		$html .= "<div align=\"center\"><table><tr>";

		// show widgets

		ksort($widgets);
		foreach($widgets as $i => $widget) {
		
			$widget->moveable = false;
			$html .= "<td style=\"vertical-align:top\">" . $widget->HTML() . "</td>"; // TODO: rename HTML function to buildHTML
		}

		$html .= "</tr></table></div>";

		return $html;
	}

	protected function getContextAttribute($context, $key) {

		if (!isset($_SESSION[$key][$context->name])) return null;
		else return $_SESSION[$key][$context->name];
	}

	protected function getAttribute($key) {

		if (!isset($_SESSION[$key])) return null;
		else return $_SESSION[$key];
	}

	protected function setContextAttribute($context, $key, $value) {

		if (!isset($_SESSION[$key])) $_SESSION[$key] = array();
		$_SESSION[$key][$context->name] = $value;
	}

	protected function removeContextAttribute($context, $key) {

		unset($_SESSION[$key][$context->name]);
	}

	protected function setAttribute($key, $value) {

		$_SESSION[$key] = $value;
	}

	private function setArgument($key, $value) {

		if (!isset($_SESSION[$key])) $_SESSION[$key] = array();
		$_SESSION[$key]["global"] = $value;

// 		print 'set $_SESSION['.$key.'][global] = '. $value . '<br>';
	}

	public function run() {

		$this->arguments = array();
		$widgets = array();

		if ((isset($_GET["new_session"])) && ($_GET["new_session"] == "true")) {

			session_unset();
			unset($_GET["new_session"]);
		}

		foreach($_GET as $key => $value) { 

			print $key . "=" . $value . "<br>";

			$splittedArguments = split(",", $value, 2);

			if (count($splittedArguments) == 2) {

				$contextName = $splittedArguments[0];
				$function = $key;
				$argument = $splittedArguments[1];
				//$context = $this->loadContext($contextName);
				
				//$context->$function($argument);

				print $contextName . "->" . $function . "(" . $argument . ");<br>";
			}

			/*$split = split(":", $key, 2); 

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
			} */
		}

		// Currently only delete, insert & update arrives as POST.

		foreach($_POST as $key => $value) { 

			$split = split(":", $key, 2);

			if (count($split) > 1) {
					
// 				print "contextattribute: " .$split[0] . "," . $split[1] . "," . $value ."<br>";
				$this->arguments[] = new ContextAttribute($split[0], $split[1], $value);
			}
		}

		// Check whether multiple contexts are available.

		$this->create_contexts = false;

		if (is_dir("./context")) {
			$this->multiple = true;
			if (is_writable("./context")) $this->create_contexts = true;
		} else if (is_file("./context")) {
			
			$found = false;

			$contextfile = "./context";
			$s = implode("", @file($contextfile));
			$context = unserialize($s);

			foreach($this->arguments as $i => $argument) if ($argument->contextname == $context->name) $found = true;
			if (!$found) $this->arguments[] = new ContextAttribute("show", $context->name, "true");
		}
		

		// Check if the contexts are present.

		foreach($this->arguments as $i => $argument) {

			if ($argument instanceof ContextAttribute) {

				if ($this->loadContext($argument->contextname) == null) {
					
					unset($this->arguments[$i]);
					$widgets[$argument->contextname] = $this->createError("Context Error", $argument->contextname, "Context \"" . $argument->contextname ."\" could not be found.");
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
				else $widgets[$argument->contextname] = $this->createError("SQL Error", $argument->contextname, $ret);
			  }
		}

		// Below is DEBUG output on the page! 

		if (true) 
		{
		  print "<div style=\"position:absolute;right:0px;top:0px;text-align:right;\">";
		  foreach ($_SESSION as $key => $value) foreach ($value as $key2 => $value2) print $key . ":" . $key2 . "=" . $value2 . "<br>";
		  print "</div>";
		}

		// Create widgets.

		foreach ($_SESSION as $key => $array) {

			if ($key == "show") foreach ($array as $contextname => $value) {

				if ($value == "true") $widgets[$contextname] = $this->createTable($contextname);
				else {

				  unset($_SESSION["show"][$contextname]);
				  unset($_SESSION["edit"][$contextname]);
				}
			}
			else if (($key == "edit") || ($key == "new")) foreach ($array as $contextname => $value) {

				$widgets[$contextname] = $this->createEditor($contextname, $value, ($key == "new"));
			}
		}

		// Print HTML header.

		print $this->buildHeader();

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

		} 
		else if (($this->multiple == false) || ($jsEnabled != "true")) $this->layoutNoJS($widgets);
		else $this->layoutJS($widgets);

		if ($this->multiple) $this->printContextbar();

		print $this->buildFooter();	
	}

	protected function loadContexts() {

		$contexts = array();
		
		$handle = opendir("./context"); // TODO: error handling.
		while (false !== ($file = readdir($handle))) {

			if (substr($file, -8) == ".context") { // TODO: get rid of .context suffix.

				$s = implode("", @file("./context/" . $file));
				$contexts[] = unserialize($s); // TODO: error handling.
			}
		}
		closedir($handle);

		return $contexts;
	}

	protected function getContextByName($name) {

		foreach($this->contexts as $i => $context) if ($context->name == $name) return $context;
		return null;
	}

	protected function processHTTPArguments() {

		$httpArguments = array_merge($_GET, $_POST);

		foreach($httpArguments as $key => $value) { 

			//print $key . "=" . $value . "<br>"; 

			$splitArguments = split(",", $value, 2);

			$argumentCount = count($splitArguments);

			if ($argumentCount == 1) {

				if ($key == "setJavascript") $this->setAttribute("javascript", $value); 
			} else if ($argumentCount == 2) {

				$contextName = $splitArguments[0];
				$argument = $splitArguments[1];

				$context = $this->getContextByName($contextName);

				if ($key == "setVisible") {
	
					if ($argument == "false") $this->removeContextAttribute($context, "edit");
					$this->setContextAttribute($context, "visible", $argument);
				}
				else if ($key == "setFlags") $this->setContextAttribute($context, "flags", $argument);
				else if ($key == "setPosition") $this->setContextAttribute($context, "position", $argument);
				else if ($key == "edit") $this->setContextAttribute($context, "edit", $argument);
				else if (($key == "update") || ($key == "insert")) {

					$options = $this->extractFieldArguments($context);

					if ($key == "update") $success = $context->database->update($context->table, $options);
					else $success = $context->database->insert($context->table, $options);

					// TODO: error handling, see success variable.

					$this->removeContextAttribute($context, "edit");
					$this->setContextAttribute($context, "visible", "true");
				} else if ($key == "remove") {

					$success = $context->database->remove($context->table, $argument);
					// TODO: error handling, see success variable.
				}
			}
		}

		return $arguments;
	}

	protected function checkJavascript() {

		if ($this->getAttribute("javascript") == null) {

			print "<script type=\"text/javascript\">
			window.location.replace(\"./index.php" . WebWidget::compileHrefArguments(array(new Attribute("setJavascript", "true"))) . "\");
			</script>";

			$this->setAttribute("javascript", "false");
		}
	}

	public function __construct() {

		// If the argument new_session=true is given, delete the current session.

		if ((isset($_GET["new_session"])) && ($_GET["new_session"] == "true")) {

			session_unset();
			unset($_GET["new_session"]);
		}

		// load contexts from disk.

		$this->contexts = $this->loadContexts();

		// process http Arguments

		$this->processHTTPArguments();
	}

	public function buildHTML() {

		$widgets = array();

		foreach($this->contexts as $i => $context) {

			if ($this->getContextAttribute($context, "edit") != null) {
				
				$id = $this->getContextAttribute($context, "edit");
				$widgets[] = $this->createEditor($context, $id);
			} else if ($this->getContextAttribute($context, "visible") == "true") {

				$flags = $this->getContextAttribute($context, "flags");
				if ($flags == null) $widgets[] = $this->createTable($context);
				else $widgets[] = $this->createTable($context, $flags);
			}
		}

		$html = "";
		$html .= $this->buildHeader();
		
		$this->checkJavascript();
		if ($this->getAttribute("javascript") == "true") $html .= $this->layoutJS($widgets);
		else $html .= $this->layoutNoJS($widgets);

		if (true) {

			$html .= "<div style=\"position:absolute;right:0px;top:0px;text-align:right;\">";
			foreach ($_SESSION as $key => $value) {
			
				if (is_array($value)) foreach ($value as $key2 => $value2) $html .= $key . "=" . $key2 . "," . $value2 . "<br>";
				else $html .= $key . "=" . $value . "<br>";
			}
			$html .= "</div>";
		}

		$html .= $this->buildFooter();

		return $html;
	}
}

$main = new Main();
print $main->buildHTML();

?>
