/*
	Projekt		:	LR-Parser-Generator
	Modul		:	GEN_PARS.C
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

VOID output_text( BYTE *string, BYTE *new_line, WORD format );
WORD cmp_error_index( HULLE_STR *hulle1, HULLE_STR *hulle2 );
WORD error( WORD error_number, ULONG para1, ULONG para2  );
VOID output_error( BYTE *delim, ERROR_STR *error_str );
VOID output_right( FILE *file, LONG nterm_index );
WORD output_debug( BYTE *string, FILE *file );
WORD generate_source_parser( VOID );
VOID output_reduce_struct( VOID );
VOID output_index_array( VOID );
VOID output_action_tab( VOID );
VOID output_follow_str( VOID );
WORD generate_sem_act( VOID );
VOID output_ziel_tab( VOID );
VOID output_jump_tab( VOID );
WORD generate_parser( VOID );
WORD generate_h_file( VOID );
VOID output_union( VOID );
VOID output_init( VOID );
VOID output_incl( VOID );


/*****************/
/* Deklarationen */
/*****************/

typedef struct
{
	/* Zustand, bis zu dem der vorherige Korrekturwert gilt */
	LONG	state;

	/* Neuer Korrekturwert */
	LONG	fix_val;

}FIX_STR;




/****************/
/* Definitionen */
/****************/

/* Parser-Source generieren/nicht generieren */
#undef DO_NOT_MAKE_PARSER

/* Ausgabe in Datei */
FILE	*parser_file = 0L;

/* Ausgaben in H-File */
FILE	*h_file;

/* Name der Include-Datei des Parsers */
BYTE	file_h_name[256];

/* Name der Parser-Quelltext-Datei */
BYTE	file_parser_name[256];

/* Anzahl der Zeichen in einer Zeile */
WORD	max_char = 0;

/* StringlÑnge der Anzahl der ZustÑnde */
WORD	state_length;

/* Parsertyp */
BYTE	*type[3] = { "LR  ", "LALR", "SLR " };

/* Puffer fÅr Ausgaben */
BYTE	parser_puffer[8 * 1024];


/***********/
/* Imports */
/***********/

MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL ERROR_STR	**error_struct;
MGLOBAL HULLE_STR	**hulle_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL TERM_STR	*term_str;
MGLOBAL LR_INFO		lr_info;
MGLOBAL LR_TAB		**lr_tab;
MGLOBAL ULONG		prod_count;
MGLOBAL ULONG		*ziel_tab;
MGLOBAL ULONG		main_dbl_count;
MGLOBAL ULONG		nterm_length;
MGLOBAL LONG		*compressed_lr_index_tab;
MGLOBAL LONG		*prod_index_arr;
MGLOBAL LONG		term_count;
MGLOBAL LONG		nterm_count;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		epsilon;
MGLOBAL BYTE		**sem_act;
MGLOBAL BYTE		*file_name;
MGLOBAL BYTE		*union_struct;
MGLOBAL BYTE		*init;
MGLOBAL BYTE		*incl;




WORD generate_parser( VOID )
{
	/* Hier wird der Quelltext des Parsers erzeugt. */

	/* Meldung ausgeben */
	fprintf( stderr, "\nErzeuge den Parser-Source-Code..." );

	/* Parser-Source generieren ? */
	#ifndef DO_NOT_MAKE_PARSER

		/* Include-Datei des Parser-Quelltextes erzeugen */
		if( generate_h_file() < 0 )
			/* Fehler */
			return( -1 );

		/* Parser-Quelltext erzeugen */
		if( generate_source_parser() < 0 )
			/* Fehler */
			return( -1 );

	#endif

	/* Alles OK */
	return( 0 );
}


