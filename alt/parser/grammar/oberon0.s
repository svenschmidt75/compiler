/*
	Oberon-0-Grammatik aus N. Wirth: Grundlagen und Techniken des Compilerbaus
	22.08.02, Sven Schmidt
*/

%grammar



ident
	->	letter { letter | digit }
	;

digit
	->	'ZERO'
	|	'ONE'
	|	'ZWO'
	|	'THREE'
	|	'FOUR'
	|	'FIVE'
	|	'SIX'
	|	'SEVEN'
	|	'EIGHT'
	|	'NINE'
	;

letter
	->	'LETTER'
	;

integer
	->	digit { digit }
	;

selector
	->	{ 'DOT' ident | 'KOPEN' expression 'KCLOSE' }
	;

factor
	->	ident selector
	|	integer
	|	'KLOPEN' expression 'KLCLOSE'
	|	'TILDE' factor
	;

term
	->	factor { ( 'MUL' | 'DIV' | 'MOD' | 'AND' ) factor }
	;

SimpleExpression
	->	[ 'AND' | 'SUB' ] term { ( 'ADD' | 'SUB' | 'OR' ) term }
	;

expression
	->	SimpleExpression [ ( 'EQUAL' | 'RAUTE' | 'LESS' | 'LESS_EQUAL'
			| 'GREATER' | 'GREATER_EQUAL' ) SimpleExpression ]
	;

assignment
	->	ident selector 'ASSIGN' expression
	;

ActualParameters
	->	'KLOPEN' [ expression { 'KOMMA' expression } ] 'KLCLOSE'
	;

ProcedureCall
	->	ident [ ActualParameters ]
	;

IfStatement
	->	'IF' expression 'THEN' StatementSequence
	{ 'ELSIF' expression 'THEN' StatementSequence }
	[ 'ELSE' StatementSequence ] 'END'
	;

WhileStatement
	->	'WHILE' expression 'DO' StatementSequence 'END'
	;

RepeatStatement
	->	'REPEAT' StatementSequence 'UNTIL' expression
	;

statement
	->	[ assignment | ProcedureCall | IfStatement | WhileStatement | RepeatStatement ]
	;

StatementSequence
	->	statement { 'SEMI' statement }
	;

IdentList
	->	ident { 'KOMMA' ident }
	;

ArrayType
	->	'ARRAY' expression 'OF' type
	;

FieldList
	->	[ IdentList 'DOPPELPUNKT' type ]
	;

RecordType
	->	'RECORD' FieldList { 'SEMI' FieldList } 'END'
	;

type
	->	ident
	|	ArrayType
	|	RecordType
	;

FPSection
	->	[ 'VAR' ] IdentList 'DOPPELPUNKT' type
	;

FormalParameters
	->	'KLOPEN' [ FPSection { 'SEMI' FPSection } ] 'KLCLOSE'
	;

ProcedureHeading
	->	'PROCEDURE' ident [ FormalParameters ]
	;

ProcedureBody
	->	declarations [ 'BEGIN' StatementSequence ] 'END'
	;

ProcedureDeclaration
	->	ProcedureHeading 'SEMI' ProcedureBody ident
	;

declarations
	->	[ 'CONST' { ident 'EQUAL' expression 'SEMI' } ]
		[ 'TYPE' { ident 'EQUAL' type 'SEMI' } ]
		[ 'VAR' { IdentList 'DOPPELPUNKT' type 'SEMI' } ]
		{ ProcedureDeclaration 'SEMI' }
		;

module
	->	'MODULE' ident 'COMMA' declarations [ 'BEGIN' StatementSequence ] 'END' ident 'PUNKT'
	;
