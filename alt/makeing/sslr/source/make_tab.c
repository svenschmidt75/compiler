/*
	Projekt		:	LR-Parser-Generator
	Modul		:	MAKE_TAB.C
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

WORD insert_lr_item( HULLE_STR *hulle, LONG item_index, LONG alt, WORD pkt_pos );
WORD insert_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos );
LONG find_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos );
WORD insert_reduce( LONG hulle, LONG term_index, LONG nterm_index );
WORD insert_error_index( HULLE_STR *hulle, LONG error_index );
WORD cmp_lr_tab( ULONG lr_tab1_index, ULONG lr_tab2_index );
WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD insert_hulle_hash( LONG elem, LONG hulle_nr );
VOID output_item( LONG nterm_index, WORD pkt_pos );
VOID output_right( FILE *file, LONG nterm_index );
WORD insert_term( FIRST_STR *first, LONG term );
WORD find_term( FIRST_STR *first, LONG term );
LONG cmp_hulle( LONG term, HULLE_STR *hulle );
LONG do_goto( HULLE_STR *hulle, LONG term );
WORD lalr_slr_hulle( HULLE_STR *hulle );
WORD lr_hulle( HULLE_STR *hulle );
WORD do_hulle( HULLE_STR *hulle );
LONG get_term_index( LONG term );
VOID set_empty_to_reduce( VOID );
VOID do_grammar_error( VOID );
WORD spead_lookaheads( VOID );
WORD append_lookahead( VOID );
WORD allocate_mem( VOID );
WORD make_hulle0( VOID );
WORD find_items( VOID );
WORD lr_to_lalr( VOID );



/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/* LR-Tabelle */
LR_TAB			**lr_tab = 0L;

/* Beinhaltet Grammatik-Fehler */
GRA_ERR_STR		*gra_err_str = 0L;

/* Sprung-Tabelle */
LONG			**jump_tab = 0L;

/* Ziel-Tabelle */
ULONG			*ziel_tab = 0L;

/* Speicher fÅr die Fehler in den HÅllen */
LONG			*error_hulle;

/* Anzahl der EintrÑge in der GRA_ERR_STR */
ULONG			gra_err_str_count = 0;




/***********/
/* Imports */
/***********/

MGLOBAL NTERM_FIRST	**nterm_first;
MGLOBAL ERROR_STR	**error_struct;
MGLOBAL NTERM_STR	**nterm_str;
MGLOBAL HULLE_STR	**hulle_str;
MGLOBAL HASH_TAB	*hash_tab;
MGLOBAL TERM_STR	*term_str;
MGLOBAL LR_INFO		lr_info;
MGLOBAL ULONG		prod_count;
MGLOBAL ULONG		nterm_first_count;
MGLOBAL LONG		*prod_index_arr;
MGLOBAL LONG		epsilon;
MGLOBAL LONG		accept;
MGLOBAL LONG		term_count;
MGLOBAL LONG		nterm_count;
MGLOBAL LONG		error_struct_count;
MGLOBAL LONG		hulle_count;
MGLOBAL WORD		sr_err;
MGLOBAL WORD		rr_err;
MGLOBAL FILE		*file;



