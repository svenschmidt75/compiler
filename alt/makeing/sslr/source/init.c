/*
	Projekt		:	LR-Parser-Generator
	Modul		:	INIT.C
	Sprache		:	ANSI-C
	Compiler	:	ATARI: PureC V1.1
*/



/************/
/* Includes */
/************/

/* Include-Datei des Parser-Generators */
#include "LR.H"


/**************/
/* Prototypen */
/**************/

WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD read_grammar_file( BYTE *filename );
WORD output_file_msg( BYTE *filename );
VOID free_mem( VOID **pointer );
WORD analyze_info_file( VOID );
WORD do_all( BYTE *filename );
WORD analyze_grammar( VOID );
WORD compress_lr_tab( VOID );
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

/* Datei bei Fehler schliessen */
#define CLOSE_FILE				1

/* Speicher bei Fehler freigeben */
#define FREE_MEMORY				2

/* Standard-Hash-Tab-Grîsse */
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

/* Ausgabe der Fehler auf FILE */
FILE		*err_file = stderr;

/* Beinhaltet die Grammatik-Datei */
BYTE		*grammar = 0L;

/* Aktuelle Zeile in der Grammatik-Datei */
ULONG		zeile = 1;

/* Hash-Tabelle */
HASH_TAB	*hash_tab = 0L;

/* Dateiname */
BYTE		*file_name = 0L;

/* Flag fÅr Fehlermeldung */
WORD		error_flag = 0;

/* Anzahl der Shift-/Reduce-Fehler */
WORD		sr_err = 0;

/* Anzahl der Reduce-/Reduce-Fehler */
WORD		rr_err = 0;

