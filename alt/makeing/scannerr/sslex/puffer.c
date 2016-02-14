/*
	Dieses kleine ProgrammstÅck implementiert eine Zeichenleseroutine
	fÅr einen Scanner.
	Es wird mit zwei Puffern gearbeitet.
	Das Verfahren ist dem Drachenbuch S. 110 entnommen.

	Modul	: PUFFER.C
	Start	: 15.04.1997
	Ende	:
	Sprache	: ANSI-C
	Compiler: PureC V1.1
*/


/************/
/* Includes */
/************/

#include <portab.h>
#include <stdio.h>




/****************/
/* Definitionen */
/****************/

/* LÑnge einer PufferhÑlfte */
#define		PUFFER_LENGTH	1500



/* Puffer1 definieren */
BYTE		puffer[( PUFFER_LENGTH * 2 ) + 1];

/* Position in Puffer */
BYTE		*p_pos;

/* FILE-Struktur fÅr Datei */
FILE		*quell_file;

/* Flag fÅr ZurÅckstellung */
WORD		unget = 0;



/**************/
/* Prototypen */
/**************/
VOID init_puffer( BYTE *filename );
VOID read_puffer( WORD part );
VOID unget_char( WORD value );
BYTE get_char( VOID );


WORD main( VOID )
{
	BYTE	name[] = "PUFFER.C", k = 0;
	WORD	count = 0;

	init_puffer( name );

	do
	{
		while( k != EOF )
		{
			k = get_char();
			putc( k, stdout );
		}

		unget_char( 3000 );
		k = 0;
		continue;

	}while( 1 );
	return( 0 );
}


VOID init_puffer( BYTE *filename )
{
	/* Hier wird der Puffer initialisiert. */

	/* Datei laden */
	quell_file = fopen( filename, "r" );

	/* Erste PufferhÑlfte laden */
	read_puffer( 0 );

	/* Position des Zeigers auf Dateianfang setzen */
	p_pos = puffer;
}


VOID read_puffer( WORD part )
{
	/* Hier wird eine PufferhÑlfte geladen */
	ULONG	offset = 0, length;

	/* Welche PufferhÑlfte laden ? */
	if( !part )
	{
		/* Ist die Eingabe schon gelesen worden ? */
		/* Erste PufferhÑlfte laden */
		length = fread( puffer, 1, PUFFER_LENGTH, quell_file );
	}

	else
	{
		/* Offset setzen */
		offset = PUFFER_LENGTH + 1;

		/* Zweite PufferhÑlfte laden */
		length = fread( &puffer[offset], 1, PUFFER_LENGTH, quell_file );
	}

	/* Datei-Ende-Zeichen setzen */
	puffer[length + offset] = EOF;
}


BYTE get_char( VOID )
{
	/* Zeichen lesen und zurÅckliefern */
	if( *p_pos == EOF )
	{
		/* Am Ende der ersten HÑlfte ? */
		if( ( p_pos - puffer ) == PUFFER_LENGTH )
		{
			/* Ja, wurde die zweite HÑlfte schon gelesen ? */
			if( !unget )
				/* Nein, die zweite HÑlfte lesen */
				read_puffer( 1 );

			/* Einfach ein weiter */
			p_pos++;

			/* Flag zurÅcksetzen */
			unget = 0;
		}

		/* Am Ende der zweiten HÑlfte ? */
		else if( ( p_pos - puffer ) == ( ( 2 * PUFFER_LENGTH ) + 1 ) )
		{
			/* Ja, ist die erste HÑlfte schon gelesen worden ? */
			if( !unget )
				/* Nein, die erste HÑlfte lesen */
				read_puffer( 0 );

			/* Position am Anfang des Puffers setzen */
			p_pos = puffer;

			/* Flag zurÅcksetzen */
			unget = 0;
		}

		else
			/* Dateiende erreicht */
			fclose( quell_file );
	}

	/* Zeichen zurÅckliefern */
	return( *p_pos++ );
}


VOID unget_char( WORD value )
{
	/* Diese Routine legt value Zeichen zurÅck. */

	/* Zu viele Zeichen ? */
	if( value > ( 2 * PUFFER_LENGTH ) )
		/* Fehler */
		fprintf( stderr, "\nDie Anzahl der zurÅckzustellenden Zeichen ist zu hoch !" );

	/* Flag setzen */
	unget++;

	/* Neue Zeigerposition berechnen */
	if( ( p_pos - value ) < puffer )
		/* p_pos - value ist in der zweiten HÑlte des Puffers */
		p_pos = ( 1 + puffer + ( 2 * PUFFER_LENGTH ) ) - ( value - ( p_pos - puffer ) );

	else
		/* Ist ok */
		p_pos -= value;
}