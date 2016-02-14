/*
;Kleine Sprache aus dem STC-Compilerkurs
;---------------------------------------
*/

%grammar


program		->	'VAR' var_list 'SEMICOLOEN' stat_list 'PRINT' ident
			;

var_list	->	ident
			|	var_list 'COMMA' ident
			;

stat_list	->	stat 'SEMICOLOEN'
			|	stat 'SEMICOLOEN' stat_list
			;

stat		->	assignment
			|	conditional
			|	loop
			;

assignment	->	ident 'EQU' expr
			;

conditional	->	'IF' expr 'THEN' stat_list 'ELSE' stat_list 'END'
			;

loop		->	'WHILE' expr 'DO' stat_list 'END'
			;

expr		->	simple_expr rel_op simple_expr
			|	simple_expr
			;

simple_expr	->	simple_expr add_op simple_expr
			|	product
			;

product		->	product mul_op primitive
			|	primitive
			;

primitive	->	'INT'
			|	'IDENT'
			|	'KOPEN' expr 'KCLOSE'
			;

rel_op		->	'EQU'
			|	'RAUTE'
			|	'LESS'
			|	'GREATER'
			|	'LESS_EQU'
			|	'GREATER_EQU'
			;

add_op		->	'ADD'
			|	'SUB'
			;

mul_op		->	'MUL'
			|	'DIV'
			|	'MOD'
			;
