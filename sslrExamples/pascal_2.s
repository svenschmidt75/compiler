;PASCAL-Grammatik aus 'Von der Quelle zum professionellen Assemblertext'
;-----------------------------------------------------------------------

namelist
	:		namelist 'COMMA' identifier
		|	identifier


program
	:		'PROGRAM' identifier program_par 'SEMI' block 'POINT'


program_par
	:		'KLOPEN' name_list 'KLCLOSE'


block
	:		dec_list begin_stat


dec_list
	:		decl
		|	decl dec_list


decl
	:		constantdecl
		|	type_decl
		|	var_decl
		|	procedure_decl
		|	function_decl


constant_decl
	:		'CONST' const_list


const_list
	:		const
		|	const const_list


const
	:		Name 'EQU' const_value 'SEMI'


const_value
	:		'NUMBER'
		|	STRING_TEXT
		|	CONSTANT


txpe_decl
	:		'TYPE' txpe_decl_list


type_decl_list
	:		type
		|	type type_decl_list


type
	:		identifier 'EQU' type_spec 'SEMI'


type_spec
	:		simple_type
		|	range_type
		|	enum_type
		|	record_type
		|	array_type
		|	pointer_type
		|	string_type


simple_type
	:		typ_bez
		|	STRING


range_type
	:		border '..' border


border
	:		'NUMBER'
		|	CONSTANT


enum_type
	:		'KLOPEN' name_list 'KLCLOSE'


record_type
	:		'RECORD' comp_list 'END'


comp_list
	:		comp_list 'SEMI' variables
		|	variables


array_type
	:		'ARRAY' 'ARROPEN' comp_type_list 'ARRCLOSE' 'OF' type_spec


comp_type_list
	:		comp_type_list 'COMMA' comp_type
		|	comp_type


comp_type
	:		simple_type
		|	range_type


pointer_type
	:		'EXP' pointer_type_name


pointer_type_name
	:		identifier
		|	typ_bez
		|	Name_FORWARD


string_type
	:		'STRING' 'ARROPEN' 'NUMBER' 'ARRCLOSE'


var_decl
	:		'VAR' var_decl_list 'SEMI'


var_decl_lsit
	:		var_decl_list 'SEMI' variables
		|	variables


variables
	:		namelist ':' type_spec


procedure_decl
	:		'PROCEDURE' identifier fpar_decl 'SEMI' block 'SEMI'


function_decl
	:		'FUNCTION' identifier fpar_decl ':' simple_type 'SEMI' block 'SEMI'


fpar_decl
	:		'KLOPEN' fpar_decl 'KLCLOSE'


fpar_list
	:		fpar_list 'SEMI' fpar
		|	fpar


fpar
	:		call_by_value_par
		|	call_by_reference_par


call_by_reference_par
	:		'VAR' namelist ':' simple_type


stat_list
	:		stat_list 'SEMI' stat
		|	stat


stat
	:		assign_stat
		|	call_stat
		|	for_stat
		|	while_stat
		|	repeat_stat
		|	if_stat
		|	if_else_stat
		|	case_stat
		|	begin_stat
		|	with_stat


begin_stat
	:		'BEGIN' stat_list 'END'


assign_stat
	:		var_or_func 'BECOMES' expr


var_or_func
	:		variable
		|	Funktion


call_stat
	:		Prozedur apars
		|	StdProc apars


apars
	:		'KLOPEN' apar_list 'KLCLOSE'


apar_list
	:		apar_list 'COMMA' apar
		|	apar


apar
	:		expr
		|	expr ':' expr


for_stat
	:		'FOR' assign_stat direction expr 'DO' stat


direction
	:		'TO'
		|	'DOWNTO'


while_stat
	:		'WHILE' expr 'DO' stat


repeat_stat
	:		'REPEAT' stat_list 'UNTIL' expr


if_stat
	:		'IF' expr 'THEN' stat


if_else_stat
	:		'IF' expr 'THEN' stat 'ELSE' stat


case_stat
	:		'CASE' expr 'OF' case_label_list default_stat 'END'


case_label_list
	:		case_label_list case_label
		|	case_label


case_label
	:		const_list ':' stat 'SEMI'


const_list
	:		const_list 'COMMA' const_value
		|	const_value


default_stat
	:		'ELSE' stat_list


with_stat
	:		'WITH' variable 'DO' stat


variable
	:		array_variable
		|	record_variable
		|	arrow_variable
		|	simple_variable


array_variable
	:		variable 'ARROPEN' expr_list 'ARRCLOSE'


record_variable
	:		variable 'POINT' record_comp


arrow_variable
	:		variable 'EXP'


record_comp
	:		Variablenbezeichnung


simple_variable
	:		Variablenbezeichnung


expr_list
	:		expr_list 'COMMA' expr
		|	expr


expr
	:		simple_expr relop simple_expr
		|	simple_expr


simple_expr
	:		simple_expr addop term
		|	term


term
	:		term multop term
		|	term


mulop
	:		'*'
		|	'/'
		|	'DIV'
		|	'MUL'
		|	'AND'
		|	'SHL'
		|	'SHR'


addop
	:		'ADD'
		|	'SUB'
		|	'OR'
		|	'XOR'


relop
	:		'LESS'
		|	'LESSEQU'
		|	'EQU'
		|	'NOTEQU'
		|	'GREATER'
		|	'GREATEREQU'


factor
	:		const_value
		|	variable
		|	'NIL'
		|	'NOT' factor
		|	Funktion apars
		|	StdFunc apars
		|	'MINUS' factor


identifier
	:		'IDENTIFIER'