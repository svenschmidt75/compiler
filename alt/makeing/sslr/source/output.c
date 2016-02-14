/*
	Projekt		:	LR-Parser-Generator
	Modul		:	OUTPUT.C
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
VOID output_item( LONG nterm_index, WORD pkt_pos );
VOID output_right( FILE *file, LONG nterm_index );
WORD show_hulle( LONG hulle_index );
LONG get_term_index( LONG term );
VOID output_goto_tab( VOID );
WORD output_grammar( VOID );


/*****************/
/* Deklarationen */
/*****************/




/****************/
/* Definitionen */
/****************/

/* Ausgabe in Datei */
FILE	*bnf_file = 0L;




/***********/
/* Imports */
/***********/

MGLOBAL NTERM_FIRST	**nterm_first;
MGLOBAL HULLE_STR	**hulle_str;
MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL TERM_STR	*term_str;
MGLOBAL LR_INFO		lr_info;
MGLOBAL LR_TAB		**lr_tab;
MGLOBAL ULONG		prod_count;
MGLOBAL ULONG		nterm_length;
MGLOBAL ULONG		nterm_first_count;
MGLOBAL ULONG		main_dbl_count;
MGLOBAL LONG		*prod_index_arr;
MGLOBAL LONG		epsilon;
MGLOBAL LONG		hulle_count;
MGLOBAL LONG		nterm_count;
MGLOBAL LONG		**jump_tab;
MGLOBAL LONG		term_count;
MGLOBAL BYTE		*file_name;
MGLOBAL FILE		*file;




WORD output_grammar( VOID )
{
	/* Hier wird die Grammatik in BNF-Form ausgegeben. */
	BYTE		*temp_file_name;
	REG LONG	i;

	/* Meldung ausgeben */
	fprintf( stderr, "\nErzeuge BNF-Grammatik-Datei..." );

	/* Dateiendung Ñndern */
	temp_file_name = strrchr( file_name, ( WORD ) '.' );
	strcpy( temp_file_name, ".GRA" );

	/* Datei zum Schreiben îffnen */
	if( ( bnf_file = fopen( file_name, "w" ) ) == NULL )
		/* Fehler */
		return( error( CANNOT_OPEN_BNF_FILE, 0, 0 ) );

	/* Namen der Grammatik ausgeben */
	fprintf( bnf_file,	";Grammatik %s in BNF-Form"
						"\n;Generiert von LR-Parser-Generator V0.00ss"
						"\n;(C) by Sven Schmidt\n", file_name );

	/* Alle NTERM_STRs */
	for( i = 0; i <= nterm_count; i++ )
	{
		/* Produktion ausgeben */
		fprintf( bnf_file, "\n\n%s\n\t\t->\t", hash_tab[nterm_str[i]->hash_tab_index].name );

		/* Rechte Seite ausgeben */
		output_right( bnf_file, i );

		/* Solange Alternativen da sind */
		while( ( ( i + 1 ) <= nterm_count ) && ( nterm_str[i + 1]->alt ) )
		{
			/* NÑchste Alternative */
			i++;

			/* Alternativen ausgeben */
			fputs( "\n\t\t|\t", bnf_file );

			/* Rechte Seite ausgeben */
			output_right( bnf_file, i );
		}

		/* Mit Semicolon abschliessen */
		fputs( "\n\t\t;", bnf_file );
	}

	/* Datei schliessen */
	if( fclose( bnf_file ) < 0 )
		/* Fehler */
		return( error( CANNOT_CLOSE_BNF_FILE, 0, 0 ) );

	/* Zuruecksetzen */
	bnf_file = 0L;

	/* Alles OK */
	return( 0 );
}


