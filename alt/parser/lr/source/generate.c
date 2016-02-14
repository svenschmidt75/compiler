/*
	Projekt		:	LR-Parser-Generator V1.00û
	Modul		:	GENERATE.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/



/************/
/* Includes */
/************/

#include <portab.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Include-Datei des Parser-Generators */
#include "LR.H"



/**************/
/* Prototypen */
/**************/

WORD get_right( LONG temp_nterm_index, LONG nterm_index, ULONG nterm, NTERM_FIRST *nterm_first_str );
WORD nterm_equal( LONG temp_nterm_index, LONG nterm_index, NTERM_FIRST *nterm_first_str );
WORD insert_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos );
LONG find_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos );
VOID insert_reduce( LONG hulle, LONG term_index, LONG nterm_index );
WORD gen_first( LONG nterm_index, NTERM_FIRST *nterm_first_str );
WORD insert_error_index( HULLE_STR *hulle, LONG error_index );
WORD insert_first( LONG term, NTERM_FIRST *nterm_first_str );
WORD cmp_lr_tab( ULONG lr_tab1_index, ULONG lr_tab2_index );
WORD error( WORD error_number, ULONG para1, ULONG para2  );
WORD insert_follow( LONG term, FIRST_STR *nterm_follow );
LONG get_nterm_first_index( LONG nterm_index );
LONG do_goto( HULLE_STR *hulle, LONG term );
WORD gen_follow( LONG nterm_index );
WORD slr_hulle( HULLE_STR *hulle );
LONG cmp_hulle( HULLE_STR *hulle );
WORD lr_hulle( HULLE_STR *hulle );
WORD do_hulle( HULLE_STR *hulle );
LONG get_term_index( LONG term );
VOID set_empty_to_reduce( VOID );
WORD compress_lr_tab( VOID );
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


/* FÅr die AbhÑngigkeiten der FOLLOW-Elemente */

typedef struct
{
	/* Da sollen die FOLLOW-Elemente hin. */
	LONG	dest;

	/* Von hier kommen sie. */
	LONG	source;

}F_STR;



/* Debug-Version erzeugen */
#undef PROT



/****************/
/* Definitionen */
/****************/

/* Beinhaltet die HULLE_STRs */
HULLE_STR	**hulle_str = 0L;

/* Anzahl der HULLE_STRs */
LONG		hulle_count = -1;

/* LR-Tabelle */
LR_TAB		**lr_tab = 0L;

/* Sprung-Tabelle */
LONG		**jump_tab = 0L;

/* Ziel-Tabelle */
ULONG		*ziel_tab = 0L;

/* ZÑhler fÅr die neue Anzahl der LR_TAB-Strukturen */
ULONG		anz_cop_lr_tab;

/* Komprimierte LR-Tabelle */
LONG		*cop_lr_tab;

/* Beinhaltet die AbhÑngigkeiten in FOLLOW */
LONG		f_str_count = 0;

/* Globaler ZÑhler fÅr die Generierung der FOLLOW-Mengen */
LONG		follow_flag;

/* Zeiger auf die F_STR-Struktur, die die  AbhÑngigkeiten beinhaltet */
F_STR		*f_str;



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
MGLOBAL LONG		epsilon;
MGLOBAL LONG		accept;
MGLOBAL LONG		term_count;
MGLOBAL LONG		nterm_count;
MGLOBAL FILE		*file;
MGLOBAL LONG		error_struct_count;




