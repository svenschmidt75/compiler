/*
	Projekt		:	LR-Parser-Generator
	Modul		:	FOL_FIR.C
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
WORD insert_follow( LONG term, FIRST_STR *nterm_follow );
WORD insert_epsilon( NTERM_FIRST *nterm_first_str );
LONG get_nterm_first_index( LONG nterm_index );
WORD get_right( LONG nterm_index, LONG nterm );
WORD gen_follow( LONG nterm_index );
WORD gen_first( LONG nterm_index );
WORD insert_first( LONG term );
WORD generate_follow( VOID );
WORD generate_first( VOID );



/*****************/
/* Deklarationen */
/*****************/


/* FÅr die AbhÑngigkeiten der FOLLOW-Elemente */

typedef struct
{
	/* Da sollen die FOLLOW-Elemente hin. */
	LONG	dest;

	/* Von hier kommen sie. */
	LONG	source;

}F_STR;


typedef struct
{
	/* Zeiger auf die NTERM_FIRST_STR des Nichtterminals */
	NTERM_FIRST		*nterm_first_str;

}STACK;



/* Debug-Version erzeugen */
#undef PROT


/****************/
/* Definitionen */
/****************/


/* Globaler ZÑhler fÅr die Generierung der FOLLOW-Mengen */
LONG		follow_flag;

/* Zeiger auf die F_STR-Struktur, die die  AbhÑngigkeiten beinhaltet */
F_STR		*f_str;

/* Beinhaltet die AbhÑngigkeiten in FOLLOW */
LONG		f_str_count = 0;

/* Stack beschleunigt die gen_first-Routine */
STACK		stack[100];

/* Stack-ZÑhler */
LONG		stack_count = 0;



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





