/*
	Dieses kleine Programmst�ck implementiert eine Zeichenleseroutine
	f�r einen Scanner.
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

/* L�nge einer Pufferh�lfte */
#define		PUFFER_LENGTH	1500



/* Puffer1 definieren */
BYTE		puffer[( PUFFER_LENGTH * 2 ) + 1];

/* Position in Puffer */
BYTE		*p_pos;

/* FILE-Struktur f�r Datei */
FILE		*quell_file;

/* Flag f�r Zur�ckstellung */
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

	/* Erste Pufferh�lfte laden */
	read_puffer( 0 );

	/* Position des Zeigers auf Dateianfang setzen */
	p_pos = puffer;
}


VOID read_puffer( WORD part )
{
	/* Hier wird eine Pufferh�lfte geladen */
	ULONG	offset = 0, length;

	/* Welche Pufferh�lfte laden ? */
	if( !part )
	{
		/* Ist die Eingabe schon gelesen worden ? */
		/* Erste Pufferh�lfte laden */
		length = fread( puffer, 1, PUFFER_LENGTH, quell_file );
	}

	else
	{
		/* Offset setzen */
		offset = PUFFER_LENGTH + 1;

		/* Zweite Pufferh�lfte laden */
		length = fread( &puffer[offset], 1, PUFFER_LENGTH, quell_file );
	}

	/* Datei-Ende-Zeichen setzen */
	puffer[length + offset] = EOF;
}


BYTE get_char( VOID )
{
	/* Zeichen lesen und zur�ckliefern */
	if( *p_pos == EOF )
	{
		/* Am Ende der ersten H�lfte ? */
		if( ( p_pos - puffer ) == PUFFER_LENGTH )
		{
			/* Ja, wurde die zweite H�lfte schon gelesen ? */
			if( !unget )
				/* Nein, die zweite H�lfte lesen */
				read_puffer( 1 );

			/* Einfach ein weiter */
			p_pos++;

			/* Flag zur�cksetzen */
			unget = 0;
		}

		/* Am Ende der zweiten H�lfte ? */
		else if( ( p_pos - puffer ) == ( ( 2 * PUFFER_LENGTH ) + 1 ) )
		{
			/* Ja, ist die erste H�lfte schon gelesen worden ? */
			if( !unget )
				/* Nein, die erste H�lfte lesen */
				read_puffer( 0 );

			/* Position am Anfang des Puffers setzen */
			p_pos = puffer;

			/* Flag zur�cksetzen */
			unget = 0;
		}

		else
			/* Dateiende erreicht */
			fclose( quell_file );
	}

	/* Zeichen zur�ckliefern */
	return( *p_pos++ );
}


VOID unget_char( WORD value )
{
	/* Diese Routine legt value Zeichen zur�ck. */

	/* Zu viele Zeichen ? */
	if( value > ( 2 * PUFFER_LENGTH ) )
		/* Fehler */
		fprintf( stderr, "\nDie Anzahl der zur�ckzustellenden Zeichen ist zu hoch !" );

	/* Flag setzen */
	unget++;

	/* Neue Zeigerposition berechnen */
	if( ( p_pos - value ) < puffer )
		/* p_pos - value ist in der zweiten H�lte des Puffers */
		p_pos = ( 1 + puffer + ( 2 * PUFFER_LENGTH ) ) - ( value - ( p_pos - puffer ) );

	else
		/* Ist ok */
		p_pos -= value;
}