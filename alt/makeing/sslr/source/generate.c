/*
	Projekt		:	LR-Parser-Generator
	Modul		:	GENERATE.C
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

WORD insert_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos );
LONG find_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos );
VOID insert_reduce( LONG hulle, LONG term_index, LONG nterm_index );
WORD insert_error_index( HULLE_STR *hulle, LONG error_index );
WORD cmp_lr_tab( ULONG lr_tab1_index, ULONG lr_tab2_index );
WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD insert_hulle_hash( LONG elem, LONG hulle_nr );
VOID output_item( LONG nterm_index, WORD pkt_pos );
WORD insert_term( FIRST_STR *first, LONG term );
LONG get_nterm_first_index( LONG nterm_index );
WORD find_term( FIRST_STR *first, LONG term );
LONG cmp_hulle( LONG term, HULLE_STR *hulle );
LONG do_goto( HULLE_STR *hulle, LONG term );
WORD lalr_slr_hulle( HULLE_STR *hulle );
WORD lr_hulle( HULLE_STR *hulle );
WORD do_hulle( HULLE_STR *hulle );
LONG get_term_index( LONG term );
VOID set_empty_to_reduce( VOID );
WORD spead_lookaheads( VOID );
WORD append_lookahead( VOID );
WORD generate_follow( VOID );
WORD generate_first( VOID );
WORD generate_goto( VOID );
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

/* Beinhaltet die HULLE_STRs */
HULLE_STR	**hulle_str = 0L;

/* Anzahl der HULLE_STRs */
LONG		hulle_count = -1;

/* ZÑhlervariable */
LONG		lookahead_flg;



/***********/
/* Imports */
/***********/

MGLOBAL NTERM_FIRST	**nterm_first;
MGLOBAL ERROR_STR	**error_struct;
MGLOBAL NTERM_STR	**nterm_str;
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
MGLOBAL FILE		*file;



