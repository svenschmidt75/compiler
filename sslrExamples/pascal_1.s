/*
;PASCAL-Grammatik nach Wirth
;---------------------------
*/

%grammar



program
	:		program_headind block 'PERIOD'
	;

program_headind
	:		'PROGRAM' identifier 'KLOPEN' file_ident_loop 'KLCLOSE' 'SEMI'
	;

file_ident_loop
	:		file_identifier
		|	file_identifier 'COMMA' file_identifier_loop
	;

file_identifier
	:		identifier
	;

block
	:		label_declaration_part constant_definition_part type_definition_part variable_declaration_part procedure_and_funktion_declaration_part statement_part
	;

label_declaration_part
	:		'EPSILON'
		|	'LABEL' label label_loop 'SEMI'
	;

label_loop
	:		label_loop
		|	'COMMA' label
	;

label
	:		unsigned_integer
	;

constante_definition_part
	:		'EPSILON'
		|	'CONST' constant_definition constant_definition_loop 'SEMI'
	;

constant_definition_loop
	:		constant_definition_loop
		|	'COMMA' constant_definition
	;

constant_definition
	:		identifier 'EQU' constant
	;

constant
	:		unsigned_number
		|	sign unsigned_number
		|	constant_identifier
		|	sign constant_identifier
		|	string
	;

unsigned_number
	:		unsigned_integer
		|	unsigned real
	;

unsigned_integer
	:		digit_loop
		|	digit
	;

unsigned_real
	:		unsigned_integer 'POINT' digit digit_loop
		|	unsigned_integer 'POINT' digit digit_loop 'E' scale_factor
		|	unsigned_integer 'E' sacle_factor
	;

digit_loop
	:		digit_loop
		|	digit
	;

scale_factor
	:		unsigned_integer
		|	sign unsigned_integer
	;

sign
	:		'PLUS'
		|	'MINUS'
	;

constant_identifier
	:		identifier
	;

string
	:		'SLASH' character character_loop 'SLASH'
	;

character_loop
	:		character_loop
		|	character
	;

type_definietion_part
	:		'EPSILON'
		|	'TYPE' type_definition type_definition_loop
	;

type_definition_loop
	:		type_definition_loop
		|	'SEMI' type_definition
	;

type_definition
	:		identifier 'EQU' type
	;

type
	:		simple_type
		|	structured_type
		|	pointer_type
	;

simple_type
	:		scalar_type
		|	subrange_type
		|	type_identifier
	;

scalar_type
	:		'KLOPEN' identifier scalar_identifier_list 'KLCLOSE'
	;

scalar_identifier_list
	:		'COMMA' identifier
		|	scalar_identifier_list
	;

subrange_type
	:		constant 'UNKONOWN_TERM' constant
	;

type_identifier
	:		identifier
	;

structured_type
	:		unpacked_structured_type
		|	'PACKED' unpacked_structured_type
	;

unpacked_structured_type
	:		array_type
		|	record_type
		|	set_type
		|	file_type
	;

array_type
	:		'ARRAY' 'ARROPEN' index_type index_type_list 'ARRCLOSE' 'OF' component_type
	;

index_type_list
	:		'COMMA' index_type
		|	index_type_list
	;

index_type
	:		simple_type
	;

component_type
	:		type
	;

record_type
	:		'RECORD' field_list 'END'
	;

field_list
	:		fixed_part
		|	fixed_part 'SEMI' variant_part
		|	variant_part
	;

fixed_part
	:		record_section record_section_list
	;

record_section_list
	:		'SEMI' record_section
		|	record_section_list
	;

variant_part
	:		'CASE' tag_field type_identifier 'OF' variant variant_list
	;

variant_list
	:		'SEMI' variant
		|	variant_list
	;

tag_field
	:		field_identifier
		|	empty
	;

variant
	:		case_label_list 'DOPPELPUNKT' 'KLOPEN' field_list 'KLCLOSE'
		|	empty
	;

case_label_list
	:		case_label case_label_list_list
	;

case_label_list_list
	:		'COMMA' case_label
		|	case_label_list_list
	;

set_type
	:		'SET' 'OF' base_type
	;

base_type
	:		simple_type
	;

file_type
	:		'FILE' 'OF' type
	;

pointer_type
	:		'EXP' type_identifier
	;

variable_declaration_part
	:		'EPSILON'
		|	'VAR' variable_declaration variable_declaration_list 'SEMI'
	;

variable_declaration_list
	:		'COMMA' variable_declaration
		|	variable_declaration_list
	;

variable_declaration
	:		identifier identifier_list 'DOPPELPUNKT' type
	;

identifier_list
	:		'COMMA' identifier
		|	identifier_list
	;

procedure_and_function_declaration_part
	:		procedure_declaration
		|	function_declaration
	;

procedure_declaration
	:		procedure_headind block
	;

procedure_headind
	:		'PROCEDURE' identifier 'SEMI'
		|	'PROCEDURE' identifier 'KLOPEN' formal_parameter_section f_p_s_list 'KLCLOSE' 'SEMI'
	;

f_p_s_list
	:		'SEMI' formal_parameter_section
		|	f_p_s_list
	;

formal_parameter_section
	:		parameter_group
		|	'VAR' parameter_group
		|	'FUNCTION' parameter_group
		|	'PROCEDURE' identifier f_p_s_i_list
	;

f_p_s_i_list
	:		'COMMA' identifier
		|	f_p_s_i_list
	;

parameter_group
	:		identifier p_g_list 'DOPPELPUNKT' type_identifier
	;

