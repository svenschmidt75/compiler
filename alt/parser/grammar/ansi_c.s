/*
ANSI-C-Grammatik aus 'Praxis des Compilerbaus'
----------------------------------------------
*/


%grammar


/*********************/
/* Grundproduktionen */
/*********************/


module
	:		file
	;


file
	:		external_definition
		|	file external_definition
	;


external_definition
	:		function_definition
		|	declaration
	;


function_definition
	:		function_declarator function_body
	;


function_declarator
	:		declarator
		|	declaration_specifiers declarator
	;


function_body
	:		compound_statement
		|	declaration_list compound_statement
	;



/*****************/
/* Deklarationen */
/*****************/


declaration_list
	:		declaration
		|	declaration_list declaration
	;


declaration
	:		declaration_specifiers 'SEMICOLON'
		|	declaration_specifiers init_declarator_list 'SEMICOLON'
	;


init_declarator_list
	:		init_declarator
		|	init_declarator_list 'COMMA' init_declarator
	;


init_declarator
	:		declarator
		|	declarator 'EQU' initializer
	;


declaration_specifiers
	:		type_definition
		|	storage_class_specifier
		|	type_specifier
		|	type_qualifier
		|	declaration_specifiers type_definition
		|	declaration_specifiers storage_class_specifier
		|	declaration_specifiers type_specifier
		|	declaration_specifiers type_qualifier
	;


type_specifier_list
	:		type_specifier
		|	type_specifier_list type_specifier
	;


type_definition
	:		'TYPEDEF'
	;


storage_class_specifier
	:		'EXTERN'
		|	'STATIC'
		|	'AUTO'
		|	'REGISTER'
	;


type_specifier
	:		'CHAR'
		|	'SHORT'
		|	'INT'
		|	'LONG'
		|	'SIGNED'
		|	'UNSIGNED'
		|	'FLOAT'
		|	'DOUBLE'
		|	'VOID'
		|	struct_or_union_specifier
		|	enum_specifier
		|	typedef_name
	;

typedef_name
	:		'IDENTIFIER'
	;


type_qualifier
	:		'CONST'
		|	'VOLATILE'
	;


initializer_list
	:		initializer
		|	initializer_list 'COMMA' initializer
	;


initializer
	:		assignment_expr
		|	'BLK_OPEN' initializer_list 'BLK_CLOSE'
		|	'BLK_OPEN' initializer_list 'COMMA' 'BLK_CLOSE'
	;


declarator
	:		direct_declarator
		|	pointer direct_declarator
	;


direct_declarator
	:		'IDENTIFIER'
		|	'KOPEN' declarator 'KCLOSE'
		|	direct_declarator 'ARRAY_OPEN' 'ARRAY_CLOSE'
		|	direct_declarator 'ARRAY_OPEN' constant_expr 'ARRAY_CLOSE'
		|	direct_declarator 'KOPEN' 'KCLOSE'
		|	direct_declarator 'KOPEN' parameter_type_list 'KCLOSE'
		|	direct_declarator 'KOPEN' parameter_identifier_list 'KCLOSE'
	;


pointer
	:		'MUL'
		|	'MUL' type_specifier_list
		|	'MUL' pointer
		|	'MUL' type_specifier_list pointer
	;


parameter_identifier_list
	:		identifier_list
		|	identifier_list 'COMMA' 'PPP'
	;


identifier_list
	:		'IDENTIFIER'
		|	identifier_list 'COMMA' 'IDENTIFIER'
	;


parameter_type_list
	:		parameter_list
		|	parameter_list 'COMMA' 'PPP'
	;


parameter_list
	:		parameter_declarator
		|	parameter_list 'COMMA' parameter_declarator
	;


parameter_declarator
	:		type_specifier_list declarator
		|	type_name
	;


type_name
	:		type_specifier_list
		|	type_specifier_list abstract_declarator
	;


abstract_declarator
	:		pointer
		|	abstract_direct_declarator
		|	pointer abstract_direct_declarator
	;


