;BNF-Grmmatk fÅr die Definition der Grammatikdatei fÅr den
;LR-Parser-Generator

/*
	NTERM		Nichtterninal
	TERM		Terminal

	TYPE_SYM	%type <x> Nichtterminale
	UNION_SYM	%union { ... };
	PREC_SYM	%prec ADD
	LEFT_SYM	%left ADD
	RIGHT_SYM	%right MUL
	ERROR_SYM	%error
	INIT_SYM	%init
	INCL_SYM	%incl

	OBRACE		{
	CBRACE		}
	NUM_OPEN	'
	DB_POINT	:
	SLASH		/
	LESS_SYM	<
	GREATER_SYM	>
*/


first
			->	left_def
			|	right_def
			|	error_def
			|	type_def
			|	union_def
			|	init_sym
			|	incl_sym
			|	prod_def

init_sym
			->	'INIT_SYM'

incl_sym
			->	'INCL_SYM'

union_def
			->	'UNION_SYM'

prod_def
			->	'NTERM' 'SEP' right { right } prec_def

right
			->	( 'TERM' | 'NTERM' )

prec_def
			->	[ 'PREC_SYM' 'TERM' ]

type_def
			->	'TYPE_SYM' 'LESS_SYM' 'IDENT' 'GREATER_SYM' right { right }

left_def
			->	'LEFT_SYM' 'TERM' { 'TERM' }

right_def
			->	'RIGHT_SYM' 'TERM' { 'TERM' }

error_def
			->	'ERROR_SYM' 'BOPEN' inner_error 'BCLOSE'

inner_error
			->	'NUMBER' [ 'DB_POINT' 'SALSH' 'TERM' ] 'ERR_TEXT' { inner_error }