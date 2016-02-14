/*
	Projekt		:	LR-Parser-Generator V1.00û
	Modul		:	GEN_PARS.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/



/************/
/* Includes */
/************/

#include <portab.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ext.h>
#include <tos.h>

/* Include-Datei des Parser-Generators */
#include "LR.H"



/**************/
/* Prototypen */
/**************/

VOID output_text( BYTE *string, BYTE *new_line, WORD format );
WORD cmp_error_index( HULLE_STR *hulle1, HULLE_STR *hulle2 );
WORD error( WORD error_number, ULONG para1, ULONG para2  );
VOID output_right( FILE *file, LONG nterm_index );
WORD output_debug( FILE *file, BYTE *string );
WORD output_shift_reduce_array( VOID );
WORD generate_source_parser( VOID );
VOID output_reduce_struct( VOID );
VOID output_reduce_tab( VOID );
VOID output_follow_str( VOID );
VOID output_shift_tab( VOID );
VOID output_ziel_tab( VOID );
VOID output_jump_tab( VOID );
WORD generate_parser( VOID );
WORD generate_h_file( VOID );


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


/* Ausgabe in Datei */
FILE	*parser_file = 0L;

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
MGLOBAL ULONG		anz_cop_lr_tab;
MGLOBAL ULONG		nterm_length;
MGLOBAL LONG		term_count;
MGLOBAL LONG		nterm_count;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		*cop_lr_tab;
MGLOBAL BYTE		*file_name;




