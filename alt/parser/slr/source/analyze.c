/*
	Projekt		:	SLR-Parser-Generator
	Modul		:	ANALYZE.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/


/* Includes */
#include <portab.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "slr.h"
#include <tos.h>


/* Defines */

/* Maximale Anzahl der zu tollerrierenden Kollisionen in der HASH-Funktion */
#define	MAX_COL					5

/* FÅr die Info-datei */
#define INF_FOLLOW			0x01
#define INF_FIRST			0x02
#define INF_G_HASH_TAB_SIZE 0x04
#define INF_DEBUG			0x08
#define INF_DEST			0x10
#define INF_ANALYZE			0x20
#define INF_ITEMS			0x40
#define INF_SLR_TAB_1		0x80
#define INF_SLR_TAB_2		0x100
#define INF_MAX_CHAR		0x200


/* Definitionen */

/* Laufende Nummer der aktuellen Produktion */
ULONG			prod_list_nr = 0;



/*	Wurzel des Baums, der die Nummern der Nichtterminale beinhaltet.
	Der Baum dient dazu, um die Grammatik auf Fehler zu ÅberprÅfen, wie
	z.B. definierte Nichtterminale, die nicht auf der rechten Seite einer
	Grammatik auftauchen.
*/
BIN_TREE		*root_left = 0L;


/*	Wurzel des Baums, der die Nummern der Nichtterminale beinhaltet.
	Der Baum dient dazu, um die Grammatik auf Fehler zu ÅberprÅfen, wie
	z.B. genannte Nichtterminale, die nicht definiert wurden.
*/
BIN_TREE		*root_right = 0L;


/*
	Wurzel des Baums, in dem alle Terminale stehen, die ja fÅr die Erzeugung
	der HÅllen( GOTO ) notwendig sind.
*/
BIN_TREE		*root_term = 0L;


/* HASH-Index von 'EPSILON' */
ULONG			epsilon_hash_ind;


/* HASH-Index von 'ACCEPT' */
ULONG			accept_hash_ind;


/* FÅr die Namen der Produktionen */
FIRST			nterm = { 0 };


/* LÑngstes Nichtterminal */
LONG			nterm_length = 0;


/* Nummer der Fehlermeldung */
ULONG			err_number = 1;


/* Anzahl der Produktionen */
ULONG			anz_prod = -1;


/* Globale Definitionen */

MGLOBAL BYTE			*grammar_file;
MGLOBAL BYTE			non_term[];
MGLOBAL ULONG			anz_zeile;
MGLOBAL LONG			prod_info_size;
MGLOBAL PROD_HASH		*prod_hash;
MGLOBAL PROD_INFO		*prod_info;
MGLOBAL FILE			*file;
MGLOBAL SLR_INFO		slr_info;



/* Prototypen */
BYTE *ignore_space( BYTE *string );
BYTE *prod_2( BYTE *string );
BYTE *isword( BYTE *string );
WORD add_blat( LONG prod_nr, BIN_TREE **root );
LONG hash_pjw( BYTE *string );
LONG insert_hash_word( BYTE *non_term, WORD kind );
WORD find_left_tree( BIN_TREE *blat, LONG prod_nr );
WORD test_non_term( BIN_TREE *blat );
WORD test_alt( VOID );
BYTE *analyze_assoc( BYTE *grammar_file );
VOID free_bintree( BIN_TREE *tree );
WORD insert_nterm( LONG nterm );



