/*
	Projekt		:	SLR-Parser-Generator
	Modul		:	OUTPUT.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/


/* Includes */
#include "slr.h"
#include <portab.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* Defines */


/* Definitionen */
BYTE	include_name_h[13];


/* Globale Definitionen */
MGLOBAL PROD_HASH		*prod_hash;
MGLOBAL PROD_INFO		*prod_info;
MGLOBAL FIRST			term_str;
MGLOBAL SLR_INFO		slr_info;
MGLOBAL ULONG			prod_list_nr;
MGLOBAL ULONG			accept_hash_ind;
MGLOBAL ULONG			anz_bel_hulle;
MGLOBAL SLR_TAB			**slr_tab;
MGLOBAL ULONG			**goto_tab;
MGLOBAL ULONG			*ziel_tab;
MGLOBAL ULONG			anz_prod;
MGLOBAL ULONG			shift_term;
MGLOBAL ULONG			shift_i;
MGLOBAL ULONG			err_str_count;
MGLOBAL	FIRST			nterm;
MGLOBAL OBER_HULLE		ober_hulle;



/* Prototypen */
WORD gen_term_includefile( BYTE *file_name );
WORD make_slr_parser( BYTE *file_name );
VOID output_reduce_tab( FILE *file );
VOID output_shift_tab( FILE *file );
VOID output_jump_tab( FILE *file );
VOID output_reduce_struct( FILE *file );
VOID output_ziel_tab( FILE *file );
VOID output_follow( FILE *file );
WORD test_of_new_line( FILE *file, WORD anz_char, BYTE *sep, BYTE *output );
WORD output_debug( FILE *file, BYTE *string );



