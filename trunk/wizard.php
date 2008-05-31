<?php 

if (!class_exists("Table")) include("table.inc");
if (!class_exists("Field")) include("field.inc");
if (!class_exists("IDField")) include("idfield.inc");
if (!class_exists("Reference")) include("reference.inc");
if (!class_exists("ListField")) include("listfield.inc");
if (!class_exists("Database")) include("database.inc");
if (!class_exists("WebContext")) include("webcontext.inc");

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("EnterDBInfo")) include("enterdbinfo.inc");
if (!class_exists("ChooseTable")) include("choosetable.inc");
if (!class_exists("ChooseIDField")) include("chooseidfield.inc");
if (!class_exists("SelectFields")) include("selectfields.inc");
if (!class_exists("EditReferences")) include("editreferences.inc");
if (!class_exists("FinishedContext")) include("finishedcontext.inc");
if (!class_exists("ChooseReferenceTable")) include("choosereferencetable.inc");
if (!class_exists("ChooseReferenceIDField")) include("choosereferenceidfield.inc");
if (!class_exists("ChooseReferenceDisplayField")) include("choosereferencedisplayfield.inc");
if (!class_exists("EditQuery")) include("editquery.inc");

function printHTML($html) {

	print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"
		. "<html><head><title>wizard</title>"
		. "<style type=\"text/css\">"
		. ".brightline { background-color:#FFFFFF; }"
		. ".darkline { background-color:#DDDDDD; }"
		. "</style>"
		. "</head><body><div id=\"wizard\" align=\"center\">";
	print $html;
	print "</div></body></html>";
}

