<?php

require_once("attribute.inc");
require_once("contextattribute.inc");
require_once("version.inc");
require_once("context.inc");
require_once("webwidget.inc");

class Peyote {

	protected $contexts;

	protected function processArguments($httpArguments) {}

	protected function buildHeader() { 

		return "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">
			<html>
			<head>
			<title>peyote</title>
			<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">
			<link rel=stylesheet type=\"text/css\" href=\"css/default.css\">
			</head>
			<body onload=\"onLoad();\"><script type=\"text/javascript\" src=\"js/layout.js\"></script>";
	}

	protected function buildFooter() {

		return "</body></html>";
	}

	protected function dojo($widgets) {

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
	
	protected function layoutJS($widgets) {

		$html = "";

		// dojo magic

		$this->dojo($widgets);

		// show widgets

		ksort($widgets);
		foreach($widgets as $i => $widget) $html .= $widget->HTML();

		// Find out layouted and custom widgets

		$positionArguments = array();
		$layoutWidgets = array();

 		foreach($widgets as $key => $widget) $layoutWidgets[$widget->domid] = $widget;

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
		$html .= "} </script>";

		return $html;
	}

	protected function layoutNoJS($widgets) {

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

	protected function loadContexts() {

		$contexts = array();
		
		$handle = opendir("./context"); // TODO: error handling.
		while (false !== ($file = readdir($handle))) {

			if (substr($file, -8) == ".context") { // TODO: get rid of .context suffix.

				$s = implode("", @file("./context/" . $file));
				$contexts[] = unserialize($s); // TODO: error handling.
				// TODO: version checking!
			}
		}
		closedir($handle);

		return $contexts;
	}

	protected function getContextByName($name) {

		foreach($this->contexts as $i => $context) if ($context->name == $name) return $context;
		return null;
	}

	protected function processFrameworkArguments() {

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

				if ($key == "setVisible") $this->setContextAttribute($context, "visible", $argument);
				else if ($key == "setPosition") $this->setContextAttribute($context, "position", $argument);
			}
		}
		$this->processArguments($httpArguments);
	}

	protected function checkJavascript() {

		if ($this->getAttribute("javascript") == null) {

			print "<script type=\"text/javascript\">
			window.location.replace(\"./index.php?setJavascript=true\");
			</script>";

			$this->setAttribute("javascript", "false");
		}
	}

	public function createWidgets() {

		$widgets = array();

		foreach($this->contexts as $i => $context) {

			if ($this->getContextAttribute($context, "visible") == "true") $widgets[] = new WebWidget($context);
		}
		
		return $widgets;
	}

	public function buildHTML() {

		$widgets = $this->createWidgets();

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

	public function init() {

		// If the argument new_session=true is given, delete the current session.

		if ((isset($_GET["new_session"])) && ($_GET["new_session"] == "true")) {

			session_unset();
			unset($_GET["new_session"]);
		}

		// load contexts from disk.

		$this->contexts = $this->loadContexts();

		// process http Arguments

		$this->processFrameworkArguments();
	}
}

?>