WORD generate_first( VOID )
{
	/* Hier wird die FIRST-Menge zu allen Produktionen gebildet. */
	REG	ULONG	i, j;

	/* Die FIRST-Menge zu allen Produktionen erzeugen. */
	for( i = 0; i < nterm_first_count; i++ )
	{
		/* Flags gegen Endlosschleife */
		for( j = 0; j <= nterm_count; j++ )
			nterm_str[j]->post_index = 0;

		/* FIRST-Menge erzeugen */
		if( gen_first( nterm_first[i]->nterm_index, nterm_first[i] ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Alles OK */
	return( 0 );
}


WORD gen_first( LONG nterm_index, NTERM_FIRST *nterm_first_str )
{
	/* Hier wird die FIRST-Menge zu nterm_index gebildet. */
	NTERM_STR	*nterm;
	ULONG		ri_count;
	LONG		temp_nterm_index = nterm_index;
	WORD		state, e_state = 0;

	/* Wurde die FIRST-Menge schon gebildet ? */
	if( nterm_str[nterm_index]->post_index++ > 0 )
		/* Ja */
		return( 0 );

	do
	{
		/* Position auf der rechten Seite der Produktion */
		ri_count = 0;

		do
		{
			/* Zeiger auf NTERM_STR holen */
			nterm = nterm_str[nterm_index];

			/* Die rechte Seite untersuchen */
			state = get_right( temp_nterm_index, nterm_index, nterm->ri[ri_count], nterm_first_str );

			/* Fehler ? */
			if( state < 0 )
				/* Fehler */
				return( -1 );

		}while( ( ++ri_count < nterm->anz_ri ) && ( state == E_ON ) );

		/* Leitete alles auf der rechten Seite EPSILON her ? */
		if( ( ri_count == nterm->anz_ri ) && ( state == E_ON ) )
		{
			/* EPSILON in nterm_first_str eintragen ? */
			if( nterm_index == nterm_first_str->nterm_index )
			{
				/* Produktion leitet EPSILON her */
				nterm_first_str->first.epsilon++;

				/* EPSILON in nterm_first_str eintragen */
				if( insert_first( epsilon, nterm_first_str ) < 0 )
					/* Fehler */
					return( -1 );
			}

			/* EPSILON */
			e_state = E_ON;
		}

	}while( ( ++nterm_index <= nterm_count ) && ( nterm_str[nterm_index]->alt ) );

	/* Alles OK */
	return( e_state );
}


WORD get_right( LONG temp_nterm_index, LONG nterm_index, ULONG nterm, NTERM_FIRST *nterm_first_str )
{
	/*
		Hier wird die rechte Seite der Produktion untersucht.
		nterm_index = Index der Produktion
		nterm		= Element der rechten Seite von nterm_index
	*/

	/* Ist nterm EPSILON ? */
	if( nterm == epsilon )
	{
		/* Ja, in nterm_first_str eintragen ? */
		if( nterm_first_str->nterm_index == temp_nterm_index )
		{
			/* EPSILON ist enthalten */
			nterm_first_str->first.epsilon++;

			/* EPSILON in nterm_first_str eintragen */
			if( insert_first( epsilon, nterm_first_str ) < 0 )
				/* Fehler */
				return( -1 );
		}

		/* EPSILON */
		return( E_ON );
	}

	/* Ist nterm ein Terminal ? */
	else if( hash_tab[nterm].term_str )
		/* Ja, in nterm_first_str eintragen */
		return( insert_first( nterm, nterm_first_str ) );

	/* nterm ist ein Nichtterminal */
	else
	{
		/*
			Ist nterm = Produktion ?
			nterm	->	nterm xxxx
		*/

		if( nterm == nterm_str[nterm_index]->hash_tab_index )
			/* Ja */
			return( nterm_equal( temp_nterm_index, nterm_index, nterm_first_str ) );

		/* First-Menge zu nterm bilden */
		return( gen_first( hash_tab[nterm].nterm_index, nterm_first_str ) );
	}
}


WORD nterm_equal( LONG temp_nterm_index, LONG nterm_index, NTERM_FIRST *nterm_first_str )
{
	/*
		Wenn das erste Element auf der rechten Seite mit dem Namen der
		Produktion Åbereinstimmt, wird diese Routine aufgerufen.
	*/
	NTERM_STR	*nterm = nterm_str[nterm_index];
	WORD		state;

	if( nterm_first_str->first.epsilon )
	{
		/*
			Ja, FIRST-Menge zu dem zweiten Element zu nterm_index hinzu-
			fÅgen.
		*/

		/* Folgt Åberhaupt ein zweites Element ? */
		if( nterm->anz_ri > 1 )
			/* Ja, FIRST-Menge zum zweiten Element bilden */
			return( get_right( temp_nterm_index, nterm_index, nterm->ri[1], nterm_first_str ) );

		else
			/* Unsinnige Alternative */
			return( error( BAD_ALTERNATIVE, ( ULONG ) hash_tab[nterm_str[nterm_index]->hash_tab_index].name, ( nterm_index - nterm_first_str->nterm_index ) - 1 ) );
	}

	else
	{
		/* Mit der nÑchsten Alternative weitermachen */

		/* Gibt es Åberhaupt eine weitere Alternative ? */
		if( ( ( nterm_index + 1 ) <= nterm_count ) && ( nterm_str[nterm_index + 1]->alt ) )
		{
			/* FIRST-Menge bilden */
			state = gen_first( nterm_index + 1, nterm_first_str );

			/* Fehler ? */
			if( state < 0 )
				/* Fehler */
				return( -1 );

			/* Wurde EPSILON hergeleitet ? */
			if( state == E_ON )
			{
				/*
					Ja, FIRST-Menge zu dem zweiten Element zu nterm_index hinzu-
					fÅgen.
				*/

				/* Folgt Åberhaupt ein zweites Element ? */
				if( nterm->anz_ri > 1 )
					/* Ja, FIRST-Menge zum zweiten Element bilden */
					return( get_right( temp_nterm_index, nterm_index, nterm->ri[1], nterm_first_str ) );

				else
					/* Unsinnige Alternative */
					return( error( BAD_ALTERNATIVE, ( ULONG ) hash_tab[nterm_str[nterm_index]->hash_tab_index].name, ( nterm_index - nterm_first_str->nterm_index ) - 1 ) );
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD insert_first( LONG term, NTERM_FIRST *nterm_first_str )
{
	/* Hier wird ein Terminal in die nterm_first_str aufgenommen. */
	REG ULONG	i;
	FIRST_STR	*first = &nterm_first_str->first;

	/* Ist das Terminal schon eingetragen worden ? */
	for( i = 0; i < first->anz_elem; i++ )
	{
		/* Ist es das einzutragende Terminal ? */
		if( first->term_arr[i] == term )
			/* Terminal ist schon eingetragen worden */
			return( 0 );
	}

	/* Speicher fÅr Terminal anfordern */
	if( !( first->term_arr = ( LONG * ) realloc( ( VOID * ) first->term_arr, ( first->anz_elem + 1 ) * sizeof( LONG ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Terminal in term_arr eintragen */
	first->term_arr[first->anz_elem++] = term;

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

	/* Flag */
	for( i = 0; i <= nterm_count; i++ )
		nterm_str[i]->post_index = 0;

	/* Alle NTERM_STRs */
	for( i = 1; i <= nterm_count; )
	{
		/* FOLLOW-Menge zu diesem Nichtterminal bilden */
		if( gen_follow( i ) < 0 )
			/* Fehler */
			return( -1 );

		/* Die Alternativen Åberspringen */
		while( ( ++i <= nterm_count ) && ( nterm_str[i]->alt ) )
			;
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
					if( !( f_str = ( F_STR * ) realloc( ( VOID * ) f_str, ( f_str_count + 1 ) * sizeof( F_STR ) ) ) )
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
		if( !( nterm_follow->term_arr = ( LONG * ) realloc( ( VOID * ) nterm_follow->term_arr, ( nterm_follow->anz_elem + 1 ) * sizeof( LONG ) ) ) )
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
		if( lr_to_lalr() < 0 )
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
	FIRST_STR	*term;

	/* Ein weiterer Eintrag */
	hulle_count++;

	/* Speicher fÅr neuen HULLE_STR-Eintrag holen */
	if( !( hulle_str = ( HULLE_STR ** ) realloc( hulle_str, sizeof( HULLE_STR * ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr HULLE_STR holen */
	if( !( hulle_str[0] = ( HULLE_STR * ) calloc( 1, sizeof( HULLE_STR ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Speicher fÅr neuen ITEM_STR-Eintrag holen */
	if( !( hulle_str[0]->item_str = ( ITEM_STR * ) realloc( hulle_str[0]->item_str, sizeof( ITEM_STR ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Struktur initialisieren */
	memset( ( VOID * ) &hulle_str[0]->item_str[0], 0, sizeof( ITEM_STR ) );

	/* Name der Produktion */
	hulle_str[0]->nterm = nterm_str[0]->hash_tab_index;

	/* Index der HÅlle */
	hulle_str[0]->index = 0;

	/* Den Kern der HÅlle(I0) bilden */
	hulle_str[0]->anz_items = hulle_str[0]->anz_kitems = 1;

	/* Eintrag des Nichtterminals in der NTERM_STR */
	hulle_str[0]->item_str[0].nterm_index = 0;

	/* Position des Punktes */
	hulle_str[0]->item_str[0].pkt_pos = 0;

	/* ACCEPT nicht bei SLR-Parser einfÅgen */
	if( lr_info.type != SLR )
	{
		/* ACCEPT an das Item anhÑngen */

		/* Zeiger auf FIRST_STR-Struktur der HÅlle holen */
		term = &hulle_str[0]->item_str[0].term;

		/* Speicher fÅr ACCEPT anfordern */
		if( !( term->term_arr = ( LONG * ) malloc( sizeof( LONG ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* ACCEPT eintragen */
		term->term_arr[term->anz_elem++] = accept;
	}

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

	/* Soll ein SLR-Parser erzeugt werden ? */
	if( lr_info.type == SLR )
		/* Ja, Funktion HÅlle fÅr SLR-Parser aufrufen */
		return( slr_hulle( hulle ) );

	else
		/* Funktion HÅlle fÅr LR-Parser aufrufen */
		return( lr_hulle( hulle ) );
}


WORD slr_hulle( HULLE_STR *hulle )
{
	/* HÅllen-Funktion fÅr SLR-Parser. */
	ITEM_STR	*item;
	NTERM_STR	*nterm;
	REG ULONG	i;
	LONG		alt;

	/* Alle Items der HÅlle bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Zeiger auf NTERM_STR des Items holen */
		nterm = nterm_str[item->nterm_index];

		/* Ist der Punkt vor einem Nichtterminal ? */
		if( ( nterm->anz_ri > item->pkt_pos ) && ( !hash_tab[nterm->ri[item->pkt_pos]].term_str ) )
		{
			/* Alle Alternativen in die HÅlle eintragen */
			alt = hash_tab[nterm->ri[item->pkt_pos]].nterm_index;

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
	/* HÅllen-Funktion fÅr LR- und LALR-Parser. */
	FIRST_STR	first;
	ITEM_STR	*item;
	NTERM_STR	*nterm;
	REG ULONG	i, j;
	LONG		term[1], *term_arr, anz_elem, alt;
	WORD		e_state;

	/* Alle Items der HÅlle bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* FIRST_STR-Struktur initialisieren */
		first.anz_elem = 0;
		first.term_arr = 0L;

		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Zeiger auf NTERM_STR des Items holen */
		nterm = nterm_str[item->nterm_index];

		/* Ist der Punkt vor einem Nichtterminal ? */
		if( ( nterm->anz_ri > item->pkt_pos ) && ( !hash_tab[nterm->ri[item->pkt_pos]].term_str ) )
		{
			/* Folgt dahinter etwas ? */
			if( nterm->anz_ri > ( item->pkt_pos + 1 ) )
			{
				/* Ist es ein Terminal ? */
				if( hash_tab[nterm->ri[item->pkt_pos + 1]].term_str)
				{
					/* Ist ein Terminal */

					/* Ist es EPSILON ? */
					e_state = nterm->ri[item->pkt_pos] == epsilon ? E_ON : E_OFF;

					/* Anzahl der zu kopierenden Elemente */
					anz_elem = 1;

					/* Terminal in Terminal-Array eintragen */
					term[0] = nterm->ri[item->pkt_pos + 1];

					/* Zeiger auf Terminal-Array */
					term_arr = term;
				}

				else
				{
					/* Ist ein Nichtterminal */

					/* Index in NTERM_FIRST-Struktur holen */
					j = get_nterm_first_index( hash_tab[nterm->ri[item->pkt_pos + 1]].nterm_index );

					/* Anzahl der zu kopierenden Elemente */
					anz_elem = nterm_first[j]->first.anz_elem;

					/* Zeiger auf Terminal-Array */
					term_arr = nterm_first[j]->first.term_arr;

					/* Ist EPSILON dabei ? */
					e_state = nterm_first[j]->first.epsilon == E_OFF ? E_OFF : E_ON;
				}
			}

			/* Folgte kein Element bzw. war es EPSILON ? */
			if( ( nterm->anz_ri <= ( item->pkt_pos + 1 ) ) || ( e_state == E_ON ) )
			{
				/* Alle Terminale des Items in first eintragen */
				for( j = 0; j < item->term.anz_elem; j++ )
				{
					/* Speicher fÅr das Terminal anfordern */
					if( !( first.term_arr = ( LONG * ) realloc( ( VOID * ) first.term_arr, ( first.anz_elem + 1 ) * sizeof( LONG ) ) ) )
						/* Fehler */
						return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

					/* Terminal in term_arr eintragen */
					first.term_arr[first.anz_elem++] = item->term.term_arr[j];
				}

				/* Anzahl der Elemente */
				anz_elem = first.anz_elem;

				/* Zeiger auf Terminale */
				term_arr = first.term_arr;
			}

			/* Alle Alternativen in die HÅlle eintragen */
			alt = hash_tab[nterm->ri[item->pkt_pos]].nterm_index;

			do
			{
				/* Alle Terminale */
				for( j = 0; j < anz_elem; j++ )
				{
					/* Item in HÅlle eintragen */
					if( insert_item( hulle, alt, term_arr[j], 0 ) < 0 )
						/* Fehler */
						return( -1 );
				}

				/* Solange noch Alternativen da sind */
			}while( ( ++alt <= nterm_count ) && ( nterm_str[alt]->alt ) );

			/* Speicher der Terminale in FIRST_STR freigeben ? */
			if( first.anz_elem )
				/* Ja */
				free( ( VOID * ) first.term_arr );
		}
	}

	/* Alles OK */
	return( 0 );
}


LONG find_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos )
{
	/* Hier wird geprÅft, ob das Item in der HÅlle ist. */
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
			/* SLR-Parser ? */
			if( lr_info.type == SLR )
				/* Item ist schon in der HÅlle */
				return( -2 );

			/* LR/LALR-Parser */

			/* Alle Terminale des Items mit term vergleichen */
			for( j = 0; j < item->term.anz_elem; j++ )
			{
				/* Sind die Terminale gleich ? */
				if( item->term.term_arr[j] == term )
					/* Das Item ist schon in der HÅlle */
					return( -2 );
			}

			/* Das Item ist da, aber das Terminal stimmt nicht */
			return( i );
		}
	}

	/* Item war nicht in der HÅlle enthalten */
	return( -1 );
}


WORD insert_item( HULLE_STR *hulle, LONG nterm_index, LONG term, WORD pkt_pos )
{
	/* Hier wird ein Item in die HÅlle eingetragen. */
	FIRST_STR	*item_first;
	REG ULONG	anz_items = hulle->anz_items;
	LONG		i;

	/* Testen, ob das Item bereits in der HÅlle eingetragen ist */
	i = find_item( hulle, nterm_index, term, pkt_pos );

	/* Zeiger auf FIRST-Struktur des Itens */
	item_first = &hulle->item_str[i].term;

	/* War das Item in der HÅlle ? */
	if( ( i >= 0 ) && ( lr_info.type != SLR ) )
	{
		/* Das Item ja, aber ohne term */

		/* Speicher fÅr das Terminal anfordern */
		if( !( item_first->term_arr = ( LONG * ) realloc( ( VOID * ) item_first->term_arr, ( item_first->anz_elem + 1 ) * sizeof( LONG ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Terminal in term_arr eintragen */
		item_first->term_arr[item_first->anz_elem++] = term;
	}

	/* Es muû ein ganz neues Item erzeugt werden */
	else if( i == -1 )
	{
		/* Nein, ein ganz neues Item erzeugen */

		/* Speicher fÅr neuen ITEM_STR-Eintrag holen */
		if( !( hulle->item_str = ( ITEM_STR * ) realloc( hulle->item_str, ( anz_items + 1 ) * sizeof( ITEM_STR ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* Struktur initialisieren */
		memset( ( VOID * ) &hulle->item_str[anz_items], 0, sizeof( ITEM_STR ) );

		/* Zeiger auf FIRST-Struktur des neu erzeugten Itens */
		item_first = &hulle->item_str[anz_items].term;

		/* Terminal nicht bei SLR-Parser einfÅgen */
		if( lr_info.type != SLR )
		{
			/* Terminal bei LR/LALR einfÅgen */

			/* Speicher fÅr das Terminal anfordern */
			if( !( item_first->term_arr = ( LONG * ) realloc( ( VOID * ) item_first->term_arr, ( item_first->anz_elem + 1 ) * sizeof( LONG ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

			/* Terminal in term_arr eintragen */
			item_first->term_arr[item_first->anz_elem++] = term;
		}

		/* Eintrag des Nichtterminals in der NTERM_STR */
		hulle->item_str[anz_items].nterm_index = nterm_index;

		/* Position des Punktes */
		hulle->item_str[anz_items].pkt_pos = pkt_pos;

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
	ITEM_STR	*item;
	LONG		h, i, temp_hulle_count;

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
		/* Alle Items durchgehen */
		for( i = 0; i < hulle_str[h]->anz_items; i++ )
		{
			/* Zeiger auf das Item holen */
			item = &hulle_str[h]->item_str[i];

			/* Zeiger auf NTERM_STR des Items holen */
			nterm = nterm_str[item->nterm_index];

			/* Ist es ein Reduce-Item ? */
			if( nterm->anz_ri > item->pkt_pos )
			{
				/* Nein, Die Anzahl der HÅllen merken */
				temp_hulle_count = hulle_count;

				/* Die HÅlle ggf. erzeugen */
				if( do_goto( hulle_str[h], nterm->ri[item->pkt_pos] ) < 0 )
					/* Fehler */
					return( -1 );

				/* Wurde eine HÅlle erzeugt ? */
				if( temp_hulle_count < hulle_count )
				{
					/* Ja, Meldung ausgeben ? */
					if( !lr_info.analyze )
						/* Meldung ausgeben */
						fprintf( file, "\n%3lu = Erzeugte HÅlle( I%3lu, %35s )", hulle_count, h, hash_tab[nterm->ri[item->pkt_pos]].name );

					/* Nichtterminal merken */
					hulle_str[hulle_count]->nterm = nterm->ri[item->pkt_pos];

					/* Index der HÅlle merken */
					hulle_str[hulle_count]->index = h;
				}
			}
		}
	}

	/* Alles OK */
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
		/* Ja, zurÅck */
		return( 0 );

	/* Speicher fÅr die neue HÅlle holen */
	if( !( new_hulle = ( HULLE_STR * ) calloc( 1, sizeof( HULLE_STR ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alle Items der HÅlle bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Zeiger auf ITEM_STR holen */
		item = &hulle->item_str[i];

		/* Ist der Punkt vor term ? */
		if( nterm_str[item->nterm_index]->ri[item->pkt_pos] == term )
		{
			/* Ist es fÅr einen SLR-Parser ? */
			if( lr_info.type == SLR )
			{
				/* Item in HÅlle eintragen */
				if( insert_item( new_hulle, item->nterm_index, 0, item->pkt_pos + 1 ) < 0 )
					/* Fehler */
					return( -1 );
			}

			else
			{
				/* Alle Terminale anhÑngen */
				for( j = 0; j < item->term.anz_elem; j++ )
				{
					/* Item in HÅlle eintragen */
					if( insert_item( new_hulle, item->nterm_index, item->term.term_arr[j], item->pkt_pos + 1 ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}
		}
	}

	/* Ist Åberhaupt eine HÅlle erzeugt worden ? */
	if( new_hulle->anz_items )
	{
		/* Anzahl der Kerne merken */
		new_hulle->anz_kitems = new_hulle->anz_items;

		/* Existiert die HÅlle bereits ? */
		if( !( h_index = cmp_hulle( new_hulle ) ) )
		{
			/* Nein, HÅlle in hulle_str aufnehmen */

			/* Speicher fÅr neuen HULLE_STR-Eintrag holen */
			if( !( hulle_str = ( HULLE_STR ** ) realloc( hulle_str, ( ++hulle_count + 1 ) * sizeof( HULLE_STR * ) ) ) )
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
				/* Terminale freigeben ? */
				if( &new_hulle->item_str[i].term )
					/* Speicher der Terminale freigeben */
					free( ( VOID * ) &new_hulle->item_str[i].term.term_arr );

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


LONG cmp_hulle( HULLE_STR *hulle )
{
	/* Hier wird geprÅft, ob es die HÅlle schon gibt. */
	REG LONG	i, j, k;
	LONG		kitem_count;

	/* Alle HÅllen bearbeiten */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Ist die Anzahl der Kerne gleich ? */
		if( hulle_str[i]->anz_kitems == hulle->anz_kitems )
		{
			/* ZÑhler fÅr die Kerne zurÅcksetzen */
			kitem_count = 0;

			/* Alle Kerne prÅfen */
			for( j = 0; j < hulle->anz_kitems; j++ )
			{
				/* Ist es fÅr ein SLR-Parser ? */
				if( lr_info.type == SLR )
				{
					/* Ist der Kern in beiden HÅllen vorhanden ? */
					if( find_item( hulle_str[i], hulle->item_str[j].nterm_index, 0, hulle->item_str[j].pkt_pos ) == -2 )
						/* Der Kern war in beiden HÅllen vorhanden */
						kitem_count++;
				}

				else
				{
					/* Alle Terminale des Kerns */
					for( k = 0; k < hulle->item_str[j].term.anz_elem; k++ )
					{
						/* Ist der Kern in beiden HÅllen vorhanden ? */
						if( find_item( hulle_str[i], hulle->item_str[j].nterm_index, hulle->item_str[j].term.term_arr[k], hulle->item_str[j].pkt_pos ) != -2 )
							/* Der Kern war nicht in der HÅlle vorhanden */
							break;
					}

					/* Waren alle Terminale gleich ? */
					if( k == hulle->item_str[j].term.anz_elem )
						/* Ja */
						kitem_count++;
				}
			}

			/* Waren alle Kerne gleich ? */
			if( hulle->anz_kitems == kitem_count )
				/* Ja */
				return( i );
		}
	}

	/* HÅllen sind nicht gleich */
	return( 0 );
}


WORD lr_to_lalr( VOID )
{
	/* Hier werden die HÅllen verschmolzen, die gleiche Kerne haben. */
	HULLE_STR	**temp_hulle_str;
	ITEM_STR	*item_j, *item_i;
	LONG		item_count;
	REG LONG	i, j, k_j, k_i;
	ULONG		temp_hulle_count = 0;

	/* Speicher fÅr Zeiger auf HÅllen reservieren */
	if( !( temp_hulle_str = ( HULLE_STR ** ) malloc( ( hulle_count + 1 ) * sizeof( HULLE_STR * ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Meldung ausgeben */
	fprintf( file, "\n\n\n/**********************/\n/* Verschmelze HÅllen */"
				   "\n/**********************/\n\n" );

	/* Alle HÅllen vergleichen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Ist eine HÅlle da ? */
		if( hulle_str[i] )
		{
			/* Alle HÅllen */
			for( j = i + 1; j <= hulle_count; j++ )
			{
				/* Ist eine HÅlle da ? */
				if( hulle_str[j] )
				{
					/* Ist die Anzahl der Kerne gleich ? */
					if( hulle_str[i]->anz_kitems == hulle_str[j]->anz_kitems )
					{
						/* ZÑhler fÅr die Kerne zurÅcksetzen */
						item_count = 0;

						/* Kerne der HÅlle i */
						for( k_i = 0; k_i < hulle_str[i]->anz_kitems; k_i++ )
						{
							/* Zeiger auf ITEM_STR holen */
							item_i = &hulle_str[i]->item_str[k_i];

							/* Kerne der HÅlle j */
							for( k_j = 0; k_j < hulle_str[j]->anz_kitems; k_j++ )
							{
								/* Zeiger auf ITEM_STR holen */
								item_j = &hulle_str[j]->item_str[k_j];

								/* Sind die Kerne gleich ? */
								if( ( item_i->nterm_index == item_j->nterm_index ) && ( item_i->pkt_pos == item_j->pkt_pos ) )
									/* Ja */
									item_count++;
							}
						}

						/* Waren die Kerne gleich ? */
						if( item_count == hulle_str[i]->anz_kitems )
						{
							/* Ja, Items von j nach i */
							for( k_j = 0; k_j < hulle_str[j]->anz_items; k_j++ )
							{
								/* Zeiger auf Item holen */
								item_j = &hulle_str[j]->item_str[k_j];

								/* Alle Terminale des Items */
								for( k_i = 0; k_i < item_j->term.anz_elem; k_i++ )
								{
									/* Item in HÅlle eintragen */
									if( insert_item( hulle_str[i], item_j->nterm_index, item_j->term.term_arr[k_i], item_j->pkt_pos ) < 0 )
										/* Fehler */
										return( -1 );
								}
							}

							/* Speicher fÅr HÅlle j freigeben */
							free( ( VOID * ) hulle_str[j] );

							/* Eintrag zurÅcksetzen */
							hulle_str[j] = 0L;

							/* Meldung ausgeben */
							fprintf( file, "\nI%3lu = I%3lu + I%3lu", i, i, j );
						}
					}
				}
			}

			/* Zeiger auf die HÅlle merken */
			temp_hulle_str[temp_hulle_count++] = hulle_str[i];
		}
	}

	/* Die Zeiger auf die HÅllen von temp_hulle_str nach hulle_str */
	for( i = 0; i < temp_hulle_count; i++ )
		/* Zeiger kopieren */
		hulle_str[i] = temp_hulle_str[i];

	/* Anzahl der HÅllen korregieren */
	hulle_count = temp_hulle_count - 1;

	/* Speicher freigeben */
	free( ( VOID * ) temp_hulle_str );

	/* Alles OK */
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
	if( !( hulle->error_index = ( LONG * ) realloc( ( VOID * ) hulle->error_index, sizeof( LONG ) * ( hulle->anz_error_index + 1 ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* error_index eintragen */
	hulle->error_index[hulle->anz_error_index++] = error_index;

	/* Alles OK */
	return( 0 );
}


WORD make_tab( VOID )
{
	/* Hier wird die LR-Tabelle erzeugt. */
	REG ULONG	i, j, k;
	NTERM_STR	*nterm;
	ITEM_STR	*item;
	LONG		h_index, t_index;
	WORD		alt;

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

	/* Die ZustandsÅbergÑnge erzeugen */
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
			if( ( nterm->anz_ri > item->pkt_pos ) && !( hash_tab[nterm->ri[item->pkt_pos]].term_str ) )
			{
				/* GOTO( Ii, j ) */
				if( ( h_index = do_goto( hulle_str[i], nterm->ri[item->pkt_pos] ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Gab es die eine passende HÅlle ? */
				if( h_index )
					/* Die HÅlle gab es */
					jump_tab[i][nterm_str[hash_tab[nterm->ri[item->pkt_pos]].nterm_index]->prod_index - 1] = h_index;
			}
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

			/* Ist der Punkt vor einem Terminal ? */
			if( ( nterm_str[item->nterm_index]->anz_ri > item->pkt_pos ) && ( hash_tab[nterm_str[item->nterm_index]->ri[item->pkt_pos]].term_str ) )
			{
				/* Ja, die HÅlle dazu bilden */
				t_index = nterm_str[item->nterm_index]->ri[item->pkt_pos];

				/* ACCEPT wird nicht geschoben */
				if( t_index != accept )
				{
					/* GOTO( Ii, Terminal ) */
					if( ( h_index = do_goto( hulle_str[i], t_index ) ) < 0 )
						/* Fehler */
						return( -1 );

					/* Wurde eine HÅlle erzeugt ? */
					if( h_index )
					{
						/* Index des Terminals holen */
						t_index = get_term_index ( t_index );

						/* EintrÑge in die LR-Tabelle vornehmen */
						lr_tab[i][t_index].state = SHIFT;
						lr_tab[i][t_index].value = h_index;
					}
				}
			}
		}
	}

	/* Zustand 0, ACCEPT -> acceptiere */
	t_index = get_term_index( accept );

	/* Eintragung vornehmen */
	lr_tab[0][t_index].state = REDUCE;
	lr_tab[0][t_index].value = 0;

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
						insert_reduce( i, t_index, item->nterm_index );
					}
				}

				else
				{
					/* Ist eine LR- bzw- LALR-Tabelle */
					for( k = 0; k < item->term.anz_elem; k++ )
					{
						/* Index des Terminals holen */
						t_index = get_term_index( item->term.term_arr[k] );

						/* Reduce eintragen */
						insert_reduce( i, t_index, item->nterm_index );
					}
				}
			}
		}
	}

	/* Leere ZustÑnde ggf auf Reduce setzen */
	set_empty_to_reduce();

	/* LR-Tabelle optional komprimieren */
	if( compress_lr_tab() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD allocate_mem( VOID )
{
	/* Hier wird der Speicher fÅr die Erzeugung der LR-Tabelle angefordert. */
	REG ULONG	i;

	/* Array mit Zeigern auf LR_TAB */
	if( !( lr_tab = ( LR_TAB ** ) malloc( sizeof( LR_TAB * ) * ( hulle_count + 1 ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Zeiger auf LR_TAB in lr_tab eintragen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Speicher fÅr LR_TAB anfordern */
		if( !( lr_tab[i] = ( LR_TAB * ) calloc( term_count + 1, sizeof( LR_TAB ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );
	}

	/* Array mit Zeigern auf die Sprungwerte */
	if( !( jump_tab = ( LONG ** ) malloc( sizeof( LONG * ) * ( hulle_count + 1 ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Zeiger auf LONG in Sprung-Tabelle eintragen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Speicher fÅr LR_TAB anfordern */
		if( !( jump_tab[i] = ( LONG * ) calloc( prod_count + 1, sizeof( LONG ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );
	}

	/* Speicher fÅr die Ziel-Tabelle anfordern */
	if( !( ziel_tab = ( ULONG * ) calloc( hulle_count + 1, sizeof( ULONG ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

	/* Alles OK */
	return( 0 );
}


LONG get_term_index( LONG term )
{
	/* Zu dem Terminal term wird der Index zurÅckgeliefert. */
	REG ULONG	i;

	/* Alle Terminale prÅfen */
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


VOID insert_reduce( LONG hulle, LONG term_index, LONG nterm_index )
{
	/* Hier wird ein Reduce-Zustand in die LR-Tabelle eingetragen. */
	WORD	term_priori, prod_priori, state = SHIFT;
	BYTE	new_nterm[256], old_nterm[256];
	LONG	nterm_i;

	/* Ist an dieser Stelle schon ein Shift-Eintrag ? */
	if( lr_tab[hulle][term_index].state == SHIFT )
	{
		/* Shift-/Reduce-Fehler */

		/* PrioritÑt des Terminals holen */
		term_priori = term_str[term_index].priori;

		/* PrioritÑt der Produktion holen */
		prod_priori = nterm_str[nterm_index]->priori;

		/* PrioritÑt der Produktion > PrioritÑt des Terminals ? */
		if( prod_priori > term_priori )
			/* Reduzieren */
			state = REDUCE;

		/* PrioritÑten gleich, Produktion linksassoziativ ? */
		else if( ( prod_priori == term_priori ) && ( nterm_str[nterm_index]->assoc == LEFT ) )
			/* Reduzieren */
			state = REDUCE;

		/* Meldung ausgeben */
		fprintf( file, "\n\n\nShift-/Reduce-Konflikt\nHÅlle        : %ld\nTerminal     : %s", hulle, hash_tab[term_str[term_index].hash_tab_index].name );

		/* Hat die neue Produktion Alternativen ? */
		if( !( nterm_str[nterm_index]->alt ) && ( ( ( nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[nterm_index + 1]->alt ) ) ) )
			/* Nein, Meldung ausgeben */
			fprintf( file, "\nProduktion   : %s", hash_tab[nterm_str[nterm_index]->hash_tab_index].name );

		else
		{
			/* Ja, mit Nummer der Alternative ausgeben */
			sprintf( new_nterm, "%s_%lu", hash_tab[nterm_str[nterm_index]->hash_tab_index].name, nterm_index - ( nterm_index - nterm_str[nterm_index]->alt ) );
			fprintf( file, "\nProduktion   : %s", new_nterm );
		}

		/* Schieben oder Reduzieren ? */
		if( state == REDUCE )
		{
			/* Hat die neue Produktion Alternativen ? */
			if( !( nterm_str[nterm_index]->alt ) && ( ( ( nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[nterm_index + 1]->alt ) ) ) )
				/* Nein, Meldung ausgeben */
				fprintf( file, "\nReduziere mit: %s", hash_tab[nterm_str[nterm_index]->hash_tab_index].name );

			else
				/* Ja, mit Nummer der Alternative ausgeben */
				fprintf( file, "\nReduziere mit: %s", new_nterm );
		}

		else
		{
			/* Schiebe */
			fprintf( file, "\nSchiebe %s", hash_tab[term_str[term_index].hash_tab_index].name );

			/* Alles OK */
			return;
		}
	}

	/* Ist an dieser Stelle schon ein Reduce-Eintrag ? */
	else if( lr_tab[hulle][term_index].state == REDUCE )
	{
		/* Reduce-/Reduce-Fehler */

		/* Meldung ausgeben */
		fprintf( file, "\n\n\nReduce-/Reduce-Konflikt\nHÅlle        : %ld\nTerminal     : %s", hulle, hash_tab[term_str[term_index].hash_tab_index].name );

		/* Index der alten Produktion holen */
		nterm_i = lr_tab[hulle][term_index].value;

		/* Hat die bereits eingetragene Produktion Alternativen ? */
		if( !( nterm_str[nterm_i]->alt ) && ( ( ( nterm_i + 1 ) < nterm_count ) && ( !( nterm_str[nterm_i + 1]->alt ) ) ) )
		{
			/* Nein */

			/* Hat die neue Produktion Alternativen ? */
			if( !( nterm_str[nterm_index]->alt ) && ( ( ( nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[nterm_index + 1]->alt ) ) ) )
				/* Nein */
				fprintf( file, "\nProduktion   : %s", hash_tab[nterm_str[nterm_index]->hash_tab_index].name );

			else
			{
				/* Ja */
				sprintf( new_nterm, "%s_%lu", hash_tab[nterm_str[nterm_index]->hash_tab_index].name, nterm_index - ( nterm_index - nterm_str[nterm_index]->alt ) );
				fprintf( file, "\nProduktion   : %s", new_nterm );
			}

			/* Alte Produktion ausgeben */
			fprintf( file, "\nReduziere mit: %s", hash_tab[nterm_str[nterm_i]->hash_tab_index].name );
		}

		else
		{
			/* Bereits eingetragene Produktion hat Alternativen */

			/* Eingetragene Produktion mit Alternativenangabe vorbereiten */
			sprintf( old_nterm, "%s_%lu", hash_tab[nterm_str[nterm_i]->hash_tab_index].name, nterm_i - ( nterm_i - nterm_str[nterm_i]->alt ) );

			/* Hat die neue Produktion Alternativen ? */
			if( !( nterm_str[nterm_index]->alt ) && ( ( ( nterm_index + 1 ) < nterm_count ) && ( !( nterm_str[nterm_index + 1]->alt ) ) ) )
				/* Nein */
				fprintf( file, "\nProduktion   : %s", hash_tab[nterm_str[nterm_index]->hash_tab_index].name );

			else
			{
				/* Ja */
				sprintf( new_nterm, "%s_%lu", hash_tab[nterm_str[nterm_index]->hash_tab_index].name, nterm_index - ( nterm_index - nterm_str[nterm_index]->alt ) );
				fprintf( file, "\nProduktion   : %s", new_nterm );
			}

			/* Alte Produktion ausgeben */
			fprintf( file, "\nReduziere mit: %s", old_nterm );
		}

		/* Alles OK */
		return;
	}

	/* EintrÑge in die LR-Tabelle vornehmen */
	lr_tab[hulle][term_index].state = REDUCE;
	lr_tab[hulle][term_index].value = nterm_index;

	/* Eintrag in Ziel-Tab merken */
	ziel_tab[hulle] = nterm_index;
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


WORD compress_lr_tab( VOID )
{
	/* Hier wird die lr_tab komprimiert. */
	REG ULONG	i, j;

	/* Anzahl der ZustÑnde merken */
	anz_cop_lr_tab = hulle_count + 1;

	/* LR-Tabelle komprimieren ? */
	if( !lr_info.compress )
	{
		/* Protokollausgabe ? */
		#ifdef PROT

				/* Meldung ausgeben */
				fprintf( file, "\n\n\nKomprimierte LR-Tabelle\n\n" );

		#endif

		/* Speicher fÅr Array holen */
		if( !( cop_lr_tab = ( LONG * ) malloc( sizeof( LONG ) * ( hulle_count + 1 ) ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0, 0 ) );

		/* cop_lr_tab initialisieren */
		for( i = 0; i <= hulle_count; i++ )
			/* Indexe Åbertragen */
			cop_lr_tab[i] = i;

		/* Alle HÅllen vergleichen */
		for( i = 0; i <= hulle_count; i++ )
		{
			/* Alle HÅllen vergleichen */
			for( j = i + 1; j <= hulle_count; j++ )
			{
				/* Sind die beiden lr_tab-EintrÑge gleich ? */
				if( ( cop_lr_tab[j] == j ) && !cmp_lr_tab( i, j ) )
				{
					/* Ja */
					cop_lr_tab[j] = i;

					/* Protokollausgabe ? */
					#ifdef PROT

						/* Ausgaben */
						fprintf( file, "\ncop_lr_tab[%lu] == lr_tab[%lu]", j, i );

					#endif

					/* Ein Zustand weniger */
					anz_cop_lr_tab--;
				}
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD cmp_lr_tab( ULONG lr_tab1_index, ULONG lr_tab2_index )
{
	/* Hier werden zwei lr_tab-EintrÑge auf Gleichheit geprÅft. */
	ULONG	i;

	/* Alle Terminale */
	for( i = 0; i <= term_count; i++ )
	{
		/* Sind die EintrÑge gleich ? */
		if( ( lr_tab[lr_tab1_index][i].state != lr_tab[lr_tab2_index][i].state ) || ( lr_tab[lr_tab1_index][i].value != lr_tab[lr_tab2_index][i].value ) )
			/* Nein */
			return( 1 );
	}

	/* lr_tab-EintrÑge gleich */
	return( 0 );
}