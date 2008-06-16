<?php

require_once("context.inc");
require_once("webform.inc");
require_once("webtable.inc");
require_once("errormessage.inc");

require_once("webidfield.inc");
require_once("weblistfield.inc");
require_once("webboolfield.inc");
require_once("webstringfield.inc");

class WebContext {

	public $context;

	function __construct($context)
	{
		$this->context = $context;
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

		if ((isset($_GET['update'])) || (isset($_GET['insert'])) || (isset($_GET['remove']))) {

			$link = "index.php?context=" . $this->context->name
				. ((isset($_GET['sort'])) ? "&sort=" . $_GET['sort'] : "")
				. ((isset($_GET['hidden'])) ? "&hidden=" . $_GET['hidden'] : "")
				. ((isset($_GET['id'])) ? "#" . $_GET['id'] : "");

			$this->context->database->connect();
			if (isset($_GET['update'])) $success = $this->context->database->update($this->context->table, $this->extractFields()); 	
			else if (isset($_GET['insert'])) $success = $this->context->database->insert($this->context->table, $this->extractFields());
			else $success = $this->context->database->remove($this->context->table, $_GET['id']);
			if (!$success) {
				
				$errormessage = new ErrorMessage("SQL Error", $this->context->database->error(), $link);
				$errormessage->printHTML();
			} 
			else header("Location: ./" . $link);
			$this->context->database->disconnect();
		} else if (isset($_GET['edit'])) {
		
			// Show form
		
			$id = $_GET['id'];

			// Create webfield array.

			$webfields = $this->createWebFields();

			// Get Row.

			$row = array();

			$return = $this->context->database->buildRow($this->context->table, $row, $id);

			if ($return) { 

				// Populate WebFields.
	
				foreach ($row as $i => $value) $webfields[$i]->value = $value;
	
				$webform = new WebForm($this->context, $webfields, $id);
				$webform->printHTML();
			} else {

				$link = "index.php?context=" . $this->context->name;
				$errormessage = new ErrorMessage("SQL Error", $this->context->database->error(), $link);
				$errormessage->printHTML();
			}
		
		} else if (isset($_GET['new'])) {
		
			// Show form.
		
			// Create webfield array.

			$webfields = $this->createWebFields();

			$webform = new WebForm($this->context, $webfields);
			$webform->printHTML();
		
		} else {

			// Show table.
		
			// Check if sort is set.

			$sort = null;
        	if (isset($_GET['sort'])) $sort = $_GET['sort'];
		
			// Fetch rows from db, $rows is given to the function as reference.
	
			$rows = array();
			$result = $this->context->database->buildTable($this->context->table, $rows, $sort);

			if ($result == false) {
	
				$errormessage = new ErrorMessage("Failure in SQL query", $this->context->table->buildQuery($this->context->database), "index.php");
				$errormessage->printHTML();
			} else {
			
				// Create content array		.
		
				$content = array();
	
				foreach ($rows as $i => $row) {
	
					// Create webfield array.
	
					$webfields = $this->createWebFields();
	
					// Populate webfield array.
	
					foreach ($row as $i => $value) $webfields[$i]->value = $value;
	
					// Add webfield array to content array.
	
					$content[] = $webfields;
				}
	
				$webtable = new WebTable($this->context, $content);
				$webtable->printHTML();
			}
		}
	}

	// Build a *row* of webfields.

	protected function createWebFields() {

		$webfields = array();

		foreach ($this->context->table->fields as $i => $field) {

			if ($field instanceof IDField) $webfields[$i] = new WebIDField($field);
			else if ($field instanceof ListField) $webfields[$i] = new WebListField($field, $this->context->database);
			else if ($field instanceof BoolField) $webfields[$i] = new WebBoolField($field);
			else if ($field instanceof StringField) $webfields[$i] = new WebStringField($field);
			else $webfields[$i] = new WebField($field);
		} 

		return $webfields;
	}
}

?>