/*
	Projekt		:	Scanner-Generator
	Modul		:	MAIN.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	18.07.1997
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/





/************/
/* Includes */
/************/

/* Include-Datei des Scanner-Generators */
#include "sslex.h"
#include "sslexpar.h"



/**************/
/* Prototypen */
/**************/

WORD analyze_it( VOID );
WORD do_predef( VOID );
WORD scanner( VOID );


/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/* Wenn Null, wird keine SC eingetragen(bei vordefinierten NEAen) */
WORD	insert_ssc = 0;

/* Beginn des REs */
BYTE	*temp_grammar;



/***********/
/* Imports */
/***********/

MGLOBAL ULONG	zeile;
MGLOBAL WORD	analyze;
MGLOBAL BYTE	*grammar;
MGLOBAL BYTE	all_bchar[];
MGLOBAL BYTE	all_char[];




WORD main( WORD argc, BYTE *argv[] )
{
	/* Die Hauptroutine */

	/* Die Scanner-Datei lesen */
	if( read_scanner_file( argv[1] ) < 0 )
		/* Fehler */
		return( -1 );

	/* Mit dem Auswerten der Eingabe beginnen */
	if( analyze_it() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD output_error( WORD err_nr )
{
	/* Hier wird ein Fehler ausgegeben. */

	/* Welchen Fehler ? */
	switch( err_nr )
	{
		case NO_MEMORY:
		{
			/* Zu wenig Speicher */
			fprintf( stderr, "\nZu wenig Speicher !" );
			break;
		}
	}

	/* Fehler */
	return( -1 );
}


WORD analyze_it( VOID )
{
	/* Hier wird die Eingabe bearbeitet. */
	WORD	sym, def_char = 0;

	/* SC-'INITIAL' eintragen */
	if( insert_sc( "INITIAL", INCLUSIVE ) < 0 )
		/* Fehler */
		return( -1 );

	/* Zeichenklasse l”schen */
	memset( ( VOID * ) all_bchar, 0, CCL_WIDTH * sizeof( BYTE ) );

	/* Zeichenklasse l”schen */
	memset( ( VOID * ) all_char, 0, 255 );

	do
	{
		/* Unn”tiges entfernen */
		delete();

		/* Textstelle merken */
		temp_grammar = grammar;

		/* Scanner aufrufen */
		sym = scanner();

		/* Auswerten */
		switch( sym )
		{
			case DO_INCL_SC:
			{
				/* Definition der inclusive-Startsymbole */
				if( !analyze )
					/* Die Startsymbole auswerten */
					do_sc( INCLUSIVE );

				else
					/* Definition der Startsymbole muž vor den REs liegen */
					fprintf( stderr, "\nZeile %lu: Die Definition der Startsymbole muž vor den REs stehen !", zeile );

				break;
			}

			case DO_EXCL_SC:
			{
				/* Definition der exclusive-Startsymbole */
				if( !analyze )
					/* Die Startsymbole auswerten */
					do_sc( EXCLUSIVE );

				else
					/* Definition der Startsymbole muž vor den REs liegen */
					fprintf( stderr, "\nZeile %lu: Die Definition der Startsymbole muž vor den REs stehen !", zeile );

				break;
			}

			case DO_PREDEF:
			{
				insert_ssc = 1;

				/* Definition der vordefinierten NEAen */
				if( !analyze )
					/* Die vordefinierten NEAen auswerten */
					do_predef();

				else
					/* Definition der vordefinierten NEAen muž vor den REs liegen */
					fprintf( stderr, "\nZeile %lu: Die Definition der vordefinierten NEAen muž vor den REs stehen !", zeile );

				insert_ssc = 0;
				break;
			}

			case DO_DEFCHAR:
			{
				insert_ssc = 1;

				/* Definition der erlaubten Zeichen */
				if( !analyze )
				{
					/* Die Angaben auswerten */
					do_defchar( 0 );

					/* Zeichen wurden definiert */
					def_char = 1;
				}

				else
					/* Definition der Zeichen muž vor den REs liegen */
					fprintf( stderr, "\nZeile %lu: Die Definition der Zeichen muž vor den REs stehen !", zeile );

				insert_ssc = 0;
				break;
			}

			case ACCEPT:
			{
				break;
			}

			default:
			{
				/* Einen RE parsen */

				/* Ursprngliche Textstelle */
				grammar = temp_grammar;

				/* Den Parser aufrufen */
				if( parse() )
					/* Es ist ein Fehler aufgetreten */ 
					return( -1 );

				break;
			}
		}

	}while( sym != ACCEPT );

	/* Wurden die erlaubten Zeichen definiert ? */
	if( !def_char )
		/* Nein, Warnung ausgeben und default festsetzen */
		do_defchar( 1 );

	/* Nun die einzelnen NEAen in den SCs zusammenfassen */
	if( finish_sc() < 0 )
		/* Fehler */
		return( -1 );

	/* Nun die einzelnen NEAen zu DEAen machen */
	if( gen_dea() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}