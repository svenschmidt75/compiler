/*
	Projekt		:	LR-Parser-Generator V1.00û
	Modul		:	ANALYZE.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/



/************/
/* Includes */
/************/

#include <portab.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <ext.h>
#include <tos.h>

/* Include-Datei des Parser-Generators */
#include "LR.H"



/**************/
/* Prototypen */
/**************/

WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD do_error( LONG prod_index, WORD analyze, WORD ebnf_err );
WORD insert_right( NTERM_STR *nterm_str, LONG index );
WORD do_prec( NTERM_STR *nterm_str, WORD analyze );
LONG insert_hash_tab( BYTE *string, WORD type );
WORD malloc_nterm_str( LONG temp_nterm_count );
LONG hash_pjw( BYTE *string, ULONG length );
WORD do_assoc( WORD assoc, WORD analyze );
LONG do_coll( BYTE *string );
WORD insert_start_sym( VOID );
WORD sort_nterm_str( VOID );
WORD extract_word( VOID );
WORD test_term( VOID );
WORD del_sep( VOID );
WORD delete( VOID );



/*****************/
/* Deklarationen */
/*****************/

/* FÅr die Info-Datei */
#define DEST			605
#define ANALYZE			510
#define SHOW_ITEMS		252
#define LR_TABELLE		459
#define TYPE			198
#define COMPRESS		860
#define DEBUG			400
#define OUTPUT_GRAMMAR	715
#define FOLLOW			768
#define FIRST			933
#define HASH_TAB_SIZE	166
#define MAX_CHAR		59

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

/* Zeiger auf ERROR_STRs */
ERROR_STR	**error_struct = 0L;

/* Anzahl der ERROR_STRs in error_struct */
LONG		error_struct_count = 0;

/* Anzahl der Produktionen */
ULONG		prod_count = 0;

/* LÑnge des lÑngsten Nichtterminals */
ULONG		nterm_length;

/* NTERM_FIRST */
NTERM_FIRST	**nterm_first = 0L;

/* ZÑhler fÅr NTERM_FIRST */
ULONG		nterm_first_count = 0;



/***********/
/* Imports */
/***********/

MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL LR_INFO		lr_info;
MGLOBAL ULONG		zeile;
MGLOBAL FILE		*file;
MGLOBAL BYTE		*grammar;