WORD gen_output( BYTE *file_name )
{
	/* Hier wird der gesamte Output erzeugt.*/

	/* Include-Datei fÅr Scanner erzeugen */
	if( gen_term_includefile( file_name ) < 0 )
		/* Fehler */
		return( -1 );

	/* Parser-Source-Code erzeugen */
	if( make_slr_parser( file_name ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}



WORD gen_term_includefile( BYTE *file_name )
{
	/*
		Hier wird die Includedatei fÅr den Scanner erzeugt.
		Es werden alle Terminale mit fortlaufender Nummer versehen.
	*/
	REG ULONG	i = 0, j;
	FILE		*file;
	BYTE		*temp_str, str[256];
	ULONG		prod_nr;

	/* Backslash suchen */
	temp_str = strrchr( file_name, ( WORD ) '\\' );

	/* War gar kein Backslash im Dateinamen angegeben worden ? */
	if( !temp_str )
		/* Dateinamen benutzen */
		temp_str = file_name;

	else
		/* Backslash lîschen */
		temp_str++;

	/* Bis zum Punkt kopieren */
	do
	{
		/* Zeichen kopieren */
		include_name_h[i] = temp_str[i];

	}while( temp_str[i++] != '.' );

	/* Extension erzeugen */
	include_name_h[i++] = 'H';
	include_name_h[i] = 0;

	/* Datei îffnen */
	if( !( file = fopen( include_name_h, "w" ) ) )
		/* Fehler */
		return( error( CANNOT_OPEN_INCLUDE_FILE, 0 ) );

	/* Header schreiben */
	fprintf( file,	"/*" );
	fprintf( file, INFO );

	/* Debug-Version ? */
	if( !( output_debug( file, "\n\nDebug-Version\n*/\n\n" ) ) )
		/* Nein */
		fprintf( file, "\n*/\n\n" );

	/* Alle Terminale */
	for( i = 0; i < term_str.anz_elem; i++ )
		/* Eintrag erzeugen */
		fprintf( file, "\n#define %s\t\t\t%lu", prod_hash[term_str.arr[i]].prod_name, i );

	/* Alle Produktionen mit fortlaufender Nummer ausgeben */
	fprintf( file, "\n\n" );

	/* Produktionen ausgeben */
	for( i = 0; i <= prod_list_nr; i++ )
	{
		/* Produktionen mit Alternativen ? */
		if( prod_info[i].prod_fl )
		{
			/* Produktion mit Alternativen */
			prod_nr = 0;

			do
			{
				/* Index der Produktion ausgeben */
				fprintf( file, "\n#define %s_%lu\t\t\t%lu  /*\t", prod_hash[prod_info[i].prod_nr].prod_name, prod_nr, i );

				/* Produktion in C-Kommentaren ausgeben */
				for( j = 0; j < prod_info[i].prod_ri_anz; j++ )
				{
					/* Terminal ? */
					if( prod_hash[prod_info[i].prod_ri[j]].prod_type == TERM )
						fprintf( file, " '%s'", prod_hash[prod_info[i].prod_ri[j]].prod_name );

					else
						fprintf( file, " %s", prod_hash[prod_info[i].prod_ri[j]].prod_name );
				}

				/* C-Kommentar schliessen */
				fprintf( file, " */" );

				/* NÑchste Alternative */
				prod_nr++;
				i++;

			}while( prod_info[i].prod_fl );

			/* Index der Produktion ausgeben */
			fprintf( file, "\n#define %s_%lu\t\t\t%lu  /*\t", prod_hash[prod_info[i].prod_nr].prod_name, prod_nr, i );

			/* Produktion in C-Kommentaren ausgeben */
			for( j = 0; j < prod_info[i].prod_ri_anz; j++ )
			{
				/* Terminal ? */
				if( prod_hash[prod_info[i].prod_ri[j]].prod_type == TERM )
					fprintf( file, " '%s'", prod_hash[prod_info[i].prod_ri[j]].prod_name );

				else
					fprintf( file, " %s", prod_hash[prod_info[i].prod_ri[j]].prod_name );
			}

			/* C-Kommentar schliessen */
			fprintf( file, " */" );
		}

		else
		{
			/* Index der Produktion ausgeben */
			fprintf( file, "\n#define %s\t\t\t%lu  /*\t", prod_hash[prod_info[i].prod_nr].prod_name, i );

			/* Produktion in C-Kommentaren ausgeben */
			for( j = 0; j < prod_info[i].prod_ri_anz; j++ )
			{
				/* Terminal ? */
				if( prod_hash[prod_info[i].prod_ri[j]].prod_type == TERM )
					fprintf( file, " '%s'", prod_hash[prod_info[i].prod_ri[j]].prod_name );

				else
					fprintf( file, " %s", prod_hash[prod_info[i].prod_ri[j]].prod_name );
			}

			/* C-Kommentar schliessen */
			fprintf( file, " */" );
		}
	}

	/* Ein paar Zeilen frei lassen */
	fprintf( file, "\n\n/* FÅr die Ziel-Tabelle */\n" );

	/* Nichtterminale fÅr jump_tab ausgeben */
	for( i = 1, j = 1; i <= prod_list_nr; )
	{
		/* Nichtterminal ausgeben */
		strcpy( str, prod_hash[prod_info[i].prod_nr].prod_name );
		fprintf( file, "\n#define %s %lu", strupr( str ), j++ );

		while( prod_info[i++].prod_fl )
			;
	}

	/* Datei schliessen */
	if( fclose( file ) == EOF )
		/* Fehler */
		return( error( CANNOT_CLOSE_INCLUDE_FILE, 0 ) );

	/* Alles OK */
	return( 0 );
}


WORD make_slr_parser( BYTE *file_name )
{
	/* Hier wird der Source-Code zum SLR-Parser erzeugt.*/
	REG ULONG	i = 0;
	FILE		*file;
	BYTE		include_name[13], *temp_str;

	/* Backslash suchen */
	temp_str = strrchr( file_name, ( WORD ) '\\' );

	/* War gar kein Backslash im Dateinamen angegeben worden ? */
	if( !temp_str )
		/* Dateinamen benutzen */
		temp_str = file_name;

	else
		/* Backslash lîschen */
		temp_str++;

	/* Bis zum Punkt kopieren */
	do
	{
		/* Zeichen kopieren */
		include_name[i] = temp_str[i];

	}while( temp_str[i++] != '.' );

	/* Extension erzeugen */
	include_name[i++] = 'S';
	include_name[i++] = 'L';
	include_name[i++] = 'R';
	include_name[i] = 0;

	/* Datei îffnen */
	if( !( file = fopen( include_name, "w" ) ) )
		/* Fehler */
		return( error( CANNOT_OPEN_INCLUDE_FILE, 0 ) );

	/* Header schreiben */
	fprintf( file,	"/*" );
	fprintf( file, INFO );

	/* Debug-Version ? */
	if( !( output_debug( file, "\n\nDebug-Version\n*/\n\n" ) ) )
		/* Nein */
		fprintf( file, "\n*/\n\n" );

	/* Includes aufnehmen */
	fprintf( file,	"\n/* Includes */\n"
					"\n#include <portab.h>"
					"\n#include <stdio.h>"
					"\n#include <stdlib.h>" );

	/* Wenn Debug-Version */
	output_debug( file, "\n#include <tos.h>" );

	/* Includedatei des Parser-Generators aufnehmen */
	fprintf( file,	"\n#include \"%s\"", include_name_h );

	/* Definitionen ausgeben */
	fprintf( file,	"\n\n\n/* Definitionen */"
					"\n\n/* Grîûe des Stacks */"
					"\n#define STACK_SIZE\t100"
					"\n\n/* Anzahl der Terminale */"
					"\n#define ANZ_TERM\t%ld", term_str.anz_elem );

	/* Wenn Debug-Version */
	output_debug( file, "\n#define OK\t0"
						"\n#define ERROR\t1"
						"\n\n/* Ausgabemedien */"
						"\n#define DEST_SCREEN	0	/* Ausgabe auf Bildschirm */"
						"\n#define DEST_FILE	1	/* Ausgabe in Datei */"
						"\n#define DEST_PRN	2	/* Ausgabe auf Drucker */" );

	/* Prototypen ausgeben */
	fprintf( file,	"\n\n\n/* Prototypen */"
					"\nVOID push( WORD value );"
					"\nWORD pop( VOID );"
					"\nWORD parse( VOID );"
					"\nVOID reduce( WORD anz_elem );"
					"\nVOID error( VOID );"
					"\nMGLOBAL WORD scanner( VOID );" );

	/* Wenn Debug-Version */
	output_debug( file, "\nVOID shift( WORD shift_state, WORD old_lahead, WORD lahead, WORD err_state );" );

	/* Deklarationen ausgeben */
	fprintf( file,	"\n\n\n/* Deklarationen */"
					"\n\n/* Lookahead */"
					"\nWORD lookahead;"
					"\n\n/* Aktueller Status */"
					"\nWORD state;"
					"\n\n/* Stackpointer */"
					"\nWORD stackpointer = -1;"
					"\n\n/* Stack */"
					"\nWORD stack[STACK_SIZE];"
					"\n\n/* Fehlerflag */"
					"\nWORD error_val = 0;" );

	/* Wenn Debug-Version */
	output_debug( file,	"\n\n/* Altes Lookahead */"
						"\nWORD old_lookahead = - 1;"
						"\n\n/* Alter Status */"
						"\nWORD old_state;"
						"\n\n/* Ausgabemedium festlegen */"
						"\nWORD dest_state = DEST_SCREEN;"
						"\n\n/* FÅr Log */"
						"\nFILE *output_file;" );

	/* Reduce-Tabelle einbringen */
	output_reduce_tab( file );

	/* Shift-Tabelle einbringen */
	output_shift_tab( file );

	/* Jump-Tabelle einbringen */
	output_jump_tab( file );

	/* Bei Debug-Version Struktur ausgeben */
	if( slr_info.debug == ON )
		/* ZusÑtzliche Struktur ausgeben */
		output_reduce_struct( file );

	/* Ziel-Tabelle einbringen */
	output_ziel_tab( file );

	/* FOLLOW-Struktur einbringen */
	output_follow( file );

	/* Debug-Version des Parser-Source einbringen ? */
	
	if( !( output_debug( file,	"\n\nVOID error( VOID )\n{\n\t/* Hier werden die Fehler behandlet. */"
								"\n\tREG WORD\ti, j, p;\n\tULONG\t\tshift_state;\n\n\t/* Fehlerflag setz"
								"en */\n\terror_val++;\n\n\t/* Alle Terminale */\n\tfor( i = 0; i < A"
								"NZ_TERM; i++ )\n\t{\n\t\t/* Shift von i mîglich ? */\n\t\tif( shift_s"
								"tate = shift_tab[state][i] )\n\t\t{\n\t\t\t/* Shift von lookahead mî"
								"glich ? */\n\t\t\tif( shift_tab[shift_state][lookahead] )\n\t\t\t{\n\t\t"
								"\t\t/* i schieben */\n\t\t\t\tshift( shift_tab[state][i], old_lookah"
								"ead, i, ERROR );\n\n\t\t\t\t/* Geschobenes Terminal merken */\n\t\t\t\to"
								"ld_lookahead = i;\n\n\t\t\t\t/* Alles OK */\n\t\t\t\treturn;\n\t\t\t}\n\n\t\t\t/*"
								" Reduziere-Zustand ? */\n\t\t\telse if( reduce_tab[shift_state][l"
								"ookahead] )\n\t\t\t{\n\t\t\t\t/* Reduziere-Zustand */\n\n\t\t\t\t/* Ist look"
								"ahead in FOLLOW( reduce_tab[shift_state][i] ) ? */\n\t\t\t\tj = 0;"
								"\n\n\t\t\t\t/* Die passende FOLLOW-Struktur holen */\n\t\t\t\tdo\n\t\t\t\t{\n\t"
								"\t\t\t\t/* Ist es die richtige ? */\n\t\t\t\t\tif( ( follow[j].beginn <"
								"= reduce_tab[shift_state][i] ) && ( follow[j].end >= reduce_t"
								"ab[shift_state][i] ) )\n\t\t\t\t\t\t/* Ja */\n\t\t\t\t\t\tbreak;\n\n\t\t\t\t\telse"
								"\n\t\t\t\t\t\t/* Nein */\n\t\t\t\t\t\tj++;\n\n\t\t\t\t}while( 1 );\n\n\t\t\t\t/* Alle F"
								"OLLOW-Elemente */\n\t\t\t\tfor( p = 0; p < follow[j].anz_follow_el"
								"em; p++ )\n\t\t\t\t{\n\t\t\t\t\t/* Ist es lookahead ? */\n\t\t\t\t\tif( follow"
								"[j].follow[p] == lookahead )\n\t\t\t\t\t{\n\t\t\t\t\t\t/* i schieben */\n\t\t"
								"\t\t\t\tshift( shift_tab[state][i], old_lookahead, i, ERROR );\n\n\t"
								"\t\t\t\t\t/* Geschobenes Terminal merken */\n\t\t\t\t\t\told_lookahead = "
								"i;\n\n\t\t\t\t\t\t/* Alles OK */\n\t\t\t\t\t\treturn;\n\t\t\t\t\t}\n\t\t\t\t}\n\n\t\t\t\t/* M"
								"eldung ausgeben */\n\t\t\t\tfprintf( output_file, \"\\n\\nLîsche Term"
								"inal %%s !\", term_name[lookahead] );\n\n\t\t\t\t/* Lookahead lîschen"
								" */\n\t\t\t\tlookahead = scanner();\n\t\t\t\ti = -1;\n\t\t\t}\n\t\t}\n\t}\n}\n\n\n\n\n"
								"VOID main( VOID )\n{\n\t/* Ausgabemedium festlegen */\n\tswitch( d"
								"est_state )\n\t{\n\t\tcase DEST_PRN:\n\t\t{\n\t\t\t/* Ausgabe auf Drucker"
								" */\n\t\t\toutput_file = stdprn;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_SCREE"
								"N:\n\t\t{\n\t\t\t/* Ausgabe auf Bildschirm */\n\t\t\toutput_file = stdou"
								"t;\n\t\t\tbreak;\n\t\t}\n\n\t\tcase DEST_FILE:\n\t\t{\n\t\t\t/* Ausgabe in Date"
								"i */\n\n\t\t\t/* Datei zum Schreiben îffnen */\n\t\t\toutput_file = fr"
								"eopen( \"PARSER.LOG\", \"w\", stdout );\n\t\t}\n\t}\n\n\t/* Stack initial"
								"isieren */\n\tpush( 0 );\n\n\t/* Eingabesymbol holen */\n\tlookahead"
								" = scanner();\n\n\t/* Parsen */\n\tparse();\n\n\t/* Datei schliessen "
								"*/\n\tfclose( output_file );\n}\n\n\nVOID push( WORD value )\n{\n\t/* "
								"Stackpointer erhîhen */\n\tstackpointer++;\n\n\t/* öberlauf ? */\n\t"
								"if( stackpointer > STACK_SIZE )\n\t{\n\t\tfprintf( output_file, \"\\"
								"n\\nStackoverflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stack sc"
								"hieben */\n\tstack[stackpointer] = value;\n}\n\n\nWORD pop( VOID )\n"
								"{\n\t/* Stackpointer dekrementieren */\n\tstackpointer--;\n\n\t/* \"U"
								"nterlauf\" ? */\n\tif( stackpointer < -1 )\n\t{\n\t\tfprintf( output_"
								"file, \"\\n\\nStackunderflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zur"
								"Åckliefern */\n\treturn( stack[stackpointer + 1] );\n}\n\n\nWORD pa"
								"rse( VOID )\n{\n\tWORD symbol, i;\n\tLONG taste;\n\n\t/* Endlosschlei"
								"fe */\n\twhile( 1 )\n\t{\n\t\t/* Aktuellen Zustand holen */\n\t\tstate "
								"= stack[stackpointer];\n\n\t\t/* Ist die Eingabe geparst ? */\n\t\ti"
								"f( ( state == 1 ) && ( lookahead == ACCEPT ) )\n\t\t{\n\t\t\t/* Gab "
								"es Fehler ? */\n\t\t\tif( error_val )\n\t\t\t\t/* Ja */\n\t\t\t\tfprintf( o"
								"utput_file, \"\\n\\nEs gab %%d Fehler !\", error_val );\n\n\t\t\telse\n\t"
								"\t\t\t/* Alles OK */\n\t\t\t\tfprintf( output_file, \"\\n\\nEingabe war "
								"OK\\n\" );\n\n\t\t\t/* Ende */\n\t\t\texit( 0 );\n\t\t}\n\n\t\t/* Schieben ? */"
								"\n\t\telse if( shift_tab[state][lookahead] > 0 )\n\t\t{\n\t\t\t/* Schie"
								"ben */\n\t\t\tshift( shift_tab[state][lookahead], old_lookahead, "
								"lookahead, OK );\n\n\t\t\t/* Altes lookahead merken */\n\t\t\told_look"
								"ahead = lookahead;\n\n\t\t\t/* Neues Eingabesymbol holen */\n\t\t\tloo"
								"kahead = scanner();\n\t\t}\n\n\t\t/* Reduzieren */\n\t\telse if( reduce"
								"_tab[state][lookahead] > 0 )\n\t\t{\n\t\t\t/* Zustand nach Reduktion"
								" holen */\n\t\t\tsymbol = ziel[state];\n\n\t\t\t/* Reduzieren */\n\t\t\tre"
								"duce( reduce_tab[state][lookahead] );\n\n\t\t\t/* Alten Zustand me"
								"rken */\n\t\t\told_state = state;\n\n\t\t\t/* Neuen Zustand holen */\n\t"
								"\t\tstate = stack[stackpointer];\n\n\t\t\t/* Meldung ausgeben */\n\t\t\t"
								"fprintf( output_file, \"\\n\\nReduziere zu  : %%s\", nterm[reduce_"
								"tab[old_state][lookahead]].prod_name );\n\t\t\tfprintf( output_fi"
								"le, \"\\nNeuer Status  : %%d\", jump_tab[state][symbol - 1] );\n\t\t"
								"\tfprintf( output_file, \"\\nAlter Status  : %%d\", old_state );\n\t"
								"\t\tfprintf( output_file, \"\\nNeues Terminal: %%s\", term_name[loo"
								"kahead] );\n\n\t\t\t/* Haben wir ein altes Terminal ? */\n\t\t\tif( ol"
								"d_lookahead >= 0 )\n\t\t\t\t/* Wird haben ein altes Terminal */\n\t\t"
								"\t\tfprintf( output_file, \"\\nAltes Terminal: %%s\", term_name[old"
								"_lookahead] );\n\n\t\t\t/* Zustand nach Reduktion herstellen */\n\t\t"
								"\tif( jump_tab[state][symbol - 1] != 0 )\n\t\t\t\t/* Wert auf Stack"
								" schieben */\n\t\t\t\tpush( jump_tab[state][symbol - 1] );\n\n\t\t\tels"
								"e\n\t\t\t{\n\t\t\t\t/* Fehler beim Springen */\n\t\t\t\tfprintf( output_fil"
								"e, \"\\n\\nFehler beim Springen !\"\\\n\t\t\t\t\t\t\"\\nAktueller Status: %%"
								"d\"\\\n\t\t\t\t\t\t\"\\nAktuelles Terminal: %%s\", old_state, term_name[lo"
								"okahead] );\n\n\t\t\t\t/* Ende */\n\t\t\t\texit( 1 );\n\t\t\t}\n\t\t}\n\n\t\t/* Syn"
								"taxfehler ? */\n\t\telse if( shift_tab[state][lookahead] == 0 )\n"
								"\t\t\t/* Fehler */\n\t\t\terror();\n\n\t\t/* Ausgabe auf Bildschirm ? */"
								"\n\t\tif( dest_state == DEST_SCREEN )\n\t\t{\n\t\t\t/* Auf Tastendruck "
								"warten */\n\t\t\ttaste = Cnecin();\n\n\t\t\t/* Stack ausgeben ? */\n\t\t\t"
								"if( ( taste & 0x000000ff ) == 0x73 )\n\t\t\t{\n\t\t\t\t/* Stack ausgeb"
								"en */\n\t\t\t\tfprintf( output_file, \"\\n\\nStack\\n-----\\n\" );\n\n\t\t\t\t"
								"/* Alle EintrÑge im Stack ausgeben */\n\t\t\t\tfor( i = 0; i < sta"
								"ckpointer; i++ )\n\t\t\t\t\t/* Eintrag ausgeben */\n\t\t\t\t\tfprintf( ou"
								"tput_file, \"%%d\\n\", stack[i] );\n\t\t\t}\n\t\t}\n\n\t\telse\n\t\t{\n\t\t\t/* Auf"
								" dem Drucker/Datei immer den Stack ausgeben */\n\n\t\t\t/* Stack a"
								"usgeben */\n\t\t\tfprintf( output_file, \"\\n\\nStack\\n-----\\n\" );\n\n"
								"\t\t\t/* Alle EintrÑge im Stack ausgeben */\n\t\t\tfor( i = 0; i < s"
								"tackpointer; i++ )\n\t\t\t\t/* Eintrag ausgeben */\n\t\t\t\tfprintf( ou"
								"tput_file, \"%%d\\n\", stack[i] );\n\t\t}\n\t}\n}\n\nVOID shift( WORD shi"
								"ft_state, WORD old_lahead, WORD lahead, WORD err_state )\n{\n\t/"
								"* Hier wird ein Eintrag aus der Shift-Tabelle auf den Stack g"
								"eschoben. */\n\n\t/* Eintrag auf Stack schieben */\n\tpush( shift_"
								"state );\n\n\t/* Fehler ? */\n\tif( err_state == ERROR )\n\t\t/* Fehl"
								"er */\n\t\tfprintf( output_file, \"\\n\\nFehler !\" );\n\n\t/* Meldung "
								"ausgeben */\n\tfprintf( output_file, \"\\n\\nSchiebe       : %%s\", "
								"term_name[lahead] );\n\tfprintf( output_file, \"\\nNeuer Status  "
								": %%d\", shift_state );\n\tfprintf( output_file, \"\\nAlter Status "
								" : %%d\", state );\n\tfprintf( output_file, \"\\nNeues Terminal: %%s"
								"\", term_name[lahead] );\n\n\t/* Haben wir ein altes Terminal ? *"
								"/\n\tif( old_lahead >= 0 )\n\t\t/* Wird haben ein altes Terminal *"
								"/\n\t\tfprintf( output_file, \"\\nAltes Terminal: %%s\", term_name[o"
								"ld_lahead] );\n}\n\nVOID reduce( WORD anz_elem )\n{\n\t/* Hier wird"
								" reduziert. */\n\tREG WORD i;\n\n\t/* Elemente vom Stack lîschen *"
								"/\n\tfor( i = 0; i < nterm[anz_elem].anz_ri; i++ )\n\t\t/* Eintrag"
								" vom Stack lîschen */\n\t\tpop();\n}" ) ) )
	/* Die normale Parser-version erzeugen */
	fprintf( file,	"\n\nVOID error( VOID )\n{\n\t/* Hier werden die Fehler behandlet. */"
					"\n\tREG WORD\ti, j, p;\n\tULONG\t\tshift_state;\n\n\t/* Fehlerflag setz"
					"en */\n\terror_val++;\n\n\t/* Alle Terminale */\n\tfor( i = 0; i < A"
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
					"\n\t\t\t}\n\t\t}\n\t}\n}\n\n\n\n\nVOID main( VOID )\n{\n\t/* Stack initialisier"
					"en */\n\tpush( 0 );\n\n\t/* Eingabesymbol holen */\n\tlookahead = sc"
					"anner();\n\n\t/* Parsen */\n\tparse();\n}\n\n\nVOID push( WORD value )"
					"\n{\n\t/* Stackpointer erhîhen */\n\tstackpointer++;\n\n\t/* öberlauf"
					" ? */\n\tif( stackpointer > STACK_SIZE )\n\t{\n\t\tprintf( \"\\n\\nStac"
					"koverflow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert auf Stack schieben "
					"*/\n\tstack[stackpointer] = value;\n}\n\n\nWORD pop( VOID )\n{\n\t/* S"
					"tackpointer dekrementieren */\n\tstackpointer--;\n\n\t/* \"Unterlau"
					"f\" ? */\n\tif( stackpointer < -1 )\n\t{\n\t\tprintf( \"\\n\\nStackunder"
					"flow !\" );\n\t\texit( 1 );\n\t}\n\n\t/* Wert zurÅckliefern */\n\treturn"
					"( stack[stackpointer + 1] );\n}\n\n\nWORD parse( VOID )\n{\n\tWORD s"
					"ymbol;\n\n\t/* Endlosschleife */\n\twhile( 1 )\n\t{\n\t\t/* Aktuellen Z"
					"ustand holen */\n\t\tstate = stack[stackpointer];\n\n\t\t/* Ist die "
					"Eingabe geparst ? */\n\t\tif( ( state == 1 ) && ( lookahead == A"
					"CCEPT ) )\n\t\t{\n\t\t\t/* Gab es Fehler ? */\n\t\t\tif( error_val )\n\t\t\t"
					"\t/* Ja */\n\t\t\t\tprintf( \"\\n\\nEs gab %%d Fehler !\", error_val );\n"
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
					"/* Fehler */\n\t\t\terror();\n\t}\n}\n\nVOID reduce( WORD anz_elem )\n{"
					"\n\t/* Hier wird reduziert. */\n\tREG WORD i;\n\n\t/* Elemente vom S"
					"tack lîschen */\n\tfor( i = 0; i < nterm[anz_elem].anz_ri; i++ "
					")\n\t\t/* Eintrag vom Stack lîschen */\n\t\tpop();\n}" );

	/* Datei schliessen */
	if( fclose( file ) == EOF )
		/* Fehler */
		return( error( CANNOT_CLOSE_INCLUDE_FILE, 0 ) );

	/* Alles OK */
	return( 0 );
}

VOID output_reduce_tab( FILE *file )
{
	/* Hier wird die Reduce-Tab in den Parser-Source geschrieben.*/
	REG ULONG	j, i;
	ULONG		pr_list_nr;
	WORD		anz_char = 0;

	/* Deklaration ausgeben */
	fprintf( file, "\n\n\nWORD reduce_tab[%lu][ANZ_TERM] = \n{\n", anz_bel_hulle + 1 );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= anz_bel_hulle; i++ )
	{
		/* Sich îffnende Klammer ausgeben */
		anz_char += fprintf( file, "{" );

		/* Alle Terminale durchgehen */
		for( j = 0; j < term_str.anz_elem; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Komma ausgeben */
				anz_char += fprintf( file, "," );

			/* Reduktion ? */
			if( slr_tab[i][j].state == REDUCE )
			{
				/* Index der Produktion holen */
				pr_list_nr = slr_tab[i][j].value;

				/* Eventuell eine neue Zeile beginnen */
				anz_char = test_of_new_line( file, anz_char, "\n ", prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name );

				/* Hat die Produktion Åberhaupt Alternativen ? */
				if( ( pr_list_nr ) && !( prod_info[pr_list_nr].prod_fl ) && !( prod_info[pr_list_nr - 1].prod_fl ) )
					/* Namen der Produktion ausgeben */
					anz_char += fprintf( file, " %s", prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name );

				else if( !( pr_list_nr ) && !( prod_info[pr_list_nr].prod_fl ) )
					/* Namen der Produktion ausgeben */
					anz_char += fprintf( file, " %s", prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name );

				else
				{
					/* Wenn es eine Alternative ist, Grungproduktion suchen */
					pr_list_nr--;

					/* Ist es eine Alternative ? */
					while( prod_info[pr_list_nr].prod_fl )
						/* Ja */
						pr_list_nr--;

					/* Dies ist die Grundproduktion */
					pr_list_nr = slr_tab[i][j].value - ( pr_list_nr + 1 );

					/* Namen der Produktion ausgeben */
					anz_char += fprintf( file, " %s_%lu", prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name, pr_list_nr );
				}
			}

			else
			{
				/* Eventuell eine neue Zeile beginnen */
				anz_char = test_of_new_line( file, anz_char, "\n ", " 0" );

				/* Null eintragen */
				anz_char += fprintf( file, " 0" );
			}
		}

		/* Komma ausgeben ? */
		if( i < anz_bel_hulle )
		{
			/* Ja */
			fprintf( file, " },\n" );

			/* Neue Zeile */
			anz_char = 0;
		}

		else
		{
			/* Nein */
			fprintf( file, " }\n" );

			/* Neue Zeile */
			anz_char = 0;
		}
	}

	/* Tabelle ist zu Ende */
	fprintf( file, "};" );
}

VOID output_shift_tab( FILE *file )
{
	/* Hier wird die Shift-Tab in den Parser-Source geschrieben.*/
	REG ULONG	j, i;
	WORD		anz_char = 0;

	/* Deklaration ausgeben */
	fprintf( file, "\n\n\nWORD shift_tab[%lu][ANZ_TERM] = \n{\n", anz_bel_hulle + 1 );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= anz_bel_hulle; i++ )
	{
		/* Sich îffnende Klammer ausgeben */
		fprintf( file, "{" );

		/* Alle Terminale durchgehen */
		for( j = 0; j < term_str.anz_elem; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Komma ausgeben */
				anz_char += fprintf( file, "," );

			/* Eventuell eine neue Zeile beginnen */
			anz_char = test_of_new_line( file, anz_char, "\n ", "    " );

			/* Schieben ? */
			if( slr_tab[i][j].state == SHIFT )
				/* Schiebe-Wert ausgeben */
				anz_char += fprintf( file, " %lu", slr_tab[i][j].value );

			else
				/* Null eintragen */
				anz_char += fprintf( file, " 0" );
		}

		/* Komma ausgeben ? */
		if( i < anz_bel_hulle )
		{
			/* Ja */
			fprintf( file, " },\n" );

			/* Neue Zeile */
			anz_char = 0;
		}

		else
		{
			/* Nein */
			fprintf( file, " }\n" );

			/* Neue Zeile */
			anz_char = 0;
		}
	}

	/* Tabelle ist zu Ende */
	fprintf( file, "};" );
}