WORD generate_parser( VOID )
{
	/* Hier wird der Quelltext des Parsers erzeugt. */

	/* Include-Datei des Parser-Quelltextes erzeugen */
	if( generate_h_file() < 0 )
		/* Fehler */
		return( -1 );

	/* Parser-Quelltext erzeugen */
	if( generate_source_parser() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD generate_h_file( VOID )
{
	/* Hier wird das Include-File fÅr den erzeugten Parser erzeugt. */
	FILE		*h_file;
	NTERM_STR	*nterm;
	REG	ULONG	i = 0;
	BYTE		*temp_file_name;

	/* Backslash in Grammatikdateinamen suchen */
	temp_file_name = strrchr( file_name, ( WORD ) '\\' );

	/* War gar kein Backslash im Dateinamen ? */
	if( !temp_file_name )
		/* Dateinamen benutzen */
		temp_file_name = file_name;

	else
		/* Backslash lîschen */
		temp_file_name++;

	/* Bis zum Punkt kopieren */
	do
	{
		/* Zeichen kopieren */
		file_h_name[i] = temp_file_name[i++];

	}while( ( temp_file_name[i] != '.' ) && ( temp_file_name[i] != 0 ) );

	/* Extension erzeugen */
	file_h_name[i++] = '.';
	file_h_name[i++] = 'H';
	file_h_name[i] = 0;

	/* Datei îffnen */
	if( !( h_file = fopen( file_h_name, "w" ) ) )
		/* Fehler */
		return( error( CANNOT_OPEN_H_FILE, 0, 0 ) );

	/* Header schreiben */
	fprintf( h_file, "/*" );
	fprintf( h_file, INFO );

	/* Debug-Version ? */
	if( !( output_debug( h_file, "\n\nDebug-Version\n*/\n\n" ) ) )
		/* Nein */
		fprintf( h_file, "\n*/\n\n" );

	/* Alle Terminale ausgeben */
	fprintf( h_file, "\n/*************/\n/* Terminale */\n/*************/\n\n" );

	/* Alle Terminale */
	for( i = 0; i <= term_count; i++ )
		/* Terminal ausgeben */
		fprintf( h_file, "\n#define %s\t\t\t%lu", hash_tab[term_str[i].hash_tab_index].name, i );

	/* Produktionen inclusive der rechten Seiten ausgeben */
	fprintf( h_file, "\n\n\n/****************/\n/* Produktionen */\n/****************/\n\n" );

	/* Alle Produktionen ausgeben */
	for( i = 0; i <= nterm_count; i++ )
	{
		/* Zeiger auf NTERM_STR holen */
		nterm = nterm_str[i];

		/* Hat die Produktion Alternativen ? */
		if( !( nterm->alt ) && ( ( ( i + 1 ) < nterm_count ) && ( !( nterm_str[i + 1]->alt ) ) ) )
			/* Nein, Produktion ausgeben */
			fprintf( h_file, "#define %s\t\t\t%lu\t/* ", hash_tab[nterm->hash_tab_index].name, lr_info.debug == 0 ? i : ( ULONG ) nterm->anz_ri );

		else
		{
			/* Ja */
			sprintf( file_parser_name, "%s_%lu", hash_tab[nterm->hash_tab_index].name, i - ( i - nterm->alt ) );
			fprintf( h_file, "#define %s\t\t\t%lu\t/* ", file_parser_name, lr_info.debug == 0 ? i : ( ULONG ) nterm->anz_ri );
		}

		/* Rechte Seite ausgeben */
		output_right( h_file, i );

		/* Kommentar schliessen */
		fprintf( h_file, " */\n" );
	}

	/* Anlauf-Produktionen nach Reduktion ausgeben */
	fprintf( h_file, "\n\n\n/**************************************/\n"
					 "/* Anlauf-Produktionen nach Reduktion */\n"
					 "/**************************************/\n\n" );

	/* Alle Produktionen ohne Alternativen */
	for( i = 1; i <= nterm_count; i++ )
	{
		/* Zeiger auf NTERM_STR holen */
		nterm = nterm_str[i];

		/* Ist es eine Alternative ? */
		if( !nterm->alt )
		{
			/* Nein, Produktion in Groûschrift wandeln */
			strcpy( file_parser_name, hash_tab[nterm->hash_tab_index].name );
			fprintf( h_file, "\n#define %s %lu", strupr( file_parser_name ), nterm->prod_index );
		}
	}

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
	WORD		anz_error_str;
	BYTE		*temp_file_name, file_parser_name[256];

	/* Backslash in Grammatikdateinamen suchen */
	temp_file_name = strrchr( file_name, ( WORD ) '\\' );

	/* War gar kein Backslash im Dateinamen ? */
	if( !temp_file_name )
		/* Dateinamen benutzen */
		temp_file_name = file_name;

	else
		/* Backslash lîschen */
		temp_file_name++;

	/* Bis zum Punkt kopieren */
	do
	{
		/* Zeichen kopieren */
		file_parser_name[i] = temp_file_name[i++];

	}while( ( temp_file_name[i] != '.' ) && ( temp_file_name[i] != 0 ) );

	/* Extension erzeugen */
	file_parser_name[i++] = '.';
	file_parser_name[i++] = 'C';
	file_parser_name[i] = 0;

	/* Datei îffnen */
	if( !( parser_file = fopen( file_parser_name, "w" ) ) )
		/* Fehler */
		return( error( CANNOT_OPEN_PARSER_FILE, 0, 0 ) );

	/* Header schreiben */
	fprintf( parser_file, "/*" );
	fprintf( parser_file, INFO );
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
		/* Ja, Anzahl der reduzuertes ZustÑnde ausgeben */
		fprintf( parser_file, " ( minus %d ZustÑnde )", ( WORD ) ( ( hulle_count - anz_cop_lr_tab ) + 1 ) );

	/* Kommentar schliessen */
	fprintf( parser_file, "\n*/\n\n" );

	/* Includes aufnehmen */
	fprintf( parser_file,	"\n/************/"
							"\n/* Includes */"
							"\n/************/"
							"\n\n#include <portab.h>"
							"\n#include <stdio.h>"
							"\n#include <stdlib.h>" );

	/* Wenn Debug-Version */
	output_debug( parser_file, "\n#include <tos.h>" );

	/* Includedatei des Parser-Generators aufnehmen */
	fprintf( parser_file, "\n#include \"%s\"", file_h_name );

	/* Definitionen ausgeben */
	fprintf( parser_file,	"\n\n\n\n/****************/"
							"\n/* Definitionen */"
							"\n/****************/"
							"\n\n/* Grîûe des Stacks */"
							"\n#define STACK_SIZE\t100"
							"\n\n\n/* Anzahl der Terminale */"
							"\n#define ANZ_TERM\t%ld", term_count + 1 );

	/* FÅr Fehler */
	output_debug( parser_file,	"\n#define OK\t\t\t0"
								"\n#define ERROR\t\t1" );

	/* Ausgabemedium */
	fprintf( parser_file,	"\n\n\n/* Ausgabemedien */"
							"\n#define DEST_SCREEN	0	/* Ausgabe auf Bildschirm */"
							"\n#define DEST_FILE	1	/* Ausgabe in Datei */"
							"\n#define DEST_PRN	2	/* Ausgabe auf Drucker */" );

	/* Prototypen ausgeben */
	fprintf( parser_file,	"\n\n\n\n/**************/"
							"\n/* Prototypen */"
							"\n/**************/\n" );

	/* Wenn Debug-Version */
	output_debug( parser_file, "\nVOID shift( LONG shift_state, LONG old_lahead, LONG lahead, WORD err_state );" );

	/* Alle weiteren Prototypen ausgeben */
	fprintf( parser_file,	"\nMGLOBAL WORD scanner( VOID );"
							"\nVOID reduce( LONG anz_elem );"
							"\nVOID push( LONG value );"
							"\nWORD parse( VOID );"
							"\nVOID error( VOID );"
							"\nLONG pop( VOID );" );

	/* Deklarationen ausgeben */
	fprintf( parser_file,
							"\n\n\n\n/*****************/"
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
							"\nWORD\terror_flg = 0;" );

	/* Wenn Debug-Version */
	output_debug( parser_file,	"\n\n/* Altes Lookahead */"
								"\nWORD\told_lookahead = - 1;"
								"\n\n/* Alter Status */"
								"\nLONG\told_state;" );

	/* FÅr Ausgabe */
	fprintf( parser_file,	"\n\n/* Ausgabemedium festlegen */"
							"\nWORD\tdest_state = DEST_SCREEN;"
							"\n\n/* FÅr Log */"
							"\nFILE\t*output_file;" );

	/* Reduce-Tabelle ausgeben */
	output_reduce_tab();

	/* Shift-Tabelle ausgeben */
	output_shift_tab();

	/* Jump-Tabelle ausgeben */
	output_jump_tab();

	/* FOLLOW-Mengen ausgeben */
	output_follow_str();

	/* Ziel-Tabelle ausgeben */
	output_ziel_tab();

	/*
		Wenn die LR-Tabelle komprimiert ausgegeben werden soll, wird nun
		das Array mit den Zeigern auf die Shift- und Reduce-Tabellen er-
		zeugt.
	*/
	if( output_shift_reduce_array() < 0 )
		/* Fehler */
		return( -1 );

	/* Bei Debug-Version Struktur ausgeben */
	if( lr_info.debug == ON )
		/* ZusÑtzliche Struktur ausgeben */
		output_reduce_struct();

	/* Einen Teil des Parser-Sources ausgeben */
	fprintf( parser_file,

"\n\nVOID error( VOID )\n{\n\t/* Hier werden die Fehler behandlet."
" */\n\tREG WORD\ti, j, p;\n\tULONG\t\tshift_state;\n\n\t/* Fehlermeldung "
"ausgeben ? */\n\tif( !error_flg )\n\t{\n\t\t/* Fehlerflag setzen */"
"\n\t\tanz_error++;\n\n\t\t/* Fehlerlawine verhindern */\n\t\terror_flg++;"
"\n\n" );

	/* Gegebenenfalls ein paar Fehlermeldungen in die error-Routine einbringen */
	if( error_struct )
	{
		/* Switch ausgeben */
		fprintf( parser_file, "\t\tswitch( ( WORD ) state )\n\t\t{" );

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
				fprintf( parser_file, "\t\t\t{\n" );

				/* ZurÅcksetzen */
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
						if( !( error_str = ( ERROR_STR ** ) realloc( ( VOID * ) error_str, sizeof( ERROR_STR * ) * ( anz_error_str + 1 ) ) ) )
							/* Fehler */
							return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

						/* error_str eintragen */
						error_str[anz_error_str++] = temp_error_str;

						/* Zeiger auf nÑchste ERROR_STR holen */
						temp_error_str = temp_error_str->error_str;
					}
				}

				/* Nach einer General-Fehlermeldung suchen */
				for( p = 0; p < ( anz_error_str - 1 ); p++ )
				{
					/* General-Fehlermeldung ? */
					if( !( error_str[p]->term ) )
					{
						/* Ja, an letzter Position */
						temp_error_str = error_str[anz_error_str - 1];
						error_str[anz_error_str - 1] = error_str[p];
						error_str[p] = temp_error_str;
					}
				}

				/* Gibt es nur eine Fehlermeldung und ohne Terminal ? */
				if( ( anz_error_str == 1 ) && !( error_str[0]->term ) )
					/* Ja, nur fprintf ausgeben */
					fprintf( parser_file, "\t\t\t\tfprintf( output_file, %s );\n", error_str[0]->err_msg );

				else
				{
					/* Die Fehlermeldungen ausgeben */
					for( p = 0; p < anz_error_str; p++ )
					{
						/* Ist es eine General-Fehlermeldung ? */
						if( error_str[p]->term )
						{
							/* Nein, eine mittleren Fehlermeldung */

							/* Ist es die erste Fehlermeldung ? */
							if( p )
								/* Nein */
								fprintf( parser_file, "\t\t\t\telse if( lookahead == %s )\n", hash_tab[error_str[p]->term].name );

							else
								/* Ja */
								fprintf( parser_file, "\t\t\t\tif( lookahead == %s )\n", hash_tab[error_str[p]->term].name );

							/* Den Fehlertext ausgeben */
							fprintf( parser_file, "\t\t\t\t\tfprintf( output_file, %s );\n\n", error_str[p]->err_msg );
						}

						else
							/* Ja, ausgeben */
							fprintf( parser_file, "\t\t\t\telse\n\t\t\t\t\tfprintf( output_file, %s );\n\n", error_str[p]->err_msg );
					}
				}

				/* Case abschliessen */
				fprintf( parser_file, "\t\t\t\tbreak;\n\t\t\t}\n" );

				/* Speicher fÅr Fehlermeldungen freigeben */
				free( ( VOID * ) hulle->error_index );

				/* Speicher freigeben */
				free( ( VOID * ) error_str );
			}
		}

		/* Default einfÅgen */
		fprintf( parser_file, "\n\t\t\tdefault:\n\t\t\t\tfprintf( output_file, \"\\n\\nFehler in Zustand %%ld !\", state );\n" );

		/* Switch abschliessen */
		fprintf( parser_file, "\t\t}\n\t}\n\n" );
	}

	/* Die Debug-Version erzeugen ? */
	if( !lr_info.debug )
	{
		/* Debug-Version erzeugen */

		/* LR-Tabelle komprimiert ? */
		if( !lr_info.compress )
			/* Ja */
			fprintf( parser_file,

"\t/* Alle Terminale */\n\tfor( i = 0; i < ANZ_TERM; i++ )\n\t{\n\t\t/"
"* Shift von i mîglich ? */\n\t\tif( shift_state = comp_lr_tab[st"
"ate].shift[i] )\n\t\t{\n\t\t\t/* Shift von lookahead mîglich ? */\n\t\t"
"\tif( comp_lr_tab[shift_state].shift[lookahead] )\n\t\t\t{\n\t\t\t\t/* i schi"
"eben */\n\t\t\t\tshift( comp_lr_tab[state].shift[i], old_lookahead"
", i, ERROR );\n\n\t\t\t\t/* Geschobenes Terminal merken */\n\t\t\t\told_"
"lookahead = i;\n\n\t\t\t\t/* Alles OK */\n\t\t\t\treturn;\n\t\t\t}\n\n\t\t\t/* Re"
"duziere-Zustand ? */\n\t\t\telse if( comp_lr_tab[shift_state].red"
"uce[lookahead] )\n\t\t\t{\n\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t/* Ist"
" lookahead in FOLLOW( comp_lr_tab[shift_state].reduce[i] ) ? "
"*/\n\t\t\t\tj = 0;\n\n\t\t\t\t/* Die passende FOLLOW-Struktur holen */\n\t"
"\t\t\tdo\n\t\t\t\t{\n\t\t\t\t\t/* Ist es die richtige ? */\n\t\t\t\t\tif( ( follo"
"w[j].beginn <= comp_lr_tab[shift_state].reduce[i] ) && ( foll"
"ow[j].end >= comp_lr_tab[shift_state].reduce[i] ) )\n\t\t\t\t\t\t/* "
"Ja */\n\t\t\t\t\t\tbreak;\n\n\t\t\t\t\telse\n\t\t\t\t\t\t/* Nein */\n\t\t\t\t\t\tj++;\n\n\t\t"
"\t\t}while( 1 );\n\n\t\t\t\t/* Alle FOLLOW-Elemente */\n\t\t\t\tfor( p = 0"
"; p < follow[j].anz_follow_elem; p++ )\n\t\t\t\t{\n\t\t\t\t\t/* Ist es l"
"ookahead ? */\n\t\t\t\t\tif( follow[j].follow[p] == lookahead )\n\t\t\t"
"\t\t{\n\t\t\t\t\t\t/* i schieben */\n\t\t\t\t\t\tshift( comp_lr_tab[state].sh"
"ift[i], old_lookahead, i, ERROR );\n\n\t\t\t\t\t\t/* Geschobenes Term"
"inal merken */\n\t\t\t\t\t\told_lookahead = i;\n\n\t\t\t\t\t\t/* Alles OK */"
"\n\t\t\t\t\t\treturn;\n\t\t\t\t\t}\n\t\t\t\t}\n\n\t\t\t\t/* Meldung ausgeben */\n\t\t\t\tf"
"printf( output_file, \"\\n\\nLîsche Terminal %%s !\", term_name[lo"
"okahead] );\n\n\t\t\t\t/* Lookahead lîschen */\n\t\t\t\tlookahead = scan"
"ner();\n\t\t\t\ti = -1;\n\t\t\t}\n\t\t}\n\n"
"\t/* Konnte ein Terminal eingeschoben werden ? */\n"
"\tif( i == ANZ_TERM )\n\t\t/* Nein, lîschen */\n\t\tlookahead = scanner();\n"
"}\n\n\n\n\nVOID main( VOID )\n{\n\t/* "
"Ausgabemedium festlegen */\n\tswitch( dest_state )\n\t{\n\t\tcase DE"
"ST_PRN:\n\t\t{\n\t\t\t/* Ausgabe auf Drucker */\n\t\t\toutput_file = std"
"prn;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_SCREEN:\n\t\t{\n\t\t\t/* Ausgabe auf"
" Bildschirm */\n\t\t\toutput_file = stdout;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase"
" DEST_FILE:\n\t\t{\n\t\t\t/* Ausgabe in Datei */\n\n\t\t\t/* Datei zum Sc"
"hreiben îffnen */\n\t\t\toutput_file = freopen( \"PARSER.LOG\", \"w\""
", stdout );\n\t\t}\n\t}\n\n\t/* Stack initialisieren */\n\tpush( 0 );\n\n"
"\t/* Eingabesymbol holen */\n\tlookahead = scanner();\n\n\t/* Parse"
"n */\n\tparse();\n\n\t/* Datei schliessen */\n\tfclose( output_file "
");\n}\n\n\nVOID push( LONG value )\n{\n\t/* Stackpointer erhîhen */\n"
"\tstackpointer++;\n\n\t/* öberlauf ? */\n\tif( stackpointer > STACK"
"_SIZE )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStackoverflow !\" );\n\t"
"\texit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben */\n\tstack[stackpo"
"inter] = value;\n}\n\n\nLONG pop( VOID )\n{\n\t/* Stackpointer dekre"
"mentieren */\n\tstackpointer--;\n\n\t/* \"Unterlauf\" ? */\n\tif( stac"
"kpointer < -1 )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStackunderflo"
"w !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurÅckliefern */\n\treturn( s"
"tack[stackpointer + 1] );\n}\n\n\nWORD parse( VOID )\n{\n\tLONG\tsymb"
"ol, i, taste;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t/"
"* Aktuellen Zustand holen */\n\t\tstate = stack[stackpointer];\n\n"
"\t\t/* Ist die Eingabe geparst ? */\n\t\tif( ( state == 1 ) && ( l"
"ookahead == ACCEPT ) )\n\t\t{\n\t\t\t/* Gab es Fehler ? */\n\t\t\tif( er"
"ror_val )\n\t\t\t\t/* Ja */\n\t\t\t\tfprintf( output_file, \"\\n\\nEs gab "
"%%d Fehler !\", anz_error );\n\n\t\t\telse\n\t\t\t\t/* Alles OK */\n\t\t\t\tfp"
"rintf( output_file, \"\\n\\nEingabe war OK\\n\" );\n\n\t\t\t/* Ende */\n"
"\t\t\texit( 0 );\n\t\t}\n\n\t\t/* Schieben ? */\n\t\telse if( comp_lr_tab["
"state].shift[lookahead] > 0 )\n\t\t{\n\t\t\t/* Schieben */\n\t\t\tshift("
" comp_lr_tab[state].shift[lookahead], old_lookahead, lookahea"
"d, OK );\n\n\t\t\t/* Altes lookahead merken */\n\t\t\told_lookahead = "
"lookahead;\n\n\t\t\t/* Neues Eingabesymbol holen */\n\t\t\tlookahead ="
" scanner();\n\n\t\t\t/* Fehlerlawine verhindern */\n\t\t\terror_flg = 0;\n\t\t}\n\n\t\t/* Reduzieren */\n\t\telse if( comp_lr_tab[st"
"ate].reduce[lookahead] > 0 )\n\t\t{\n\t\t\t/* Zustand nach Reduktion"
" holen */\n\t\t\tsymbol = ziel[state];\n\n\t\t\t/* Reduzieren */\n\t\t\tre"
"duce( comp_lr_tab[state].reduce[lookahead] );\n\n\t\t\t/* Alten Zu"
"stand merken */\n\t\t\told_state = state;\n\n\t\t\t/* Neuen Zustand ho"
"len */\n\t\t\tstate = stack[stackpointer];\n\n\t\t\t/* Meldung ausgebe"
"n */\n\t\t\tfprintf( output_file, \"\\n\\nReduziere zu  : %%s\", nterm"
"[comp_lr_tab[old_state].reduce[lookahead]].prod_name );\n\t\t\tfp"
"rintf( output_file, \"\\nNeuer Status  : %%ld\", jump_tab[state][s"
"ymbol - 1] );\n\t\t\tfprintf( output_file, \"\\nAlter Status  : %%ld\""
", old_state );\n\t\t\tfprintf( output_file, \"\\nNeues Terminal: %%s"
"\", term_name[lookahead] );\n\n\t\t\t/* Haben wir ein altes Termina"
"l ? */\n\t\t\tif( old_lookahead >= 0 )\n\t\t\t\t/* Wird haben ein alte"
"s Terminal */\n\t\t\t\tfprintf( output_file, \"\\nAltes Terminal: %%s"
"\", term_name[old_lookahead] );\n\n\t\t\t/* Zustand nach Reduktion "
"herstellen */\n\t\t\tif( jump_tab[state][symbol - 1] != 0 )\n\t\t\t\t/"
"* Wert auf Stack schieben */\n\t\t\t\tpush( jump_tab[state][symbol"
" - 1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t\t/* Fehler beim Springen */\n\t\t\t\tfpr"
"intf( output_file, \"\\n\\nFehler beim Springen !\"\\\n\t\t\t\t\t\t\"\\nAkt"
"ueller Status  : %%ld\"\\\n\t\t\t\t\t\t\"\\nAktuelles Terminal: %%s\", old_sta"
"te, term_name[lookahead] );\n\n\t\t\t\t/* Ende */\n\t\t\t\texit( 1 );\n\t\t"
"\t}\n\t\t}\n\n\t\t/* Syntaxfehler ? */\n\t\telse if( comp_lr_tab[state]."
"shift[lookahead] == 0 )\n\t\t\t/* Fehler */\n\t\t\terror();\n\n\t\t/* Aus"
"gabe auf Bildschirm ? */\n\t\tif( dest_state == DEST_SCREEN )\n\t\t"
"{\n\t\t\t/* Auf Tastendruck warten */\n\t\t\ttaste = Cnecin();\n\n\t\t\t/*"
" Stack ausgeben ? */\n\t\t\tif( ( taste & 0x000000ff ) == 0x73 )\n"
"\t\t\t{\n\t\t\t\t/* Stack ausgeben */\n\t\t\t\tfprintf( output_file, \"\\n\\n"
"Stack\\n-----\\n\" );\n\n\t\t\t\t/* Alle EintrÑge im Stack ausgeben */"
"\n\t\t\t\tfor( i = 0; i < stackpointer; i++ )\n\t\t\t\t\t/* Eintrag ausg"
"eben */\n\t\t\t\t\tfprintf( output_file, \"%%ld\\n\", stack[i] );\n\t\t\t}\n\t"
"\t}\n\n\t\telse\n\t\t{\n\t\t\t/* Auf dem Drucker/Datei immer den Stack au"
"sgeben */\n\n\t\t\t/* Stack ausgeben */\n\t\t\tfprintf( output_file, \""
"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t/* Alle EintrÑge im Stack ausgeben"
" */\n\t\t\tfor( i = 0; i < stackpointer; i++ )\n\t\t\t\t/* Eintrag aus"
"geben */\n\t\t\t\tfprintf( output_file, \"%%ld\\n\", stack[i] );\n\t\t}\n\t}"
"\n}\n\nVOID shift( LONG shift_state, LONG old_lahead, LONG lahea"
"d, WORD err_state )\n{\n\t/* Hier wird ein Eintrag aus der Shift"
"-Tabelle auf den Stack geschoben. */\n\n\t/* Eintrag auf Stack s"
"chieben */\n\tpush( shift_state );\n\n\t/* Fehler ? */\n\tif( err_st"
"ate == ERROR )\n\t\t/* Fehler */\n\t\tfprintf( output_file, \"\\n\\nFe"
"hler !\" );\n\n\t/* Meldung ausgeben */\n\tfprintf( output_file, \"\\"
"n\\nSchiebe       : %%s\", term_name[lahead] );\n\tfprintf( output"
"_file, \"\\nNeuer Status  : %%ld\", shift_state );\n\tfprintf( outpu"
"t_file, \"\\nAlter Status  : %%ld\", state );\n\tfprintf( output_fil"
"e, \"\\nNeues Terminal: %%s\", term_name[lahead] );\n\n\t/* Haben wi"
"r ein altes Terminal ? */\n\tif( old_lahead >= 0 )\n\t\t/* Wird ha"
"ben ein altes Terminal */\n\t\tfprintf( output_file, \"\\nAltes Te"
"rminal: %%s\", term_name[old_lahead] );\n}\n\nVOID reduce( LONG an"
"z_elem )\n{\n\t/* Hier wird reduziert. */\n\tREG WORD i;\n\n\t/* Elem"
"ente vom Stack lîschen */\n\tfor( i = 0; i < nterm[anz_elem].an"
"z_ri; i++ )\n\t\t/* Eintrag vom Stack lîschen */\n\t\tpop();\n}" );

	else
		/* Nicht komprimierte LR-Tabelle */
		fprintf( parser_file,

"\t/* Alle Terminale */\n\tfor( i = 0; i < ANZ_TERM; i++ )\n"
"\t{\n\t\t/* Shift von i mîglich ? */\n\t\tif( shift_state = shift_ta"
"b[state][i] )\n\t\t{\n\t\t\t/* Shift von lookahead mîglich ? */\n\t\t\ti"
"f( shift_tab[shift_state][lookahead] )\n\t\t\t{\n\t\t\t\t/* i schieben"
" */\n\t\t\t\tshift( shift_tab[state][i], old_lookahead, i, ERROR )"
";\n\n\t\t\t\t/* Geschobenes Terminal merken */\n\t\t\t\told_lookahead = "
"i;\n\n\t\t\t\t/* Alles OK */\n\t\t\t\treturn;\n\t\t\t}\n\n\t\t\t/* Reduziere-Zust"
"and ? */\n\t\t\telse if( reduce_tab[shift_state][lookahead] )\n\t\t\t"
"{\n\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t/* Ist lookahead in FOLLOW"
"( reduce_tab[shift_state][i] ) ? */\n\t\t\t\tj = 0;\n\n\t\t\t\t/* Die pa"
"ssende FOLLOW-Struktur holen */\n\t\t\t\tdo\n\t\t\t\t{\n\t\t\t\t\t/* Ist es d"
"ie richtige ? */\n\t\t\t\t\tif( ( follow[j].beginn <= reduce_tab[sh"
"ift_state][i] ) && ( follow[j].end >= reduce_tab[shift_state]"
"[i] ) )\n\t\t\t\t\t\t/* Ja */\n\t\t\t\t\t\tbreak;\n\n\t\t\t\t\telse\n\t\t\t\t\t\t/* Nein "
"*/\n\t\t\t\t\t\tj++;\n\n\t\t\t\t}while( 1 );\n\n\t\t\t\t/* Alle FOLLOW-Elemente "
"*/\n\t\t\t\tfor( p = 0; p < follow[j].anz_follow_elem; p++ )\n\t\t\t\t{"
"\n\t\t\t\t\t/* Ist es lookahead ? */\n\t\t\t\t\tif( follow[j].follow[p] ="
"= lookahead )\n\t\t\t\t\t{\n\t\t\t\t\t\t/* i schieben */\n\t\t\t\t\t\tshift( shif"
"t_tab[state][i], old_lookahead, i, ERROR );\n\n\t\t\t\t\t\t/* Geschob"
"enes Terminal merken */\n\t\t\t\t\t\told_lookahead = i;\n\n\t\t\t\t\t\t/* Al"
"les OK */\n\t\t\t\t\t\treturn;\n\t\t\t\t\t}\n\t\t\t\t}\n\n\t\t\t\t/* Meldung ausgeben"
" */\n\t\t\t\tfprintf( output_file, \"\\n\\nLîsche Terminal %%s !\", ter"
"m_name[lookahead] );\n\n\t\t\t\t/* Lookahead lîschen */\n\t\t\t\tlookahe"
"ad = scanner();\n\t\t\t\ti = -1;\n\t\t\t}\n\t\t}\n\t}\n\n"
"\t/* Konnte ein Terminal eingeschoben werden ? */\n"
"\tif( i == ANZ_TERM )\n\t\t/* Nein, lîschen */\n\t\tlookahead = scanner();\n"
"}\n\n\n\n\nVOID main( VOID"
" )\n{\n\t/* Ausgabemedium festlegen */\n\tswitch( dest_state )\n\t{\n"
"\t\tcase DEST_PRN:\n\t\t{\n\t\t\t/* Ausgabe auf Drucker */\n\t\t\toutput_f"
"ile = stdprn;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_SCREEN:\n\t\t{\n\t\t\t/* Au"
"sgabe auf Bildschirm */\n\t\t\toutput_file = stdout;\n\t\t\tbreak;\n\t\t"
"}\n\n\t\tcase DEST_FILE:\n\t\t{\n\t\t\t/* Ausgabe in Datei */\n\n\t\t\t/* Dat"
"ei zum Schreiben îffnen */\n\t\t\toutput_file = freopen( \"PARSER."
"LOG\", \"w\", stdout );\n\t\t}\n\t}\n\n\t/* Stack initialisieren */\n\tpus"
"h( 0 );\n\n\t/* Eingabesymbol holen */\n\tlookahead = scanner();\n\n"
"\t/* Parsen */\n\tparse();\n\n\t/* Datei schliessen */\n\tfclose( out"
"put_file );\n}\n\n\nVOID push( LONG value )\n{\n\t/* Stackpointer er"
"hîhen */\n\tstackpointer++;\n\n\t/* öberlauf ? */\n\tif( stackpointe"
"r > STACK_SIZE )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStackoverflo"
"w !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben */\n\tstac"
"k[stackpointer] = value;\n}\n\n\nLONG pop( VOID )\n{\n\t/* Stackpoin"
"ter dekrementieren */\n\tstackpointer--;\n\n\t/* \"Unterlauf\" ? */\n"
"\tif( stackpointer < -1 )\n\t{\n\t\tfprintf( output_file, \"\\n\\nStac"
"kunderflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurÅckliefern */\n\t"
"return( stack[stackpointer + 1] );\n}\n\n\nWORD parse( VOID )\n{\n\t"
"LONG\tsymbol, i, taste;\n\n\t/* Endlosschleife */\n\twhile( 1"
" )\n\t{\n\t\t/* Aktuellen Zustand holen */\n\t\tstate = stack[stackpo"
"inter];\n\n\t\t/* Ist die Eingabe geparst ? */\n\t\tif( ( state == 1"
" ) && ( lookahead == ACCEPT ) )\n\t\t{\n\t\t\t/* Gab es Fehler ? */\n"
"\t\t\tif( anz_error )\n\t\t\t\t/* Ja */\n\t\t\t\tfprintf( output_file, \"\\n"
"\\nEs gab %%d Fehler !\", anz_error );\n\n\t\t\telse\n\t\t\t\t/* Alles OK "
"*/\n\t\t\t\tfprintf( output_file, \"\\n\\nEingabe war OK\\n\" );\n\n\t\t\t/*"
" Ende */\n\t\t\texit( 0 );\n\t\t}\n\n\t\t/* Schieben ? */\n\t\telse if( shi"
"ft_tab[state][lookahead] > 0 )\n\t\t{\n\t\t\t/* Schieben */\n\t\t\tshift"
"( shift_tab[state][lookahead], old_lookahead, lookahead, OK )"
";\n\n\t\t\t/* Altes lookahead merken */\n\t\t\told_lookahead = lookahe"
"ad;\n\n\t\t\t/* Neues Eingabesymbol holen */\n\t\t\tlookahead = scanne"
"r();\n\n\t\t\t/* Fehlerlawine verhindern */\n\t\t\terror_flg = 0;\n\t\t}\n\n\t\t/* Reduzieren */\n\t\telse if( reduce_tab[state][loo"
"kahead] > 0 )\n\t\t{\n\t\t\t/* Zustand nach Reduktion holen */\n\t\t\tsy"
"mbol = ziel[state];\n\n\t\t\t/* Reduzieren */\n\t\t\treduce( reduce_ta"
"b[state][lookahead] );\n\n\t\t\t/* Alten Zustand merken */\n\t\t\told_"
"state = state;\n\n\t\t\t/* Neuen Zustand holen */\n\t\t\tstate = stack"
"[stackpointer];\n\n\t\t\t/* Meldung ausgeben */\n\t\t\tfprintf( output"
"_file, \"\\n\\nReduziere zu  : %%s\", nterm[reduce_tab[old_state]["
"lookahead]].prod_name );\n\t\t\tfprintf( output_file, \"\\nNeuer St"
"atus  : %%ld\", jump_tab[state][symbol - 1] );\n\t\t\tfprintf( outpu"
"t_file, \"\\nAlter Status  : %%ld\", old_state );\n\t\t\tfprintf( outp"
"ut_file, \"\\nNeues Terminal: %%s\", term_name[lookahead] );\n\n\t\t\t"
"/* Haben wir ein altes Terminal ? */\n\t\t\tif( old_lookahead >= "
"0 )\n\t\t\t\t/* Wird haben ein altes Terminal */\n\t\t\t\tfprintf( outp"
"ut_file, \"\\nAltes Terminal: %%s\", term_name[old_lookahead] );\n"
"\n\t\t\t/* Zustand nach Reduktion herstellen */\n\t\t\tif( jump_tab[s"
"tate][symbol - 1] != 0 )\n\t\t\t\t/* Wert auf Stack schieben */\n\t\t"
"\t\tpush( jump_tab[state][symbol - 1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t\t/* F"
"ehler beim Springen */\n\t\t\t\tfprintf( output_file, \"\\n\\nFehler "
"beim Springen !\"\\\n\t\t\t\t\t\t\"\\nAktueller Status: %%ld\"\\\n\t\t\t\t\t\t\"\\nAk"
"tuelles Terminal: %%s\", old_state, term_name[lookahead] );\n\n\t\t"
"\t\t/* Ende */\n\t\t\t\texit( 1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syntaxfehler ? */\n"
"\t\telse if( shift_tab[state][lookahead] == 0 )\n\t\t\t/* Fehler */"
"\n\t\t\terror();\n\n\t\t/* Ausgabe auf Bildschirm ? */\n\t\tif( dest_sta"
"te == DEST_SCREEN )\n\t\t{\n\t\t\t/* Auf Tastendruck warten */\n\t\t\tta"
"ste = Cnecin();\n\n\t\t\t/* Stack ausgeben ? */\n\t\t\tif( ( taste & 0"
"x000000ff ) == 0x73 )\n\t\t\t{\n\t\t\t\t/* Stack ausgeben */\n\t\t\t\tfprin"
"tf( output_file, \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t\t/* Alle EintrÑg"
"e im Stack ausgeben */\n\t\t\t\tfor( i = 0; i < stackpointer; i++ "
")\n\t\t\t\t\t/* Eintrag ausgeben */\n\t\t\t\t\tfprintf( output_file, \"%%ld\\"
"n\", stack[i] );\n\t\t\t}\n\t\t}\n\n\t\telse\n\t\t{\n\t\t\t/* Auf dem Drucker/Da"
"tei immer den Stack ausgeben */\n\n\t\t\t/* Stack ausgeben */\n\t\t\tf"
"printf( output_file, \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t/* Alle Eint"
"rÑge im Stack ausgeben */\n\t\t\tfor( i = 0; i < stackpointer; i+"
"+ )\n\t\t\t\t/* Eintrag ausgeben */\n\t\t\t\tfprintf( output_file, \"%%ld\\"
"n\", stack[i] );\n\t\t}\n\t}\n}\n\nVOID shift( LONG shift_state, LONG "
"old_lahead, LONG lahead, WORD err_state )\n{\n\t/* Hier wird ein"
" Eintrag aus der Shift-Tabelle auf den Stack geschoben. */\n\n\t"
"/* Eintrag auf Stack schieben */\n\tpush( shift_state );\n\n\t/* F"
"ehler ? */\n\tif( err_state == ERROR )\n\t\t/* Fehler */\n\t\tfprintf"
"( output_file, \"\\n\\nFehler !\" );\n\n\t/* Meldung ausgeben */\n\tfp"
"rintf( output_file, \"\\n\\nSchiebe       : %%s\", term_name[lahea"
"d] );\n\tfprintf( output_file, \"\\nNeuer Status  : %%ld\", shift_st"
"ate );\n\tfprintf( output_file, \"\\nAlter Status  : %%ld\", state )"
";\n\tfprintf( output_file, \"\\nNeues Terminal: %%s\", term_name[la"
"head] );\n\n\t/* Haben wir ein altes Terminal ? */\n\tif( old_lahe"
"ad >= 0 )\n\t\t/* Wird haben ein altes Terminal */\n\t\tfprintf( ou"
"tput_file, \"\\nAltes Terminal: %%s\", term_name[old_lahead] );\n}"
"\n\nVOID reduce( LONG anz_elem )\n{\n\t/* Hier wird reduziert. */\n"
"\tREG WORD i;\n\n\t/* Elemente vom Stack lîschen */\n\tfor( i = 0; "
"i < nterm[anz_elem].anz_ri; i++ )\n\t\t/* Eintrag vom Stack lîsc"
"hen */\n\t\tpop();\n}" );

	}

	else
	{
		/* Die normale Parser-Version erzeugen */

		/* LR-Tabelle komprimiert ? */
		if( !lr_info.compress )
			/* Ja */
			fprintf( parser_file,

"\t/* Alle Terminale */\n\tfor( i = 0; i < ANZ_TERM; i++ )\n\t{\n\t\t/"
"* Shift von i mîglich ? */\n\t\tif( shift_state = comp_lr_tab[st"
"ate].shift[i] )\n\t\t{\n\t\t\t/* Shift von lookahead mîglich ? */\n\t\t"
"\tif( comp_lr_tab[shift_state].shift[lookahead] )\n\t\t\t{\n\t\t\t\t/* "
"i schieben */\n\t\t\t\tpush( comp_lr_tab[state].shift[i] );\n\n\t\t\t\t/"
"* Alles OK */\n\t\t\t\treturn;\n\t\t\t}\n\n\t\t\t/* Reduziere-Zustand ? */\n"
"\t\t\telse if( comp_lr_tab[shift_state].reduce[lookahead] )\n\t\t\t{"
"\n\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t/* Ist lookahead in FOLLOW("
" comp_lr_tab[shift_state].reduce[i] ) ? */\n\t\t\t\tj = 0;\n\n\t\t\t\t/*"
" Die passende FOLLOW-Struktur holen */\n\t\t\t\tdo\n\t\t\t\t{\n\t\t\t\t\t/* I"
"st es die richtige ? */\n\t\t\t\t\tif( ( follow[j].beginn <= comp_l"
"r_tab[shift_state].reduce[i] ) && ( follow[j].end >= comp_lr_"
"tab[shift_state].reduce[i] ) )\n\t\t\t\t\t\t/* Ja */\n\t\t\t\t\t\tbreak;\n\n\t"
"\t\t\t\telse\n\t\t\t\t\t\t/* Nein */\n\t\t\t\t\t\tj++;\n\n\t\t\t\t}while( 1 );\n\n\t\t\t\t/"
"* Alle FOLLOW-Elemente */\n\t\t\t\tfor( p = 0; p < follow[j].anz_f"
"ollow_elem; p++ )\n\t\t\t\t{\n\t\t\t\t\t/* Ist es lookahead ? */\n\t\t\t\t\tif"
"( follow[j].follow[p] == lookahead )\n\t\t\t\t\t{\n\t\t\t\t\t\t/* i schieb"
"en */\n\t\t\t\t\t\tpush( comp_lr_tab[state].shift[i] );\n\n\t\t\t\t\t\t/* Al"
"les OK */\n\t\t\t\t\t\treturn;\n\t\t\t\t\t}\n\t\t\t\t}\n\n\t\t\t\t/* Lookahead lîsche"
"n */\n\t\t\t\tlookahead = scanner();\n\t\t\t\ti = -1;\n\t\t\t}\n\t\t}\n\t}\n\n"
"\t/* Konnte ein Terminal eingeschoben werden ? */\n"
"\tif( i == ANZ_TERM )\n\t\t/* Nein, lîschen */\n\t\tlookahead = scanner();\n\n\n\n\n"
"\nVOID main( VOID )\n{\n\t/* Ausgabemedium festlegen */\n\tswitch( "
"dest_state )\n\t{\n\t\tcase DEST_PRN:\n\t\t{\n\t\t\t/* Ausgabe auf Drucke"
"r */\n\t\t\toutput_file = stdprn;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_SCRE"
"EN:\n\t\t{\n\t\t\t/* Ausgabe auf Bildschirm */\n\t\t\toutput_file = stdo"
"ut;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_FILE:\n\t\t{\n\t\t\t/* Ausgabe in Dat"
"ei */\n\n\t\t\t/* Datei zum Schreiben îffnen */\n\t\t\toutput_file = f"
"reopen( \"PARSER.LOG\", \"w\", stdout );\n\t\t}\n\t}\n\n\t/* Stack initia"
"lisieren */\n\tpush( 0 );\n\n\t/* Eingabesymbol holen */\n\tlookahea"
"d = scanner();\n\n\t/* Parsen */\n\tparse();\n\n\t/* Datei schliessen"
" */\n\tfclose( output_file );\n}\n\n\nVOID push( LONG value )\n{\n\t/*"
" Stackpointer erhîhen */\n\tstackpointer++;\n\n\t/* öberlauf ? */\n"
"\tif( stackpointer > STACK_SIZE )\n\t{\n\t\tprintf( \"\\n\\nStackoverf"
"low !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben */\n\tst"
"ack[stackpointer] = value;\n}\n\n\nLONG pop( VOID )\n{\n\t/* Stackpo"
"inter dekrementieren */\n\tstackpointer--;\n\n\t/* \"Unterlauf\" ? *"
"/\n\tif( stackpointer < -1 )\n\t{\n\t\tprintf( \"\\n\\nStackunderflow !"
"\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurÅckliefern */\n\treturn( stac"
"k[stackpointer + 1] );\n}\n\n\nWORD parse( VOID )\n{\n\tLONG\tsymbol;"
"\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t/* Aktuellen Zustand"
" holen */\n\t\tstate = stack[stackpointer];\n\n\t\t/* Ist die Eingab"
"e geparst ? */\n\t\tif( ( state == 1 ) && ( lookahead == ACCEPT "
") )\n\t\t{\n\t\t\t/* Gab es Fehler ? */\n\t\t\tif( anz_error )\n\t\t\t\t/* Ja"
" */\n\t\t\t\tprintf( \"\\n\\nEs gab %%d Fehler !\", anz_error );\n\n\t\t\tel"
"se\n\t\t\t\t/* Alles OK */\n\t\t\t\tprintf( \"\\n\\nEingabe war OK\\n\" );\n\n"
"\t\t\t/* Ende */\n\t\t\texit( 0 );\n\t\t}\n\n\t\t/* Schieben ? */\n\t\telse if"
"( comp_lr_tab[state].shift[lookahead] > 0 )\n\t\t{\n\t\t\t/* Schiebe"
"n */\n\t\t\tpush( comp_lr_tab[state].shift[lookahead] );\n\n\t\t\t/* N"
"eues Eingabesymbol holen */\n\t\t\tlookahead = scanner();\n\t\t}\n\n\t\t"
"/* Reduzieren */\n\t\telse if( comp_lr_tab[state].reduce[lookahe"
"ad] > 0 )\n\t\t{\n\t\t\t/* Zustand nach Reduktion holen */\n\t\t\tsymbol"
" = ziel[state];\n\n\t\t\t/* Reduzieren */\n\t\t\treduce( comp_lr_tab[s"
"tate].reduce[lookahead] );\n\n\t\t\t/* Neuen Zustand holen */\n\t\t\ts"
"tate = stack[stackpointer];\n\n\t\t\t/* Zustand nach Reduktion her"
"stellen */\n\t\t\tif( jump_tab[state][symbol - 1] != 0 )\n\t\t\t\t/* W"
"ert auf Stack schieben */\n\t\t\t\tpush( jump_tab[state][symbol - "
"1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t\t/* Fehler beim Springen */\n\t\t\t\texit( "
"1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syntaxfehler ? */\n\t\telse if( comp_lr_tab["
"state].shift[lookahead] == 0 )\n\t\t\t/* Fehler */\n\t\t\terror();\n\t}"
"\n}\n\nVOID reduce( LONG anz_elem )\n{\n\t/* Hier wird reduziert. *"
"/\n\tREG WORD i;\n\n\t/* Elemente vom Stack lîschen */\n\tfor( i = 0"
"; i < anz_elem; i++ )\n\t\t/* Eintrag vom Stack lîschen */\n\t\tpop"
"();\n}" );

	else
		/* Nicht komprimierte LR-Tabelle */
		fprintf( parser_file,

"\t/* Alle Terminale */\n\tfor( i = 0; i < A"
"NZ_TERM; i++ )\n\t{\n\t\t/* Shift von i mîglich ? */\n\t\tif( shift_s"
"tate = shift_tab[state][i] )\n\t\t{\n\t\t\t/* Shift von lookahead mî"
"glich ? */\n\t\t\tif( shift_tab[shift_state][lookahead] )\n\t\t\t{\n\t\t"
"\t\t/* i schieben */\n\t\t\t\tpush( shift_tab[state][i] );\n\n\t\t\t\t/* A"
"lles OK */\n\t\t\t\treturn;\n\t\t\t}\n\n\t\t\t/* Reduziere-Zustand ? */\n\t\t\t"
"else if( reduce_tab[shift_state][lookahead] )\n\t\t\t{\n\t\t\t\t/* Red"
"uziere-Zustand */\n\n\t\t\t\t/* Ist lookahead in FOLLOW( reduce_tab"
"[shift_state][i] ) ? */\n\t\t\t\tj = 0;\n\n\t\t\t\t/* Die passende FOLLO"
"W-Struktur holen */\n\t\t\t\tdo\n\t\t\t\t{\n\t\t\t\t\t/* Ist es die richtige "
"? */\n\t\t\t\t\tif( ( follow[j].beginn <= reduce_tab[shift_state][i"
"] ) && ( follow[j].end >= reduce_tab[shift_state][i] ) )\n\t\t\t\t"
"\t\t/* Ja */\n\t\t\t\t\t\tbreak;\n\n\t\t\t\t\telse\n\t\t\t\t\t\t/* Nein */\n\t\t\t\t\t\tj++"
";\n\n\t\t\t\t}while( 1 );\n\n\t\t\t\t/* Alle FOLLOW-Elemente */\n\t\t\t\tfor( "
"p = 0; p < follow[j].anz_follow_elem; p++ )\n\t\t\t\t{\n\t\t\t\t\t/* Ist"
" es lookahead ? */\n\t\t\t\t\tif( follow[j].follow[p] == lookahead "
")\n\t\t\t\t\t{\n\t\t\t\t\t\t/* i schieben */\n\t\t\t\t\t\tpush( shift_tab[state]["
"i] );\n\n\t\t\t\t\t\t/* Alles OK */\n\t\t\t\t\t\treturn;\n\t\t\t\t\t}\n\t\t\t\t}\n\n\t\t\t\t/"
"* Lookahead lîschen */\n\t\t\t\tlookahead = scanner();\n\t\t\t\ti = -1;"
"\n\t\t\t}\n\t\t}\n\t}\n\n"
"\t/* Konnte ein Terminal eingeschoben werden ? */\n"
"\tif( i == ANZ_TERM )\n\t\t/* Nein, lîschen */\n\t\tlookahead = scanner();\n"
"}\n\n\n\n\nVOID main( VOID )\n{\n\t/* Stack initialisier"
"en */\n\tpush( 0 );\n\n\t/* Eingabesymbol holen */\n\tlookahead = sc"
"anner();\n\n\t/* Parsen */\n\tparse();\n}\n\n\nVOID push( LONG value )"
"\n{\n\t/* Stackpointer erhîhen */\n\tstackpointer++;\n\n\t/* öberlauf"
" ? */\n\tif( stackpointer > STACK_SIZE )\n\t{\n\t\tprintf( \"\\n\\nStac"
"koverflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben "
"*/\n\tstack[stackpointer] = value;\n}\n\n\nLONG pop( VOID )\n{\n\t/* S"
"tackpointer dekrementieren */\n\tstackpointer--;\n\n\t/* \"Unterlau"
"f\" ? */\n\tif( stackpointer < -1 )\n\t{\n\t\tprintf( \"\\n\\nStackunder"
"flow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurÅckliefern */\n\treturn"
"( stack[stackpointer + 1] );\n}\n\n\nWORD parse( VOID )\n{\n\tLONG\ts"
"ymbol;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t/* Aktuellen Z"
"ustand holen */\n\t\tstate = stack[stackpointer];\n\n\t\t/* Ist die "
"Eingabe geparst ? */\n\t\tif( ( state == 1 ) && ( lookahead == A"
"CCEPT ) )\n\t\t{\n\t\t\t/* Gab es Fehler ? */\n\t\t\tif( anz_error )\n\t\t\t"
"\t/* Ja */\n\t\t\t\tprintf( \"\\n\\nEs gab %%d Fehler !\", anz_error );\n"
"\n\t\t\telse\n\t\t\t\t/* Alles OK */\n\t\t\t\tprintf( \"\\n\\nEingabe war OK\\n"
"\" );\n\n\t\t\t/* Ende */\n\t\t\texit( 0 );\n\t\t}\n\n\t\t/* Schieben ? */\n\t\te"
"lse if( shift_tab[state][lookahead] > 0 )\n\t\t{\n\t\t\t/* Schieben "
"*/\n\t\t\tpush( shift_tab[state][lookahead] );\n\n\t\t\t/* Neues Einga"
"besymbol holen */\n\t\t\tlookahead = scanner();\n\t\t}\n\n\t\t/* Reduzie"
"ren */\n\t\telse if( reduce_tab[state][lookahead] > 0 )\n\t\t{\n\t\t\t/"
"* Zustand nach Reduktion holen */\n\t\t\tsymbol = ziel[state];\n\n\t"
"\t\t/* Reduzieren */\n\t\t\treduce( reduce_tab[state][lookahead] );"
"\n\n\t\t\t/* Neuen Zustand holen */\n\t\t\tstate = stack[stackpointer]"
";\n\n\t\t\t/* Zustand nach Reduktion herstellen */\n\t\t\tif( jump_tab"
"[state][symbol - 1] != 0 )\n\t\t\t\t/* Wert auf Stack schieben */\n"
"\t\t\t\tpush( jump_tab[state][symbol - 1] );\n\n\t\t\telse\n\t\t\t{\n\t\t\t\t/*"
" Fehler beim Springen */\n\t\t\t\texit( 1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syntax"
"fehler ? */\n\t\telse if( shift_tab[state][lookahead] == 0 )\n\t\t\t"
"/* Fehler */\n\t\t\terror();\n\t}\n}\n\nVOID reduce( LONG anz_elem )\n{"
"\n\t/* Hier wird reduziert. */\n\tREG WORD i;\n\n\t/* Elemente vom S"
"tack lîschen */\n\tfor( i = 0; i < anz_elem; i++ "
")\n\t\t/* Eintrag vom Stack lîschen */\n\t\tpop();\n}" );

	}

	/* Datei schliessen */
	if( fclose( parser_file ) == EOF )
		/* Fehler */
		return( error( CANNOT_CLOSE_PARSER_FILE, 0, 0 ) );

	/* ZurÅcksetzen */
	parser_file = 0L;

	/* Alles OK */
	return( 0 );
}


