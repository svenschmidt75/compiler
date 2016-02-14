/*
	Projekt		:	Scanner-Generator
	Modul		:	MIN_DFA.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	30.03.1998
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/





/************/
/* Includes */
/************/

/* Include-Datei des Scanner-Generators */
#include "sslex.h"
#include "sslexpar.h"



/*****************/
/* Deklarationen */
/*****************/

/* Debug-Modus */
#define DUMP


/* Beinhaltet die Zustandsmengen einer Teilgruppe */
struct ugroup
{
	/* Indizies in DSTATES */
	WORD			*ind;

	/* Gruppen Indizies */
	WORD			*grp_ind;

	/* FÅr die Trennung der ZustÑnde bei diversen öbergÑngen */
	WORD			*cpy_ind;

	/* Anzahl der Elemente in ind, grp_ind und cpy_ind */
	WORD			count;

};


/* Beinhaltet die Teilgruppen */
typedef struct
{
	/* Untergruppen */
	struct ugroup	*ugroup;

	/* Anzahl der Unter-Gruppen in dieser Gruppe */
	WORD			count;

}GRP;


/* Struktur fÅr die Anpassung der DSTATES-Struktur nach der Zustandsminimierung */
typedef struct
{
	/* Alter Index in DSTATES */
	WORD	old;

	/* Neuner Index in DSTATES */
	WORD	new;

}D_IND_STR;


/****************/
/* Definitionen */
/****************/


/* Alte Gruppe */
GRP			ogrp = { 0L, 0 };

/* Neue Gruppe */
GRP			ngrp = { 0L, 0 };


/* DSTATES-Indizes nach der Zustandsminimierung */
D_IND_STR	*d_ind;

/* Anzahl der Elemente in d_ind */
WORD		d_ind_count = 0;




/**************/
/* Prototypen */
/**************/

WORD insert_ugroup( GRP *grp, WORD grp_index, WORD dstates_index );
WORD bra_d( LONG state, LONG nea_ind, NEA_STATE *nea_st );
WORD get_dstate_index( WORD dstates_index, UBYTE c );
WORD copy_grp ( GRP *from, GRP *to, WORD ugrp_ind );
WORD sep_grp( GRP *to, WORD dstates_index );
WORD cmp_grp( GRP *grp1, GRP *grp2 );
VOID output_one_state( LONG st );
WORD create_group( GRP *grp );
WORD copy_all_grp ( VOID );
WORD sep_old_group( VOID );
WORD delete_states( VOID );
VOID output_ogroup( VOID );
WORD set_dstates( VOID );


/***********/
/* Imports */
/***********/

MGLOBAL NEA_STATE	*nea_state;
MGLOBAL NEA_STR		*nea_str;
MGLOBAL CCL_STR		all_bchar;
MGLOBAL DSTATES		*dstates;
MGLOBAL ULONG		nea_count;
MGLOBAL WORD		dstates_count;
MGLOBAL WORD		*state_arr;
MGLOBAL WORD		all_char_count;
MGLOBAL BYTE		all_char[];