/* Fehlermeldungen */
BYTE		*error_msgs[] =
{
/* NO_GRAMMAR_FILE_NAME */        "\nEs wurde keine Grammatikdatei angegeben !",
/* GRAMMAR_FILE_NOT_FOUND */      "\nKann Grammatik-Datei nicht finden !",
/* CANNOT_OPEN_GRAMMAR_FILE */    "\nKann Grammatik-Datei nicht îffnen !",
/* CANNOT_READ_GRAMMAR_FILE */    "\nKann Grammatik-Datei nicht lesen !",
/* CANNOT_CLOSE_GRAMMAR_FILE */   "\nKann Grammatik-Datei nicht schliessen !",
/* CANNOT_OPEN_INFO_FILE */       "\nKann Info-Datei nicht îffnen !",
/* CANNOT_READ_INFO_FILE */       "\nKann Info-Datei nicht lesen !",
/* CANNOT_CLOSE_INFO_FILE */      "\nKann Info-Datei nicht schliessen !",
/* CANNOT_CLOSE_FILE */           "\nKann Ausgabe-Datei nicht schliessen !",
/* NOT_ENOUGH_MEMORY */           "\nNicht genug Speicher !",
/* INVALID_CHAR */                "\nZeile %lu: Illegales Zeichen %c !",
/* UNKNOWN_INFO_FILE_STATEMENT */ "\nZeile %lu: Unbekannte Angabe in der Info-Datei !",
/* HASH_TAB_ERROR */              "\nDie Hash-Tabelle ist zu klein !",
/* PREC_ERR */                    "\nZeile %lu: %%prec ist nur in einer Produktion erlaubt !",
/* NOT_USED_TERM */               "\nZeile %lu: Terminal %s wurde nicht benutzt !",
/* PROD_SEP_ERR */                "\nZeile %lu: Seperator fehlerhaft !",
/* PROD_BEGINN */                 "\nZeile %lu: Produktion muss mit einem Nichtterminal beginnen !",
/* PREC_PRIORI */                 "\nZeile %lu: Die PrioritÑt des Terminals ist kleiner als die PrioritÑt"
                                  "\nder Produktion !",
/* MAX_ANZ_RI_ERR */              "\nZeile %lu: Maximal zulÑssige Anzahl an Elementen auf der rechten Seite Åberschritten !",
/* EBNF_SYM_ERR */                "\nZeile %lu: Fehlendes Meta-Symbol %c in Produktion %s !",
/* ERROR_ERR */                   "\nZeile %lu: %%error ist nur in einer Produktionen erlaubt !",
/* LR_ERR */                      "\nZeile %lu: %%left und %%right muessen vor der Grammatik stehen !",
/* TOO_MANY_EBNF_SYMB */          "\nZeile %lu: Meta-Symbol %c in Produktion %s ueberfluessig !",
/* NOT_DEFINED_NTERM */           "\nZeile %lu: Nichtterminal %s wurde nicht definiert !",
/* NOT_USED_NTERM */              "\nZeile %lu: Nichtterminal %s wurde nicht benutzt !",
/* NOT_DEFINED_PREC_TERM */       "\nZu dem Terminal %s wurde keine PrioritÑt angegeben !",
/* YET_DEFINED_TERM */            "\nZeile %lu: Das Nichtterminal %s wurde bereits definiert !",
/* CANNOT_CLOSE_BNF_FILE */       "\nKann BNF-Grammatik-Datei nicht schliessen !",
/* UNTERMINATED_TERMINAL */       "\nZeile %lu: Nicht abgeschlossenes Terminal %s !",
/* BAD_ALTERNATIVE */             "\nUnsinnige Alternative Nr. %d in Produktion %s !",
/* CANNOT_OPEN_H_FILE */          "\nKann Include-Datei des Parser-Quelltextes nicht îffnen !",
/* CANNOT_CLOSE_H_FILE */         "\nKann Include-Datei des Parser-Quelltextes nicht schliessen !",
/* CANNOT_OPEN_PARSER_FILE */     "\nKann Parser-Quelltext-Datei nicht îffnen !",
/* CANNOT_CLOSE_PARSER_FILE */    "\nKann Parser-Quelltext-Datei nicht schliessen !",
/* CANNOT_CLOSE_ERR_FILE */       "\nKann Fehlerdatei nicht schliessen !",
/* UNION_ERR */                   "\nZeile %lu: %%union muss vor der Grammatik stehen !",
/* UNION_DBL */                   "\nZeile %lu: %%union ist schon angegeben worden !",
/* TERM_TYPE_ERR */               "\nZeile %lu: %%type_def muss vor der Grammatik stehen !",
/* NTERM_TYPE_ERR */              "\nZeile %lu: %%ntype_def muss vor der Grammatik stehen !",
/* INIT_ERR */                    "\nZeile %lu: %%init muss vor der Grammatik stehen !",
/* INCL_ERR */                    "\nZeile %lu: %%incl muss vor der Grammatik stehen !",
/* TERM_IS_NTERM */               "\nZeile %lu: %s wurde schon als Terminal definiert !",
/* NTERM_IS_TERM */               "\nZeile %lu: %s wurde schon als Nichtterminal definiert !",
/* INIT_DBL */                    "\nZeile %lu: %%init ist schon angegeben worden !",
/* INCL_DBL */                    "\nZeile %lu: %%incl ist schon angegeben worden !",
/* SEM_ACT_ERR */                 "\nZeile %lu: $x zu gross !",
/* UNKNOWN_ATTRIBUTE */           "\nZeile %lu: Unbekanntes Attribut %s !",
/* MISSING_NTERM_ATTRIBUTE */     "\nZeile %lu: Nichtterminal %s hat kein Attribut !",
/* SX_ATTRIBUTE_ERROR */          "\nZeile %lu: %s hat kein Attribut !",
/* MISSING_UNION_STRUCT */        "\nAttribute ohne UNION-Struktur verwendet !",
/* BAD_SEM_ACT */                 "\nZeile %lu: Semmantische Aktion wird nie ausgefuehrt !",
/* CANNOT_OPEN_BNF_FILE */        "\nKann BNF-Grammatik-Datei nicht îffnen !",
/* ALL_ERR */                     "\nZeile %lu: Nur eine allgemeine Fehlermeldung pro HÅlle erlaubt !",
/* OR_OPERATOR */				  "\nZeile %lu: | erwartet !",
/* PROD_END */					  "\nZeile %lu: Produktion %s wurde nicht mit ; beendet !"
};



/***********/
/* Imports */
/***********/