WORD output_shift_reduce_array( VOID )
{
	/*
		Wenn die LR-Tabelle komprimiert ausgegeben werden soll, wird nun
		das Array mit den Zeigern auf die Shift- und Reduce-Tabellen er-
		zeugt.
	*/
	FIX_STR		*fix_str;
	REG ULONG	i, j, p;
	LONG		fix_str_count = 0, fix_val;

	/* Soll die komprimierte Version erzeugt werden ? */
	if( !lr_info.compress )
	{
		/* Speicher fÅr die Korrekturwerte-Struktur holen */
		if( !( fix_str = ( FIX_STR * ) malloc( sizeof( FIX_STR ) * ( ( hulle_count - anz_cop_lr_tab ) + 1 ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Struktur-Deklaration ausgeben */
		fprintf( parser_file,	"\n\n\ntypedef struct\n{\n\t/* Zeiger auf Shift-Tabellen-Eintrag */\n\tLONG"
								"\t*shift;\n\n\t/* Zeiger auf Reduce-Tabellen-Eintrag */\n\tLONG\t*"
								"reduce;\n\n}COMP_LR_TAB;" );

		/* Deklaration ausgeben */
		fprintf( parser_file, "\n\n\nCOMP_LR_TAB comp_lr_tab[%lu] = {", hulle_count + 1 );

		/* Alle EintrÑge in der cop_lr_tab durchgehen */
		for( i = 0, j = 0; i <= hulle_count; i++ )
		{
			/* Nummer des Zustandes ausgeben */
			fprintf( parser_file, "\n/* %*lu */ ", state_length, i );

			/* HÅlle durch Komprimierung weggefallen ? */
			if( cop_lr_tab[i] < i )
			{
				/* Ist ein Korrekturwert zu beachten ? */
				if( fix_str_count )
				{
					/* Korrekturwert */
					fix_val = 0;

					/* Ja */
					for( p = 0; p < fix_str_count; p++ )
					{
						/* Grîûer ? */
						if( cop_lr_tab[i] > fix_str[p].state )
							/* Ja, Korrekturwert merken */
							fix_val = fix_str[p].fix_val;
					}

					/* Struktur îffnen */
					fprintf( parser_file, "{ shift_tab[%lu], reduce_tab[%lu] }", cop_lr_tab[i] - fix_val, cop_lr_tab[i] - fix_val );
				}

				else
					/* Nein, Struktur îffnen */
					fprintf( parser_file, "{ shift_tab[%lu], reduce_tab[%lu] }", cop_lr_tab[i], cop_lr_tab[i] );

				/* Damit die Werte auch richtig ausgegeben werden */
				j++;

				/* Korrekturwert merken */
				fix_str[fix_str_count].state = i;
				fix_str[fix_str_count++].fix_val = j;
			}

			else
			{
				/* Ist ein Korrekturwert zu beachten ? */
				if( fix_str_count )
				{
					/* Korrekturwert */
					fix_val = 0;

					/* Ja */
					for( p = 0; p < fix_str_count; p++ )
					{
						/* Grîûer ? */
						if( i > fix_str[p].state )
							/* Ja, Korrekturwert merken */
							fix_val = fix_str[p].fix_val;
					}

					/* Struktur îffnen */
					fprintf( parser_file, "{ shift_tab[%lu], reduce_tab[%lu] }", i - fix_val, i - fix_val );
				}

				else
					/* Nein, Struktur îffnen */
					fprintf( parser_file, "{ shift_tab[%lu], reduce_tab[%lu] }", i, i );
			}

			/* Letzter Eintrag ? */
			if( i != hulle_count )
				/* Komma ausgeben */
				fprintf( parser_file, "," );
		}

		/* Struktur schliessen */
		fprintf( parser_file, " };\n" );

		/* Speicher wieder freigeben */
		free( ( VOID * ) fix_str );
	}

	/* Alles OK */
	return( 0 );
}


VOID output_reduce_tab( VOID )
{
	/* Hier wird die Reduce-Tabelle ausgegeben. */
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
							"\n\nLONG\treduce_tab[%lu][ANZ_TERM] = {\n", type[lr_info.type], lr_info.compress != 0 ? hulle_count + 1 : anz_cop_lr_tab );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Soll die Tabelle komprimiert werden ? */
		if( !lr_info.compress )
		{
			/* Ja, wurde der Zustand schon ausgegeben ? */
			if( cop_lr_tab[i] != i )
				/* Ja, nÑchsten Zustand ausgeben */
				continue;
		}

		/* Nummer des Zustandes ausgeben */
		fprintf( parser_file, "/* %*lu */ ", state_length, i );

		/* Sich îffnende Klammer ausgeben */
		fprintf( parser_file, "{ " );

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

			else
				/* Null ausgeben */
				output_text( "0", "\n", ( WORD ) nterm_length + 2 );
		}

		/* Komma ausgeben ? */
		if( i == hulle_count )
			/* Nein, da letzte HÅlle */
			fprintf( parser_file, " }\n" );

		else
			/* Ja */
			fprintf( parser_file, " },\n" );

		/* Neue Zeile */
		max_char = 0;
	}

	/* Tabelle ist zu Ende */
	fprintf( parser_file, "};" );
}


VOID output_shift_tab( VOID )
{
	/* Hier wird die Shift-Tabelle ausgegeben. */
	REG ULONG	i, j;

	/* Anzahl der Zeichen in einer Zeile zurÅcksetzen */
	max_char = 0;

	/* Deklaration ausgeben */
	fprintf( parser_file, "\n\n\nLONG\tshift_tab[%lu][ANZ_TERM] = {\n", lr_info.compress != 0 ? hulle_count + 1 : anz_cop_lr_tab );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Soll die Tabelle komprimiert werden ? */
		if( !lr_info.compress )
		{
			/* Ja, wurde der Zustand schon ausgegeben ? */
			if( cop_lr_tab[i] != i )
				/* Ja, nÑchsten Zustand ausgeben */
				continue;
		}

		/* Nummer des Zustandes ausgeben */
		fprintf( parser_file, "/* %*lu */ ", state_length, i );

		/* Sich îffnende Klammer ausgeben */
		fprintf( parser_file, "{ " );

		/* Alle Terminale durchgehen */
		for( j = 0; j <= term_count; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Komma ausgeben */
				output_text( ", ", "\n            ", 0 );

			/* Shift-Eintrag ? */
			if( lr_tab[i][j].state == SHIFT )
			{
				/* Wert ausgeben */
				sprintf( file_parser_name, "%*lu", state_length, lr_tab[i][j].value );
				output_text( file_parser_name, "\n", 0 );
			}

			else
				/* Null ausgeben */
				output_text( "0", "\n", state_length );
		}

		/* Komma ausgeben ? */
		if( i == hulle_count )
			/* Nein, da letzte HÅlle */
			fprintf( parser_file, " }\n" );

		else
			/* Ja */
			fprintf( parser_file, " },\n" );

		/* Neue Zeile */
		max_char = 0;
	}

	/* Tabelle ist zu Ende */
	fprintf( parser_file, "};" );
}