VOID output_jump_tab( FILE *file )
{
	/* Hier wird die Sprung-Tab in den Parser-Source geschrieben.*/
	REG ULONG	j, i;
	WORD		anz_char = 0;

	/* Deklaration ausgeben */
	fprintf( file, "\n\n\nWORD jump_tab[%lu][%lu] = \n{\n", anz_bel_hulle + 1, anz_prod );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= anz_bel_hulle; i++ )
	{
		/* Sich îffnende Klammer ausgeben */
		fprintf( file, "{" );

		/* Alle Terminale durchgehen */
		for( j = 0; j < anz_prod; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Komma ausgeben */
				anz_char += fprintf( file, "," );

			/* Eventuell eine neue Zeile beginnen */
			anz_char = test_of_new_line( file, anz_char, "\n ", "    " );

			/* Jump-Wert ausgeben */
			anz_char += fprintf( file, " %lu", goto_tab[i][j] );
		}

		/* Komma ausgeben ? */
		if( i < anz_bel_hulle )
		{
			/* Ja */
			fprintf( file, " },\n" );

			/* Neue Zeile */
			anz_char = 0;
		}

		else
		{
			/* Nein */
			fprintf( file, " }\n" );

			/* Neue Zeile */
			anz_char = 0;
		}
	}

	/* Tabelle ist zu Ende */
	fprintf( file, "};" );
}