MGLOBAL GRA_ERR_STR	*gra_err_str;
MGLOBAL NTERM_FIRST	**nterm_first;
MGLOBAL ERROR_STR	**error_struct;
MGLOBAL HULLE_STR	**hulle_str;
MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL TERM_STR	*term_str;
MGLOBAL SEM_ACT		*sem_act;
MGLOBAL LR_TAB		**lr_tab;
MGLOBAL ULONG		nterm_first_count;
MGLOBAL ULONG		*ziel_tab;
MGLOBAL LONG		*prod_index_arr;
MGLOBAL LONG		nterm_count;
MGLOBAL LONG		error_struct_count;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL LONG		sem_act_count;
MGLOBAL LONG		*old_nterm_index;
MGLOBAL LONG		type_arr_count;
MGLOBAL LONG		*error_hulle;
MGLOBAL FILE		*bnf_file;
MGLOBAL FILE		*parser_file;
MGLOBAL BYTE		*init;
MGLOBAL BYTE		*incl;
MGLOBAL BYTE		**type_arr;




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

	/* Den Dateinamen merken */
	file_name = argv[1];

	/* Hauptroutine aufrufen */
	state = do_all( argv[1] );

	/* Traten Shift-/Reduce-Fehler auf ? */
	if( sr_err || rr_err )
		/* Es traten Shift-Reduce-Fehler auf */
		fprintf( stderr, "\n\n%d Shift-/Reduce-Fehler, %d Reduce-/Reduce-Fehler registriert.", sr_err, rr_err );
	
	/* Ausgabe der Fehler in Datei ? */
	if( lr_info.err_dest == DFILE )
	{
		/* Sind Fehler aufgetreten ? */
		if( error_flag )
			/* Meldung ausgeben */
			fprintf( stderr, "\n\nEs sind Fehler aufgetreten !" );

		else
			/* Meldung ausgeben */
			fprintf( stderr, "\n\nEs sind keine Fehler aufgetreten !" );
	}

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
			/* Das Lookahead-Array aller Items freigeben */
			for( j = 0; j < hulle_str[i]->anz_items; j++ )
				/* Speicher freigeben */
				free( ( VOID * ) hulle_str[i]->item_str[j].lookahead.term_arr );

			/* Speicher der ITEM_STR freigeben */
			free( ( VOID * ) hulle_str[i]->item_str );
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
		/* Speicher der ZustandsuebergÑnge freigeben */
		free( ( VOID * ) jump_tab );

	if( ziel_tab )
		/* Speicher der Ziel-Tabelle freigeben */
		free( ( VOID * ) ziel_tab );

	if( nterm_str )
	{
		/* Speicher sÑmtlicher NTERM_STRs zurueckgeben */
		for( i = 1; i <= nterm_count; i++ )
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

	if( prod_index_arr )
		/* Speicher der Indexe der Produktionen freigeben */
		free( ( VOID * ) prod_index_arr );

	if( gra_err_str )
		/* Speicher freigeben */
		free( ( VOID * ) gra_err_str );

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

	if( old_nterm_index )
		/* Speicher freigeben */
		free( ( VOID * ) old_nterm_index );

	if( init )
		/* Speicher freigeben */
		free( ( VOID * ) init );

	if( incl )
		/* Speicher freigeben */
		free( ( VOID * ) incl );

	if( error_hulle )
		/* Speicher freigeben */
		free( ( VOID * ) error_hulle );

	if( type_arr )
	{
		/* Alle EintrÑge freigeben */
		for( i = 0; i < type_arr_count; i++ )
			/* Speicher freigeben */
			free( ( VOID * ) type_arr[i] );

		/* Speicher des Arrays freigeben */
		free( ( VOID * ) type_arr );
	}

	if( sem_act )
	{
		/* Alle EintrÑge freigeben */
		for( i = 0; i < sem_act_count; i++ )
			/* Speicher freigeben */
			free( ( VOID * ) sem_act[i].sem_act_text );

		/* Speicher des Arrays freigeben */
		free( ( VOID * ) sem_act );
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
				free( ( VOID * ) hash_tab[i].hulle_arr );
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

	/* Fehler-Datei schliessen */
	if( fclose( err_file ) < 0 )
		/* Fehler */
		return( error( CANNOT_CLOSE_ERR_FILE, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD do_all( BYTE *filename )
{
	/* Hauptroutine */
	BYTE	*temp_file_name;

	/* Info-Datei lesen */
	if( read_info_file() < 0 )
		/* Fehler */
		return( -1 );

	/* Zeile zuruecksetzen */
	zeile = 1;

	/* Grammatik-Datei lesen */
	if( read_grammar_file( filename ) < 0 )
		/* Fehler */
		return( -1 );

	/* Die Hash-Tabelle initialisieren */
	if( ( hash_tab = ( HASH_TAB * ) calloc( lr_info.hash_tab_size, sizeof( HASH_TAB ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Dateinamen ausgeben */
	if( output_file_msg( filename ) < 0 )
		/* Fehler */
		return( -1 );

	/* Ausgaben in Datei bzw. auf den Bildschirm */
	if( lr_info.dest == DFILE )
	{
		/* Dateinamen aufbereiten */
		temp_file_name = strrchr( filename, ( WORD ) '.' );
		strcpy( temp_file_name, ".TXT" );

		/* Datei zum Schreiben îffnen */
		file = freopen( filename, "w", 0L );
	}

	/* Ausgabe auf Bildschirm ? */
	else if( lr_info.dest == DSCREEN )
		/* Bildschirm */
		file = stdout;

	else
		/* Drucker */
		file = stdprn;

	/* Ausgabe der Fehler auf Bildschirm ? */
	if( lr_info.err_dest == DSCREEN )
		/* Bildschirm */
		err_file = stderr;

	else
		/* Drucker */
		err_file = stdprn;

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

	/* LR-Tabelle optional komprimieren */
	if( !lr_info.compress )
	{
		/* LR-Tabelle komprimieren */
		if( compress_lr_tab() < 0 )
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
	/*	Diese Routine liesst die Grammatik-Datei ein. */
	struct ffblk	file_str;

	/* Ist die Datei existent ? */
	if( findfirst( filename, &file_str, 0 ) < 0 )
		/* Fehler */
		return( error( GRAMMAR_FILE_NOT_FOUND, 0, 0 ) );

	/* Datei îffnen */
	if( ( handle = open( filename, O_RDONLY ) ) < 0 )
		/* Fehler */
		return( error( CANNOT_OPEN_GRAMMAR_FILE, 0, 0 ) );

	/* Speicher fÅr Grammatik-Datei anfordern */
	if( ( grammar = ( BYTE * ) malloc( file_str.ff_fsize + 1 ) ) == NULL )
		/* Grammatikdatei schliessen -> Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Grammatik-Datei lesen */
	if( read( handle, ( VOID * ) grammar, file_str.ff_fsize ) < 0 )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( error( CANNOT_READ_GRAMMAR_FILE, 0, 0 ) );

	/* Null-Byte anhÑngen */
	grammar[file_str.ff_fsize] = 0;

	/* Datei schliessen */
	if( close( handle ) < 0 )
		/* Grammatik-Datei schliessen und Speicher freigeben -> Fehler */
		return( error( CANNOT_CLOSE_GRAMMAR_FILE, 0, 0 ) );

	/* Zuruecksetzen */
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
	BYTE			*temp_file;

	/* LR_INFO-Struktur initialisieren */
	memset( ( VOID * ) &lr_info, -1, sizeof( LR_INFO ) );

	/* Ist die Datei existent ? */
	if( !findfirst( "SSLR.INF", &file_str, 0 ) )
	{
		/* Datei îffnen */
		if( ( handle = open( "SSLR.INF", O_RDONLY ) ) < 0 )
			/* Fehler */
			return( error( CANNOT_OPEN_INFO_FILE, 0, 0 ) );

		/* Speicher fÅr die Info-Datei anfordern */
		if( ( grammar = temp_file = ( BYTE * ) malloc( file_str.ff_fsize + 1 ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Info-Datei lesen */
		if( read( handle, ( VOID * ) grammar, file_str.ff_fsize ) < 0 )
			/* Info-Datei schliessen */
			return( error( CANNOT_READ_INFO_FILE, 0, 0 ) );

		/* Mit Null-Byte abschliessen */
		grammar[file_str.ff_fsize] = 0;

		/* Datei schliessen */
		if( close( handle ) < 0 )
			/* Info-Datei schliessen */
			return( error( CANNOT_CLOSE_INFO_FILE, 0, 0 ) );

		/* Zuruecksetzen */
		handle = -1;

		/* Info-File analysieren */
		analyze_info_file();

		/* Speicher der Info-Datei freigeben */
		free_mem( ( VOID * ) &temp_file );
	}

	/* Ist die Ausgabe angegeben worden ? */
	if( lr_info.dest < 0 )
		/* Ausgabe auf Bildschirm */
		lr_info.dest = DSCREEN;

	/* Ist die Ausgabe der Fehler angegeben worden ? */
	if( lr_info.err_dest < 0 )
		/* Ausgabe der Fehler auf Bildschirm */
		lr_info.err_dest = DSCREEN;

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

	/* Soll die Grammatik anschliessend ausgegeben werden ? */
	if( lr_info.output_grammar < 0 )
		/* Grammatik nicht ausgeben */
		lr_info.output_grammar = OFF;

	/*
		Sollen Informationen ueber die Grammatik-Analyse ausgegeben
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

	/* Ist die Grîsse der Hash-Tabelle angegeben worden ? */
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
	BYTE	*temp_file_name;

	/* Sollen die Fehler in eine Datei ausgegeben werden ? */
	if( ( !error_flag ) && ( lr_info.err_dest == DFILE ) )
	{
		/* Wurde Åberhaupt eine Grammatik-Datei angegeben ? */
		if( file_name )
		{
			/* Dateinamen aufbereiten */
			temp_file_name = strrchr( file_name, ( WORD ) '.' );
			strcpy( temp_file_name, ".ERR" );

			/* Datei zum Schreiben îffnen */
			err_file = freopen( file_name, "w", 0L );
		}

		else
			/* Nein, auf dem Bildschirm ausgeben */
			err_file = stderr;
	}

	/* Auf die Fehlermeldungen individuell eingehen */
	switch( error_number )
	{
		case YET_DEFINED_TERM:
		case UNTERMINATED_TERMINAL:
		case NTERM_IS_TERM:
		case TERM_IS_NTERM:
		case PROD_END:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( err_file, error_msgs[error_number], zeile, ( BYTE * ) para1 );

			/* Abbruch */
			break;
		}

		case NOT_DEFINED_PREC_TERM:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( err_file, error_msgs[error_number], ( BYTE * ) para1 );

			/* Abbruch */
			break;
		}

		case EBNF_SYM_ERR:
		case INVALID_CHAR:
		case TOO_MANY_EBNF_SYMB:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( err_file, error_msgs[error_number], zeile, ( BYTE ) para1, ( BYTE * ) para2 );

			/* Abbruch */
			break;
		}

		case NOT_USED_NTERM:
		case NOT_USED_TERM:
		case NOT_DEFINED_NTERM:
		case BAD_ALTERNATIVE:
		case UNKNOWN_ATTRIBUTE:
		case MISSING_NTERM_ATTRIBUTE:
		case BAD_SEM_ACT:
		case SX_ATTRIBUTE_ERROR:
		case ALL_ERR:
		{
			/* Fehlermeldung mit Parameter ausgeben */
			fprintf( err_file, error_msgs[error_number], para1, ( BYTE * ) para2 );

			/* Abbruch */
			break;
		}

		default:
		{
			/* Fehlermeldung ausgeben */
			fprintf( err_file, error_msgs[error_number], zeile );

			/* Abbruch */
			break;
		}
	}

	/* Fehlerflag setzen */
	error_flag++;

	/* Fehler */
	return( -1 );
}


VOID free_mem( VOID **pointer )
{
	/* Hier wird der Speicher einer Variablen freigegeben. */
	free( *pointer );

	/* Erkennbar machen, dass der Speicher freigegeben wurde */
	*pointer = 0L;
}


WORD output_file_msg( BYTE *filename )
{
	/* Den Dateinamen der Grammatikdatei ausgeben. */
	BYTE	*file_name;

	/* Wurde ein Grammatikdatei angegeben ? */
	if( !filename )
		/* Fehler*/
		return( error( NO_GRAMMAR_FILE_NAME, 0, 0 ) );

	/* Grammatikdateinamen extrahieren */
	file_name = strrchr( filename, ( WORD ) '\\' );

	/* Ist kein '\\' im Dateinamen angeben worden ? */
	if( !file_name )
		/* Ja */
		file_name = filename;

	else
		/* '\\' uebergehen */
		file_name++;

	/* Dateinamen in Grossbuchstaben wandeln */
	file_name = strupr( file_name );

	/* Meldungen ausgeben */
	fprintf( stderr, "\n\nAnalysiere %s\nBitte warten...\n", file_name );

	/* Alles OK */
	return( 0 );
}