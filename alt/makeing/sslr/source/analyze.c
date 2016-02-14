/*
	Projekt		:	LR-Parser-Generator
	Modul		:	ANALYZE.C
	Sprache		:	ANSI-C
	Compiler	:	ATARI: PureC V1.1
*/



/************/
/* Includes */
/************/

/* Include-Datei des Parser-Generators */
#include "LR.H"
#include <ctype.h>



/**************/
/* Prototypen */
/**************/

WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD insert_right( NTERM_STR *nterm_str, LONG index );
WORD do_sem_act( LONG nterm_index, WORD ebnf_sym );
WORD do_error( LONG prod_index, WORD ebnf_err );
LONG insert_hash_tab( BYTE *string, WORD type );
WORD malloc_nterm_str( LONG temp_nterm_count );
LONG hash_pjw( BYTE *string, ULONG length );
WORD do_prec( NTERM_STR *nterm_str );
WORD insert_start_sym( VOID );
LONG do_coll( BYTE *string );
WORD do_assoc( WORD assoc );
WORD sort_nterm_str( VOID );
WORD do_nterm_type( VOID );
WORD do_term_type( VOID );
WORD extract_word( VOID );
WORD test_term( VOID );
WORD do_union( VOID );
WORD del_sep( VOID );
WORD do_init( VOID );
WORD do_incl( VOID );
WORD delete( VOID );



/*****************/
/* Deklarationen */
/*****************/

/* Max. zulÑssige Anzahl an Kollisionen bei der Hash-Tabelle */
#define MAX_COL			5


/* Stack-Struktur */
typedef struct
{
	/* Index der Produktion */
	ULONG	prod_index;

	/* Anzahl der Elemente in der NTERM_STR */
	LONG	nterm_count;

	/* Anzahl der Alternativen */
	WORD	alt;

	/* Meta-Symbol fÅr Fehlerbehandlung */
	WORD	ebnf_err;

}STACK;



/****************/
/* Definitionen */
/****************/

/* Speicher fÅr erkannte Wîrter */
BYTE		grammar_word[256];

/* Beinhaltet alle Nichtterminale */
NTERM_STR	**nterm_str = 0L;

/* Anzahl der NTERM_STRs in nterm_str */
LONG		nterm_count = 0;

/* Beinhaltet alle Terminale */
TERM_STR	*term_str = 0L;

/* Anzahl der TERM_STRs in term_str */
LONG		term_count = -1;

/* Hash-Index von ACCEPT */
LONG		accept;

/* Hash-Index von EPSILON */
LONG		epsilon;

/* Beinhaltet die Indexe der Produktionen in der NTERM_STR */
LONG		*prod_index_arr = 0L;

/* Anzahl der Produktionen */
ULONG		prod_count = 0;

/* LÑnge des lÑngsten Nichtterminals */
ULONG		nterm_length;

/* NTERM_FIRST */
NTERM_FIRST	**nterm_first = 0L;

/* ZÑhler fÅr NTERM_FIRST */
ULONG		nterm_first_count = 0;

/* Beinhaltet die Hash-Indexe alle in der Grammatik verwendeten Nichtterminale */
LONG		*nterm_arr = 0L;

/* ZÑhler fÅr nterm_arr */
LONG		nterm_arr_count = 0;

/* Array mit den neuen Indexen der Produktionen */
LONG		*old_nterm_index;


/* Flag fÅr Anfang einer Zeile */
WORD		beg_line = 0;



/***********/
/* Imports */
/***********/

MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL LR_INFO		lr_info;
MGLOBAL SEM_ACT		*sem_act;
MGLOBAL ULONG		zeile;
MGLOBAL LONG		sem_act_count;
MGLOBAL WORD		error_flag;
MGLOBAL BYTE		*grammar;
MGLOBAL	BYTE		*union_struct;
MGLOBAL	BYTE		*incl;
MGLOBAL	BYTE		*init;
MGLOBAL FILE		*file;


WORD delete( VOID )
{
	/* In dieser Routine werden alle unnuetzen Dinge gelîscht. */
	WORD	comment = 0;

	/* Endlosschleife */
	do
	{
		/* Anhand des Zeichens die Massnahmen einleiten */
		switch( *grammar )
		{
			/* Space, Tab */
			case ' ':
			case '\t':
			{
				/* Zeichen ueberlesen */
				grammar++;

				/* NÑchstes Zeichen bearbeiten */
				beg_line++;
				break;
			}

			/* Neue Zeile */
			case 0x0d:
			{
				/* Zeichen ueberlesen */
				grammar++;
			}

			/* LineFeed */
			case '\n':
			{
				/* NÑchste Zeile */
				zeile++;

				/* NÑchstes Zeichen bearbeiten */
				grammar++;

				/* Anfang einer Zeile */
				beg_line = 0;
				break;
			}

			case '/':
			{
				/* Beginn eines C-Kommentares ? */
				if( grammar[1] == '*' )
					/* Flag fÅr Kommentar setzen */
					comment++;

				/* Ende eines C-Kommentares ? */
				else if( grammar[-1] == '*' )
					/* Flag fÅr Kommentar lîschen */
					comment--;

				/* NÑchstes Zeichen bearbeiten */
				grammar++;
				beg_line++;
				break;
			}

			/* Eingabe zu Ende ? */
			case 0x00:
				/* Eingabe ist zu Ende */
				return( END );

			/* Alles andere */
			default:
			{
				/* Noch im Kommentar ? */
				if( comment )
				{
					/* Ja, mit dem nÑchsten Zeichen weitermachen */
					grammar++;
					beg_line++;
					break;
				}

				else
					/* ZurÅck */
					return( OK );
			}
		}

	}while( 1 );
}


