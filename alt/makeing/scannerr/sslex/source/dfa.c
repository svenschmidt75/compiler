/*
	Projekt		:	Scanner-Generator
	Modul		:	DFA.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	27.03.1998
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/





/************/
/* Includes */
/************/

/* Include-Datei des Scanner-Generators */
#include "sslex.h"
#include "sslexpar.h"



/****************/
/* Definitionen */
/****************/


/* Dump-Modus */
#undef  DUMP
#define DUMP2
#define DUMP3


/* Zustandsminimierung unterbinden */
#define	NO_MIN_DEA




/* F�r Stack und e_closure */
typedef struct
{
	/* Das Array mit den Zust�nden */
	LONG	*states;

	/* Position in states */
	WORD	pos;

	/* Anzahl der Zust�nde in states */
	WORD	count;

}STATE_STR;



/*****************/
/* Deklarationen */
/*****************/


/* Die meisten Zust�nde in einem NEA */
LONG		nea_states = 0;

/* Struktur mit Zust�nden */
STATE_STR	e_cls = { 0 }, e_stack = { 0 }, move = { 0 };

/* Array mit den Zustandsmengen */
DSTATES		*dstates = 0L;

/* Anzahl der Elemente in dstates */
WORD		dstates_count = 0;

 


/**************/
/* Prototypen */
/**************/

WORD insert_dstates( STATE_STR *s, SS_STR *ss );
VOID copy( STATE_STR *source, STATE_STR *dest );
WORD insert_dtran( WORD from, WORD to, WORD c );
VOID insert_state( STATE_STR *s, LONG state );
WORD move_it( WORD dstates_ind, WORD c );
WORD e_clos( STATE_STR *st, SS_STR *ss );
LONG pop_stack( STATE_STR *stack );
WORD look_dstates( STATE_STR *s );
WORD create_dtran( WORD pos );
WORD gen_dtran( SS_STR *ss );
WORD rise_dtran( WORD pos );
VOID output_dstates( VOID );
VOID push_tab( VOID );
WORD init( VOID );



/***********/
/* Imports */
/***********/


MGLOBAL NEA_STATE	*nea_state;
MGLOBAL NEA_STR		*nea_str;
MGLOBAL SS_STR		*ss_str;
MGLOBAL ULONG		nea_count;
MGLOBAL LONG		ss_str_count;
MGLOBAL WORD		all_char_count;
MGLOBAL BYTE		all_char[];




