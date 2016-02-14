/*
;Grammatik zu MINI-C aus 'Praxis des Compilerbaus'
;-------------------------------------------------
*/

%grammar


program		->	'BLK_OPEN' stmt_list 'BLK_CLOSE'
			;

stmt_list	->	stmt_list
			|	stmt
			;

stmt		->	ident 'EQU' expr 'SEMICOLON'
			|	'PRINT' expr 'SEMICOLON'
			|	expr 'SEMICOLON'
			|	'WHILE' expr stmt
			|	'DO' 'BLK_OPEN' stmt_list 'BLK_CLOSE' 'WHILE'  expr
			|	'IF' expr stmt 'ELSE' stmt
			|	'IF' expr stmt
			|	'BLK_OPEN' stmt_list 'BLK_CLOSE'
			;

expr		->	'KOPEN' expr 'LESS' expr 'KCLOSE'
			|	'KOPEN' expr 'GREATER' expr 'KCLOSE'
			|	'KOPEN' expr 'LESS_EQU' expr 'KCLOSE'
			|	'KOPEN' expr 'GREATER_EQU' expr 'KCLOSE'
			|	'KOPEN' expr 'EQUAL' expr 'KCLOSE'
			|	'KOPEN' expr 'NOT_EQUAL' expr 'KCLOSE'
			|	expr 'ADD' term
			|	expr 'SUB' term
			|	term
			;

term		->	term 'MUL' factor
			|	term 'DIV' factor
			|	term 'MOD' factor
			|	factor
			;

factor		->	'KOPEN' expr 'KCLOSE'
			|	'NUMBER'
			|	'IDENT'
			;
