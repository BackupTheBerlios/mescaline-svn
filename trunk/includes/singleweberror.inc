<?php

require_once("weberror.inc");

class SingleWebError extends WebError
{

	function __construct($caption, $domid, $error, $link) {

		$this->moveable = true;
		$this->caption = $caption;
		$this->domid = $domid;
		$this->error = $error;
		$this->link = $link;
	}

	protected function buildHandle() {}
}

?>