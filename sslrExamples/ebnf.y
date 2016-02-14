/*
	EBNF in EBNF selbst.
	aus "Compiler bauen mit Unix", Hanser Verlag
*/

%%

grammar
	:	grammar rule
	|	rule
	;

rule
	:	rule '|' formulation
	|	formulation
	;

formulation
	:	formulation symbol
	|	symbol
	;

symbol
	:	NTERM
	|	TERM
	|	'{' formulation '}'
	|	'[' formulation ']'
	|	'(' formulation ')'
	;