WORD analyze_grammar( VOID )
{
	/*	In dieser Routine wird die Grammatik zerlegt und ÅberprÅft.
		Zerlegen:
			Das Nichtterminal auf der linken Seite bildet die Nummer des
			Nichtterminals, mit dieser die Infos in die Struktur
			PROD_INFO eingetragen werden.
			Des weiteren werden die Nichtterminale und Terminale auf der
			rechten Seite in diese Struktur eingetragen.
			Auûerdem werden Produktionen, die mit einem | versehen sind
			getrennt.			

		öberprÅfen:
			Alle Nichtterminale, die auf der linken Seite stehen, werden
			in der Struktur BIN_TREE vermerkt, damit man im Nachhinein
			feststellen kann, ob die Produktion Åberhaupt von einer anderen
			aufgerufen wurde.
			Des weiteren werden alle Nichtterminale auf der rechten Seite
			einer Produktion in die gleiche Struktur eingetragen, damit
			man feststellen kann, ob die Produktion, dessen Nichterminal
			aufgerufen wird, Åberhaupt definiert wurde.
	*/
	REG UWORD	count;
	REG LONG	i, j;
	LONG		index, index_nonterm;
	PROD_INFO	*temp_prod_info;
	ULONG		priori = 0, assoc = 0;

	/* War ein Terminal -> In HASH-Tab eintragen */
	if( ( epsilon_hash_ind = insert_hash_word( "EPSILON", TERM ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* War ein Terminal -> In HASH-Tab eintragen */
	if( ( accept_hash_ind = insert_hash_word( "ACCEPT", TERM ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Die PrioritÑt und AssoziativitÑt gegebenenfalls analysieren */

	/* Ist es CR, LF ? */
	grammar_file = ignore_space( grammar_file );

	/* %left bzw. %right ? */
	if( ( grammar_file[0] == '%' ) && ( grammar_file[1] != 'e' ) && ( grammar_file[1] != 'E' ) )
	{
		/* PrioritÑt und AssoziativitÑt auswerten */
		if( ( grammar_file = analyze_assoc( grammar_file ) ) == ( BYTE * ) -1 )
			/* Fehler */
			return( -1 );

		/* '#' lîschen */
		grammar_file++;
	}

	/* Ist die Grammatik zu Ende ? */
	while( *grammar_file )
	{
		/* Ist es CR, LF ? */
		grammar_file = ignore_space( grammar_file );

		/* Steht ein Nichtterminal bevor ? */
		if( ( grammar_file = isword( grammar_file ) ) != ( BYTE * ) -1L )
		{
			/* Anzahl der Produktionen merken */
			anz_prod++;

			/* LÑnge des Nichtterminals holen */
			index = ( LONG ) strlen( non_term );

			/* Ist das Nichtterminal lÑnger ? */
			if( index > nterm_length )
				/* Ja */
				nterm_length = index;

			/* Ist ein Nichtterminal -> in HASH-Tab eintragen */
			if( ( index = insert_hash_word( non_term, NON_TERM ) ) < 0 )
				/* Fehler */
				return( -1 );

			/* In linken Baum eintragen */
			if( !add_blat( index, &root_left ) )
			{
				/*
					Nummer des Nichtterminals zusÑtzlich in ein Array ein-
					tragen.( Wird fÅr die FOLLOW-Mengen benîtigt. )
				*/
				if( insert_nterm( index ) < 0 )
					/* Fehler */
					return( -1 );

				/* Trenner analysieren */
				if( ( grammar_file = prod_2( grammar_file ) ) != ( BYTE * ) -1L )
				{
					/* Meldung ausgeben ? */
					if( slr_info.analyze == ON )
						/* Rechte Seite der Produktion analysieren */
						fprintf( file, "\nAnalysiere Produktion %s...", non_term );

					/* Nummer des Nichtterminals in PROD_INFO eintragen */
					prod_info[prod_list_nr].prod_nr = index;

					/* Leerzeichen lîschen */
					grammar_file = ignore_space( grammar_file );

					do
					{
						/* Muû PROD_INFO vergrîûert werden ? */
						if( prod_list_nr > ( prod_info_size - 10 ) )
						{
							/* PROD_INFO-Strukt muû vergrîûert werden */
							if( !( temp_prod_info = ( PROD_INFO * ) calloc( sizeof( PROD_INFO ), prod_info_size + DEFAULT_PROD_INFO_LENGTH ) ) )
								/* Fehler */
								return( error( NOT_ENOUGH_MEMORY, 0 ) );

							/* Speicherbereiche umkopieren */
							memcpy( ( VOID * ) temp_prod_info, ( VOID * ) prod_info, prod_info_size * sizeof( PROD_INFO ) );

							/*	Das PROD_INFO-Array ist um DEFAULT_PROD_INFO_LENGTH
								Elemente gewachsen.
							*/
							prod_info_size += DEFAULT_PROD_INFO_LENGTH;

							/* Alten Speicherbereich freigeben */
							free( ( VOID * ) prod_info );

							/* Zeiger anpassen */
							prod_info = temp_prod_info;
						}

						if( *grammar_file == '|' )
						{
							/* | Åberspringen */
							grammar_file++;

							/* Leerzeichen lîschen */
							grammar_file = ignore_space( grammar_file );

							/* PrioritÑt der analysierten Produktion merken */
							prod_info[prod_list_nr].priori = priori;

							/* AssoziativitÑt der analysierten Produktion merken */
							prod_info[prod_list_nr].assoc = assoc;

							/* PrioritÑt/AssoziativitÑt ist vermerkt */
							priori = 0;

							/* Vermerken */
							prod_info[prod_list_nr++].prod_fl++;

							/* Nummer des Nichtterminals in PROD_INFO eintragen */
							prod_info[prod_list_nr].prod_nr = index;
						}
						else if( ( grammar_file[-2] == 0x0a ) || ( ( grammar_file[-2] == 0x0d ) && ( grammar_file[-1] == '\n' ) ) )
						{
							/* PrioritÑt der analysierten Produktion merken */
							prod_info[prod_list_nr].priori = priori;

							/* AssoziativitÑt der analysierten Produktion merken */
							prod_info[prod_list_nr].assoc = assoc;

							/* PrioritÑt/AssoziativitÑt ist vermerkt */
							priori = 0;

							/* Neue Produktion */
							prod_list_nr++;
							break;
						}

						else if( ( *grammar_file == '\'' ) || ( *grammar_file == '\"' ) )
						{
							/* Terminal erkannt */
							count = 0;

							/* ' bzw. " Åberspringen */
							grammar_file++;

							/* String in non_term eintragen */
							while( ( *grammar_file != '\'' ) && ( *grammar_file != '\"' ) )
								non_term[count++] = *grammar_file++;

							/* Null-Byte anhÑngen */
							non_term[count] = 0;

							/* ' bzw. " Åberspringen */
							grammar_file++;

							/* LÑnge des Terminals holen */
							index_nonterm = ( LONG ) strlen( non_term );

							/* Ist das Terminal lÑnger ? */
							if( index_nonterm > nterm_length )
								/* Ja */
								nterm_length = index_nonterm;

							/* War ein Terminal -> In HASH-Tab eintragen */
							if( ( index_nonterm = insert_hash_word( non_term, TERM ) ) < 0 )
								/* Fehler */
								return( -1 );

							/* Ist es Epsilon ? */
							if( index_nonterm != epsilon_hash_ind )
							{
								/* Nein, es ist nicht EPSILON */

								/* PrioritÑt des Terminals merken */
								priori = prod_hash[index_nonterm].priori;

								/*
									Ist die AssoziativitÑt des Terminals
									angegeben worden ?
								*/
								if( !prod_hash[index_nonterm].assoc )
									/* Default ist Rechtsassoziativ */
									prod_hash[index_nonterm].assoc = RIGHT;

								else
									/* AssoziativitÑt des Terminals merken */
									assoc = prod_hash[index_nonterm].assoc;

								/* Flag zur öberprÅfung der Terminale */
								prod_hash[index_nonterm].defined++;

								/* In Baum eintragen zwecks öberprÅfung */
								if( add_blat( index_nonterm, &root_term ) < 0 )
									/* Fehler */
									return( -1 );
							}

							/* Terminal in PROD_INFO eintragen */
							prod_info[prod_list_nr].prod_ri[prod_info[prod_list_nr].prod_ri_anz] = index_nonterm;

							/* Ein weiteres Terminal auf der rechten Seite */
							prod_info[prod_list_nr].prod_ri_anz++;

							/* Leerzeichen lîschen */
							grammar_file = ignore_space( grammar_file );
						}

						else
						{
							/* Ist ein Nichtterminal */
							if( ( grammar_file = isword( grammar_file ) ) == ( BYTE * ) -1L )
								/* Fehler */
								return( -1 );

							/* War ein Nichtterminal -> In HASH-Tab eintragen */
							if( ( index_nonterm = insert_hash_word( non_term, NON_TERM ) ) < 0 )
								/* Fehler */
								return( -1 );

							/* In Baum eintragen zwecks ÅberprÅfung */
							if( add_blat( index_nonterm, &root_right ) < 0 )
								/* Fehler */
								return( -1 );

							/* Nichtterminal in PROD_INFO eintragen */
							prod_info[prod_list_nr].prod_ri[prod_info[prod_list_nr].prod_ri_anz] = index_nonterm;

							/* Ein weiteres Nichtterminal auf der rechten Seite */
							prod_info[prod_list_nr].prod_ri_anz++;

							/* Leerzeichen lîschen */
							grammar_file = ignore_space( grammar_file );

							/*	Ist die PrioritÑt der Produktion durch
								%prec angegeben ?
							*/
							if( !strncmp( grammar_file, "%prec", 5 ) )
							{
								/* PrioritÑt eintragen */
								grammar_file += 5;

								/* Leerzeichen lîschen */
								grammar_file = ignore_space( grammar_file );

								/* Terminal erkannt */
								if( ( grammar_file = isword( grammar_file ) ) == ( BYTE * ) -1 )
									/* Fehler */
									return( -1 );

								/* War ein Terminal -> In HASH-Tab eintragen */
								if( ( index_nonterm = insert_hash_word( non_term, TERM ) ) < 0 )
									/* Fehler */
									return( -1 );

								/* PrioritÑt des Terminals eintragen */
								prod_info[prod_list_nr].priori = prod_hash[index_nonterm].priori;

								/* AssoziativitÑt des Terminals eintragen */
								prod_info[prod_list_nr].assoc = prod_hash[index_nonterm].assoc;

								/* PrioritÑt ist vermerkt */
								priori = 0;

								/* Flag zur öberprÅfung der Terminale */
								prod_hash[index_nonterm].defined++;
							}
						}

					}while( *grammar_file );
				}

				else
					/* Fehler */
					return( -1 );
			}

			else
				/* Fehler */
				return( -1 );
		}

		else
			/* Das Nichtterminal beinhaltet nicht erlaubte Zeichen */
			return( -1 );
	}

	/* PrioritÑt der letzten Produktion merken */
	prod_info[prod_list_nr].priori = priori;

	/* AssoziativitÑt muû noch eingetragen werden */
	prod_info[prod_list_nr].assoc = assoc;

	/* Laufende Nummern der Nichtterminale in Hash-Tab eintragen */
	for( i = 0, j = 0; i <= prod_list_nr; i++, j = i )
	{
		/* Hat die Produktion Alternativen ? */
		if( !prod_info[i].prod_fl )
			/* Laufende Nummer eintragen */
			prod_hash[prod_info[i].prod_nr].prod_list_nr = j;

		else
		{
			/* Alle Alternativen bekommen die selbe laufende Nummer */
			while( prod_info[i].prod_fl )
			{
				prod_hash[prod_info[i].prod_nr].prod_list_nr = j;
				i++;
			}

			/*
				Auch die letzte Alternative bekommt die gleiche laufende
				Nummer
			*/
			prod_hash[prod_info[i].prod_nr].prod_list_nr = j;
		}
	}

	/* Hat die Grammatik eine Startproduktion ? */
	if( ( prod_info[0].prod_ri_anz != 1 ) || ( prod_hash[prod_info[0].prod_ri[0]].prod_type != NON_TERM ) )
		/* Grammatik hat keine Startproduktion */
		return( error( NO_START_PROD, 0 ) );

	/* Nichtterminale ÅberprÅfen */
	if( test_non_term( root_right ) < 0 )
		/* Fehler */
		return( -1 );

	/* Doppelte Alternativen melden */
	if( test_alt() < 0 )
		/* Fehler */
		return( -1 );

	/* Erstmal alle Nummern der Terminale sammeln. */
	if( get_all_term_nr( root_term ) < 0 )
		/* Fehler */
		return( -1 );

	/* Speicher der binÑren BÑume freigeben */
	free_bintree( root_right );
	free_bintree( root_left );
	free_bintree( root_term );

	/* Meldung ausgeben ? */
	if( slr_info.analyze == ON )
		/* Meldung ausgeben */
		fprintf( file, "\n\n%lu Produktionen wurden analysiert...", prod_list_nr );

	/* Alles OK */
	return( 0 );
}


BYTE *analyze_assoc( BYTE *grammar_file )
{
	/*	Hier wird die PrioritÑt und die AssoziativitÑt ausgewertet.
		%left und %right
	*/
	REG ULONG	priori = 1;
	ULONG		index_nonterm;

	/* Suchen, bis ein # erscheint */
	while( *grammar_file != '#' )
	{
		/* Ist es %left oder %right ? */
		if( grammar_file[1] == 'l' )
		{
			/* Es ist %left */

			/* %left Åberspringen */
			grammar_file += 5;

			/* öberpringen */
			grammar_file = ignore_space( grammar_file );

			/* Alle Terminale bearbeiten */
			while( ( *grammar_file != '%' ) && ( *grammar_file != '#' ) )
			{
				/* Terminal erkannt */
				if( ( grammar_file = isword( grammar_file ) ) == ( BYTE * ) -1 )
					/* Fehler */
					return( ( BYTE * ) -1 );

				/* War ein Terminal -> In HASH-Tab eintragen */
				if( ( index_nonterm = insert_hash_word( non_term, TERM ) ) < 0 )
					/* Fehler */
					return( ( BYTE * ) -1 );

				/* In Baum eintragen zwecks ÅberprÅfung */
				if( add_blat( index_nonterm, &root_term ) < 0 )
					/* Fehler */
					return( ( BYTE * ) -1 );

				/* PrioritÑt in HASH-Tabelle eintragen */
				prod_hash[index_nonterm].priori = priori;

				/* AssoziativitÑt in HASH-Tabelle eintragen */
				prod_hash[index_nonterm].assoc = LEFT;

				/* öberpringen */
				grammar_file = ignore_space( grammar_file );
			}

			/* PrioritÑt erhîhen */
			priori++;
		}

		/* Ist es %right ? */
		else if( grammar_file[1] == 'r' )
		{
			/* Es ist %right */

			/* %right Åberspringen */
			grammar_file += 6;

			/* öberpringen */
			grammar_file = ignore_space( grammar_file );

			/* Alle Terminale bearbeiten */
			while( ( *grammar_file != '%' ) && ( *grammar_file != '#' ) )
			{
				/* Terminal erkannt */
				if( ( grammar_file = isword( grammar_file ) ) == ( BYTE * ) -1 )
					/* Fehler */
					return( ( BYTE * ) -1 );

				/* War ein Terminal -> In HASH-Tab eintragen */
				if( ( index_nonterm = insert_hash_word( non_term, TERM ) ) < 0 )
					/* Fehler */
					return( ( BYTE * ) -1 );

				/* In Baum eintragen zwecks ÅberprÅfung */
				if( add_blat( index_nonterm, &root_term ) < 0 )
					/* Fehler */
					return( ( BYTE * ) -1 );

				/* PrioritÑt in HASH-Tabelle eintragen */
				prod_hash[index_nonterm].priori = priori;

				/* AssoziativitÑt in HASH-Tabelle eintragen */
				prod_hash[index_nonterm].assoc = RIGHT;

				/* öberpringen */
				grammar_file = ignore_space( grammar_file );
			}

			/* PrioritÑt erhîhen */
			priori++;
		}

		else
			/* Fehler */
			return( ( BYTE * ) error( ASSOC_ERR, 0 ) );
	}

	/* Alles OK */
	return( grammar_file );
}


LONG insert_hash_word( BYTE *non_term, WORD kind )
{
	/*
		Hier wird ein Terminal/Nichtterminal in die Hash-Tab eingetragen.
		non_term	= Zeiger auf Nichtterminal/Terminal-String
		kind		= TERM = Terminal, NON_TERM = Nichtterminal
	*/
	LONG	index, hash_col_count;
	WORD	insert_hash_tab_count;
	BYTE	*prod_name1, *prod_name2;

	/* Die Position in der HASH-Tab bestimmen */
	index = hash_pjw( non_term );

	/* Ist diese Stelle bereits belegt ? */
	if( ( prod_hash[index].prod_name ) && ( strcmp( non_term, prod_hash[index].prod_name ) ) )
	{
		/* Ja -> Es ist eine Kollision aufgetreten */
		hash_col_count = 2;

		/* Anzahl der Kollisionen zÑhlen */
		insert_hash_tab_count = 0;

		do
		{
			/* Zeiger auf die Namen der Nichtterminale/Terminale holen */
			prod_name1 = prod_hash[index + hash_col_count].prod_name;
			prod_name2 = prod_hash[index - hash_col_count].prod_name;

			/* Sind die Strings gleich ? */
			if( ( prod_name1 ) && !( strcmp( non_term, prod_name1 ) ) )
				/* Nichtterminal/Terminal ist schon eingetragen */
				return( index + hash_col_count );

			/* Sind die Strings gleich ? */
			else if( ( prod_name2 ) && !( strcmp( non_term, prod_name2 ) ) )
				/* Nichtterminal/Terminal ist schon eingetragen */
				return( index - hash_col_count );

			/* Ist der Eintrag frei ? */
			if( !prod_name1 )
			{
				/* Eintrag ist frei -> Nichtterminal eintragen */
				index += hash_col_count;
				break;
			}

			else if( !prod_name2 )
			{
				/* Eintrag ist frei -> Nichtterminal eintragen */
				index -= hash_col_count;
				break;
			}

			/* Eintrag ist nicht frei */
			hash_col_count *= 2;

			/* öberlauf ? */
			if( hash_col_count > slr_info.gr_hash_tab_size )
				/* Wert ist zu groû */
				hash_col_count %= slr_info.gr_hash_tab_size;

			else if( ( index - hash_col_count ) < 0 )
			{
				/* Unterlauf */
				insert_hash_tab_count = MAX_COL + 1;
				break;
			}

			/* Anzahl der DurchlÑufe zÑhlen */
			insert_hash_tab_count++;

		}while( 1 );

		/* War die Anzahl der Kollisionen etwas zu Groû ? */
		if( insert_hash_tab_count > MAX_COL )
			/* HASH-Tabelle zu klein */
			return( error( NOT_ENOUGH_GR_HASH_MEMORY, 0 ) );
	}

	/* Speicher fÅr den String anfordern */
	if( !( prod_hash[index].prod_name = ( BYTE * ) malloc( strlen( non_term ) + 1 ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Nichtterminal/Terminal eintragen */
	strcpy( prod_hash[index].prod_name, non_term );

	/* Ist es ein Nichtterminal oder ein Terminal ? */
	if( kind == NON_TERM )
		/* Ist ein Nichtterminal */
		prod_hash[index].prod_type = NON_TERM;

	else
		/* Ist ein Terminal */
		prod_hash[index].prod_type = TERM;

	/* Den Index zurÅckliefern */
	return( index );
}


LONG hash_pjw( BYTE *string )
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

	return( ( LONG ) ( ( h % slr_info.gr_hash_tab_size ) + 1 ) );
}


BYTE *isword( BYTE *string )
{
	/*	Diese Routine prÅft, ob es sich bei String um ein gÅltiges Nicht-
		terminal handelt.
	*/
	BYTE	*temp_str = string;

	while( !isspace( *string ) && ( *string != '\'' ) && ( *string ) && ( *string != ':' ) )
	{
		/* Ist der Buchstabe erlaubt ? */
		switch( ( WORD ) *string )
		{
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
			case '@':
			case '$':
			{
				/* Erlaubtes Zeichen */
				string++;
				break;
			}

			default:
				/* Ein nicht erlaubtes Zeichen wurde gefunden */
				return( ( BYTE * ) error( INVALID_CHAR, 0 ) );
		}
	}

	/* Erkanntes Nichtterminal merken */
	strncpy( non_term, temp_str, ( string - temp_str ) );

	/* Null-Byte anhÑngen */
	non_term[string - temp_str] = 0;

	/* Ist ein korrkter String */
	return( string );
}


BYTE *prod_2( BYTE *string )
{
	/*	In dieser Routine wird ÅberprÅft, ob die linke Seiter einer
		Produktion ordnungsgemÑû durch ':', ':=', '::=', `=` oder '->' von
		der rechten Seite getrennt ist.
	*/
	REG WORD	del = 0;

	/* Leerzeichen lîschen */
	string = ignore_space( string );

	switch( string[0] )
	{
		/* ':' */
		case ':':
		{
			/* Nichts ? */
			if( isspace( string[1] ) )
				del++;

			/* '::=' ? */
			else if( string[1] == ':' )
			{
				if( string[2] == '=' )
					del = 3;

				else
					/* Fehler */
					return( ( BYTE * ) error( FALSE_PROD_SEPERATOR, 0 ) );
			}

			else if( string[1] == '=' )
				del = 2;

			else
				/* Fehler */
				return( ( BYTE * ) error( FALSE_PROD_SEPERATOR, 0 ) );

			/* Alles klar */
			break;
		}

		case '=':
		{
			/* Kommt dahinter noch etwas ? */
			if( !isspace( string[1] ) )
				/* Ja -> Fehler */
				return( ( BYTE * ) error( FALSE_PROD_SEPERATOR, 0 ) );

			/* Alles OK */
			break;
		}

		case '-':
		{
			/* '->' */
			if( string[1] != '>' )
				/* Fehler */
				return( ( BYTE * ) error( FALSE_PROD_SEPERATOR, 0 ) );

			/* -> Åberspringen */
			del += 2;

			/* Alles OK */
			break;
		}

		default:
			/* Fehler */
			return( ( BYTE * ) error( FALSE_PROD_SEPERATOR, 0 ) );
	}

	/* Alles OK */
	return( string + del );
}


WORD add_blat( LONG prod_nr, BIN_TREE **root )
{
	/*	Hier wird ein neues Blatt erzeugt, und am Baum angehÑngt.
		prod_nr = Nummer des Nichtterminals
	*/
	BIN_TREE	*blat, *temp_root;

	/* Ist genug Speicher fÅr ein weiteres Blatt frei ? */
	if( ( blat = ( BIN_TREE * ) calloc( sizeof( BIN_TREE ), 1 ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Produktionsnummer im Baum eintragen */
	blat->prod_nr = prod_nr;

	/* Zu wenig Speicher frei ? */
	if( !blat )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Stelle im Baum finden, an der das Blatt angehÑngt werden muû */
	if( !( *root ) )
		/* Der Baum ist leer */
		*root = blat;

	else
	{
		/* Wurzel holen */
		temp_root = *root;

		while( 1 )
		{
			/* Grîûer prod_nr ? */
			if( prod_nr > temp_root->prod_nr )
			{
				/* Ist es das letzte Blatt ? */
				if( !temp_root->right )
				{
					/* Neus Blatt anhÑngen */
					temp_root->right = blat;

					/* Raus aus der while-Schleife */
					break;
				}

				else
					/* Rechten Zeiger holen */
					temp_root = temp_root->right;
			}

			/* Kleiner prod_nr ? */
			else if( prod_nr < temp_root->prod_nr )
			{
				/* Ist es das letzte Blatt ? */
				if( !temp_root->left )
				{
					/* Neus Blatt anhÑngen */
					temp_root->left = blat;

					/* Raus aus der while-Schleife */
					break;
				}

				else
					/* Rechten Zeiger holen */
					temp_root = temp_root->left;
			}

			else
			{
				/*	Soll die Nummer des Nichtterminals im Baum fÅr die
					linke Seite einer Produktion eingetragen werden ?
				*/
				if( *root == root_left )
					/* Fehler -> Nichtterminal redefiniert */
					return( error( REDEFINED_NONTERM, 0 ) );

				/* Die Produktion ist schon in dem Baum */
				free( ( VOID * ) blat );

				/* Raus aus der while-Schleife */
				break;
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD test_non_term( BIN_TREE *blat )
{
	/*
		Hier wird ÅberprÅft, ob die Nichtterminale, die auf den rechten
		Seiten der Produktionen angegeben wurden, auch definiert wurden.
	*/

	/* Ist noch etwas da ? */
	if( blat )
	{
		if( !( find_left_tree( root_left, blat->prod_nr ) ) )
		{
			/* Mit dem rechten Blatt weitermachen */
			if( blat->right )
				test_non_term( blat->right );

			/* Mit dem linken Blatt weitermachen */
			if( blat->left )
				test_non_term( blat->left );
		}

		else
		{
			/* Nichtterminal wurde nicht definiert */
			strcpy( non_term, prod_hash[blat->prod_nr].prod_name );

			/* Fehlermeldung ausgeben */
			error( NOT_DEFINED_NONTERM, 0 );

			/* Mit dem rechten Blatt weitermachen */
			if( blat->right )
				test_non_term( blat->right );

			/* Mit dem linken Blatt weitermachen */
			if( blat->left )
				test_non_term( blat->left );
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD test_alt( VOID )
{
	/*
		In dieser Routine wird ÅberprÅft, ob Produktionen doppelte
		Alternativen haben.
	*/
	ULONG	prod_count, temp_prod_count, count1, count2, count3;
	WORD	err_flg = 0;

	/* Alle Produktionen bearbeiten */
	for( prod_count = 0; prod_count <= prod_list_nr; prod_count++ )
	{
		/* Ist eine doppelte Alternative erkannt worden ? */
		if( err_flg )
		{
			/* Ja, mit der nÑchsten Produktion weitermachen */
			prod_count += ( count1-- );

			/* Das Fehlerflag zurÅcksetzen */
			err_flg--;
		}

		/* Feststellen, wieviele Alternativen die Produktion hat */
		temp_prod_count = prod_count;
		count1 = 1;

		while( prod_info[temp_prod_count++].prod_fl )
			count1++;

		/* Hat die Produktion min. zwei Alternativen ? */
		if( count1 > 1 )
		{
			/* Alle Alternativen ÅberprÅfen */
			for( count2 = prod_count; count2 < prod_count + count1; count2++ )
			{
				/* Ist eine doppelte Alternative erkannt worden ? */
				if( err_flg )
					/* Ja, mit der nÑchsten Produktion weitermachen */
					break;

				for( count3 = prod_count; count3 < count2; count3++ )
				{
					/* Alternativen vergleichen */
					if( !memcmp( ( VOID * ) &prod_info[count3], ( VOID * ) &prod_info[count2], sizeof( PROD_INFO ) - 2 ) )
					{
						/* Alternativen sind doppelt */
						strcpy( non_term, prod_hash[prod_info[prod_count].prod_nr].prod_name );

						/* Fehlermeldung ausgeben */
						error( DOUBLE_ALT, 0 );

						/* NÑchste Produktion untersuchen */
						err_flg++;
					}
				}

				for( count3++; count3 < prod_count + count1; count3++ )
				{
					/* Alternativen vergleichen */
					if( !memcmp( ( VOID * ) &prod_info[count3], ( VOID * ) &prod_info[count2], sizeof( PROD_INFO ) - 2 ) )
					{
						/* Alternativen sind doppelt */
						strcpy( non_term, prod_hash[prod_info[prod_count].prod_nr].prod_name );

						/* Fehlermeldung ausgeben */
						error( DOUBLE_ALT, 0 );

						/* NÑchste Produktion untersuchen */
						err_flg++;
					}
				}
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD find_left_tree( BIN_TREE *blat, LONG prod_nr )
{
	/* Ist es die gesuchte Produktion ? */
	if( blat )
	{
		/* Ist es die gesuchte Produktion ? */
		if( blat->prod_nr == prod_nr )
			/* Produktion wurde gefunden */
			return( 0 );

		/* NÑchste Produktion untersuchen */
		else if( prod_nr > blat->prod_nr )
			/* Mit der rechten Seite weitermachen */
			return( find_left_tree( blat->right, prod_nr ) );

		else if( prod_nr < blat->prod_nr )
			/* Mit der linken Seite weitermachen */
			return( find_left_tree( blat->left, prod_nr ) );
	}

	/* Produktion wurde nicht definiert */
	return( -1 );
}


BYTE *ignore_space( BYTE *string )
{
	/* In dieser Routine werden Spaces und 13, 10 Åbersprungen */

	while( *string )
	{
		/* Welches Zeichen ist es denn ? */
		switch( *string )
		{
			/* Space */
			case ' ':
			case '\t':
			{
				/* öberspringen */
				string++;
				break;
			}

			case 0x0d:
			{
				/* öberspringen */
				string += 2;

				/* ZeilenzÑhler erhîhen */
				anz_zeile++;
				break;
			}

			case 0x0a:
			{
				/* öberspringen */
				string++;

				/* ZeilenzÑhler erhîhen */
				anz_zeile++;
				break;
			}

			case ';':
			{
				/* Kommentar */
				while( *string != 0x0a )
					string++;

				/* ZeilenzÑhler erhîhen */
				anz_zeile++;

				/* 0x0a Åberspringen */
				string++;
				break;
			}

			case '/':
			{
				/* C-Kommentar ? */
				if( string[1] == '*' )
				{
					/* Ist ein C-Kommentar */
					while( ( string[0] != '*' ) || ( string[1] != '/' ) )
					{
						/* LineFeed ? */
						if( *string == 0x0a )
							/* ZeilenzÑhler erhîhen */
							anz_zeile++;

						else if( *string == 0x0d )
						{
							/* ZeilenzÑhler erhîhen */
							anz_zeile++;

							/* Zeichen lîschen */
							string++;
						}

						/* Zeichen lîschen */
						string++;
					}
				}

				/* Kommentarende Åberspringen */
				string += 2;

				/* Ist kein C-Kommentar */
				break;
			}

			default:
				/* Ist kein Space etc. */
				return( string );
		}
	}
}

VOID free_bintree( BIN_TREE *tree )
{
	/*
		In dieser Routine wird der Speicher, der fÅr den binÑren Baum
		gebraucht wurde, wieder freigegeben.
	*/

	/* Hat das Blatt einen linken Nachfolger ? */
	if( tree->left )
		/* Ja*/
		free_bintree( tree->left );

	/* Hat das Blatt einen rechten Nachfolger ? */
	if( tree->right )
		/* Ja*/
		free_bintree( tree->right );

	/* Speicher freigeben */
	free( ( VOID * ) tree );
}

WORD insert_nterm( LONG nonterm )
{
	/*
		Hier werden die Nichtterminale der Produktionen gespeichert.
		Wird fÅr die Generierung der FOLLOW-Mengen benîtigt.
	*/
	REG ULONG	i;

	/* Ist das Element schon vorhanden ? */
	for( i = 0; i < nterm.anz_elem; i++ )
	{
		/* Sind die Elemente gleich ? */
		if( nonterm == nterm.arr[i] )
			/* Ja */
			return( 0 );
	}

	/* Nichtterminal eintragen */
	if( nterm.max_anz_elem == nterm.anz_elem )
	{
		/* Mehr Platz schaffen */
		nterm.max_anz_elem += 10;

		/* Array vergrîûern */
		if( !( nterm.arr = ( ULONG * ) realloc( ( VOID * ) nterm.arr, sizeof( ULONG ) * nterm.max_anz_elem ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0 ) );
	}

	/* Nichtterminal eintragen */
	nterm.arr[nterm.anz_elem++] = nonterm;

	/* Alles OK */
	return( 0 );
}


VOID analyze_inf_file( BYTE *inf_mem, ULONG size )
{
	/* Hier wird die Info-Datei analysiert */
	BYTE		*beg = inf_mem, *end = inf_mem + size;
	WORD		flag, kind = 0;
	REG WORD	count;

	/* Datei zu Ende ? */
	while( beg < end )
	{
		/* Kommentare ignorieren */
		beg = ignore_space( beg );

		/* Word extrahieren */
		beg = isword( beg );

		/* Flag zurÅcksetzen */
		flag = 0;

		/* FOLLOW ? */
		if( !( strcmp( strupr( non_term ), "FOLLOW" ) ) )
			/* Ist FOLLOW */
			flag = INF_FOLLOW;

		/* FIRST ? */
		else if( !( strcmp( strupr( non_term ), "FIRST" ) ) )
			/* Ist FIRST */
			flag = INF_FIRST;

		/* Grîûe der Grammatik-Hash-Tabelle ? */
		else if( !( strcmp( strupr( non_term ), "G_HASH_TAB_SIZE" ) ) )
			/* Ja */
			flag = INF_G_HASH_TAB_SIZE;

		/* DEBUG ? */
		else if( !( strcmp( strupr( non_term ), "DEBUG" ) ) )
			/* Ist DEBUG */
			flag = INF_DEBUG;

		/* DEST ? */
		else if( !( strcmp( strupr( non_term ), "DEST" ) ) )
			/* Ist DEST */
			flag = INF_DEST;

		/* ANALYZE ? */
		else if( !( strcmp( strupr( non_term ), "ANALYZE" ) ) )
			/* Ist ANALYZE */
			flag = INF_ANALYZE;

		/* ITEMS ? */
		else if( !( strcmp( strupr( non_term ), "ITEMS" ) ) )
			/* Ist ITEMS */
			flag = INF_ITEMS;

		/* SLR_TAB_1 ? */
		else if( !( strcmp( strupr( non_term ), "SLR_TAB_1" ) ) )
			/* Ist SLR_TAB_1 */
			flag = INF_SLR_TAB_1;

		/* SLR_TAB_2 ? */
		else if( !( strcmp( strupr( non_term ), "SLR_TAB_2" ) ) )
			/* Ist SLR_TAB_2 */
			flag = INF_SLR_TAB_2;

		/* MAX_CHAR ? */
		else if( !( strcmp( strupr( non_term ), "MAX_CHAR" ) ) )
			/* Ist MAX_CHAR */
			flag = INF_MAX_CHAR;

		/* Spaces ignorieren */
		beg = ignore_space( beg );

		/* = lîschen */
		beg++;

		/* Spaces ignorieren */
		beg = ignore_space( beg );

		/* Was war es ? */
		switch( flag )
		{
			case INF_FOLLOW:
			{
				/* Es ist FOLLOW */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.follow = ON;

				else
					/* OFF */
					slr_info.follow = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_FIRST:
			{
				/* Es ist FIRST */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.first = ON;

				else
					/* OFF */
					slr_info.first = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_DEBUG:
			{
				/* Es ist DEBUG */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.debug = ON;

				else
					/* OFF */
					slr_info.debug = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_ANALYZE:
			{
				/* Es ist ANALYZE */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.analyze = ON;

				else
					/* OFF */
					slr_info.analyze = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_ITEMS:
			{
				/* Es ist ITEMS */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.slr_items = ON;

				else
					/* OFF */
					slr_info.slr_items = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_SLR_TAB_1:
			{
				/* Es ist SLR_TAB_1 */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.slr_tab_1 = ON;

				else
					/* OFF */
					slr_info.slr_tab_1 = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_SLR_TAB_2:
			{
				/* Es ist SLR_TAB_2 */

				/* ON oder OFF ? */
				beg = isword( beg );

				/* ON ? */
				if( !strcmp( non_term, "ON" ) )
					/* ON */
					slr_info.slr_tab_2 = ON;

				else
					/* OFF */
					slr_info.slr_tab_2 = OFF;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_DEST:
			{
				/* Es ist DEST */

				/* SCREEN oder FILE ? */
				beg = isword( beg );

				/* Ausgabe in Datei ? */
				if( !strcmp( non_term, "FILE" ) )
					/* Ausgabe in Datei */
					slr_info.dest = DEST_FILE;

				/* Ausgabe auf Drucker ? */
				else if( !strcmp( non_term, "PRN" ) )
					/* Ausgabe auf Drucker */
					slr_info.dest = DEST_PRN;

				else
					/* Ausgabe auf Bildschirm */
					slr_info.dest = DEST_SCREEN;

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_G_HASH_TAB_SIZE:
			{
				/* Es ist INF_G_HASH_TAB_SIZE */

				/* ZÑhler initialisieren */
				count = 0;

				/* Anzahl der Bytes holen */
				while( ( *beg != 0x0a ) && ( *beg != 0x0d ) && ( beg < end ) )
					non_term[count++] = *beg++;

				/* Mit Null-Byte abschliessen */
				non_term[count] = 0;

				/* In binÑr wandeln */
				slr_info.gr_hash_tab_size = atol( non_term );

				/* NÑchste Angabe bearbeiten */
				break;
			}

			case INF_MAX_CHAR:
			{
				/* Es ist INF_MAX_CHAR */

				/* ZÑhler initialisieren */
				count = 0;

				/* Anzahl der Bytes holen */
				while( ( *beg != 0x0a ) && ( *beg != 0x0d ) && ( beg < end ) )
					non_term[count++] = *beg++;

				/* Mit Null-Byte abschliessen */
				non_term[count] = 0;

				/* In binÑr wandeln */
				slr_info.max_char = atoi( non_term );

				/* NÑchste Angabe bearbeiten */
				break;
			}

			default:
			{
				/* Unbekannte Angabe */
				error( INF_FILE_ERR, 0 );

				/* Bis Zeilenvorschub lîschen */
				beg = strchr( beg, 0x0a );

				/* NÑchste Angabe bearbeiten */
				break;
			}
		}

		/* Doppelte Angabe ? */
		if( ( flag ) && ( kind & flag ) )
			/* Ist doppelt angegeben worden */
			error( INF_ERR, 0 );

		else
			/* Ist in Ordnung */
			kind |= flag;
	}

	/* Anzahl der Zeilen zurÅcksetzen */
	anz_zeile = 0;
}