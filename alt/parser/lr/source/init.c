/*
	Projekt		:	LR-Parser-Generator V1.00û
	Modul		:	INIT.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/



/************/
/* Includes */
/************/

#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include <tos.h>

/* Include-Datei des Parser-Generators */
#include "LR.H"



/**************/
/* Prototypen */
/**************/

WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD read_grammar_file( BYTE *filename );
BYTE *output_file_msg( BYTE *filename );
VOID free_mem( VOID **pointer );
VOID analyze_info_file( VOID );
WORD do_all( BYTE *filename );
WORD analyze_grammar( VOID );
WORD generate_follow( VOID );
WORD generate_parser( VOID );
WORD read_info_file( VOID );
WORD output_grammar( VOID );
WORD generate_items( VOID );
WORD output_follow( VOID );
WORD output_lr_tab( VOID );
WORD output_items( VOID );
WORD output_first( VOID );
WORD make_tab( VOID );




/*****************/
/* Deklarationen */
/*****************/

/* Datei bei Fehler schlieûen */
#define CLOSE_FILE				1

/* Speicher bei Fehler freigeben */
#define FREE_MEMORY				2

/* Standard-Hash-Tab-Grîûe */
#define DEFAULT_HASH_TAB_SIZE	2000

/* Anzahl der Zeichen pro Zeile */
#define MAX_CHAR				65



/****************/
/* Definitionen */
/****************/


/* Beinhaltet die Angaben der Info-Datei */
LR_INFO		lr_info;

/* Dateihandle */
WORD		handle = -1;

/* Ausgabe auf FILE */
FILE		*file = stderr;

/* Beinhaltet die Grammatik-Datei */
BYTE		*grammar = 0L;

/* Aktuelle Zeile in der Grammatik-Datei */
ULONG		zeile = 1;

/* Hash-Tabelle */
HASH_TAB	*hash_tab = 0L;

/* Dateiname */
BYTE		*file_name = 0L;

/* Fehlermeldungen */
BYTE		*error_msgs[] =
{
"\nEs wurde keine Grammatikdatei angegeben !",
"\nKann Grammatik-Datei nicht finden !",
"\nKann Grammatik-Datei nicht îffnen !",
"\nKann Grammatik-Datei nicht lesen !",
"\nKann Grammatik-Datei nicht schlieûen !",
"\nKann Info-Datei nicht îffnen !",
"\nKann Info-Datei nicht lesen !",
"\nKann Info-Datei nicht schlieûen !",
"\nKann Ausgabe-Datei nicht schlieûen !",
"\nNicht genug Speicher !",
"\nZeile %lu: Illegales Zeichen %c !",
"\nZeile %lu: Unbekannte Angabe in der Info-Datei !",
"\nDie Hash-Tabelle ist zu klein !",
"\nZeile %lu: %%prec ist nur in einer Produktion erlaubt !",
"\nTerminal %s wurde nicht benutzt !",
"\nZeile %lu: Doppelte Fehlermeldung !",
"\nZeile %lu: Seperator fehlerhaft !",
"\nZeile %lu: Produktion muss mit einem Nichtterminal beginnen !",
"\nZeile %lu: Die PrioritÑt des Terminals ist kleiner als die PrioritÑt"
"\nder Produktion !",
"\nMaximal zulÑssige Anzahl an Elementen auf der rechten Seite Åberschritten !",
"\nZeile %lu: Fehlendes Meta-Symbol %c in Produktion %s !",
"\nZeile %lu: %%error ist nur in einer Produktionen erlaubt !",
"\nZeile %lu: %%left und %%right mÅssen vor der Grammatik stehen !",
"\nZeile %lu: Meta-Symbol %c in Produktion %s ÅberflÅssig !",
"\nNichtterminal %s wurde nicht definiert !",
"\nNichtterminal %s wurde nicht benutzt !",
"\nZu dem Terminal %s wurde keine PrioritÑt angegeben !",
"\nZeile %lu: Das Nichtterminal %s wurde bereits definiert !",
"\nKann BNF-Grammatik-Datei nicht schlieûen !",
"\nZeile %lu: Nicht abgeschlossenes Terminal %s !",
"\nUnsinnige Alternative Nr. %d in Produktion %s !",
"\nKonnte Include-Datei des Parser-Quelltextes nicht îffnen !",
"\nKonnte Include-Datei des Parser-Quelltextes nicht schlieûen !",
"\nKonnte Parser-Quelltext-Datei nicht îffnen !",
"\nKonnte Parser-Quelltext-Datei nicht schlieûen !"
};