function Step1() {

	$dark = 1;

	$ret .= "<p><b>Step 1</b>: Enter the database configuration.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"2\">";
	
	$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Context:</td>";
	$dark = $dark ^ 1;
	$ret .= "<td><input name=\"context\" value=\"\"></td></tr>";

	$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Database:</td>";
	$dark = $dark ^ 1;
	$ret .= "<td><input name=\"database\" value=\"\"></td></tr>";	

	$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Hostname:</td>";
	$dark = $dark ^ 1;
	$ret .= "<td><input name=\"hostname\" value=\"\"></td></tr>";

	$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">User:</td>";
	$dark = $dark ^ 1;
	$ret .= "<td><input name=\"user\" value=\"\"></td></tr>";

	$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">Password:</td>";
	$dark = $dark ^ 1;
	$ret .= "<td><input name=\"password\" value=\"\"></td></tr>";

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step2($context, $dbname, $hostname, $user, $password) {

	$database = new Database($dbname, $hostname, $user, $password);
	$tables = getTables($database);
	$webcontext = new WebContext($context, null, $database); 

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 2</b>: Choose a table.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"3\">";
	
	foreach ($tables as $i => $table) { 
		
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\" name=\"table\" value=\"" . $i . "\">" . $table->toString() . "</td></tr>";
		$dark = $dark ^ 1;
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step3($context, $index) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);
	
	if ($webcontext->table == null) {
		
		$tables = getTables($webcontext->database);
		$table = $tables[$index];
		$webcontext->table = $table;
	}

	$fields = getFields($webcontext->database, $webcontext->table);
	$webcontext->table->fields = $fields;

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 3</b>: Choose the tables ID field.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"4\">";
	
	foreach ($fields as $i => $field) { 
		
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\" name=\"field\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
		$dark = $dark ^ 1;
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step4($context, $index) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);
	
	$fields = $webcontext->table->fields;
	$idfield = $fields[$index];
	$fields = replaceElement($idfield, new IDField($idfield->name), $fields);
	$webcontext->table->fields = $fields;

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 4</b>: Select the needed columns.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"5\">";

	// disabled boxes send no value obviously

	$ret .= "<input type=\"hidden\" name=\"0\" value=\"0\">";

	foreach ($webcontext->table->fields as $i => $field) { 
		
		if ($field instanceof IDField) $ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input disabled=\"disabled\" checked=\"checked\" type=\"checkbox\" name=\"" . $i . "\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
		else $ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"checkbox\" name=\"" . $i . "\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
		$dark = $dark ^ 1;
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step5($context) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);
	
	$selection = array();
	foreach ($webcontext->table->fields as $i => $field) if (isset($_GET[$i])) $selection[] = $field;
	$webcontext->table->fields = $selection;

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 5</b>: Edit fields which reference other tables.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"6\">";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context ."\">";

	foreach ($webcontext->table->fields as $i => $field) { 
		
		if (($field instanceof IDField) == false) { 
			$ret .= "<tr nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><td>" . $field->toString() . "</td><td><a href=\"./wizard.php?context=" . $context . "&step=5_1&field=" . $i ."\"><img src=\"edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a></td></tr>";
			$dark = $dark ^ 1;
		}
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step5_1($context, $fieldindex) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	$field = $webcontext->table->fields[$fieldindex];
	$webcontext->table->fields[$fieldindex] = new ListField($field->name, new Reference(null, null, null), $field->optional);
	$tables = getTables($webcontext->database);

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 5.1</b>: Choose the referenced table.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"5_2\">";
	$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $fieldindex . "\">";	

	foreach ($tables as $i => $table) { 
		
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\" name=\"table\" value=\"" . $i . "\">" . $table->toString() . "</td></tr>";
		$dark = $dark ^ 1;
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step5_2($context, $fieldindex, $tableindex) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	$tables = getTables($webcontext->database);
	$table = $tables[$tableindex];

	$webcontext->table->fields[$fieldindex]->reference->table = $table;
	$webcontext->table->fields[$fieldindex]->reference->idfield = new IDField(null);

	$fields = getFields($webcontext->database, $table);

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 5.2</b>: Select the referenced tables id field.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"5_3\">";
  $ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $fieldindex . "\">";		
	$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $tableindex . "\">";	

	foreach ($fields as $i => $field) { 
		
		$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\" name=\"idfield\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
		$dark = $dark ^ 1;
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step5_3($context, $fieldindex, $tableindex, $idfieldindex) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	// Get fields from referenced table 

	$tables = getTables($webcontext->database);
	$fields = getFields($webcontext->database, $tables[$tableindex]);

	// Set new ID field with new

	$webcontext->table->fields[$fieldindex]->reference->idfield = new IDField($fields[$idfieldindex]->name, $tables[$tableindex]);

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 5.3</b>: Select the referenced tables display field.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"5_4\">";
	$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $fieldindex . "\">";		
	$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $tableindex . "\">";		
	$ret .= "<input type=\"hidden\" name=\"idfield\" value=\"" . $tableindex . "\">";

	foreach ($fields as $i => $field) { 
		
		if ($i != $idfieldindex) { 
			$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><input type=\"radio\" name=\"displayfield\" value=\"" . $i . "\">" . $field->toString() . "</td></tr>";
			$dark = $dark ^ 1;
		}
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step5_4($context, $fieldindex, $tableindex, $idfieldindex, $displayindex, $error = false) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	// Get fields from referenced table 

	$tables = getTables($webcontext->database);
	$fields = getFields($webcontext->database, $tables[$tableindex]);

	// Set displayfield & update query

	$webcontext->table->fields[$fieldindex]->reference->displayfield = $fields[$displayindex];
	$webcontext->table->fields[$fieldindex]->query = $webcontext->table->fields[$fieldindex]->buildQuery($webcontext->database);

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 5.4</b>: Edit the list fields SQL query" . ($error ? "<font color=\"red\"> (The custom query has errors).</font>" : "." ) . "</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context . "\">";	
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"5_5\">";
	$ret .= "<input type=\"hidden\" name=\"field\" value=\"" . $fieldindex . "\">";		 
	$ret .= "<input type=\"hidden\" name=\"table\" value=\"" . $tableindex . "\">";		
	$ret .= "<input type=\"hidden\" name=\"idfield\" value=\"" . $tableindex . "\">";
	$ret .= "<input type=\"hidden\" name=\"displayfield\" value=\"" . $tableindex . "\">";

	$ret .= "<tr><td nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\">". $webcontext->table->fields[$fieldindex]->toString() . "</td>";
	$ret .= "<td><input name=\"query\" value=\"" . $webcontext->table->fields[$fieldindex]->query . "\"></td></tr>";

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step5_5($context, $fieldindex, $tableindex, $idfieldindex, $displayindex, $query) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	// Check query

	if (testQuery($webcontext->database, $query) == false) return Step5_4($context, $fieldindex, $tableindex, $idfieldindex, $displayindex, true);
	
	// Set new query

	$webcontext->table->fields[$fieldindex]->query = $query;

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".wizard", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;

	$ret .= "<p><b>Step 5</b>: Edit fields which reference other tables.</p>";
	$ret .= "<form action=\"./wizard.php\">";
	$ret .= "<table border=0 cellpadding=0 cellspacing=0>";
	$ret .= "<input type=\"hidden\" name=\"step\" value=\"6\">";
	$ret .= "<input type=\"hidden\" name=\"context\" value=\"" . $context ."\">";

	foreach ($webcontext->table->fields as $i => $field) { 
		
		if (($field instanceof IDField) == false) {
			$ret .= "<tr nowrap=\"nowrap\" class=\"" . ($dark ? "darkline" : "brightline") . "\"><td>" . $field->toString() . "</td><td><a href=\"./wizard.php?context=" . $context . "&step=5_1&field=" . $i ."\"><img src=\"edit.png\" width=\"20\" height=\"20\" border=\"0\" style=\"vertical-align:middle\" alt=\"edit\"></a></td></tr>";
			$dark = $dark ^ 1;
		}
	}

	$ret .= "</table><br>";
	$ret .= "<input type=\"image\" src=\"ok.png\" alt=\"ok\"></form>";

	return $ret;
}

function Step6($context) {

	$contextfile = $context . ".wizard";

	$s = implode("", @file($contextfile));
	$webcontext = unserialize($s);

	$s = serialize($webcontext);
	$fp = fopen($webcontext->name . ".context", "w");
	fputs($fp, $s);
	fclose($fp);

	$dark = 1;
}

function removeElement($remove, $array) {

	foreach ($array as $i => $element) if ($remove == $element) unset($array[$i]);

	return $array;
}

function replaceElement($old, $new, $array) {

	foreach ($array as $i => $element) if ($old == $element) $array[$i] = $new;

	return $array;
}

function choose($array, $msg) {
	
	foreach($array as $i => $element) print "[" . $i . "] " . $element->toString() ."\n";
	
	print $msg;

	$input = trim(fgets(STDIN));

	return $array[$input];
}

function select($oldarray, $msg) {

	$newarray = array();

	while (true) {
	
		foreach ($oldarray as $i => $element) print "[" . $i . "] [" . ((array_key_exists($i, $newarray)) ? "x" : " ") . "] " . $element->toString() . "\n";

		print $msg;
		$input = trim(fgets(STDIN));
		if ($input == "") break;
		if (array_key_exists($input, $newarray)) unset($newarray[$input]);
		else $newarray[$input] = $oldarray[$input];
	}

	return $newarray;
}

function testQuery($database, $query) {

	$database->connect();
	$result = mysql_query($query, $database->link);
	$database->disconnect();

	if ($result == false) return false;
	else return true;
}

function getTables($database) {

	$database->connect();
	$tables = $database->tables();
	$database->disconnect();

	return $tables;
}

function getFields($database, $table) {

	$database->connect();
	$fields = $database->columns($table);
	$database->disconnect();

	return $fields;
}

$nextstep = new WizardStep("next1");
$previousstep = new WizardStep("previous1");

$enterdbinfo = new EnterDBInfo("enterdbinfo");
$choosetable = new ChooseTable("choosetable");
$chooseidfield = new ChooseIDField("chooseidfield");
$selectfields = new SelectFields("selectfields");
$editreferences = new EditReferences("editreferences");
$finishedcontext = new FinishedContext("finishedcontext");
$choosereferencetable = new ChooseReferenceTable("choosereferencetable");
$choosereferenceidfield = new ChooseReferenceIDField("choosereferenceidfield");
$choosereferencedisplayfield = new ChooseReferenceDisplayField("choosereferencedisplayfield");
$editquery = new EditQuery("editquery");

$enterdbinfo->next = $choosetable;
$choosetable->previous = $enterdbinfo;
$choosetable->next = $chooseidfield;
$chooseidfield->previous = $choosetable;
$chooseidfield->next = $selectfields;
$selectfields->previous = $chooseidfield;
$selectfields->next = $editreferences;
$editreferences->previous = $selectfields;
$editreferences->next = $finishedcontext;
$choosereferencetable->previous = $editreferences;
$choosereferencetable->next = $choosereferenceidfield;
$choosereferenceidfield->previous = $choosereferencetable;
$choosereferenceidfield->next = $choosereferencedisplayfield;
$choosereferencedisplayfield->previous = $choosereferenceidfield;
$choosereferencedisplayfield->next = $editquery;

$dialogs = array();

$dialogs[$enterdbinfo->name] = $enterdbinfo;
$dialogs[$choosetable->name] = $choosetable;
$dialogs[$chooseidfield->name] = $chooseidfield;
$dialogs[$selectfields->name] = $selectfields;
$dialogs[$editreferences->name] = $editreferences;
$dialogs[$finishedcontext->name] = $finishedcontext;
$dialogs[$choosereferencetable->name] = $choosereferencetable;
$dialogs[$choosereferenceidfield->name] = $choosereferenceidfield;
$dialogs[$choosereferencedisplayfield->name] = $choosereferencedisplayfield;
$dialogs[$editquery->name] = $editquery;

if (isset($_GET['dialog'])) {

	$dialog = $dialogs[$_GET['dialog']];
	$dialog->printHTML();

} else {

	$enterdbinfo->printHTML();
}


/*if (isset($_GET['step'])) {

	$step = $_GET['step'];

	if ($step == "1") printHTML(Step1());
	else if ($step == "2") printHTML(Step2($_GET['context'], $_GET['database'], $_GET['hostname'], $_GET['user'], $_GET['password']));
	else if ($step == "3") printHTML(Step3($_GET['context'], $_GET['table']));
	else if ($step == "4") printHTML(Step4($_GET['context'], $_GET['field']));
	else if ($step == "5") printHTML(Step5($_GET['context']));
	else if ($step == "5_1") printHTML(Step5_1($_GET['context'], $_GET['field']));
	else if ($step == "5_2") printHTML(Step5_2($_GET['context'], $_GET['field'], $_GET['table']));
	else if ($step == "5_3") printHTML(Step5_3($_GET['context'], $_GET['field'], $_GET['table'], $_GET['idfield']));
	else if ($step == "5_4") printHTML(Step5_4($_GET['context'], $_GET['field'], $_GET['table'], $_GET['idfield'], $_GET['displayfield']));
	else if ($step == "5_5") printHTML(Step5_5($_GET['context'], $_GET['field'], $_GET['table'], $_GET['idfield'], $_GET['displayfield'], $_GET['query']));
	else if ($step == "6") {
	
		Step6($_GET['context']);
		printHTML("<p>Finished. Context " . $_GET['context'] . " is available.</p>");
	}
}
else printHTML(Step1());*/

?>