/*
	Projekt		:	SLR-Parser-Generator
	Modul		:	MAIN.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/


/* Includes */
#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include <tos.h>
#include "slr.h"


/* Defines */


/* Aktionen, die in AbhÑngigkeit vom Fehler ausgelîst werden */

/* Grammatik-Datei schliessen */
#define	CLOSE_FILE						0

/* Speicher der Grammatik-Datei freigeben */
#define FREE_MEMORY						1



/* Default-Grîûe der Grammatik-Hash-Tabelle */
#define	DEFAULT_GR_HASH_TAB_SIZE		2000 + 1

/* Max. Anzahl der Zeichen pro Zeile */
#define MAX_CHAR						65



/* Deklarationen */

/* Hash-Tabelle fÅr die Nichtterminale/Terminale */
PROD_HASH		*prod_hash;


/* PROD_INFO-Struktur */
PROD_INFO		*prod_info;


/* Ausgabe in Datei/Bildschirm */
FILE			*file;


/* Erkanntes Nichtterminal */
BYTE			non_term[256];


/* Fehlermeldungen */
BYTE	*error_msgs[]	=	{	"\nDatei nicht gefunden !",
								"\nFehler beim ôffnen der Grammatik-Datei !",
								"\nNicht genug Speicher frei, um die Datei zu lesen !",
								"\nFehler beim Lesen der Grammatik-Datei !",
								"\nFehler beim Schlieûen der Grammatik-Datei !",
								"\nZeile %lu: Unerlaubtes Zeichen !",
								"\nZu wenig Speicher fÅr die Grammatik-Hash-Tabelle frei !",
								"\nZeile %lu: Die rechte Seite der Produktion ist von der"
								"\nlinken Seite falsch getrennt worden !",
								"\nZeile %lu: Nichtterminal %s wurde schon"
								" definiert !",
								"\nDer Speicher reicht nicht !",
								"\nZeile %lu: Das Nichtterminal %s wurde nicht definiert !",
								"\nZeile %lu: Die Produktion %s hat doppelte Alternativen !",
								"\nZeile %lu: Epsilon-Fehler in Produktion %s !",
								"\nDie Grammatik hat keine Startproduktion !",
								"\nFehler bei der Angabe der AssoziativitÑt bzw."
								"\nPrioritÑt( nur %%left u. %%right ) !",
								"\nFehler beim ôffnen der Info-Datei !",
								"\nFehler beim Lesen der Info-Datei !",
								"\nFehler beim Schlieûen der Info-Datei !",
								"\nFehler beim ôffnen der Include-Datei !",
								"\nFehler beim Schlieûen der Include-Datei !",
								"\nZeile %lu: Unbekanntes Terminal %s !",
								"\nFehler in der Info-Datei !",
								"\nDoppelte Angabe in der Info-Datei !",
								"\nEs wurde keine Grammatikdatei angegeben !" };


/* Beinhaltet Zeiger auf Grammatik-Datei */
BYTE			*grammar_file;


/* Grammatik-Dateihandle */
WORD			grammar_filehandle;


/* Grîûe der PROD_INFO-Strukt in PROD_INFO-Einheiten */
LONG			prod_info_size = DEFAULT_PROD_INFO_LENGTH;


/* Anzahl der Zeilen zÑhlen */
ULONG			anz_zeile = 1;


/* Struktur fÅr die INF-Datei */
SLR_INFO		slr_info;



/* Prototypen */
WORD init( VOID );
WORD read_grammar_file( BYTE *grammar_filename );
WORD inf_file( VOID );
VOID analyze_inf_file( BYTE *inf_mem, ULONG size );



