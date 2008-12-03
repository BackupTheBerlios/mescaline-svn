<?php

class WhereClause {

  public $clause;

  function __construct($clause) {

	$this->clause = $clause;
  }

  function toString() {

	return $this->clause;
  }
  
}

?>