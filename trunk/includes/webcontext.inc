<?php

require_once("context.inc");
require_once("webform.inc");
require_once("webtable.inc");

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

		if (isset($_GET['remove'])) {
	
			$this->context->database->connect();
			$this->context->database->remove($this->context->table, $_GET['id']);
			$this->context->database->disconnect();
		
			header("Location: ./index.php?context=" . $this->context->name);
		
		} else if (isset($_GET['update'])) {
			
			$this->context->database->connect();	
			$this->context->database->update($this->context->table, $this->extractFields());
			$this->context->database->disconnect();

			header("Location: ./index.php?context=" . $this->context->name);
		
		} else if (isset($_GET['insert'])) {
			
			$this->context->database->connect();
			$this->context->database->insert($this->context->table, $this->extractFields());
			$this->context->database->disconnect();
		
			header("Location: ./index.php?context=" . $this->context->name);
		
		} else if (isset($_GET['edit'])) {
		
			// Show form
		
			$id = $_GET['id'];

			// Create webfield array.

			$webfields = $this->createWebFields();

			// Get Row.

			$row = $this->context->database->buildRow($this->context->table, $id);

			// Populate WebFields.

			foreach ($row as $i => $value) $webfields[$i]->value = $value;

			$webform = new WebForm($this->context, $webfields, $id);
			$webform->printHTML();
		
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
		
			// Fetch rows from db.
	
			$rows = $this->context->database->buildTable($this->context->table, $sort);

			if ($rows == false) {
	
				return "ERROR: table could not be build from query: " . $this->context->table->buildQuery($this->context->database);
			}

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

	protected function createWebFields() {

		$webfields = array();

		foreach ($this->context->table->fields as $i => $field) {

			if ($field instanceof IDField) $webfields[$i] = new WebIDField($field);
			else if ($field instanceof ListField) $webfields[$i] = new WebListField($field, $this->context->database);
			else if ($field instanceof BoolField) $webfields[$i] = new WebBoolField($field);
			else $webfields[$i] = new WebField($field);
		} 

		return $webfields;
	}
}

?>