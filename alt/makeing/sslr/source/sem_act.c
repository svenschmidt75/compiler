/*
	Projekt		:	LR-Parser-Generator
	Modul		:	SEM_ACT.C
	Sprache		:	ANSI-C
	Compiler	:	ATARI: PureC V1.1
*/



/************/
/* Includes */
/************/

/* Include-Datei des Parser-Generators */
#include "LR.H"
#include <ctype.h>


/**************/
/* Prototypen */
/**************/

WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD do_sem_act( LONG nterm_index, WORD ebnf_sym );
VOID output_right( FILE *file, LONG nterm_index );
LONG get_attribute( BYTE *text, LONG zeile );
WORD test_sem_act( ULONG index );
LONG cmp_type( BYTE *type );
WORD delete( VOID );



/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/*
	Beinhaltet Zeiger auf SEM_ACT-Strukturen, die die semmantischen Aktionen
	beinhalten.
*/
SEM_ACT		*sem_act = 0L;

/* Anzahl der SEM_ACT-EintrÑge */
LONG		sem_act_count = 0;




/***********/
/* Imports */
/***********/

MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL	HULLE_STR	**hulle_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL LR_INFO		lr_info;
MGLOBAL	LR_TAB		**lr_tab;
MGLOBAL ULONG		zeile;
MGLOBAL ULONG		prod_count;
MGLOBAL ULONG		*ziel_tab;
MGLOBAL FILE		*file;
MGLOBAL FILE		*parser_file;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		term_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL LONG		*old_nterm_index;
MGLOBAL LONG		type_arr_count;
MGLOBAL BYTE		*grammar;
MGLOBAL BYTE		grammar_word[];
MGLOBAL BYTE		**type_arr;
MGLOBAL BYTE		*union_struct;



	

