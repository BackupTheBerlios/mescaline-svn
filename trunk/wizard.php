<?php 

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