;Pascal-Untermenge aus dem 'Drachenbuch'
;---------------------------------------

program
	:		'PROGRAM' ident 'KOPEN' identifier_list 'KCLOSE' 'SEMICOLON' declaration
			subprogram_declarations compound_statement


identifier_list
	:		ident
		|	identifier_list 'IDENT'


declarations
	:		declarations 'VAR' identifier_list 'DBL_POINT' type 'SEMICOLON'
		|	'EPSILON'


type
	:		standart_type
		|	'ARRAY' 'ARRAY_OPEN' NUMBER '..' NUMBER 'ARRAY_CLOSE' 'OF' standart_type


standart_type
	:		`INTEGER'
		|	'LONG'
		|	'SHORT'


subprogram_declarations
	:		subprogram_declarations subprogram_declaration 'SEMICOLON'
		|	'EPSILON'


subprogram_declaration
	:		subprogram_head declarations compound_statement


subprogram_head
	:		'FUNCTION' ident argument 'DBL_POINT' standart_type
		|	'PROCEDURE' ident arguments 'SEMICOLON'


arguments
	:		'KOPEN' parameter_list 'KCLOSE'
		|	'EPSILON'


parameter_list
	:		identifier_list 'DBL_POINT' type
		|	parameter_list 'SEMICOLON' identifier_list 'DBL_POINT' type


compound_statement
	:		'BEGIN' optional_statement 'END'


optional_statement
	:		statement_list
		|	'EPSILON'


statement_list
	:		statement
		|	statament_list 'SEMICOLON' statement


statement
	:		variable 'EQU' expression
		|	procedure_statement
		|	compound_statement
		|	'IF' expression 'THEN' statement
		|	'IF' expression 'THEN' statement 'ELSE' statement
		|	'WHILE' expression 'DO' statement 'END'
		|	'REPEAT' statement 'UNTIL' expression


variable
	:		ident
		|	ident 'ARRAY_OPEN' expression 'ARRAY_CLOSE'


procedure_statement
	:		ident
		|	ident 'KOPEN' expression_list 'KCLOSE'


expression_list
	:		expression
		|	expression_list 'COMMA' expression


expression
	:		simple_expression
		|	simple_expression 'LESS' simple_expression
		|	simple_expression 'GREATER' simple_expression
		|	simple_expression 'NOT_EQUAL<>' simple_expression
		|	simple_expression 'LESS_EQU' simple_expression
		|	simple_expression 'GREATER_EQU' simple_expression


simple_expression
	:		term
		|	sign term
		|	simple_expression 'ADD' simple_expression
		|	simple_expression 'SUB' simple_expression
		|	simple_expression 'OR' simple_expression
		|	simple_expression 'XOR' simple_expression
		|	simple_expression 'AND' simple_expression


term
	:		factor
		|	term 'MUL' factor
		|	term 'DIV' factor
		|	term 'MOD' factor
		|	term 'SHL' factor
		|	term 'SHR' factor


factor
	:		'IDENT'
		|	'IDENT' 'KOPEN' expression_list 'KCLOSE'
		|	'NUMBER'
		|	'KOPEN' expression 'KCLOSE'
		|	'NOT' factor


sign
	:		'PLUS'
		|	'MINUS'