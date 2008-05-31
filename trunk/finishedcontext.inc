<?php

if (!class_exists("WebContext")) include("webcontext.inc");

class FinishedContext extends WizardStep {

		public $context;

		public function content() {

		// There has to be a context yet.

		$contextfile = $_GET['context'] . ".wizard";
		$s = implode("", @file($contextfile));
		$this->context = unserialize($s);

		unlink($contextfile);

		$s = serialize($this->context);
		$fp = fopen($this->context->name . ".context", "w");
		fputs($fp, $s);
		fclose($fp);

			return "<b>Finished</b><br><br>
				Context <i>" . $this->context->name . "</i> is available";
		}
}

?>