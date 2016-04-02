/*
	Projekt		:	Scanner-Generator
	Modul		:	NEA.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	23.07.1997
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/



/************/
/* Includes */
/************/

/* Include-Datei des Scanner-Generators */
#include "sslex.h"



/**************/
/* Prototypen */
/**************/

VOID output_nea( NEA_STR *nea_str );
LONG new_nea( VOID );



/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/* Array mit den divsersen NEAen */
NEA_STR		*nea_str = 0L;

/* Anzahl der NEA, fuer die Speicher angefordert wurde */
WORD		nea_str_count = 0;


/* Array mit allen NEA-Zust�nden */
NEA_STATE	*nea_state = 0L;

/* Anzahl der NEA-Zust�nde */
LONG		nea_count = 0;



/***********/
/* Imports */
/***********/

MGLOBAL NEA_STR	*akt_nea;
MGLOBAL LONG	zeile;
MGLOBAL WORD	all_char_count;
MGLOBAL BYTE	all_char[];



LONG init_nea( VOID )
{
	/* Hier wird ein neuer NEA erzeugt. */
	LONG	nea_ind, state;

	/* Speicher fuer einen NEA anfordern */
	if( ( nea_ind = new_nea() ) < 0 )
		/* Fehler */
		return( -1 );

	/* Speicher fuer einen Zustand anfordern */
	if( ( state = new_nea_state( nea_ind ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Werte im Zustand setzen */
	nea_state[state].state = state;

	/* Start-Zustand in NEA_STR festhalten */
	nea_str[nea_ind].start_state = state;

	/* Einen EPSILON-�bergang zum zweiten Zustand erzeugen.	*/
	if( nea_bra( nea_ind, START_STATE, NEW_FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( nea_ind );
}


WORD make_nea_optional( LONG nea_ind )
{
	/*
		*-Operator.
		NEA kann beliebig oft durchlaufen werden.
	*/

	/* Einen EPSILON-�bergang vom ersten zum letzen Zustand erzeugen.	*/
	if( nea_bra( nea_ind, START_STATE, FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* Einen EPSILON-�bergang vom letzen zum ersten Zustand erzeugen.	*/
	if( nea_bra( nea_ind, FINIT_STATE, START_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD make_nea_once( LONG nea_ind )
{
	/*
		+-Operator.
		NEA muss min. einmal durchlaufen werden.
	*/

	/* Einen EPSILON-�bergang vom letzen zum ersten Zustand erzeugen.	*/
	if( nea_bra( nea_ind, FINIT_STATE, START_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD make_nea_once_or_never( LONG nea_ind )
{
	/*
		?-Operator.
		NEA kann nullmal oder einmal durchlaufen werden.
	*/

	/* Einen EPSILON-�bergang vom ersten zum letzen Zustand erzeugen.	*/
	if( nea_bra( nea_ind, START_STATE, FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


LONG or_nea( LONG nea_ind1, LONG nea_ind2 )
{
	/*
		Hier werden zwei NEAen "verodert".

		START	-> EPSILON -> NEA1 -> EPSILON -> FINIT
		START	-> EPSILON -> NEA2 -> EPSILON -> FINIT
	 */
	REG LONG	i;

	/* Einen neuen Startzustand erzeugen mit EPSILON-�bergang zum Startzustand des ersten NEA */
	if( nea_bra( nea_ind1, NEW_START_STATE, START_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* EPSILON-�bergang zum Startzustand des zweiten NEA */
	if( nea_bra( nea_ind1, START_STATE, nea_str[nea_ind2].start_state, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* Ja, einen neuen Finitzustand erzeugen mit EPSILON-�bergang vom Finitzustand des ersten NEA */
	if( nea_bra( nea_ind1, nea_str[nea_ind1].finit_state, NEW_FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* EPSILON-�bergang vom Finit-State des zweiten NEA */
	if( nea_bra( nea_ind1, nea_str[nea_ind2].finit_state, nea_str[nea_ind1].finit_state, 0L, 0, 0, _EPSILON ) < 0 )
		/* Fehler */
		return( -1 );

	/* state_arr anpassen */
	if( ( nea_str[nea_ind1].state_arr = ( LONG * ) realloc( ( VOID * ) nea_str[nea_ind1].state_arr, ( nea_str[nea_ind1].state_arr_count + nea_str[nea_ind2].state_arr_count ) * sizeof( LONG ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Werte �bertragen */
	for( i = 0; i < nea_str[nea_ind2].state_arr_count; i++ )
		/* Zustand �bertragen */
		nea_str[nea_ind1].state_arr[nea_str[nea_ind1].state_arr_count + i] = nea_str[nea_ind2].state_arr[i];

	/* Anzahl der Zust�nde festlegen */
	nea_str[nea_ind1].state_arr_count += nea_str[nea_ind2].state_arr_count;

	/* Den kleinsten Zustand im NEA bestimmen */
	nea_str[nea_ind1].least_state = MIN( nea_str[nea_ind1].least_state, nea_str[nea_ind2].least_state );

	/* Den gr��ten Zustand im NEA festlegen */
	nea_str[nea_ind1].biggest_state = MAX( nea_str[nea_ind1].biggest_state, nea_str[nea_ind2].biggest_state );

	/* Alles OK */
	return( nea_ind1 );
}


LONG link_nea( LONG nea_ind1, LONG nea_ind2 )
{
	/* Hier werden zwei NEAen zu einem verknuepft. */
	REG LONG	i;

	/* Einfach einen Sprung�bergang vom Endzustand des ersten zum First-State vom zweiten erzeugen */
	if( ( nea_bra( nea_ind1, FINIT_STATE, nea_str[nea_ind2].start_state, 0L, 0, 0, 1 ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* state_arr anpassen */
	if( ( nea_str[nea_ind1].state_arr = ( LONG * ) realloc( ( VOID * ) nea_str[nea_ind1].state_arr, ( nea_str[nea_ind1].state_arr_count + nea_str[nea_ind2].state_arr_count ) * sizeof( LONG ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Werte �bertragen */
	for( i = 0; i < nea_str[nea_ind2].state_arr_count; i++ )
		nea_str[nea_ind1].state_arr[nea_str[nea_ind1].state_arr_count + i] = nea_str[nea_ind2].state_arr[i];

	/* Anzahl der Zust�nde festlegen */
	nea_str[nea_ind1].state_arr_count += nea_str[nea_ind2].state_arr_count;

	/* Neuen Finit-Zustand eintragen */
	nea_str[nea_ind1].finit_state = nea_str[nea_ind2].finit_state;

	/* Den kleinsten Zustand im NEA bestimmen */
	nea_str[nea_ind1].least_state = MIN( nea_str[nea_ind1].least_state, nea_str[nea_ind2].least_state );

	/* Den gr��ten Zustand im NEA festlegen */
	nea_str[nea_ind1].biggest_state = MAX( nea_str[nea_ind1].biggest_state, nea_str[nea_ind2].biggest_state );

	/*
		Ist der Index des zweiten NEA(nea_ind2) in der PREDEF_NEA_STR, dann wird er nicht gel�scht.
	*/
	if( if_predef_nea( nea_ind2 ) < 0 )
	{
		/* Den Speicher f�r die NEA_STR freigeben */
		free( ( VOID * ) &nea_str[nea_ind2] );

		/* Eine NEA_STR weniger */
		nea_str_count--;
	}

	/* Index des ersten NEA zur�ckliefern */
	return( nea_ind1 );
}


LONG rep_nea( LONG nea_ind, LONG from, LONG to )
{
	/*
		Hier wird ein NEA vervielfaeltigt.
		Bsp: 	-{0,9}	- 0 - 9mal
				-{5,}	- 5mal - beliebig oft
				-{5}	- 5mal
	*/
	REG LONG	i;
	REG LONG	k = 0;
	LONG		eps_state, eps_arr[100], copy_nea_ind, temp_nea_ind;
	WORD		anz_copy, eps_count = 0;

	/*
		Spezielf�lle behandeln.
		1. {1,} - einmal bis beliebig oft ( + )
		2. {0,} - keinmal bis beliebig oft ( * )
	*/
	if( ( from == 1 ) && ( to == -1 ) )
	{
		/* Einen EPSILON-�bergang vom letzen zum ersten Zustand erzeugen.	*/
		if( nea_bra( nea_ind, FINIT_STATE, START_STATE, 0L, 0, 0, _EPSILON ) < 0 )
			/* Fehler */
			return( -1 );
	}

	else if( !( from ) && ( to == -1 ) )
	{
		/* Einen EPSILON-�bergang vom ersten zum letzen Zustand erzeugen.	*/
		if( nea_bra( nea_ind, START_STATE, FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
			/* Fehler */
			return( -1 );

		/* Einen EPSILON-�bergang vom letzen zum ersten Zustand erzeugen.	*/
		if( nea_bra( nea_ind, FINIT_STATE, START_STATE, 0L, 0, 0, _EPSILON ) < 0 )
			/* Fehler */
			return( -1 );
	}

	else
	{
		/* Anzahl der Kopien : {x,y} oder {x,} oder {x} */
		anz_copy = ( WORD ) ( to > 0 ? ( to - 1 ) : ( from - 1 ) );

		/*
			Den NEA kopieren.
			Er dient dann als Kopiervorlage, da sich der urspr�ngliche NEA ver�ndert.
		*/
		if( ( copy_nea_ind = copy_nea( nea_ind ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* Anzahl der Kopien */
		while( k++ < anz_copy )
		{
			/* Den ersten neuen Zustand merken */
			eps_state = nea_str[nea_ind].finit_state;

			/* Den NEA kopieren */
			if( ( temp_nea_ind = copy_nea( copy_nea_ind ) ) < 0 )
				/* Fehler */
				return( -1 );

			/* Den alten und den kopierten NEA verbinden */
			if( link_nea( nea_ind, temp_nea_ind ) < 0 )
				/* Fehler */
				return( -1 );

			/* EPSILON-�bergang einf�gen ? */
			if( k >= ( from - 1 ) )
				/* Ja, Zustand merken */
				eps_arr[eps_count++] = eps_state;
		}

		/* EPSILON-�berg�nge erzeugen */
		for( i = 0; i < eps_count; i++ )
		{
			/* EPSILON-�bergang zum Finit-Zustand */
			if( nea_bra( nea_ind, eps_arr[i], FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
				/* Fehler */
				return( -1 );
		}

		/* War from == 0 ? */
		if( !from )
		{
			/* Ja, EPSILON-�bergang vom Startzustand zum Endzustand */
			if( nea_bra( nea_ind, START_STATE, FINIT_STATE, 0L, 0, 0, _EPSILON ) < 0 )
				/* Fehler */
				return( -1 );
		}

		/* Die Kopiervorlage l�schen */
		free( ( VOID * ) nea_str[copy_nea_ind].state_arr );
		free( ( VOID * ) &nea_str[copy_nea_ind] );

		/* Nun alle folgenden NEAen um eine Einheit verschieben */
		memmove( ( VOID * ) &nea_str[copy_nea_ind], ( VOID * ) &nea_str[copy_nea_ind + 1], sizeof( NEA_STR ) * ( nea_str_count - copy_nea_ind ) );
		nea_str_count--;
	}

	/* Alles OK */
	return( 0 );
}


/*
	Debug-Hilfe
*/

VOID output_nea( NEA_STR *nea_str )
{
	/* Hier wird ein NEA ausgegeben */
	NEA_STATE	*state;
	WORD		i, j, p;

	/* Main-NEA ausgeben */
	fprintf( stderr, "\n\nNEA                 : %s", nea_str->nea_text );
	fprintf( stderr, "Anzahl der Zust�nde : %d", nea_str->state_arr_count );
	fprintf( stderr, "\nStartzustand        : %lu", nea_str->start_state );
	fprintf( stderr, "\nEndzustand          : %lu", nea_str->finit_state );
	fprintf( stderr, "\nLookahead           : %lu", nea_str->lookahead_state );
	fprintf( stderr, "\nGr��ter Zustand     : %lu", nea_str->biggest_state );
	fprintf( stderr, "\nKleinster Zustand   : %lu\n", nea_str->least_state );

	/* Alle NEA ausgeben */
	for( i = 0; i < nea_str->state_arr_count; i++ )
	{
		state = &nea_state[nea_str->state_arr[i]];

		for( j = 0; j < state->bra_count; j++ )
		{
			fprintf( stderr, "\nVon %lu nach %lu mit EPSILON: %d ", state->state, state->bra_str[j].bra, state->bra_str[j].eps );

			for( p = 0; p < all_char_count; p++ )
			{
				if( state->bra_str[j].ccl_str.ccl[all_char[p] / CHAR_BIT] & ( 1 << ( all_char[p] % CHAR_BIT ) ) )
					fprintf( stderr, " %c,", ( BYTE ) all_char[p] );
			}
		}
	}

	fprintf( stderr, "\n\n" );
}


LONG new_nea_state( LONG nea_ind )
{
	/* Speicher fuer einen neuen NEA-Zustand anfordern */
	if( ( nea_state = ( NEA_STATE * ) realloc( ( VOID * ) nea_state, ( nea_count + 1 ) * sizeof( NEA_STATE ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Den Index des Zustands merken */
	if( ( nea_str[nea_ind].state_arr = ( LONG * ) realloc( ( VOID * ) nea_str[nea_ind].state_arr, ( nea_str[nea_ind].state_arr_count + 1 ) * sizeof( LONG ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Den Index des Zustands merken */
	nea_str[nea_ind].state_arr[nea_str[nea_ind].state_arr_count++] = nea_count;

	/* Neuen Zustand initialisieren */
	nea_state[nea_count].state = nea_count;
	nea_state[nea_count].bra_str = 0L;
	nea_state[nea_count].bra_count = 0;

	/* Den kleinsten Zustand im NEA bestimmen */
	nea_str[nea_ind].least_state = MIN( nea_str[nea_ind].least_state, nea_count );

	/* Den gr��ten Zustand im NEA festlegen */
	nea_str[nea_ind].biggest_state = MAX( nea_str[nea_ind].biggest_state, nea_count );

	/* Index zur�ckliefern */
	return( nea_count++ );
}


LONG new_nea( VOID )
{
	/* Speicher f�r einen neuen NEA anfordern */
	if( ( nea_str = ( NEA_STR * ) realloc( ( VOID * ) nea_str, ( nea_str_count + 1 ) * sizeof( NEA_STR ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* NEA_STR initialisieren */
	memset( ( VOID * ) &nea_str[nea_str_count], 0, sizeof( NEA_STR ) );

	/* Den gr��ten Zustand setzen */
	nea_str[nea_str_count].biggest_state = 0;

	/* Den kleinsten Zustand setzen */
	nea_str[nea_str_count].least_state = nea_count;

	/* Index zur�ckliefern */
	return( nea_str_count++ );
}


WORD nea_bra( LONG nea_ind, LONG from, LONG to, UBYTE ccl[], WORD neg, WORD any, WORD eps )
{
	/* Hier wird ein Zustands�bergang mit den Zeichen in ccl erzeugt. */
	NEA_STATE	*nea;
	REG LONG	i, j;
	BRA_STR		*bra_str;

	/* �bergang vom Startzustand aus ? */
	if( from == START_STATE )
		/* Ja */
		from = nea_str[nea_ind].start_state;

	/* �bergang vom Endzustand aus ? */
	else if( from == FINIT_STATE )
		/* Nun den Index des Finit-Zustands holen */
		from = nea_str[nea_ind].finit_state;

	/* An den Startzustand anh�ngen ? */
	if( to == START_STATE )
		/* Ja */
		to = nea_str[nea_ind].start_state;

	/* An den End-Zustand anh�ngen ? */
	else if( to == FINIT_STATE )
		/* Ja */
		to = nea_str[nea_ind].finit_state;

	/* Einen neuen End-Zustand erzeugen ? */
	else if( to == NEW_FINIT_STATE )
	{
		/* Speicher f�r einen Zustand anfordern */
		if( ( to = new_nea_state( nea_ind ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* Neuen Finit-Zustand festlegen */
		nea_str[nea_ind].finit_state = to;
	}

	/* Einen neuen Start-Zustand erzeugen ? */
	if( from == NEW_START_STATE )
	{
		/* Ja, Speicher fuer einen Zustand anfordern */
		if( ( from = new_nea_state( nea_ind ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* to ist der alte Start-Zustand */
		to = nea_str[nea_ind].start_state;

		/* Neuen Start-Zustand festlegen */
		nea_str[nea_ind].start_state = from;
	}

	/* Zeiger auf Zustand holen */
	nea = &nea_state[from];

	/* Alle vorhandenen Zustands�berg�nge pr�fen */
	if( nea->bra_str )
	{
		/* Alle vorhandenen Zustands�berg�nge pr�fen */
		for( i = 0; i < nea->bra_count; i++ )
		{
			/* Gibt es bereits einen Zustands�bergang zu to ? */
			if( nea->bra_str[i].bra == to )
			{
				/* Ja, wenn die "�u�eren" Umst�nde passen, brauch keine neue BRA_STR erzeugt werden. */
				if( ccl && ( nea->bra_str[i].neg_ccl == neg ) && ( nea->bra_str[i].any == any ) && ( nea->bra_str[i].eps == eps ) )
				{
					/* Die neue Zeichenklasse in die existierende odern */
					for( j = 0; j < CCL_WIDTH; j++ )
						/* Odern */
						nea->bra_str[i].ccl_str.ccl[j] |= ccl[j];
				}

				else
				{
					/* Die �u�eren Umst�nde passen nicht */
					if( ( nea->bra_str = ( BRA_STR * ) realloc( ( VOID * ) nea->bra_str, ( nea->bra_count + 1 ) * sizeof( BRA_STR ) ) ) == NULL )
						/* Zu wenig Speicher */
						return( output_error( NO_MEMORY ) );

					/* Zeiger auf BRA_STR holen */
					bra_str = &nea->bra_str[nea->bra_count++];

					/* Werte eintragen */
					bra_str->bra = to;
					bra_str->neg_ccl = neg;
					bra_str->any = any;
					bra_str->eps = eps;

					/* Ist �berhaupt etwas in der CCL ? */
					if( ccl )
						/* Die neue Zeichenklasse eintragen */
						memcpy( ( VOID * ) bra_str->ccl_str.ccl, ( VOID * ) ccl, CCL_WIDTH );

					else
						/* CCL initialisieren */
						memset( ( VOID * ) bra_str->ccl_str.ccl, 0, CCL_WIDTH );
				}

				/* Raus hier */
				return( 0 );
			}
		}
	}

	/* Es gibt noch keine Zustands�berg�nge */
	if( ( nea->bra_str = ( BRA_STR * ) realloc( ( VOID * ) nea->bra_str, ( nea->bra_count + 1 ) * sizeof( BRA_STR ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Zeiger auf BRA_STR holen */
	bra_str = &nea->bra_str[nea->bra_count++];

	/* Werte eintragen */
	bra_str->bra = to;
	bra_str->neg_ccl = neg;
	bra_str->any = any;
	bra_str->eps = eps;

	/* Ist �berhaupt etwas in der CCL ? */
	if( ccl )
		/* Die neue Zeichenklasse in die BRA_STR �bertragen */
		memcpy( ( VOID * ) bra_str->ccl_str.ccl, ( VOID * ) ccl, CCL_WIDTH );


	else
		/* CCL initialisieren */
		memset( ( VOID * ) bra_str->ccl_str.ccl, 0, CCL_WIDTH );

	/* Alles OK */
	return( 0 );
}


LONG copy_nea( LONG nea_ind )
{
	/* Hier wird ein ganzer NEA kopiert. */
	REG LONG	i, j;
	NEA_STR		*nea;
	LONG		new_nea_ind, state;
	WORD		anz_states;

	/* Speicher f�r einen neuen NEA anfordern */
	if( ( new_nea_ind = new_nea() ) < 0 )
		/* Fehler */
		return( -1 );

	/* Neuen Start-Zustand festlegen */
	nea_str[new_nea_ind].start_state = nea_count;

	/* Zeiger auf NEA holen */
	nea = &nea_str[nea_ind];

	/* Anzahl der Zust�nde merken */
	anz_states = nea->state_arr_count;

	/* Alle Zust�nde kopieren */
	for( i = 0; i < anz_states; i++ )
	{
		/* Speicher f�r einen Zustand anfordern */
		if( ( state = new_nea_state( new_nea_ind ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* NEA_STATE-Struktur kopieren */
		nea_state[state].bra_count = nea_state[nea->state_arr[i]].bra_count;

		/* Speicher f�r BRA_STR anfordern */
		if( ( nea_state[state].bra_str = ( BRA_STR * ) malloc( sizeof( BRA_STR ) * nea_state[nea->state_arr[i]].bra_count ) ) == NULL )
			/* Zu wenig Speicher */
			return( -1 );

		/* Alle BRA_STR-Strukturen des Zustands kopieren */
		for( j = 0; j < nea_state[nea->state_arr[i]].bra_count; j++ )
		{
			/* Inhalt kopieren */
			memcpy( ( VOID * ) &nea_state[state].bra_str[j], ( VOID * ) &nea_state[nea->state_arr[i]].bra_str[j], sizeof( BRA_STR ) );

			/* Nummer des Zustands anpassen */
			nea_state[state].bra_str[j].bra = ( state + nea_state[nea->state_arr[i]].bra_str[j].bra - nea->state_arr[i] );
		}
	}

	/* Neuen End-Zustand festlegen */
	nea_str[new_nea_ind].finit_state = nea_count - 1;

	/* Alles OK */
	return( new_nea_ind );
}


VOID free_nea( LONG nea_ind )
{
	/* Hier wird der Speicher eines NEA wieder freigegeben. */
	REG LONG	i;

	/* Alle Zust�nde */
	for( i = 0; i < nea_str[nea_ind].state_arr_count; i++ )
		/* Alle BRA_STRs freigeben */
		free( ( VOID * ) nea_state[nea_str[nea_ind].state_arr[i]].bra_str );

	/* Speicher freigeben */
	free( ( VOID * ) nea_str[nea_ind].state_arr );
	free( ( VOID * ) &nea_str[nea_ind] );
}


WORD insert_re( LONG nea_ind, BYTE *start, BYTE *end )
{
	/* Hier wird der RE als Text in die NEA_STR-Struktur eingetragen(zu Debugzwecken). */
	REG ULONG	len;

	/* Die L�nge des REs feststellen */
	len = ( ULONG ) ( end - start );

	/* Speicher reservieren */
	if( ( nea_str[nea_ind].nea_text = ( BYTE * ) malloc( sizeof( BYTE ) * ( len + 1 ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( -1 );

	/* String kopieren */
	strncpy( ( VOID * ) nea_str[nea_ind].nea_text, ( VOID * ) start, len );

	/* Null-Byte anh�ngen */
	nea_str[nea_ind].nea_text[len] = 0;

	/* Alles OK */
	return( 0 );
}