abstract_direct_declarator
	:		'KOPEN' abstract_declarator 'KCLOSE'
		|	'ARRAY_OPEN' 'ARRAY_CLOSE'
		|	'ARRAY_OPEN' constant_expr 'ARRAY_CLOSE'
		|	abstract_direct_declarator 'ARRAY_OPEN' 'ARRAY_CLOSE'
		|	abstract_direct_declarator 'ARRAY_OPEN' constant_expr 'ARRAY_CLOSE'
		|	'KOPEN' 'KCLOSE'
		|	'KOPEN' parameter_type_list 'KCLOSE'
		|	abstract_direct_declarator 'KOPEN' 'KCLOSE'
		|	abstract_direct_declarator 'KOPEN' parameter_type_list 'KCLOSE'
	;


struct_or_union_specifier
	:		struct_or_union 'IDENTIFIER' 'BLK_OPEN' struct_declaration_list 'BLK_CLOSE'
		|	struct_or_union 'BLK_OPEN' struct_declaration_list 'BLK_CLOSE'
		|	struct_or_union 'IDENTIFIER'
	;


struct_or_union
	:		'STRUCT'
		|	'UNION'
	;


struct_declaration_list
	:		struct_declaration
		|	struct_declaration_list struct_declaration
	;


struct_declaration
	:		type_specifier_list struct_declarator_list 'SEMICOLON'
	;


struct_declarator_list
	:		struct_declarator
		|	struct_declarator_list 'COMMA' struct_declarator
	;


struct_declarator
	:		declarator
		|	'DBL_POINT' constant_expr
		|	declarator 'DBL_POINT' constant_expr
	;


enum_specifier
	:		'ENUM' 'BLK_OPEN' enumerator_list 'BLK_CLOSE'
		|	'ENUM' 'IDENTIFIER' 'BLK_OPEN' enumerator_list 'BLK_CLOSE'
		|	'ENUM' 'IDENTIFIER'
	;


enumerator_list
	:		enumerator
		|	enumerator_list 'COMMA' enumerator
	;


enumerator
	:		'IDENTIFIER'
		|	'IDENTIFIER' 'EQU' constant_expr
	;


/***************/
/* Anweisungen */
/***************/


statement_list
	:		statement
		|	statement_list statement
	;


statement
	:		labeled_statement
		|	compound_statement
		|	expression_statement
		|	selection_statement
		|	iteration_statement
		|	jump_statement
	;


labeled_statement
	:		'IDENTIFIER' 'DBL_POINT' statement
		|	'CASE' constant_expr 'DBL_POINT' statement
		|	'DEFAULT' 'DBL_POINT' statement
	;


compound_statement
	:		'BLK_OPEN' 'BLK_CLOSE'
		|	'BLK_OPEN' statement_list 'BLK_CLOSE'
		|	'BLK_OPEN' declaration_list 'BLK_CLOSE'
		|	'BLK_OPEN' declaration_list statement_list 'BLK_CLOSE'
	;


expression_statement
	:		'SEMICOLON'
		|	expr 'SEMICOLON'
	;


selection_statement
	:		'IF' 'KOPEN' expr 'KCLOSE' statement
		|	'IF' 'KOPEN' expr 'KCLOSE' statement 'ELSE' statement
		|	'SWITCH' 'KOPEN' expr 'KCLOSE' statement
	;


iteration_statement
	:		'WHILE' 'KOPEN' expr 'KCLOSE' statement
		|	'DO' statement 'WHILE' 'KOPEN' expr 'KCLOSE' 'SEMICOLON'
		|	'FOR' 'KOPEN' 'SEMICOLON' 'SEMICOLON' 'KCLOSE' statement
		|	'FOR' 'KOPEN' 'SEMICOLON' 'SEMICOLON' expr 'KCLOSE' statement
		|	'FOR' 'KOPEN' 'SEMICOLON' expr 'SEMICOLON' 'KCLOSE' statement
		|	'FOR' 'KOPEN' 'SEMICOLON' expr 'SEMICOLON' expr 'KCLOSE' statement
		|	'FOR' 'KOPEN' expr 'SEMICOLON' 'SEMICOLON' 'KCLOSE' statement
		|	'FOR' 'KOPEN' expr 'SEMICOLON' 'SEMICOLON' expr 'KCLOSE' statement
		|	'FOR' 'KOPEN' expr 'SEMICOLON' expr 'SEMICOLON' 'KCLOSE' statement
		|	'FOR' 'KOPEN' expr 'SEMICOLON' expr 'SEMICOLON' expr 'KCLOSE' statement
	;