WORD generate_first( VOID )
{
	/* Hier wird die FIRST-Menge zu allen Produktionen gebildet. */
	REG	ULONG	i, j;

	/* Meldung ausgeben */
	fprintf( stderr, "\nGeneriere FIRST-Mengen..." );

	/* Flags gegen Endlosschleife */
	for( j = 0; j <= nterm_count; j++ )
		nterm_str[j]->post_index = 0;

	/* Die FIRST-Menge zu allen Produktionen erzeugen. */
	for( i = 0; i < nterm_first_count; i++ )
	{
		/* FIRST-Menge erzeugen */
		if( gen_first( nterm_first[i]->nterm_index ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Alles OK */
	return( 0 );
}


WORD gen_first( LONG nterm_index )
{
	/* Hier wird die FIRST-Menge zu nterm_index gebildet. */
	NTERM_STR	*nterm;
	WORD		state, e_state = 0, ri_count;

	/* Waren wir hier schon ? */
	if( nterm_str[nterm_index]->post_index++ )
		/* Ja, zurÅck */
		return( nterm_first[get_nterm_first_index( nterm_index )]->first.epsilon > 0 ? E_ON : E_OFF );

	/* NTERM_FIRST auf den STACK packen */
	stack[stack_count++].nterm_first_str = nterm_first[get_nterm_first_index( nterm_index )];

	do
	{
		/* Position auf der rechten Seite der Produktion */
		ri_count = 0;

		do
		{
			/* Zeiger auf NTERM_STR holen */
			nterm = nterm_str[nterm_index];

			if( !( hash_tab[nterm->ri[ri_count]].term_str ) && ( nterm->ri[ri_count] == nterm->hash_tab_index ) )
				/* Die rechte Seite untersuchen */
				state = gen_first( nterm_index + 1 );

			else
				/* Die rechte Seite untersuchen */
				state = get_right( nterm_index, nterm->ri[ri_count] );

			/* Fehler ? */
			if( state < 0 )
				/* Fehler */
				return( -1 );

		}while( ( ++ri_count < nterm->anz_ri ) && ( state == E_ON ) );

		/* Leitete alles auf der rechten Seite EPSILON her ? */
		if( ( ri_count == nterm->anz_ri ) && ( state == E_ON ) )
		{
			/* Ja, EPSILON nur in der aktuellen NTERM_FIRST-Struktur eintragen */

			/* EPSILON in die NTERM_FIRST-Struktur eintragen */
			if( insert_epsilon( nterm_first[get_nterm_first_index( nterm_index )] ) < 0 )
				/* Fehler */
				return( -1 );

			/* EPSILON */
			e_state = E_ON;
		}

	}while( ( ++nterm_index <= nterm_count ) && ( nterm_str[nterm_index]->alt ) );

	/* Brauchen wir nicht mehr */
	stack_count--;

	/* Alles OK */
	return( e_state );
}


WORD get_right( LONG nterm_index, LONG nterm )
{
	/*
		Hier wird die rechte Seite der Produktion untersucht.
		nterm_index = Index der Produktion
		nterm		= Element der rechten Seite von nterm_index
	*/

	/* Ist nterm EPSILON ? */
	if( nterm == epsilon )
	{
		/* Ja, EPSILON nur in die aktuelle NTERM_FIRST-Struktur eintragen */

		/* EPSILON in die NTERM_FIRST-Struktur eintragen */
		if( insert_epsilon( nterm_first[get_nterm_first_index( nterm_index )] ) < 0 )
			/* Fehler */
			return( -1 );

		/* EPSILON */
		return( E_ON );
	}

	/* Ist nterm ein Terminal ? */
	else if( hash_tab[nterm].term_str )
		/* Ja, in die NTERM_FIRST-Strukturen eintragen */
		return( insert_first( nterm ) );

	/* FIRST-Menge zu nterm bilden */
	return( gen_first( hash_tab[nterm].nterm_index ) );
}


WORD insert_first( LONG term )
{
	/* Hier wird ein Terminal in die NTERM_FIRST-Strukturen aufgenommen. */
	REG ULONG	i, j;
	FIRST_STR	*first;

	/* Das Terminal in allen NTERM_FIRST-Strukturen, die im STACK liegen, eintragen */
	for( i = 0; i < stack_count; i++ )
	{
		/* Zeiger auf FIRST-Struktur holen */
		first = &stack[i].nterm_first_str->first;

		/* Ist das Terminal schon eingetragen worden ? */
		for( j = 0; j < first->anz_elem; j++ )
		{
			/* Ist es das einzutragende Terminal ? */
			if( first->term_arr[j] == term )
				/* Terminal ist schon eingetragen worden */
				break;
		}

		/* Muss das Terminal eingetragen werden ? */
		if( j == first->anz_elem )
		{
			/* Speicher fÅr Terminal anfordern */
			if( ( first->term_arr = ( LONG * ) realloc( ( VOID * ) first->term_arr, ( first->anz_elem + 1 ) * sizeof( LONG ) ) ) == NULL )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Terminal in term_arr eintragen */
			first->term_arr[first->anz_elem++] = term;
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_epsilon( NTERM_FIRST *nterm_first_str )
{
	/* Hier wird EPSILON in die NTERM_FIRST-Struktur aufgenommen. */
	REG ULONG	i;
	FIRST_STR	*first = &nterm_first_str->first;

	/* Ist EPSILON schon eingetragen worden ? */
	for( i = 0; i < first->anz_elem; i++ )
	{
		/* Ist es EPSILON ? */
		if( first->term_arr[i] == epsilon )
			/* EPSILON ist schon eingetragen worden */
			return( 0 );
	}

	/* Speicher fÅr Terminal anfordern */
	if( ( first->term_arr = ( LONG * ) realloc( ( VOID * ) first->term_arr, ( first->anz_elem + 1 ) * sizeof( LONG ) ) ) == NULL )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Terminal in term_arr eintragen */
	first->term_arr[first->anz_elem++] = epsilon;

	/* Produktion leitet EPSILON her */
	nterm_first_str->first.epsilon++;

	/* Alles OK */
	return( 0 );
}


LONG get_nterm_first_index( LONG nterm_index )
{
	/*
		Hier wird die Position des nterm_index in der NTERM_FIRST-Struktur
		bestimmt.
	*/
	REG LONG	i;

	/* Ist es die Produktion ? */
	while( nterm_str[nterm_index]->alt )
		/* Nein */
		nterm_index--;

	/* Alle EintrÑge in der NTERM_FIRST-Struktur */
	for( i = 0; i < nterm_first_count; i++ )
	{
		/* Ist es der gesuchte Eintrag ? */
		if( nterm_first[i]->nterm_index == nterm_index )
			/* Ja */
			return( i );
	}

	/* Nicht gefunden */
	return( -1 );
}


WORD generate_follow( VOID )
{
	/* Hier werden die FOLLOW-Mengen der Produktionen gebildet. */
	FIRST_STR	*dest, *source;
	REG LONG	i = 0, j;

	/* Meldung ausgeben */
	fprintf( stderr, "\nGeneriere FOLLOW-Mengen..." );

	/* Flag */
	for( i = 0; i <= nterm_count; i++ )
		nterm_str[i]->post_index = 0;

	/* Alle NTERM_STRs */
	for( i = 0; i < prod_count; i++ )
	{
		/* FOLLOW-Menge zu diesem Nichtterminal bilden */
		if( gen_follow( prod_index_arr[i] ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Nun die AbhÑngigkeiten auflîsen */
	do
	{
		/* ZÑhler zurÅcksetzen */
		follow_flag = 0;

		/* Alle F_STR-EintrÑge durchgehen */
		for( j = 0; j < f_str_count; j++ )
		{
			/* Quelle holen */
			dest = &nterm_str[hash_tab[f_str[j].dest].nterm_index]->follow;

			/* Ziel holen */
			source = &nterm_str[hash_tab[f_str[j].source].nterm_index]->follow;

			/* Elemente Åbertragen */
			for( i = 0; i < source->anz_elem; i++ )
			{
				/* Element in nterm eintragen */
				if( insert_follow( source->term_arr[i], dest ) < 0 )
					/* Fehler */
					return( -1 );
			}
		}

	}while( follow_flag );

	/* Speicher wieder freigeben */
	free( ( VOID * ) f_str );

	/* Alles OK */
	return( 0 );
}


WORD gen_follow( LONG nterm_index )
{
	/* Hier wird die FOLLOW-Menge zu einer Produktion gebildet. */
	REG ULONG	i, j, k, temp_j;
	LONG		term[1], *term_arr, anz_elem;
	WORD		e_state;
	NTERM_STR	*nterm = nterm_str[nterm_index], *nterm_ri;

	/* Wurde die FOLLOW-Menge schon gebildet ? */
	if( nterm_str[nterm_index]->post_index++ > 0 )
		/* Ja */
		return( 0 );

	/* Protokollmeldung ausgeben ? */
	#ifdef PROT

		/* Meldung ausgeben */
		fprintf( file, "\n\n\nFOLLOW( %s )\n", hash_tab[nterm->hash_tab_index].name );

	#endif

	/* Alle Produktionen bearbeiten */
	for( i = 0; i <= nterm_count; i++ )
	{
		/*
			Das Nichtterminal nterm auf der rechten Seite jeder Produktion
			suchen.
		*/

		/* Flag fÅr EPSILON zurÅcksetzen */
		e_state = 0;

		/* Zeiger auf nterm_str[i] holen */
		nterm_ri = nterm_str[i];

		/* Alle Elemente auf der rechten Seite prÅfen */
		for( j = 0; j < nterm_ri->anz_ri; j++ )
		{
			/* Ist ri[j] == nterm ? */
			if( nterm_ri->ri[j] == nterm->hash_tab_index )
			{
				/* j sichern */
				temp_j = j;

				/* Folgt hinter j noch ein Element ? */
				if( j < ( nterm_ri->anz_ri - 1 ) )
				{
					do
					{
						/* NÑchstes Element */
						j++;

						/* Ja, FIRST( j ) zu FOLLOW( nterm ) */
						if( hash_tab[nterm_ri->ri[j]].term_str )
						{
							/* j ist ein Terminal */

							/* Ist es EPSILON ? */
							e_state = nterm_ri->ri[j] == epsilon ? E_ON : E_OFF;

							/* Anzahl der zu kopierenden Elemente */
							anz_elem = 1;

							/* Terminal in Terminal-Array eintragen */
							term[0] = nterm_ri->ri[j];

							/* Zeiger auf Terminal-Array */
							term_arr = term;

							/* Protokollmeldung ausgeben ? */
							#ifdef PROT

								/* Meldung ausgeben */
								fprintf( file, "\n\nFÅge FIRST( %s ) zu FOLLOW( %s ) hinzu.", hash_tab[term[0]].name, hash_tab[nterm->hash_tab_index].name );

							#endif
						}

						else
						{
							/* j ist ein Nichtterminal */

							/* Index in NTERM_FIRST-Struktur holen */
							k = get_nterm_first_index( hash_tab[nterm_ri->ri[j]].nterm_index );

							/* Anzahl der zu kopierenden Elemente */
							anz_elem = nterm_first[k]->first.anz_elem;

							/* Zeiger auf Terminal-Array */
							term_arr = nterm_first[k]->first.term_arr;

							/* Ist EPSILON dabei ? */
							e_state = nterm_first[k]->first.epsilon == E_OFF ? E_OFF : E_ON;

							/* Protokollmeldung ausgeben ? */
							#ifdef PROT

								/* Meldung ausgeben */
								fprintf( file, "\n\nFÅge FIRST( %s ) zu FOLLOW( %s ) hinzu.", hash_tab[nterm_ri->ri[j]].name, hash_tab[nterm->hash_tab_index].name );

							#endif
						}

						/* Elemente von Produktion nach nterm Åbernehmen */
						for( k = 0; k < anz_elem; k++ )
						{
							/* Element in nterm eintragen */
							if( insert_follow( term_arr[k], &nterm->follow ) < 0 )
								/* Fehler */
								return( -1 );
						}

						/* Protokollmeldung ausgeben ? */
						#ifdef PROT

							/* Die Produktion mit ausgeben */

							/* Produktion ausgeben */
							fprintf( file, "\n%s\n\t\t->\t", hash_tab[nterm_ri->hash_tab_index].name );

							/* Alle Elemente auf der rechten Seite */
							for( k = 0; k < nterm_ri->anz_ri; k++ )
							{
								/* Ist es ein Terminal ? */
								if( hash_tab[nterm_ri->ri[k]].term_str )
									/* Ja, ist ein Terminal */
									fprintf( file, " \'%s\'", hash_tab[nterm_ri->ri[k]].name );

								else
									/* Ist ein Nichtterminal */
									fprintf( file, " %s", hash_tab[nterm_ri->ri[k]].name );
							}

							/* Wurde EPSILON hergeleitet ? */
							if( e_state == E_ON )
								/* Entsprechende Meldung ausgeben */
								fprintf( file, "\n\n%s => EPSILON her, deshalb auch", hash_tab[nterm_ri->ri[j]].name );

						#endif

						/*
							Solange EPSILON hergeleitet wird, und auf der
							rechten Seite noch ein Element folgt.
						*/
						
					}while( ( e_state == E_ON ) && ( j < ( nterm_ri->anz_ri - 1 ) ) );
				}

				/* War j => EPSILON, oder war hinter j doch kein Element ? */
				if( ( e_state == E_ON ) || ( temp_j == ( nterm_ri->anz_ri - 1 ) ) )
				{
					/* Die AbhÑngigkeiten werden nachher aufgelîst. */

					/* Protokollmeldung ausgeben ? */
					#ifdef PROT

						/* Wurde EPSILON hergeleitet ? */
						if( e_state == E_ON )
							/* EPSILON wurde hergeleitet */
							fprintf( file, "\n\nFÅge FOLLOW( %s ) zu FOLLOW( %s => EPSILON ) hinzu.", hash_tab[nterm_ri->hash_tab_index].name, hash_tab[nterm_ri->ri[j]].name );

						else
							/* Es gibt kein Folgeelement */
							fprintf( file, "\n\nFÅge FOLLOW( %s ) zu FOLLOW( %s  / kein Folgeelement ) hinzu.", hash_tab[nterm_ri->hash_tab_index].name, hash_tab[nterm->hash_tab_index].name );

						/* Produktion ausgeben */
						fprintf( file, "\n%s\n\t\t->\t", hash_tab[nterm_ri->hash_tab_index].name );

						/* Alle Elemente auf der rechten Seite */
						for( k = 0; k < nterm_ri->anz_ri; k++ )
						{
							/* Ist es ein Terminal ? */
							if( hash_tab[nterm_ri->ri[k]].term_str )
								/* Ja, ist ein Terminal */
								fprintf( file, " \'%s\'", hash_tab[nterm_ri->ri[k]].name );

							else
								/* Ist ein Nichtterminal */
								fprintf( file, " %s", hash_tab[nterm_ri->ri[k]].name );
						}

					#endif

					/* Speicher fÅr neuen Struktureintrag holen */
					if( ( f_str = ( F_STR * ) realloc( ( VOID * ) f_str, ( f_str_count + 1 ) * sizeof( F_STR ) ) ) == NULL )
						/* Fehler */
						return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

					/* Ziel merken */
					f_str[f_str_count].dest = nterm->hash_tab_index;

					/* Quelle merken */
					f_str[f_str_count++].source = nterm_ri->hash_tab_index;
				}

				/* j wiederherstellen */
				j = temp_j;
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_follow( LONG term, FIRST_STR *nterm_follow )
{
	/* Hier wird ein Terminal in die nterm_follow aufgenommen. */
	REG ULONG	i;

	/* Ist es EPSILON ? */
	if( term != epsilon )
	{
		/* Ist das Terminal schon eingetragen worden ? */
		for( i = 0; i < nterm_follow->anz_elem; i++ )
		{
			/* Ist es das einzutragende Terminal ? */
			if( nterm_follow->term_arr[i] == term )
				/* Terminal ist schon eingetragen worden */
				return( 0 );
		}

		/* Speicher fÅr Terminal anfordern */
		if( ( nterm_follow->term_arr = ( LONG * ) realloc( ( VOID * ) nterm_follow->term_arr, ( nterm_follow->anz_elem + 1 ) * sizeof( LONG ) ) ) == NULL )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Terminal in term_arr eintragen */
		nterm_follow->term_arr[nterm_follow->anz_elem++] = term;

		/* Es wurde ein Element hinzugefÅgt */
		follow_flag++;
	}

	/* Alles OK */
	return( 0 );
}