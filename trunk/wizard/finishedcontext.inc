<?php

if (!class_exists("WizardStep")) include("wizardstep.inc");
if (!class_exists("WebContext")) include("../webcontext.inc");

class FinishedContext extends WizardStep {

		public $context;

		public function content() {

		// There has to be a context yet.

		$s = implode("", @file($this->wizardFilename()));
		$this->context = unserialize($s);

		unlink($this->wizardFilename());

		$s = serialize($this->context);
		$fp = fopen("../context/" . $this->context->name . ".context", "w");
		fputs($fp, $s);
		fclose($fp);

			return "<b>Finished</b><br><br>
				Context <i>" . $this->context->name . "</i> is available";
		}
}

?>