VOID output_reduce_struct( FILE *file )
{
	/*
		Hier wird bei der Debug-Version die zusÑtzliche Struktur eingebunden,
		die die Namen der Produktionen etc. enthÑlt.
	*/
	REG ULONG	i, j, alt_count;
	WORD		anz_char = 0;

	/* Struktur-Deklaration ausgeben */
	fprintf( file,	"\n\ntypedef struct\n{\n\t/* Name der Produktion */\n\tBYTE\t*prod_name;\n"
					"\t/* Anzahl der Elemente auf der rechten Seite */\n\tWORD\tanz_ri;\n\n}NTERM;" );

	/* Definition */
	fprintf( file, "\n\nNTERM\tnterm[] = \n{\n" );

	/* Alle HÅllen durchgehen */
	for( i = 0; i <= prod_list_nr; i++ )
	{
		/* Hat die neue mit der alten Produktion nichts zu tun ? */
		if( ( i ) && !( prod_info[i - 1].prod_fl ) )
			/* Nein */
			alt_count = 0;

		/* Sich îffnende Klammer ausgeben */
		fprintf( file, "{" );

		/* Hat die Produktion Åberhaupt Alternativen ? */
		if( ( i ) && !( prod_info[i].prod_fl ) && !( prod_info[i - 1].prod_fl ) )
			/* Namen der Produktion ausgeben */
			fprintf( file, " \"%s\", ", prod_hash[prod_info[i].prod_nr].prod_name );

		else if( !( i ) && !( prod_info[i].prod_fl ) )
			/* Namen der Produktion ausgeben */
			fprintf( file, " \"%s\", ", prod_hash[prod_info[i].prod_nr].prod_name );

		else
			/* Namen der Produktion ausgeben */
			fprintf( file, " \"%s_%lu\", ", prod_hash[prod_info[i].prod_nr].prod_name, alt_count );

		/* NÑchste Alternative */
		alt_count++;

		/* Anzahl der Elemente auf der rechten Seite ausgeben */
		fprintf( file, "%d", prod_info[i].prod_ri_anz );

		/* Komma ausgeben ? */
		if( i < prod_list_nr )
			/* Ja */
			fprintf( file, " }," );

		else
			/* Nein */
			fprintf( file, " }" );

		/* C-Kommentar îffnen */
		fprintf( file, "\t/*" );

		/* Produktion in C-Kommentaren ausgeben */
		for( j = 0; j < prod_info[i].prod_ri_anz; j++ )
		{
			/* Terminal ? */
			if( prod_hash[prod_info[i].prod_ri[j]].prod_type == TERM )
				fprintf( file, " '%s'", prod_hash[prod_info[i].prod_ri[j]].prod_name );

			else
				fprintf( file, " %s", prod_hash[prod_info[i].prod_ri[j]].prod_name );
		}

		/* C-Kommentar schliessen */
		fprintf( file, " */\n" );
	}

	/* Tabelle ist zu Ende */
	fprintf( file, "};" );

	/* Definition ausgeben */
	fprintf( file,  "\n\nBYTE\t*term_name[ANZ_TERM] = { " );

	/* Terminale-Tabelle ausgeben */
	for( i = 0; i < term_str.anz_elem; i++ )
	{
		/* Komma ausgeben ? */
		if( i )
			/* Ja */
			anz_char += fprintf( file, ", " );

		/* Eventuell eine neue Zeile beginnen */
		anz_char = test_of_new_line( file, anz_char, "\n ", strupr( prod_hash[term_str.arr[i]].prod_name ) );

		/* Terminal ausgeben */
		anz_char += fprintf( file, "\"%s\"", strupr( prod_hash[term_str.arr[i]].prod_name ) );
	}

	/* Initialisierung abschlieûen */
	fprintf( file,  " };\n" );

	/* ZurÅcksetzen */
	anz_char = 0;
}