WORD delete( VOID )
{
	/* In dieser Routine werden alle unnÅtzen Dinge gelîscht. */
	WORD	comment = 0;

	/* Endlosschleife */
	do
	{
		/* Anhand des Zeichens die Maûnahmen einleiten */
		switch( *grammar )
		{
			/* Space, Tab */
			case ' ':
			case '\t':
			{
				/* Zeichen Åberlesen */
				grammar++;

				/* NÑchstes Zeichen bearbeiten */
				break;
			}

			/* Neue Zeile */
			case 0x0d:
			{
				/* Zeichen Åberlesen */
				grammar++;
			}

			/* LineFeed */
			case '\n':
			{
				/* NÑchste Zeile */
				zeile++;

				/* NÑchstes Zeichen bearbeiten */
				grammar++;
				break;
			}

			case ';':
			{
				/* Kommentar */
				while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
					/* Zeichen Åberlesen */
					grammar++;

				/* NÑchstes Zeichen bearbeiten */
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
		/* Ist es ein Terminal oder ein Nichtterminal ? */
		switch( *grammar )
		{
			/* Ist es ein Terminal ? */
			case '\'':
			case '\"':
			{
				/* Ist ein Terminal erkannt worden ? */
				if( type == TERM )
				{
					/* Wort merken */
					strncpy( grammar_word, temp_grammar, ( grammar - temp_grammar ) );

					/* Null-Byte anhÑngen */
					grammar_word[grammar - temp_grammar] = 0;

					/* Terminal-Abschluûzeichen Åberlesen */
					grammar++;

					/* War ein Terminal */
					length = ( ULONG ) ( grammar - temp_grammar );

					/* Die LÑnge des Terminals festhalten */
					if( length > nterm_length )
						/* Neues Term ist lÑnger als das Alte */
						nterm_length = length;

					/* War ein Terminal */
					return( TERM );
				}

				/* Wurde ein Nichtterminal erkannt ? */
				else if( type == NTERM )
					/* Fehlermeldung ausgeben */
					return( error( INVALID_CHAR, ( ULONG ) *grammar, 0 ) );

				/* Ist ein Terminal */
				type = TERM;

				/* Zeichen Åberlesen */
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
			case 'û':
			{
				/* Ist es Nichtterminal ? */
				if( type < 0 )
					/* Ist ein Nichtterminal */
					type = NTERM;

				/* Mit dem nÑchsten Zeichen weitermachen */
				grammar++;
				break;
			}

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
				/* Fehlermeldung ausgeben */
				return( error( INVALID_CHAR, ( ULONG ) *grammar, 0 ) );
		}

	}while( 1 );
}


WORD analyze_info_file( VOID )
{
	/* Hier wird die Info-Datei ausgewertet. */
	WORD	state = OK, value;

	/* Spaces etc. lîschen */
	state = delete();

	/* Solange die Datei nicht zu Ende ist */
	while( state != END )
	{
		/* Wort extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Hashen */
		value = ( WORD ) hash_pjw( grammar_word, 1000 );

		/* Spaces etc. lîschen */
		delete();

		/* Seperator lîschen */
		grammar++;

		/* Spaces etc. lîschen */
		delete();

		/* Angabe extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Welches ist es ? */
		switch( value )
		{
			case DEST:
			{
				/* Ausgabe ind Datei ? */
				if( !strcmp( grammar_word, "FILE" ) )
					/* Ausgabe in Datei */
					lr_info.dest = DFILE;

				/* Ausgabe auf Drucker ? */
				else if( !strcmp( grammar_word, "PRINTER" ) )
					/* Ausgabe auf Drucker */
					lr_info.dest = DPRINTER;

				else
					/* Ausgabe auf Bildschirm */
					lr_info.dest = DSCREEN;

				/* Weitermachen */
				break;
			}

			case ANALYZE:
			{
				/* Analyse der Grammatik */

				/* Meldungen ausgeben ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* Meldungen ausgeben */
					lr_info.analyze = ON;

				else
					/* Meldungen nicht ausgeben */
					lr_info.analyze = OFF;

				/* Weitermachen */
				break;
			}

			case SHOW_ITEMS:
			{
				/* LR-Items */

				/* LR-Items ausgeben ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* LR-Items ausgeben */
					lr_info.lr_items = ON;

				else
					/* LR-Items nicht ausgeben */
					lr_info.lr_items = OFF;

				/* Weitermachen */
				break;
			}

			case LR_TABELLE:
			{
				/* LR-Tabelle */

				/* LR-Tabelle ausgeben ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* LR-Tabelle ausgeben */
					lr_info.lr_tab = ON;

				else
					/* LR-Tabelle nicht ausgeben */
					lr_info.lr_tab = OFF;

				/* Weitermachen */
				break;
			}

			case TYPE:
			{
				/* LR-Parser-Typ */

				/* LR-Parser erzeugen ? */
				if( !strcmp( grammar_word, "LR" ) )
					/* LR-Parser erzeugen */
					lr_info.type = LR;

				/* LALR-Parser erzeugen ? */
				else if( !strcmp( grammar_word, "LALR" ) )
					/* LALR-Parser erzeugen */
					lr_info.type = LALR;

				else
					/* SLR-Parser erzeugen */
					lr_info.type = SLR;

				/* Weitermachen */
				break;
			}

			case DEBUG:
			{
				/* Debug-Version */

				/* Debug-Version erzeugen ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* Debug-Version erzeugen */
					lr_info.debug = ON;

				else
					/* Debug-Version nicht erzeugen */
					lr_info.debug = OFF;

				/* Weitermachen */
				break;
			}

			case COMPRESS:
			{
				/* LR-Tabelle komprimieren ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* LR-Tabelle komprimieren */
					lr_info.compress = ON;

				else
					/* LR-Tabelle nicht komprimieren */
					lr_info.compress = OFF;

				/* Weitermachen */
				break;
			}

			case OUTPUT_GRAMMAR:
			{
				/* Grammatik ausgeben */

				/* Umgewandelte Grammatik ausgeben ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* Umgewandelte Grammatik ausgeben */
					lr_info.output_grammar = ON;

				else
					/* Umgewandelte Grammatik nicht ausgeben */
					lr_info.output_grammar = OFF;

				/* Weitermachen */
				break;
			}

			case FOLLOW:
			{
				/* FOLLOW-Elemente */

				/* Follow-Menge ausgeben ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* Follow-Menge ausgeben */
					lr_info.follow = ON;

				else
					/* Follow-Menge nicht ausgeben */
					lr_info.follow = OFF;

				/* Weitermachen */
				break;
			}

			case FIRST:
			{
				/* FIRST-Elemente */

				/* First-Menge ausgeben ? */
				if( !strcmp( grammar_word, "ON" ) )
					/* First-Menge ausgeben */
					lr_info.first = ON;

				else
					/* First-Menge nicht ausgeben */
					lr_info.first = OFF;

				/* Weitermachen */
				break;
			}

			case HASH_TAB_SIZE:
			{
				/* Grîûe der Hash-Tabelle */

				/* String in Zahl wandeln */
				lr_info.hash_tab_size = ( ULONG ) atol( grammar_word );

				/* Weitermachen */
				break;
			}

			case MAX_CHAR:
			{
				/* Max. Anzahl der Zeichen in einer Zeile */

				/* String in Zahl wandeln */
				lr_info.max_char = atoi( grammar_word );

				/* Weitermachen */
				break;
			}

			default:
				/* Unbekannte Angabe */
				return( error( UNKNOWN_INFO_FILE_STATEMENT, 0, 0 ) );
		}

		/* Spaces etc. lîschen */
		state = delete();
	}

	/* Alles OK */
	return( 0 );
}


LONG insert_hash_tab( BYTE *string, WORD type )
{
	/* Hier wird ein Terminal/Nichtterminal in die Hash-Tab eingetragen. */
	ULONG	index;

	/* Auf Kollisionen ÅberprÅfen */
	index = do_coll( string );

	/* Ist der Eintrag noch frei ? */
	if( !hash_tab[index].name )
	{
		/* Speicher fÅr den String anfordern */
		if( !( hash_tab[index].name = ( BYTE * ) malloc( strlen( string ) + 1 ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* String eintragen */
		strcpy( hash_tab[index].name, string );

		/* Ist es ein Terminal ? */
		if( type == TERM )
		{
			/* Speicher fÅr TERM_STR anfordern */
			if( !( term_str = ( TERM_STR * ) realloc( term_str, ( ++term_count + 1 ) * sizeof( TERM_STR ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Struktur initialisieren */
			memset( ( VOID * ) &term_str[term_count], 0, sizeof( TERM_STR ) );

			/* Index des Hash-Eintrages merken */
			term_str[term_count].hash_tab_index = index;

			/* Zeiger auf TERM_STR in Hash-Tabelle vermerken */
			hash_tab[index].term_str = &term_str[term_count];
		}
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

			/* TabellenÅberlauf ? */
			if( ( index + col_jump ) > lr_info.hash_tab_size )
				/* Wert anpassen */
				index %= lr_info.hash_tab_size;

			/* Max. zulÑssige Anzahl an Kollisionen Åberschritten ? */
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
	/* Diese Funktion liefert einen HASH-Index fÅr den String zurÅck. */
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
	ULONG		temp_zeile, prod_index, main_prod_index, temp;
	LONG		temp_nterm_count = 0, index;
	STACK		stack[100];

	/* LÑnge von ACCEPT ist Default-LÑnge */
	nterm_length = 6;

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
		/* Ist die Anzahl der Zeilen nun grîûer ? */
		if( zeile > temp_zeile )
			/* Ja, eine neue Produktion oder eine Alternative( | ) */
			analyze_prod = 0;

		switch( *grammar )
		{
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
					/* FÅr Fehlerbehandlung */
					ebnf_err = 3;

				else
					/* FÅr Fehlerbehandlung */
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

				/* Eine weitere Produktion */
				prod_count++;

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

				/* FÅr Fehlerbehandlung */
				ebnf_err = 1;

				/* Noch keine Alternativen */
				alt = 1;

				/* Speicher fÅr NTERM_STR holen */
				if( ( malloc_nterm_str( temp_nterm_count ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Index des aux_syms in Hash-Tabelle eintragen */
				nterm_str[nterm_count]->hash_tab_index = prod_index = index;

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
				/* Ausdruck schlieûen */

				/* Stackpointer heruntersetzen */
				sp--;

				/* Stackunterlauf ? */
				if( sp >= 0 )
				{
					/* Fehler bei den Meta-Symbolen ? */
					if( ebnf_err != 3 )
						/* Fehler */
						return( error( EBNF_SYM_ERR, ( ULONG ) ebnf_sym[ebnf_err - 1], ( ULONG ) hash_tab[main_prod_index].name ) );

					/* Wichtige Variablen zurÅckholen */
					prod_index = stack[sp].prod_index;
					ebnf_err = stack[sp].ebnf_err;
					alt = stack[sp].alt;

					/* nterm_count merken */
					temp_nterm_count = nterm_count;
					nterm_count = stack[sp].nterm_count;
				}

				else
					/* Stack-Unterlauf */
					error( TOO_MANY_EBNF_SYMB, ( BYTE ) *grammar, ( ULONG ) hash_tab[main_prod_index].name );

				/* Meta-Symbol lîschen */
				grammar++;

				/* Weitermachen */
				break;
			}

			case '}':
			case ']':
			{
				/* Ausdruck schlieûen */

				/* Stackunterlauf ? */
				if( ( sp - 1 ) >= 0 )
				{
					/* FÅr die Fehlerbehandlung */
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

					/* Wichtige Variablen zurÅckholen */
					prod_index = stack[sp].prod_index;
					ebnf_err = stack[sp].ebnf_err;
					alt = stack[sp].alt;

					/* nterm_count merken */
					temp_nterm_count = nterm_count;
					nterm_count = stack[sp].nterm_count;
				}

				else
					/* Stack-Unterlauf */
					error( TOO_MANY_EBNF_SYMB, ( BYTE ) *grammar, ( ULONG ) hash_tab[main_prod_index].name );

				/* Meta-Symbol lîschen */
				grammar++;

				/* Weitermachen */
				break;
			}

			case '%':
			{
				/* %left, %right, %prec und %error */

				/* %prec ? */
				if( !strncmp( grammar, "%prec", 5 ) )
				{
					/* PrioritÑt der Produktion Ñndern */
					if( ( state = do_prec( nterm_str[nterm_count], analyze ) ) < 0 )
						/* Fehler */
						return( -1 );

					/* Weitermachen */
					break;
				}

				/* %error ? */
				else if( !strncmp( grammar, "%error", 6 ) )
				{
					/* Fehlermeldung auswerten */
					if( do_error( nterm_count, analyze, ebnf_err ) < 0 )
						/* Fehler */
						return( -1 );

					/* Weitermachen */
					break;
				}

				/* %left ? */
				else if( !strncmp( grammar, "%left", 5 ) )
				{
					/* Terminal soll Links-Assoziativ sein */
					assoc = LEFT;

					/* %left Åberspringen */
					grammar += 5;
				}

				/* %right ? */
				else if( !strncmp( grammar, "%right", 6 ) )
				{
					/* Terminal soll Rechts-Assoziativ sein */
					assoc = RIGHT;

					/* %right Åberspringen */
					grammar += 6;
				}

				/* AssoziativitÑt und PrioritÑt des Terminals auswerten */
				if( do_assoc( assoc, analyze ) < 0 )
					/* Fehler */
					return( -1 );

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

				/* Wort extrahieren */
				if( ( state = extract_word() ) < 0 )
					/* Fehler */
					return( -1 );

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
							return( error( YET_DEFINED_TERM, ( ULONG ) hash_tab[index].name, 0 ) );

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

				/* Weitermachen */
				break;
			}
		}

		/* Anzahl der Zeilen merken */
		temp_zeile = zeile;

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

	/* Die Nichtterminale und Terminale prÅfen */
	if( test_term() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD do_prec( NTERM_STR *nterm_str, WORD analyze )
{
	/* Hier wird die PrioritÑt der Produktion geÑndert. */
	ULONG		index;
	WORD		state;

	/* %prec nur im Zusammenhang mit einer Produktion erlaubt */
	if( !analyze )
		/* Fehler */
		return( error( PREC_ERR, 0, 0 ) );

	/* %prec Åberspringen */
	grammar += 5;

	/* Spaces etc. lîschen */
	state = delete();

	/* Terminal extrahieren */
	if( extract_word() < 0 )
		/* Fehler */
		return( -1 );

	/* PrÅfen, ob das Terminal schon in der Hash-Tabelle eingetragen worden ist */
	index = do_coll( grammar_word );

	/* Leerer Eintrag ? */
	if( !hash_tab[index].name )
		/* Terminal wurde nicht definiert */
		error( NOT_DEFINED_PREC_TERM, ( ULONG ) grammar_word, 0 );

	/* PrioritÑt und AssoziativitÑt des Terminals Åbernehmen */
	else
	{
		/* PrioritÑt des Terminals < PrioritÑt der Porduktion ? */
		if( hash_tab[index].term_str->priori < nterm_str->priori )
			/* Meldung ausgeben */
			error( PREC_PRIORI, 0, 0 );

		/* Terminal wurde benutzt */
		hash_tab[index].def++;

		/* PrioritÑt des Terminals in Porduktion eintragen */
		nterm_str->priori = hash_tab[index].term_str->priori;
				
		/* AssoziativitÑt des Terminals in Porduktion eintragen */
		nterm_str->assoc = hash_tab[index].term_str->assoc;
	}

	/* Alles OK */
	return( state );
}


WORD do_error( LONG prod_index, WORD analyze, WORD ebnf_err )
{
	/* Hier wird die Fehlermeldung ausgewertet. */
	LONG		index = 0, i;
	WORD		pkt_pos, flg;
	ERROR_STR	*error_str, **temp_error_str;

	/* %error muû im Zusammenhang mit Produktionen stehen */
	if( !analyze )
		/* Fehler */
		return( error( ERROR_ERR, 0, 0 ) );

	/* %error lîschen */
	grammar += 6;

	/* Spaces etc. lîschen */
	delete();

	/* Den Blockbeginn lîschen */
	grammar++;

	/* Spaces etc. lîschen */
	delete();

	/* Solange Angaben da sind */
	while( *grammar != '}' )
	{
		/* Zahl extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Punkt-Position merken */
		pkt_pos = atoi( grammar_word );

		/* Ist es eine Produktion, 0-beliebig oft ? */
		if( ebnf_err == 1)
			/* Ja, das erste Element ist ein aux_sym -> korregieren */
			pkt_pos++;

		/* Doppelpunkt lîschen */
		grammar++;

		/* Folgt ein Terminal ? */
		if( *grammar == '\\' )
		{
			/* Ja, eine Fehlermeldung der Art 'use x instead of y !' */
			grammar++;

			/* Terminal extrahieren */
			if( extract_word() < 0 )
				/* Fehler */
				return( -1 );

			/* Terminal in Hash-Tabelle eintragen */
			if( ( index = insert_hash_tab( grammar_word, TERM ) ) < 0 )
				/* Fehler */
				return( -1 );
		}

		/* Spaces etc. lîschen */
		delete();

		/* Fehlermeldung extrahieren */
		for( i = 0; ( *grammar != 0x0a ) && ( *grammar != 0x0d ); )
			/* Zeichenweise kopieren */
			grammar_word[i++] = *grammar++;

		/* Null-Byte anhÑngen */
		grammar_word[i] = 0;

		/* Speicher fÅr die ERROR_STR anfordern */
		if( !( error_str = ( ERROR_STR * ) calloc( 1, sizeof( ERROR_STR ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Speicher fÅr Fehlermeldung anfordern */
		if( !( error_str->err_msg = ( BYTE * ) malloc( strlen( grammar_word ) + 1 ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Fehlereldung in ERROR_STR eintragen */
		strcpy( error_str->err_msg, grammar_word );

		/* Index der Produktion eintragen */
		error_str->nterm_index = prod_index;

		/* Position des Punktes eintragen */
		error_str->pkt_pos = pkt_pos;

		/* Terminal eintragen */
		error_str->term = index;

		/* index zurÅcksetzen */
		index = 0;

		/* Flag zurÅcksetzen */
		flg = 0;

		/*
			Ist schon ein Eintrag mit der gleichen Produktion und gleicher
			Punkt-Position da ?
		*/
		if( error_struct_count )
		{
			/* Gibt es schon eine ERROR_STR fÅr die gleiche pkt_pos ? */
			for( i = 0; ( i < error_struct_count ) && ( !flg ); i++ )
			{
				/* Gleiche Produktion und pkt_pos ? */
				if( ( error_struct[i]->nterm_index == prod_index ) && ( error_struct[i]->pkt_pos == pkt_pos ) )
				{
					/* Ja */

					/* Fehlermeldung an die vorherige anhÑngen */
					temp_error_str = &error_struct[i];

					do
					{
						/* Ist der Zeiger auf die nachste ERROR_STR leer ? */
						if( !(*temp_error_str)->error_str )
						{
							/* Ja, neue Fehlerstruktur anhÑngen */
							(*temp_error_str)->error_str = error_str;

							/* Flag setzen */
							flg++;
							break;
						}

						/* Adresse holen */
						temp_error_str = &(*temp_error_str)->error_str;

					}while( *temp_error_str );
				}
			}

			/* Wurde die ERROR_STR eingetragen ? */
			if( !flg )
			{
				/* Nein, ERROR_STR in ERROR_STR-Array eintragen */
				if( !( error_struct = ( ERROR_STR ** ) realloc( error_struct, ( error_struct_count + 1 ) * sizeof( ERROR_STR * ) ) ) )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

				/* ERROR_STR eintragen */
				error_struct[error_struct_count++] = error_str;
			}
		}

		else
		{
			/* ERROR_STR in ERROR_STR-Array eintragen */
			if( !( error_struct = ( ERROR_STR ** ) realloc( error_struct, ( error_struct_count + 1 ) * sizeof( ERROR_STR * ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* ERROR_STR eintragen */
			error_struct[error_struct_count++] = error_str;
		}

		/* Spaces etc. lîschen */
		delete();
	}

	/* Das Blockende lîschen */
	grammar++;

	/* Alles OK */
	return( 0 );
}


WORD do_assoc( WORD assoc, WORD analyze )
{
	/* Hier wird die AssoziativitÑt und PrioritÑt eines Terminals vermerkt. */
	MLOCAL WORD	priori = 1;
	LONG		temp_zeile = zeile, index;

	/* %left und %right mÅssen vor der Grammatik stehen */
	if( analyze )
		/* Fehler */
		return( error( LR_ERR, 0, 0 ) );

	/* Spaces etc. lîschen */
	delete();

	/* Bis Zeilenwechsel */
	while( zeile == temp_zeile )
	{
		/* Terminal extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Terminal Hash-Tabelle eintragen */
		if( ( index = insert_hash_tab( grammar_word, TERM ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* PrioritÑt vermerken */
		hash_tab[index].term_str->priori = priori++;

		/* AssoziativitÑt vermerken */
		hash_tab[index].term_str->assoc = assoc;

		/* Spaces etc. lîschen */
		delete();
	}

	/* Alles OK */
	return( 0 );
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

	/* Eine letzte öberprÅfung des Seperators */
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
	if( nterm_str->anz_ri > MAX_ANZ_RI_ELEM )
		/* Nicht mehr genug Platz da */
		return( error( MAX_ANZ_RI_ERR, 0, 0 ) );

	/* Element eintragen */
	nterm_str->ri[nterm_str->anz_ri++] = index;

	/* Alles OK */
	return( 0 );
}


WORD malloc_nterm_str( LONG temp_nterm_count )
{
	/* Hier wird eine NTERM_STR-Struktur zu nterm_str hinzugefÅgt. */

	/* temp_nterm_count > nterm_count ? */
	if( temp_nterm_count > nterm_count )
	{
		/* Ja, Åbernehmen */
		nterm_count = temp_nterm_count;

		/* temp_nterm_count lîschen */
		temp_nterm_count = 0;
	}

	/* Ein weiterer Eintrag */
	nterm_count++;

	/* Speicher fÅr neuen NTERM_STR-Eintrag holen */
	if( !( nterm_str = ( NTERM_STR ** ) realloc( nterm_str, ( nterm_count + 1 ) * sizeof( NTERM_STR * ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr NTERM_STR holen */
	if( !( nterm_str[nterm_count] = ( NTERM_STR * ) calloc( 1, sizeof( NTERM_STR ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD sort_nterm_str( VOID )
{
	/* Hier werden die NTERM_STRs sortiert. */
	REG LONG	i, j = 1, p, k;
	NTERM_STR	**temp_nterm_str;

	/* Speicher fÅr neuen NTERM_FIRST-Eintrag holen */
	if( !( nterm_first = ( NTERM_FIRST ** ) realloc( ( VOID * ) nterm_first, ( nterm_first_count + 1 ) * sizeof( NTERM_FIRST * ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr NTERM_FIRST holen */
	if( !( nterm_first[nterm_first_count] = ( NTERM_FIRST * ) calloc( 1, sizeof( NTERM_FIRST ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* nterm_index von nterm_str in nterm_first eintragen */
	nterm_first[nterm_first_count++]->nterm_index = 0;

	/* Speicher fÅr temp_nterm_str anfordern */
	if( !( temp_nterm_str = ( NTERM_STR ** ) malloc( nterm_count * sizeof( NTERM_STR * ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alle NTERM_STRs */
	for( i = 1, k = 1; j <= nterm_count; i++ )
	{
		/* Ist es eine Produktion ? */
		if( !nterm_str[i]->alt )
		{
			/* Produktion eintragen */
			temp_nterm_str[j] = nterm_str[i];

			/* Nummer der Produktion */
			temp_nterm_str[j]->prod_index = k++;

			/* Speicher fÅr neuen NTERM_FIRST-Eintrag holen */
			if( !( nterm_first = ( NTERM_FIRST ** ) realloc( ( VOID * ) nterm_first, ( nterm_first_count + 1 ) * sizeof( NTERM_FIRST * ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Speicher fÅr NTERM_FIRST holen */
			if( !( nterm_first[nterm_first_count] = ( NTERM_FIRST * ) calloc( 1, sizeof( NTERM_FIRST ) ) ) )
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

				/* Die Position in der nterm_str merken */
				if( !hash_tab[temp_nterm_str[j++]->hash_tab_index].nterm_index )
					/* Ja */
					hash_tab[temp_nterm_str[j - 1]->hash_tab_index].nterm_index = j - 1;
			}
		}
	}

	/* Speicher freigeben */
	free( ( VOID * ) nterm_str );

	/* Sortierte NTERM_STRs Åbernehmen */
	nterm_str = temp_nterm_str;

	/* Alles OK */
	return( 0 );
}


WORD test_term( VOID )
{
	/* Hier werden die Terminale und Nichtterminale ÅberprÅft. */
	REG LONG	i;

	/* Alle NTERM_STRs */
	for( i = 1; i <= nterm_count; i++ )
	{
		/* Wurde des Nichtterminal definiert ? */
		if( ( !hash_tab[nterm_str[i]->hash_tab_index].left_def ) && ( !nterm_str[i]->alt ) )
			/* Nein, Fehler */
			return( error( NOT_DEFINED_NTERM, ( ULONG ) hash_tab[nterm_str[i]->hash_tab_index].name, 0 ) );

		/* Wurde des Nichtterminal benutzt ? */
		else if( !hash_tab[nterm_str[i]->hash_tab_index].def )
			/* Nein, Meldung ausgeben */
			error( NOT_USED_NTERM, ( ULONG ) hash_tab[nterm_str[i]->hash_tab_index].name, 0 );
	}

	/*	Alle TERM_STRs
		EPSILON(0) und ACCEPT(1) nicht prÅfen, da sie immer eingetragen
		werden.
	*/
	for( i = 2; i <= term_count; i++ )
	{
		/* Wurde das Terminal benutzt ? */
		if( !hash_tab[term_str[i].hash_tab_index].def )
			/* Nein, Meldung ausgeben */
			error( NOT_USED_TERM, ( ULONG ) hash_tab[term_str[i].hash_tab_index].name, 0 );
	}

	/* Alles OK */
	return( 0 );
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
	if( !( nterm_str[0] = ( NTERM_STR * ) calloc( 1, sizeof( NTERM_STR ) ) ) )
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

	/* ACCEPT auf der rechten Seite der Start-Produktion eintragen */
	if( insert_right( nterm_str[0], accept ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}