/*
	Projekt		:	Scanner-Generator
	Modul		:	IO.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	26.03.1998
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/





/************/
/* Includes */
/************/

#include <ext.h>
#include <portab.h>
#include <stdlib.h>
#include <stdio.h>


/**************/
/* Prototypen */
/**************/




/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/* Scanner-Datei */
BYTE	*grammar;




/***********/
/* Imports */
/***********/





WORD read_scanner_file( BYTE *filename )
{
	/* Hier wird die Scanner-Datei gelesen. */
	struct ffblk	file_str;
	WORD			handle;

	/* Ist die Datei existent ? */
	if( findfirst( filename, &file_str, 0 ) < 0 )
		/* Fehler */
		return( -1 );

	/* Datei ”ffnen */
	if( ( handle = open( filename, O_RDONLY ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Speicher fr Grammatik-Datei anfordern */
	if( ( grammar = ( BYTE * ) malloc( file_str.ff_fsize + 1 ) ) == NULL )
		/* Grammatikdatei schliessen -> Fehler */
		return( -1 );

	/* Grammatik-Datei lesen */
	if( read( handle, ( VOID * ) grammar, file_str.ff_fsize ) < 0 )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( -1 );

	/* Null-Byte anh„ngen */
	grammar[file_str.ff_fsize] = 0;

	/* Datei schliessen */
	if( close( handle ) < 0 )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( -1 );

	/* Zuruecksetzen */
	handle = -1;

	/* Alles OK */
	return( 0 );
}