jump_statement
	:		'GOTO' 'IDENTIFIER' 'SEMICOLON'
		|	'CONTINUE' 'SEMICOLON'
		|	'BREAK' 'SEMICOLON'
		|	'RETURN' 'SEMICOLON'
		|	'RETURN' expr 'SEMICOLON'
	;


/*************/
/* AusdrÅcke */
/*************/


primary_expr
	:		'CONSTANT'
		|	'IDENTIFIER'
		|	'STRING'
		|	'KOPEN' expr 'KCLOSE'
	;


postfix_expr
	:		primary_expr
		|	postfix_expr 'ARRAY_OPEN' expr 'ARRAY_CLOSE'
		|	postfix_expr 'KOPEN' 'KCLOSE'
		|	postfix_expr 'KOPEN' argument_expr_list 'KCLOSE'
		|	postfix_expr 'POINT' 'IDENTIFIER'
//		|	postfix_expr 'ARRAY_OPEN' expr 'ARRAY_CLOSE'
		|	postfix_expr 'ARROW' 'IDENTIFIER'
		|	postfix_expr 'ADD_ADD'
		|	postfix_expr 'SUB_SUB'
	;


unary_expr
	:		postfix_expr
		|	'ADD_ADD' unary_expr
		|	'SUB_SUB' unary_expr
		|	unary_operator cast_expr
		|	'SIZEOF' unary_expr
		|	'SIZEOF' 'KOPEN' type_name 'KCLOSE'
	;


unary_operator
	:		'AND'
		|	'MUL'
		|	'ADD'
		|	'SUB'
		|	'NEG'
		|	'NOT'
	;


cast_expr
	:		unary_expr
		|	'KOPEN' type_name 'KCLOSE' cast_expr
	;


multiplicative_expr
	:		cast_expr
		|	multiplicative_expr 'MUL' cast_expr
		|	multiplicative_expr 'DIV' cast_expr
		|	multiplicative_expr 'MOD' cast_expr
	;


additive_expr
	:		multiplicative_expr
		|	shift_expr 'LESS_LESS' additive_expr
		|	shift_expr 'GREATER_GREATER' additive_expr
	;


shift_expr
	:		additive_expr
		|	shift_expr 'LEFT_OP' additive_expr
		|	shift_expr 'RIGHT_OP' additive_expr
	;


relational_expr
	:		shift_expr
		|	relational_expr 'LESS' shift_expr
		|	relational_expr 'GREATER' shift_expr
		|	relational_expr 'LESS_EQU' shift_expr
		|	relational_expr 'GREATER_EQU' shift_expr
	;


equality_expr
	:		relational_expr
		|	equality_expr 'EQUAL' relational_expr
		|	equality_expr 'NOT_EQUAL' relational_expr
	;


and_expr
	:		equality_expr
		|	and_expr 'AND' equality_expr
	;


exclusive_or_expr
	:		and_expr
		|	exclusive_or_expr 'XOR' and_expr
	;


inclusive_or_expr
	:		exclusive_or_expr
		|	inclusive_or_expr 'OR' exclusive_or_expr
	;


logical_and_expr
	:		inclusive_or_expr
		|	logical_and_expr 'AND_AND' inclusive_or_expr
	;


logical_or_expr
	:		logical_and_expr
		|	logical_or_expr 'OR_OR' logical_and_expr
	;


conditional_expr
	:		logical_or_expr
		|	logical_or_expr 'QM' expr 'DBL_POINT' conditional_expr
	;


assignment_expr
	:		conditional_expr
		|	unary_expr assignment_operator assignment_expr
	;


assignment_operator
	:		'EQU'		// =
		|	'MUL_EQU'	// *=
		|	'DIV_EQU'	// /=
		|	'MOD_EQU'	// %=
		|	'ADD_EQU'	// +=
		|	'SUB_EQU'	// -=
		|	'AND_EQU'	// &=
		|	'XOR_EQU'	// ^=
		|	'OR_EQU'	// |=
		|	'SHL_EQU'	// <<=
		|	'SHR_EQU'	// >>=
	;


expr
	:		assignment_expr
		|	expr 'COMMA' assignment_expr
	;


constant_expr
	:		conditional_expr
	;


argument_expr_list
	:		assignment_expr
		|	argument_expr_list 'SEMICOLON' assignment_expr
	;
