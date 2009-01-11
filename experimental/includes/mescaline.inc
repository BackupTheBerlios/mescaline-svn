<?php

require_once("peyote/peyote.inc");

require_once("mysqlcontext.inc");

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

class Mescaline extends Peyote {

// 	private function printContextbar() {
// 
// 		print "<div class=\"contextbar\">" . ($this->create_contexts ? "<div class=\"contextbar_item\">
// 		  <a href=\"./wizard/index.php\">
// 		  <img src=\"./icons/new.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"new\"></a></div>" : "" );
// 		if ($handle = opendir('./context')) {
// 
// 			$contextarray = array();
// 
// 			while (false !== ($file = readdir($handle))) {
// 
// 				if (substr($file, -8) == ".context") {
// 
// 					$contextname = substr($file, 0, -8);
// 					if ((!isset($_SESSION["show"][$contextname])) || ($_SESSION["show"][$contextname] != "true")) $contextarray[] = $contextname;
// 
// 				}
// 			}
// 
// 			foreach($contextarray as $i => $contextname) {
// 			
// 				print "<div class=\"contextbar_item\"><a href=\"./index.php?" . session_name() . "=" . session_id() . "&show:" . $contextname . "=true\">" . $contextname . "</a></div>";
// 				 if (end($contextarray) != $contextname) print "<div class=\"contextbar_item\">|</div>";	
// 			}
// 		}
// 		closedir($handle);
// 		print "</div>";
// 	}

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

	private function createError($caption, $domid, $error) { // TODO: probably move to peyote

		if ($this->multiple) return new WebError($caption, $domid, $error);
		else return new SingleWebError($caption, $domid, $error);
	}

	private function createEditor($context, $id) {

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
	}

	private function createTable($context, $flags = 0) {

		$table = new WebTable($context, $flags);

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

			// Add webfield array to content array.
			$content[] = $webfields;
		}

		$table->content = $content;
		return $table;
	}

	protected function processArguments($httpArguments) {

		foreach($httpArguments as $key => $value) { 

			//print $key . "=" . $value . "<br>"; 

			$splitArguments = split(",", $value, 2);

			$argumentCount = count($splitArguments);

			if ($argumentCount == 2) {

				$contextName = $splitArguments[0];
				$argument = $splitArguments[1];

				$context = $this->getContextByName($contextName);

				if (($key == "setVisible") && ($argument == "false")) $this->removeContextAttribute($context, "edit");
				else if ($key == "setFlags") $this->setContextAttribute($context, "flags", $argument);
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
	}

	public function createWidgets() {

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

		return $widgets;
	}
}

?>