WORD gen_d( SS_STR *ss )
{
	/*
		Hier wird ein neuer Zustand d erzeugt.
		Jeder Zustand im NEA nea_all/nea_nall hat ein öbergang zu d mit den Zeichen der Eingabemenge, von denen noch
		kein öbergang hinausgeht.
		Alle erlaubten Zeichen sind in der CCL all_chr enthalten.
	*/
	REG WORD	i, j;
	NEA_STR		*nea;
	LONG		state;
	UBYTE		ccl[CCL_WIDTH];

	/* ccl initialisieren */
	memset( ( VOID * ) ccl, 0xffff, CCL_WIDTH );

	/* Einen neuen Zustand d einfÅhren */
	if( ( state = new_nea_state( ss->nea_all ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* state muû fÅr alle Eingaben eien öbergang zu sich selbst haben */
	if( nea_bra( ss->nea_all, state, state, ccl, 0, 0, 0 ) < 0 )
		/* Fehler */
		return( -1 );
	
	/* Alle NEAen in der SC */
	for( i = 0; i < ss->ss_nea_count; i++ )
	{
		/* Zeiger auf den NEA holen */
		nea = &nea_str[ss->ss_nea[i]];

		/* Alle ZustÑnde des NEAs */
		for( j = 0; j < nea->state_arr_count; j++ )
		{
			/* Alle ZustandsÅbergÑnge prÅfen */
			if( bra_d( state, ss->ss_nea[i], &nea_state[nea->state_arr[j]] ) < 0 )
				/* Fehler */
				return( -1 );
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD bra_d( LONG state, LONG nea_ind, NEA_STATE *nea_st )
{
	/* Hier wird ein öbergang von allen in bra_str enthaltenen ZustÑnde zu state erzeugt. */
	REG WORD	i, j;
	CCL_STR		ccl_str;
	WORD		count = nea_st->bra_count;

	/* ccl initialisieren */
	memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );

	/* Alle öbergÑnge */
	for( j = 0; j < count; j++ )
	{
		/* Die ccl mit der ccl des Zustandes xodern */
		for( i = 0; i < CCL_WIDTH; i++ )
		{
			/* Schon vorhandene ZustÑnde kopieren */
			ccl_str.ccl[i] = nea_st->bra_str[j].ccl_str.ccl[i];

			/* Nun die vorhandenen ausblenden */
			ccl_str.ccl[i] ^= all_bchar.ccl[i];

			/* Nur die erlaubten der Åbrigen */
			ccl_str.ccl[i] &= all_bchar.ccl[i];
		}

		/* öbergang zu state(Zustand d) erzeugen */
		if( nea_bra( nea_ind, nea_st->state, state, ccl_str.ccl, 0, 0, 0 ) < 0 )
			/* Fehler */
			return( -1 );

		/* ccl zurÅcksetzen */
		memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );
	}

	/* Alles OK */
	return( 0 );
}


VOID output_one_state( LONG st )
{
	/* Hier wird ein NEA ausgegeben */
	NEA_STATE	*state = &nea_state[st];
	WORD		i, j, p, k;

	/* Main-NEA ausgeben */
	fprintf( stderr, "\nZustand : %ld\n", st );

	for( j = 0; j < state->bra_count; j++ )
	{
		fprintf( stderr, "\nVon %lu nach %lu mit EPSILON: %d ", state->state, state->bra_str[j].bra, state->bra_str[j].eps );

		for( p = k = 0; p < all_char_count; p++, k++ )
		{
			if( state->bra_str[j].ccl_str.ccl[all_char[p] / CHAR_BIT] & ( 1 << ( all_char[p] % CHAR_BIT ) ) )
				fprintf( stderr, " %c,", ( BYTE ) all_char[p] );

			if( k / 60 )
			{
				k = 0;
				fprintf( stderr, "\n" );
			}
		}
	}

	fprintf( stderr, "\n\n" );
}


WORD min_dfa( VOID )
{
	/* Hier wird die Zustandsmenge eines DEA minimiert. */
	REG WORD	i, j;
	WORD		na, a;

	/*
		Zwei Zustandsmengen bilden:

		1. Menge der akzeptierenden ZustÑnde,
		2. Menge der nicht-akzeptierenden ZustÑnde.
	*/

	/* Eine Gruppe fÅr die akzeptierenden ZustÑnde */
	if( ( a = create_group( &ogrp ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Eine Gruppe fÅr die nicht-akzeptierenden ZustÑnde */
	if( ( na = create_group( &ogrp ) ) < 0 )
		/* Fehler */
		return( -1 );

	for( i = 0; i < dstates_count; i++ )
	{
#ifdef DUMP
		WORD	j;

		/* Alle Zustandsmengen in DSTATES ausgeben */
		fprintf( stderr, "\nDSTATES[%d] = ", i );

		for( j = 0; j < nea_count; j++ )
		{
			if( dstates[i].states[j / CHAR_BIT] & ( 1 << ( j % CHAR_BIT ) ) )
				fprintf( stderr, "%d, ", j );
		}
#endif

		/* Akzeptierende Zustandsmenge ? */
		if( dstates[i].type == FINIT )
		{
			dstates[i].group = a;

			/* Ja, in ogrp eintragen */
			if( insert_ugroup( &ogrp, a, i ) < 0 )
				/* Fehler */
				return( -1 );
		}

		else
		{
			dstates[i].group = na;

			/* Nein, in ngrp eintragen */
			if( insert_ugroup( &ogrp, na, i ) < 0 )
				/* Fehler */
				return( -1 );
		}
	}

	/* Solange, bis keine neuen Teilgruppen mehr entstehen */
	do
	{
		/*
			In ogrp sind die "alten" Teilgruppen.
			Nach Aufrufen von set_old_group sind in ngrp die neuen Teilgruppen.
			Sind ngrp und ogrp gleich, geht es weiter.
		*/

#ifdef DUMP
		fprintf( stderr, "\n\n\n\n%d Teilgruppen in ogrp.", ogrp.count );

		for( i = 0; i < ogrp.count; i++ )
		{
			fprintf( stderr, "\n\nTeilgruppen %d hat %d EintrÑge.", i, ogrp.ugroup[i].count );

			for( j = 0; j < ogrp.ugroup[i].count; j++ )
			{
				fprintf( stderr, "\nogrp[%d][%d] = DSTATES[%d].", i, j, ogrp.ugroup[i].ind[j] );
			}
		}
#endif

		/* ogrp aufspalten */
		sep_old_group();

		if( cmp_grp( &ngrp, &ogrp ) < 0 )
		{
			/* Beide Gruppen sind nicht gleich, ngrp nach ogrp */
			if( copy_all_grp() < 0 )
				/* Fehler */
				return( -1 );
		}

		else
			/* Beide Gruppen sind gleich */
			break;

	}while( 1 );

#ifdef DUMP
		fprintf( stderr, "\n\n\n\n%d Teilgruppen in ogrp.", ogrp.count );

		for( i = 0; i < ogrp.count; i++ )
		{
			fprintf( stderr, "\n\nTeilgruppen %d hat %d EintrÑge.", i, ogrp.ugroup[i].count );

			for( j = 0; j < ogrp.ugroup[i].count; j++ )
			{
				fprintf( stderr, "\nogrp[%d][%d] = DSTATES[%d].", i, j, ogrp.ugroup[i].ind[j] );
			}
		}

		output_ogroup();

#endif

	/* DSTATES anpassen */
	if( set_dstates() < 0 )
		/* Fehler */
		return( -1 );

	/* UnnÅtze ZustÑnde lîschen */
	if( delete_states() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD sep_old_group( VOID )
{
	/* Hier wird die alte Gruppe von Zustandsmengen ggf. aufgeteilt und in die neue Gruppe eingetragen. */
	REG WORD		i, j, k;
	WORD			tmp, temp, ind;

	/* Alle Teilguppen in ogrp */
	for( i = 0; i < ogrp.count; i++ )
	{
		fprintf( stderr, "\n\n\n\nTeilgruppe %d aus ogrp.", i );

		/* Eine neue Gruppe fÅr die Zustandsmenge */
		if( ( tmp = create_group( &ngrp ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* Hat diese Teilgruppe mehr als eine Zustandsmenge ? */
		if( ogrp.ugroup[i].count < 2 )
		{
			fprintf( stderr, "\nDSTATES[%d] -> Eigene Teilgruppe in ngrp.", ogrp.ugroup[i].ind[0] );

			/* Nein, diese Teilgruppe auch in ngrp aufnehmen */
			if( insert_ugroup( &ngrp, tmp, ogrp.ugroup[i].ind[0] ) < 0 )
				/* Fehler */
				return( -1 );
		}

		else
		{
			/*
				Hier sei nochmals drauf aufmerksam gemacht, daû der Zustand j aus i fÅr alle Zeichen k einen öbergang
				haben MUû.
				(Zustand d).
			*/

			/* Alle Eingabesymbole */
			for( k = 0; k < all_char_count; k++ )
			{
				/* Alle Zustandsmengen */
				for( j = 0; j < ogrp.ugroup[i].count; j++ )
				{
					/*
						Wenn die Zustandsmengen j und j+1 bei öbergang mit a in die gleiche Gruppe gelangen,
						muû nicht aufgetrennt werden.
					*/
					ind = get_dstate_index( ogrp.ugroup[i].ind[j], all_char[k] );

					/* Gruppe merken */
					ogrp.ugroup[i].grp_ind[j] = dstates[ind].group;


					fprintf( stderr, "\nDSTATES[%d][%c] = %d", ogrp.ugroup[i].ind[j], ( BYTE ) all_char[k], ind );
					fprintf( stderr, "\nZielgruppe: %d", dstates[ind].group );
				}

				/* Referenz-Gruppe */
				temp = ogrp.ugroup[i].grp_ind[0];

				/* Alle Ziel-Gruppen vergleichen */
				for( j = 1; j < ogrp.ugroup[i].count; j++ )
				{
					/* Sind beide gleich ? */
					if( temp != ogrp.ugroup[i].grp_ind[j] )
						/* Nein, Gruppen aufteilen */
						ogrp.ugroup[i].cpy_ind[j] = 1;
				}
			}

			/* Alle Ziel-Gruppen vergleichen */
			for( j = 0; j < ogrp.ugroup[i].count; j++ )
			{
				/* Gruppe aufteilen ? */
				if( ogrp.ugroup[i].cpy_ind[j] )
				{
					fprintf( stderr, "\nTrennen, DSTATES[%d] -> Eigene Gruppe in ngrp.\n", ogrp.ugroup[i].ind[j] );
					sep_grp( &ngrp, ogrp.ugroup[i].ind[j] );
				}

				else
				{
					fprintf( stderr, "\nKeine Trennung, DSTATES[%d] von ogrp nach ngrp.\n", ogrp.ugroup[i].ind[j] );

					/* Zustandsmenge bleibt erhalten, in ngrp Åbernehmen */
					if( insert_ugroup( &ngrp, tmp, ogrp.ugroup[i].ind[j] ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_ugroup( GRP *grp, WORD grp_index, WORD dstates_index )
{
	/* Hier wird eine neue Zustandsmenge in eine Untergruppe von grp eingetragen. */

	/* Speicher holen */
	if( ( grp->ugroup[grp_index].ind = ( WORD * ) realloc( ( VOID * ) grp->ugroup[grp_index].ind, ( grp->ugroup[grp_index].count + 1 ) * sizeof( WORD ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Speicher holen */
	if( ( grp->ugroup[grp_index].grp_ind = ( WORD * ) realloc( ( VOID * ) grp->ugroup[grp_index].grp_ind, ( grp->ugroup[grp_index].count + 1 ) * sizeof( WORD ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Speicher holen */
	if( ( grp->ugroup[grp_index].cpy_ind = ( WORD * ) realloc( ( VOID * ) grp->ugroup[grp_index].cpy_ind, ( grp->ugroup[grp_index].count + 1 ) * sizeof( WORD ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

#ifdef DUMP

	fprintf( stderr, "\nDSTATES[%d] ist in Teilgruppe %d.", dstates_index, grp_index );

#endif

	/* Eintragen */
	grp->ugroup[grp_index].ind[grp->ugroup[grp_index].count] = dstates_index;
	grp->ugroup[grp_index].grp_ind[grp->ugroup[grp_index].count] = 0;
	grp->ugroup[grp_index].cpy_ind[grp->ugroup[grp_index].count++] = 0;

	/* Alles OK */
	return( 0 );
}


WORD create_group( GRP *grp )
{
	/* Hier wird eine neue Untergruppe erzeugt. */

	/* Speicher holen */
	if( ( grp->ugroup = ( struct ugroup * ) realloc( ( VOID * ) grp->ugroup, ( grp->count + 1 ) * sizeof( struct ugroup ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Untergrupe initialisieren */
	grp->ugroup[grp->count].ind = 0L;
	grp->ugroup[grp->count].grp_ind = 0L;
	grp->ugroup[grp->count].cpy_ind = 0L;
	grp->ugroup[grp->count].count = 0;

	/* Alles OK */
	return( grp->count++ );
}


WORD copy_all_grp ( VOID )
{
	/* Hier werden die Teilgruppen von ngrp in ogrp eingetragen. */
	REG WORD	i, j;

	/* Genug Platz vorhanden ? */
	if( ngrp.count > ogrp.count )
	{
		/* Nein, genug Platz schaffen */
		if( ( ogrp.ugroup = ( struct ugroup * ) realloc( ( VOID * ) ogrp.ugroup, ( ogrp.count + ngrp.count - ogrp.count ) * sizeof( struct ugroup ) ) ) == NULL )
			/* Zu wenig Speicher */
			return( output_error( NO_MEMORY ) );
	}

	/* Alle Zeiger auf die Untergruppen von ngrp nach ogrp kopieren */
	for( i = 0; i < ngrp.count; i++ )
	{
		/* Alle Zustandsmengen */
		for( j = 0; j < ngrp.ugroup[i].count; j++ )
			/* Gruppen in DSTATES anpassen */
			dstates[ngrp.ugroup[i].ind[j]].group = i;

		/* Kopieren */
		ogrp.ugroup[i] = ngrp.ugroup[i];
	}

	/* Anzahl der neuen ZustÑnde festlegen */
	ogrp.count = ngrp.count;

	/* Initialisieren */
	ngrp.ugroup = 0L;
	ngrp.count = 0;

	/* Alles OK */
	return( 0 );
}


WORD copy_grp( GRP *from, GRP *to, WORD ugrp_ind )
{
	/* Hier wird eine Teilgruppe von from zu to kopiert. */

	/* Genug Platz vorhanden ? */
	if( from->count > to->count )
	{
		/* Nein, genug Platz schaffen */
		if( ( to->ugroup = ( struct ugroup * ) realloc( ( VOID * ) to->ugroup, ( to->count + 1 ) * sizeof( struct ugroup ) ) ) == NULL )
			/* Zu wenig Speicher */
			return( output_error( NO_MEMORY ) );
	}

	/* Diese eine Untergruppe von from nach to kopieren */
	to->ugroup[to->count++] = from->ugroup[ugrp_ind];

	/* Alles OK */
	return( 0 );
}


WORD sep_grp( GRP *to, WORD dstates_index )
{
	/* Hier wird eine Zustandsmenge aus from extrahiert und anschlieûend in to als eine neue Gruppe eingetragen. */
	REG WORD	a;

	/* Eine neue Gruppe in to erzeugen */
	if( ( a = create_group( to ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Zustandsmenge eintragen */
	if( ( insert_ugroup( to, a, dstates_index ) ) < 0 )
		/* Fehler */
		return( -1 );

	fprintf( stderr, "\nDSTATES[%d] von ogrp nach ngrp.", dstates_index );

	/* Alles OK */
	return( 0 );
}


WORD cmp_grp( GRP *grp1, GRP *grp2 )
{
	/* Hier werden zwei Gruppen miteinander verglichen. */
	REG WORD	i, j, p;

	/* Ist die Anzahl beider Teilgruppen gleich ? */
	if( grp1->count == grp2->count )
	{
		/* Ja, alle Zustandsmengen beider Teilgruppen vergleichen */
		for( i = 0; i < grp1->count; i++ )
		{
			/* Ist die Anzahl beider Zustandsmengen in der Teilgruppe i gleich ? */
			if( grp1->ugroup[i].count == grp2->ugroup[i].count )
			{
				/* Ja, alle Zustandsmengen beider Teilgruppen vergleichen */
				for( j = 0; j < grp1->ugroup[i].count; j++ )
				{
					/* Jede Zustandsmenge in grp2 */
					for( p = 0; p < grp2->ugroup[i].count; p++ )
					{
						/* Sind beide gleich ? */
						if( grp1->ugroup[i].ind[j] == grp2->ugroup[i].ind[p] )
							/* Ja, break */
							break;
					}

					/* War die Zustandsmenge von grp2 in grp1 enthalten ? */
					if( p == grp2->ugroup[i].count )
						/* Nein */
						return( -1 );
				}
			}

			else
				/* grp1 und grp2 sind nicht gleich */
				return( -1 );
		}
	}

	else
		/* grp1 und grp2 sind nicht gleich */
		return( -1 );

	/* Beide gleich */
	return( 0 );
}


WORD get_dstate_index( WORD dstates_index, UBYTE c )
{
	/* Liefert den Index in DSTATES zurÅck, den die Zustandsmenge in dstates_index bei c einnimmt. */
	REG WORD	i;

	/* Alle EintrÑge */
	for( i = 0; i < dstates[dstates_index].dtran_count; i++ )
	{
		/* Ist das Zeichen c gesetzt ? */
		if( dstates[dstates_index].dtran[i].ccl_str.ccl[c / CHAR_BIT] & ( 1 << ( c % CHAR_BIT ) ) )
			/* Ja, index zurÅckliefern */
			return( dstates[dstates_index].dtran[i].dstates_index );
	}

	/* Nicht gefunden */
	fprintf( stderr, "\nFehler in get_dstates_index !" );
	return( -1 );
}


WORD set_dstates( VOID )
{
	/*
		Hier werden die Ergebnisse der Zustandsminimierung ausgewertet, d.h., daû die DSTATES-Indizes in gleichen Gruppen
		aus dem DSTATES-Array herausgenommen werden.
	*/
	REG WORD	i, j, l, p;
	WORD		*dst_ind, *fix_d_ind;

	/* Speicher fÅr die Anpassung der ZustandsÅbergÑng */
	if( ( fix_d_ind = ( WORD * ) calloc( 1, dstates_count * sizeof( WORD ) ) ) == NULL )
		/* Fehler */
		return( -1 );

	/* Speicher fÅr neue Indizes in DSTATES anfordern */
	if( ( dst_ind = ( WORD * ) calloc( 1, dstates_count * sizeof( WORD ) ) ) == NULL )
		/* Fehler */
		return( -1 );

	/*
		Speicher fÅr die ZustÑnde holen.
		Die D_IND_STR hat folgenden Aufbau: [i].old = Alter Index in DSTATES
											[i].new = Neuer Index in DSTATES
	*/
	if( ( d_ind = ( D_IND_STR * ) malloc( 2 * dstates_count * sizeof( D_IND_STR ) ) ) == NULL )
		/* Fehler */
		return( -1 );

	/* Alle Gruppen durchlaufen */
	for( i = 0; i < ogrp.count; i++ )
	{
		/* Hat die ite Teilgruppe mehrere Zustandsmengen vereint ? */
		if( ogrp.ugroup[i].count > 1 )
		{
			/* Ab den zweiten Eintrag */
			for( j = 1; j < ogrp.ugroup[i].count; j++ )
			{
				/* Ja, den ReprÑsentanten der Teilgruppe(immer die erste Zustandsmenge) in d_ind eintragen. */
				d_ind[d_ind_count].old = ogrp.ugroup[i].ind[j];
				d_ind[d_ind_count++].new = ogrp.ugroup[i].ind[0];

				/* Anpassen */
				dst_ind[ogrp.ugroup[i].ind[j]] = 1;

				/* Wert zur Korrektur */
				for( l = ogrp.ugroup[i].ind[j] + 1; l < dstates_count; l++ )
					fix_d_ind[l]++;
			}
		}
	}

	/* Nun die DSTATES-Struktur anpassen */
	for( i = 0, p = 0; i < dstates_count; p++ )
	{
		/* Bleibt dieser Eintrag erhalten ? */
		if( dst_ind[p] )
		{
			/* Nein, Eintrag lîschen, die Åbrigen um eine Einheit vorholen */
			free( ( VOID * ) &dstates[i].states );
			free( ( VOID * ) &dstates[i].dtran );
			free( ( VOID * ) &dstates[i] );

			/* Kopieren */
			memcpy( ( VOID * ) &dstates[i], ( VOID * ) &dstates[i + 1], ( ULONG ) ( dstates_count - i - 1 ) * sizeof( DSTATES ) );

			/* Ein Eintrag weniger */
			dstates_count--;
		}

		else
		{
			/* Dieser Eintrag bleibt erhalten, nun die ZustandsÅbergÑnge anpassen */
			for( j = 0; j < dstates[i].dtran_count; j++ )
			{
				/* Hier stehen die neuen öbergÑnge */
				for( l = 0; l < d_ind_count; l++ )
				{
					/* Stimmen beide öbergangsindizes Åberein ? */
					if( dstates[i].dtran[j].dstates_index == d_ind[l].old )
						/* Ja, Ñndern */
						dstates[i].dtran[j].dstates_index = d_ind[l].new;
				}

				/* Den Index anpassen */
				dstates[i].dtran[j].dstates_index -= fix_d_ind[dstates[i].dtran[j].dstates_index];
			}

			/* Es wurde kein DSTATES-Eintrag gelîscht */
			i++;
		}
	}

	/* Speicher freigeben */
	free( ( VOID * ) dst_ind );
	free( ( VOID * ) d_ind );
	free( ( VOID * ) fix_d_ind );

	/* Nun DSTATES anpassen */
	return( 0 );
}


WORD delete_states( VOID )
{
	/*
		Hier werden die ZustÑnde, die mit allen Eingabesymbolen nur öbergÑnge zu sich selbst haben und nicht
		Finit-States sind, gelîscht.
		öbergÑnge auf sie werden gelîscht.
	*/
	REG WORD	i, j, l;
	DSTATES		*d;
	WORD		*states, *fix_d_ind, main_flg, flg, count;

	/* Speicher fÅr die Anpassung der ZustandsÅbergÑng */
	if( ( fix_d_ind = ( WORD * ) calloc( 1, dstates_count * sizeof( WORD ) ) ) == NULL )
		/* Fehler */
		return( -1 );

	/* Speicher anfordern */
	if( ( states = ( WORD * ) malloc( dstates_count * sizeof( WORD ) ) ) == NULL )
		/* Fehler */
		return( -1 );

	do
	{
		/* ZÑhler lîschen */
		count = 0;

		/* Hauptflag lîschen */
		main_flg = 0;

		/* Alle DSTATES-EintrÑge abarbeiten */
		for( i = 0; i < dstates_count; i++ )
		{
			/* Hat DSTATES[i] nur öbergÑnge zu sich selbst ? */
			if( ( dstates[i].dtran_count == 1 ) && ( dstates[i].type != FINIT ) )
			{
				/* all_bchar.ccl und dtran[0].ccl vergleichen */
				if( !memcmp( ( VOID * ) all_bchar.ccl, ( VOID * ) dstates[i].dtran[0].ccl_str.ccl, CCL_WIDTH ) )
				{
					/* Diesen Zustand mit in states aufnehmen */
					states[count++] = i;

					/* Hauptflag setzen */
					main_flg++;
				}
			}
		}

		/* Solange etwas geÑndert wird */
		if( main_flg )
		{
			do
			{
				/* Flag ungÅltig machen */
				flg = 0;

				/* Alle DSTATES-EintrÑge */
				for( i = 0; i < dstates_count; i++ )
				{
					/* Alle DTRAN-EintrÑge */
					for( j = 0; j < dstates[i].dtran_count; j++ )
					{
						/* Nun alle EintrÑge in states */
						for( l = 0; l < count; l++ )
						{
							/* Hat DSTATES[i] einen öbergang zu states[l] ? */
							if( dstates[i].dtran[j].dstates_index == states[l] )
							{
								/* Ja, Flag setzen */
								flg++;

								/* Zeiger auf DSTATES[i] holen */
								d = &dstates[i];

								/* MÅssen folgende DTRAN-Zeiger um eine Einheit vorgeholt werden ? */
								if( j < ( d->dtran_count - 1 ) )
									/* Ja */
									memcpy( ( VOID * ) &d->dtran[j], &d->dtran[j + 1], ( ULONG ) sizeof( DTRAN ) * ( d->dtran_count - j - 1 ) );

								/* Ein öbergang weniger */
								d->dtran_count--;
							}
						}
					}
				}

			}while( flg );
		}

	}while( main_flg );

	/* Array lîschen */
	memset( ( VOID * ) states, 0, dstates_count * sizeof( WORD ) );

	/* Alle benutzten DSTATES-EintrÑge feststellen */
	for( i = 0; i < dstates_count; i++ )
	{
		/* Alle DTRAN-EintrÑge */
		for( j = 0; j < dstates[i].dtran_count; j++ )
			/* 1, wenn benutzt */
			states[dstates[i].dtran[j].dstates_index] = 1;
	}

	/* Nun alle EintrÑge durchgehen */
	for( i = 1, l = 1; i < dstates_count; l++ )
	{
		/* Wurde der Zustand nicht angesprungen ? */
		if( !( states[l] ) && !( dstates[i].type ) )
		{
			/* Wert zur Korrektur */
			for( j = i + 1; j < dstates_count; j++ )
				fix_d_ind[j]++;

			/* Nun DSTATES[i] lîschen */
			free( ( VOID * ) &dstates[i].states );
			free( ( VOID * ) &dstates[i].dtran );
			free( ( VOID * ) &dstates[i] );

			/* Kopieren */
			memcpy( ( VOID * ) &dstates[i], ( VOID * ) &dstates[i + 1], ( ULONG ) ( dstates_count - i - 1 ) * sizeof( DSTATES ) );

			/* Ein Eintrag weniger */
			dstates_count--;
		}

		else
			/* Zustand wird angesprungen */
			i++;
	}

	/* Nun die ZustandsÅbergÑnge anpassen */
	for( i = 0; i < dstates_count; i++ )
	{
		/* Alle DTRAN-EintrÑge */
		for( j = 0; j < dstates[i].dtran_count; j++ )
			/* Anpassen */
			dstates[i].dtran[j].dstates_index -= fix_d_ind[dstates[i].dtran[j].dstates_index];
	}

	/* Speicher wieder freigeben */
	free( ( VOID * ) states );
	free( ( VOID * ) fix_d_ind );

	/* Alles OK */
	return( 0 );
}


VOID output_ogroup( VOID )
{
	/* Hier wird die minimierte DTRAN-Tabelle ausgegeben. */
	REG WORD	i, j, k, p;

	fprintf( stderr, "\n\n" );

	for( i = 0; i < ogrp.count; i++ )
	{
		for( p = 0; p < dstates[ ogrp.ugroup[i].ind[0] ].dtran_count; p++ )
		{
			for( k = 0; k < all_char_count; k++ )
			{
				if( dstates[ ogrp.ugroup[i].ind[0] ].dtran[p].ccl_str.ccl[all_char[k] / CHAR_BIT] & ( 1 << ( all_char[k] % CHAR_BIT ) ) )
					fprintf( stderr, "\nZustand %d: Zeichen %c, Ziel %d.", ogrp.ugroup[i].ind[0], ( BYTE ) all_char[k], dstates[dstates[ogrp.ugroup[i].ind[0]].dtran[p].dstates_index].group );
			}
		}
	}

	fprintf( stderr, "\n\n" );

	for( i = 0; i < dstates_count; i++ )
	{
		fprintf( stderr, "\n\nDSTATES[%d]: dtran_count = %d.", i, dstates[i].dtran_count );

		for( j = 0; j < dstates[i].dtran_count; j++ )
		{
			fprintf( stderr, "\n%d: dstates_index = %d, Gruppe: %d.", j, dstates[i].dtran[j].dstates_index, dstates[dstates[i].dtran[j].dstates_index].group );
		}
	}


	fprintf( stderr, "\n\n" );

	for( i = 0; i < ogrp.count; i++ )
	{
		for( j = 0; j < ogrp.ugroup[i].count; j++ )
		{
			fprintf( stderr, "\nGruppe %d: dstates_index %d = %d.", i, j, ogrp.ugroup[i].ind[j] );
		}
	}
}