WORD main( WORD argc, BYTE *argv[] )
{
	/* Hauptroutine */
	BYTE		*file_name, *temp_file_name;
	struct time zeit1, zeit2;
	ULONG		sec1, sec2;

	/* Meldung ausgeben */
	puts( INFO );

	/* Wurde ein Grammatikdatei angegeben ? */
	if( !argv[1] )
		/* Fehler*/
		return( error( NO_GRFILE_NAME, 0 ) );

	/* Grammatikdateinamen extrahieren */
	file_name = strrchr( argv[1], ( WORD ) '\\' );

	/* Ist kein '\\' im Filename angeben worden ? */
	if( !file_name )
		/* Ja */
		file_name = argv[1];

	else
		/* '\\' Åbergehen */
		file_name++;

	/* Dateinamen in Grossbuchstaben wandeln */
	file_name = strupr( file_name );

	/* INF-Datei lesen */
	if( inf_file() < 0 )
		/* Fehler */
		return( -1 );

	/* Meldungen ausgeben */
	fprintf( stderr, "\n\nAnalysiere %s\nBitte warten...\n", file_name );

	/* Zentrale Dinge erledigen */
	if( init() < 0 )
		/* Fehler */
		return( -1 );

	/* Grammatik-Datei lesen */
	if( read_grammar_file( argv[1] ) < 0 )
		/* Fehler */
		return( -1 );

	/* Ausgaben in Datei bzw. auf den Bildschirm */
	if( slr_info.dest == DEST_FILE )
	{
		/* Dateinamen aufbereiten */
		temp_file_name = strrchr( file_name, ( WORD ) '.' );
		strcpy( temp_file_name, ".TXT" );

		/* Datei zum Schreiben îffnen */
		file = freopen( file_name, "w", stdout );
	}

	/* Ausgabe auf Bildschirm ? */
	else if( slr_info.dest == DEST_SCREEN )
		/* Bildschirm */
		file = stdout;

	else
		/* Drucker */
		file = stdprn;

	/* Zeit merken */
	gettime( &zeit1 );

	/* Zeit in Sekunden umwandeln */
	sec1 = ( zeit1.ti_hour * 3600L ) + ( zeit1.ti_min * 60L ) + zeit1.ti_sec;

	/* Grammatik analysieren */
	if( analyze_grammar() < 0 )
		/* Fehler */
		return( -1 );

	/* Menge aller Items berechnen */
	if( gen_items() < 0 )
		/* Fehler */
		return( -1 );

	/* Parse-Tabellen erzeugen */
	if( make_tab() < 0 )
		/* Fehler */
		return( -1 );

	/* Output erzeugen */
	if( gen_output( argv[1] ) < 0 )
		/* Fehler */
		return( -1 );

	/* Zeit holen */
	gettime( &zeit2 );

	/* Zeit in Sekunden umwandeln */
	sec2 = ( zeit2.ti_hour * 3600L ) + ( zeit2.ti_min * 60L ) + zeit2.ti_sec;

	/* Zeit merken */
	fprintf( file, "Benîtigte Zeit: %02lu:%02lu:%02lu", ( sec2 - sec1 ) / 3600L, ( ( sec2 - sec1 ) % 3600L ) / 60L, ( ( sec2 - sec1 ) % 3600L ) % 60L );

	/* Ausgabe auf Bildschirm ? */
	if( slr_info.dest == DEST_SCREEN )
		/* Auf Tastendruck warten */
		Cnecin();

	/* Datei schliessen */
	if( fclose( file ) < 0 )
		/* Fehler */
		return( error( CANNOT_CLOSE_FILE, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD init( VOID )
{
	/* Speicher fÅr die HASH-Tabellen anfordern */

	/* Speicher fÅr Grammatik-Hash-Tabelle anfordern */
	if( !( prod_hash = ( PROD_HASH * ) calloc( sizeof( PROD_HASH ), slr_info.gr_hash_tab_size ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_GR_HASH_MEMORY, 0 ) );

	/* Speicher fÅr PROD_INFO anfordern */
	if( !( prod_info = ( PROD_INFO * ) calloc( sizeof( PROD_INFO ), DEFAULT_PROD_INFO_LENGTH ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD read_grammar_file( BYTE *grammar_filename )
{
	/*	Diese Routine lieût die Grammatik-Datei ein. */
	struct ffblk	grammar_fcontent;

	/* Ist die Datei existent ? */
	if( findfirst( grammar_filename, &grammar_fcontent, 0 ) )
		/* Fehler */
		return( error( FILE_NOT_FOUND, 0 ) );

	/* Datei îffnen */
	if( ( grammar_filehandle = open( grammar_filename, O_RDONLY ) ) <= 0 )
		/* Fehler */
		return( error( CANNOT_OPEN_FILE, 0 ) );

	/* Genug Speicher anfordern */
	if( !( grammar_file = ( BYTE * ) malloc( grammar_fcontent.ff_fsize + 1 ) ) )
		/* Grammatikdatei schliessen -> Fehler */
		return( error( NOT_ENOUGH_FILE_MEMORY, CLOSE_FILE ) );

	/* Grammatik-Datei lesen */
	if( read( grammar_filehandle, ( VOID * ) grammar_file, grammar_fcontent.ff_fsize ) != grammar_fcontent.ff_fsize )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( error( CANNOT_READ_FILE, CLOSE_FILE | FREE_MEMORY ) );

	/* Null-Byte anhÑngen */
	grammar_file[grammar_fcontent.ff_fsize] = 0;

	/* Datei schliessen */
	if( close( grammar_filehandle ) )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( error( CANNOT_CLOSE_FILE, CLOSE_FILE | FREE_MEMORY ) );

	/* Alles OK */
	return( 0 );
}


WORD inf_file( VOID )
{
	/*
		Hier wird die INF-Datei gelesen, wenn sie existiert.
		Andernfalls wird die SLR_INFO_Struktur mit Default-Werten
		initialisiert.
	*/
	struct ffblk	inf_file;
	BYTE			*inf_mem;

	/* SLR-INFO-Tabelle initialisieren */
	memset( ( VOID * ) &slr_info, -1, sizeof( SLR_INFO ) );

	/* Ist die Datei existent ? */
	if( !findfirst( "SLR.INF", &inf_file, 0 ) )
	{
		/* Datei îffnen */
		if( ( grammar_filehandle = open( "SLR.INF", O_RDONLY ) ) <= 0 )
			/* Fehler */
			return( error( CANNOT_OPEN_INF_FILE, 0 ) );

		/* Speicher fÅr die Info-Datei anfordern */
		if( !( inf_mem = ( BYTE * ) malloc( inf_file.ff_fsize ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0 ) );

		/* INF-Datei lesen */
		if( read( grammar_filehandle, ( VOID * ) inf_mem, inf_file.ff_fsize ) != inf_file.ff_fsize )
			/* INF-Datei schliessen */
			return( error( CANNOT_READ_INF_FILE, CLOSE_FILE ) );

		/* Datei schliessen */
		if( close( grammar_filehandle ) )
			/* INF-Datei schliessen */
			return( error( CANNOT_CLOSE_INF_FILE, CLOSE_FILE ) );

		/* INF-File analysieren */
		analyze_inf_file( inf_mem, inf_file.ff_fsize );
	}

	/* Ist die Ausgabe angegeben worden ? */
	if( slr_info.dest < 0 )
		/* Ausgabe auf Bildschirm */
		slr_info.dest = DEST_SCREEN;

	/* Sollen die FOLLOW-Mengen ausgegeben werden ? */
	if( slr_info.follow < 0 )
		/* FOLLOW-Mengen erzeugen */
		slr_info.follow = ON;

	/* Sollen die FIRST-Mengen ausgegeben werden ? */
	if( slr_info.first < 0 )
		/* FIRST-Mengen erzeugen */
		slr_info.first = ON;

	/* Sollen die ITEMS ausgegeben werden ? */
	if( slr_info.slr_items < 0 )
		/* ITEMS ausgeben */
		slr_info.slr_items = ON;

	/*
		Sollen Informationen Åber die Grammatik-Analyse ausgegeben
		werden ?
	 */
	if( slr_info.analyze < 0 )
		/* Informationen ausgeben */
		slr_info.analyze = ON;

	/* Soll die Tabelleninformation ausgegeben werden ? */
	if( slr_info.slr_tab_1 < 0 )
		/* SLR-Tabelle ausgeben */
		slr_info.slr_tab_1 = ON;

	/* Soll die SLR-Tabelle ausgegeben werden ? */
	if( slr_info.slr_tab_2 < 0 )
		/* SLR-Tabelle ausgeben */
		slr_info.slr_tab_2 = ON;

	/* Soll die DEBUG-Version erzeugt werden ? */
	if( slr_info.debug < 0 )
		/* DEBUG-Version erzeugen */
		slr_info.debug = ON;

	/* Ist die Grîûe der Grammatik-Hash-Tabelle angegeben worden ? */
	if( slr_info.gr_hash_tab_size < 0 )
		slr_info.gr_hash_tab_size = DEFAULT_GR_HASH_TAB_SIZE;

	/* Max. Anzahl der Zeichen pro Zeile ? */
	if( slr_info.max_char < 0 )
		/* Max. Anzahl der Zeichen festlegen */
		slr_info.max_char = MAX_CHAR;

	/* Alles OK */
	return( 0 );
}


WORD error( WORD err_no, WORD err )
{
	/* Diese Routine gibt eine Fehlermeldung aus. */
	printf( error_msgs[err_no], anz_zeile, non_term );

	/* Aktionen in AbhÑngigkeit vom Fehler einleiten */
	if( err & CLOSE_FILE )
	{
		/* Grammatik-Datei schliessen */
		if( close( grammar_filehandle ) )
			/* Fehler im Fehler :-) */
			return( error( CANNOT_CLOSE_FILE, 0 ) );
	}

	else if( err & FREE_MEMORY )
		/* Speicher von der Grammatik-Datei freigeben */
		free( ( VOID * ) grammar_file );

	/* Fehler */
	return( -1 );
}