VOID output_follow( FILE *file )
{
	/* FOLLOW-Mengen ausgeben */
	REG ULONG	i, j;
	WORD		anz_char = 0;

	/* Alle Follow-Elemente ausgeben */
	for( i = 0; i < prod_list_nr; i++ )
	{
		/* Definition ausgeben */
		fprintf( file, "\n\nULONG\t%s_follow[] = { ", prod_hash[prod_info[i].prod_nr].prod_name );

		/* Alle Follow-Elemente ausgeben */
		for( j = 0; j < prod_hash[prod_info[i].prod_nr].follow.anz_elem; j++ )
		{
			/* Komma ausgeben ? */
			if( j )
				/* Ja */
				anz_char += fprintf( file, ", " );

			/* Eventuell eine neue Zeile beginnen */
			anz_char = test_of_new_line( file, anz_char, "\n ", prod_hash[prod_hash[prod_info[i].prod_nr].follow.arr[j]].prod_name );

			/* Terminal ausgeben */
			anz_char += fprintf( file, "%s", strupr( prod_hash[prod_hash[prod_info[i].prod_nr].follow.arr[j]].prod_name ) );
		}

		/* Letzte Alternative holen */
		while( prod_info[i].prod_fl )
			i++;

		/* Neue Zeile */
		anz_char = 0;

		/* Struktur schliessen */
		fprintf( file, " };" );
	}

	/* Struktur-Definition ausgeben */
	fprintf( file,	"\n\ntypedef struct\n{\n\t/* FOLLOW-Menge gilt ab Alternative begin */\n\t" \
					"ULONG\tbeginn;\n\n\t/* FOLLOW-Menge gilt bis Alternative end */\n" \
					"\tULONG\tend;\n\n\t/* Anzahl der FOLLOW-Elemente */\n"
					"\tULONG\tanz_follow_elem;\n\n\t/* FOLLOW-Menge */\n" \
					"\tULONG\t*follow;\n\n}FOLLOW;\n\nFOLLOW\t" \
					"follow[] =\n{\n" );

	/* Alle Follow-Elemente ausgeben */
	for( i = 0; i < prod_list_nr; i++ )
	{
		fprintf( file, "{ " );

		/* Von eintragen */
		fprintf( file, "%ld, ", i );

		/* Index merken */
		j = i;

		/* Letzte Alternative holen */
		while( prod_info[i].prod_fl )
			i++;

		/* Bis eintragen */
		fprintf( file, "%ld, %ld, ", i, prod_hash[prod_info[j].prod_nr].follow.anz_elem );

		/* Follow-Menge ausgeben */
		fprintf( file,	"%s_follow },\n", prod_hash[prod_info[i].prod_nr].prod_name );
	}

	/* Initialisierung abschlieûen */
	fprintf( file,	"};" );
}