VOID output_jump_tab( VOID )
{
	/* Hier wird die Shift-Tabelle ausgegeben. */
	REG ULONG	i, j;

	/* Anzahl der Zeichen in einer Zeile zurÅcksetzen */
	max_char = 0;

	/* Deklaration ausgeben */
	fprintf( parser_file, "\n\n\nLONG\tjump_tab[%lu][%lu] = {\n", hulle_count + 1, prod_count );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Nummer des Zustandes ausgeben */
		fprintf( parser_file, "/* %*lu */ ", state_length, i );

		/* Sich îffnende Klammer ausgeben */
		fprintf( parser_file, "{ " );

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
		if( i == hulle_count )
			/* Nein, da letzte HÅlle */
			fprintf( parser_file, " }\n" );

		else
			/* Ja */
			fprintf( parser_file, " },\n" );

		/* Neue Zeile */
		max_char = 0;
	}

	/* Tabelle ist zu Ende */
	fprintf( parser_file, "};" );
}


VOID output_reduce_struct( VOID )
{
	/*
		Hier wird bei der Debug-Version eine zusÑtzliche Struktur eingebunden,
		die die Namen der Produktionen enthÑlt.
	*/
	NTERM_STR	*nterm;
	REG ULONG	i;

	/* Struktur-Deklaration ausgeben */
	fprintf( parser_file,	"\n\n\ntypedef struct\n{\n\t/* Name der Produktion */\n\tBYTE\t*prod_name;\n"
							"\n\t/* Anzahl der Elemente auf der rechten Seite */\n\tWORD\tanz_ri;\n\n}NTERM;" );

	/* Definition */
	fprintf( parser_file, "\n\nNTERM\tnterm[] = {\n\n" );

	/* Alle Produktionen ausgeben */
	for( i = 0; i <= nterm_count; i++ )
	{
		/* Struktur îffnen */
		fprintf( parser_file, "{ " );

		/* Zeiger auf NTERM_STR holen */
		nterm = nterm_str[i];

		/* Hat die Produktion Alternativen ? */
		if( !( nterm->alt ) && ( ( ( i + 1 ) < nterm_count ) && ( !( nterm_str[i + 1]->alt ) ) ) )
			/* Nein, Produktion ausgeben */
			fprintf( parser_file, "\"%s\", ", hash_tab[nterm->hash_tab_index].name );

		else
		{
			/* Ja */
			sprintf( file_parser_name, "%s_%lu", hash_tab[nterm->hash_tab_index].name, i - ( i - nterm->alt ) );
			fprintf( parser_file, "\"%s\", ", file_parser_name );
		}

		/* Anzahl der Elemente auf der rechten Seite ausgeben */
		fprintf( parser_file, "%d", nterm->anz_ri );

		/* Struktur schliessen */
		fprintf( parser_file, " }" );

		/* War es die letzte Produktion ? */
		if( i < nterm_count )
			/* Nein, Komma ausgeben */
			fprintf( parser_file, "," );

		/* Kommentar îffnen */
		fprintf( parser_file, " /*" );

		/* Rechte Seite ausgeben */
		output_right( parser_file, i );

		/* Kommentar schliessen */
		fprintf( parser_file, " */\n" );
	}

	/* NTERM-Struktur schliessen */
	fprintf( parser_file, "};\n\n" );

	/* Die Strings zu den Terminalen ausgeben */

	/* Anzahl der Zeichen in einer Zeile zurÅcksetzen */
	max_char = 0;

	/* Definition ausgeben */
	fprintf( parser_file, "\nBYTE\t*term_name[ANZ_TERM] = { " );

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
	fprintf( parser_file, " };\n" );
}


