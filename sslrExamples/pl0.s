//PL/0-Grammatik in EBNF

%definition

%grammar


program		:	block 'PERIOD'
			;

block		:	[ 'CONST' 'IDENT' 'EQU' 'NUMBER' { 'COMMA' 'IDENT' 'EQU' 'NUMBER' } 'SEMI' ]
			|	[ 'VAR' 'IDENT' { 'COMA' 'IDENT' } 'SEMI' ]
			|	{ 'PROCEDURE' 'IDENT' 'SEMI' block 'SEMI' } statement
			;

statement	:	[ 'IDENT' 'BECOMES' expr
			|	'CALL' 'IDENT'
			|	'BEGIN' statement { 'SEMI' statement } 'END'
			|	'IF' condition 'THEN' statement
			|	'WHILE' condition 'DO' statement ]
			;

condition	:	'ODD' expr
			|	expr ( 'EQU' | '#' | 'LESS' | 'LESS_EQU' | 'GREATER' | 'GREATER_EQU' ) expr
			;

expr		:	[ 'ADD' | 'SUB' ] term { ( 'ADD' | 'SUB' ) term }
			;

term		:	factor { ( 'MUL' | 'DIV' ) factor }
			;

factor		:	'IDENT'
			|	'NUMBER'
			|	'KLOPEN' expr 'KLCLOSE'
			;