VOID output_right( FILE *file, LONG nterm_index )
{
	/* Hier wird die rechte Seite ausgegeben. */
	NTERM_STR	*nterm = nterm_str[nterm_index];
	WORD		j;

	/* Alle Elemente auf der rechten Seite ausgeben */
	for( j = 0; j < nterm->anz_ri; j++ )
	{
		/* Ist es ein Terminal ? */
		if( hash_tab[nterm->ri[j]].term_str )
			/* Ja, ist ein Terminal */
			fprintf( file, " \'%s\'", hash_tab[nterm->ri[j]].name );

		else
			/* Ist ein Nichtterminal */
			fprintf( file, " %s", hash_tab[nterm->ri[j]].name );
	}
}


WORD output_first( VOID )
{
	/* Hier werden die FIRST-Mengen ausgegeben. */
	REG ULONG	i, j;
	BYTE		sep[256];

	/* Header ausgeben */
	fputs(	"\n\n\n****************\n* FIRST-Mengen *\n"
			"****************\n", file );

	/* Alle NTERM_STRs */
	for( i = 0; i < nterm_first_count; i++ )
	{
		/* Nichtterminal optisch trennen */
		for( j = 0; j < ( WORD ) strlen( hash_tab[nterm_str[nterm_first[i]->nterm_index]->hash_tab_index].name ); j++ )
			/* - schreiben */
			sep[j] = '-';

		/* Null-Byte */
		sep[j] = 0;

		/* Nichtterminal ausgeben */
		fprintf( file, "\n\n%s\n%s\n\n", hash_tab[nterm_str[nterm_first[i]->nterm_index]->hash_tab_index].name, sep );

		/* FIRST-Menge ausgeben */
		for( j = 0; j < nterm_first[i]->first.anz_elem; j++ )
			/* Terminal ausgeben */
			fprintf( file, "%s\n", hash_tab[nterm_first[i]->first.term_arr[j]].name );
	}

	/* Alles OK */
	return( 0 );
}


WORD output_follow( VOID )
{
	/* Hier werden die FOLLOW-Mengen ausgegeben. */
	REG ULONG	i, j;
	BYTE		sep[256];

	/* Header ausgeben */
	fputs(	"\n\n\n*****************\n* FOLLOW-Mengen *\n"
			"*****************\n", file );

	/* Alle NTERM_STRs */
	for( i = 0; i < prod_count; i++ )
	{
		/* Nichtterminal optisch trennen */
		for( j = 0; j < ( WORD ) strlen( hash_tab[nterm_str[prod_index_arr[i]]->hash_tab_index].name ); j++ )
			/* - schreiben */
			sep[j] = '-';

		/* Null-Byte */
		sep[j] = 0;

		/* Nichtterminal ausgeben */
		fprintf( file, "\n\n%s\n%s\n\n", hash_tab[nterm_str[prod_index_arr[i]]->hash_tab_index].name, sep );

		/* FOLLOW-Menge ausgeben */
		for( j = 0; j < nterm_str[prod_index_arr[i]]->follow.anz_elem; j++ )
			/* Terminal ausgeben */
			fprintf( file, "%s\n", hash_tab[nterm_str[prod_index_arr[i]]->follow.term_arr[j]].name );
	}

	/* Alles OK */
	return( 0 );
}


