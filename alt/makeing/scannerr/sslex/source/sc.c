/*
	Projekt		:	Scanner-Generator
	Modul		:	SC.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	20.03.1998
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/





/************/
/* Includes */
/************/

/* Include-Datei des Scanner-Generators */
#include "sslex.h"
#include "sslexpar.h"



/**************/
/* Prototypen */
/**************/

WORD insert_finit_state( SS_STR *ss, LONG finit_state );
VOID output_sc( WORD sc_ind );



/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/* Zeiger mit den Startsymbolen */
SS_STR			*ss_str = 0L;

/* Zaehler fuer SS_STR */
LONG			ss_str_count = 0;


/* Vordefinierte NEAen */
PREDEF_NEA_STR	*predef_nea_str = 0L;

/* ZÑhler */
LONG			predef_nea_str_count = 0;



/***********/
/* Imports */
/***********/

MGLOBAL NEA_STATE	*nea_state;
MGLOBAL NEA_STR		*nea_str;
MGLOBAL LONG		most_nea_states;
MGLOBAL LONG		nea_states;
MGLOBAL LONG		base;
MGLOBAL LONG		nea_index;
MGLOBAL LONG		nea_count;
MGLOBAL LONG		zeile;
MGLOBAL BYTE		*grammar;
MGLOBAL BYTE		string[];
MGLOBAL WORD		insert_ssc;



WORD do_sc( WORD type )
{
	/* Hier werden die inclusive-/exclusive-Startconditionen eingetragen. */
	WORD	state = 0;

	/* Die Bereichsdeklaration lîschen ( %incl_sc ) */
	grammar += 9;

	/* Unsinn lîschen */
	delete();

	/* { lîschen */
	grammar++;

	/* Unsinn lîschen */
	delete();

	/* Solange etwas da ist */
	while( !( state ) && ( *grammar != '}' ) )
	{
		/* Wort extrahieren */
		extract_word();

		/* Startcondition eintragen */
		if( ( insert_sc( string, type ) ) < 0 )
			/* Fehler */
			return( -1 );

		/* Unsinn lîschen */
		state = delete();
	}

	/* } lîschen */
	grammar++;

	/* Alles OK */
	return( 0 );
}