WORD extract_word( VOID )
{
	/* Hier wird ein Wort extrahiert. */
	WORD	type = -1;
	BYTE	*temp_grammar = grammar;
	ULONG	length;

	/* Endlosschleife */
	do
	{
		/* Nicht mehr am Anfang einer Zeile */
		beg_line++;

		/* Ist es ein Terminal oder ein Nichtterminal ? */
		switch( *grammar )
		{
			/* Ist es ein Terminal ? */
			case '\'':
			case '\"':
			{
				/* Ist ein Terminal erkannt worden ? */
				if( ( type == TERM ) || ( type == NTERM ) )
				{
					/* Wort merken */
					strncpy( grammar_word, temp_grammar, ( grammar - temp_grammar ) );

					/* Null-Byte anhÑngen */
					grammar_word[grammar - temp_grammar] = 0;

					/* Terminal-Abschlusszeichen ueberlesen */
					grammar++;

					/* War ein Terminal */
					length = ( ULONG ) ( grammar - temp_grammar );

					/* Die LÑnge des Terminals festhalten */
					if( length > nterm_length )
						/* Neues Term ist lÑnger als das Alte */
						nterm_length = length;

					/* War es ein Terminal ? */
					if( type == TERM )
						/* Ja, Alles OK */
						return( TERM );
				
					else
					{
						/* Nein, ein Nichtterminal */

						/* Fehlermeldung ausgeben */
						error( INVALID_CHAR, ( ULONG ) grammar[-1], 0 );

						/* Ist ein Nichtterminal */
						return( NTERM );
					}
				}

				/* Ist ein Terminal */
				type = TERM;

				/* Zeichen ueberlesen */
				temp_grammar = ++grammar;
			}

			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '_':
			case 'ss':
			{
				/* Ist es Nichtterminal ? */
				if( type < 0 )
					/* Ist ein Nichtterminal */
					type = NTERM;

				/* Mit dem nÑchsten Zeichen weitermachen */
				grammar++;
				break;
			}

			case '(':
			case ')':
			case ':':
			case '=':
			case '-':
			case ' ':
			case '\t':
			case '\n':
			case 0x0d:
			case 0x00:
			{
				/* Nichtterminal merken */
				strncpy( grammar_word, temp_grammar, ( grammar - temp_grammar ) );

				/* Null-Byte anhÑngen */
				grammar_word[grammar - temp_grammar] = 0;

				/* Ist es ein Terminal ? */
				if( type == TERM )
				{
					/* Nicht abgeschlossenes Terminal */
					error( UNTERMINATED_TERMINAL, ( ULONG ) grammar_word, 0 );

					/* War ein Terminal */
					return( TERM );
				}

				/* Ist die Eingabe zu Ende ? */
				if( !( *grammar ) && ( type < 0 ) )
					/* Eingabe ist zu Ende */
					return( END );

				else
				{
					/* War ein Nichtterminal */
					length = ( ULONG ) ( grammar - temp_grammar );

					/* Die LÑnge des Nichtterminals festhalten */
					if( length > nterm_length )
						/* Neues Nterm ist lÑnger als das Alte */
						nterm_length = length;

					/* Ist ein Nichtterminal */
					return( NTERM );
				}
			}

			/* Unerlaubtes Zeichen */
			default:
			{
				/* Fehlermeldung ausgeben */
				error( INVALID_CHAR, ( ULONG ) *grammar, 0 );

				/* Wort merken */
				strncpy( grammar_word, temp_grammar, ( grammar - temp_grammar ) );

				/* Null-Byte anhÑngen */
				grammar_word[grammar - temp_grammar] = 0;

				/* LÑnge bestimmen */
				length = ( ULONG ) ( grammar - temp_grammar );

				/* Die LÑnge des Terminals festhalten */
				if( length > nterm_length )
					/* Neues Term ist lÑnger als das Alte */
					nterm_length = length;

				/* Terminal ? */
				if( type == TERM )
				{
					/* Fehlerhaftes Zeichen ueberlesen */
					while( ( *grammar != '\'' ) && ( *grammar != '\"' ) )
						/* Zeichen lîschen */
						grammar++;
				}

				/* Nichtterminal */
				grammar++;

				/* Typ zurueckliefern */
				return( type );
			}
		}

	}while( 1 );
}