/***********/
/* Imports */
/***********/

MGLOBAL NTERM_FIRST	**nterm_first;
MGLOBAL ERROR_STR	**error_struct;
MGLOBAL HULLE_STR	**hulle_str;
MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL TERM_STR	*term_str;
MGLOBAL LR_TAB		**lr_tab;
MGLOBAL ULONG		nterm_first_count;
MGLOBAL ULONG		*ziel_tab;
MGLOBAL LONG		nterm_count;
MGLOBAL LONG		error_struct_count;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL LONG		*cop_lr_tab;
MGLOBAL FILE		*bnf_file;
MGLOBAL FILE		*parser_file;




WORD main( WORD argc, BYTE *argv[] )
{
	/* Hauptroutine */
	struct time	zeit1, zeit2;
	ERROR_STR	*error_str, *temp_error_str;
	REG ULONG	i, j;
	ULONG		sec1, sec2;
	WORD		state;

	/* Meldung ausgeben */
	puts( INFO );

	/* Zeit merken */
	gettime( &zeit1 );

	/* Zeit in Sekunden umwandeln */
	sec1 = ( zeit1.ti_hour * 3600L ) + ( zeit1.ti_min * 60L ) + zeit1.ti_sec;

	/* Hauptroutine aufrufen */
	state = do_all( argv[1] );

	/* Speicher freigeben */
	if( grammar )
		/* Speicher der Grammatikdatei freigeben */
		free( ( VOID * ) grammar );

	if( error_struct )
	{
		/* Speicher der ERROR-Strukturen freigeben */
		for( i = 0; i < error_struct_count; i++ )
		{
			/* Zeiger auf ERROR_STR holen */
			error_str = error_struct[i];

			/* Muss noch Speciher freigegeben werden ? */
			while( error_str )
			{
				/* Folge-Struktur merken */
				temp_error_str = error_str->error_str;

				/* Speicher des Fehlermeldung-Textes freigeben */
				free( ( VOID * ) error_str->err_msg );

				/* Speicher der aktuellen ERROR_STR freigeben */
				free( ( VOID * ) error_str );

				/* Pointer umkopieren */
				error_str = temp_error_str;
			}
		}
	}

	if( hulle_str )
	{
		/* Speicher sÑmtlicher HÅllen freigeben */
		for( i = 0; i <= hulle_count; i++ )
		{
			/* ITEM_STRs freigeben */
			for( j = 0; j < hulle_str[i]->anz_items; j++ )
			{
				/* Speicher freigeben */
				free( ( VOID * ) hulle_str[i]->item_str[j].term.term_arr );
				free( ( VOID * ) hulle_str[i]->item_str );
			}
		}
	}

	if( lr_tab )
	{
		/* Speicher der LR-Tabelle freigeben */
		for( i = 0; i <= hulle_count; i++ )
			free( ( VOID * ) lr_tab[i] );

		/* Den Speicher des Arrays freigeben */
		free( ( VOID * ) lr_tab );
	}

	if( jump_tab )
		/* Speicher der ZustandsÅbergÑnge freigeben */
		free( ( VOID * ) jump_tab );

	if( ziel_tab )
		/* Speicher der Ziel-Tabelle freigeben */
		free( ( VOID * ) ziel_tab );

	if( cop_lr_tab )
		/* Speicher der komprimierten LR-Tabelle freigeben */
		free( ( VOID * ) cop_lr_tab );

	if( nterm_str )
	{
		/* Speicher sÑmtlicher NTERM_STRs zurÅckgeben */
		for( i = 0; i < nterm_count; i++ )
		{
			/* Die FOLLOW-Elemente freigeben */
			free( ( VOID * ) nterm_str[i]->follow.term_arr );

			/* Die NTERM_STR-Struktur freigeben */
			free( ( VOID * ) nterm_str[i] );
		}

		/* Den Speicher des Arrays freigeben */
		free( ( VOID * ) nterm_str );
	}

	if( term_str )
		/* Speicher der Terminale freigeben */
		free( ( VOID * ) term_str );

	if( nterm_first )
	{
		/* Speicher der FIRST-Elemente freigeben */
		for( i = 0; i < nterm_first_count; i++ )
		{
			/* Speicher der FIRST-Terminale freigeben */
			free( ( VOID * ) nterm_first[i]->first.term_arr );
			free( ( VOID * ) nterm_first[i] );
		}

		/* Den Speicher des Arrays freigeben */
		free( ( VOID * ) nterm_first );
	}

	if( hash_tab )
	{
		/* Speicher der HASH-Tabelle freigeben */
		for( i = 0; i < lr_info.hash_tab_size; i++ )
		{
			/* Ist etwas eingetragen ? */
			if( hash_tab[i].name )
			{
				/* Ja, Speicher freigeben */
				free( ( VOID * ) hash_tab[i].name );
				free( ( VOID * ) hash_tab[i].term_str );
			}

			/* Speicher der eigentlichen HASH-Tabelle freigeben */
			free( ( VOID * ) hash_tab );
		}
	}

	/* Datei schliessen */
	if( handle != -1 )
	{
		/* Datei schliessen */
		if( close( handle ) < 0 )
			/* Fehler */
			return( error( CANNOT_CLOSE_GRAMMAR_FILE, 0, 0 ) );
	}

	/* Parser-Source-Datei schliessen ? */
	if( parser_file )
	{
		/* Datei schliessen */
		if( fclose( parser_file ) == EOF )
			/* Fehler */
			return( error( CANNOT_CLOSE_PARSER_FILE, 0, 0 ) );
	}

	/* BNF-Grammatik-Datei schliessen ? */
	if( bnf_file )
	{
		/* Datei schliessen */
		if( fclose( bnf_file ) == EOF )
			/* Fehler */
			return( error( CANNOT_CLOSE_BNF_FILE, 0, 0 ) );
	}

	/* Ist ein Fehler aufgetreten ? */
	if( state < 0 )
		/* Fehler */
		return( -1 );

	/* Zeit holen */
	gettime( &zeit2 );

	/* Zeit in Sekunden umwandeln */
	sec2 = ( zeit2.ti_hour * 3600L ) + ( zeit2.ti_min * 60L ) + zeit2.ti_sec;

	/* Zeit merken */
	fprintf( file, "\n\nBenîtigte Zeit: %02lu:%02lu:%02lu", ( sec2 - sec1 ) / 3600L, ( ( sec2 - sec1 ) % 3600L ) / 60L, ( ( sec2 - sec1 ) % 3600L ) % 60L );

	/* Ausgabe auf Bildschirm ? */
	if( lr_info.dest == DSCREEN )
		/* Auf Tastendruck warten */
		Cnecin();

	/* Datei schliessen */
	if( fclose( file ) < 0 )
		/* Fehler */
		return( error( CANNOT_CLOSE_FILE, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD do_all( BYTE *filename )
{
	/* Hauptroutine */
	BYTE		*temp_file_name;

	/* Dateinamen ausgeben */
	if( ( file_name = output_file_msg( filename ) ) == ( BYTE * ) -1L )
		/* Fehler */
		return( -1 );

	/* Info-Datei lesen */
	if( read_info_file() < 0 )
		/* Fehler */
		return( -1 );

	/* Zeile zurÅcksetzen */
	zeile = 1;

	/* Grammatik-Datei lesen */
	if( read_grammar_file( filename ) < 0 )
		/* Fehler */
		return( -1 );

	/* Die Hash-Tabelle initialisieren */
	if( !( hash_tab = ( HASH_TAB * ) calloc( lr_info.hash_tab_size, sizeof( HASH_TAB ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Ausgaben in Datei bzw. auf den Bildschirm */
	if( lr_info.dest == DFILE )
	{
		/* Dateinamen aufbereiten */
		temp_file_name = strrchr( file_name, ( WORD ) '.' );
		strcpy( temp_file_name, ".TXT" );

		/* Datei zum Schreiben îffnen */
		file = freopen( file_name, "w", 0L );
	}

	/* Ausgabe auf Bildschirm ? */
	else if( lr_info.dest == DSCREEN )
		/* Bildschirm */
		file = stdout;

	else
		/* Drucker */
		file = stdprn;

	/* Grammatik analysieren */
	if( analyze_grammar() < 0 )
		/* Fehler */
		return( -1 );

	/* Speicher der Grammatik-Datei freigeben */
	free_mem( ( VOID * ) &grammar );

	/* Grammatik in BNF ausgeben ? */
	if( !lr_info.output_grammar )
	{
		/* Ja */
		if( output_grammar() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Items generieren */
	if( generate_items() < 0 )
		/* Fehler */
		return( -1 );

	/* Die LR-Tabelle erzeugen */
	if( make_tab() < 0 )
		/* Fehler */
		return( -1 );

	/* FIRST-Mengen ausgeben ? */
	if( !lr_info.first )
	{
		/* Ja */
		if( output_first() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* FOLLOW-Mengen ausgeben ? */
	if( !lr_info.follow )
	{
		/* Ja */
		if( output_follow() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Items ausgeben ? */
	if( !lr_info.lr_items )
	{
		/* Ja */
		if( output_items() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* LR-Tabelle ausgeben ? */
	if( !lr_info.lr_tab )
	{
		/* Ja */
		if( output_lr_tab() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Parser-Quelltext erzeugen */
	if( generate_parser() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD read_grammar_file( BYTE *filename )
{
	/*	Diese Routine lieût die Grammatik-Datei ein. */
	struct ffblk	file_str;

	/* Ist die Datei existent ? */
	if( findfirst( filename, &file_str, 0 ) )
		/* Fehler */
		return( error( GRAMMAR_FILE_NOT_FOUND, 0, 0 ) );

	/* Datei îffnen */
	if( ( handle = open( filename, O_RDONLY ) ) <= 0 )
		/* Fehler */
		return( error( CANNOT_OPEN_GRAMMAR_FILE, 0, 0 ) );

	/* Speicher fÅr Grammatik-Datei anfordern */
	if( !( grammar = ( BYTE * ) malloc( file_str.ff_fsize + 1 ) ) )
		/* Grammatikdatei schlieûen -> Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Grammatik-Datei lesen */
	if( read( handle, ( VOID * ) grammar, file_str.ff_fsize ) != file_str.ff_fsize )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( error( CANNOT_READ_GRAMMAR_FILE, 0, 0 ) );

	/* Null-Byte anhÑngen */
	grammar[file_str.ff_fsize] = 0;

	/* Datei schlieûen */
	if( close( handle ) )
		/* Grammatik-Datei schlieûen und Speicher freigeben -> Fehler */
		return( error( CANNOT_CLOSE_GRAMMAR_FILE, 0, 0 ) );

	/* ZurÅcksetzen */
	handle = -1;

	/* Alles OK */
	return( 0 );
}


WORD read_info_file( VOID )
{
	/*
		Hier wird die INF-Datei gelesen, wenn sie existiert.
		Andernfalls wird die LR_INFO-Struktur mit Default-Werten
		initialisiert.
	*/
	struct ffblk	file_str;

	/* LR_INFO-Struktur initialisieren */
	memset( ( VOID * ) &lr_info, -1, sizeof( LR_INFO ) );

	/* Ist die Datei existent ? */
	if( !findfirst( "LR.INF", &file_str, 0 ) )
	{
		/* Datei îffnen */
		if( ( handle = open( "LR.INF", O_RDONLY ) ) <= 0 )
			/* Fehler */
			return( error( CANNOT_OPEN_INFO_FILE, 0, 0 ) );

		/* Speicher fÅr die Info-Datei anfordern */
		if( !( grammar = ( BYTE * ) malloc( file_str.ff_fsize + 1 ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Info-Datei lesen */
		if( read( handle, ( VOID * ) grammar, file_str.ff_fsize ) != file_str.ff_fsize )
			/* Info-Datei schlieûen */
			return( error( CANNOT_READ_INFO_FILE, 0, 0 ) );

		/* Mit Null-Byte abschlieûen */
		grammar[file_str.ff_fsize] = 0;

		/* Datei schlieûen */
		if( close( handle ) )
			/* Info-Datei schlieûen */
			return( error( CANNOT_CLOSE_INFO_FILE, 0, 0 ) );

		/* ZurÅcksetzen */
		handle = -1;

		/* Info-File analysieren */
		analyze_info_file();

		/* Speicher der Info-Datei freigeben */
		free_mem( ( VOID * ) &grammar );
	}

	/* Ist die Ausgabe angegeben worden ? */
	if( lr_info.dest < 0 )
		/* Ausgabe auf Bildschirm */
		lr_info.dest = DSCREEN;

	/* Sollen die FOLLOW-Mengen ausgegeben werden ? */
	if( lr_info.follow < 0 )
		/* FOLLOW-Mengen nicht erzeugen */
		lr_info.follow = OFF;

	/* Sollen die FIRST-Mengen ausgegeben werden ? */
	if( lr_info.first < 0 )
		/* FIRST-Mengen nicht erzeugen */
		lr_info.first = OFF;

	/* Sollen die LR-Items ausgegeben werden ? */
	if( lr_info.lr_items < 0 )
		/* LR-Items nicht ausgeben */
		lr_info.lr_items = OFF;

	/* Soll die Grammatik anschlieûend ausgegeben werden ? */
	if( lr_info.output_grammar < 0 )
		/* Grammatik nicht ausgeben */
		lr_info.output_grammar = OFF;

	/*
		Sollen Informationen Åber die Grammatik-Analyse ausgegeben
		werden ?
	 */
	if( lr_info.analyze < 0 )
		/* Informationen nicht ausgeben */
		lr_info.analyze = OFF;

	/* Soll die LR-Tabelle komprimiert werden ? */
	if( lr_info.compress < 0 )
		/* LR-Tabelle nicht komprimieren */
		lr_info.compress = OFF;

	/* Soll die LR-Tabelle ausgegeben werden ? */
	if( lr_info.lr_tab < 0 )
		/* LR-Tabelle nicht ausgeben */
		lr_info.lr_tab = OFF;

	/* Ist die Art des Parsers festgelegt worden ? */
	if( lr_info.type < 0 )
		/* LALR-Parser erzeugen */
		lr_info.type = LALR;

	/* Soll die Debug-Version erzeugt werden ? */
	if( lr_info.debug < 0 )
		/* Debug-Version erzeugen */
		lr_info.debug = ON;

	/* Ist die Grîûe der Hash-Tabelle angegeben worden ? */
	if( lr_info.hash_tab_size < 0 )
		lr_info.hash_tab_size = DEFAULT_HASH_TAB_SIZE * sizeof( HASH_TAB );

	/* Ist die max. zulÑssige Anzahl der Zeichen pro Zeile angegeben worden ? */
	if( lr_info.max_char < 0 )
		/* Max. Anzahl der Zeichen festlegen */
		lr_info.max_char = MAX_CHAR;

	/* Alles OK */
	return( 0 );
}


WORD error( WORD error_number, ULONG para1, ULONG para2 )
{
	/* Diese Routine gibt eine Fehlermeldung aus. */

	/* Auf die Fehlermeldungen individuell eingehen */
	switch( error_number )
	{
		case YET_DEFINED_TERM:
		case UNTERMINATED_TERMINAL:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( stderr, error_msgs[error_number], zeile, ( BYTE * ) para1 );

			/* Abbruch */
			break;
		}

		case NOT_DEFINED_PREC_TERM:
		case NOT_DEFINED_NTERM:
		case NOT_USED_NTERM:
		case NOT_USED_TERM:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( stderr, error_msgs[error_number], ( BYTE * ) para1 );

			/* Abbruch */
			break;
		}

		case EBNF_SYM_ERR:
		case INVALID_CHAR:
		case TOO_MANY_EBNF_SYMB:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( stderr, error_msgs[error_number], zeile, ( BYTE ) para1, ( BYTE * ) para2 );

			/* Abbruch */
			break;
		}

		case BAD_ALTERNATIVE:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( stderr, error_msgs[error_number], para1, ( BYTE * ) para2 );

			/* Abbruch */
			break;
		}

		default:
		{
			/* Fehlermeldung ausgeben */
			fprintf( stderr, error_msgs[error_number], zeile );

			/* Abbruch */
			break;
		}
	}

	/* Fehler */
	return( -1 );
}


VOID free_mem( VOID **pointer )
{
	/* Hier wird der Speicher einer Variablen freigegeben. */
	free( *pointer );

	/* Erkennbar machen, daû der Speicher freigegeben wurde */
	*pointer = 0L;
}


BYTE *output_file_msg( BYTE *filename )
{
	/* Den Dateinamen der Grammatikdatei ausgeben. */
	BYTE	*file_name;

	/* Wurde ein Grammatikdatei angegeben ? */
	if( !filename )
		/* Fehler*/
		return( ( BYTE * ) error( NO_GRAMMAR_FILE_NAME, 0, 0 ) );

	/* Grammatikdateinamen extrahieren */
	file_name = strrchr( filename, ( WORD ) '\\' );

	/* Ist kein '\\' im Dateinamen angeben worden ? */
	if( !file_name )
		/* Ja */
		file_name = filename;

	else
		/* '\\' Åbergehen */
		file_name++;

	/* Dateinamen in Grossbuchstaben wandeln */
	file_name = strupr( file_name );

	/* Meldungen ausgeben */
	fprintf( stderr, "\n\nAnalysiere %s\nBitte warten...\n", file_name );

	/* Alles OK */
	return( file_name );
}