LONG insert_sc( BYTE s_c[], WORD type )
{
	/* Hier wird eine neue Startcondition erzeugt. */
	REG LONG	i;
	SS_STR		*ss;

	/* Wurde die SC schon eingetragen ? */
	for( i = 0; i < ss_str_count; i++ )
	{
		/* Vergleichen */
		if( !strcmp( ss_str[i].start_sym, s_c ) )
		{
			/* Ja, Fehlermeldung */
			fprintf( stderr, "\nZeile %lu: Startcondition %s wurde schon eingetragen !", zeile, s_c );

			/* Raus hier */
			return( 0 );
		}
	}

	/* Genug Speicher anfordern */
	if( ( ss_str = ( SS_STR * ) realloc( ( VOID * ) ss_str, sizeof( SS_STR ) * ( ss_str_count + 1 ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Zeiger auf SS_STR holen */
	ss = &ss_str[ss_str_count];

	/* Die neue SS_STR initialisieren */
	memset( ( VOID * ) ss, 0, sizeof( SS_STR ) );

	/* Speicher fÅr die SC holen */
	if( ( ss->start_sym = ( BYTE * ) malloc( strlen( s_c ) + 1 ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* String kopieren */
	strcpy( ( VOID * ) ss->start_sym, ( VOID * ) s_c );

	/* Typ eintragen */
	ss->ss_type = type;

	/* Alles OK */
	return( ss_str_count++ );
}


WORD insert_nea_sc( LONG nea_ind, LONG sc_ind, WORD type, WORD flag )
{
	/* Hier wird ein nea in einer oder in allen Start- oder Inclusive-Conditionen eingetragen. */
	REG LONG	i;

	if( insert_ssc )
		return( 0 );

	/* In allen eintragen ? */
	if( flag )
	{
		/* Ja, in allen */
		for( i = 0; i < ss_str_count; i++ )
		{
			/* Ist es eine SC vom gleichen Typ ? */
			if( ss_str[i].ss_type == type )
			{
				/* Ja, nea_ind eintragen */
				if( ( ss_str[i].ss_nea = ( LONG * ) realloc( ( VOID * ) ss_str[i].ss_nea, sizeof( LONG ) * ( ss_str[i].ss_nea_count + 1 ) ) ) == NULL )
					/* Zu wenig Speicher */
					return( output_error( NO_MEMORY ) );

				/* NEA eintragen */
				ss_str[i].ss_nea[ss_str[i].ss_nea_count++] = nea_ind;
			}
		}
	}

	else
	{
		/* Nein, in einer */
		if( ( ss_str[sc_ind].ss_nea = ( LONG * ) realloc( ( VOID * ) ss_str[sc_ind].ss_nea, sizeof( LONG ) * ( ss_str[sc_ind].ss_nea_count + 1 ) ) ) == NULL )
			/* Zu wenig Speicher */
			return( output_error( NO_MEMORY ) );

		/* NEA eintragen */
		ss_str[sc_ind].ss_nea[ss_str[sc_ind].ss_nea_count++] = nea_ind;
	}
/*
	/* Nun den Finit-Zustand des NEAs eintragen */
	if( ( ss_str[i].finit_states = ( LONG * ) realloc( ( VOID * ) ss_str[i].finit_states, sizeof( LONG ) * ( ss_str[i].finit_states_count + 1 ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Finit-Zustand des NEA eintragen */
	ss_str[i].finit_states[ss_str[i].finit_states_count++] = nea_str[nea_ind].finit_state;
*/
	/* Alles OK */
	return( 0 );
}


LONG get_sc_ind( BYTE *s_c )
{
	/* Liefert den Index der SC zurÅck. */
	REG LONG	i;

	/* SC suchen */
	for( i = 0; i < ss_str_count; i++ )
	{
		/* Beide Strings gleich ? */
		if( !strcmp( ss_str[i].start_sym, s_c ) )
			/* Ja */
			return( i );
	}

	/* SC ist nicht definiert worden */
	return( -1 );
}


LONG get_predef_nea( BYTE *nea_string )
{
	/* Liefert den Index des vordefinierten NEA zurÅck. */
	REG LONG	i = 0;

	/* Alle EintrÑge in der PREDEF-Strutkur */
	for( i = 0; i < predef_nea_str_count; i++ )
	{
		/* Ist es der gesuchte NEA ? */
		if( !strcmp( predef_nea_str[i].name, nea_string ) )
			/* Ja */
			return( predef_nea_str[i].nea_ind );
	}

	/* NEA gibt es nicht ! */
	fprintf( stderr, "\nZeile %lu: %s wurde nicht definiert !", zeile, nea_string );

	/* Fehler */
	return( -1 );
}


WORD if_predef_nea( LONG nea_ind )
{
	/* 0, wenn der NEA vordefiniert ist, sonst < 0. */
	REG LONG	i = 0;

	/* Alle EintrÑge in der PREDEF-Strutkur */
	for( i = 0; i < predef_nea_str_count; i++ )
	{
		/* Ist es der gesuchte NEA ? */
		if( predef_nea_str[i].nea_ind == nea_ind )
			/* Ja */
			return( 0 );
	}

	/* NEA gibt es nicht ! */
	return( -1 );
}


LONG insert_predef_nea( BYTE *nea_string, LONG nea_ind )
{
	/* Hier wird ein vordefinierter NEA eingetragen. */
	PREDEF_NEA_STR	*pd_nea;
	REG LONG		i;

	/* Wurde der NEA schon eingetragen ? */
	for( i = 0; i < predef_nea_str_count; i++ )
	{
		/* Vergleichen */
		if( !strcmp( predef_nea_str[i].name, nea_string ) )
		{
			/* Ja, Fehlermeldung */
			fprintf( stderr, "\nZeile %lu: %s wurde schon eingetragen !", zeile, nea_string );

			/* Raus hier */
			return( 0 );
		}
	}

	/* Genug Speicher anfordern */
	if( ( predef_nea_str = ( PREDEF_NEA_STR * ) realloc( ( VOID * ) predef_nea_str, sizeof( predef_nea_str ) * ( predef_nea_str_count + 1 ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Zeiger auf PREDEF_NEA-Str holen */
	pd_nea = &predef_nea_str[predef_nea_str_count];

	/* Speicher fÅr den Namen holen */
	if( ( pd_nea->name = ( BYTE * ) malloc( strlen( nea_string ) + 1 ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* String kopieren */
	strcpy( ( VOID * ) pd_nea->name, ( VOID * ) nea_string );

	/* Index merken */
	pd_nea->nea_ind = nea_ind;

	/* Alles OK */
	return( predef_nea_str_count++ );
}


WORD finish_sc( VOID )
{
	/*
		Hier werden die einzelnen NEAen der SCs verodert.

		Alle NEAen, die nur am Zeilenanfang stehen dÅrfen sowie die, die Åberall auftreten dÅrfen,
		werden zusammengelegt.
		Die nur am Zeilenanfang stehen dÅrfen, extra.
	*/
	REG LONG	i, j;
	NEA_STR		*nea;
	SS_STR		*ss;
	LONG		nea_all, nea_nall, min, max, nmin, nmax;
	WORD		flg, anz_nall, anz_all;

	/* Alle StartConditionen */
	for( i = 0; i < ss_str_count; i++ )
	{
		/*
			Der erste NEA beinhaltet einen EPSILON-Sprung zu allen, die am Zeilenanfang stehen kînnen.
			Das sind alle normal-deklarierten und die, die ein ^ vor dem RE stehen haben.
		*/
		if( ( nea_all = init_nea() ) < 0 )
			/* Fehler */
			return( -1 );


		/*
			Der zweite NEA beinhaltet einen EPSILON-Sprung zu allen, auûer denen, die explizit am Zeilenanfang
			stehen mÅssen. Das sind alle normal-deklarierten.
		*/
		if( ( nea_nall = init_nea() ) < 0 )
			/* Fehler */
			return( -1 );


		/* Gibt an, ob der nea_nall gebraucht wurde */
		flg = 0;

		/* Anzahl der ZustÑnde */
		anz_all = anz_nall = 0;

		/* Grîûten und kleinsten Zustand festhalten */
		max = nea_str[nea_all].start_state;
		nmax = nea_str[nea_nall].start_state;
		min = nea_count;
		nmin = nea_count;

		/* Zeiger auf die SS_STR-Struktur holen */
		ss = &ss_str[i];

		/* Alle NEAen in der StartCondition */
		for( j = 0; j < ss->ss_nea_count; j++ )
		{
			/* Zeiger auf den NEA holen */
			nea = &nea_str[ss->ss_nea[j]];

			/* Den Finit-State in die SC eintragen */
			if( insert_finit_state( ss, nea->finit_state ) < 0 )
				/* Fehler */
				return( -1 );

			/* Den NEA in nea_all eintragen, da er Åberall stehen darf */
			if( nea_bra( nea_all, FINIT_STATE, nea->start_state, 0L, 0, 0, _EPSILON ) < 0 )
				/* Fehler */
				return( -1 );

			/* Darf der NEA ausschlieûlich am Zeilenanfang stehen ? */
			if( nea->beg_line )
			{
				/* Ja, in nea_nall eintragen */

				/* ZustandsÅbergang vom Finit-State des nea_all zum Start-State des hiesigen NEAs */
				if( nea_bra( nea_nall, FINIT_STATE, nea->start_state, 0L, 0, 0, _EPSILON ) < 0 )
					/* Fehler */
					return( -1 );

				/* Flag setzen */
				flg++;

				/* Den kleinsten Zustand im NEA bestimmen */
				nmin = MIN( nmin, nea->least_state );
				nmax = MAX( nmax, nea->biggest_state );

				/* Anzahl der ZustÑnde festhalten */
				anz_nall += nea_str[ss->ss_nea[j]].state_arr_count;
			}

			/* Anzahl der ZustÑnde festhalten */
			anz_all += nea_str[ss->ss_nea[j]].state_arr_count;

			/* Den kleinsten Zustand im NEA bestimmen */
			min = MIN( min, nea->least_state );
			max = MAX( max, nea->biggest_state );
		}

		/* Grîûten und kleinsten Zustand aller NEAen eintragen */
		nea_str[nea_all].biggest_state = max;
		nea_str[nea_all].least_state = min;
		nea_str[nea_nall].biggest_state = nmax;
		nea_str[nea_nall].least_state = nmin;

		/* Die Anzahl der ZustÑnde festhalten */
		nea_str[nea_all].state_arr_count += anz_all;
		nea_str[nea_nall].state_arr_count += anz_nall;

		/* Die Indizies der NEAen merken */
		ss->nea_all = nea_all;

		/* Wurde der nea_nall gebraucht ? */
		if( flg )
		{
			/* Ja, eintragen */
			ss->nea_nall = nea_nall;
			flg = 0;
		}

		else
		{
			/* Nein */
			ss->nea_nall = -1;

			/* Speicher des NEA wieder freigeben */
			free_nea( nea_nall );
		}

		/* SC ausgeben */
		output_sc( ( WORD ) i );
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_finit_state( SS_STR *ss, LONG finit_state )
{
	/* Hier wird ein Finit-State in eine SC eingetragen. */

	/* Speicher anfordern */
	if( ( ss->finit_states = ( LONG * ) realloc( ( VOID * ) ss->finit_states, sizeof( LONG ) * ( ss->finit_states_count + 1 ) ) ) == NULL )
		/* Zu wenig Speicher */
		return( output_error( NO_MEMORY ) );

	/* Finit-State eintragen */
	ss->finit_states[ss->finit_states_count++] = finit_state;

	/* Alles OK */
	return( 0 );
}


WORD do_predef( VOID )
{
	/* Hier werden die vordefinierten NEAen ausgewertet. */

	/* Symbol lîschen */
	grammar += 11;

	/* UnnÅtzes lîschen */
	delete();

	/* { lîschen */
	grammar++;

	do
	{
		/* UnnÅtzes lîschen */
		delete();

		/* Namen des NEA extrahieren */
		extract_word();

		/* UnnÅtzes lîschen */
		delete();

		/* RE parsen */
		if( parse() )
			/* Es sind Fehler aufgetreten */
			return( -1 );

		/* Vordefinierten NEA eintragen */
		if( ( insert_predef_nea( string, nea_index ) ) < 0 )
			/* Fehler */
			return( -1 );

	}while( *grammar != '}' );

	/* Das } lîschen */
	grammar++;

	/* Alles OK */
	return( 0 );
}


VOID output_sc( WORD sc_ind )
{
	/* Hier wird zu Debug-Zwecken eine Startcondition und ihre NEAen ausgegeben. */
	SS_STR		*ss = &ss_str[sc_ind];
	REG WORD	i;

	/* SC ausgeben */
	fprintf( stderr, "\n\nStartcondition       : %s, %s", ss->start_sym, ss->ss_type == INCLUSIVE ? "Inclusive" : "Exclusive" );

	/* Anzahl der NEAen */
	fprintf( stderr, "\nAnzahl der NEAen     : %d", ss->ss_nea_count );

	/* Startzustand des NEAs mit allen REs */
	fprintf( stderr, "\nStartzustand nea_all : %d", nea_str[ss->nea_all].start_state );

	/* Startzustand des NEAs ohne die, die nur am Zeilenanfang stehen dÅrfen */
	fprintf( stderr, "\nStartzustand nea_nall: %d", nea_str[ss->nea_nall].start_state );

	/* Finit-ZustÑnde */
	for( i = 0; i < ss->finit_states_count; i++ )
		fprintf( stderr, "\nFinit-Zustand %d      : %d", i, ss->finit_states[i] );

	/* Ausgabe des NEA, in dem alle REs am Zeilenanfang stehen dÅrfen. */
	fprintf( stderr, "\n\nNEA mit allen REs." );
}