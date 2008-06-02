<?php 

ini_set('include_path', '../includes/');

require_once("wizardstep.inc");
require_once("enterdbinfo.inc");
require_once("choosetable.inc");
require_once("chooseidfield.inc");
require_once("selectfields.inc");
require_once("editreferences.inc");
require_once("finishedcontext.inc");
require_once("choosereferencetable.inc");
require_once("choosereferenceidfield.inc");
require_once("choosereferencedisplayfield.inc");
require_once("editquery.inc");

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
$editquery->previous = $choosereferencedisplayfield;
$editquery->next = $editreferences;

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

?>