WORD generate_items( VOID )
{
	/* Hier werden die LR(0)(SLR)- bzw. LR(1)(LR/LALR)-Items erzeugt. */

	/* Die FIRST-Mengen bilden */
	if( generate_first() < 0 )
		/* Fehler */
		return( -1 );

	/* Die FOLLOW-Mengen bilden */
	if( generate_follow() < 0 )
		/* Fehler */
		return( -1 );

	/* HÅlle( I0 ) bilden */
	if( make_hulle0() < 0 )
		/* Fehler */
		return( -1 );

	/* Die Åbrigen HÅllen bilden */
	if( generate_goto() < 0 )
		/* Fehler */
		return( -1 );

	/* Soll ein LALR-Parser erzeugt werden ? */
	if( lr_info.type == LALR )
	{
		/* HÅllen anpassen */
		if( append_lookahead() < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Nun die Items fÅr die Fehlerbehandlung suchen */
	if( find_items() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD make_hulle0( VOID )
{
	/* Hier wird die erste HÅlle erzeugt. */

	/* Ein weiterer Eintrag */
	hulle_count++;

	/* Speicher fÅr neuen HULLE_STR-Eintrag holen */
	if( ( hulle_str = ( HULLE_STR ** ) realloc( hulle_str, sizeof( HULLE_STR * ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr HULLE_STR holen */
	if( ( hulle_str[0] = ( HULLE_STR * ) calloc( 1, sizeof( HULLE_STR ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Name der Produktion */
	hulle_str[0]->nterm = nterm_str[0]->hash_tab_index;

	/* Index der HÅlle */
	hulle_str[0]->index = 0;

	/* ACCEPT nur bei LR-Parser einfuegen */
	if( lr_info.type == LR )
	{
		/* ACCEPT an das Item anhÑngen */

		/* SLR/LALR -> Erstes Item in die HÅlle eintragen */
		if( insert_item( hulle_str[0], 0, accept, 0 ) < 0 )
			/* Fehler */
			return( -1 );
	}

	else
	{
		/* SLR/LALR -> Erstes Item in die HÅlle eintragen */
		if( insert_item( hulle_str[0], 0, 0, 0 ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Die HÅlle(I0) hat einen Kern */
	hulle_str[0]->anz_kitems = 1;

	/* Aus dem Kern die HÅlle bilden */
	if( do_hulle( hulle_str[0] ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD do_hulle( HULLE_STR *hulle )
{
	/*
		Hier werden alle Nichtterminale in die HÅlle aufgenommen, wenn
		der Punkt eines Kernes vor diesem steht.
	*/

	/* Soll ein LR-Parser erzeugt werden ? */
	if( lr_info.type == LR )
		/* Funktion HÅlle fÅr LR-Parser aufrufen */
		return( lr_hulle( hulle ) );

	else
		/* Ja, Funktion HÅlle fÅr SLR- bzw. LALR-Parser aufrufen */
		return( lalr_slr_hulle( hulle ) );
}


WORD lalr_slr_hulle( HULLE_STR *hulle )
{
	/* HÅllen-Funktion fÅr SLR- und LALR-Parser. */
	REG ULONG	i;
	ITEM_STR	*item;
	LONG		alt;

	/* Alle Items der HÅlle bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Ist der Punkt vor einem Nichtterminal ? */
		if( item->elem && !hash_tab[item->elem].term_str )
		{
			/* Alle Alternativen in die HÅlle eintragen */
			alt = hash_tab[item->elem].nterm_index;

			do
			{
				/* Item in HÅlle eintragen */
				if( insert_item( hulle, alt, 0, 0 ) < 0 )
					/* Fehler */
					return( -1 );

				/* Solange noch Alternativen da sind */
			}while( ( ++alt <= nterm_count ) && ( nterm_str[alt]->alt ) );
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD lr_hulle( HULLE_STR *hulle )
{
	/* HÅllen-Funktion fÅr LR-Parser. */
	NTERM_STR	*nterm;
	REG ULONG	i, j;
	ITEM_STR	*item;
	LONG		alt, *term = 0L, *term_2 = 0L, anz_elem, anz_elem_2;
	WORD		e_state;

	/* Alle Items der HÅlle bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Zeiger auf NTERM_STR des Items holen */
		nterm = nterm_str[item->nterm_index];

		/* Ist der Punkt vor einem Nichtterminal ? */
		if( item->elem && !hash_tab[item->elem].term_str )
		{
			/* Folgt dahinter etwas ? */
			if( ( nterm->anz_ri - 2 ) >= item->pkt_pos )
			{
				/* Ist es ein Terminal ? */
				if( hash_tab[nterm->ri[item->pkt_pos + 1]].term_str)
				{
					/* Ist ein Terminal */

					/* Ist es EPSILON ? */
					e_state = nterm->ri[item->pkt_pos + 1] == epsilon ? E_ON : E_OFF;

					/* Anzahl der Lookaheads = 1 */
					anz_elem = 1;

					/* Zeiger auf das Lookahead */
					term = &nterm->ri[item->pkt_pos + 1];
				}

				else
				{
					/* Ist ein Nichtterminal */

					/* Index in NTERM_FIRST-Struktur holen */
					j = get_nterm_first_index( hash_tab[nterm->ri[item->pkt_pos + 1]].nterm_index );

					/* Anzahl der zu kopierenden Lookaheads */
					anz_elem = nterm_first[j]->first.anz_elem;

					/* Zeiger auf die Lookaheads */
					term = nterm_first[j]->first.term_arr;

					/* Ist EPSILON dabei ? */
					e_state = nterm_first[j]->first.epsilon == E_OFF ? E_OFF : E_ON;
				}
			}

			/* Folgte kein Element bzw. war es EPSILON ? */
			if(	( ( ( item->pkt_pos + 2 ) > nterm->anz_ri ) || ( e_state == E_ON ) ) )
			{
				/* Anzahl der zu kopierenden Lookaheads */
				anz_elem_2 = item->lookahead.anz_elem;

				/* Zeiger auf die Lookaheads */
				term_2 = item->lookahead.term_arr;
			}

			/* Alle Alternativen in die HÅlle eintragen */
			alt = hash_tab[item->elem].nterm_index;

			do
			{
				/* Ist etwas da ? */
				if( term )
				{
					/* Alle Lookaheads an das Item anhÑngen */
					for( j = 0; j < anz_elem; j++ )
					{
						/* Item in HÅlle eintragen */
						if( insert_item( hulle, alt, term[j], 0 ) < 0 )
							/* Fehler */
							return( -1 );
					}
				}

				/* Muss nochmehr uebertragen werden ? */
				if( term_2 )
				{
					/* Ja, alle Lookaheads an das Item anhÑngen */
					for( j = 0; j < anz_elem_2; j++ )
					{
						/* Item in HÅlle eintragen */
						if( insert_item( hulle, alt, term_2[j], 0 ) < 0 )
							/* Fehler */
							return( -1 );
					}
				}

				/* Solange noch Alternativen da sind */
			}while( ( ++alt <= nterm_count ) && ( nterm_str[alt]->alt ) );

			/* Zuruecksetzen */
			term = 0L;
			term_2 = 0L;
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD append_lookahead( VOID )
{
	/*
		In dieser Routine werden die Lookaheads an die LR(0)-Menge von
		Elementen angehÑngt.
	*/
	HULLE_STR	hulle = { 0 };
	ITEM_STR	*item, *kern, *goto_kern;
	REG ULONG	i, j, k, l, o, m;

	/* Meldung ausgeben */
	fprintf( stderr, "\nErzeuge Lookaheads..." );

	/* ACCEPT ist erstes Lookahead im ersten Item */
	if( insert_term( &hulle_str[0]->item_str[0].lookahead, accept ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alle Kerne */
	for( i = 0; i < hulle_str[0]->anz_kitems; i++ )
	{
		/* Zeiger auf Kern holen */
		kern = &hulle_str[0]->item_str[i];

		/* Ist der Punkt vor einem Nichtterminal ? */
		if( ( nterm_str[kern->nterm_index]->anz_ri > kern->pkt_pos ) && ( !hash_tab[kern->elem].term_str ) )
		{
			/* Hat der Kern Lookaheads ? */
			if( kern->lookahead.anz_elem )
			{
				/*
					Den Kern sowie seine Lookaheads in die separate HÅlle
					einfÅgen.
				*/
				for( m = 0; m < kern->lookahead.anz_elem; m++ )
				{
					/* Kern in separate HÅlle einfuegen */
					if( insert_item( &hulle, kern->nterm_index, kern->lookahead.term_arr[m], kern->pkt_pos ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}

			else
			{
				/* Kern in separate HÅlle einfÅgen */
				if( insert_item( &hulle, kern->nterm_index, 0, kern->pkt_pos ) < 0 )
					/* Fehler */
					return( -1 );
			}

			/* Die Lookaheads behachten */
			lr_info.type = LR;

			/* Die dazugehîrige HÅlle bilden */
			if( do_hulle( &hulle ) < 0 )
				/* Fehler */
				return( -1 );
	
			/* LALR-Parser */
			lr_info.type = LALR;

			/* Anzahl der Kerne merken */
			hulle.anz_kitems = hulle_str[0]->anz_kitems;

			/*****************************/
			/* Protokollmeldung ausgeben */
			/*****************************/
			#ifdef PROT

				/* Meldung ausgeben */
				fprintf( file, "\nErzeuge aus Item " );

				/* Item ausgeben */
				output_item( kern->nterm_index, kern->pkt_pos );

				/* Meldung ausgeben */
				fprintf( file, " die separate HÅlle.\n" );

			#endif

			/* Nicht die Kerne in der separaten HÅlle */
			for( j = hulle.anz_kitems; j < hulle.anz_items; j++ )
			{
				/* Zeiger auf Kern holen */
				item = &hulle.item_str[j];

				/* Alle HÅllen */
				for( k = 0; k <= hulle_count; k++ )
				{
					/* Alle Kerne */
					for( l = 0; l < hulle_str[k]->anz_kitems; l++ )
					{
						/* Zeiger auf Item holen */
						goto_kern = &hulle_str[k]->item_str[l];

						/* Ist es der gesuchte Kern ? */
						if( ( goto_kern->nterm_index == item->nterm_index ) && ( ( item->pkt_pos + 1 ) == goto_kern->pkt_pos ) )
						{
							/* Alle Lookaheads, ausser die des Kerns Åbertragen. */

							/*****************************/
							/* Protokollmeldung ausgeben */
							/*****************************/
							#ifdef PROT

								/* Meldung ausgeben */
								fprintf( file, "\n\nSeparate HÅlle: " );

								/* Item ausgeben */
								output_item( item->nterm_index, item->pkt_pos );

								/* Freie Zeile */
								fprintf( file, "\nHÅlle %lu: ", k );

								/* Item ausgeben */
								output_item( goto_kern->nterm_index, goto_kern->pkt_pos );

								/* Meldung ausgeben */
								fprintf( file, "\n\nDie Lookaheads des Items\n" );

								/* Item ausgeben */
								output_item( item->nterm_index, item->pkt_pos );

								/* Freie Zeile */
								fprintf( file, "\n" );

								/* Terminale ausgeben */
								for( o = 0; o < item->term.anz_elem; o++ )
									/* Terminal ausgeben */
									fprintf( file, "%s\n", hash_tab[item->term.term_arr[o]].name );
	
								/* Freie Zeile */
								fprintf( file, "\n" );

							#endif

							for( o = 0; o < item->lookahead.anz_elem; o++ )
							{
								/*****************************/
								/* Protokollmeldung ausgeben */
								/*****************************/
								#ifdef PROT

									/* Meldung ausgeben */
									fprintf( file, "\nSuche %s in\n", hash_tab[item->term.term_arr[o]].name );

									/* Item ausgeben */
									output_item( kern->nterm_index, kern->pkt_pos );

									/* Freie Zeile */
									fprintf( file, "\n" );

								#endif

								/* Kern aus Kern-Element ? */
								if( !find_term( &kern->lookahead, item->lookahead.term_arr[o] ) )
								{
									/*****************************/
									/* Protokollmeldung ausgeben */
									/*****************************/
									#ifdef PROT

										/* Meldung ausgeben */
										fprintf( file, "\nNicht gefunden, trage %s in\n", hash_tab[item->term.term_arr[o]].name );

										/* Item ausgeben */
										output_item( goto_kern->nterm_index, goto_kern->pkt_pos );

										/* Freie Zeile */
										fprintf( file, " ein.\n" );

									#endif

									/* Element uebertragen */
									if( insert_term( &goto_kern->lookahead, item->lookahead.term_arr[o] ) < 0 )
										/* Fehler */
										return( -1 );
								}
							}
						}
					}
				}
			}
		}
	}

	/* Die Lookaheads behachten */
	lr_info.type = LR;

	/* Die dazugehîrige HÅlle bilden */
	if( do_hulle( hulle_str[0] ) < 0 )
		/* Fehler */
		return( -1 );

	/* LALR-Parser */
	lr_info.type = LALR;

	/* Lookaheads verbreiten */
	if( spead_lookaheads() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD spead_lookaheads( VOID )
{
	/* Hier verbreiten sich die Lookaheads auf die Kerne. */
	NTERM_STR	*nterm;
	ITEM_STR	*item, *sep_hulle_kern;
	REG ULONG	i, j, l, o;
	LONG		temp_lookahead_flg;

	/* Flag zuruecksetzen */
	lookahead_flg = 0;

	do
	{
		/* Den Wert merken */
		temp_lookahead_flg = lookahead_flg;
	
		/* Lookaheads ausbreiten */
		for( i = 0; i <= hulle_count; i++ )
		{
			/* Die Lookaheads behachten */
			lr_info.type = LR;

			/* Die dazugehîrige HÅlle bilden */
			if( do_hulle( hulle_str[i] ) < 0 )
				/* Fehler */
				return( -1 );

			/* LALR-Parser */
			lr_info.type = LALR;

			/* Alle Kerne */
			for( j = 0; j < hulle_str[i]->anz_items; j++ )
			{
				/* Zeiger auf Items holen */
				item = &hulle_str[i]->item_str[j];

				/* Zeiger auf NTERM_STR */
				nterm = nterm_str[item->nterm_index];

				/* Folgt hinter dem Punkt noch ein Element ? */
				if( ( nterm->anz_ri > item->pkt_pos ) && ( item->elem != epsilon ) )
				{
					/* Alle Kerne */
					for( l = 0; l < hulle_str[item->next_state]->anz_kitems; l++ )
					{
						/* Zeiger auf Kern */
						sep_hulle_kern = &hulle_str[item->next_state]->item_str[l];
	
						/* Ist es das gesuchte Kern ? */
						if( ( item->nterm_index == sep_hulle_kern->nterm_index ) && ( item->pkt_pos == ( sep_hulle_kern->pkt_pos - 1 ) ) )
						{
							/* Ja, Lookaheads uebertragen */
							for( o = 0; o < item->lookahead.anz_elem; o++ )
							{
								/* Eintragen */
								if( insert_term( &sep_hulle_kern->lookahead, item->lookahead.term_arr[o] ) < 0 )
									/* Fehler */
									return( -1 );
							}
	
							/*****************************/
							/* Protokollmeldung ausgeben */
							/*****************************/
							#ifdef PROT
	
								/* Freie Zeile ausgeben */
								fprintf( file, "\n\nHÅlle %lu\n", i );
	
								/* Item ausgeben */
								output_item( item->nterm_index, item->pkt_pos );
	
								/* Freie Zeile */
								fprintf( file, "\nHÅlle %lu\n", item->next_state );
	
								/* Item ausgeben */
								output_item( sep_hulle_kern->nterm_index, sep_hulle_kern->pkt_pos );
	
								/* Meldung ausgeben */
								fprintf( file, "\n\nGebe alle Items aus." );
	
								/* Terminale ausgeben */
								for( o = 0; o < sep_hulle_kern->term.anz_elem; o++ )
									/* Terminal ausgeben */
									fprintf( file, "\n%s", hash_tab[sep_hulle_kern->term.term_arr[o]].name );
	
							#endif
						}
					}
				}
			}
		}

		/* Solange Lookaheads Åbertragen wurden. */

	}while( temp_lookahead_flg != lookahead_flg );

	/* Alles OK */
	return( 0 );
}


WORD insert_term( FIRST_STR *first, LONG term )
{
	/* Hier wird ein Terminal in die FIRST_STR eingetragen. */
	REG ULONG	i;

	/* Ist das Terminal schon eingetragen worden ? */
	for( i = 0; i < first->anz_elem; i++ )
	{
		/* Ist es das einzutragende Terminal ? */
		if( first->term_arr[i] == term )
			/* Terminal ist schon eingetragen worden */
			return( 0 );
	}

	/* Speicher fÅr Terminal anfordern */
	if( ( first->term_arr = ( LONG * ) realloc( ( VOID * ) first->term_arr, ( first->anz_elem + 1 ) * sizeof( LONG ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Terminal in term_arr eintragen */
	first->term_arr[first->anz_elem++] = term;

	/* Es wurde ein Terminal eingetragen */
	lookahead_flg++;

	/* Alles OK */
	return( 0 );
}


WORD find_term( FIRST_STR *first, LONG term )
{
	/* Hier wird das Terminal term in first gesucht. */
	REG ULONG	i;

	/* Ist das Terminal in first ? */
	for( i = 0; i < first->anz_elem; i++ )
	{
		/* Ist es das einzutragende Terminal ? */
		if( first->term_arr[i] == term )
			/* Terminal ist in first */
			return( 1 );
	}

	/* Terminal ist nicht in first */
	return( 0 );
}


LONG find_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos )
{
	/* Hier wird geprueft, ob das Item in der HÅlle ist. */
	ITEM_STR	*item;
	REG ULONG	i, j;

	/* Alle Items der HÅlle */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Ist es das gesuchte Item ? */
		if( ( item->nterm_index == nterm_index ) && ( item->pkt_pos == pkt_pos ) )
		{
			/* LR-Parser ? */
			if( lr_info.type == LR )
			{
				/* LR-Parser */

				/* Alle Lookaheads des Items mit term vergleichen */
				for( j = 0; j < item->lookahead.anz_elem; j++ )
				{
					/* Sind die Terminale gleich ? */
					if( item->lookahead.term_arr[j] == term )
						/* Das Item ist schon in der HÅlle */
						return( -2 );
				}

				/* Das Item ist da, aber das Terminal stimmt nicht */
				return( i );
			}

			else
				/* Item ist schon in der HÅlle */
				return( -2 );
		}
	}

	/* Item war nicht in der HÅlle enthalten */
	return( -1 );
}


WORD insert_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos )
{
	/* Hier wird ein Item in die HÅlle eingetragen. */
	REG ULONG	anz_items = hulle->anz_items;
	ITEM_STR	*item;
	LONG		i;

	/* Testen, ob das Item bereits in der HÅlle eingetragen ist */
	i = find_item( hulle, nterm_index, term, pkt_pos );

	/* War das Item in der HÅlle ? */
	if( ( i >= 0 ) && ( lr_info.type == LR ) && ( term != epsilon ) )
	{
		/* Zeiger auf Item holen */
		item = &hulle->item_str[i];

		/* Das Item ja, aber ohne term */

		/* Speicher fÅr das Terminal anfordern */
		if( ( item->lookahead.term_arr = ( LONG * ) realloc( ( VOID * ) item->lookahead.term_arr, ( item->lookahead.anz_elem + 1 ) * sizeof( LONG ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Terminal in term_arr eintragen */
		item->lookahead.term_arr[item->lookahead.anz_elem++] = term;
	}

	/* Es muss ein ganz neues Item erzeugt werden */
	else if( i == -1 )
	{
		/* Nein, ein ganz neues Item erzeugen */

		/* Speicher fÅr neuen ITEM_STR-Eintrag holen */
		if( ( hulle->item_str = ( ITEM_STR * ) realloc( ( VOID * ) hulle->item_str, ( anz_items + 1 ) * sizeof( ITEM_STR ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Zeiger auf Item holen */
		item = &hulle->item_str[anz_items];
		
		/* Struktur initialisieren */
		memset( ( VOID * ) item, 0, sizeof( ITEM_STR ) );

		/* Terminal nur bei LR-Parser einfuegen */
		if( term && ( term != epsilon ) )
		{
			/* Terminal bei LR einfuegen */

			/* Speicher fÅr das Terminal anfordern */
			if( ( item->lookahead.term_arr = ( LONG * ) realloc( ( VOID * ) item->lookahead.term_arr, ( item->lookahead.anz_elem + 1 ) * sizeof( LONG ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Terminal in term_arr eintragen */
			item->lookahead.term_arr[item->lookahead.anz_elem++] = term;
		}

		/* Eintrag des Nichtterminals in der NTERM_STR */
		item->nterm_index = nterm_index;

		/* Position des Punktes */
		item->pkt_pos = pkt_pos;

		/* Das Element vor dem Punkt eintragen */
		item->elem = nterm_str[nterm_index]->ri[pkt_pos];

		/* Ein weiteres Item */
		hulle->anz_items++;
	}

	/* Alles OK */
	return( 0 );
}


WORD generate_goto( VOID )
{
	/* Hier werden die HÅllen durch GOTO erzeugt. */
	NTERM_STR	*nterm;
	ITEM_STR	*item, *item_str;
	LONG		i, k, temp_hulle_count, elem, h;

	/* Meldung ausgeben */
	fprintf( stderr, "\nErzeuge die HÅllen..." );

	/* Meldung ausgeben ? */
	if( !lr_info.analyze )
	{
		/* SLR-Parser ? */
		if( lr_info.type == SLR )
			/* Meldung ausgeben */
			fprintf( file, "\n\n***********************\n"
						   "* Erzeuge LR(0)-Items *\n"
						   "***********************\n\n" );

		else
			/* Meldung ausgeben */
			fprintf( file, "\n\n***********************\n"
						   "* Erzeuge LR(1)-Items *\n"
						   "***********************\n\n" );
	}

	/* Alle HÅllen bearbeiten */
	for( h = 0; h <= hulle_count; h++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item_str = hulle_str[h]->item_str;

		/* Alle Items durchgehen */
		for( i = 0; i < hulle_str[h]->anz_items; i++ )
		{
			/* Zeiger auf das Item holen */
			item = &item_str[i];

			/* Zeiger auf NTERM_STR des Items holen */
			nterm = nterm_str[item->nterm_index];

			/* Ist es ein Reduce-Item oder ist der Punkt vor EPSILON ? */
			if( ( nterm->anz_ri > item->pkt_pos ) && ( item->elem != epsilon ) )
			{
				/* Nein, die Anzahl der HÅllen merken */
				temp_hulle_count = hulle_count;

				/* Die HÅlle ggf. erzeugen */
				if( ( elem = do_goto( hulle_str[h], item->elem ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Wurde eine HÅlle erzeugt ? */
				if( temp_hulle_count < hulle_count )
				{
					/* Ja, Meldung ausgeben ? */
					if( !lr_info.analyze )
						/* Meldung ausgeben */
						fprintf( file, "\n%3lu = Erzeugte HÅlle( I%3lu, %35s )", hulle_count, h, hash_tab[item->elem].name );

					/* Nichtterminal merken */
					hulle_str[hulle_count]->nterm = item->elem;

					/* Index der HÅlle merken */
					hulle_str[hulle_count]->index = h;

					/* Die neue Nummer der HÅlle in die Hash-Tab eintragen */
					if( insert_hulle_hash( item->elem, hulle_count ) < 0 )
						/* Fehler */
						return( -1 );

					/* Alle Items mit dem Punkt vor elem */
					for( k = i; ( k < hulle_str[h]->anz_items ) && ( item_str[k].elem == item->elem ); k++ )
						/* Den Folgezustand des Items merken */
						item_str[k].next_state = hulle_count;
/*
					/* i anpassen */
					i = k - 1;
*/
				}

				else
					/* HÅlle wurde schon erzeugt */
					item->next_state = elem;
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_hulle_hash( LONG elem, LONG hulle_nr )
{
	/* Hier wird die Nummer einer HÅlle in die Hash-Tab eingetragen. */
	HASH_TAB	*hash_str = &hash_tab[elem];
	REG LONG	i, *hulle_arr;

	/* Zeiger auf die HÅllen holen */
	hulle_arr = hash_str->hulle_arr;

	/* Alle HÅllen-Nummern */
	for( i = 0; i < hash_str->hulle_arr_count; i++ )
	{
		/* Ist die HÅllen-Nummer schon eingetragen worden ? */
		if( hulle_arr[i] == hulle_nr )
			/* Ja */
			return( 0 );
	}

	/* Speicher holen */
	if( ( hulle_arr = ( LONG * ) realloc( hulle_arr, ( hash_str->hulle_arr_count++ + 1 ) * sizeof( LONG ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* hulle_nr in hash_tab eintragen */
	hulle_arr[i] = hulle_nr;

	/* énderungen eintragen */
	hash_str->hulle_arr = hulle_arr;

	/* Alles Ok */
	return( 0 );
}


LONG do_goto( HULLE_STR *hulle, LONG term )
{
	/* Hier wird ggf. eine Neue HÅlle erzeugt. */
	HULLE_STR	*new_hulle;
	ITEM_STR	*item;
	REG ULONG	i, j;
	LONG		h_index;

	/* Ist es ACCEPT ? */
	if( term == accept )
		/* Ja, zurueck */
		return( 0 );

	/* Speicher fÅr die neue HÅlle holen */
	if( ( new_hulle = ( HULLE_STR * ) calloc( 1, sizeof( HULLE_STR ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alle Items der HÅlle bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Ist der Punkt vor term ? */
		if( item->elem == term )
		{
			/* Ist es fÅr einen SLR- oder LALR-Parser ? */
			if( lr_info.type != LR )
			{
				/* Item in HÅlle eintragen */
				if( insert_item( new_hulle, item->nterm_index, 0, item->pkt_pos + 1 ) < 0 )
					/* Fehler */
					return( -1 );
			}

			else
			{
				/* Alle Lookaheads an das Item anhÑngen */
				for( j = 0; j < item->lookahead.anz_elem; j++ )
				{
					/* Item in HÅlle eintragen */
					if( insert_item( new_hulle, item->nterm_index, item->lookahead.term_arr[j], item->pkt_pos + 1 ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}
		}
	}

	/* Ist ueberhaupt eine HÅlle erzeugt worden ? */
	if( new_hulle->anz_items )
	{
		/* Anzahl der Kerne merken */
		new_hulle->anz_kitems = new_hulle->anz_items;

		/* Existiert die HÅlle bereits ? */
		if( ( h_index = cmp_hulle( term, new_hulle ) ) == 0L )
		{
			/* Nein, HÅlle in hulle_str aufnehmen */

			/* Speicher fÅr neuen HULLE_STR-Eintrag holen */
			if( ( hulle_str = ( HULLE_STR ** ) realloc( hulle_str, ( ++hulle_count + 1 ) * sizeof( HULLE_STR * ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* HÅlle eintragen */
			hulle_str[hulle_count] = new_hulle;
		}

		else
		{
			/* Speicher der Items freigeben */
			for( i = 0; i < new_hulle->anz_items; i++ )
			{
				/* Speicher fÅr Lookaheads freigeben ? */
				if( &new_hulle->item_str[i].lookahead )
					/* Speicher der Lookaheads freigeben */
					free( ( VOID * ) &new_hulle->item_str[i].lookahead.term_arr );

				/* Speicher des Items freigeben */
				free( ( VOID * ) &new_hulle->item_str[i] );
			}

			/* Die HÅlle existierte bereits */
			free( ( VOID * ) new_hulle );

			/* Es gab diese HÅlle schon */
			return( h_index );
		}

		/* Aus den Kernen die HÅlle bilden */
		if( do_hulle( new_hulle ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Alles OK */
	return( 0 );
}


LONG cmp_hulle( LONG term, HULLE_STR *hulle )
{
	/* Hier wird geprueft, ob es die HÅlle hulle schon gibt. */
	REG LONG	i, j, k;
	LONG		kitem_count, *hulle_arr;

	/* Zeiger auf die zu vergleichenden HÅllen holen */
	hulle_arr = hash_tab[term].hulle_arr;

	/* Die HÅllen vergleichen */
	for( i = 0; i < hash_tab[term].hulle_arr_count; i++ )
	{
		/* Ist die Anzahl der Kerne gleich ? */
		if( hulle_str[hulle_arr[i]]->anz_kitems == hulle->anz_kitems )
		{
			/* ZÑhler fÅr die Kerne zuruecksetzen */
			kitem_count = 0;

			/* Alle Kerne pruefen */
			for( j = 0; j < hulle->anz_kitems; j++ )
			{
				/* Ist es fÅr ein SLR- oder LALR-Parser ? */
				if( lr_info.type != LR )
				{
					/* Ist der Kern in beiden HÅllen vorhanden ? */
					if( find_item( hulle_str[hulle_arr[i]], hulle->item_str[j].nterm_index, 0, hulle->item_str[j].pkt_pos ) == -2 )
						/* Der Kern war in beiden HÅllen vorhanden */
						kitem_count++;
				}

				else
				{
					/* Alle Lookaheads des Kerns */
					for( k = 0; k < hulle->item_str[j].lookahead.anz_elem; k++ )
					{
						/* Ist der Kern in beiden HÅllen vorhanden ? */
						if( find_item( hulle_str[hulle_arr[i]], hulle->item_str[j].nterm_index, hulle->item_str[j].lookahead.term_arr[k], hulle->item_str[j].pkt_pos ) != -2 )
							/* Der Kern war nicht in der HÅlle vorhanden */
							break;
					}

					/* Waren alle Lookaheads gleich ? */
					if( k == hulle->item_str[j].lookahead.anz_elem )
						/* Ja */
						kitem_count++;
				}
			}

			/* Waren alle Kerne gleich ? */
			if( hulle->anz_kitems == kitem_count )
				/* Ja, den Index zurueckliefern */
				return( hulle_arr[i] );
		}
	}

	/* HÅllen sind nicht gleich */
	return( 0 );
}


WORD find_items( VOID )
{
	/* Hier werden die ERROR_STRs ausgewertet. */
	REG ULONG	i, j, k;
	ITEM_STR	*item;

	/* Alle ERROR_STRs */
	for( i = 0; i < error_struct_count; i++ )
	{
		/* Alle HÅllen */
		for( j = 0; j <= hulle_count; j++ )
		{
			/* Alle Items */
			for( k = 0; k < hulle_str[j]->anz_items; k++ )
			{
				/* Zeiger auf Item holen */
				item = &hulle_str[j]->item_str[k];

				/* nterm_index des Items mit dem der ERROR_STR vergleichen */
				if( ( item->nterm_index == error_struct[i]->nterm_index ) && ( item->pkt_pos == error_struct[i]->pkt_pos ) )
				{
					/* Item ist in dieser HÅlle */
					if( insert_error_index( hulle_str[j], i + 1 ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_error_index( HULLE_STR *hulle, LONG error_index )
{
	/* Hier wird ein error_index in die HULLE_STR eingetragen. */
	REG ULONG	i;

	/* Alle bisherigen EintrÑge prÅfen */
	for( i = 0; i < hulle->anz_error_index; i++ )
	{
		/* Ist es der gleiche ? */
		if( hulle->error_index[i] == error_index )
			/* Ist schon eingetragen */
			return( 0 );
	}

	/* Speicher anfordern */
	if( ( hulle->error_index = ( LONG * ) realloc( ( VOID * ) hulle->error_index, sizeof( LONG ) * ( hulle->anz_error_index + 1 ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* error_index eintragen */
	hulle->error_index[hulle->anz_error_index++] = error_index;

	/* Alles OK */
	return( 0 );
}