WORD make_tab( VOID )
{
	/* Hier wird die LR-Tabelle erzeugt. */
	REG ULONG	i, j, k;
	NTERM_STR	*nterm;
	ITEM_STR	*item;
	LONG		t_index;
	WORD		alt;

	/* Meldung ausgeben */
	fprintf( stderr, "\nErzeuge die LR-Tabelle..." );

	/* Speicher fÅr die Fehler in den HÅllen anfordern */
	if( ( error_hulle = ( LONG * ) calloc( ( hulle_count + 1 ), sizeof( LONG ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Protokollausgabe ? */
	#ifdef PROT

		/* Ausgabe der NTERM_STR */
		for( i = 0; i <= nterm_count; i++ )
		{
			fprintf( file, "\n\nnterm_str[%3lu]->hash_tab_index  = 0x%lu = %s_%lu", i, nterm_str[i]->hash_tab_index, hash_tab[nterm_str[i]->hash_tab_index].name, i - ( i - nterm_str[i]->alt ) );
			fprintf( file, "\nnterm_str[%3lu]->priori          = %d", i, nterm_str[i]->priori );
			fprintf( file, "\nnterm_str[%3lu]->assoc           = %d", i, nterm_str[i]->assoc );
			fprintf( file, "\nnterm_str[%3lu]->alt             = %d", i, nterm_str[i]->alt );
			fprintf( file, "\nnterm_str[%3lu]->post_index      = %ld", i, nterm_str[i]->post_index );
			fprintf( file, "\nnterm_str[%3lu]->prod_index      = %ld", i, nterm_str[i]->prod_index );
			fprintf( file, "\nnterm_str[%3lu]->anz_ri          = %d", i, nterm_str[i]->anz_ri );
			fprintf( file, "\nnterm_str[%3lu]->follow          = 0x%p", i, &nterm_str[i]->follow );
			fprintf( file, "\nnterm_str[%3lu]->follow.anz_elem = %lu", i, nterm_str[i]->follow.anz_elem );

			/* Die Produktion ausgeben */
			fprintf( file, "\n%s\t\t->\t", hash_tab[nterm_str[i]->hash_tab_index].name );

			/* Alle Elemente auf der rechten Seite ausgeben */
			for( j = 0; j < nterm_str[i]->anz_ri; j++ )
			{
				/* Ist es ein Terminal ? */
				if( hash_tab[nterm_str[i]->ri[j]].term_str )
					/* Ja, ist ein Terminal */
					fprintf( file, " \'%s\'", hash_tab[nterm_str[i]->ri[j]].name );

				else
					/* Ist ein Nichtterminal */
					fprintf( file, " %s", hash_tab[nterm_str[i]->ri[j]].name );
			}
		}

	#endif

	/* Zuerst Speicher anfordern */
	if( allocate_mem() < 0 )
		/* Fehler */
		return( -1 );

	/* Die ZustandsuebergÑnge erzeugen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Alle Items durchgehen */
		for( j = 0; j < hulle_str[i]->anz_items; j++ )
		{
			/* Zeiger auf das Item holen */
			item = &hulle_str[i]->item_str[j];

			/* Zeiger auf NTERM_STR des Items holen */
			nterm = nterm_str[item->nterm_index];

			/* Ist es ein Reduce-Item ? */
			if( ( nterm->anz_ri > item->pkt_pos ) && !( hash_tab[item->elem].term_str ) )
				/* Den Folgezustand eintragen */
				jump_tab[i][nterm_str[hash_tab[item->elem].nterm_index]->prod_index - 1] = item->next_state;
		}
	}

	/* Die Shift-EintrÑge erzeugen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Alle Items */
		for( j = 0; j < hulle_str[i]->anz_items; j++ )
		{
			/* Zeiger auf das Item holen */
			item = &hulle_str[i]->item_str[j];

			/* Zeiger auf NTERM_STR holen */
			nterm = nterm_str[item->nterm_index];

			/* Ist der Punkt vor einem Terminal ? */
			if( ( nterm->anz_ri > item->pkt_pos ) && ( hash_tab[item->elem].term_str ) )
			{
				/* Ja, ist es EPSILON ? */
				if( item->elem == epsilon )
				{
					/* Index von EPSILON holen */
					t_index = get_term_index( epsilon );

					/* Reduce eintragen */
					insert_reduce( i, t_index, item->nterm_index );
				}

				else
				{
					/* Die HÅlle dazu bilden */

					/* ACCEPT wird nicht geschoben */
					if( item->elem != accept )
					{
						/* Index des Terminals holen */
						t_index = get_term_index ( item->elem );

						/* EintrÑge in die LR-Tabelle vornehmen */
						lr_tab[i][t_index].state = SHIFT;
						lr_tab[i][t_index].value = item->next_state;
					}
				}
			}
		}
	}

	/* Zustand 0, ACCEPT -> acceptiere */
	t_index = get_term_index( accept );

	/* Eintragung vornehmen */
	lr_tab[1][t_index].state = REDUCE;
	lr_tab[1][t_index].value = 0;

	/* Die Reduce-EintrÑge erzeugen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Alle Items */
		for( j = 0; j < hulle_str[i]->anz_items; j++ )
		{
			/* Zeiger auf das Item holen */
			item = &hulle_str[i]->item_str[j];

			/* Ist es ein Reduce-Item ? */
			if( ( nterm_str[item->nterm_index]->anz_ri == item->pkt_pos ) || ( ( nterm_str[item->nterm_index]->anz_ri == 1 ) && ( nterm_str[item->nterm_index]->ri[0] == epsilon ) ) )
			{
				/* Ist es eine SLR-Tabelle ? */
				if( lr_info.type == SLR )
				{
					/* Alternativen beachten */
					alt = nterm_str[item->nterm_index]->alt;

					/* FOLLOW-Menge holen */
					for( k = 0; k < nterm_str[item->nterm_index - alt]->follow.anz_elem; k++ )
					{
						/* Index des Terminals holen */
						t_index = get_term_index( nterm_str[item->nterm_index - alt]->follow.term_arr[k] );

						/* Reduce eintragen */
						if( insert_reduce( i, t_index, item->nterm_index ) < 0 )
							/* Fehler */
							return( -1 );
					}
				}

				else
				{
					/* Ist eine LR- bzw- LALR-Tabelle */
					for( k = 0; k < item->lookahead.anz_elem; k++ )
					{
						/* Index des Terminals holen */
						t_index = get_term_index( item->lookahead.term_arr[k] );

						/* Reduce eintragen */
						if( insert_reduce( i, t_index, item->nterm_index ) < 0 )
							/* Fehler */
							return( -1 );
					}
				}
			}
		}
	}

	/* Leere ZustÑnde ggf auf Reduce setzen */
	set_empty_to_reduce();

	/* Die Grammatik-Fehler auswerten */
	do_grammar_error();

	/* Speicher wieder freigeben */
	free( ( VOID * ) error_hulle );

	/* Zuruecksetzen */
	error_hulle = 0L;

	/* Alles OK */
	return( 0 );
}


WORD allocate_mem( VOID )
{
	/* Hier wird der Speicher fÅr die Erzeugung der LR-Tabelle angefordert. */
	REG ULONG	i;

	/* Array mit Zeigern auf LR_TAB */
	if( ( lr_tab = ( LR_TAB ** ) malloc( sizeof( LR_TAB * ) * ( hulle_count + 1 ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Zeiger auf LR_TAB in lr_tab eintragen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Speicher fÅr LR_TAB anfordern */
		if( ( lr_tab[i] = ( LR_TAB * ) calloc( term_count + 1, sizeof( LR_TAB ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );
	}

	/* Array mit Zeigern auf die Sprungwerte */
	if( ( jump_tab = ( LONG ** ) malloc( sizeof( LONG * ) * ( hulle_count + 1 ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Zeiger auf LONG in Sprung-Tabelle eintragen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Speicher fÅr LR_TAB anfordern */
		if( ( jump_tab[i] = ( LONG * ) calloc( prod_count + 1, sizeof( LONG ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );
	}

	/* Speicher fÅr die Ziel-Tabelle anfordern */
	if( ( ziel_tab = ( ULONG * ) calloc( hulle_count + 1, sizeof( ULONG ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


LONG get_term_index( LONG term )
{
	/* Zu dem Terminal term wird der Index zurueckgeliefert. */
	REG ULONG	i;

	/* Alle Terminale pruefen */
	for( i = 0; i <= term_count; i++ )
	{
		/* Ist es das gesuchte Terminal ? */
		if( term_str[i].hash_tab_index == term )
			/* Ja */
			return( i );
	}

	/* Terminal war nicht dabei */
	return( -1 );
}


WORD insert_reduce( LONG hulle, LONG term_index, LONG nterm_index )
{
	/* Hier wird ein Reduce-Zustand in die LR-Tabelle eingetragen. */
	GRA_ERR_STR		*err_str;
	REG ULONG		i;
	WORD			term_priori, prod_priori, state = SHIFT;
	LONG			nterm_i;

	/* Ist an dieser Stelle schon ein Reduce-Eintrag ? */
	if( lr_tab[hulle][term_index].state == REDUCE )
	{
		/* Ja, es liegt ein Reduce-/Reduce-Konflikt vor */

		/* Speicher fÅr eine GRA_ERR_STR-Struktur anfordern */
		if( ( gra_err_str = ( GRA_ERR_STR * ) realloc( ( VOID * ) gra_err_str, sizeof( GRA_ERR_STR ) * ( gra_err_str_count + 1 ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Ist ein Reduce-/Reduce-Fehler */
		gra_err_str[gra_err_str_count].type = REDUCE;

		/* Nummer der HÅlle eintragen */
		gra_err_str[gra_err_str_count].hulle = hulle;

		/* Index der alten Produktion holen */
		nterm_i = lr_tab[hulle][term_index].value;

		/* Index der alten Produktion */
		gra_err_str[gra_err_str_count].prod1 = nterm_i;

		/* Ist es eine Alternative ? */
		while( nterm_str[nterm_i]->alt )
			/* Ja */
			nterm_i--;

		/* Nummer der Alternative eintragen */
		gra_err_str[gra_err_str_count].prod1_alt = ( WORD ) ( nterm_i > 0 ? ( nterm_i - lr_tab[hulle][term_index].value ) : -1 );

		/* Index der neuen Produktion */
		gra_err_str[gra_err_str_count].prod2 = nterm_i = nterm_index;

		/* Ist es eine Alternative ? */
		while( nterm_str[nterm_i]->alt )
			/* Ja */
			nterm_i--;

		/* Nummer der Alternative eintragen */
		gra_err_str[gra_err_str_count++].prod2_alt = ( WORD ) ( nterm_i > 0 ? ( nterm_index - nterm_i ) : -1 );
	}

	/* Ist an dieser Stelle schon ein Shift-Eintrag ? */
	else if( lr_tab[hulle][term_index].state == SHIFT )
	{
		/* Ja, es liegt ein Shift-/Reduce-Konflikt vor */
		for( i = 0; i < gra_err_str_count; i++ )
		{
			/* Gibt es fÅr diese HÅlle bereits eine Struktur ? */
			if( ( gra_err_str[i].hulle == hulle ) && ( gra_err_str[i].type == SHIFT ) && ( gra_err_str[i].prod1 == nterm_index ) )
			{
				/* Ja */

				/* Default ist schieben */
				state = SHIFT;

				/* PrioritÑt des Terminals holen */
				term_priori = term_str[term_index].priori;

				/* PrioritÑt der Produktion holen */
				prod_priori = nterm_str[nterm_index]->priori;

				/* Nur testen, wenn eine der beiden PrioritÑten angegeben wurde ! */
				if( ( prod_priori >= 0 ) || ( term_priori >= 0 ) )
				{
					/* PrioritÑt der Produktion > PrioritÑt des Terminals ? */
					if( prod_priori > term_priori )
						/* Reduzieren */
						state = REDUCE;

					/* PrioritÑten gleich, Produktion linksassoziativ ? */
					else if( ( prod_priori == term_priori ) && ( nterm_str[nterm_index]->assoc == LEFT ) )
						/* Reduzieren */
						state = REDUCE;

					else
						/* Schieben */
						state = SHIFT;
				}

				else
					/* Weder die PrioritÑt des Terminals noch die der Produktion wurden angegeben -> schieben */
					state = SHIFT;

				/* Sind die beiden States gleich ? */
				if( state == gra_err_str[i].state )
				{
					/* Ja, nur ein weiteres Terminal hinzufÅgen */
					if( ( gra_err_str[i].term = ( LONG * ) realloc( ( VOID * ) gra_err_str[i].term, sizeof( LONG ) * ( gra_err_str[i].term_count + 1 ) ) ) == NULL )
						/* Fehler */
						return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

					/* Index der Terminals eintragen */
					gra_err_str[i].term[gra_err_str[i].term_count++] = term_index;
				}

				break;
			}
		}

		/* Gibt es fÅr diese HÅlle bereits eine Struktur ? */
		if( i == gra_err_str_count )
		{
			/* Speicher fÅr eine GRA_ERR_STR-Struktur anfordern */
			if( ( gra_err_str = ( GRA_ERR_STR * ) realloc( ( VOID * ) gra_err_str, sizeof( GRA_ERR_STR ) * ( gra_err_str_count + 1 ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Zeiger auf Struktur holen */
			err_str = &gra_err_str[gra_err_str_count++];

			/* Zeiger Initialisieren */
			err_str->term = 0L;
			err_str->term_count = 0;

			/* Index des Terminals holen */
			if( ( err_str->term = ( LONG * ) realloc( ( VOID * ) err_str->term, sizeof( LONG ) * ( err_str->term_count + 1 ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Index der Terminals eintragen */
			err_str->term[err_str->term_count++] = term_index;

			/* PrioritÑt des Terminals holen */
			term_priori = term_str[term_index].priori;

			/* PrioritÑt der Produktion holen */
			prod_priori = nterm_str[nterm_index]->priori;

			/* Nur testen, wenn einer der beiden PrioritÑten angegeben wurde ! */
			if( ( prod_priori >= 0 ) || ( term_priori >= 0 ) )
			{
				/* PrioritÑt der Produktion > PrioritÑt des Terminals ? */
				if( prod_priori > term_priori )
					/* Reduzieren */
					err_str->state = REDUCE;

				/* PrioritÑten gleich, Produktion linksassoziativ ? */
				else if( ( prod_priori == term_priori ) && ( nterm_str[nterm_index]->assoc == LEFT ) )
					/* Reduzieren */
					err_str->state = REDUCE;

				else
					/* Schieben */
					err_str->state = SHIFT;
			}

			else
				/* Weder die PrioritÑt des Terminals noch die der Produktion wurden angegeben -> schieben */
				err_str->state = SHIFT;

			/* Nummer der HÅlle eintragen */
			err_str->hulle = hulle;

			/* Index der Produktion */
			err_str->prod1 = nterm_i = nterm_index;

			/* Ist es eine Alternative ? */
			while( nterm_str[nterm_i]->alt )
				/* Ja */
				nterm_i--;

			/* Nummer der Alternative eintragen */
			err_str->prod1_alt = ( WORD ) ( nterm_i > 0 ? ( nterm_index - nterm_i ) : -1 );

			/* Ist ein Shift-/Reduce-Fehler */
			err_str->type = SHIFT;
		}
	}

	else
	{
		/* EintrÑge in die LR-Tabelle vornehmen */
		lr_tab[hulle][term_index].state = REDUCE;
		lr_tab[hulle][term_index].value = nterm_index;
	}

	/* Ist schon etwas eingetagen worden ? */
	if( !ziel_tab[hulle] )
		/* Eintrag in Ziel-Tab merken */
		ziel_tab[hulle] = nterm_index;

	/* Alles OK */
	return( 0 );
}


VOID set_empty_to_reduce( VOID )
{
	/*
		Gibt es in einem Zustand nur Reduces, so werden auch alle
		LeereintrÑge auf Reduce gesetzt.
	*/
	REG ULONG	i, j, reduce;

	/* Alle HÅllen */
	for( j = 0; j <= hulle_count; j++ )
	{
		/* Alle Terminale durchgehen */
		for( i = 0; i <= term_count; i++ )
		{
			/* Ist es ein Reduce-Eintrag ? */
			if( lr_tab[j][i].state == REDUCE )
			{
				/* Den Wert merken */
				reduce = lr_tab[j][i].value;

				/* Alle Terminale durchgehen */
				for( i = 0; i <= term_count; i++ )
				{
					/* Leereintrag ? */
					if( !lr_tab[j][i].state )
					{
						/* Alle LeereintrÑge auf Reduce setzen */
						lr_tab[j][i].state = REDUCE;
						lr_tab[j][i].value = reduce;
					}
				}
			}
		}
	}
}


VOID do_grammar_error( VOID )
{
	/* Hier werden shift-reduce-/reduce-reduce-Fehler ausgewertet. */
	REG ULONG	i, j;
	GRA_ERR_STR	*gra_str;
	BYTE		*output_text;

/*
	Zum Shift-/Reduce-Fehler
	------------------------

	Ein Shift-/Reduce-Fehler tritt dann aus, wenn das Terminal geschoben werden kann, oder es in dem
	Nichtterminal als Follow-Symbol ist, mit dem reduziert werden kann.

	Bsp:

	FOLLOW( NTERM1 ) = { TERM1, TERM2 }


	HÅlle(0)
	--------

	NTERM1		->	NTERM2.
	NTERM1		->	.NTERM3
	NTERM3		->	.TERM1


	TERM1 ist in FOLLOW( NTERM1 ) enthalten( mit dem Nichtterminal wird reduziert ), und TERM1 kann auch geschoben
	werden.

	Abhilfe: AssoziativitÑt/PrioritÑt
*/

	/* Alle EintrÑge in der GRA_ERR_STR */
	for( i = 0; i < gra_err_str_count; i++ )
	{
		/* Zeiger auf die GRA_ERR_STR holen */
		gra_str = &gra_err_str[i];

		/* Meldung ausgeben */
		if( gra_str->type == SHIFT )
		{
			/* ZÑhler hochsetzen */
			sr_err++;

			/* Ist ein Shift-/Reduce-Konflikt */
			fprintf( file, "\n\n\nShift-/Reduce-Konflikt\nHÅlle      : %ld\nProduktion : ", gra_str->hulle, hash_tab[term_str[gra_str->term].hash_tab_index].name );

			/* Ist es eine Alternative, oder eine Produktion ? */
			if( gra_str->prod1_alt >= 0 )
				/* Ja */
				output_text = "%s_%d -> ";

			else
				/* Nein */
				output_text = "%s -> ";

			/* Produktion ausgeben */
			fprintf( file, output_text, hash_tab[nterm_str[gra_str->prod1]->hash_tab_index].name, gra_str->prod1_alt );

			/* Rechte Seite ausgeben */
			output_right( file, gra_str->prod1 );

			/* Terminale ausgeben */
			for( j = 0; j < gra_str->term_count; j++ )
			{
				/* Terminal ausgeben */
				fprintf( file, "\nTerminal   : %s\t->\t", hash_tab[term_str[gra_str->term[j]].hash_tab_index].name );

				/* Schieben oder reduzieren ? */
				if( gra_str->state == REDUCE )
				{
					/* Reduzieren */
					lr_tab[gra_str->hulle][gra_str->term[j]].state = REDUCE;
					lr_tab[gra_str->hulle][gra_str->term[j]].value = gra_str->prod1;

					/* Meldung ausgeben */
					fprintf( file, "reduziere zu %s...", hash_tab[nterm_str[gra_str->prod1]->hash_tab_index].name );
				}

				else
					/* Reduzieren */
					fprintf( file, "schiebe %s...", hash_tab[term_str[gra_str->term[j]].hash_tab_index].name );
			}
		}

		else
		{
			/* ZÑhler hochsetzen */
			rr_err++;

			/* Ist ein Reduce-/Reduce-Konflikt */
			fprintf( file, "\n\n\nReduce-/Reduce-Konflikt\nHÅlle               : %ld\nAlte Produktion     : ", gra_str->hulle );

			/* Ist es eine Alternative, oder eine Produktion ? */
			if( gra_str->prod1_alt >= 0 )
				/* Ja */
				output_text = "%s_%d -> ";

			else
				/* Nein */
				output_text = "%s -> ";

			/* Produktion ausgeben */
			fprintf( file, output_text, hash_tab[nterm_str[gra_str->prod1]->hash_tab_index].name, gra_str->prod1_alt );

			/* Rechte Seite der alten Produktion ausgeben */
			output_right( file, gra_str->prod1 );

			/* Ist es eine Alternative, oder eine Produktion ? */
			if( gra_str->prod2_alt >= 0 )
				/* Ja */
				output_text = "\nKonflikt-Produktion : %s_%d -> ";

			else
				/* Nein */
				output_text = "\nKonflikt-Produktion : %s -> ";

			/* Produktion ausgeben */
			fprintf( file, output_text, hash_tab[nterm_str[gra_str->prod2]->hash_tab_index].name, gra_str->prod2_alt );

			/* Rechte Seite der alten Produktion ausgeben */
			output_right( file, gra_str->prod2 );

			/* Meldung ausgeben */
			fprintf( file, "\nReduziere mit alter Produktion..." );
		}

		/* Speicher freigeben */
		free( ( VOID * ) gra_err_str[i].term );
	}

	/* Speicher freigeben */
	free( ( VOID * ) gra_err_str );

	/* Variable zuruecksetzen */
	gra_err_str = 0L;
}