VOID output_follow_str( VOID )
{
	/*
		Hier werden die FOLLOW-Mengen fÅr die automatische Fehlerbehand-
		lung ausgegeben.
	*/
	REG	ULONG	i, j;

	/* Alle Produktionen */
	for( i = 1; i <= nterm_count; )
	{
		/* Anzahl der Zeichen in einer Zeile zurÅcksetzen */
		max_char = 0;

		/* Deklaration ausgeben */
		sprintf( file_parser_name, "\n\nLONG\t%s_follow[] = { ", hash_tab[nterm_str[i]->hash_tab_index].name );
		output_text( file_parser_name, "\n", 0 );

		/* Alle FOLLOW-Terminale ausgeben */
		for( j = 0; j < nterm_str[i]->follow.anz_elem; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Ja */
				output_text( ", ", "\n", 0 );

			/* Terminal ausgeben */
			sprintf( file_parser_name, "%s", strupr( hash_tab[nterm_str[i]->follow.term_arr[j]].name ) );
			output_text( file_parser_name, "\n", 0 );
		}

		/* Struktur schliessen */
		fprintf( parser_file, " };" );

		/* Alternativen Åberspringen */
		while( ( ++i <= nterm_count ) && ( nterm_str[i]->alt ) )
			;
	}

	/* Struktur-Definition ausgeben */
	fprintf( parser_file,	"\n\ntypedef struct\n{\n\t/* FOLLOW-Menge gilt ab Alternative begin */\n\t" \
							"ULONG\tbeginn;\n\n\t/* FOLLOW-Menge gilt bis Alternative end */\n" \
							"\tULONG\tend;\n\n\t/* Anzahl der FOLLOW-Elemente */\n"
							"\tULONG\tanz_follow_elem;\n\n\t/* FOLLOW-Menge */\n" \
							"\tLONG\t*follow;\n\n}FOLLOW;\n\nFOLLOW\t" \
							"follow[] =\n{\n" );

	/* Eintrag in FOLLOW-Struktur zu jeder Produktion erzeugen */
	for( i = 1; i <= nterm_count; )
	{
		/* Struktur îffnen */
		fprintf( parser_file, "{ " );

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
	fprintf( parser_file, "};" );
}


VOID output_ziel_tab( VOID )
{
	/*
		Hier wird die ZustÑnge in eine Struktur eingetragen, die der Parser
		nach erfolgreicher Reduktion einnimmt.
	*/
	REG	ULONG	i;
	BYTE		text[256];

	/* Anzahl der Zeichen in einer Zeile zurÅcksetzen */
	max_char = 0;

	/* Definition ausgeben */
	fprintf( parser_file, "\n\nLONG\tziel[%lu] = { ", hulle_count + 1 );

	/* Alle HÅllen */
	for( i = 0; i <= hulle_count; i++ )
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
	fprintf( parser_file, " };" );
}


VOID output_text( BYTE *string, BYTE *new_line, WORD format )
{
	/*
		Hier wird der string unter Beachtung der max. Anzahl der Zeichen
		pro Zeile ausgegeben.
	*/

	/* String einrÅcken ? */
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
		fprintf( parser_file, new_line );

		/* max_char zurÅcksetzen */
		max_char = 0;
	}
}


WORD output_debug( FILE *file, BYTE *string )
{
	/*
		string wird nur ausgegeben, wenn die Debug-Version des Parsers
		erzeugt werden soll.
	*/

	/* Debug-Version ? */
	if( !lr_info.debug )
		/* Ja, string ausgeben */
		return( fprintf( file, string ) );

	/* War nicht die Debug-Version */
	return( 0 );
}


WORD cmp_error_index( HULLE_STR *hulle1, HULLE_STR *hulle2 )
{
	/* Hier werden die FehlereintrÑge der beiden HÅllen auf Gleichheit geprÅft. */
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

	/* Entsprechenden Funktionswert zurÅckliefern */
	return( hulle1->anz_error_index == anz_err ? 0 : 1 );
}