LONG insert_hash_tab( BYTE *string, WORD type )
{
	/* Hier wird ein Terminal/Nichtterminal in die Hash-Tab eingetragen. */
	ULONG	index;

	/* Auf Kollisionen ueberpruefen */
	index = do_coll( string );

	/* Ist der Eintrag noch frei ? */
	if( !hash_tab[index].name )
	{
		/* Speicher fÅr den String anfordern */
		if( ( hash_tab[index].name = ( BYTE * ) malloc( strlen( string ) + 1 ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* String eintragen */
		strcpy( hash_tab[index].name, string );

		/* Die Zeilennummer merken */
		hash_tab[index].zeile = zeile;

		/* Terminal/Nichtterminal hat zur Zeit kein Attribut */
		hash_tab[index].type = -1;

		/* Ist es ein Terminal ? */
		if( type == TERM )
		{
			/* Speicher fÅr TERM_STR anfordern */
			if( ( term_str = ( TERM_STR * ) realloc( term_str, ( ++term_count + 1 ) * sizeof( TERM_STR ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Struktur initialisieren */
			memset( ( VOID * ) &term_str[term_count], 0, sizeof( TERM_STR ) );

			/* Index des Hash-Eintrages merken */
			term_str[term_count].hash_tab_index = index;

			/* Zeiger auf TERM_STR in Hash-Tabelle vermerken */
			hash_tab[index].term_str = &term_str[term_count];

			/* PrioritÑt des Terminals zuruecksetzen */
			term_str[term_count].priori = -1;
		}

		else
		{
			/* Ist ein Nichtterminal */

			/* Speicher fÅr Aray anfordern */
			if( ( nterm_arr = ( LONG * ) realloc( nterm_arr, ( nterm_arr_count + 1 ) * sizeof( LONG ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Index des Hash-Eintrages merken */
			nterm_arr[nterm_arr_count++] = index;
		}
	}

	/* Terminal schon als Nichtterminal benutzt ? */
	else if( ( hash_tab[index].term_str ) && ( type != TERM ) )
		/* Fehler */
		error( TERM_IS_NTERM, ( LONG ) hash_tab[index].name, 0 );

	/* Nichtterminal schon als Terminal benutzt ? */
	else if( !( hash_tab[index].term_str ) && ( type != NTERM ) )
	{
		/* Fehler */
		error( NTERM_IS_TERM, ( LONG ) hash_tab[index].name, 0 );

		/* Dummy-Terminal zurueckliefern */
		index = epsilon;
	}

	/* Alles OK */
	return( index );
}


LONG do_coll( BYTE *string )
{
	/* Hier wird eine Kollision in der Hash-Tabelle behoben. */
	LONG	index;
	WORD	col_count = 0, col_jump;
	BYTE	*name1, *name2;

	/* Hashen */
	index = hash_pjw( string, lr_info.hash_tab_size );

	/* Ist der Eintrag bereits belegt ? */
	if( hash_tab[index].name )
	{
		/* Ist es schon eingetragen worden ? */
		if( !strcmp( hash_tab[index].name, string ) )
			/* Ja */
			return( index );

		/* Einen leeren Eintrag finden */
		col_jump = 2;

		/* Endlosschleife */
		do
		{
			/* Zeiger aus Strings holen */
			name1 = hash_tab[index + col_jump].name;
			name2 = hash_tab[index - col_jump].name;

			/* Ist der Eintrag frei ? */
			if( !hash_tab[index + col_jump].name )
			{
				/* Ja, Eintragen */
				index += col_jump;
				break;
			}

			/* Ist string schon eingetragen worden ? */
			else if( !strcmp( name1, string ) )
				/* Ist es schon eingetragen worden ? */
				return( index + col_jump );

			/* Ist der Eintrag frei ? */
			else if( !hash_tab[index - col_jump].name )
			{
				/* Ja, Eintragen */
				index -= col_jump;
				break;
			}

			/* Ist string schon eingetragen worden ? */
			else if( !strcmp( name2, string ) )
				/* Ist es schon eingetragen worden ? */
				return( index - col_jump );

			/* NÑchsten Eintrag untersuchen */
			col_jump *= 2;

			/* Tabellenueberlauf ? */
			if( ( index + col_jump ) > lr_info.hash_tab_size )
				/* Wert anpassen */
				index %= lr_info.hash_tab_size;

			/* Max. zulÑssige Anzahl an Kollisionen ueberschritten ? */
			if( col_count >= MAX_COL )
				/* Ja, Hash-Tab ist zu Klein */
				return( error( HASH_TAB_ERROR, 0, 0 ) );

			else
				/* NÑchste Kollision */
				col_count++;

		}while ( 1 );
	}

	/* Alles OK */
	return( index );
}


LONG hash_pjw( BYTE *string, ULONG length )
{
	/* Diese Funktion liefert einen HASH-Index fÅr den String zurueck. */
	REG BYTE	*p;
	ULONG		h = 0, g;

	for( p = string; *p; p = p + 1 )
	{
		h = ( h << 4 ) + *p;

		if( g = h & 0xf0000000L )
		{
			h = h ^ ( g >> 24 );
			h = h ^ g;
		}
	}

	return( ( LONG ) ( ( h % length ) + 1 ) );
}


WORD analyze_grammar( VOID )
{
	/* Hier wird die Grammatik-Datei analysiert. */
	WORD		state = OK, analyze = 0, assoc, analyze_prod = 0;
	WORD		aux_sym_count = 1, sp = 0, alt, ebnf_err = 0;
	BYTE		aux_sym[12], ebnf_sym[3] = { '}', ']', ')' };
	ULONG		prod_index, main_prod_index, temp;
	LONG		temp_nterm_count = 0, index;
	STACK		stack[100];

	/* Meldung ausgeben */
	fprintf( stderr, "\n\nAnalysiere Grammatik..." );

	/* LÑnge von ACCEPT ist Default-LÑnge */
	nterm_length = 6;

	/* Keine Fehler */
	error_flag = 0;

	/* Hash-Index von EPSILON holen */
	if( ( epsilon = insert_hash_tab( "EPSILON", TERM ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Hash-Index von ACCEPT */
	if( ( accept = insert_hash_tab( "ACCEPT", TERM ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Terminal wurde benutzt */
	hash_tab[accept].def++;

	/* Meldung ausgeben ? */
	if( !lr_info.analyze )
		/* Meldung ausgeben */
		fprintf( file, "\n\n***************************\n"
					   "* Analysiere Produktionen *\n"
					   "***************************\n\n" );

	/* Spaces etc. lîschen */
	state = delete();

	/* Solange die Eingabe nicht zu Ende ist */
	while( state != END )
	{
		switch( *grammar )
		{
			case ';':
			{
				/* Eine Produktion wurde abgeschlossen */
				analyze_prod = 0;

				/* Semikolon lîschen */
				grammar++;
				break;
			}

			case '\\':
			{
				/* Semmantische Aktion */
				if( do_sem_act( nterm_count, ebnf_err ) < 0 )
					/* Fehler */
					return( -1 );

				/* Case verlassen */
				break;
			}

			case '(':
			case '[':
			{
				/* Ein EBNF-Symbol */

				/* Ein aux_sym vorbereiten */
				sprintf( aux_sym, "aux_sym%d", aux_sym_count++ );

				/* Das aux_sym in die Hash-Tabelle eintragen */
				if( ( index = insert_hash_tab( aux_sym, NTERM ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Das aux_sym auf der rechten Seite der Produktion eintragen */
				if( insert_right( nterm_str[nterm_count], index ) < 0 )
					/* Fehler */
					return( -1 );

				/* Nichtterminal wurde benutzt */
				hash_tab[index].def++;

				/* Wichtige Variablen merken */
				stack[sp].prod_index = prod_index;
				stack[sp].nterm_count = nterm_count;
				stack[sp].alt = alt;
				stack[sp].ebnf_err = ebnf_err;

				/* NÑchster STACK-Eintrag */
				sp++;

				/* Ist es ( oder [ ? */
				if( *grammar++ == '(' )
					/* fÅr Fehlerbehandlung */
					ebnf_err = 3;

				else
					/* fÅr Fehlerbehandlung */
					ebnf_err = 2;

				/* Noch keine Alternativen */
				alt = 1;

				/* Speicher fÅr NTERM_STR holen */
				if( ( malloc_nterm_str( temp_nterm_count ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* aux_sym eintragen */
				nterm_str[nterm_count]->hash_tab_index = prod_index = index;

				/* Anzahl der Produktionen merken */
				prod_count++;

				/* Nichtterminal wurde definiert */
				hash_tab[nterm_str[nterm_count]->hash_tab_index].left_def++;

				/* Das Attribut von dem Nichtterminal uebernehmen */
				hash_tab[index].type = hash_tab[nterm_str[stack[sp - 1].nterm_count]->hash_tab_index].type;

				/* Weitermachen */
				break;
			}

			case '{':
			{
				/* 0 - beliebig oft */

				/* Zeichen lîschen */
				grammar++;

				/* Ein aux_sym vorbereiten */
				sprintf( aux_sym, "aux_sym%d", aux_sym_count++ );

				/* Das aux_sym in die Hash-Tabelle eintragen */
				if( ( index = insert_hash_tab( aux_sym, NTERM ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Das aux_sym auf der rechten Seite der Produktion eintragen */
				if( insert_right( nterm_str[nterm_count], index ) < 0 )
					/* Fehler */
					return( -1 );

				/* Wichtige Variablen merken */
				stack[sp].prod_index = prod_index;
				stack[sp].nterm_count = nterm_count;
				stack[sp].alt = alt;
				stack[sp].ebnf_err = ebnf_err;

				/* NÑchster STACK-Eintrag */
				sp++;

				/* fÅr Fehlerbehandlung */
				ebnf_err = 1;

				/* Noch keine Alternativen */
				alt = 1;

				/* Speicher fÅr NTERM_STR holen */
				if( ( malloc_nterm_str( temp_nterm_count ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Index des aux_syms in Hash-Tabelle eintragen */
				nterm_str[nterm_count]->hash_tab_index = prod_index = index;

				/* Das Attribut von dem Nichtterminal uebernehmen */
				hash_tab[index].type = hash_tab[nterm_str[stack[sp - 1].nterm_count]->hash_tab_index].type;

				/* Anzahl der Produktionen merken */
				prod_count++;

				/* Nichtterminal wurde definiert */
				hash_tab[nterm_str[nterm_count]->hash_tab_index].left_def++;

				/* aux_sym auf der rechten Seite der Produktion eintragen */
				if( insert_right( nterm_str[nterm_count], index ) < 0 )
					/* Fehler */
					return( -1 );

				/* Nichtterminal wurde benutzt */
				hash_tab[index].def++;

				/* Weitermachen */
				break;
			}

			case ')':
			{
				/* Ausdruck schliessen */

				/* Stackpointer heruntersetzen */
				sp--;

				/* Stackunterlauf ? */
				if( sp >= 0 )
				{
					/* Fehler bei den Meta-Symbolen ? */
					if( ebnf_err != 3 )
						/* Fehler */
						return( error( EBNF_SYM_ERR, ( ULONG ) ebnf_sym[ebnf_err - 1], ( ULONG ) hash_tab[main_prod_index].name ) );

					/* Wichtige Variablen zurueckholen */
					prod_index = stack[sp].prod_index;
					ebnf_err = stack[sp].ebnf_err;
					alt = stack[sp].alt;

					/* nterm_count merken */
					temp_nterm_count = nterm_count;
					nterm_count = stack[sp].nterm_count;
				}

				else
					/* Stack-Unterlauf */
					return( error( TOO_MANY_EBNF_SYMB, ( BYTE ) *grammar, ( ULONG ) hash_tab[main_prod_index].name ) );

				/* Meta-Symbol lîschen */
				grammar++;

				/* Weitermachen */
				break;
			}

			case '}':
			case ']':
			{
				/* Ausdruck schliessen */

				/* Stackunterlauf ? */
				if( ( sp - 1 ) >= 0 )
				{
					/* fÅr die Fehlerbehandlung */
					if( *grammar == '}' )
					{
						/* Fehler bei dem Meta-Symbole } ? */
						if( ebnf_err != 1 )
							/* Fehler */
							return( error( EBNF_SYM_ERR, ( ULONG ) ebnf_sym[ebnf_err - 1], ( ULONG ) hash_tab[main_prod_index].name ) );
					}

					else
					{
						/* Fehler bei dem Meta-Symbole ] ? */
						if( ebnf_err != 2 )
							/* Fehler */
							return( error( EBNF_SYM_ERR, ( ULONG ) ebnf_sym[ebnf_err - 1], ( ULONG ) hash_tab[main_prod_index].name ) );
					}

					/* nterm_count merken */
					temp = nterm_count;

					/* Speicher fÅr NTERM_STR holen */
					if( ( malloc_nterm_str( temp_nterm_count ) ) < 0 )
						/* Fehler */
						return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

					/* Index des aux_syms in Hash-Tabelle eintragen */
					nterm_str[nterm_count]->hash_tab_index = prod_index;

					/* EPSILON auf der rechten Seite der Produktion eintragen */
					if( insert_right( nterm_str[nterm_count], epsilon ) < 0 )
						/* Fehler */
						return( -1 );

					/* Terminal wird benutzt */
					hash_tab[epsilon].def++;

					/* Nachfolger eintragen */
					nterm_str[temp]->post_index = nterm_count;

					/* Anzahl der Alternativen */
					nterm_str[nterm_count]->alt = alt++;

					/* Stackpointer heruntersetzen */
					sp--;

					/* Wichtige Variablen zurueckholen */
					prod_index = stack[sp].prod_index;
					ebnf_err = stack[sp].ebnf_err;
					alt = stack[sp].alt;

					/* nterm_count merken */
					temp_nterm_count = nterm_count;
					nterm_count = stack[sp].nterm_count;
				}

				else
					/* Stack-Unterlauf */
					return( error( TOO_MANY_EBNF_SYMB, ( BYTE ) *grammar, ( ULONG ) hash_tab[main_prod_index].name ) );

				/* Meta-Symbol lîschen */
				grammar++;

				/* Weitermachen */
				break;
			}

			case '%':
			{
				/*
					%nterm_def, %term_def, %union, %left, %right,
					%prec und %error
				*/

				/* %init ? */
				if( !strncmp( grammar, "%init", 5 ) )
				{
					/* %init muss vor der Grammatik stehen */
					/* %incl muss vor der Grammatik stehen */
					if( ( analyze ) || ( incl ) )
					{
						/* Ist die Union-Struktur in der Grammatik ? */
						if( analyze )
							/* Fehlermeldung ausgeben */
							error( INIT_ERR, 0, 0 );

						else
							/* incl ist schon angegeben worden */
							error( INIT_DBL, 0, 0 );

						/* Bis zum %} alles lîschen */
						do
						{
							/* Zeilenende ? */
							if( ( grammar[-2] == 0x0d ) && ( grammar[-1] == 0x0a ) )
								/* Eine weitere Zeile */
								zeile++;

							else if( ( grammar[-2] == '%' ) && ( grammar[-1] == '}' ) )
								/* Ende  */
								break;

							/* Lîschen */
							grammar++;

						}while( 1 );
					}

					else
					{
						/* Fehlermeldung auswerten */
						if( do_init() < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %incl ? */
				else if( !strncmp( grammar, "%incl", 5 ) )
				{
					/* %incl muss vor der Grammatik stehen */
					if( ( analyze ) || ( incl ) )
					{
						/* Ist die Union-Struktur in der Grammatik ? */
						if( analyze )
							/* Fehlermeldung ausgeben */
							error( INCL_ERR, 0, 0 );

						else
							/* incl ist schon angegeben worden */
							error( INCL_DBL, 0, 0 );

						/* Bis zum %} alles lîschen */
						do
						{
							/* Zeilenende ? */
							if( ( grammar[-2] == 0x0d ) && ( grammar[-1] == 0x0a ) )
								/* Eine weitere Zeile */
								zeile++;

							else if( ( grammar[-2] == '%' ) && ( grammar[-1] == '}' ) )
								/* Ende  */
								break;

							/* Lîschen */
							grammar++;

						}while( 1 );
					}

					else
					{
						/* Fehlermeldung auswerten */
						if( do_incl() < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %nterm_type ? */
				else if( !strncmp( grammar, "%nterm_type", 11 ) )
				{
					/* %nterm_type muss vor der Grammatik stehen */
					if( analyze )
					{
						/* Fehlermeldung ausgeben */
						error( NTERM_TYPE_ERR, 0, 0 );

						/* Bis zum Zeilenende alles lîschen */
						while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
							/* Lîschen */
							grammar++;
					}

					else
					{
						/* Fehlermeldung auswerten */
						if( do_nterm_type() < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %term_type ? */
				else if( !strncmp( grammar, "%term_type", 10 ) )
				{
					/* %term_type muss vor der Grammatik stehen */
					if( analyze )
					{
						/* Fehlermeldung ausgeben */
						error( TERM_TYPE_ERR, 0, 0 );

						/* Bis zum Zeilenende alles lîschen */
						while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
							/* Lîschen */
							grammar++;
					}

					else
					{
						/* Fehlermeldung auswerten */
						if( do_term_type() < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %union ? */
				else if( !strncmp( grammar, "%union", 6 ) )
				{
					/* %union muss vor der Grammatik stehen */
					if( ( analyze ) || ( union_struct ) )
					{
						/* Ist die Union-Struktur in der Grammatik ? */
						if( analyze )
							/* Fehlermeldung ausgeben */
							error( UNION_ERR, 0, 0 );

						else
							/* union ist schon angegeben worden */
							error( UNION_DBL, 0, 0 );

						/* Bis zum }; alles lîschen */
						do
						{
							/* Zeilenende ? */
							if( ( grammar[-2] == 0x0d ) && ( grammar[-1] == 0x0a ) )
								/* Eine weitere Zeile */
								zeile++;

							else if( ( grammar[-2] == '}' ) && ( grammar[-1] == ';' ) )
								/* Ende  */
								break;

							/* Lîschen */
							grammar++;

						}while( 1 );
					}

					else
					{
						/* Fehlermeldung auswerten */
						if( do_union() < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %prec ? */
				else if( !strncmp( grammar, "%prec", 5 ) )
				{
					/* %prec nur im Zusammenhang mit einer Produktion erlaubt */
					if( !analyze )
					{
						/* Fehlermeldung ausgeben */
						error( PREC_ERR, 0, 0 );

						/* Bis zum Zeilenende alles lîschen */
						while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
							/* Lîschen */
							grammar++;
					}

					else
					{
						/* PrioritÑt der Produktion Ñndern */
						if( ( state = do_prec( nterm_str[nterm_count] ) ) < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %error ? */
				else if( !strncmp( grammar, "%error", 6 ) )
				{
					/* %error muss im Zusammenhang mit Produktionen stehen */
					if( !analyze )
					{
						/* Fehlermeldung ausgeben */
						error( ERROR_ERR, 0, 0 );

						/* Bis zum Zeilenende alles lîschen */
						while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
							/* Lîschen */
							grammar++;
					}

					else
					{
						/* Fehlermeldung auswerten */
						if( do_error( nterm_count, ebnf_err ) < 0 )
							/* Fehler */
							return( -1 );
					}

					/* Weitermachen */
					break;
				}

				/* %left ? */
				else if( !strncmp( grammar, "%left", 5 ) )
				{
					/* Terminal soll Links-Assoziativ sein */
					assoc = LEFT;

					/* %left ueberspringen */
					grammar += 5;
				}

				/* %right ? */
				else if( !strncmp( grammar, "%right", 6 ) )
				{
					/* Terminal soll Rechts-Assoziativ sein */
					assoc = RIGHT;

					/* %right ueberspringen */
					grammar += 6;
				}

				/* %left und %right muessen vor der Grammatik stehen */
				if( analyze )
				{
					/* Fehlermeldung ausgeben */
					error( LR_ERR, 0, 0 );

					/* Bis zum Zeilenende alles lîschen */
					while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
						/* Lîschen */
						grammar++;
				}

				else
				{
					/* AssoziativitÑt und PrioritÑt des Terminals auswerten */
					if( do_assoc( assoc ) < 0 )
						/* Fehler */
						return( -1 );
				}

				/* Weitermachen */
				break;
			}

			case '|':
			{
				/* Eine Alternative */
				grammar++;

				/* Produktion analysieren */
				analyze_prod++;

				/* nterm_count merken */
				temp = nterm_count;

				/* Speicher fÅr NTERM_STR holen */
				if( ( malloc_nterm_str( temp_nterm_count ) ) < 0 )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

				/* Index in Hash-Tabelle eintragen */
				nterm_str[nterm_count]->hash_tab_index = prod_index;

				/* Nachfolger eintragen */
				nterm_str[temp]->post_index = nterm_count;

				/* Anzahl der Alternativen */
				nterm_str[nterm_count]->alt = alt++;

				/* Weitermachen */
				break;
			}

			default:
			{
				/* Terminal, Nichtterminal oder fehlerhaftes Zeichen */

				/* Spaces etc. lîschen */
				delete();

				/* Sind wir am Anfang einer Zeile ? */
				if( !beg_line && analyze_prod )
				{
					/* Das Produktion wurde nicht mit einem Semikolon beendet. */
					error( PROD_END, ( ULONG ) hash_tab[prod_index].name, 0 );

					/* Prodktion beenden */
					analyze_prod = 0;
				}

				/* Wort extrahieren */
				if( ( state = extract_word() ) < 0 )
					/* Fehler */
					return( -1 );

				/* Wurde gar nichts eingetragen ? */
				if( beg_line && !grammar_word[0] )
				{
					/* Fehler, Seperator wird erwartet */
					error( OR_OPERATOR, 0, 0 );

					/* Zeile lîschen */
					while( ( *grammar != 0x0d ) && ( *grammar != 0x0a ) )
						grammar++;

					/* Raus hier */
					break;
				}

				/* Ist es ein Nichtterminal ? */
				if( state == NTERM )
				{
					/* Die Grammatik wird analysiert */
					analyze++;

					/* Nichtterminal in Hash-Tabelle eintragen */
					if( ( index = insert_hash_tab( grammar_word, NTERM ) ) < 0 )
						/* Fehler */
						return( -1 );

					/* Wird gerade keine Produktion analysiert ? */
					if( !analyze_prod )
					{
						/* Ist ein EBNF-Symbol vergessen worden ? */
						if( ebnf_err )
							/* Fehler */
							return( error( EBNF_SYM_ERR, ( ULONG ) ebnf_sym[ebnf_err - 1], ( ULONG ) hash_tab[main_prod_index].name ) );

						/* Produktion analysieren */
						analyze_prod++;

						/* Index der Produktion merken */
						main_prod_index = prod_index = index;

						/* Seperator lîschen */
						if( del_sep() < 0 )
							/* Fehler */
							return( -1 );

						/* Speicher fÅr NTERM_STR holen */
						if( ( malloc_nterm_str( temp_nterm_count ) ) < 0 )
							/* Fehler */
							return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

						/* Es gibt noch keine Alternativen */
						alt = 1;

						/* Index in Hash-Tabelle merken */
						nterm_str[nterm_count]->hash_tab_index = index;

						/* Anzahl der Produktionen merken */
						prod_count++;

						/* Wurde das Nichtterminal schon definiert ? */
						if( hash_tab[nterm_str[nterm_count]->hash_tab_index].left_def )
							/* Nichtterminal wurde schon definiert */
							error( YET_DEFINED_TERM, ( ULONG ) hash_tab[index].name, 0 );

						else
							/* Nichtterminal wurde noch nicht definiert */
							hash_tab[nterm_str[nterm_count]->hash_tab_index].left_def++;

						/* Meldung ausgeben ? */
						if( !lr_info.analyze )
							/* Ja */
							fprintf( file, "\nAnalysiere %s...", hash_tab[index].name );
					}

					else
					{
						/* Nichtterminal auf der rechten Seite der Produktion eintragen */
						if( insert_right( nterm_str[nterm_count], index ) < 0 )
							/* Fehler */
							return( -1 );

						/* Nichtterminal wird benutzt */
						hash_tab[index].def++;
					}
				}

				/* Ist es ein Terminal ? */
				else if( state == TERM )
				{
					/* Wird gerade keine Produktion analysiert ? */
					if( !analyze_prod )
						/* Fehler */
						return( error( PROD_BEGINN, 0, 0 ) );

					/* Terminal in Hash-Tabelle eintragen */
					if( ( index = insert_hash_tab( grammar_word, TERM ) ) < 0 )
						/* Fehler */
						return( -1 );

					/* Terminal wird benutzt */
					hash_tab[index].def++;

					/* PrioritÑt der Produktion = PrioritÑt des Terminals */
					nterm_str[nterm_count]->priori = hash_tab[index].term_str->priori;

					/* AssoziativitÑt der Produktion = AssoziativitÑt des Terminals */
					nterm_str[nterm_count]->assoc = hash_tab[index].term_str->assoc;

					/* Terminal auf der rechten Seite der Produktion eintragen */
					if( insert_right( nterm_str[nterm_count], index ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}

			/* Weitermachen */
			break;
		}

		/* Spaces etc. lîschen */
		state = delete();
	}

	/* Meldung ausgeben ? */
	if( !lr_info.analyze )
		/* Meldung ausgeben */
		fprintf( file, "\n\n%lu Produktionen analysiert !\n\n"
					   "------------------------------------------------------"
					   "--------------------", prod_count );

	/* NTERM_STRs sortieren */
	if( sort_nterm_str() < 0 )
		/* Fehler */
		return( -1 );

	/* Das Startsymbol in die Grammatik eintragen */
	if( insert_start_sym() < 0 )
		/* Fehler */
		return( -1 );

	/* Die Nichtterminale und Terminale pruefen */
	if( test_term() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( error_flag > 0 ? -1 : 0 );
}


WORD del_sep( VOID )
{
	/* Hier wird der Seperator der Produktion gelîscht. */

	/* Spaces etc. lîschen */
	delete();

	/* Art des Seperators bestimmen */
	switch(	*grammar )
	{
		case '=':
		{
			/* == ? */
			if( grammar[1] == '=' )
				/* Seperator lîschen */
				grammar += 2;

			/* = */
			else
				/* Seperator lîschen */
				grammar++;

			/* Weitermachen */
			break;
		}

		case '-':
		{
			/* -> */
			if( grammar[1] != '>' )
				/* Fehler */
				return( error( PROD_SEP_ERR, 0, 0 ) );

			else
				/* Seperator lîschen */
				grammar += 2;

			/* Weitermachen */
			break;
		}

		case ':':
		{
			/* ::= ? */
			if( grammar[1] == ':' )
			{
				/* ::= */
				if( grammar[2] != '=' )
					/* Fehler */
					return( error( PROD_SEP_ERR, 0, 0 ) );

				else
					/* Seperator lîschen */
					grammar += 3;
			}

			/* := ? */
			else if( grammar[1] == '=' )
				/* Seperator lîschen */
				grammar += 2;

			/* : */
			else
				/* Seperator lîschen */
				grammar++;

			/* Weitermachen */
			break;
		}		
	}

	/* Eine letzte Ueberpruefung des Seperators */
	if( !isspace( *grammar ) )
		/* Fehler */
		return( error( PROD_SEP_ERR, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD insert_right( NTERM_STR *nterm_str, LONG index )
{
	/*
		Hier wird das Element index auf der rechten Seite von nterm_str
		vermerkt.
	*/

	/* Ist noch Platz da ? */
	if( nterm_str->anz_ri > MAX_ANZ_RI )
		/* Nicht mehr genug Platz da */
		return( error( MAX_ANZ_RI_ERR, 0, 0 ) );

	/* Element eintragen */
	nterm_str->ri[nterm_str->anz_ri++] = index;

	/* Alles OK */
	return( 0 );
}


WORD malloc_nterm_str( LONG temp_nterm_count )
{
	/* Hier wird eine NTERM_STR-Struktur zu nterm_str hinzugefuegt. */

	/* temp_nterm_count > nterm_count ? */
	if( temp_nterm_count > nterm_count )
	{
		/* Ja, uebernehmen */
		nterm_count = temp_nterm_count;

		/* temp_nterm_count lîschen */
		temp_nterm_count = 0;
	}

	/* Ein weiterer Eintrag */
	nterm_count++;

	/* Speicher fÅr neuen NTERM_STR-Eintrag holen */
	if( ( nterm_str = ( NTERM_STR ** ) realloc( ( VOID * ) nterm_str, ( nterm_count + 1 ) * sizeof( NTERM_STR * ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr NTERM_STR holen */
	if( ( nterm_str[nterm_count] = ( NTERM_STR * ) calloc( 1, sizeof( NTERM_STR ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Die PrioritÑt auf -1 setzen */
	nterm_str[nterm_count]->priori = -1;

	/* Alles OK */
	return( 0 );
}


WORD sort_nterm_str( VOID )
{
	/* Hier werden die NTERM_STRs sortiert. */
	REG LONG	i, j, p, k;
	NTERM_STR	**temp_nterm_str;

	/* Speicher fÅr die Indexe der Produktionen holen */
	if( ( prod_index_arr = ( LONG * ) malloc( sizeof( LONG ) * prod_count ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr alte Indexe der Produktionen holen */
	if( ( old_nterm_index = ( LONG * ) malloc( ( nterm_count + 1 ) * sizeof( LONG ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr neuen NTERM_FIRST-Eintrag holen */
	if( ( nterm_first = ( NTERM_FIRST ** ) realloc( ( VOID * ) nterm_first, ( nterm_first_count + 1 ) * sizeof( NTERM_FIRST * ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr NTERM_FIRST holen */
	if( ( nterm_first[nterm_first_count] = ( NTERM_FIRST * ) calloc( 1, sizeof( NTERM_FIRST ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* nterm_index von nterm_str in nterm_first eintragen */
	nterm_first[nterm_first_count++]->nterm_index = 0;

	/* Speicher fÅr temp_nterm_str anfordern */
	if( ( temp_nterm_str = ( NTERM_STR ** ) malloc( nterm_count * sizeof( NTERM_STR * ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alle NTERM_STRs */
	for( i = j = k = 1; j <= nterm_count; i++ )
	{
		/* Ist es eine Produktion ? */
		if( !nterm_str[i]->alt )
		{
			/* Produktion eintragen */
			temp_nterm_str[j] = nterm_str[i];

			/* Die alten Produktions-Indexe fÅr SEM_ACT merken */
			old_nterm_index[i] = j;

			/* Index merken */
			prod_index_arr[k - 1] = j;

			/* Nummer der Produktion */
			temp_nterm_str[j]->prod_index = k++;

			/* Speicher fÅr neuen NTERM_FIRST-Eintrag holen */
			if( ( nterm_first = ( NTERM_FIRST ** ) realloc( ( VOID * ) nterm_first, ( nterm_first_count + 1 ) * sizeof( NTERM_FIRST * ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Speicher fÅr NTERM_FIRST holen */
			if( ( nterm_first[nterm_first_count] = ( NTERM_FIRST * ) calloc( 1, sizeof( NTERM_FIRST ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* nterm_index von nterm_str in nterm_first eintragen */
			nterm_first[nterm_first_count++]->nterm_index = j;

			/* Die Position in der nterm_str merken ? */
			if( !hash_tab[temp_nterm_str[j++]->hash_tab_index].nterm_index )
				/* Ja */
				hash_tab[temp_nterm_str[j - 1]->hash_tab_index].nterm_index = j - 1;

			/* i merken */
			p = i;

			/* Solange Nachfolger existieren */
			while( nterm_str[p]->post_index )
			{
				/* Nachfolger merken */
				p = nterm_str[p]->post_index;

				/* Alternative merken */
				temp_nterm_str[j] = nterm_str[p];

				/* Die alten Produktions-Indexe fÅr SEM_ACT merken */
				old_nterm_index[p] = j;

				/* Die Position in der nterm_str merken */
				if( !hash_tab[temp_nterm_str[j++]->hash_tab_index].nterm_index )
					/* Ja */
					hash_tab[temp_nterm_str[j - 1]->hash_tab_index].nterm_index = j - 1;
			}
		}
	}

	/* Speicher freigeben */
	free( ( VOID * ) nterm_str );

	/* Sortierte NTERM_STRs uebernehmen */
	nterm_str = temp_nterm_str;

	/* Alles OK */
	return( 0 );
}


WORD test_term( VOID )
{
	/* Hier werden die Terminale und Nichtterminale ueberprueft. */
	REG LONG	i, err_flg = 0;

	/* Alle Nichtterminale pruefen */
	for( i = 0; i < nterm_arr_count; i++ )
	{
		/* Wurde das Nichtterminal definiert ? */
		if( !hash_tab[nterm_arr[i]].left_def )
		{
			/* Nein, Fehler */
			err_flg++;

			/* Fehlermeldung ausgeben */
			error( NOT_DEFINED_NTERM, hash_tab[nterm_arr[i]].zeile, ( ULONG ) hash_tab[nterm_arr[i]].name );
		}

		/* Wurde des Nichtterminal benutzt ? */
		else if( !hash_tab[nterm_arr[i]].def )
			/* Fehlermeldung ausgeben */
			error( NOT_USED_NTERM, hash_tab[nterm_arr[i]].zeile, ( ULONG ) hash_tab[nterm_arr[i]].name );
	}

	/* Speicher des nterm_arr freigeben */
	free( ( VOID * ) nterm_arr );

	/*
		Alle TERM_STRs
		EPSILON(0) und ACCEPT(1) nicht pruefen, da sie immer eingetragen
		werden.
	*/
	for( i = 2; i <= term_count; i++ )
	{
		/* Wurde das Terminal benutzt ? */
		if( !hash_tab[term_str[i].hash_tab_index].def )
			/* Meldung ausgeben */
			error( NOT_USED_TERM, hash_tab[term_str[i].hash_tab_index].zeile, ( ULONG ) hash_tab[term_str[i].hash_tab_index].name );
	}

	/* Alles OK */
	return( err_flg == 0 ? 0 : 1 );
}


WORD insert_start_sym( VOID )
{
	/* Hier wird das Startsymbol in die Grammatik eingetragen. */
	LONG	index;

	/* Nichtterminal in Hash-Tabelle eintragen */
	if( ( index = insert_hash_tab( "start", NTERM ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Speicher fÅr NTERM_STR holen */
	if( ( nterm_str[0] = ( NTERM_STR * ) calloc( 1, sizeof( NTERM_STR ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Index in der NTERM_STR eintragen */
	nterm_first[0]->nterm_index = 0;

	/* Hash-Index merken */
	nterm_str[0]->hash_tab_index = index;

	/* Nichtterminal auf der rechten Seite der Start-Produktion eintragen */
	if( insert_right( nterm_str[0], nterm_str[1]->hash_tab_index ) < 0 )
		/* Fehler */
		return( -1 );

	/* Nichtterminal wird benutzt */
	hash_tab[nterm_str[1]->hash_tab_index].def++;

	/* start-Nichtterminal wurde definiert */
	hash_tab[index].def++;
	hash_tab[index].left_def++;

	/* ACCEPT auf der rechten Seite der Start-Produktion eintragen */
	if( insert_right( nterm_str[0], accept ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}