WORD output_items( VOID )
{
	/* Hier werden die SLR- bzw. LR-Items ausgegeben. */
	REG LONG	i;

	/* Meldung ausgeben */
	fprintf( stderr, "\nGebe Items aus..." );

	/* Meldung ausgeben */
	if( !lr_info.analyze )
	{
		/* SLR-Parser ? */
		if( lr_info.type == SLR )
			/* Meldung ausgeben */
			fputs(	"\n\n\n***************\n"
					"* LR(0)-Items *\n***************\n", file );

		else
			/* Meldung ausgeben */
			fputs(	"\n\n\n***************\n"
					"* LR(1)-Items *\n***************\n", file );
	}

	/* Alle HÅllen bearbeiten */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Meldung */
		fprintf( file, "\n\n\nI%lu = Sprung( I%lu, %s )\n", i, hulle_str[i]->index, hash_tab[hulle_str[i]->nterm].name );

		/* HÅlle ausgeben */
		if( show_hulle( i ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Alles OK */
	return( 0 );
}


WORD show_hulle( LONG hulle_index )
{
	/* In dieser Routine wird eine HÅlle ausgebenen. */
	REG ULONG	i, j;
	ITEM_STR	*item;
	NTERM_STR	*nterm;
	LONG		index;
	BYTE		temp[256];

	/* Alle Items der HÅlle ausgeben */
	for( i = 0; i < hulle_str[hulle_index]->anz_items; i++ )
	{
		/* Zeiger auf Item holen */
		item = &hulle_str[hulle_index]->item_str[i];

		/* Zeiger auf NTERM_STR holen */
		nterm = nterm_str[item->nterm_index];

		/* Ist das Item ein Kern ? */
		if( i < hulle_str[hulle_index]->anz_kitems )
			/* Ja */
			fputs( "\nKern:\t", file );

		else
			/* Nein */
			fputs( "\n\t\t", file );

		/* Ist es ein Reduce-Item ? */
		if( item->pkt_pos < nterm->anz_ri )
		{
			/*
				Nun den Zustand ausgeben, den der Parser erreicht, wenn er das
				Element nach dem Punkt auf der rechten Seite geschoben hat.
			*/
			/* Ist es ein Terminal ? */
			if( hash_tab[item->elem].term_str )
			{
				/* Ja, ist ein Terminal */

				/* Index des Terminals holen */
				if( ( index = get_term_index( item->elem ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Aufgrund Shift-/Reduce-Error reduzieren ? */
				if( lr_tab[hulle_index][index].state == REDUCE )
					/* Ja */
					fputs( "\t\t", file );

				else
					/* Zustand ausgeben */
					fprintf( file, "%3ld:\t", lr_tab[hulle_index][index].value );
			}

			else
				/* Zustand ausgeben */
				fprintf( file, "%3ld:\t", jump_tab[hulle_index][nterm_str[hash_tab[item->elem].nterm_index]->prod_index - 1] );
		}

		else
			/* Ist ein Reduce-Item */
			fputs( "\t\t", file );

		/* Hat die Produktion Alternativen ? */
		if( !( nterm->alt ) && ( ( ( item->nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[item->nterm_index + 1]->alt ) ) ) )
			/* Nein, Produktion ausgeben */
			fprintf( file, "%s\t->\t", hash_tab[nterm->hash_tab_index].name );

		else
		{
			/* Ja */
			sprintf( temp, "%s_%lu", hash_tab[nterm->hash_tab_index].name, item->nterm_index - ( item->nterm_index - nterm->alt ) );
			fprintf( file, "%s\t->\t", temp );
		}

		/* Item ausgeben */
		output_item( item->nterm_index, item->pkt_pos );

		/* Lookahead ausgeben ? */
		if( item->lookahead.anz_elem )
		{
			/* Ja */
			for( j = 0; j < item->lookahead.anz_elem; j++ )
				/* Lookahead ausgeben */
				fprintf( file, ", %s", hash_tab[item->lookahead.term_arr[j]].name );
		}
	}

	#ifdef ATARI
		/* Ausgabe auf Bildschirm ? */
		if( lr_info.dest == DSCREEN )
			/* Auf Tastendruck warten */
			Cnecin();
	#endif

	/* Alles OK */
	return( 0 );
}


VOID output_item( LONG nterm_index, WORD pkt_pos )
{
	/* Hier wird ein Item ausgegeben. */
	NTERM_STR	*nterm = nterm_str[nterm_index];
	REG ULONG	i;

	/* Die Rechte Seite ausgeben */
	for( i = 0; i < nterm->anz_ri; i++ )
	{
		/* Punkt ausgeben ? */
		if( pkt_pos == i )
			/* Punkt ausgeben */
			fputs( ".", file );

		/* Ist es ein Terminal ? */
		if( hash_tab[nterm->ri[i]].term_str )
			/* Ja, ist ein Terminal */
			fprintf( file, " \'%s\'", hash_tab[nterm->ri[i]].name );

		else
			/* Ist ein Nichtterminal */
			fprintf( file, " %s", hash_tab[nterm->ri[i]].name );
	}

	/* Punkt ausgeben ? */
	if( pkt_pos == i )
		/* Punkt ausgeben */
		fputs( ".", file );
}


WORD output_lr_tab( VOID )
{
	/* Hier wird die LR-Tabelle ausgegeben. */
	WORD		max_term;
	REG ULONG	t_count, eps_flg, i, j, nterm_index;
	BYTE		nterm[256], sep[256];

	/* Meldung ausgeben */
	fprintf( stderr, "\nGebe LR-Tabelle aus..." );

	/* Jump-Tabelle ausgeben */
	output_goto_tab();

	/* Korrekte formatierte Ausgabe erzwingen */
	nterm_length += 3;

	/* Terminale optisch trennen */
	for( i = 0; i < nterm_length; i++ )
		/* * schreiben */
		sep[i] = '*';

	/* Null-Byte */
	sep[i] = 0;

	/* LR-Tabelle ausgeben */
	fputs( "\n\n\n**************\n* LR-Tabelle *\n**************", file );

	/* Anzahl der Terminale in einer Zeile */
	max_term = ( WORD ) ( lr_info.max_char / ( nterm_length ) ) - 1;

	/* Wurde EPSILON benutzt ? */
	if( hash_tab[epsilon].def )
		/* Ja, mit ausgeben */
		t_count = eps_flg = 0;

	else
		/* Nein, nicht mit ausgeben */
		t_count = eps_flg = 1;

	/* Alle Terminale */
	while( t_count < term_count )
	{
		/* NÑchste Zeile */
		fputs( "\n\n\n\t\t|", file );

		/* Alle Terminale bearbeiten */
		for( i = t_count; ( i <= ( t_count + max_term ) ) && ( i <= term_count ); i++ )
			/* Terminal ausgeben */
			fprintf( file, " %-*s |", ( WORD ) nterm_length, hash_tab[term_str[i].hash_tab_index].name );

		/* Optische Trennung */
		fputs( "\n\t\t|", file );

		/* Ausgeben */
		for( i = t_count; ( i <= ( t_count + max_term ) ) && ( i <= term_count ); i++ )
			/* Terminal ausgeben */
			fprintf( file, "*%s*|", sep );

		/* Alle HÅllen durchgehen */
		for( i = 0; i <= hulle_count; i++ )
		{
			/* Nummer der HÅlle ausgeben */
			fprintf( file, "\n\t%4lu|", i );

			/* Alle Terminale durchgehen */
			for( j = t_count; j <= ( t_count + max_term ) && ( j <= term_count ); j++ )
			{
				/* Shift ? */
				if( lr_tab[i][j].state == SHIFT )
					/* Ja */
					fprintf( file, " s%-*lu|", ( WORD ) nterm_length, lr_tab[i][j].value );

				/* Reduce ? */
				else if( lr_tab[i][j].state == REDUCE )
				{
					/* Index der Produktion holen */
					nterm_index = lr_tab[i][j].value;

					/* Hat die Produktion Alternativen ? */
					if( !( nterm_str[nterm_index]->alt ) && ( ( ( nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[nterm_index + 1]->alt ) ) ) )
						/* Nein */
						fprintf( file, " %-*s |", ( WORD ) nterm_length, hash_tab[nterm_str[nterm_index]->hash_tab_index].name );

					else
					{
						/* Produktion hat Alternativen */
						sprintf( nterm, "%s_%lu", hash_tab[nterm_str[nterm_index]->hash_tab_index].name, nterm_index - ( nterm_index - nterm_str[nterm_index]->alt ) );
						fprintf( file, " %-*s |", ( WORD ) nterm_length, nterm );
					}
				}

				else
					/* Fehlereintrag */
					fprintf( file, " %-*d |", ( WORD ) nterm_length, 0 );
			}
		}

		/* Die nÑchsten Terminale ausgeben */
		t_count += ( max_term + 1 );
	}

	/* Zweite Form der Tabelle ausgeben */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Nummer der HÅlle ausgeben */
		fprintf( file, "\n\n***************\n* Zustand %3lu *\n***************\n", i );

		/* Alle Terminale */
		for( j = eps_flg; j <= term_count; j++ )
		{
			/* Shift oder Reduce ? */
			if( lr_tab[i][j].state )
			{
				/* Terminal ausgeben */
				fprintf( file, "\n%-*s: ", ( WORD ) nterm_length + 2, hash_tab[term_str[j].hash_tab_index].name );

				/* Shift ? */
				if( lr_tab[i][j].state == SHIFT )
					/* Zustand ausgeben */
					fprintf( file, "s%lu", lr_tab[i][j].value );
		
				else
				{
					/* Reduce */

					/* Index der Produktion holen */
					nterm_index = lr_tab[i][j].value;

					/* Hat die Produktion Alternativen ? */
					if( !( nterm_str[nterm_index]->alt ) && ( ( ( nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[nterm_index + 1]->alt ) ) ) )
						/* Nein */
						fprintf( file, "%s", hash_tab[nterm_str[nterm_index]->hash_tab_index].name );

					else
					{
						/* Produktion hat Alternativen */
						sprintf( nterm, "%s_%lu", hash_tab[nterm_str[nterm_index]->hash_tab_index].name, nterm_index - ( nterm_index - nterm_str[nterm_index]->alt ) );
						fprintf( file, "%s", nterm );
					}
				}
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


VOID output_goto_tab( VOID )
{
	/* Hier wird die GOTO-Tab ausgegeben. */
	REG ULONG	i, j, p_count;
	WORD		max_prod;
	BYTE		sep[256], empty[256];

	/* Meldung ausgeben */
	fputs(	"\n\n\n****************\n* GOTO-Tabelle *\n"
			"****************\n", file );

	/* Produktionen optisch trennen */
	for( i = 0; i < nterm_length; i++ )
	{
		/* * schreiben */
		sep[i] = '*';
		empty[i] = ' ';
	}

	/* Null-Byte */
	sep[i] = '*';
	sep[i + 1] = '*';
	sep[i + 2] = '|';
	sep[i + 3] = 0;

	/* fÅr die leeren EintrÑge */
	empty[i] = ' ';
	empty[i + 1] = ' ';
	empty[i + 2] = '|';
	empty[i + 3] = 0;

	/* Anzahl der Produktionen in einer Zeile */
	max_prod = ( WORD ) ( lr_info.max_char / ( nterm_length ) ) - 1;

	/* Alle Produktionen */
	for( p_count = 0; p_count < prod_count; p_count++ )
	{
		/* NÑchste Zeile */
		fputs( "\n\n\n\t\t|", file );

		/* Alle Produktionen bearbeiten */
		for( i = p_count; ( i <= ( p_count + max_prod ) ) && ( i < prod_count ); i++ )
			/* Produktion ausgeben */
			fprintf( file, " %-*s |", ( WORD ) nterm_length, hash_tab[nterm_str[prod_index_arr[i]]->hash_tab_index].name );

		/* Optische Trennung */
		fputs( "\n\t\t|", file );

		/* Ausgeben */
		for( i = p_count; ( i <= ( p_count + max_prod ) ) && ( i < prod_count ); i++ )
			/* Produktion ausgeben */
			fputs( sep, file );

		/* Alle HÅllen durchgehen */
		for( i = 0; i <= hulle_count; i++ )
		{
			/* Nummer der HÅlle ausgeben */
			fprintf( file, "\n\t%4lu|", i );

			/* Ausgeben */
			for( j = p_count; ( j <= ( p_count + max_prod ) ) && ( j < prod_count ); j++ )
			{
				/* Gibt es einen Sprung ? */
				if( jump_tab[i][j] )
					/* Ja */
					fprintf( file, " %-*lu |", ( WORD ) nterm_length, jump_tab[i][j] );

				else
					/* Nein */
					fputs( empty, file );
			}
		}

		/* Die nÑchsten Produktionen ausgeben */
		p_count += max_prod;
	}
}