WORD gen_dea( VOID )
{
	/* Hier wird nun aus den NEAen DEAen. */
	REG LONG	i;
/*
	/* Alle SCs */
	for( i = 0; i < ss_str_count; i++ )
	{
*/

#ifndef NO_MIN_DEA

		/* Die Zustandsminimierung durchf�hren */
		if( gen_d( &ss_str[0] ) < 0 )
			/* Fehler */
			return( -1 );
#endif

		/* Die NEAen in DEAen wandeln */
		if( gen_dtran( &ss_str[0] ) < 0 )
			/* Fehler */
			return( -1 );

#ifndef NO_MIN_DEA

		/* Die Zustandsmenge des DEA minimieren */
		if( min_dfa() < 0 )
			/* Fehler */
			return( -1 );
#endif

		output_dstates();

/*
		/* Die Zustandsminimierung durchf�hren */
		if( gen_d( &ss_str[0] ) < 0 )
			/* Fehler */
			return( -1 );

		/* Die Zustandsmenge des DEA minimieren */
		if( min_dfa() < 0 )
			/* Fehler */
			return( -1 );

		/* Den Zeiger auf DTRAN in die SS_STR eintragen */
		ss_str[i].dtran = dtran;

		/* Speicher freigeben */
		free( ( VOID * ) dtran );
*/

#ifdef DUMP3
		push_tab();
#endif

/*	}
*/
	/* Alles OK */
	return( 0 );
}


WORD init( VOID )
{
	/* Hier werden ein paar wichtige Datenstrukturen initialisiert. */

	/* Array f�r die Zust�nde */
	if( ( e_cls.states = ( LONG * ) malloc( sizeof( LONG ) * nea_count ) ) == NULL )
		/* Fehler */
		return( -1 );

	/* Array f�r die Zust�nde */
	if( ( e_stack.states = ( LONG * ) malloc( sizeof( LONG ) * nea_count ) ) == NULL )
		/* Fehler */
		return( -1 );

	/* Array f�r die Zust�nde */
	if( ( move.states = ( LONG * ) malloc( sizeof( LONG ) * nea_count ) ) == NULL )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD gen_dtran( SS_STR *ss )
{
	/* Hier werden die NEAen in der SC ss zu DEAen. */
	REG WORD	i;
	WORD		pos, ind = 0;

	/* Anzahl der Zust�nde dieses NEAs festlegen */
	nea_states = nea_str[ss->nea_all].state_arr_count;

	/* Ersteinmal ein bi�chen initialisieren */
	if( init() < 0 )
		/* Fehler */
		return( -1 );

	/* Den Startzustand des NEA eintragen, der alle die beinhaltet, die am Zeilenanfang stehen d�rfen */
	insert_state( &move, nea_str[ss->nea_all].start_state );

	/* e_closure(0) */
	if( ( pos = e_clos( &move, ss ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Solange es einen unmarkierten Zustand in DSTATES gibt */
	while( ( ind < dstates_count ) && !dstates[ind].marked )
	{
		/* Zustand markieren */
		dstates[ind].marked++;

		/* Nun alle Eingabezeichen, move etc. */
		for( i = 0; i < all_char_count; i++ )
		{
			/* Die Zust�nde bestimmen, die aus e_cls einen Sprung�bergang mit i haben */
			if( !( move_it( ind, all_char[i] ) ) )
			{
				/* Nun die e_closure-Menge berechnen */
				if( ( pos = e_clos( &move, ss ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Zu Zustandsmenge in DTRAN eintragen */
				insert_dtran( ind, pos, all_char[i] );
			}
		}

		/* N�chsten Eintrag in DSTATES untersuchen */
		ind++;
	}

	/* Alles OK */
	return( 0 );
}


VOID insert_state( STATE_STR *s, LONG state )
{
	/* Hier wird ein Zustand in aufsteigender Reihenfolge in s eingetragen. */
	REG WORD	i = 0;

	/* Ist der Wert schon eingetragen worden ? */
	while( i < s->count )
	{
		/* Sind die Werte gleich ? */
		if( s->states[i++] == state )
			/* Ja, ende */
			return;
	}

	/* Wert eintragen */
	s->states[s->count++] = state;
}


WORD e_clos( STATE_STR *st, SS_STR *ss )
{
	/* Hier werden die Zust�nde in state_str eingetragen, die von st mit EPSILON-�berg�ngen aus erreichbar sind. */
	NEA_STATE	*nea;
	REG WORD	i;
	LONG		state;
	WORD		pos;

	/* Alle Zust�nde von st auf den Stack */
	copy( st, &e_stack );
	copy( st, &e_cls );

#ifdef DUMP

	for( i = 0; i < e_cls.count; i++ )
		fprintf( stderr, "\ne_closure(%ld, EPSILON) = %ld", e_cls.states[i], e_cls.states[i] );

#endif
		
	/* Solange Zust�nde auf dem Stack sind */
	while( ( state = pop_stack( &e_stack ) ) >= 0 )
	{
		/* Zeiger auf NEA_STATE holen */
		nea = &nea_state[state];

		/* Alle Zielzust�nde von states auf EPSILON-�berg�nge pr�fen */
		for( i = 0; i < nea->bra_count; i++ )
		{
			/* Hat state EPSILON-�berg�nge ? */
			if( nea->bra_str[i].eps )
			{
#ifdef DUMP
				fprintf( stderr, "\ne_closure(%ld, EPSILON) = %ld", state, nea->bra_str[i].bra );
#endif
				/* Ja, in e_cls und auf den Stack einf�gen */
				insert_state( &e_cls, nea->bra_str[i].bra );
				insert_state( &e_stack, nea->bra_str[i].bra );
			}
#ifdef DUMP
			else
				fprintf( stderr, "\ne_closure(%ld, EPSILON) = FALSE", state );
#endif
		}
	}

	/* Nun die Zustandsmenge in DSTATES eintragen */
	if( ( pos = insert_dstates( &e_cls, ss ) ) < 0 )
		/* Fehler */
		return( -1 );

	/* Position im Stack zur�cksetzen */
	e_stack.pos = 0;
	e_stack.count = 0;

	/* e_cls zur�cksetzen */
	e_cls.count = 0;

	/* Alles OK */
	return( pos );
}


VOID copy( STATE_STR *source, STATE_STR *dest )
{
	/* Alle Zust�nde von source nach dest. */
	REG LONG	i;

	/* Alle Eintr�ge kopieren */
	for( i = 0; i < source->count; i++ )
		/* Zust�nde kopieren */
		dest->states[i] = source->states[i];

	/* Anzahl der Eintr�ge merken */
	dest->count += source->count;
}


LONG pop_stack( STATE_STR *stack )
{
	/* Pop liefert einen Zustand aus stack zur�ck, oder -1, wenn es keinen mehr gab. */

	/* Gibt es an dieser Stelle einen Zustand ? */
	if( stack->pos < stack->count )
		/* Wert zur�ckliefern */
		return( stack->states[stack->pos++] );

	/* Keine Werte */
	return( -1 );
}


WORD move_it( WORD dstates_ind, WORD c )
{
	/* Hier werden die Zust�nde eingetragen, die in der Zustandsmenge dstates einen �bergang mit c haben. */
	NEA_STATE	*nea;
	REG WORD	i;
	WORD		pos = 0, flg = 0;

	/* Initialisieren */
	move.pos = move.count = 0;

	/* Solange ein Zustand da ist */
	while( pos++ < nea_count )
	{
		/* Ist das Bit gesetzt ? */
		if( dstates[dstates_ind].states[pos / CHAR_BIT] & ( 1 << ( pos % CHAR_BIT ) ) )
		{
			/* Zeiger auf NEA_STATE holen */
			nea = &nea_state[pos];

			/* Alle Zielzust�nde von states auf c-�berg�nge pr�fen */
			for( i = 0; i < nea->bra_count; i++ )
			{
				/* Hat state einen �bergang mit c ? */
				if( nea->bra_str[i].ccl_str.ccl[c / CHAR_BIT] & ( 1 << ( c % CHAR_BIT ) ) )
				{
#ifdef DUMP
					fprintf( stderr, "\nMOVE[%d][%c] = %d", pos, ( BYTE ) c, ( WORD ) nea->bra_str[i].bra );
#endif
					/* Ja, in move eintragen */
					insert_state( &move, nea->bra_str[i].bra );

					/* Flag setzen */
					flg++;
				}
			}
		}
	}

	/* Wert zur�ckliefern */
	return( flg > 0 ? 0 : 1 );
}


WORD insert_dstates( STATE_STR *s, SS_STR *ss )
{
	/* Hier wird die Zustandsmenge aus s in DSTATES gesucht. Ist sie nicht in DSTATES enthalten, wird sie hinzugef�gt. */
	REG WORD	i, j, pos;
	DSTATES		*d;

	/* Ist die Zustandsmenge aus s schon in DSTATES ? */
	if( ( pos = look_dstates( s ) ) < 0 )
	{
		/* Nein, Zustandsmenge aus s zu DSTATES hinzuf�gen */
		pos = dstates_count;

		/* DSTATES vergr��ern */
		if( ( dstates = ( DSTATES * ) realloc( ( VOID * ) dstates, ( dstates_count + 1 ) * sizeof( DSTATES ) ) ) == NULL )
			/* Zu wenig Speicher */
			return( -1 );

		/* Zeiger auf das neue DSTATES-Element holen */
		d = &dstates[dstates_count];

		/* d initialisieren */
		memset( ( VOID * ) d, 0, sizeof( DSTATES ) );

		/* Speicher f�r die Bit-Repr�sentation der Zustandsmenge holen */
		if( ( d->states = ( UBYTE * ) calloc( 1, ( ( nea_count / CHAR_BIT ) + 1 ) ) ) == NULL )
			/* Zu wenig Speicher */
			return( -1 );

#ifdef DUMP2
		fprintf( stderr, "\n\nDSTATES[%d] = ", dstates_count );
#endif

		/* Nun die einzelnen Zust�nde aus s in dstates setzen */
		for( i = 0; i < s->count; i++ )
		{
			/* Bits in dstates setzen */
			d->states[s->states[i] / CHAR_BIT] |= ( 1L << ( s->states[i] % CHAR_BIT ) );

#ifdef DUMP2
			fprintf( stderr, "%ld ", s->states[i] );
#endif

			/* Ist das ein Finit-Zustand ? */
			for( j = 0; j < ss->finit_states_count; j++ )
			{
				/* Ist s->states[i] in finit_states enthalten ? */
				if( s->states[i] == ss->finit_states[j] )
				{
					/* Ja, entsprechenden Flag in DSTATES setzen */
					d->type = 1;
					break;
				}
			}
		}

#ifdef DUMP2
		fprintf( stderr, "\n%d Zust�nde in der Zustandsmenge.", s->count );
#endif

		/* Die Anzahl der Zust�nde eintragen */
		d->count = s->count;

		/* N�chster Eintrag */
		dstates_count++;
	}

	/* Alles OK */
	return( pos );
}


WORD look_dstates( STATE_STR *s )
{
	/* Hier wird die Zustandsmenge aus s in DSTATES gesucht. */
	REG WORD	i, pos;

	/* Alle Eintr�ge durchgehen */
	for( i = 0; i < dstates_count; i++ )
	{
		/* Ist die Anzahl der Eintr�ge bei beiden gleich ? */
		if( dstates[i].count == s->count )
		{
			/* Ja, nun die einzelnen Zust�nde vergleichen */
			for( pos = 0; pos < s->count; pos++ )
			{
				/* Zust�nde gleich ? */
				if( !( dstates[i].states[s->states[pos] / CHAR_BIT] & ( 1L << ( s->states[pos] % CHAR_BIT ) ) ) )
					/* Nein */
					break;
			}

			/* Waren die Zust�nde gleich ? */
			if( pos == s->count )
				/* Die Zust�nde sind gleich */
				return( i );
		}
	}

	/* Zust�nde sind nicht gleich */
	return( -1 );
}


VOID push_tab( VOID )
{
	/* 08/15-Ausgabe f�r die Scanner-Tabelle */
	REG WORD	i, j, k;

	for( i = 0; i < dstates_count; i++ )
	{
		for( j = 0; j < dstates[i].dtran_count; j++ )
		{
			for( k = 0; k < all_char_count; k++ )
			{
				if( dstates[i].dtran[j].ccl_str.ccl[all_char[k] / CHAR_BIT] & ( 1 << ( all_char[k] % CHAR_BIT ) ) )
				{
					fprintf( stderr, "\nZustand %d: Zeichen %c, Ziel %d", i, ( BYTE ) all_char[k], dstates[i].dtran[j].dstates_index );

					if( dstates[i].type == FINIT )
						fprintf( stderr, "\tFinit-State" );
				}
			}
		}
	}

	for( i = 0; i < dstates_count; i++ )
	{
		if( dstates[i].type == FINIT )
			fprintf( stderr, "\n%d: Finit-State", i );
	}
}


WORD insert_dtran( WORD from, WORD to, WORD c )
{
	/* Hier wird ein Eintrag in der Scanner-Tabelle vorgenommen. */
	REG	WORD	i;
	DSTATES		*d;

#ifdef DUMP

	fprintf( stderr, "\nDTRAN[%d][%c] = %d", from, ( BYTE ) c, to );

#endif

	/* Gucken, ob die Zustandsmenge schon in DTRAN eingetragen ist */
	for( i = 0; i < dstates[from].dtran_count; i++ )
	{
		/* Zustandsmengen vergleichen */
		if( to == dstates[from].dtran[i].dstates_index )
		{
			/* Ja, nur das Zeichen hinzuf�gen */
			dstates[from].dtran[i].ccl_str.ccl[c / CHAR_BIT] |= ( 1 << ( c % CHAR_BIT ) );

			/* Routine verlassen */
			return( 0 );
		}
	}

	/* Neue DTRAN-Struktur in DSTATES anlegen */

	/* Zeiger auf DSTATES holen */
	d = &dstates[from];

	/* Speicher f�r eine weitere DTRAN-Struktur anfordern */
	if( ( d->dtran = ( DTRAN * ) realloc( ( VOID * ) d->dtran, ( d->dtran_count + 1 ) * sizeof( DTRAN ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( -1 );

	/* Initialisieren */
	memset( ( VOID * ) &d->dtran[d->dtran_count], 0, sizeof( DTRAN ) );

	/* Werte eintragen */
	d->dtran[d->dtran_count].dstates_index = to;
	dstates[from].dtran[d->dtran_count++].ccl_str.ccl[c / CHAR_BIT] |= ( 1 << ( c % CHAR_BIT ) );

	/* Alles OK */
	return( 0 );
}


VOID output_dstates( VOID )
{
	/*
		Hier werden die DSTATES-Strukturen ausgegeben.
		DEGUB
	*/
	REG WORD	i, j, c;

	/* Absatz */
	fprintf( stderr, "\n\n\n***********\n* DSTATES *\n***********\n" );

	/* Alle DSTATES-Strukturen */
	for( i = 0; i < dstates_count; i++ )
	{
		/* Absatz */
		fprintf( stderr, "\n" );

		/* Alle DTRAN-Strjturen */
		for( j = 0; j < dstates[i].dtran_count; j++ )
		{
			/* Alle Zeichen */
			for( c = 0; c < 255; c++ )
			{
				/* Ist das Zeichen gesetzt ? */
				if( dstates[i].dtran[j].ccl_str.ccl[c / CHAR_BIT] & ( 1 << ( c % CHAR_BIT ) ) )
					fprintf( stderr, "\nDSTATES[%d,%c] = %d", i, ( BYTE ) c, dstates[i].dtran[j].dstates_index );
			}
		}
	}
}