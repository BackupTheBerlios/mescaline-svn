<?php

class WhereCombination extends WhereClause {

  public $combination;
  public $left;
  public $right;

  function __construct($left, $combination, $right)
  {
	$this->clause = null;
	$this->left = $left;
	$this->combination = $combination;
	$this->right = $right;
  }

 function toString() {

	return "(" . $this->left->toString() . ") " . $this->combination . " (" . $this->right->toString() . ")";
  }
}