VOID output_ziel_tab( FILE *file )
{
	/* Hier wird die Ziel-Tabelle ausgegeben. */
	REG ULONG	j;
	WORD		anz_char = 0;
	BYTE		str[256];

	/* Definition ausgeben */
	fprintf( file, "\n\nWORD\tziel[%lu] = \n{\n", anz_bel_hulle + 1 );

	/* Alle HÅllen durchlaufen */
	for( j = 0; j <= anz_bel_hulle; j++ )
	{
		/* Komma ausgeben ? */
		if( j )
			/* Ja */
			anz_char += fprintf( file, ", " );

		/* Gibt es in HÅlle j eine Reduktion ? */
		if( ziel_tab[j] )
		{
			/* Eventuell eine neue Zeile beginnen */
			anz_char = test_of_new_line( file, anz_char, "\n ", prod_hash[prod_info[ziel_tab[j]].prod_nr].prod_name );

			/* String kopieren */
			strcpy( str, prod_hash[prod_info[ziel_tab[j]].prod_nr].prod_name );

			/* Nichtterminal ausgeben */
			anz_char += fprintf( file, "%s", strupr( str ) );
		}

		else
		{
			/* Eventuell eine neue Zeile beginnen */
			anz_char = test_of_new_line( file, anz_char, "\n ", "0" );

			/* Es gibt keine Reduktion */
			anz_char += fprintf( file, "0" );
		}
	}

	/* Tabelle ist zu Ende */
	fprintf( file, "\n};" );
}

WORD test_of_new_line( FILE *file, WORD anz_char, BYTE *sep, BYTE *output )
{
	/* Hier wird geprÅft, ob ein Zeilewechsel ausgefÅhrt werden soll */
	WORD str_len;

	/* LÑnge des Strings holen */
	str_len = ( WORD )strlen( output );

	/* Ist die Anzahl der Zeichen zu Groû ? */
	if( ( anz_char + str_len ) > slr_info.max_char )
	{
		/* Neue Zeile */
		fprintf( file, "%s", sep );

		/* Neue Zeile */
		return( 0 );
	}

	/* Nichts Ñndern */
	return( anz_char );
}

WORD output_debug( FILE *file, BYTE *string )
{
	/* string nur ausgeben, wenn die Debug-Version erzeugt werden soll. */

	/* Debug-Version ? */
	if( slr_info.debug == ON )
		/* string ausgeben */
		return( fprintf( file, string ) );

	/* War nicht die Debug-Version */
	return( 0 );
}