WORD do_sem_act( LONG nterm_index, WORD ebnf_sym )
{
	/*
		nterm_index	->	Nummer der aktuellen Produktion/Alternative,
						zu der die semmantische Aktion gehîrt

		ebnf_sym	1:	{ }	- EBNF-Symbol
					2:	[ ]	- EBNF-Symbol
					3:	( )	- EBNF-Symbol
	*/
	SEM_ACT	*sem_act_str;
	BYTE	sem_act_puff[256];
	WORD	sem_act_puff_count = 0, malloc_count = 0;

	/* Ist es wirklich eine semmantische Aktion ? */
	if( *++grammar == '{' )
	{
		/* Ueberlesen */
		grammar++;

		/* Speicher fÅr ein SEM_ACT-Eintrag anfordern */
		if( ( sem_act = ( SEM_ACT * ) realloc( ( VOID * ) sem_act, ( sem_act_count + 1 ) * sizeof( SEM_ACT ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Zeiger auf Struktur holen */
		sem_act_str = &sem_act[sem_act_count];

		/* Die Struktur initialisieren */
		sem_act_str->nterm_index = nterm_index;
		sem_act_str->right_pos = nterm_str[nterm_index]->anz_ri;
		sem_act_str->term = nterm_str[nterm_index]->hash_tab_index;
		sem_act_str->ebnf = ebnf_sym == 1 ? 1 : 0;
		sem_act_str->zeile = zeile;
		sem_act_str->sem_act_text = 0L;

		/* Den Text der semmantischen Aktion uebertragen */
		while( !( ( grammar[0] == '\\' ) && ( grammar[1] == '}' ) ) )
		{
			/* Neue Zeile */
			if( *grammar == '\n' )
				/* NÑchste Zeile */
				zeile++;

			/* Den Text uebertragen */
			sem_act_puff[sem_act_puff_count++] = *grammar++;

			/* Muss der Text uebertragen werden ? */
			if( sem_act_puff_count == 256 )
			{
				/* Ja, Speicher anfordern */
				if( ( sem_act_str->sem_act_text = ( BYTE * ) realloc( ( VOID * ) sem_act_str->sem_act_text, ( malloc_count + 1 ) * 256 + 1 ) ) == NULL )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

				/* Speicher uebertragen */
				memcpy( ( VOID * ) ( sem_act_str->sem_act_text + ( malloc_count++ * 256 ) ), ( VOID * ) sem_act_puff, 256 );

				/* ZÑhler zuruecksetzen */
				sem_act_puff_count = 0;
			}
		}

		/* Muss ein Rest noch uebertragen werden */
		if( ( sem_act_puff_count ) && ( sem_act_puff_count != 256 ) )
		{
			/* Speicher anfordern */
			if( ( sem_act_str->sem_act_text = ( BYTE * ) realloc( ( VOID * ) sem_act_str->sem_act_text, ( malloc_count * 256 ) + sem_act_puff_count + 1 ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Speicher uebertragen */
			memcpy( ( VOID * ) ( sem_act_str->sem_act_text + ( malloc_count * 256 ) ), ( VOID * ) sem_act_puff, sem_act_puff_count );
		}

		/* Null-Byte anhÑngen */
		*( sem_act_str->sem_act_text + ( malloc_count * 256 ) + sem_act_puff_count ) = 0;

		/* \} lîschen */
		grammar += 2;
	}

	/* NÑchsten Eintrag */
	sem_act_count++;

	/* Alles OK */
	return( 0 );
}


WORD generate_sem_act( VOID )
{
	/*
		Hier werden die semmantischen Aktionen in den Parser-Source-Text
		eingefuegt.
		Sie werden auch auf Fehler ueberprueft.
	*/
	REG ULONG	i;
	WORD		comment = 0, val;
	LONG		nterm_attrib, elem;
	BYTE		*text, num[3] = { 0 };

	/* Flag fÅr die Ausfuehrung der semmantischen Aktionen ausgeben */
	fputs( "\t\t\t/* Semmantische Aktionen ausfuehren ? */\n\t\t\tif( !sem_flg )\n\t\t\t{\n", parser_file );

	/* Ausgeben, komprimierte Version ? */
	if( !lr_info.compress )
		/* Ja */
		fputs( "\t\t\t\tswitch( ( WORD ) lr_tab[c_lr[state]][lookahead] - 1 )\n\t\t\t\t{", parser_file );

	else
		/* Nein */
		fputs( "\t\t\t\tswitch( ( WORD ) lr_tab[state][lookahead] - 1 )\n\t\t\t\t{", parser_file );

	/* Alle SEM_ACT-EintrÑge */
	for( i = 0; i < sem_act_count; i++ )
	{
		/* nterm_index in der SEM_ACT anpassen */
		sem_act[i].nterm_index = old_nterm_index[sem_act[i].nterm_index];

		/* Wird die semmantische Aktion jemals ausgefuehrt ? */
		if( test_sem_act( i ) < 0 )
			/* Nein */
			continue;

		/* case ausgeben */
		fprintf( parser_file, "\n\t\t\t\t\tcase %ld:\n\t\t\t\t\t{\n\t\t\t\t\t\t/* %s ->", sem_act[i].nterm_index + 1, hash_tab[nterm_str[sem_act[i].nterm_index]->hash_tab_index].name );

		/* Die rechte Seite ausgeben */
		output_right( parser_file, sem_act[i].nterm_index );

		/* C-Kommentar abschliessen */
		fputs( " */\n\n\t\t\t\t\t", parser_file );

		/* Zeiger auf semmantischen Aktions-Text holen */
		text = sem_act[i].sem_act_text;

		/* Bis zum Ende */
		while( *text )
		{
			/* Ist es 0x0d ? */
			if( *text == 0x0d )
				/* Zeichen lîschen */
				text++;

			/* Ist es ein Kommentar ? */
			if( *text == '/' )
			{
				/* Beginn eines C-Kommentares ? */
				if( text[1] == '*' )
					/* Flag fÅr Kommentar setzen */
					comment++;

				/* Ende eines C-Kommentares ? */
				else if( text[-1] == '*' )
					/* Flag fÅr Kommentar lîschen */
					comment--;

				/* Das Zeichen uebertragen */
				fputc( *text++, parser_file );
			}

			/* Ist es ein Steuerzeichen ? */
			else if( ( *text == '$' ) && !comment )
			{
				/* Ist $$ gemeint ? */
				if( text[1] == '$' )
				{
					/* Ja, statt dessen lr_val ausgeben */
					fputs( "lr_val", parser_file );

					/* $$ lîschen */
					text += 2;

					/* Attribut des Nichtterminal holen */
					nterm_attrib = hash_tab[nterm_str[sem_act[i].nterm_index]->hash_tab_index].type;

					/* Hat das Nichtterminal ein Attribut ? */
					if( nterm_attrib < 0 )
						/* Nein, Fehler */
						error( MISSING_NTERM_ATTRIBUTE, sem_act[i].zeile, ( ULONG ) hash_tab[nterm_str[sem_act[i].nterm_index]->hash_tab_index].name );

					else
						/* Attribut ausgeben */
						fprintf( parser_file, ".%s", type_arr[nterm_attrib] );
				}

				/* Ist es ein Speicherplatz ? */
				else if( text[1] == 'a' )
				{
					/* Ja, merken */
					num[0] = text[2];

					/* x lîschen */
					text += 3;

					/* Ist der Wert zweistellig ? */
					if( isdigit( text[0] ) )
					{
						/* Ja, zusammenfuegen */
						num[1] = text[0];
						text++;
					}

					/* Wert in eine Zahl wandeln */
					elem = atoi( num );

					/* Speicherplatz einfuegen */
					fprintf( parser_file, "a_stack[%ld]", elem );

					/* Zuruecksetzen */
					num[0] = 0;
					num[1] = 0;
				}

				/* $x ist gemeint */
				else
				{
					/* x lîschen */
					text++;

					/* Ist ein Vorzeichen angegeben worden ? */
					if( !isdigit( text[0] ) )
					{
						/* Ja, Minus ? */
						if( text[0] == '-' )
							/* Ja, Minus */
							val = 1;

						else
							/* Ist Plus */
							val = 0;

						/* Vorzeichen lîschen */
						text++;
					}

					else
						/* Default ist immer positiv */
						val = 0;

					/* Die erste Ziffer merken */
					num[0] = *text++;

					/* Ist der Wert zweistellig ? */
					if( isdigit( text[0] ) )
						/* Ja, zusammenfuegen */
						num[1] = *text++;

					/* Wert in eine Zahl wandeln */
					elem = atoi( num );

					/* Vorzeichen setzen */
					val == 1 ? elem = -elem : elem;

					/* Ist es eine EBNF-{ }-Produktion ? */
					if( sem_act[i].ebnf )
						/* Ja, erstes Element ist aux_sym */
						elem++;

					/* Ein Ueberlauf ? */
					if( elem > MAX_ANZ_RI )
						/* Ja, Meldung ausgeben */
						error( SEM_ACT_ERR, sem_act[i].zeile, 0 );

					/* Gibt es eine Differenz ? */
					if( nterm_str[sem_act[i].nterm_index]->anz_ri - elem )
						/* Korrekt ausgeben */
						fprintf( parser_file, "lr_sem_stack[lr_sem_count - %ld]", nterm_str[sem_act[i].nterm_index]->anz_ri - elem );

					else
						/* Es gibt keine Differenz */
						fputs( "lr_sem_stack[lr_sem_count]", parser_file );

					/* Attribut des Elements holen */
					nterm_attrib = hash_tab[nterm_str[sem_act[i].nterm_index]->ri[elem - 1]].type;

					/* Hat das Element ein Attribut ? */
					if( nterm_attrib < 0 )
						/* Fehler */
						error( SX_ATTRIBUTE_ERROR, sem_act[i].zeile, ( ULONG ) hash_tab[nterm_str[sem_act[i].nterm_index]->ri[elem - 1]].name );

					else
						/* Attribut ausgeben */
						fprintf( parser_file, ".%s", type_arr[nterm_attrib] );

					/* Zuruecksetzen */
					num[0] = 0;
					num[1] = 0;
				}
			}

			else
				/* Das Zeichen uebertragen */
				fputc( *text++, parser_file );
		}

		/* Ein break einfuegen */
		fputs( "\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n", parser_file );
	}

	/* switch abschliessen */
	fputs(	"\t\t\t\t}\n\n\t\t\t\t/* Stack korregieren */\n\t\t\t\tlr_sem_count -= ", parser_file );

	/* Debug-Version ? */
	if( !lr_info.debug )
	{
		/* Ja, komprimierte Version ? */
		if( !lr_info.compress )
			/* Ja */
			fputs( "nterm[lr_tab[c_lr[state]][lookahead] - 1].anz_ri", parser_file );

		else
			/* Nein */
			fputs( "nterm[lr_tab[state][lookahead] - 1].anz_ri", parser_file );
	}

	else
	{
		/* Nein, komprimierte Version ? */
		if( !lr_info.compress )
			/* Ja */
			fputs( "anz_ri[lr_tab[c_lr[state]][lookahead] - 1]", parser_file );

		else
			/* Nein */
			fputs( "anz_ri[lr_tab[state][lookahead] - 1]", parser_file );
	}
	
	/* switch abschliessen */
	fputs( ";\n\t\t\t\tlr_sem_stack[++lr_sem_count] = lr_val;\n\t\t\t}\n\n", parser_file );

	/* Wurden Attribute verwendet ohne die UNION-Struktur ? */
	if( !( union_struct ) && type_arr )
		/* Ja, Fehler */
		error( MISSING_UNION_STRUCT, 0, 0 );

	/* Alles OK */
	return( 0 );
}


LONG get_attribute( BYTE *text, LONG zeile )
{
	/* Hier wird der Index des Attributs wiedergegeben. */
	REG ULONG	i = 0;
	LONG		type_index;
	BYTE		type[256];

	/* Ist es -> ? */
	if( ( text[0] == '-' ) && ( text[1] == '>' ) )
	{
		/* Lîschen */
		text += 2;

		/* Das Attribut extrahieren */
		while( ( isalnum( *text ) ) || ( *text == '_' ) )
			/* Zeichen merken */
			type[i++] = *text++;

		/* Null-Byte anhÑngen */
		type[i] = 0;

		/* Gibt es das Attribut ? */
		if( ( type_index = cmp_type( type ) ) < 0 )
		{
			/* Nein, Fehlermeldung ausgeben */
			error( UNKNOWN_ATTRIBUTE, zeile, ( ULONG ) type );

			/* Eine weitere Fehlermeldung unterbinden */
			return( -2 );
		}

		else
			/* Den Index des Attributes zurueckliefern */
			return( type_index );
	}

	/* Ist es . ? */
	else if( text[0] == '.' )
	{
		/* Lîschen */
		text++;

		/* Das Attribut extrahieren */
		while( ( isalnum( *text ) ) || ( *text == '_' ) )
			/* Zeichen merken */
			type[i++] = *text++;

		/* Null-Byte anhÑngen */
		type[i] = 0;

		/* Gibt es das Attribut ? */
		if( ( type_index = cmp_type( type ) ) < 0 )
		{
			/* Nein, Fehlermeldung ausgeben */
			error( UNKNOWN_ATTRIBUTE, zeile, ( ULONG ) type );

			/* Eine weitere Fehlermeldung unterbinden */
			return( -2 );
		}

		else
			/* Den Index des Attributes zurueckliefern */
			return( type_index );
	}

	else
		/* Es wurde kein Attribut angegeben */
		return( -1 );
}


WORD test_sem_act( ULONG index )
{
	/*
		Hier wird geprueft, ob es ueberhaupt ein Reduce-Item zur semmantischen
		Aktion gibt.
	*/
	REG ULONG	i, j;
	HULLE_STR	*hulle;
	WORD		anz_ri;

	/* Alle HÅllen bearbeiten */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Zeiger auf HÅlle holen */
		hulle = hulle_str[i];

		/* Alle Items der HÅlle prÅfen */
		for( j = 0; j < hulle->anz_items; j++ )
		{
			/* Anzahl der Elemente auf der rechten Seite holen */
			anz_ri = nterm_str[hulle->item_str[j].nterm_index]->anz_ri;

			/* Passt die semmantische Aktion ? */
			if( ( hulle->item_str[j].nterm_index == sem_act[index].nterm_index ) && ( anz_ri == sem_act[index].right_pos ) )
				/* Ja */
				return( 0 );
		}
	}

	/* Semmantische Aktion passt nicht */
	return( error( BAD_SEM_ACT, sem_act[index].zeile, 0 ) );
}