WORD generate_h_file( VOID )
{
	/* Hier wird das Include-File fÅr den erzeugten Parser erzeugt. */
	NTERM_STR	*nterm;
	REG	ULONG	i = 0;
	BYTE		*temp_file_name;

	/* Backslash in Grammatikdateinamen suchen */
	temp_file_name = strrchr( file_name, ( WORD ) '.' );

	/* Extension erzeugen */
	temp_file_name++;
	*temp_file_name++ = 'H';
	*temp_file_name = 0;

	/* Datei îffnen */
	if( ( h_file = fopen( file_name, "w" ) ) == NULL )
		/* Fehler */
		return( error( CANNOT_OPEN_H_FILE, 0, 0 ) );

	/* Backslash in Grammatikdateinamen suchen */
	temp_file_name = strrchr( file_name, ( WORD ) '\\' );

	/* War gar kein Backslash im Dateinamen ? */
	if( !temp_file_name )
		/* Dateinamen benutzen */
		temp_file_name = file_name;

	else
		/* Backslash lîschen */
		temp_file_name++;

	/* Dateinamen kopieren */
	strcpy( file_h_name, temp_file_name );

	/* Header schreiben */
	fputs( "/*", h_file );
	fputs( INFO, h_file );

	/* Debug-Version ? */
	if( output_debug( "\n\nDebug-Version\n*/\n\n", h_file ) < 0 )
		/* Nein */
		fputs( "\n*/\n\n", h_file );

	/* Alle Terminale ausgeben */
	fputs( "\n/*************/\n/* Terminale */\n/*************/\n\n", h_file );

	/* Alle Terminale */
	for( i = 0; i <= term_count; i++ )
		/* Terminal ausgeben */
		fprintf( h_file, "\n#define %s\t\t\t%lu", hash_tab[term_str[i].hash_tab_index].name, i );

	/* Produktionen inclusive der rechten Seiten ausgeben */
	fputs( "\n\n\n/****************/\n/* Produktionen */\n/****************/\n\n", h_file );

	/* Alle Produktionen ausgeben */
	for( i = 0; i <= nterm_count; i++ )
	{
		/* Zeiger auf NTERM_STR holen */
		nterm = nterm_str[i];

		/* Hat die Produktion Alternativen ? */
		if( !( nterm->alt ) && ( ( ( i + 1 ) < nterm_count ) && ( !( nterm_str[i + 1]->alt ) ) ) )
			/* Nein, Produktion ausgeben */
			fprintf( h_file, "#define %s\t\t\t%lu\t/* ", hash_tab[nterm->hash_tab_index].name, i + 1 );

		else
		{
			/* Ja */
			sprintf( file_parser_name, "%s_%lu", hash_tab[nterm->hash_tab_index].name, i - ( i - nterm->alt ) );
			fprintf( h_file, "#define %s\t\t\t%lu\t/* ", file_parser_name, i + 1 );
		}

		/* Rechte Seite ausgeben */
		output_right( h_file, i );

		/* Kommentar schliessen */
		fputs( " */\n", h_file );
	}

	/* Anlauf-Produktionen nach Reduktion ausgeben */
	fputs(	 "\n\n\n/**************************************/\n"
			 "/* Anlauf-Produktionen nach Reduktion */\n"
			 "/**************************************/\n\n", h_file );

	/* Alle Produktionen ohne Alternativen */
	for( i = 1; i <= nterm_count; i++ )
	{
		/* Zeiger auf NTERM_STR holen */
		nterm = nterm_str[i];

		/* Ist es eine Alternative ? */
		if( !nterm->alt )
		{
			/* Nein, Produktion in Grossschrift wandeln */
			strcpy( file_parser_name, hash_tab[nterm->hash_tab_index].name );
			fprintf( h_file, "\n#define %s %lu", strupr( file_parser_name ), nterm->prod_index );
		}
	}

	/* Die Union-Struktur ausgeben */
	output_union();

	/* Funktionsprototypen ausgeben */
	fputs(	"\n\n/* Funktionsprototypen */"
			"\nMGLOBAL WORD scanner( VOID );"
			"\nMGLOBAL WORD parse( VOID );", h_file );

	/* Datei schliessen */
	if( fclose( h_file ) == EOF )
		/* Fehler */
		return( error( CANNOT_CLOSE_H_FILE, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD generate_source_parser( VOID )
{
	/* Hier wird der Quelltext zum erzeugten Parser erzeugt. */
	struct time	zeit;
	struct date	datum;
	HULLE_STR	*hulle;
	ERROR_STR	**error_str, *temp_error_str;
	REG	ULONG	i = 0, k, p;
	WORD		anz_error_str, output_term = 0, all_err;
	BYTE		*temp_file_name;

	/* Backslash in Grammatikdateinamen suchen */
	temp_file_name = strrchr( file_name, ( WORD ) '.' );

	/* Extension erzeugen */
	temp_file_name++;
	*temp_file_name++ = 'C';
	*temp_file_name = 0;

	/* Datei îffnen */
	if( ( parser_file = fopen( file_name, "w" ) ) == NULL )
		/* Fehler */
		return( error( CANNOT_OPEN_PARSER_FILE, 0, 0 ) );

	/* Grîsseren Puffer festlegen */
	setbuf( parser_file, ( VOID * ) parser_puffer );

	/* Header schreiben */
	fputs( "/*", parser_file );
	fputs( INFO, parser_file );
	fprintf( parser_file, "\n\nParsertyp     : %s", type[lr_info.type] );

	/* Das Datum ausgeben */
	getdate( &datum );
	fprintf( parser_file, "\nDatum         : %02d.%02d.%d", datum.da_day, datum.da_mon, datum.da_year );

	/* Die Zeit ausgeben */
	gettime( &zeit );
	fprintf( parser_file, "\nZeit          : %02d:%02d", zeit.ti_hour, zeit.ti_min );

	/* Debug-Version */
	fprintf( parser_file, "\nDebug-Version : %s", lr_info.debug == 0 ? "Ja" : "Nein" );

	/* Komprimiert */
	fprintf( parser_file, "\nKomprimiert   : %s", lr_info.compress == 0 ? "Ja" : "Nein" );

	/* Komrimiert ? */
	if( !lr_info.compress )
	{
		/* Ja, wurden ZustÑnde entfernt ? */
		if( main_dbl_count )
			/* Ja, ausgeben */
			fprintf( parser_file, " ( minus %lu ZustÑnde )", main_dbl_count );

		else
			/* Nein */
			fprintf( parser_file, " ( Es gab keine doppelten ZustÑnde ! )" );
	}

	/* Includes aufnehmen */
	fputs(	"\n*/\n\n\n/************/\n/* Includes */"
			"\n/************/\n\n#include <portab.h>"
			"\n#include <stdio.h>\n#include <stdlib.h>", parser_file );

	/* Wenn Debug-Version */
	output_debug( "\n#include <tos.h>", parser_file );

	/* Includedatei des Parser-Generators aufnehmen */
	fprintf( parser_file, "\n#include \"%s\"", file_h_name );

	/* Definitionen ausgeben */
	fputs(	"\n\n\n\n/****************/\n/* Definitionen */"
			"\n/****************/\n\n/* Grîsse des Stacks */"
			"\n#define STACK_SIZE\t100\n\n\n/* Anzahl der Terminale */", parser_file );

	/* Anzahl der Terminale ausgeben */
	fprintf( parser_file, "\n#define ANZ_TERM\t%ld", term_count + 1 );

	/* fÅr Fehler */
	output_debug( "\n#define OK\t\t\t0\n#define ERROR\t\t1", parser_file );

	/* Ausgabemedium */
	fputs(	"\n\n\n/* Ausgabemedien */\n#define DEST_SCREEN\t0"
			"\t/* Ausgabe auf Bildschirm */\n#define DEST_FILE\t1"
			"\t/* Ausgabe in Datei */\n#define DEST_PRN\t2"
			"\t/* Ausgabe auf Drucker */", parser_file );

	/* Prototypen ausgeben */
	fputs( "\n\n\n\n/**************/\n/* Prototypen */\n/**************/\n", parser_file );

	/* Wenn Debug-Version */
	output_debug( "\nVOID shift( LONG shift_state, LONG old_lahead, LONG lahead );"
				  "\nVOID output_prod( LONG prod_index );", parser_file );

	/* Alle weiteren Prototypen ausgeben */
	fputs(	"\nVOID reduce( LONG prod_index );"
			"\nVOID push( LONG value );"
			"\nWORD do_parse( VOID );"
			"\nWORD error( VOID );"
			"\nLONG pop( VOID );", parser_file );

	/* Deklarationen ausgeben */
	fputs(	"\n\n\n\n/*****************/"
			"\n/* Deklarationen */"
			"\n/*****************/"
			"\n\n/* Lookahead */"
			"\nWORD\tlookahead;"
			"\n\n/* Aktueller Status */"
			"\nLONG\tstate;"
			"\n\n/* Stackpointer */"
			"\nLONG\tstackpointer = -1;"
			"\n\n/* Stack */"
			"\nLONG\tstack[STACK_SIZE];"
			"\n\n/* Anzahl der Fehlermeldungen */"
			"\nWORD\tanz_error = 0;"
			"\n\n/* Flag fÅr Fehlermeldungen */"
			"\nWORD\terror_flg = 0;", parser_file );

	/* Wenn Debug-Version */
	output_debug(	"\n\n/* Altes Lookahead */"
					"\nWORD\told_lookahead = - 1;"
					"\n\n/* Alter Status */"
					"\nLONG\told_state;", parser_file );

	/* fÅr Ausgabe */
	fputs(	"\n\n/* Ausgabemedium festlegen */"
			"\nWORD\tdest_state = DEST_SCREEN;"
			"\n\n/* fÅr Log */"
			"\nFILE\t*output_file;", parser_file );

	/* Sind semmantische Aktionen gefragt ? */
	if( sem_act )
	{
		/* Speicherdefinition fÅr Stack etc. ausgeben */
		fputs(	"\n\n/* Der Stack fÅr die semmantischen Aktionen */\nLR_TYPE\tlr_sem_stack[100];\n"
				"\n/* Stack-Conter */\nWORD\tlr_sem_count = 0;\n"
				"\n/* Variable, die vom Scanner zurueckgeliefert wird */\nLR_TYPE\tscan_val;\n"
				"\n/* fÅr die interne Verwaltung */\nLR_TYPE\tlr_val;\n"
				"\n/* Attribut-Stack */\nLONG\ta_stack[10];\n"
				"\n/* Keine semmantischen Aktionen im Fehlerfall */\nWORD\tsem_flg = 0;\n", parser_file );
	}

	/* Initialisierung ausgeben */
	output_init();

	/* LR-Tabelle ausgeben */
	output_action_tab();

	/* Jump-Tabelle ausgeben */
	output_jump_tab();

	/* FOLLOW-Mengen ausgeben */
	output_follow_str();

	/* Ziel-Tabelle ausgeben */
	output_ziel_tab();

	/* ZusÑtzliche Struktur ausgeben */
	output_reduce_struct();

	/*
		Falls die LR-Tabelle komprimiert werden soll, das Array mit den
		Indexen ausgeben.
	*/
	if( !lr_info.compress )
		/* Ausgaben */
		output_index_array();

	/* Einen Teil des Parser-Sources ausgeben */
	fputs(	"\n\nWORD error( VOID )\n{\n\t/* Hier werden die Fehler behandlet."
			" */\n\tREG WORD\ti, j, p;\n\tLONG\t\tshift_state;\n\n\t/* Fehlermeldung "
			"ausgeben ? */\n\tif( !error_flg )\n\t{\n\t\t/* Fehlerflag setzen */"
			"\n\t\tanz_error++;\n\n\t\t/* Fehlerlawine verhindern */\n\t\terror_flg++;\n", parser_file );

	/* Werden semmantische Aktionen verwendet ? */
	if( sem_act )
		/* Ja, die semmantischen Aktionen im Fehlerfall unterbinden */
		fputs( "\n\t\t/* Keine semmantischen Aktionen ausfuehren */\n\t\tsem_flg++;\n", parser_file );

	/* Gegebenenfalls ein paar Fehlermeldungen in die error-Routine einbringen */
	if( error_struct )
	{
		/* Switch ausgeben */
		fputs( "\n\t\tswitch( ( WORD ) state )\n\t\t{", parser_file );

		/* Alle HÅllen */
		for( i = 0; i <= hulle_count; i++ )
		{
			/* Gibt es zu der HÅlle Fehlermeldungen ? */
			if( hulle_str[i]->anz_error_index )
			{
				/* Zeiger auf HÅlle holen */
				hulle = hulle_str[i];

				/* case ausgeben */
				fprintf( parser_file, "\n\t\t\tcase %lu:\n", i );

				/* Cases der HÅllen mit gleichen Fehlermeldungen ausgeben */
				for( k = i + 1; k <= hulle_count; k++ )
				{
					/* Sind die Fehlermeldungen gleich ? */
					if( ( hulle->anz_error_index == hulle_str[k]->anz_error_index ) && !cmp_error_index( hulle, hulle_str[k] ) )
					{
						/* Ja, dazugehîriges case ausgeben */
						fprintf( parser_file, "\t\t\tcase %lu:\n", k );

						/* Speicher fÅr Fehlermeldungen freigeben */
						free( ( VOID * ) hulle_str[k]->error_index );

						/* Markieren */
						hulle_str[k]->anz_error_index = 0;
					}
				}

				/* Meldung ausgeben */
				fputs( "\t\t\t{\n", parser_file );

				/* Zuruecksetzen */
				error_str = 0L;
				anz_error_str = 0;

				/* Alle Fehlermeldungen */
				for( p = 0; p < hulle->anz_error_index; p++ )
				{
					/* Zeiger auf ERROR_STR holen */
					temp_error_str = error_struct[hulle->error_index[p] - 1];

					while( temp_error_str )
					{
						/* Speicher fÅr einen Eintrag anfordern */
						if( ( error_str = ( ERROR_STR ** ) realloc( ( VOID * ) error_str, sizeof( ERROR_STR * ) * ( anz_error_str + 1 ) ) ) == NULL )
							/* Fehler */
							return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

						/* error_str eintragen */
						error_str[anz_error_str++] = temp_error_str;

						/* Zeiger auf nÑchste ERROR_STR holen */
						temp_error_str = temp_error_str->error_str;
					}
				}

				/* ZÑhler fÅr allgemeine Fehlermeldungen */
				all_err = 0;

				/* Nach einer General-Fehlermeldung suchen */
				for( p = 0; p < anz_error_str; p++ )
				{
					/* General-Fehlermeldung ? */
					if( !( error_str[p]->term ) )
						/* Ja, merken */
						all_err++;
				}

				/* Wurden mehrere allgemeine Fehlermeldungen angegeben ? */
				if( all_err > 1 )
					/* Ja, Fehlermeldung ausgeben */
					error( ALL_ERR, error_str[anz_error_str-1]->zeile, 0 );

				/* Die Fehlermeldungen mit einem Terminal vor allen anderen. */
				for( p = 0, k = 0; p < ( anz_error_str - 1 ); p++ )
				{
					/* Hat die Fehlermeldung ein Terminal ? */
					if( error_str[p]->term )
					{
						/* Ja, an Position i */
						temp_error_str = error_str[k];
						error_str[k++] = error_str[p];
						error_str[p] = temp_error_str;
					}
				}

				/* Die Fehlermeldungen ausgeben */
				for( p = 0; p < anz_error_str; p++ )
				{
					/* Wurde ein Terminal angegeben ? */
					if( error_str[p]->term )
					{
						/* Ja, entsprechend merken */
						output_term++;

						/* Ist es die erste Fehlermeldung ? */
						if( p )
						{
							/* Nein, einen Block ausgeben ? */
							if( error_str[p]->type[0] )
							{
								/* Ja */
								fprintf( parser_file,	"\t\t\t\telse if( lookahead == %s )\n\t\t\t\t{\n"
														"\t\t\t\t\tfprintf( output_file, %s );\n\n", hash_tab[error_str[p]->term].name, error_str[p]->err_msg );

								/* Fehlerbeeinflussung ausgeben */
								output_error( "\t\t\t\t\t", error_str[p] );

								/* Block schliessen */
								fputs( "\t\t\t\t}\n\n", parser_file );
							}

							else
								/* Nein */
								fprintf( parser_file,	"\t\t\t\telse if( lookahead == %s )\n"
														"\t\t\t\t\tfprintf( output_file, %s );\n\n", hash_tab[error_str[p]->term].name, error_str[p]->err_msg );
						}

						else
						{
							/* Ja, einen Block ausgeben ? */
							if( error_str[p]->type[0] )
							{
								/* Ja */
								fprintf( parser_file,	"\t\t\t\tif( lookahead == %s )\n\t\t\t\t{\n"
														"\t\t\t\t\tfprintf( output_file, %s );\n\n", hash_tab[error_str[p]->term].name, error_str[p]->err_msg );

								/* Fehlerbeeinflussung ausgeben */
								output_error( "\t\t\t\t\t", error_str[p] );

								/* Block schliessen */
								fputs( "\t\t\t\t}\n\n", parser_file );
							}

							else
								/* Nein */
								fprintf( parser_file,	"\t\t\t\tif( lookahead == %s )\n"
														"\t\t\t\t\tfprintf( output_file, %s );\n\n", hash_tab[error_str[p]->term].name, error_str[p]->err_msg );
						}
					}

					else
					{
						/* Nein, es wurde kein Terminal angegeben */

						/* Mit else ausgeben ? */
						if( output_term )
							/* Ja */
							fprintf( parser_file, "\t\t\t\telse\n\t\t\t\t\tfprintf( output_file, %s );\n\n", error_str[p]->err_msg );

						else
							/* Nein */
							fprintf( parser_file, "\t\t\t\tfprintf( output_file, %s );\n\n", error_str[p]->err_msg );

						/* Fehlerbeeinflussung ? */
						if( error_str[p]->type[0] )
							/* Ja, ausgeben */
							output_error( "\t\t\t\t", error_str[p] );
					}
				}

				/* Flag zuruecksetzen */
				output_term = 0;

				/* Case abschliessen */
				fputs( "\t\t\t\tbreak;\n\t\t\t}\n", parser_file );

				/* Speicher fÅr Fehlermeldungen freigeben */
				free( ( VOID * ) hulle->error_index );

				/* Speicher freigeben */
				free( ( VOID * ) error_str );
			}
		}

		/* Default einfuegen */
		fputs(	"\n\t\t\tdefault:\n\t\t\t\tfprintf( output_file, \"\\n\\nFehler in Zustand %ld !\", state );\n"
				"\t\t}\n\t}\n\n", parser_file );
	}

	else
		/* Keine Fehlermeldungen */
		fputs( "\t}\n\n", parser_file );

	/* Die Debug-Version erzeugen ? */
	if( !lr_info.debug )
	{
		/* Debug-Version erzeugen, LR-Tabelle komprimiert ? */
		if( !lr_info.compress )
		{
			/* Ja */
			fputs(

"\tdo\n\t{\n\t\t/* Alle Terminale */\n\t\tfor( i = 0; i < ANZ_TERM; i++ )\n\t\t{\n\t\t\t/"
"* Shift von i mîglich ? */\n\t\t\tif( ( shift_state = lr_tab[c_lr["
"state]][i] ) < 0 )\n\t\t\t{\n\t\t\t\t/* Shift von lookahead mîglich ? */"
"\n\t\t\t\tif( lr_tab[c_lr[~shift_state]][lookahead] < 0 )\n\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t/* i schieben */\n\t\t\t\t\tshift( lr_tab[c_lr[state]][i], old_lookahead, i"
" );\n\n\t\t\t\t\t/* Geschobenes Terminal merken */\n\t\t\t\t\told_lookahead "
"= i;\n\n\t\t\t\t\t/* Alles OK */\n\t\t\t\t\treturn( 0 );\n\t\t\t\t}\n\n\t\t\t\t/* Reduziere-Zu"
"stand ? */\n\t\t\t\telse if( lr_tab[c_lr[~shift_state]][lookahead] > 0 )\n\t"
"\t\t\t{\n\t\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t\t/* Ist lookahead in FOLL"
"OW( lr_tab[c_lr[~shift_state]][i] ) ? */\n\t\t\t\t\tj = 0;\n\n\t\t\t\t\t/* Die pass"
"ende FOLLOW-Struktur holen */\n\t\t\t\t\tdo\n\t\t\t\t\t{\n\t\t\t\t\t\t/* Ist es die"
" richtige ? */\n\t\t\t\t\t\tif( ( follow[j].beginn <= ( lr_tab[c_lr[~shift_"
"state]][i] - 1 ) ) && ( follow[j].end >= ( lr_tab[c_lr[~shift_state]]"
"[i] - 1 ) ) )\n\t\t\t\t\t\t\t/* Ja */\n\t\t\t\t\t\t\tbreak;\n\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\t/*"
" Nein */\n\t\t\t\t\t\t\tj++;\n\n\t\t\t\t\t}while( 1 );\n\n\t\t\t\t\t/* Alle FOLLOW-Ele"
"mente */\n\t\t\t\t\tfor( p = 0; p < follow[j].anz_follow_elem; p++ )"
"\n\t\t\t\t\t{\n\t\t\t\t\t\t/* Ist es lookahead ? */\n\t\t\t\t\t\tif( follow[j].follo"
"w[p] == lookahead )\n\t\t\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t\t\t/* i schieben */\n\t\t\t\t\t\t\tshift"
"( lr_tab[c_lr[state]][i], old_lookahead, i );\n\n\t\t\t\t\t\t\t/* Gesch"
"obenes Terminal merken */\n\t\t\t\t\t\t\told_lookahead = i;\n\n\t\t\t\t\t\t\t/* "
"Alles OK */\n\t\t\t\t\t\t\treturn( 0 );\n"
"\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\n\t\t/* Konnte ein Terminal einge"
"schoben werden ? */\n\t\tif( i == ANZ_TERM )\n\t\t{\n\t\t\t/* Ist es ACCEPT ? */\n\t\t\tif"
"( lookahead == ACCEPT )\n\t\t\t\t/* Ja, Parsing abbrechen */\n\t\t\t\treturn( -1 );\n\n"
"\t\t\t/* Nein, Meldung ausgeben */\n\t\t\tfprintf( output_file, \"\\n\\nLîsche Termina"
"l %s !\", term_name[lookahead] );\n\n\t\t\t/* Lookahead lîschen */\n"
"\t\t\tlookahead = scanner();\n\t\t}\n\n\t}while( 1 );\n}\n\n\n\nWORD parse( VOID )"
"\n{\n\tWORD\tstate;\n\n\t/* Ausgabemedium festlegen */\n\tswitch( dest_state )\n\t{\n\t\t"
"case DEST_PRN:\n\t\t{\n\t\t\t/* Ausgabe auf Drucker */\n\t\t\toutput_fil"
"e = stdprn;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_SCREEN:\n\t\t{\n\t\t\t/* Ausg"
"abe auf Bildschirm */\n\t\t\toutput_file = stdout;\n\t\t\tbreak;\n\t\t}\n"
"\n\t\tcase DEST_FILE:\n\t\t{\n\t\t\t/* Ausgabe in Datei */\n\n\t\t\t/* Datei"
" zum Schreiben îffnen */\n\t\t\toutput_file = freopen( \"PARSER.LO"
"G\", \"w\", stdout );\n\t\t}\n\t}\n\n\t/* Stack initialisieren */\n\tpush("
" 0 );\n\n\t/* Eingabesymbol holen */\n\tlookahead = scanner();\n\n\t/"
"* Parsen */\n\tstate = do_parse();\n\n\t/* Ausgabe auf Datei ? */\n\tif( dest_state == DEST_FILE )\n"
"\t\t/* Datei schliessen */\n\t\tfclose( outpu"
"t_file );\n\n\t/* Alles OK */\n\treturn( state );\n}\n\n\nVOID push( LONG value )\n{\n\t/* Stackpointer erhî"
"hen */\n\tstackpointer++;\n\n\t/* Ueberlauf ? */\n\tif( stackpointer "
"> STACK_SIZE )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStackoverflow "
"!\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben */\n\tstack["
"stackpointer] = value;\n}\n\n\nLONG pop( VOID )\n{\n\t/* Stackpointe"
"r dekrementieren */\n\tstackpointer--;\n\n\t/* \"Unterlauf\" ? */\n\ti"
"f( stackpointer < -1 )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStacku"
"nderflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurueckliefern */\n\tre"
"turn( stack[stackpointer + 1] );\n}\n\n\nWORD do_parse( VOID )\n{\n\tLO"
"NG taste, symbol, i;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t"
"/* Aktuellen Zustand holen */\n\t\tstate = stack[stackpointer];\n\n"
"\t\t/* Ist die Eingabe geparst ? */\n\t\tif( state == 1 )\n\t\t{\n\t"
"\t\t/* Ist das aktuelle Terminal nicht ACCEPT ? */\n\t\t\tif( look"
"ahead != ACCEPT )\n\t\t\t{\n\t\t\t\t/* Nein, Meldung ausgeben */\n\t\t"
"\t\tfprintf( output_file, \"\\n\\nLetztes Terminal ist nicht ACCEPT "
"!\" );\n\t\t\t\tanz_error++;\n\t\t\t}\n\n\t\t\t/* Gab es Fehler ? */\n\t\t"
"\tif( anz_error )\n\t\t\t\t/* Ja */\n\t\t\t\tfprintf( output_file, \"\\n"
"\\nEs gab %d Fehler !\", anz_error );\n\n\t\t\telse\n\t\t\t\t/* Alles "
"OK */\n\t\t\t\tfprintf( output_file, \"\\n\\nEingabe war OK\\n\" );\n\n"
"\t\t\t/* Zurueck zum Aufrufer */\n\t\t\treturn( anz_error );\n\t\t}"
"\n\n\t\t/* Schieben ? */\n\t\telse if( lr_tab[c_lr"
"[state]][lookahead] < 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Schieben */\n\t\t\tshift( lr_tab[c_lr[state]][lookahead], old_lookahead, lookahead );\n\n\t\t\t"
"/* Altes lookahead merken */\n\t\t\told_lookahead = lookahead;\n\n\t"
"\t\t/* Neues Eingabesymbol holen */\n\t\t\tlookahead = scanner();\n\t"
"\t}\n\n\t\t/* Reduzieren */\n\t\telse if( lr_tab[c_lr[state]][lookahe"
"ad] > 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
	{
		/* Ja */
		if( generate_sem_act() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Zustand nach Reduktion holen */\n\t\t\tsymbol"
" = ziel[state];\n\n\t\t\t/* Reduzieren */\n\t\t\treduce( lr_tab[c_lr[s"
"tate]][lookahead] );\n\n\t\t\t/* Alten Zustand merken */\n\t\t\told_st"
"ate = state;\n\n\t\t\t/* Neuen Zustand holen */\n\t\t\tstate = s"
"tack[stackpointer];\n\n\t\t\t/* Meldung ausgeben */\n\t\t\tfprintf( ou"
"tput_file, \"\\n\\nReduziere mit : \" );\n\n\t\t\t/* Die Produktion au"
"sgeben */\n\t\t\toutput_prod( lr_tab[c_lr[old_state]][lookahead] - 1 );"
"\n\n\t\t\tfprintf( output_file, \"\\nNeuer Status  : %ld\", jump_tab["
"state][symbol - 1] );\n\t\t\tfprintf( output_file, \"\\nAlter Statu"
"s  : %ld\", old_state );\n\t\t\tfprintf( output_file, \"\\nNeues Ter"
"minal: %s\", term_name[lookahead] );\n\n\t\t\t/* Haben wir ein alte"
"s Terminal ? */\n\t\t\tif( old_lookahead >= 0 )\n\t\t\t\t/* Wird haben"
" ein altes Terminal */\n\t\t\t\tfprintf( output_file, \"\\nAltes Ter"
"minal: %s\", term_name[old_lookahead] );\n\n\t\t\t/* Zustand nach R"
"eduktion herstellen */\n\t\t\tif( jump_tab[state][symbol - 1] != "
"0 )\n\t\t\t\t/* Wert auf Stack schieben */\n\t\t\t\tpush( jump_tab[stat"
"e][symbol - 1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t\t/* Fehler beim Springen *"
"/\n\t\t\t\tfprintf( output_file, \"\\n\\nFehler beim Springen !\"\\\n\t\t\t"
"\t\t\t\"\\nAktueller Status  : %ld\"\\\n\t\t\t\t\t\t\"\\nAktuelles Terminal: "
"%s\", old_state, term_name[lookahead] );\n\n\t\t\t\t/* Ende */\n\t\t\t\te"
"xit( 1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syntaxfehler ? */\n\t\telse if( lr_tab["
"c_lr[state]][lookahead] == 0 )\n\t\t{\n\t\t\t/* Fehler */\n\t\t\tif( error() < 0 )"
"\n\t\t\t\t/* Parsing beenden */\n\t\t\t\treturn( -1 );\n\t\t}\n\n"
"\t\t/* Ausgabe auf Bildschirm ? */\n\t\tif( dest_state == DEST_SCRE"
"EN )\n\t\t{\n\t\t\t/* Auf Tastendruck warten */\n\t\t\ttaste = Cnecin();"
"\n\n\t\t\t/* Stack ausgeben ? */\n\t\t\tif( ( taste & 0x000000ff ) == "
"0x73 )\n\t\t\t{\n\t\t\t\t/* Stack ausgeben */\n\t\t\t\tfprintf( output_file"
", \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t\t/* Alle EintrÑge im Stack ausg"
"eben */\n\t\t\t\tfor( i = 0; i < stackpointer; i++ )\n\t\t\t\t\t/* Eintr"
"ag ausgeben */\n\t\t\t\t\tfprintf( output_file, \"%ld\\n\", stack[i] )"
";\n\t\t\t}\n\t\t}\n\n\t\telse\n\t\t{\n\t\t\t/* Auf dem Drucker/Datei immer den "
"Stack ausgeben */\n\n\t\t\t/* Stack ausgeben */\n\t\t\tfprintf( output"
"_file, \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t/* Alle EintrÑge im Stack "
"ausgeben */\n\t\t\tfor( i = 0; i < stackpointer; i++ )\n\t\t\t\t/* Ein"
"trag ausgeben */\n\t\t\t\tfprintf( output_file, \"%ld\\n\", stack[i] "
");\n\t\t}\n\t}\n}\n\nVOID shift( LONG shift_state, LONG old_lahead, L"
"ONG lahead )\n{\n\t/* Hier wird ein Eintrag aus der Shift-Tabell"
"e auf den Stack geschoben. */\n\n\t/* Eintrag auf Stack schieben"
" */\n\tpush( ~shift_state );\n\n\t/* Meldung ausgeben */\n\tfprintf("
" output_file, \"\\n\\nSchiebe       : %s\", term_name[lahead] );\n"
"\tfprintf( output_file, \"\\nNeuer Status  : %ld\", ~shift_state "
");\n\tfprintf( output_file, \"\\nAlter Status  : %ld\", state );\n\t"
"fprintf( output_file, \"\\nNeues Terminal: %s\", term_name[lahea"
"d] );\n\n\t/* Haben wir ein altes Terminal ? */\n\tif( old_lahead "
">= 0 )\n\t\t/* Wird haben ein altes Terminal */\n\t\tfprintf( outpu"
"t_file, \"\\nAltes Terminal: %s\", term_name[old_lahead] );\n}\n\nV"
"OID reduce( LONG nterm_index )\n{\n\t/* Hier wird reduziert. */\n"
"\tREG WORD i;\n\n\t/* Elemente vom Stack lîschen */\n\tfor( i = 0; "
"i < nterm[nterm_index - 1].anz_ri; i++ )\n\t\t/* Eintrag vom Sta"
"ck lîschen */\n\t\tpop();\n}\n\nVOID output_prod( LONG prod_index )"
"\n{\n\t/* Hier wird eine Produktion ausgegeben. */\n\tREG ULONG\ti;"
"\n\tBYTE\t\t**ri;\n\n\t/* Zeiger auf rechte Seite holen */\n\tri = nte"
"rm[prod_index].ri;\n\n\t/* Die Produktion ausgeben */\n\tfprintf( "
"output_file, \"%s -> \", nterm[prod_index].prod_name );\n\n\t/* Is"
"t es eine EPSILON-Produktion ? */\n\tif( !nterm[prod_index].anz"
"_ri )\n\t\t/* Ja, nur EPSILON ausgeben */\n\t\tfprintf( output_file"
", \"EPSILON\" );\n\n\telse\n\t{\n\t\t/* Rechte Seite ausgeben */\n\t\tfor("
" i = 0; i < nterm[prod_index].anz_ri; i++ )\n\t\t\t/* String ausg"
"eben */\n\t\t\tfprintf( output_file, \"%s \", ri[i] );\n\t}\n}", parser_file );

		}

		else
		{
			/* Nicht komprimiert */
			fputs(

"\tdo\n\t{\n\t\t/* Alle Terminale */\n\t\tfor( i = 0; i < ANZ_TERM; i++"
" )\n\t\t{\n\t\t\t/* Shift von i mîglich ? */\n\t\t\tif( ( shift_state = "
"lr_tab[state][i] ) < 0 )\n\t\t\t{\n\t\t\t\t/* Shift von lookahead mîgl"
"ich ? */\n\t\t\t\tif( lr_tab[~shift_state][lookahead] < 0 )\n\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t/* i schieben */\n\t\t\t\t\tshift( lr_tab[state][i], old_looka"
"head, i );\n\n\t\t\t\t\t/* Geschobenes Terminal merken */\n\t\t\t\t\told_l"
"ookahead = i;\n\n\t\t\t\t\t/* Alles OK */\n\t\t\t\t\treturn( 0 );\n\t\t\t\t}\n\n\t\t\t\t/*"
" Reduziere-Zustand ? */\n\t\t\t\telse if( lr_tab[~shift_state][loo"
"kahead] > 0 )\n\t\t\t\t{\n\t\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t\t/* Ist"
" lookahead in FOLLOW( lr_tab[~shift_state][i] ) ? */\n\t\t\t\t\tj ="
" 0;\n\n\t\t\t\t\t/* Die passende FOLLOW-Struktur holen */\n\t\t\t\t\tdo\n\t\t"
"\t\t\t{\n\t\t\t\t\t\t/* Ist es die richtige ? */\n\t\t\t\t\t\tif( ( follow[j]."
"beginn <= ( lr_tab[~shift_state][i] - 1 ) ) && ( follow[j].en"
"d >= ( lr_tab[~shift_state][i] - 1 ) ) )\n\t\t\t\t\t\t\t/* Ja */\n\t\t\t\t"
"\t\t\tbreak;\n\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\t/* Nein */\n\t\t\t\t\t\t\tj++;\n\n\t\t\t\t\t}wh"
"ile( 1 );\n\n\t\t\t\t\t/* Alle FOLLOW-Elemente */\n\t\t\t\t\tfor( p = 0; p"
" < follow[j].anz_follow_elem; p++ )\n\t\t\t\t\t{\n\t\t\t\t\t\t/* Ist es lo"
"okahead ? */\n\t\t\t\t\t\tif( follow[j].follow[p] == lookahead )\n\t\t\t"
"\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t\t\t/* i schieben */\n\t\t\t\t\t\t\tshift( lr_tab[state][i], "
"old_lookahead, i );\n\n\t\t\t\t\t\t\t/* Geschobenes Terminal merken */"
"\n\t\t\t\t\t\t\told_lookahead = i;\n\n\t\t\t\t\t\t\t/* Alles OK */\n\t\t\t\t\t\t\tretu"
"rn( 0 );\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\n\t\t/* Konnte ein Terminal e"
"ingeschoben werden ? */\n\t\tif( i == ANZ_TERM )\n\t\t{\n\t\t\t/* Ist es ACCEPT ? */\n\t\t\tif"
"( lookahead == ACCEPT )\n\t\t\t\t/* Ja, Parsing abbrechen */\n\t\t\t\treturn( -1 );\n\n\t\t\t/* Nein,"
" Meldung ausgeben */\n\t\t\tfprintf( output_file, \"\\n\\nLîsche Ter"
"minal %s !\", term_name[lookahead] );\n\n\t\t\t/* Lookahead lîschen"
" */\n\t\t\tlookahead = scanner();\n\t\t}\n\n\t}while( 1 );\n}\n"
"\n\n\n\n\nWORD parse( VOID )\n{\n\tWORD\tstate;\n\n\t/* Au"
"sgabemedium festlegen */\n\tswitch( dest_state )\n\t{\n\t\tcase DEST"
"_PRN:\n\t\t{\n\t\t\t/* Ausgabe auf Drucker */\n\t\t\toutput_file = stdpr"
"n;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_SCREEN:\n\t\t{\n\t\t\t/* Ausgabe auf B"
"ildschirm */\n\t\t\toutput_file = stdout;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase D"
"EST_FILE:\n\t\t{\n\t\t\t/* Ausgabe in Datei */\n\n\t\t\t/* Datei zum Schr"
"eiben îffnen */\n\t\t\toutput_file = freopen( \"PARSER.LOG\", \"w\", "
"stdout );\n\t\t}\n\t}\n\n\t/* Stack initialisieren */\n\tpush( 0 );\n\n\t/"
"* Eingabesymbol holen */\n\tlookahead = scanner();\n\n\t/* Parsen "
"*/\n\tstate = do_parse();\n\n\t/* Datei schliessen */\n\tfclose( output_file );"
"\n\n\t/* Alles OK */\n\treturn( state );\n}\n\n\nVOID push( LONG value )\n{\n\t/* Stackpointer erhîhen */\n\ts"
"tackpointer++;\n\n\t/* Ueberlauf ? */\n\tif( stackpointer > STACK_S"
"IZE )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStackoverflow !\" );\n\t\te"
"xit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben */\n\tstack[stackpoin"
"ter] = value;\n}\n\n\nLONG pop( VOID )\n{\n\t/* Stackpointer dekreme"
"ntieren */\n\tstackpointer--;\n\n\t/* \"Unterlauf\" ? */\n\tif( stackp"
"ointer < -1 )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStackunderflow "
"!\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurueckliefern */\n\treturn( sta"
"ck[stackpointer + 1] );\n}\n\n\nWORD do_parse( VOID )\n{\n\tLONG taste,"
" symbol, i;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t/* Aktuel"
"len Zustand holen */\n\t\tstate = stack[stackpointer];\n\n"
"\t\t/* Ist die Eingabe geparst ? */\n\t\tif( state == 1 )\n\t\t{\n\t"
"\t\t/* Ist das aktuelle Terminal nicht ACCEPT ? */\n\t\t\tif( look"
"ahead != ACCEPT )\n\t\t\t{\n\t\t\t\t/* Nein, Meldung ausgeben */\n\t\t"
"\t\tfprintf( output_file, \"\\n\\nLetztes Terminal ist nicht ACCEPT "
"!\" );\n\t\t\t\tanz_error++;\n\t\t\t}\n\n\t\t\t/* Gab es Fehler ? */\n\t\t"
"\tif( anz_error )\n\t\t\t\t/* Ja */\n\t\t\t\tfprintf( output_file, \"\\n"
"\\nEs gab %d Fehler !\", anz_error );\n\n\t\t\telse\n\t\t\t\t/* Alles "
"OK */\n\t\t\t\tfprintf( output_file, \"\\n\\nEingabe war OK\\n\" );\n\n"
"\t\t\t/* Zurueck zum Aufrufer */\n\t\t\treturn( anz_error );\n\t\t}"
"\n\n\t\t/* Schieben ? */\n\t\telse if( lr_tab[state][lookah"
"ead] < 0 )\n\t\t{\n", parser_file );


	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Schieben */\n\t\t\tshift( lr_tab[state][look"
"ahead], old_lookahead, lookahead );\n\n\t\t\t/* Altes lookahead me"
"rken */\n\t\t\told_lookahead = lookahead;\n\n\t\t\t/* Neues Eingabesym"
"bol holen */\n\t\t\tlookahead = scanner();\n\t\t}\n\n\t\t/* Reduzieren *"
"/\n\t\telse if( lr_tab[state][lookahead] > 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
	{
		/* Ja */
		if( generate_sem_act() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Den Rest ausgeben */
	fputs(

"\t\t\t/* Zustand nach Reduktion holen */\n\t\t\tsymbol = ziel[state];\n\n\t\t\t/* Redu"
"zieren */\n\t\t\treduce( lr_tab[state][lookahead] );\n\n\t\t\t/* Alten"
" Zustand merken */\n\t\t\told_state = state;\n\n\t\t\t/* Neuen Zustand"
" holen */\n\t\t\tstate = stack[stackpointer];\n\n\t\t\t/* Meldung ausg"
"eben */\n\t\t\tfprintf( output_file, \"\\n\\nReduziere mit : \" );\n\n\t"
"\t\t/* Die Produktion ausgeben */\n\t\t\toutput_prod( lr_tab[ol"
"d_state][lookahead] - 1 );\n\n\t\t\tfprintf( output_file, \"\\nNeuer"
" Status  : %ld\", jump_tab[state][symbol - 1] );\n\t\t\tfprintf( o"
"utput_file, \"\\nAlter Status  : %ld\", old_state );\n\t\t\tfprintf("
" output_file, \"\\nNeues Terminal: %s\", term_name[lookahead] );"
"\n\n\t\t\t/* Haben wir ein altes Terminal ? */\n\t\t\tif( old_lookahea"
"d >= 0 )\n\t\t\t\t/* Wird haben ein altes Terminal */\n\t\t\t\tfprintf("
" output_file, \"\\nAltes Terminal: %s\", term_name[old_lookahead"
"] );\n\n\t\t\t/* Zustand nach Reduktion herstellen */\n\t\t\tif( jump_"
"tab[state][symbol - 1] != 0 )\n\t\t\t\t/* Wert auf Stack schieben "
"*/\n\t\t\t\tpush( jump_tab[state][symbol - 1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t"
"\t/* Fehler beim Springen */\n\t\t\t\tfprintf( output_file, \"\\n\\nFe"
"hler beim Springen !\"\\\n\t\t\t\t\t\t\"\\nAktueller Status  : %ld\"\\\n\t\t\t"
"\t\t\t\"\\nAktuelles Terminal: %s\", old_state, term_name[lookahead"
"] );\n\n\t\t\t\t/* Ende */\n\t\t\t\texit( 1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syntaxfehl"
"er ? */\n\t\telse if( lr_tab[state][lookahead] == 0 )\n\t\t{\n\t\t\t/* Fehler */\n\t\t\tif"
"( error() < 0 )\n\t\t\t\t/* Parsing beenden */\n\t\t\t\treturn( -1 );\n\t\t}\n\n"
"\t\t/* Ausgabe auf Bildschirm ? */\n\t\tif( des"
"t_state == DEST_SCREEN )\n\t\t{\n\t\t\t/* Auf Tastendruck warten */\n"
"\t\t\ttaste = Cnecin();\n\n\t\t\t/* Stack ausgeben ? */\n\t\t\tif( ( tast"
"e & 0x000000ff ) == 0x73 )\n\t\t\t{\n\t\t\t\t/* Stack ausgeben */\n\t\t\t\t"
"fprintf( output_file, \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t\t/* Alle Ei"
"ntrÑge im Stack ausgeben */\n\t\t\t\tfor( i = 0; i < stackpointer;"
" i++ )\n\t\t\t\t\t/* Eintrag ausgeben */\n\t\t\t\t\tfprintf( output_file,"
" \"%ld\\n\", stack[i] );\n\t\t\t}\n\t\t}\n\n\t\telse\n\t\t{\n\t\t\t/* Auf dem Druc"
"ker/Datei immer den Stack ausgeben */\n\n\t\t\t/* Stack ausgeben *"
"/\n\t\t\tfprintf( output_file, \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t/* All"
"e EintrÑge im Stack ausgeben */\n\t\t\tfor( i = 0; i < stackpoint"
"er; i++ )\n\t\t\t\t/* Eintrag ausgeben */\n\t\t\t\tfprintf( output_file"
", \"%ld\\n\", stack[i] );\n\t\t}\n\t}\n}\n\nVOID shift( LONG shift_state"
", LONG old_lahead, LONG lahead )\n{\n\t/* Hier wird ein Eintrag "
"aus der Shift-Tabelle auf den Stack geschoben. */\n\n\t/* Eintra"
"g auf Stack schieben */\n\tpush( ~shift_state );\n\n\t/* Meldung a"
"usgeben */\n\tfprintf( output_file, \"\\n\\nSchiebe       : %s\", t"
"erm_name[lahead] );\n\tfprintf( output_file, \"\\nNeuer Status  :"
" %ld\", ~shift_state );\n\tfprintf( output_file, \"\\nAlter Status"
"  : %ld\", state );\n\tfprintf( output_file, \"\\nNeues Terminal: "
"%s\", term_name[lahead] );\n\n\t/* Haben wir ein altes Terminal ?"
" */\n\tif( old_lahead >= 0 )\n\t\t/* Wird haben ein altes Terminal"
" */\n\t\tfprintf( output_file, \"\\nAltes Terminal: %s\", term_name"
"[old_lahead] );\n}\n\nVOID reduce( LONG nterm_index )\n{\n\t/* Hier"
" wird reduziert. */\n\tREG WORD i;\n\n\t/* Elemente vom Stack lîsc"
"hen */\n\tfor( i = 0; i < nterm[nterm_index - 1].anz_ri; i++ )\n"
"\t\t/* Eintrag vom Stack lîschen */\n\t\tpop();\n}\n\nVOID output_pro"
"d( LONG prod_index )\n{\n\t/* Hier wird eine Produktion ausgegeb"
"en. */\n\tREG ULONG\ti;\n\tBYTE\t\t**ri;\n\n\t/* Zeiger auf rechte Seit"
"e holen */\n\tri = nterm[prod_index].ri;\n\n\t/* Die Produktion au"
"sgeben */\n\tfprintf( output_file, \"%s -> \", nterm[prod_index]."
"prod_name );\n\n\t/* Ist es eine EPSILON-Produktion ? */\n\tif( !n"
"term[prod_index].anz_ri )\n\t\t/* Ja, nur EPSILON ausgeben */\n\t\t"
"fprintf( output_file, \"EPSILON\" );\n\n\telse\n\t{\n\t\t/* Rechte Seit"
"e ausgeben */\n\t\tfor( i = 0; i < nterm[prod_index].anz_ri; i++"
" )\n\t\t\t/* String ausgeben */\n\t\t\tfprintf( output_file, \"%s \", r"
"i[i] );\n\t}\n}", parser_file );

		}
	}

	else
	{
		/* Die normale Parser-Version erzeugen, LR-Tabelle komprimiert ? */
		if( !lr_info.compress )
		{
			/* Ja */
			fputs( 

"\tdo\n\t{\n\t\t/* Alle Terminale */\n\t\tfor( i = 0; i < ANZ_TERM; i++"
" )\n\t\t{\n\t\t\t/* Shift von i mîglich ? */\n\t\t\tif( ( shift_state = "
"lr_tab[c_lr[state]][i] ) < 0 )\n\t\t\t{\n\t\t\t\t/* Shift von lookahea"
"d mîglich ? */\n\t\t\t\tif( lr_tab[c_lr[~shift_state]][lookahead] "
"< 0 )\n\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t/* i schieben */\n\t\t\t\t\tpush( ~lr_tab[c_lr[sta"
"te]][i] );\n\n\t\t\t\t\t/* Alles OK */\n\t\t\t\t\treturn( 0 );\n\t\t\t\t}\n\n\t\t\t\t/* Re"
"duziere-Zustand ? */\n\t\t\t\telse if( lr_tab[c_lr[~shift_state]]["
"lookahead] > 0 )\n\t\t\t\t{\n\t\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t\t/* "
"Ist lookahead in FOLLOW( lr_tab[c_lr[~shift_state]][i] ) ? */"
"\n\t\t\t\t\tj = 0;\n\n\t\t\t\t\t/* Die passende FOLLOW-Struktur holen */\n\t"
"\t\t\t\tdo\n\t\t\t\t\t{\n\t\t\t\t\t\t/* Ist es die richtige ? */\n\t\t\t\t\t\tif( ( f"
"ollow[j].beginn <= ( lr_tab[c_lr[~shift_state]][i] - 1 ) ) &&"
" ( follow[j].end >= ( lr_tab[c_lr[~shift_state]][i] - 1 ) ) )"
"\n\t\t\t\t\t\t\t/* Ja */\n\t\t\t\t\t\t\tbreak;\n\n\t\t\t\t\t\telse\n\t\t\t\t\t\t\t/* Nein */\n"
"\t\t\t\t\t\t\tj++;\n\n\t\t\t\t\t}while( 1 );\n\n\t\t\t\t\t/* Alle FOLLOW-Elemente "
"*/\n\t\t\t\t\tfor( p = 0; p < follow[j].anz_follow_elem; p++ )\n\t\t\t\t"
"\t{\n\t\t\t\t\t\t/* Ist es lookahead ? */\n\t\t\t\t\t\tif( follow[j].follow["
"p] == lookahead )\n\t\t\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t\t\t/* i schieben */\n\t\t\t\t\t\t\tpush"
"( ~lr_tab[c_lr[state]][i] );\n\n\t\t\t\t\t\t\t/* Alles OK */\n\t\t\t\t\t\t\tre"
"turn( 0 );\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\n\t\t/* Konnte ein Terminal"
" eingeschoben werden ? */\n\t\tif( i == ANZ_TERM )"
"\n\t\t{\n\t\t\t/* Ist es ACCEPT ? */\n\t\t\tif( lookahead == ACCEPT )\n\t\t\t\t/* Ja, Parsing abbrechen */\n"
"\t\t\t\treturn( -1 );\n\n\t\t\t/* Nein, lîschen */\n\t\t\tlookahead = scanner();\n\t\t}\n"
"\n\t}while( 1 );\n}\n\n\n\n\nWORD parse( VOID )\n{\n\n\tWORD\tstate;\n\n\t/* Ausgabemedium festl"
"egen */\n\tswitch( dest_state )\n\t{\n\t\tcase DEST_PRN:\n\t\t{\n\t\t\t/* A"
"usgabe auf Drucker */\n\t\t\toutput_file = stdprn;\n\t\t\tbreak;\n\t\t}\n"
"\n\t\tcase DEST_SCREEN:\n\t\t{\n\t\t\t/* Ausgabe auf Bildschirm */\n\t\t\to"
"utput_file = stdout;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_FILE:\n\t\t{\n\t\t\t"
"/* Ausgabe in Datei */\n\n\t\t\t/* Datei zum Schreiben îffnen */\n\t"
"\t\toutput_file = freopen( \"PARSER.LOG\", \"w\", stdout );\n\t\t}\n\t}\n"
"\n\t/* Stack initialisieren */\n\tpush( 0 );\n\n\t/* Eingabesymbol h"
"olen */\n\tlookahead = scanner();\n\n\t/* Parsen */\n\tstate = do_parse();\n\n\t/* Ausgabe auf Datei ? */\n"
"\tif( dest_state == DEST_FILE )\n\t\t/* Datei schliessen */\n\tfclose( output_file );\n\n\t/* Alles OK */\n"
"\treturn( state );\n}\n\n\nVOID push( L"
"ONG value )\n{\n\t/* Stackpointer erhîhen */\n\tstackpointer++;\n\n\t"
"/* Ueberlauf ? */\n\tif( stackpointer > STACK_SIZE )\n\t{\n\t\tprintf"
"( \"\\n\\nStackoverflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stac"
"k schieben */\n\tstack[stackpointer] = value;\n}\n\n\nLONG pop( VOI"
"D )\n{\n\t/* Stackpointer dekrementieren */\n\tstackpointer--;\n\n\t/"
"* \"Unterlauf\" ? */\n\tif( stackpointer < -1 )\n\t{\n\t\tprintf( \"\\n\\"
"nStackunderflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurueckliefern"
" */\n\treturn( stack[stackpointer + 1] );\n}\n\n\nWORD do_parse( VOID "
")\n{\n\tLONG\tsymbol;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t/* "
"Aktuellen Zustand holen */\n\t\tstate = stack[stackpointer];\n\n"
"\t\t/* Ist die Eingabe geparst ? */\n\t\tif( state == 1 )\n\t\t{\n\t\t\t/"
"* Ist das aktuelle Terminal nicht ACCEPT ? */\n\t\t\tif( lookahea"
"d != ACCEPT )\n\t\t\t\t/* Nein, Fehler */\n\t\t\t\tanz_error++;\n\n\t\t\t/* "
"Zurueck zum Aufrufer */\n\t\t\treturn( anz_error > 0 ? anz_error :"
" 0 );\n\t\t}\n\n\t\t/* Schieben ? */\n\t\telse if( lr_tab[c_lr[state]][lookahead] < 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Schieben */\n\t\t\tpush( ~lr_tab[c_lr[state]][lookahead] );\n"
"\n\t\t\t/* Neues Eingabesymbol holen */\n\t\t\tlookahead = scanner();"
"\n\n\t\t\t/* Fehlerlawine vermeiden */\n\t\t\terror_flg = 0;\n\t\t}\n\n\t\t/*"
" Reduzieren */\n\t\telse if( lr_tab[c_lr[state]][lookahead] > 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
	{
		/* Ja */
		if( generate_sem_act() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Zustand nach Reduktion holen */\n\t\t\tsymbol = ziel["
"state];\n\n\t\t\t/* Reduzieren */\n\t\t\treduce( lr_tab[c_lr[state]][l"
"ookahead] - 1 );\n\n\t\t\t/* Neuen Zustand holen */\n\t\t\tstate = sta"
"ck[stackpointer];\n\n\t\t\t/* Zustand nach Reduktion herstellen */"
"\n\t\t\tif( jump_tab[state][symbol - 1] != 0 )\n\t\t\t\t/* Wert auf St"
"ack schieben */\n\t\t\t\tpush( jump_tab[state][symbol - 1] );\n\n\t\t\t"
"else\n\t\t\t{\n\t\t\t\t/* Fehler beim Springen */\n\t\t\t\texit( 1 );\n\t\t\t}\n"
"\t\t}\n\n\t\t/* Syntaxfehler ? */\n\t\telse if( lr_tab[c_lr[state]][lo"
"okahead] == 0 )\n\t\t{\n\t\t\t/* Fehler */\n\t\t\tif( error() < 0 )\n\t\t\t\t"
"/* Parsing beenden */\n\t\t\t\treturn( -1 );\n\t\t}\n\t}\n}\n\nVOID reduce"
"( LONG prod_index )\n{\n\t/* Hier wird reduziert. */\n\tREG WORD i"
";\n\n\t/* Elemente vom Stack lîschen */\n\tfor( i = 0; i < anz_ri["
"prod_index]; i++ )\n\t\t/* Eintrag vom Stack lîschen */\n\t\tpop();"
"\n}", parser_file );

		}

		else
		{
			/* Nicht komprimiert */
			fputs(

"\tdo\n\t{\n\t\t/* Alle Terminale */\n\t\tfor( i = 0; i < ANZ_TERM; i++"
" )\n\t\t{\n\t\t\t/* Shift von i mîglich ? */\n\t\t\tif( ( shift_state = "
"lr_tab[state][i] ) < 0 )\n\t\t\t{\n\t\t\t\t/* Shift von lookahead mîgl"
"ich ? */\n\t\t\t\tif( lr_tab[~shift_state][lookahead] < 0 )\n\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t/* i schieben */\n\t\t\t\t\tpush( ~lr_tab[state][i] );\n\n\t\t\t\t\t/"
"* Alles OK */\n\t\t\t\t\treturn( 0 );\n\t\t\t\t}\n\n\t\t\t\t/* Reduziere-Zustand ? "
"*/\n\t\t\t\telse if( lr_tab[~shift_state][lookahead] > 0 )\n\t\t\t\t{\n\t"
"\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t\t/* Ist lookahead in FOLLOW("
" lr_tab[~shift_state][i] ) ? */\n\t\t\t\t\tj = 0;\n\n\t\t\t\t\t/* Die pass"
"ende FOLLOW-Struktur holen */\n\t\t\t\t\tdo\n\t\t\t\t\t{\n\t\t\t\t\t\t/* Ist es "
"die richtige ? */\n\t\t\t\t\t\tif( ( follow[j].beginn <= ( lr_tab[~s"
"hift_state][i] - 1 ) ) && ( follow[j].end >= ( lr_tab[~shift_"
"state][i] - 1 ) ) )\n\t\t\t\t\t\t\t/* Ja */\n\t\t\t\t\t\t\tbreak;\n\n\t\t\t\t\t\telse"
"\n\t\t\t\t\t\t\t/* Nein */\n\t\t\t\t\t\t\tj++;\n\n\t\t\t\t\t}while( 1 );\n\n\t\t\t\t\t/* Al"
"le FOLLOW-Elemente */\n\t\t\t\t\tfor( p = 0; p < follow[j].anz_foll"
"ow_elem; p++ )\n\t\t\t\t\t{\n\t\t\t\t\t\t/* Ist es lookahead ? */\n\t\t\t\t\t\tif"
"( follow[j].follow[p] == lookahead )\n\t\t\t\t\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t\t\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\t\t\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t\t\t\t\t/* i schi"
"eben */\n\t\t\t\t\t\t\tpush( ~lr_tab[state][i] );\n\n\t\t\t\t\t\t\t/* Alles OK"
" */\n\t\t\t\t\t\t\treturn( 0 );\n\t\t\t\t\t\t}\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t}\n\n\t\t/* Konnte"
" ein Terminal eingeschoben werden ? */\n\t\tif( i == ANZ_TERM )"
"\n\t\t{\n\t\t\t/* Ist es ACCEPT ? */\n\t\t\tif( lookahead == ACCEPT )\n\t\t\t\t/* Ja, Parsing abbrechen */\n"
"\t\t\t\treturn( -1 );\n\n\t\t\t/* Nein, lîschen */\n\t\t\tlookahead = scanner();\n\t\t}\n"
"\n\t}while( 1 );\n}\n\n\n\nWORD parse( VOID )\n{\n\n\t/* Ausgabemedium "
"festlegen */\n\tswitch( dest_state )\n\t{\n\t\tcase DEST_PRN:\n\t\t{\n\t\t"
"\t/* Ausgabe auf Drucker */\n\t\t\toutput_file = stdprn;\n\t\t\tbreak;"
"\n\t\t}\n\n\t\tcase DEST_SCREEN:\n\t\t{\n\t\t\t/* Ausgabe auf Bildschirm */"
"\n\t\t\toutput_file = stdout;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_FILE:\n\t\t"
"{\n\t\t\t/* Ausgabe in Datei */\n\n\t\t\t/* Datei zum Schreiben îffnen"
" */\n\t\t\toutput_file = freopen( \"PARSER.LOG\", \"w\", stdout );\n\t\t"
"}\n\t}\n\n\t/* Stack initialisieren */\n\tpush( 0 );\n\n\t/* Eingabesym"
"bol holen */\n\tlookahead = scanner();\n\n\t/* Parsen */\n\tstate = do_parse();"
"\n\n\t/* Datei schliessen */\n\tfclose( output_file );\n\n\t/* Alles OK */\n\treturn( state );\n}\n\n\nVOID pu"
"sh( LONG value )\n{\n\t/* Stackpointer erhîhen */\n\tstackpointer+"
"+;\n\n\t/* Ueberlauf ? */\n\tif( stackpointer > STACK_SIZE )\n\t{\n\t\tp"
"rintf( \"\\n\\nStackoverflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf"
" Stack schieben */\n\tstack[stackpointer] = value;\n}\n\n\nLONG pop"
"( VOID )\n{\n\t/* Stackpointer dekrementieren */\n\tstackpointer--"
";\n\n\t/* \"Unterlauf\" ? */\n\tif( stackpointer < -1 )\n\t{\n\t\tprintf("
" \"\\n\\nStackunderflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurueckli"
"efern */\n\treturn( stack[stackpointer + 1] );\n}\n\n\nWORD do_parse( "
"VOID )\n{\n\tLONG\tsymbol;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n"
"\t\t/* Aktuellen Zustand holen */\n\t\tstate = stack[stackpointer]"
";\n\n\t\t/* Ist die Eingabe geparst ? */\n\t\tif( state == 1 )\n\t\t{\n\t\t\t/"
"* Ist das aktuelle Terminal nicht ACCEPT ? */\n\t\t\tif( lookahea"
"d != ACCEPT )\n\t\t\t\t/* Nein, Fehler */\n\t\t\t\tanz_error++;\n\n\t\t\t/* "
"Zurueck zum Aufrufer */\n\t\t\treturn( anz_error > 0 ? anz_error :"
" 0 );\n\t\t}\n\n\t\t/* Schieben ? */\n\t\telse if( lr_tab[state][lookahead] < 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
		/* Ja */
		fputs(	"\t\t\t/* Wert vom Scanner auf den Stack legen */\n"
				"\t\t\tlr_sem_stack[++lr_sem_count] = scan_val;\n\n", parser_file );

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Schieben */\n\t\t\tpush( ~lr_tab[state][lookahead] );\n\n\t\t\t/* N"
"eues Eingabesymbol holen */\n\t\t\tlookahead = scanner();\n\n\t\t\t/* "
"Fehlerlawine vermeiden */\n\t\t\terror_flg = 0;\n\t\t}\n\n\t\t/* Reduzie"
"ren */\n\t\telse if( lr_tab[state][lookahead] > 0 )\n\t\t{\n", parser_file );

	/* Semmantische Aktionen ? */
	if( sem_act )
	{
		/* Ja */
		if( generate_sem_act() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Den Rest ausgeben */
	fputs( 

"\t\t\t/* Zustand nach Reduktion holen */\n\t\t\tsymbol = ziel[state];\n\n\t\t\t/*"
" Reduzieren */\n\t\t\treduce( lr_tab[state][lookahead] - 1 );\n\n\t\t"
"\t/* Neuen Zustand holen */\n\t\t\tstate = stack[stackpointer];\n\n\t"
"\t\t/* Zustand nach Reduktion herstellen */\n\t\t\tif( jump_tab[sta"
"te][symbol - 1] != 0 )\n\t\t\t\t/* Wert auf Stack schieben */\n\t\t\t\t"
"push( jump_tab[state][symbol - 1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t\t/* Feh"
"ler beim Springen */\n\t\t\t\texit( 1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syntaxfehl"
"er ? */\n\t\telse if( lr_tab[state][lookahead] == 0 )\n"
"\t\t{\n\t\t\t/* Fehler */\n\t\t\tif( error() < 0 )\n\t\t\t\t/* Parsing beenden */\n\t\t\t\treturn( -1 );\n\t\t}\n"
"\t}\n}\n\nVOID reduce( LONG prod_index )\n{\n\t/* "
"Hier wird reduziert. */\n\tREG WORD i;\n\n\t/* Elemente vom Stack "
"lîschen */\n\tfor( i = 0; i < anz_ri[prod_index]; i++ )\n\t\t/* Ei"
"ntrag vom Stack lîschen */\n\t\tpop();\n}", parser_file );

		}
	}

	/* Hilfsroutinen ausgeben */
	output_incl();

	/* Datei schliessen */
	if( fclose( parser_file ) == EOF )
		/* Fehler */
		return( error( CANNOT_CLOSE_PARSER_FILE, 0, 0 ) );

	/* Zuruecksetzen */
	parser_file = 0L;

	/* Alles OK */
	return( 0 );
}


VOID output_action_tab( VOID )
{
	/* Hier wird die LR-Tabelle ausgegeben. */
	NTERM_STR	*nterm;
	REG ULONG	i, j;
	BYTE		state_val[32+1];

	/* LÑnge der Anzahl der ZustÑnde */
	ltoa( hulle_count, state_val, 10 );
	state_length = ( WORD ) strlen( state_val );

	/* String fÅr neue Zeile */
	state_val[0] = '\n';

	for( i = 0; i < ( 10 + state_length ); i++ )
		/* Anzahl der Blankes */
		state_val[i + 1] = ' ';

	/* Mit Null-Byte abschliessen */
	state_val[i] = 0;

	/* Deklaration ausgeben */
	fprintf( parser_file,	"\n\n\n\n/****************/"
							"\n/* %s-Tabelle */"
							"\n/****************/"
							"\n\nLONG\tlr_tab[%lu][ANZ_TERM] = {\n", type[lr_info.type], ( hulle_count + 1 ) );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Nummer des Zustandes ausgeben */
		fprintf( parser_file, "/* %*lu */ ", state_length, i );

		/* Sich îffnende Klammer ausgeben */
		fputs( "{ ", parser_file );

		/* Alle Terminale durchgehen */
		for( j = 0; j <= term_count; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Komma ausgeben */
				output_text( ", ", state_val, 0 );

			/* Reduce-Eintrag ? */
			if( lr_tab[i][j].state == REDUCE )
			{
				/* Zeiger auf NTERM_STR holen */
				nterm = nterm_str[lr_tab[i][j].value];

				/* Hat die Produktion Alternativen ? */
				if( !( nterm->alt ) && ( ( ( lr_tab[i][j].value + 1 ) < nterm_count ) && ( !( nterm_str[lr_tab[i][j].value + 1]->alt ) ) ) )
				{
					/* Nein, Produktion ausgeben */
					sprintf( file_parser_name, "%s", hash_tab[nterm->hash_tab_index].name );
					output_text( file_parser_name, "\n", ( WORD ) nterm_length + 2 );
				}

				else
				{
					/* Ja */
					sprintf( file_parser_name, "%s_%lu", hash_tab[nterm->hash_tab_index].name, lr_tab[i][j].value - ( lr_tab[i][j].value - nterm->alt ) );
					output_text( file_parser_name, "\n", ( WORD ) nterm_length + 2 );
				}
			}

			/* Shift-Eintrag ? */
			else if( lr_tab[i][j].state == SHIFT )
			{
				/* Ist ein Shift-Eintrag */

				/* Wert ausgeben */
				sprintf( file_parser_name, "%*ld", ( WORD ) nterm_length + 2, ~lr_tab[i][j].value );
				output_text( file_parser_name, "\n", 0 );
			}

			else
				/* Null ausgeben */
				output_text( "0", "\n", ( WORD ) nterm_length + 2 );
		}

		/* Komma ausgeben ? */
		if( i == hulle_count )
			/* Nein, da letzte HÅlle */
			fputs( " }\n", parser_file );

		else
			/* Ja */
			fputs( " },\n", parser_file );

		/* Neue Zeile */
		max_char = 0;
	}

	/* Tabelle ist zu Ende */
	fputs( "};", parser_file );
}


VOID output_index_array( VOID )
{
	/*
		Hier wird das Array mit den Indexen der komprimierten
		LR-Tab ausgegeben.
	*/
	REG ULONG	i;

	/* Neue Zeile */
	max_char = 0;

	/* Ersten Eintrag setzen  */
	compressed_lr_index_tab[0] = 0;

	/* Deklaration ausgeben */
	fprintf( parser_file,	"\n\n\n/* LR-Tabellen-Indexe */"
							"\n\nLONG\tc_lr[%lu] = { ", ( hulle_count + 1 + main_dbl_count ) );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= ( hulle_count + main_dbl_count ); i++ )
	{
		/* Nummer des Zustandes ausgeben */
		sprintf( file_parser_name, "%ld", compressed_lr_index_tab[i] );
		output_text( file_parser_name, "\n", 0 );

		/* Komma ausgeben ? */
		if( i != ( hulle_count + main_dbl_count ) )
			/* Ja */
			fputs( ", ", parser_file );

		/* Neue Zeile */
		max_char = 0;
	}

	/* Array abschliessen */
	fputs( " };", parser_file );
}


VOID output_jump_tab( VOID )
{
	/* Hier wird die Shift-Tabelle ausgegeben. */
	REG ULONG	i, j;

	/* Anzahl der Zeichen in einer Zeile zuruecksetzen */
	max_char = 0;

	/* Deklaration ausgeben */
	fprintf( parser_file, "\n\n\nLONG\tjump_tab[%lu][%lu] = {\n", ( hulle_count + main_dbl_count + 1 ), prod_count );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= ( hulle_count + main_dbl_count ); i++ )
	{
		/* Nummer des Zustandes ausgeben */
		fprintf( parser_file, "/* %*lu */ ", state_length, i );

		/* Sich îffnende Klammer ausgeben */
		fputs( "{ ", parser_file );

		/* Alle Produktionen durchgehen */
		for( j = 0; j < prod_count; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Komma ausgeben */
				output_text( ", ", "\n            ", 0 );

			/* Wert ausgeben */
			sprintf( file_parser_name, "%*lu", state_length, jump_tab[i][j] );
			output_text( file_parser_name, "\n", 0 );
		}

		/* Komma ausgeben ? */
		if( i == ( hulle_count + main_dbl_count ) )
			/* Nein, da letzte HÅlle */
			fputs( " }\n", parser_file );

		else
			/* Ja */
			fputs( " },\n", parser_file );

		/* Neue Zeile */
		max_char = 0;
	}

	/* Tabelle ist zu Ende */
	fputs( "};\n", parser_file );
}


VOID output_reduce_struct( VOID )
{
	/*
		Hier wird bei der Debug-Version eine zusÑtzliche Struktur eingebunden,
		die die Namen der Produktionen enthÑlt.
	*/
	NTERM_STR	*nterm;
	REG ULONG	i, j;

	/* Debug-Version erzeugen ? */
	if( lr_info.debug )
	{
		/* Nein */
		fprintf( parser_file,	"\n\n\n/* Die Anzahl der Elemente auf der "
								"rechten Seite der Produktionen */\n"
								"WORD\tanz_ri[%lu] = { ", nterm_count + 1 );

		/* Anzahl der Elemente in einer Zeile zuruecksetzen */
		max_char = 0;

		/* Alle Produktionen durchgehen */
		for( i = 0; i <= nterm_count; i++ )
		{
			/* Komma ausgeben ? */
			if( i )
				/* Komma ausgeben */
				output_text( ", ", "\n            ", 0 );

			/* Wert ausgeben */
			sprintf( file_parser_name, "%2d", nterm_str[i]->anz_ri );
			output_text( file_parser_name, "\n", 0 );
		}

		/* Array ist zu Ende */
		fputs( " };\n", parser_file );
	}

	else
	{
		/* Ja, Kommentar ausgeben */
		fputs( "\n\n\n/* Die rechte Seite aller Produktionen */", parser_file );

		/* Die rechte Seite aller Produktionen ausgeben */
		for( i = 0; i <= nterm_count; i++ )
		{
			/* Initialisieren */
			fputs( "\nBYTE\t*", parser_file );

			/* Zeiger auf NTERM_STR holen */
			nterm = nterm_str[i];

			/* Hat die Produktion Alternativen ? */
			if( !( nterm->alt ) && ( ( ( i + 1 ) < nterm_count ) && ( !( nterm_str[i + 1]->alt ) ) ) )
				/* Nein */
				sprintf( file_parser_name, "%s", hash_tab[nterm->hash_tab_index].name );

			else
				/* Ja */
				sprintf( file_parser_name, "%s_%lu", hash_tab[nterm->hash_tab_index].name, i - ( i - nterm->alt ) );

			/* Name der Produktion */
			fprintf( parser_file, "%s_ri[%d] = {", strlwr( file_parser_name ), nterm->anz_ri );

			/* Alle Elemente auf der rechten Seite ausgeben */
			for( j = 0; j < nterm->anz_ri; j++ )
			{
				/* Ist es ein Terminal ? */
				if( hash_tab[nterm->ri[j]].term_str )
					/* Ja */
					fprintf( parser_file, " \"\\'%s\\'\"", hash_tab[nterm->ri[j]].name );

				else
					/* Nein */
					fprintf( parser_file, " \"%s\"", hash_tab[nterm->ri[j]].name );

				/* War es das letzte Element ? */
				if( j != ( nterm->anz_ri - 1 ) )
					/* Nein, Komma ausgeben */
					fputs( ",", parser_file );
			}

			/* Abschliessen */
			fputs( " };", parser_file );
		}

		/* Struktur-Deklaration ausgeben */
		fputs(	"\n\n\ntypedef struct\n{\n\t/* Name der Produktion */\n\tBYTE\t*prod_name;\n"
				"\n\t/* Anzahl der Elemente auf der rechten Seite */\n\tWORD\tanz_ri;\n\n\t/* Zeiger auf rechte Seite */\n\tBYTE\t**ri;\n\n}NTERM;", parser_file );

		/* Definition */
		fputs( "\n\nNTERM\tnterm[] = {\n\n", parser_file );

		/* Alle Produktionen ausgeben */
		for( i = 0; i <= nterm_count; i++ )
		{
			/* Struktur îffnen */
			fputs( "{ ", parser_file );

			/* Zeiger auf NTERM_STR holen */
			nterm = nterm_str[i];

			/* Hat die Produktion Alternativen ? */
			if( !( nterm->alt ) && ( ( ( i + 1 ) < nterm_count ) && ( !( nterm_str[i + 1]->alt ) ) ) )
			{
				/* Nein, Produktion ausgeben */
				sprintf( file_parser_name, "%s", hash_tab[nterm->hash_tab_index].name );
				fprintf( parser_file, "\"%s\", ", file_parser_name );
			}

			else
			{
				/* Ja */
				sprintf( file_parser_name, "%s_%lu", hash_tab[nterm->hash_tab_index].name, i - ( i - nterm->alt ) );
				fprintf( parser_file, "\"%s\", ", file_parser_name );
			}

			/* Ist es eine EPSILON-Produktion ? */
			if( ( nterm->anz_ri == 1 ) && ( nterm->ri[0] == epsilon ) )
				/* Ja, Anzahl der Elemente gleich null */
				fputs( "0", parser_file );

			else
				/* Anzahl der Elemente auf der rechten Seite ausgeben */
				fprintf( parser_file, "%d", nterm->anz_ri );

			/* Zeiger auf rechte Seite ausgeben */
			fprintf( parser_file, ", %s_ri", strlwr( file_parser_name ) );

			/* Struktur schliessen */
			fputs( " }", parser_file );

			/* War es die letzte Produktion ? */
			if( i < nterm_count )
				/* Nein, Komma ausgeben */
				fputs( ",", parser_file );

			/* Kommentar îffnen */
			fputs( " /*", parser_file );

			/* Rechte Seite ausgeben */
			output_right( parser_file, i );

			/* Kommentar schliessen */
			fputs( " */\n", parser_file );
		}

		/* NTERM-Struktur schliessen */
		fputs( "};\n\n", parser_file );

		/* Die Strings zu den Terminalen ausgeben */

		/* Anzahl der Zeichen in einer Zeile zuruecksetzen */
		max_char = 0;

		/* Kommentar ausgeben */
		fputs( "/* Die Terminale */", parser_file );

		/* Definition ausgeben */
		fputs( "\nBYTE\t*term_name[ANZ_TERM] = { ", parser_file );

		/* Alle Terminale */
		for( i = 0; i <= term_count; i++ )
		{
			/* Komma ausgeben ? */
			if( i )
				/* Ja */
				output_text( ", ", "\n", 0 );

			/* Terminal ausgeben */
			sprintf( file_parser_name, "\"%s\"", strupr( hash_tab[term_str[i].hash_tab_index].name ) );
			output_text( file_parser_name, "\n", 0 );
		}

		/* Array schliessen */
		fputs( " };\n", parser_file );
	}
}


VOID output_follow_str( VOID )
{
	/*
		Hier werden die FOLLOW-Mengen fÅr die automatische Fehlerbehand-
		lung ausgegeben.
	*/
	REG	ULONG	i, j;

	/* Alle Produktionen */
	for( i = 0; i < prod_count; i++ )
	{
		/* Anzahl der Zeichen in einer Zeile zuruecksetzen */
		max_char = 0;

		/* Deklaration ausgeben */
		sprintf( file_parser_name, "\nLONG\t%s_follow[] = { ", hash_tab[nterm_str[prod_index_arr[i]]->hash_tab_index].name );
		output_text( file_parser_name, "\n", 0 );

		/* Alle FOLLOW-Terminale ausgeben */
		for( j = 0; j < nterm_str[prod_index_arr[i]]->follow.anz_elem; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Ja */
				output_text( ", ", "\n", 0 );

			/* Terminal ausgeben */
			sprintf( file_parser_name, "%s", strupr( hash_tab[nterm_str[prod_index_arr[i]]->follow.term_arr[j]].name ) );
			output_text( file_parser_name, "\n", 0 );
		}

		/* Struktur schliessen */
		fputs( " };", parser_file );
	}

	/* Struktur-Definition ausgeben */
	fputs(	"\n\ntypedef struct\n{\n\t/* FOLLOW-Menge gilt ab Alternative begin */\n\t" \
			"ULONG\tbeginn;\n\n\t/* FOLLOW-Menge gilt bis Alternative end */\n" \
			"\tULONG\tend;\n\n\t/* Anzahl der FOLLOW-Elemente */\n"
			"\tULONG\tanz_follow_elem;\n\n\t/* FOLLOW-Menge */\n" \
			"\tLONG\t*follow;\n\n}FOLLOW;\n\nFOLLOW\t" \
			"follow[] =\n{\n", parser_file );

	/* Eintrag in FOLLOW-Struktur zu jeder Produktion erzeugen */
	for( i = 1; i <= nterm_count; )
	{
		/* Struktur îffnen */
		fputs( "{ ", parser_file );

		/* Ab dieser Produktion gilt die FOLLOW-Menge */
		fprintf( parser_file, "%ld, ", i );

		/* Index der Produktion merken */
		j = i;

		/* Index der letzten Alternative bestimmen */
		while( ( ++i <= nterm_count ) && ( nterm_str[i]->alt ) )
			;

		/* Bis dieser Alternative gilt die FOLLOW-Menge */
		fprintf( parser_file, "%ld, %ld, ", i - 1, nterm_str[j]->follow.anz_elem );

		/* Zeiger auf Follow-Struktur eintragen */
		fprintf( parser_file, "%s_follow },\n", hash_tab[nterm_str[j]->hash_tab_index].name );
	}

	/* Struktur schliessen */
	fputs( "};", parser_file );
}


VOID output_ziel_tab( VOID )
{
	/*
		Hier wird die ZustÑnge in eine Struktur eingetragen, die der Parser
		nach erfolgreicher Reduktion einnimmt.
	*/
	REG	ULONG	i;
	BYTE		text[256];

	/* Anzahl der Zeichen in einer Zeile zuruecksetzen */
	max_char = 0;

	/* Kommentar ausgeben */
	fputs( "\n\n\n/* Die Ziel-Tabelle */", parser_file );

	/* Definition ausgeben */
	fprintf( parser_file, "\nLONG\tziel[%lu] = { ", ( hulle_count + 1 + main_dbl_count ) );

	/* Alle HÅllen */
	for( i = 0; i <= ( hulle_count + main_dbl_count ); i++ )
	{
		/* Komma ausgeben ? */
		if( i )
			/* Ja */
			output_text( ", ", "\n", 0 );

		/* Gibt es in HÅlle i eine Reduktion ? */
		if( ziel_tab[i] )
		{
			/* Nichtterminal ausgeben */
			strcpy( text, hash_tab[nterm_str[ziel_tab[i]]->hash_tab_index].name );
			sprintf( file_parser_name, "%s", strupr( text ) );
			output_text( file_parser_name, "\n", 0 );
		}

		else
			/* Keine Reduktion in HÅlle i */
			output_text( "0", "\n", 0 );
	}

	/* Struktur abschliessen */
	fputs( " };", parser_file );
}


VOID output_text( BYTE *string, BYTE *new_line, WORD format )
{
	/*
		Hier wird der string unter Beachtung der max. Anzahl der Zeichen
		pro Zeile ausgegeben.
	*/

	/* String einruecken ? */
	if( format )
		/* Ja */
		max_char += fprintf( parser_file, "%*s", format, string );

	else
		/* Nein */
		max_char += fprintf( parser_file, "%s", string );

	/* Neue Zeile ? */
	if( max_char >= lr_info.max_char )
	{
		/* Neue Zeile beginnen */
		fputs( new_line, parser_file );

		/* max_char zuruecksetzen */
		max_char = 0;
	}
}


WORD output_debug( BYTE *string, FILE *file )
{
	/*
		string wird nur ausgegeben, wenn die Debug-Version des Parsers
		erzeugt werden soll.
	*/

	/* Debug-Version ? */
	if( !lr_info.debug )
		/* Ja, string ausgeben */
		return( fputs( string, file ) );

	/* War nicht die Debug-Version */
	return( -1 );
}


WORD cmp_error_index( HULLE_STR *hulle1, HULLE_STR *hulle2 )
{
	/* Hier werden die FehlereintrÑge der beiden HÅllen auf Gleichheit geprueft. */
	REG ULONG	i, j;
	LONG		anz_err = 0;

	/* Alle EintrÑge der ersten HÅlle */
	for( i = 0; i < hulle1->anz_error_index; i++ )
	{
		/* Alle EintrÑge der ersten HÅlle */
		for( j = 0; j < hulle2->anz_error_index; j++ )
		{
			/* Sind die EintrÑge gleich ? */
			if( hulle1->error_index[i] == hulle2->error_index[j] )
			{
				/* Ja */
				anz_err++;
				break;
			}
		}
	}

	/* Entsprechenden Funktionswert zurueckliefern */
	return( hulle1->anz_error_index == anz_err ? 0 : 1 );
}


VOID output_error( BYTE *delim, ERROR_STR *error_str )
{
	/* Hier wird eine Fehlerbehandlung im Parser-Quelltext ausgegeben. */

	if( error_str->type[0] & 0x10 )
		/* Semmentische Aktionen zulassen */
		fprintf( parser_file, "%s/* Semmantische Aktionen zulassen */\n%ssem_flg = 0;\n\n", delim, delim );

	/* Ueberlesen von Terminalen ? */
	if( error_str->type[0] & 0x01 )
		/* Ja */
		fprintf( parser_file, "%swhile( lookahead != %s )\n%s\tlookahead = scanner();\n\n%sreturn( 0 );\n", delim, hash_tab[error_str->type[1]].name, delim, delim );

	if( error_str->type[0] & 0x02 )
	{
		/* Terminal einfuegen */

		/* Debug-Version ? */
		if( !lr_info.debug )
		{
			/* Ja, komprimierte LR-Tabelle ? */
			if( !lr_info.compress )
				/* Ja */
				fprintf( parser_file, "%sshift( lr_tab[c_lr[state]][%s], old_lookahead, lookahead );\n\n", delim, hash_tab[error_str->type[1]].name );

			else
				/* Nein */
				fprintf( parser_file, "%sshift( lr_tab[state][%s], old_lookahead, lookahead );\n\n", delim, hash_tab[error_str->type[1]].name );
		}

		else
		{
			/* Non-Debug, komprimierte LR-Tabelle ? */
			if( !lr_info.compress )
				/* Ja */
				fprintf( parser_file, "%spush( lr_tab[c_lr[state]][%s] );\n\n", delim, hash_tab[error_str->type[1]].name );

			else
				/* Nein */
				fprintf( parser_file, "%spush( lr_tab[state][%s] );\n\n", delim, hash_tab[error_str->type[1]].name );
		}
	}

	if( error_str->type[0] & 0x04 )
		/* Terminal lîschen */
		fprintf( parser_file, "%s/* Terminal lîschen */\n%slookahead = scanner();\n%sreturn( 0 );\n\n", delim, delim, delim );

	if( error_str->type[0] & 0x08 )
		/* Parsing beenden */
		fprintf( parser_file, "%s/* Parsing beenden */\n%sreturn( -1 );\n", delim, delim );
}


VOID output_union( VOID )
{
	/* Hier wird die Union-Struktur ausgegeben. */

	/* Sind semmantische Aktionen benutzt worden ? */
	if( sem_act )
	{
		/* Ist eine Union-Struktur angegenben worden ? */
		if( union_struct )
		{
			/* Ja, ausgeben */
			fputs( "\n\n\n/* Typ-Definition fÅr die semmantischen Aktionen */\ntypedef union\n{\n", h_file );
			fputs( union_struct, h_file );
			fputs( "LR_TYPE;\n\n", h_file );
		}

		else
			/* Nein, ist Word */
			fputs( "\n\n\n/* Typ-Definition fÅr die semmantischen Aktionen */\ntypedef WORD LR_TYPE;\n\n", h_file );

		/* Speicher fÅr union_struct freigeben */
		free( ( VOID * ) union_struct );
	}
}


VOID output_init( VOID )
{
	/* Hier wird die Initialisierung ausgegeben. */

	/* Wurde diese ueberhaupt angegeben ? */
	if( init )
	{
		/* Ja, ausgeben */
		fputs( init, parser_file );

		/* Speicher fÅr init freigeben */
		free( ( VOID * ) init );
	}
}

VOID output_incl( VOID )
{
	/* Hier werden die Hilfsroutinen ausgegeben. */

	/* Wurden diese ueberhaupt angegeben ? */
	if( incl )
	{
		/* Ja, ausgeben */
		fputs( incl, parser_file );

		/* Speicher fÅr incl freigeben */
		free( ( VOID * ) incl );
	}
}