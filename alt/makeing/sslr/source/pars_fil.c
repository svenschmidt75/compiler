/*
	Projekt		:	LR-Parser-Generator
	Modul		:	PARS_FIL.C
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
WORD cmp_lr_entry( LONG sour_lr_tab, LONG dest_lr_tab );
LONG insert_hash_tab( BYTE *string, WORD type );
WORD do_error( LONG prod_index, WORD ebnf_err );
WORD do_prec( NTERM_STR *nterm_str );
WORD compress_lr_tab( VOID );
LONG do_coll( BYTE *string );
WORD do_assoc( WORD assoc );
LONG cmp_type( BYTE *type );
WORD do_nterm_type( VOID );
WORD do_term_type( VOID );
WORD extract_word( VOID );
WORD do_union( VOID );
WORD do_init( VOID );
WORD do_incl( VOID );
WORD delete( VOID );


/*****************/
/* Deklarationen */
/*****************/


/* fÅr die Info-Datei */
#define DEST			0
#define ERR_DEST		1
#define ANALYZE			2
#define SHOW_ITEMS		3
#define LR_TABELLE		4
#define TYPE			5
#define COMPRESS		6
#define DEBUG			7
#define OUTPUT_GRAMMAR	8
#define FOLLOW			9
#define FIRST			10
#define HASH_TAB_SIZE	11
#define MAX_CHAR		12


/****************/
/* Definitionen */
/****************/


/* Zeiger auf ERROR_STRs */
ERROR_STR	**error_struct = 0L;

/* Anzahl der ERROR_STRs in error_struct */
LONG		error_struct_count = 0;

/* Anzahl der doppelten HÅllen */
ULONG		main_dbl_count = 0;

/* Beinhaltet Zeiger auf die LR-Tabellen-EintrÑge */
LONG		*compressed_lr_index_tab;

/* Bezeichner in der Union-Struktur */
BYTE		**union_ident;

/* Beinhaltet die Union aus der Grammatik-Datei */
BYTE		*union_struct = 0L;

/* Beinhaltet die Initialisierung des Parsers */
BYTE		*init;

/* Beinhaltet die Hilfsroutinen des Parsers */
BYTE		*incl;

/* Array mit Zeigern auf die Typen der Terminale/Nichtterminale */
BYTE		**type_arr = 0L;

/* Anzahl der Eintragungen in type_arr */
LONG		type_arr_count = 0;


/***********/
/* Imports */
/***********/

MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL	HULLE_STR	**hulle_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL LR_INFO		lr_info;
MGLOBAL	LR_TAB		**lr_tab;
MGLOBAL ULONG		zeile;
MGLOBAL ULONG		prod_count;
MGLOBAL ULONG		*ziel_tab;
MGLOBAL FILE		*file;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		term_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL BYTE		*grammar;
MGLOBAL BYTE		grammar_word[];



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
		{
			/* Fehler, bis zum Zeilenende alles lîschen */
			while( *grammar && ( ( grammar[0] != 0x0d ) && ( grammar[0] != '\n' ) ) )
				/* lîschen */
				grammar++;

			/* Lîschen */
			state = delete();

			/* Weitermachen */
			continue;
		}

		/* Welches Wort ist es ? */
		if( !strcmp( grammar_word, "DEST" ) )
			/* Ausgabe */
			value = DEST;

		else if( !strcmp( grammar_word, "ERR_DEST" ) )
			/* Ausgabe der Fehler */
			value = ERR_DEST;

		else if( !strcmp( grammar_word, "ANALYZE" ) )
			/* Analyse */
			value = ANALYZE;

		else if( !strcmp( grammar_word, "SHOW_ITEMS" ) )
			/* Items */
			value = SHOW_ITEMS;

		else if( !strcmp( grammar_word, "COMPRESS" ) )
			/* Komprimierte LR-Tabelle */
			value = COMPRESS;

		else if( !strcmp( grammar_word, "LR_TAB" ) )
			/* LR-Tabelle */
			value = LR_TABELLE;

		else if( !strcmp( grammar_word, "TYPE" ) )
			/* Parser-Typ */
			value = TYPE;

		else if( !strcmp( grammar_word, "DEBUG" ) )
			/* Debug-Version */
			value = DEBUG;

		else if( !strcmp( grammar_word, "OUTPUT_GRAMMAR" ) )
			/* Grammatik in BNF */
			value = OUTPUT_GRAMMAR;

		else if( !strcmp( grammar_word, "FOLLOW" ) )
			/* Follow-Mengen */
			value = FOLLOW;

		else if( !strcmp( grammar_word, "FIRST" ) )
			/* First-Mengen */
			value = FIRST;

		else if( !strcmp( grammar_word, "HASH_TAB_SIZE" ) )
			/* Hash-Tab-Grîsse */
			value = HASH_TAB_SIZE;

		else if( !strcmp( grammar_word, "MAX_CHAR" ) )
			/* Max. Anzahl de Zeichen */
			value = MAX_CHAR;

		else
			/* Fehler */
			value = -1;

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
				/* Ausgabe in Datei ? */
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

			case ERR_DEST:
			{
				/* Ausgabe der Fehler in Datei ? */
				if( !strcmp( grammar_word, "FILE" ) )
					/* Ausgabe in Datei */
					lr_info.err_dest = DFILE;

				/* Ausgabe der Fehler auf Drucker ? */
				else if( !strcmp( grammar_word, "PRINTER" ) )
					/* Ausgabe auf Drucker */
					lr_info.err_dest = DPRINTER;

				else
					/* Ausgabe der Fehler auf Bildschirm */
					lr_info.err_dest = DSCREEN;

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
				/* Grîsse der Hash-Tabelle */

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
			{
				/* Unbekannte Angabe */
				error( UNKNOWN_INFO_FILE_STATEMENT, 0, 0 );

				/* Bis zur nÑchsten Zeile alles lîschen */
				while( *grammar && ( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) ) )
					/* Lîschen */
					grammar++;

				/* Lîschen */
				state = delete();
			}
		}

		/* Spaces etc. lîschen */
		state = delete();
	}

	/* Alles OK */
	return( 0 );
}