p_g_list
	:		'COMMA' identifier
		|	p_g_list
	;

function_declaration
	:		function_heading block
	;

function_heading
	:		'FUNCTION' identifier 'DOPPELPUNKT' result_type 'SEMI'
		|	'FUNCTION' identifier 'KLOPEN' formal_parameter_section 'KLCLOSE' f_p_s_list
	;

f_p_s_list
	:		'SEMI' formal_parameter_section
		|	f_p_s_list
	;

result_type
	:		type_identifier
	;

statement_part
	:		compound_statement
	;

statement
	:		unlabelled_statement
		|	label 'DOPPELPUNKT' unlabelled_statement
	;

unlabelled_statement
	:		simple_statement
		|	structured_statement
	;

simple_statement
	:		assignment_statement
		|	procedure_statement
		|	goto_statement
		|	empty_statement
	;

empty_statement
	:		'EPSILON'
	;

assegment_statement
	:		variable 'BECOMES' expression
		|	function_identifier 'BECOMES' expression
	;

variable
	:		entire_variable
		|	component_variable
		|	referenced_variable
	;

entire_variable
	:		variable_identifier
	;

variable_identifier
	:		identifier
	;

component_variable
	:		indexed_variable
		|	field_designator
		|	file_buffer
	;

indexed_variable
	:		array_variable 'ARROPEN' expression i_v_list 'ARRCLOSE'
	;

i_v_list
	:		'COMMA' expression
		|	i_v_list
	;

array_variable
	:		variable
	;

field_designator
	:		record_variable 'POINT' field_identifier
	;

record_variable
	:		variable
	;

field_identifier
	:		identifier
	;

file_buffer
	:		file_variable 'EXP'
	;

file_varaible
	:		variable
	;

referenced_variable
	:		variable
	;

pointer_variable
	:		variable
	;

expression
	:		simple_expression
		|	simple_expression relational_operator simple_expression
	;

relational_operator
	:		'EQU'
		|	'NOTEQU'
		|	'LESS'
		|	'LESSEQU'
		|	'GREATER'
		|	'GREATEREQU'
		|	'IN'
	;

simple_expression
	:		term
		|	sign term
		|	simple_expression adding_operator term
	;

adding_operator
	:		'ADD'
		|	'SUB'
	;

term
	:		factor
		|	term multiplying_operator factor
	;

multipliying_operator
	:		'MUL'
		|	'DIV'
		|	'MOD'
		|	'AND'
	;

factor
	:		variable
		|	unsigned_constant
		|	'KLOPEN' expression 'KLCLOSE'
		|	fuction_designator
		|	set
		|	'NOT' factor
	;

unsigned_constant
	:		unsigned_number
		|	string
		|	constant_identifier
		|	'NIL'
	;

function_designator
	:		function_identifier
		|	function_identifier 'KLOPEN' actual_parameter f_d_list 'KLCLOSE'
	;

f_d_list
	:		'SEMI' actual_parameter
		|	fd_list
	;

function_identifier
	:		identifier
	;

set
	:		'ARROPEN' element_list 'ARRCLOSE'
	;

element_list
	:		element e_l_list
		|	'EPSILON'
	;

e_l_list
	:		'COMMA' element
		|	e_l_list
	;

elemnt
	:		expression
		|	expression 'UNKNOWN_TERM' expression
	;

procedure_statement
	:		procedure_identifier
		|	procedure_identifier 'KLOPEN' actual_parameter p_s_list 'KLCLOSE'
	;

p_s_list
	:		'COMMA' actual_parameter
		|	p_s_list
	;

procedure_identfier
	:		identifier
	;

actual_parameter
	:		expression
		|	variable
		|	procedure_identifier
		|	function_identifier
	;

go_to_statement
	:		'GOTO' label
	;

structured_statement
	:		compound_statement
		|	conditional_statement
		|	repetetive_statement
		|	width_statement
	;

compound_statement
	:		'BEGIN' statemnent c_s_list 'END'
	;

c_s_list
	:		'COMMA' statement
		|	c_s_list
	;

conditional_statement
	:		if_statement
		|	case_statement
	;

if_statement
	:		'IF' expression 'THEN' statement
		|	'IF' expression 'THEN' statement else statement
	;

case_statement
	:		'CASE' expression 'OF' case_list_element c_l_e_list 'END'
	;

c_l_e_list
	:		'SEMI' case_list_element
		|	c_l_e_list
	;


case_list_element
	:		case_label_list 'DOPPELPUNKT' statement
		|	'EPSILON'
	;

case_label_list
	:		case_label c_l_list
	;

c_l_list
	:		'COMMA' case_label
		|	case_label_list
	;

repetetive_statement
	:		while_statement
		|	repeat_statement
		|	for_statement
	;

while_statement
	:		'WHILE' expression 'DO' statement
	;

repeat_statement
	:		'REPEAT' statement r_s_list 'UNTIL' expression
	;

r_s_list
	:		'SEMI' statement
		|	r_s_list
	;

for_statement
	:		'FOR' control_variable 'BECOMES' for_list 'DO' statement
	;

for_list
	:		initial_value 'TO' final_value
		|	initial_value 'DOWNTO' final_value
	;

control_variable
	:		identifier
	;

initial_value
	:		expression
	;

final_value
	:		expression
	;

with_statement
	:		'WITH' record_variable_list 'DO' statement
	;

repeat_varaible_list
	:		record_variable r_v_list
	;

r_v_list
	:		'COMMA' record_variable
		|	r_v_list
	;