WORD do_prec( NTERM_STR *nterm_str )
{
	/* Hier wird die PrioritÑt der Produktion geÑndert. */
	ULONG		index;
	WORD		state;

	/* %prec ueberspringen */
	grammar += 5;

	/* Spaces etc. lîschen */
	state = delete();

	/* Terminal extrahieren */
	if( extract_word() < 0 )
		/* Fehler */
		return( -1 );

	/* Pruefen, ob das Terminal schon in der Hash-Tabelle eingetragen worden ist */
	index = do_coll( grammar_word );

	/* Leerer Eintrag ? */
	if( !hash_tab[index].name )
		/* Terminal wurde nicht definiert */
		error( NOT_DEFINED_PREC_TERM, ( ULONG ) grammar_word, 0 );

	/* PrioritÑt und AssoziativitÑt des Terminals uebernehmen */
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


WORD do_error( LONG prod_index, WORD ebnf_err )
{
	/* Hier wird die Fehlermeldung ausgewertet. */
	LONG		term_index, index, i, type[2];
	WORD		pkt_pos, flg;
	ERROR_STR	*error_str, **temp_error_str;

	/* %error lîschen */
	grammar += 6;

	/* Spaces etc. lîschen */
	delete();

	/* Den Blockbeginn lîschen */
	grammar += 2;

	/* Spaces etc. lîschen */
	delete();

	/* Solange Angaben da sind */
	while( *grammar != '\\' )
	{
		/* fÅr Fehlerbeeinflussung */
		type[0] = term_index = 0;

		/* Position des Punktes(Zahl) extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Punkt-Position merken */
		pkt_pos = atoi( grammar_word );

		/* Ist es eine Produktion, 0 - beliebig oft ? */
		if( ebnf_err == 1 )
			/* Ja, das erste Element ist ein aux_sym -> korregieren */
			pkt_pos++;

		/* lîschen */
		type[0] = 0;

		/* Folgt ein Terminal/eine Fehlerbeeinflussung ? */
		while( *grammar == ':' )
		{
			/* Ja */
			grammar++;

			/* Terminal extrahieren */
			if( extract_word() < 0 )
				/* Fehler */
				return( -1 );

			if( !strcmp( "OVERREAD", grammar_word ) )
				/* Terminale ueberlesen */
				type[0] |= 0x01;

			else if( !strcmp( "INSERT", grammar_word ) )
				/* Terminal einfuegen */
				type[0] |= 0x02;

			else if( !strcmp( "DELETE", grammar_word ) )
				/* Terminal lîschen */
				type[0] |= 0x04;

			else if( !strcmp( "EXIT", grammar_word ) )
				/* Das Parsing abbrechen */
				type[0] |= 0x08;

			else if( !strcmp( "SEM", grammar_word ) )
				/* Semmantische Aktionen trotz Fehler zulassen */
				type[0] |= 0x10;

			else
			{
				/*
					Es ist eine Fehlermeldung der Art Use x instead of y
					beabsichtigt.
				*/

				/* Terminal in Hash-Tabelle eintragen */
				if( ( term_index = insert_hash_tab( grammar_word, TERM ) ) < 0 )
					/* Fehler */
					return( -1 );
			}

			/* Wurde eine Fehlerbeeinflussung angegeben ? */
			if( ( type[0] & 0x01 ) || ( type[0] & 0x02 ) )
			{
				/* '(' lîschen */
				grammar++;

				/* Terminal extrahieren */
				if( extract_word() < 0 )
					/* Fehler */
					return( -1 );

				/* Terminal in Hash-Tabelle eintragen */
				if( ( index = insert_hash_tab( grammar_word, TERM ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Terminal merken */
				type[1] = index;

				/* ')' lîschen */
				grammar++;
			}
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
		if( ( error_str = ( ERROR_STR * ) calloc( 1, sizeof( ERROR_STR ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Speicher fÅr Fehlermeldung anfordern */
		if( ( error_str->err_msg = ( BYTE * ) malloc( strlen( grammar_word ) + 1 ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Fehlereldung in ERROR_STR eintragen */
		strcpy( error_str->err_msg, grammar_word );

		/* Index der Produktion eintragen */
		error_str->nterm_index = prod_index;

		/* Position des Punktes eintragen */
		error_str->pkt_pos = pkt_pos;

		/* Terminal eintragen */
		error_str->term = term_index;

		/* Fehlerbeeinflussung */
		error_str->type[0] = type[0];

		/* Terminal eintragen */
		error_str->type[1] = type[1];

		/* Zeile der Definition */
		error_str->zeile = zeile;

		/* index zuruecksetzen */
		index = 0;

		/* Flag zuruecksetzen */
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
				if( ( error_struct = ( ERROR_STR ** ) realloc( error_struct, ( error_struct_count + 1 ) * sizeof( ERROR_STR * ) ) ) == NULL )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

				/* ERROR_STR eintragen */
				error_struct[error_struct_count++] = error_str;
			}
		}

		else
		{
			/* ERROR_STR in ERROR_STR-Array eintragen */
			if( ( error_struct = ( ERROR_STR ** ) realloc( error_struct, ( error_struct_count + 1 ) * sizeof( ERROR_STR * ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* ERROR_STR eintragen */
			error_struct[error_struct_count++] = error_str;
		}

		/* Spaces etc. lîschen */
		delete();
	}

	/* Das Blockende lîschen */
	grammar += 2;

	/* Alles OK */
	return( 0 );
}


WORD do_assoc( WORD assoc )
{
	/* Hier wird die AssoziativitÑt und PrioritÑt eines Terminals vermerkt. */
	MLOCAL WORD	priori = 1;
	LONG		temp_zeile = zeile, index;

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


WORD do_union( VOID )
{
	/* Hier wird die Union-Struktur ausgewertet. */
	ULONG	union_buff_size = 1000, i = 0, j = 0;

	/* %union ueberspringen */
	grammar += 6;

	/* Unnuetzes Zeug ueberlesen */
	delete();

	/* \{ lîschen */
	grammar += 2;

	/* Speicher fÅr die Union-Struktur anfordern */
	do
	{
		/* Speicher anfordern */
		if( ( union_struct = ( BYTE * ) realloc( ( VOID * ) union_struct, union_buff_size * sizeof( BYTE ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Falls noch mehr Speicher benîtigt wird */
		union_buff_size += 1000;

		/* Inhalt der Union-Struktur uebertragen */
		for( ; i < ( i + 1000 ); i++, j++ )
		{
			/* Byte kopieren */
			union_struct[j] = grammar[i];

			/* Ist die union-Struktur zu Ende ? */
			if( ( grammar[i - 1] == '\\' ) && ( grammar[i] == '}' ) )
			{
				/* Ja */
				union_struct[j - 1] = '}';
				union_struct[j] = 0;
				break;
			}

			/* Zeilenwechsel ? */
			else if( grammar[i] == 0x0a )
			{
				/* Eine weitere Zeile */
				zeile++;

				/* Nur \n eintragen */
				union_struct[--j] = '\n';
			}
		}

	}while( union_buff_size % 1000 );

	/* grammar anpassen */
	grammar += ( i + 1 );

	/* Alles OK */
	return( 0 );
}


WORD do_term_type( VOID )
{
	/* Hier kann man den Typ eines Terminals festlegen. */
	REG	ULONG	i = 0, t_zeile = zeile;
	LONG		index, type_index;
	BYTE		typ[256];

	/* %term_type lîschen */
	grammar += 10;

	/* Spaces etc. lîschen */
	delete();

	/* <typ> */
	grammar++;

	/* Solange nicht > */
	while( *grammar != '>' )
		/* Zeichen uebertragen */
		typ[i++] = *grammar++;

	/* Mit Null abschliessen */
	typ[i] = 0;

	/* Befindet sich der Typ schon im Array ? */
	if( ( type_index = cmp_type( typ ) ) < 0 )
	{
		/* Nein, Array mit den Typen anpassen */
		if( ( type_arr = ( BYTE ** ) realloc( ( VOID ** ) type_arr, sizeof( BYTE * ) * ( type_arr_count + 1 ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Speicher fÅr das Array selbst anordern */
		if( ( type_arr[type_arr_count] = ( BYTE * ) malloc( i + 1 ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Den Typ in das Typen-Array kopieren */
		strcpy( type_arr[type_arr_count], typ );

		/* Den Index des Typs merken */
		type_index = type_arr_count++;
	}

	/* > ueberlesen */
	grammar++;

	/* Spaces etc. lîschen */
	delete();

	/* Nun folgen die Terminale */
	while( zeile == t_zeile )
	{
		/* Terminal extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Terminal Hash-Tabelle eintragen */
		if( ( index = insert_hash_tab( grammar_word, TERM ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* Den Typ uebertragen */
		hash_tab[index].type = type_index;

		/* Die Zeilennummer merken */
		hash_tab[index].zeile = zeile;

		/* Spaces etc. lîschen */
		delete();
	}

	/* Alles OK */
	return( 0 );
}


WORD do_nterm_type( VOID )
{
	/* Hier kann man den Typ einer Produktion festlegen. */
	REG	ULONG	i = 0, t_zeile = zeile;
	LONG		index, type_index;
	BYTE		typ[256];

	/* %nterm_type lîschen */
	grammar += 11;

	/* Spaces etc. lîschen */
	delete();

	/* <typ> */
	grammar++;

	/* Solange nicht > */
	while( *grammar != '>' )
		/* Zeichen uebertragen */
		typ[i++] = *grammar++;

	/* Mit Null abschliessen */
	typ[i] = 0;

	/* Befindet sich der Typ schon im Array ? */
	if( ( type_index = cmp_type( typ ) ) < 0 )
	{
		/* Nein, Array mit den Typen anpassen */
		if( ( type_arr = ( BYTE ** ) realloc( ( VOID ** ) type_arr, sizeof( BYTE * ) * ( type_arr_count + 1 ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Speicher fÅr das Array selbst anordern */
		if( ( type_arr[type_arr_count] = ( BYTE * ) malloc( i + 1 ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Den Typ in das Typen-Array kopieren */
		strcpy( type_arr[type_arr_count], typ );

		/* Den Index des Typs merken */
		type_index = type_arr_count++;
	}

	/* > ueberlesen */
	grammar++;

	/* Spaces etc. lîschen */
	delete();

	/* Nun folgen die Nichtterminale */
	while( zeile == t_zeile )
	{
		/* Nichtterminal extrahieren */
		if( extract_word() < 0 )
			/* Fehler */
			return( -1 );

		/* Nichterminal Hash-Tabelle eintragen */
		if( ( index = insert_hash_tab( grammar_word, NTERM ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* Den Typ uebertragen */
		hash_tab[index].type = type_index;

		/* Die Zeilennummer merken */
		hash_tab[index].zeile = zeile;

		/* Spaces etc. lîschen */
		delete();
	}

	/* Alles OK */
	return( 0 );
}


WORD do_init( VOID )
{
	/* Dient zur Initialisierung des Parsers. */
	ULONG	init_buff_size = 1000, i = 0, j = 0;

	/* %init ueberspringen */
	grammar += 6;

	/* Spaces etc. lîschen */
	delete();

	/* \{ lîschen */
	grammar += 2;

	/* Speicher fÅr Init anfordern */
	do
	{
		/* Speicher anfordern */
		if( ( init = ( BYTE * ) realloc( init, init_buff_size * sizeof( BYTE ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Falls noch mehr Speicher benîtigt wird */
		init_buff_size += 1000;

		/* Inhalt von Init uebertragen */
		for( ; i < ( i + 1000 ); i++, j++ )
		{
			/* Byte kopieren */
			init[j] = grammar[i];

			/* Ist init zu Ende ? */
			if( ( grammar[i - 1] == '\\' ) && ( grammar[i] == '}' ) )
			{
				/* Ja */
				init[j - 1] = 0;
				break;
			}

			/* Zeilenwechsel ? */
			else if( grammar[i] == 0x0a )
			{
				/* Eine weitere Zeile */
				zeile++;

				/* Nur \n eintragen */
				init[--j] = '\n';
			}
		}

	}while( init_buff_size % 1000 );

	/* grammar anpassen */
	grammar += ( i + 1 );

	/* Alles OK */
	return( 0 );
}


WORD do_incl( VOID )
{
	/* Beinhaltet Hilfsroutinen fÅr den Parsers. */
	ULONG	incl_buff_size = 1000, i = 0, j = 0;

	/* %incl ueberspringen */
	grammar += 6;

	/* Spaces etc. lîschen */
	delete();

	/* \{ lîschen */
	grammar += 2;

	/* Speicher fÅr Incl anfordern */
	do
	{
		/* Speicher anfordern */
		if( ( incl = ( BYTE * ) realloc( incl, incl_buff_size * sizeof( BYTE ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Falls noch mehr Speicher benîtigt wird */
		incl_buff_size += 1000;

		/* Inhalt von Incl uebertragen */
		for( ; i < ( i + 1000 ); i++, j++ )
		{
			/* Byte kopieren */
			incl[j] = grammar[i];

			/* Ist incl zu Ende ? */
			if( ( grammar[i - 1] == '\\' ) && ( grammar[i] == '}' ) )
			{
				/* Ja */
				incl[j - 1] = 0;
				break;
			}

			/* Zeilenwechsel ? */
			else if( grammar[i] == 0x0a )
			{
				/* Eine weitere Zeile */
				zeile++;

				/* Nur \n eintragen */
				incl[--j] = '\n';
			}
		}

	}while( incl_buff_size % 1000 );

	/* grammar anpassen */
	grammar += ( i + 1 );

	/* Alles OK */
	return( 0 );
}


WORD compress_lr_tab( VOID )
{
	/* Hier werden doppelte ZustÑnde aus der LR-Tabelle entfernt. */
	REG LONG	i, j, x;
	LONG		*dbl_array, dbl_count = 0;

	/* Meldung ausgeben */
	fprintf( stderr, "\nKomprimiere LR-Tabelle..." );

	/* TemporÑren Speicher fÅr die doppelten 
	n-EintrÑge anfordern */
	if( ( dbl_array = ( LONG * ) malloc( sizeof( LONG ) * ( hulle_count + 1 ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr die HÅllen-EintrÑge anfordern */
	if( ( compressed_lr_index_tab = ( LONG * ) malloc( sizeof( LONG ) * ( hulle_count + 1 ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	for( i = 0; i <= hulle_count; i++ )
		compressed_lr_index_tab[i] = i;

	/* Alle ZustÑnde */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Gibt es die HÅlle noch ? */
		if( !lr_tab[i] )
			/* Nein, sie wurde schon gelîscht */
			continue;

		/* Nicht mit sich selbst vergleichen */
		for( j = i + 1; j <= hulle_count; j++ )
		{
			/* Gibt es die HÅlle noch ? */
			if( !lr_tab[j] )
				/* Nein, sie wurde schon gelîscht */
				continue;

			/* Die Werte in der LR-Tabelle vergleichen */
			if( cmp_lr_entry( i, j ) )
			{
				/* Es gibt zu lîschende HÅllen */
				main_dbl_count++;

				/* Werte sind gleich, j merken */
				compressed_lr_index_tab[j] = i;
				dbl_array[dbl_count++] = j;

				/* Den Speicher freigeben */
				free( ( VOID * ) lr_tab[j] );

				/* Zu lîschende HÅllen entsprechend markieren */
				lr_tab[j] = 0L;

				/* Ist es die erste Meldung ? */
				if( main_dbl_count == 1 )
					/* Ja */
					fprintf( file, "\n\n\nKomprimiere LR-Tabelle.\n" );

				/* Gleiche ZustÑnde ausgeben */
				fprintf( file, "\nZustÑnde %lu und %lu doppelt.", i, j );
			}
		}
	}

	/* Waren HÅllen mehrfach vorhanden ? */
	if( main_dbl_count )
	{
		/* Bubble-Sort -> dbl_array sortieren */
		for( i = 1; i < dbl_count; i++ )
		{
			for( j = dbl_count - 1; j >= i; j-- )
			{
				/* Muessen die Werte vertauscht werden ? */
				if( dbl_array[j - 1] > dbl_array[j] )
				{
					x = dbl_array[j - 1];
					dbl_array[j - 1] = dbl_array[j];
					dbl_array[j] = x;
				}
			}
		}

		/* Die Indexe in der Tabelle anpassen */
		for( i = 0; i <= hulle_count; i++ )
		{
			for( j = dbl_count - 1; j >= 0; j-- )
			{
				/* Muss der Wert angepasst werden ? */
				if( compressed_lr_index_tab[i] >= dbl_array[j] )
				{
					/* Ja */
					compressed_lr_index_tab[i] -= ( j + 1 );
					break;
				}
			}
		}

		/* HÅllen entsprechend entfernen */
		for( i = 0, j = 0; i <= hulle_count; )
		{
			/* Ist der Eintrag gelîscht worden ? */
			if( !lr_tab[i] )
				/* Gelîschte ZustÑnde ignorieren */
				i++;

			else
				/* Uebernehmen */
				lr_tab[j++] = lr_tab[i++];
		}

		/* Anzahl der HÅllen anpassen */
		hulle_count -= main_dbl_count;
	}

	/* Speicher wieder freigeben */
	free( ( VOID * ) dbl_array );

	/* Alles OK */
	return( 0 );
}


WORD cmp_lr_entry( LONG sour_lr_tab, LONG dest_lr_tab )
{
	/* Hier werden zwei LR-ZustÑnde verglichen. */
	REG	LONG	i;

	/* Alle Terminale */
	for( i = 0; i <= term_count; i++ )
	{
		/* Sind die EintrÑge gleich ? */
		if( ( lr_tab[sour_lr_tab][i].state != lr_tab[dest_lr_tab][i].state ) || ( lr_tab[sour_lr_tab][i].value != lr_tab[dest_lr_tab][i].value ) )
			/* Nein */
			return( 0 );
	}

	/* EintrÑge sind gleich */
	return( 1 );
}


LONG cmp_type( BYTE *type )
{
	/* Hier wird geprueft, ob der Typ schon im Type-Array ist. */
	REG LONG	i;

	/* Alle EintrÑge prÅfen */
	for( i = 0; i < type_arr_count; i++ )
	{
		/* Ist es der gesuchte Eintrag ? */
		if( !strcmp( type, type_arr[i] ) )
			/* Ja */
			return( i );
	}

	/* Der Typ befindet sich nicht im Array */
	return( -1 );
}