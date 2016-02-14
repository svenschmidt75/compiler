/*
	Projekt		:	SLR-Parser-Generator
	Modul		:	GENERATE.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/


/* Includes */
#include <portab.h>
#include <stdlib.h>
#include <tos.h>
#include <stdio.h>
#include <string.h>
#include "slr.h"


/* Defines */

#define	ADD_ITEM				20	/*	FÅr 20 Items mehr Speicher
										anfordern
									*/


/* Definitionen */

/* Beinhaltet die Mengen der Items ( HÅlle ) */
OBER_HULLE			ober_hulle = { 0 };


/* Beinhaltet die Nummern aller Terminale */
FIRST				term_str = { 0 };


/* Anzahl der belegten HÅllen */
ULONG				anz_bel_hulle = 0;

/* Goto-Tabelle */
ULONG				**goto_tab;

/* Ziel-Tabelle */
ULONG				*ziel_tab;

/* SLR-Tabelle */
SLR_TAB				**slr_tab;

/* Eine FIRST-Struktur zum Erzeugen der FIRST/FOLLOW-Mengen */
FIRST				first = { 0 };



/* Globale Definitionen */
MGLOBAL ULONG			prod_list_nr;
MGLOBAL BIN_TREE		*root_term;
MGLOBAL PROD_INFO		*prod_info;
MGLOBAL PROD_HASH		*prod_hash;
MGLOBAL	FIRST			first;
MGLOBAL	FIRST			nterm;
MGLOBAL	ULONG			epsilon_hash_ind;
MGLOBAL	ULONG			accept_hash_ind;
MGLOBAL	LONG			nterm_length;
MGLOBAL	FILE			*file;
MGLOBAL	SLR_INFO		slr_info;
MGLOBAL	ULONG			anz_prod;
MGLOBAL ULONG			prod_err_anz_elem;
MGLOBAL ULONG			err_number;
MGLOBAL BYTE			non_term[];


/* Prototypen */
WORD gen_goto( VOID );
VOID show_hulle( VOID );
WORD extend_twodim( WORD flg );
WORD gen_f( LONG nterm );
WORD cmp_memcpy( FIRST *dest, FIRST *source );
VOID show_follow( VOID );
WORD s_r( ULONG prod_priori, ULONG term_priori, ULONG prod_assoc );
WORD gen_follow( VOID );
VOID test_of_reduce( VOID );
LONG insert_hash_err_msg( BYTE *non_term );



WORD gen_items( VOID )
{
	/*
		Hier werden alle Items der Grammatik gebildet.
		Auûerdem wird in dieser Routine die Menge der kononischen
		Items gebildet.
	*/
	REG ULONG	i, j;
	WORD		index;

	/* Alle FOLLOW-Mengen bilden */
	gen_follow();

	/* FIRST-Mengen ausgeben ? */
	if( slr_info.first == ON )
	{
		/* Meldung ausgeben */
		fprintf( file, "\n\n\nErzeuge FIRST-Mengen\n" );

		/* FIRST-Mengen erzeugen */
		for( i = 0; i <= prod_list_nr; i++ )
		{
			/* Alternativen Åberspringen */
			if( prod_info[i].prod_fl )
				continue;

			/* Es befinden sich keine Elemente in FIRST */
			first.anz_elem = 0;

			/* FIRST-Mengen erzeugen */
			gen_first( &first, prod_info[i].prod_nr, ( WORD * ) &index );

			/* FIRST-Menge anzeigen */
			fprintf( file, "\n%s\n", prod_hash[prod_info[i].prod_nr].prod_name );
			fprintf( file, "-----------------------------------------------\n" );

			/* Alle Terminale ausgeben */
			for( j = 0; j < first.anz_elem; j++ )
			{
				/* Ist es das Terminal EPSILON ? */
				if( first.arr[j] == epsilon_hash_ind )
					fprintf( file, "EPSILON\n" );

				else
					/* Terminal ausgeben */
					fprintf( file, "%s\n", prod_hash[first.arr[j]].prod_name );
			}

			/* Ausgabe auf Bildschirm ? */
			if( slr_info.dest == DEST_SCREEN )
				/* Auf Tastendruck warten */
				Cnecin();
		}
	}

	/* FOLLOW-Mengen ausgeben ? */
	if( slr_info.follow == ON )
	{
		/* Meldung ausgeben */
		fprintf( file, "\n\n\nErzeuge FOLLOW-Mengen\n" );

		/* FOLLOW-Mengen ausgeben */
		show_follow();
	}

	/* Speicher fÅr die HÅllen-Strukturen anfordern */
	if( extend_twodim( PTR_HULLE ) < 0 )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Meldung ausgeben ? */
	if( slr_info.analyze == ON )
		/* Entsprechende Meldung ausgeben */
		fprintf( file, "\n\nErzeuge HÅlle( I0 )..." );

	/* Erstes Element erzeugen */
	add_item( ober_hulle.hulle[0], prod_info[0].prod_nr, prod_hash[prod_info[0].prod_nr].prod_list_nr, 0 );

	/* Nichtterminal merken */
	ober_hulle.hulle[0]->nterm = prod_info[0].prod_nr;
	ober_hulle.hulle[0]->index = 0;

	/* Anzahl der Kerne merken */
	ober_hulle.hulle[0]->anz_kitems = 1;

	/* Hier werden die Åbrigen Elemente zur HÅlle( I0 ) hinzugefÅgt. */
	if( add_prod( ober_hulle.hulle[0] ) < 0 )
		/* Fehler */
		return( -1 );

	/* Nun werden alle Åbrigen HÅllen Åber GOTO erzeugt. */
	if( gen_goto() < 0 )
		/* Fehler */
		return( -1 );

	/* Alles OK */
	return( 0 );
}


WORD add_prod( HULLE *hulle )
{
	/*
		In dieser Routine werden die Produktionen zur HÅlle hinzugefÅgt,
		zu denen ein Punkt vor dem Namen der Produktion steht.
		Bsp.: expr -> .term MUL factor
			  term wird nun zur HÅlle hinzugefÅgt
	*/
	REG	LONG	i, j;
	LONG		nterm, nterm_list_nr;
	WORD		pkt_pos;

	/* Alle Items bearbeiten */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Position des Punktes holen */
		pkt_pos = hulle->items_array[i]->pkt_pos;

		/* Element auf der rechten Seite holen */
		nterm = prod_info[hulle->items_array[i]->prod_list_nr].prod_ri[pkt_pos];

		/* Ist es ein Nichtterminal ? */
		if( prod_hash[nterm].prod_type == NON_TERM )
		{
			/* Ja, Items zur HÅlle hinzufÅgen */
			nterm_list_nr = prod_hash[nterm].prod_list_nr;

			do
			{
				/* Alle Alternativen zur HÅlle hinzufÅgen */

				/* Ist das Item schon in der HÅlle ? */
				for( j = 0; j < hulle->anz_items; j++ )
				{
					/* Sind die Items gleich ? */
					if( hulle->items_array[j]->item_list_nr == nterm_list_nr * ITEM_LIST_FAC )
						/* Ja */
						break;
				}

				/* Waren die Items gleich ? */
				if( j == hulle->anz_items )
				{
					/* Nein, Item erzeugen */
					if( add_item( ober_hulle.hulle[anz_bel_hulle], nterm, nterm_list_nr, 0 ) < 0 )
						/* Fehler */
						return( error( NOT_ENOUGH_MEMORY, 0 ) );
				}

			}while( prod_info[nterm_list_nr++].prod_fl );
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD gen_goto( VOID )
{
	/*
		In dieser Routine werden alle Åbrigen Mengen von Elementen Åber
		GOTO( Ix, X ) erzeugt.
	*/
	REG ULONG	i, h_count;
	ULONG		temp_bel_hulle;

	/* Mit der nullten HÅlle beginnen */
	h_count = 0;

	do
	{
		/* Alle HÅllen auf das Nichtterminal untersuchen */
		for( i = 0; i <= prod_list_nr; i++ )
		{
			/* Anzahl der HÅllen merken */
			temp_bel_hulle = anz_bel_hulle;

			/* GOTO( Ix, i ) */
			if( ( _goto( ober_hulle.hulle[h_count], prod_info[i].prod_nr, h_count, 1 ) ) != 0UL )
				/* Fehler */
				return( -1 );

			/* Alternativen interessieren hier nicht */
			while( prod_info[i].prod_fl )
				i++;

			/* Ist eine HÅlle erzeugt worden ? */
			if( ( temp_bel_hulle < anz_bel_hulle ) && ( slr_info.analyze == ON ) )
					/* HÅlle( Ix, X ) bilden */
					fprintf( file, "\nI%lu = Erzeuge HÅlle( I%3lu, %35s )...", anz_bel_hulle, h_count, prod_hash[prod_info[i].prod_nr].prod_name );
		}

		/* Alle Terminale bearbeiten */
		for( i = 0; i < term_str.anz_elem; i++ )
		{
			/* Anzahl der HÅllen merken */
			temp_bel_hulle = anz_bel_hulle;

			/* GOTO( Ix, i ) */
			if( _goto( ober_hulle.hulle[h_count], term_str.arr[i], h_count, 1 ) < 0 )
				/* Fehler */
				return( -1 );

			/* Ist eine HÅlle erzeugt worden ? */
			if( ( temp_bel_hulle < anz_bel_hulle ) && ( slr_info.analyze == ON ) )
				/* HÅlle( Ix, X ) bilden */
				fprintf( file, "\nI%lu = Erzeuge HÅlle( I%3lu, %35s )...", anz_bel_hulle, h_count, prod_hash[term_str.arr[i]].prod_name );
		}

		/* Die nÑchste HÅlle bearbeiten */
		h_count++;

	}while( h_count <= anz_bel_hulle );

	/* Alles OK */
	return( 0 );
}


ULONG _goto( HULLE *hulle, LONG n_term, ULONG h_count, WORD state )
{
	/*
		In dieser Routine werden die passenden Items zu der HÅlle hinzu-
		gefÅgt.
		hulle	=	Hier wird nach den Items gesucht
		n_term	=	Terminal bzw. Nichtterminal
	*/
	REG	ULONG	i;
	WORD		h_flg = 0, pkt_pos;

	/* Das Grammatiksymbol n_term in der HÅlle suchen */
	for( i = 0; i < hulle->anz_items; i++ )
	{
		/* Position des Punktes bei diesem Item */
		pkt_pos = hulle->items_array[i]->pkt_pos;

		/* Zeiger auf die Elemente auf der rechten Seite der Produktion */
		if( n_term == prod_info[hulle->items_array[i]->prod_list_nr].prod_ri[hulle->items_array[i]->pkt_pos] )
		{
			/* Ja, Element gefunden -> In HÅlle aufnehmen */

			/* Ist es das erste in die HÅlle einzutragene Item ? */
			if( !h_flg )
			{
				/* Ja, nÑchste HÅlle nehmen */
				anz_bel_hulle++;

				/* HÅllen-Struktur mit Nullen initialisieren */
				memset( ( VOID * ) ober_hulle.hulle[anz_bel_hulle], 0, sizeof( HULLE ) );

				/* Findet ein HÅllen-öberlauf statt ? */
				if( ( anz_bel_hulle + 1 ) >= ober_hulle.anz_max_hulle )
				{
					/* Ja, Speicher fÅr mehr anfordern */
					if( extend_twodim( PTR_HULLE ) < 0 )
						/* Fehler */
						return( -1 );
				}

				/* HÅllen-Flag lîschen */
				h_flg++;

				/* Nichtterminal/Terminal merken */
				ober_hulle.hulle[anz_bel_hulle]->nterm = n_term;
				ober_hulle.hulle[anz_bel_hulle]->index = h_count;
			}

			/* Item erzeugen */
			if( add_item( ober_hulle.hulle[anz_bel_hulle], hulle->items_array[i]->prod_nr, hulle->items_array[i]->prod_list_nr, pkt_pos + 1 ) < 0 )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0 ) );
		}
	}

	/* Ist die Anzahl der Kerne schon vermerkt ? */
	if( !ober_hulle.hulle[anz_bel_hulle]->anz_kitems )
		/* Anzahl der Kerne merken */
		ober_hulle.hulle[anz_bel_hulle]->anz_kitems = ober_hulle.hulle[anz_bel_hulle]->anz_items;

	/* Hier werden die Åbrigen Elemente zur HÅlle( Ix ) hinzugefÅgt. */
	if( add_prod( ober_hulle.hulle[anz_bel_hulle] ) < 0 )
		/* Fehler */
		return( -1 );

	/* Sollen doppelte HÅllen gelîscht werden ? */
	if( state )
	{
		/* Alle HÅllen ÅberprÅfen */
		for( i = 0; i < anz_bel_hulle; i++ )
		{
			/* Ist die HÅlle schon vorhanden ? */
			if( !cmp_hulle( ober_hulle.hulle[i], ober_hulle.hulle[anz_bel_hulle] ) )
			{
				/* Speicher fÅr die Items wieder freigeben */
				for( i = 0; i < ober_hulle.hulle[anz_bel_hulle]->anz_items; i++ )
					/* Speicher freigeben */
					free( ( VOID * ) ober_hulle.hulle[anz_bel_hulle]->items_array[i] );

				/* Speicher fÅr Itemsarray freigeben */
				free( ( VOID * ) ober_hulle.hulle[anz_bel_hulle]->items_array );

				/* HÅlle lîschen */
				anz_bel_hulle--;

				/* For-Schleife verlassen */
				break;
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD get_all_term_nr( BIN_TREE *blat )
{
	/* Hier werden alle Nummern der Terminale in eine Struktur eingefÅgt. */

	/* Kein Terminal mehr da ? */
	if( root_term )
	{
		/* Wurde das Terminal Åberhaupt benutzt ? */
		if( prod_hash[blat->prod_nr].defined )
		{
			/* Terminal in term_str eintragen */
			if( insert_first( blat->prod_nr, &term_str ) < 0 )
				/* Fehler */
				return( -1 );
		}

		else
			/* Terminal wurde nicht benutzt */
			printf("\n\nTerminal %s wurde nicht in der Grammatik benutzt !", prod_hash[blat->prod_nr].prod_name );

		/* Links weitermachen ? */
		if( blat->left )
			/* Links weitermachen */
			get_all_term_nr( blat->left );

		/* Rechts weitermachen ? */
		if( blat->right )
			/* Rechts weitermachen */
			get_all_term_nr( blat->right );
	}

	/* Alles OK */
	return( 0 );
}

WORD cmp_hulle( HULLE *h_1, HULLE *h_2 )
{
	/* Diese Funktion vergleicht zwei HÅllen-Strukturen miteinander. */
	REG	LONG	i;

	/* Ist die Anzahl der Items gleich ? */
	if( h_1->anz_items != h_2->anz_items )
		/* Nein, HÅllen kînnen nicht gleich sein */
		return( -1 );

	/* Speicherbereiche vergleichen */
	for( i = 0; i < h_1->anz_items; i++ )
	{
		/* Alle Items miteinander vergleichen */
		if( h_1->items_array[i]->item_list_nr != h_2->items_array[i]->item_list_nr )
			/* HÅllen sind ungleich */
			return( -1 );
	}

	/* HÅllen sind gleich */
	return( 0 );
}


WORD add_item( HULLE *hulle, LONG prod_nr, ULONG prod_list_nr, WORD pkt_pos )
{
	/* In dieser Routine wird ein Items zu einer HÅlle hinzugefÅgt. */
	ULONG		item = hulle->anz_items;

	/* Ist noch genÅgend Speicher vorhanden ? */
	if( hulle->anz_max_items < ( hulle->anz_items + 1 ) )
	{
		/* Platz fÅr zusÑtzliche Items schaffen */
		hulle->anz_max_items += ADD_ITEM;

		/* GenÅgend Speicher anfordern */
		if( !( hulle->items_array = ( PROD_ITEM ** ) realloc( ( VOID * ) hulle->items_array, sizeof( PROD_ITEM ) * hulle->anz_max_items ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0 ) );
	}

	/* Item erzeugen */

	/* Speicher fÅr PROD_ITEM anfordern */
	if( !( hulle->items_array[item] = ( PROD_ITEM * ) malloc( sizeof( PROD_ITEM ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Nummer der Produktion */
	hulle->items_array[item]->prod_nr = prod_nr;

	/* Laufende Nummer der Produktion ( fÅr PROD_INFO ) */
	hulle->items_array[item]->prod_list_nr = prod_list_nr;

	/* Position des Punktes */
	hulle->items_array[item]->pkt_pos = pkt_pos;

	/* Nummer des Items */
	hulle->items_array[item]->item_list_nr = ( prod_list_nr * ITEM_LIST_FAC ) + pkt_pos;

	/* Ein Element ist bereits in der HÅlle */
	hulle->anz_items++;

	/* Alles OK */
	return( 0 );
}


VOID show_hulle( VOID )
{
	/* Hier werden alle HÅllen angezeigt */
	REG WORD i, j, k, prod_count;

	/* Sollen die Items Åberhaupt gezeigt werden ? */
	if( slr_info.slr_items == ON )
	{
		/* Zeile freilassen */
		fprintf( file, "\n\n" );

		/* Alle HÅllen ausgeben */
		for( i = 0; i <= anz_bel_hulle; i++ )
		{
			/* Nummer der HÅlle */
			fprintf( file, "\nI%d = GOTO( I%lu, %s )\n", i, ober_hulle.hulle[i]->index, prod_hash[ober_hulle.hulle[i]->nterm].prod_name );

			/* Alle Items zu der HÅlle ausgeben */
			for( j = 0; j < ober_hulle.hulle[i]->anz_items; j++ )
			{
				/* ZÑhler */
				k = 0;

				/* Ist es ein Kern ? */
				if( j < ober_hulle.hulle[i]->anz_kitems )
					/* Ja */
					fprintf( file, "\nKern: %s\t->\t", prod_hash[prod_info[ober_hulle.hulle[i]->items_array[j]->prod_list_nr].prod_nr].prod_name );

				else
					/* Produktionsnamen ausgeben */
					fprintf( file, "\n%s\t->\t", prod_hash[prod_info[ober_hulle.hulle[i]->items_array[j]->prod_list_nr].prod_nr].prod_name );

				/* Rechte Seite holen */
				while( prod_info[ober_hulle.hulle[i]->items_array[j]->prod_list_nr].prod_ri[k] )
				{
					/* Den Punkt ausgeben ? */
					if( k == ober_hulle.hulle[i]->items_array[j]->pkt_pos )
						/* Punkt ausgeben */
						fprintf( file, "." );

					/* Terminal ? */
					if( prod_hash[prod_info[ober_hulle.hulle[i]->items_array[j]->prod_list_nr].prod_ri[k]].prod_type == TERM )
						/* Terminal ausgeben */
						fprintf( file, " \'%s\'", prod_hash[prod_info[ober_hulle.hulle[i]->items_array[j]->prod_list_nr].prod_ri[k++]].prod_name );

					else
						/*  Nichtterminal ausgeben */
						fprintf( file, " %s", prod_hash[prod_info[ober_hulle.hulle[i]->items_array[j]->prod_list_nr].prod_ri[k++]].prod_name );
				}

				/* Den Punkt ausgeben ? */
				if( k == ober_hulle.hulle[i]->items_array[j]->pkt_pos )
					/* Punkt ausgeben */
					fprintf( file, "." );
			}

			/* NÑchste Zeile */
			fprintf( file, "\n" );

			/* GOTO-Tabelle ausgeben */
			for( j = 0; j < anz_prod; j++ )
			{
				/* Gibt es in der HÅlle einen ZustandsÅbergang ? */
				if( goto_tab[i][j] )
				{
					/* Index der Produktion besorgen */
					for( k = 1, prod_count = 0; prod_count != j; k++ )
					{
						/* Ist es eine Produktion oder eine Alternative ? */
						if( !prod_info[k].prod_fl )
							/* Ist die letzte Alternative */
							prod_count++;
					}

					/* Ja, Produktion und Sprungziel ausgeben */
					fprintf( file, "\n%*s: goto %lu", ( WORD ) nterm_length, prod_hash[prod_info[k].prod_nr].prod_name, goto_tab[i][j] );
				}
			}

			/* NÑchste HÅlle ausgeben */
			fprintf( file, "\n\n\n\n" );

			/* Ausgabe auf Bildschirm ? */
			if( slr_info.dest == DEST_SCREEN )
				/* Auf Testendruck warten */
				Cnecin();
		}
	}
}


WORD init_twodim( VOID ***pointer, WORD flg, ULONG x_anz, ULONG y_anz )
{
	/* Hier werden zweidimensionale Arrays initialisiert. */
	REG ULONG	i;
	ULONG		**ptr_ulong;
	SLR_TAB		**ptr_slr_tab;

	/* Welches Array ist es denn ? */
	switch( flg )
	{
		/* ULONG ? */
		case PTR_ULONG:
		{
			/* Speicher fÅr das Array anfordern */
			if( !( ptr_ulong = ( ULONG ** ) malloc( x_anz * sizeof( ULONG * ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0 ) );

			/* Pointer nun initialisieren */
			for( i = 0; i < x_anz; i++ )
			{
				/* Speicher anfordern */
				if( !( ptr_ulong[i] = ( ULONG * ) calloc( y_anz, sizeof( ULONG ) ) ) )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0 ) );
			}

			/* Pointer merken */
			*pointer = ( VOID ** ) ptr_ulong;

			/* Und TschÅû */
			break;
		}

		case PTR_SLR_TAB:
		{
			/* Speicher fÅr die SLR-Tabelle anfordern */
			if( !( ptr_slr_tab = ( SLR_TAB ** ) malloc( x_anz * sizeof( SLR_TAB * ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0 ) );

			/* Pointer nun initialisieren */
			for( i = 0; i < x_anz; i++ )
			{
				/* Speicher anfordern */
				if( !( ptr_slr_tab[i] = ( SLR_TAB * ) calloc( sizeof( SLR_TAB ), y_anz ) ) )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0 ) );
			}

			/* Pointer merken */
			*pointer = ( VOID ** ) ptr_slr_tab;

			/* Und TschÅû */
			break;
		}
	}

	/* Alles OK */
	return( 0 );
}

WORD extend_twodim( WORD flg )
{
	/* Hier werden zweisdimensionale Arrays erweitert. */
	REG WORD	i;
	ULONG		anz_max;
	HULLE		**ptr_hulle;

	/* Um was geht's ? */
	switch( flg )
	{
		case PTR_HULLE:
		{
			/* Anzahl der gesamten EintrÑge holen */
			anz_max = DEFAULT_HULLE_LENGTH + ober_hulle.anz_max_hulle;

			/* Array, in dem die HÅllen-Strukturen liegen werden, anlegen */
			if( !( ptr_hulle = ( HULLE ** ) malloc( anz_max * sizeof( HULLE ) ) ) )
				/* Fehler */
				return( error( NOT_ENOUGH_MEMORY, 0 ) );

			/* Pointer nun initialisieren */
			for( i = 0; i < anz_max; i++ )
			{
				/* Speicher anfordern */
				if( !( ptr_hulle[i] = ( HULLE * ) calloc( 1, sizeof( HULLE ) ) ) )
					/* Fehler */
					return( error( NOT_ENOUGH_MEMORY, 0 ) );
			}

			/* Max. Anzahl der HÅllen, fÅr die Platz ist */
			ober_hulle.anz_max_hulle += DEFAULT_HULLE_LENGTH;

			/* Ist Åberhaupt schon etwas in den Strukturen ? */
			if( anz_bel_hulle )
			{
				/* Es mÅssen alle HÅllen einzelnd kopiert werden :-( */
				for( i = 0; i <= anz_bel_hulle; i++ )
					/* HÅlle kopieren */
					memcpy( ( VOID * ) ptr_hulle[i], ( VOID * ) ober_hulle.hulle[i], sizeof( HULLE ) );

				/* Alten Speicherbereich freigeben */
				free( ( VOID * ) *ober_hulle.hulle );
			}

			/* Neuen eintragen */
			ober_hulle.hulle = ptr_hulle;
		}
	}

	/* Alles OK */
	return( 0 );
}

WORD gen_follow( VOID )
{
	/* Hier werden sÑmtliche FOLLOW-Mengen gebildet. */
	REG ULONG	i;

	/* Alle Nichtterminale auf der linken Seite durchgehen */
	for( i = 0; i < nterm.anz_elem; i++ )
	{
		/* FOLLOW-Menge zum Nichtterminal bilden */
		if( gen_f( nterm.arr[i] ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/* Alles OK */
	return( 0 );
}

WORD gen_f( LONG nterm )
{
	/* In dieser Routine werden die FOLLOW-Mengen gebildet. */
	ULONG	i, j;
	WORD	anz_elem, dummy;

	/* Nichtterminal wird bearbeitet */
	prod_hash[nterm].defined++;

	/* Ist es die Startproduktion ? */
	if( !prod_hash[nterm].prod_list_nr )
	{
		/* ACCEPT zu FOLLOW( nterm ) hinzufÅgen */
		if( insert_first( accept_hash_ind, &prod_hash[nterm].follow ) < 0 )
			/* Fehler */
			return( -1 );
	}

	/*
		Das Nichtterminal nterm auf der rechten Seite jeder Produktion suchen.
	*/

	/* Alle Produktionen bearbeiten */
	for( i = 0; i <= prod_list_nr; i++ )
	{
		/* Anzahl der Elemente auf der rechten Seite holen */
		anz_elem = prod_info[i].prod_ri_anz;

		/* Rechte Seite untersuchen */
		for( j = 0; j < anz_elem; j++ )
		{
			/* Ist es das gesuchte Nichtterminal ? */
			if( prod_info[i].prod_ri[j] == nterm )
			{
				/* Folgt noch ein Element ? */
				if( ( j + 1 ) < anz_elem )
				{
					/* FIRST( j + 1 ) */
					if( gen_first( &first, prod_info[i].prod_ri[j + 1], &dummy ) < 0 )
						/* Fehler */
						return( -1 );
				}

				/*
					Ist es das letzte Element bzw. ist FIRST( j + 1 )
					=> EPSILON ? ( Drachenbuch Regel 3 )
				*/
				if( ( ( j + 1 ) == anz_elem ) || ( dummy ) )
				{
					/* FOLLOW( nterm ) = FOLLOW( i ) */

					/* Ist die FOLLOW-Menge schon gebildet worden ? */
					if( !prod_hash[prod_info[i].prod_nr].follow.arr )
					{
						/* Endlosschleife ? */
						if( !prod_hash[prod_info[i].prod_nr].defined )
						{
							/* Nein, FOLLOW( i ) bilden */
							if( gen_f( prod_info[i].prod_nr ) < 0 )
								/* Fehler */
								return( -1 );
						}
					}

					/* Speicherbereich kopieren */
					if( cmp_memcpy( &prod_hash[nterm].follow, &prod_hash[prod_info[i].prod_nr].follow ) < 0 )
						/* Fehler */
						return( -1 );
				}

				/* Folgt noch ein Element ?*/
				if( ( anz_elem -1 ) > j )
				{
					/* FOLLOW( nterm ) += FIRST( j + 1 ) */
					/* Drachenbuch Regel 2 */

					/* Speicherbereich kopieren */
					if( cmp_memcpy( &prod_hash[nterm].follow, &first ) < 0 )
						/* Fehler */
						return( -1 );
				}
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


WORD gen_first( FIRST *first, LONG nterm, WORD *state )
{
	/*	Hier werden die FIRST-Mengen gebildet. */
	ULONG	prod_list_count;
	WORD	anz_ri, ri_count;

	/* Kein Epsilon */
	*state = 0;

	/* Die laufende Nummer zu nterm holen */
	prod_list_count = prod_hash[nterm].prod_list_nr;

	/* Alle Alternativen der Produktion bearbeiten */
	do
	{
		/* Ist nterm ein Terminal ? */
		if( prod_hash[nterm].prod_type == TERM )
		{
			/* Ist es Epsilon ? */
			if( nterm == epsilon_hash_ind )
				/* Es ist Epsilon */
				(*state)++;

			/* Terminal in fst eintragen */
			if( insert_first( nterm, first ) < 0 )
				/* Fehler */
				return( -1 );

			/* NÑchste Alternative bearbeiten */
			continue;
		}

		else
		{
			/* A -> Y1 Y2 Y3 */

			/* Anzahl der Elemente auf der rechten Seite holen */
			anz_ri = prod_info[prod_list_count].prod_ri_anz;

			/* ZÑhler */
			ri_count = 0;

			while( anz_ri > ri_count )
			{
				/*	Stimmt das erste Element auf der rechten Seite
					mit nterm Åberein ?
				*/
				if( nterm == prod_info[prod_list_count].prod_ri[ri_count] )
					/* Ja, mit der nÑchsten Alternative weitermachen */
					break;

				/*	FIRST-Mengen zum ersten Element auf der rechten
					Seite bilden.
				*/
				gen_first( first, prod_info[prod_list_count].prod_ri[ri_count], state );

				/* War das erste Element auf der rechten Seite ein Epsilon ? */
				if( *state != 0 )
					/* Ja */
					ri_count++;

				else
					/* NÑchste Alternative untersuchen */
					break;
			}
		}

	}while( prod_info[prod_list_count++].prod_fl );

	/* Ist FIRST leer ? */
	if( !first->anz_elem )
	{
		/* EPSILON eintragen */
		if( insert_first( epsilon_hash_ind, first ) < 0 )
			/* Fehler */
			return( -1 );

		/* Epsilon */
		(*state)++;
	}

	/* Alles OK */
	return( 0 );
}

WORD insert_first( LONG nterm, FIRST *first )
{
	/* In dieser Routine werden die Terminale in das FIRST-Array eingetragen. */
	REG		ULONG i;

	/* Findet ein öberlauf statt ? */
	if( first->anz_elem == first->max_anz_elem )
	{
		/* Neue Anzahl der max. Elemente */
		first->max_anz_elem += 10;

		/* GenÅgend Speicher anfordern */
		if( !( first->arr = ( ULONG * ) realloc( ( VOID * ) first->arr, sizeof( ULONG ) * first->max_anz_elem ) ) )
			/* Fehler */
			return( error( NOT_ENOUGH_MEMORY, 0 ) );
	}

	/* Ist das Item schon eingetragen worden ? */
	for( i = 0; i < first->anz_elem; i++ )
	{
		/* Gleich ? */
		if( nterm == first->arr[i] )
			/* Alles OK */
			return( 0 );
	}

	/* Element eintragen */
	first->arr[first->anz_elem++] = nterm;

	/* Alles OK */
	return( 0 );
}

WORD cmp_memcpy( FIRST *dest, FIRST *source )
{
	/* Hier werden doppelte Elemente nicht Åbernommen. */
	REG ULONG	i, j;

	/* Alle Elemente miteinander vergleichen */
	for( i = 0; i < source->anz_elem; i++ )
	{
		for( j = 0; j < dest->anz_elem; j++ )
		{
			/* Sind die Elemente gleich ? */
			if( dest->arr[j] == source->arr[i] )
				/* Elemente sind gleich */
				break;
		}

		/* Ist es EPSILON ? */
		if( source->arr[i] != epsilon_hash_ind )
		{
			/* Waren die Elemente ungleich ? */
			if( j == dest->anz_elem )
			{
				/* Ist noch Platz fÅr ein Element ? */
				if( dest->max_anz_elem == dest->anz_elem )
				{
					/* FÅr 5 weitere Elemente Platz schaffen */
					dest->max_anz_elem += 5;

					/* GenÅgend Speicher anfordern */
					if( !( dest->arr = ( ULONG * ) realloc( ( VOID * ) dest->arr, sizeof( ULONG ) * dest->max_anz_elem ) ) )
						/* Fehler */
						return( error( NOT_ENOUGH_MEMORY, 0 ) );
				}

				/* Element in dest aufnehmen */
				dest->arr[dest->anz_elem++] = source->arr[i];
			}
		}
	}

	/* Alles OK */
	return( 0 );
}


VOID show_follow( VOID )
{
	/* Diese Routine zeigt die FOLLOW-Mengen der Grammatik an. */
	REG ULONG	i, j;
	FIRST		*follow;

	/* Alle Produktionen bearbeiten */
	for( i = 0; i < nterm.anz_elem; i++ )
	{
		/* Name der Produktion ausgeben */
		fprintf( file, "\n\n%s", prod_hash[nterm.arr[i]].prod_name );
		fprintf( file, "\n-------------------------------------------" );

		/* Alle Terminale ausgeben */
		for( j = 0; j < prod_hash[nterm.arr[i]].follow.anz_elem; j++ )
		{
			/* Zeiger auf die FOLLOW-Mengen holen */
			follow = &prod_hash[nterm.arr[i]].follow;

			/* Terminal ausgeben */
			fprintf( file, "\n%s", prod_hash[follow->arr[j]].prod_name );
		}

		/* Ausgabe auf Bildschirm ? */
		if( slr_info.dest == DEST_SCREEN )
			/* Auf Tastendruck warten */
			Cnecin();
	}
}


WORD make_tab( VOID )
{
	/* Hier werden die Tabellen des SLR-Parsers erzeugt. */
	BYTE		nterm[256];
	WORD		pkt_pos, term_count, max_term;
	REG ULONG	i, j, p;
	ULONG		temp_bel_hulle = anz_bel_hulle, h_count, prod_count;
	ULONG		item_count, prod_priori, term_priori, pr_list_nr;
	FIRST		*first;

	/* Anzahl der Produktionen ausmachen */
	for( h_count = 0, prod_count = 0; h_count <= prod_list_nr; h_count++, prod_count++ )
	{
		/* Nummer an die Produktion vergeben */
		prod_hash[prod_info[h_count++].prod_nr].index = prod_count;
		
		while( prod_info[h_count].prod_fl )
			prod_hash[prod_info[h_count++].prod_nr].index = prod_count;
	}

	/* Speicher fÅr die ZustandsÅbergÑnge-Tabelle anfordern */
	if( init_twodim( ( VOID *** ) &goto_tab, PTR_ULONG, anz_bel_hulle + 1, anz_prod ) < 0 )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Speicher fÅr die SLR-Tabelle anfordern */
	if( init_twodim( ( VOID *** ) &slr_tab, PTR_SLR_TAB, anz_bel_hulle + 1, term_str.anz_elem + 1 ) < 0 )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Speicher fÅr Ziel-Tabelle anfordern */
	if( !( ziel_tab = ( ULONG * ) calloc( anz_bel_hulle + 1, sizeof( ULONG ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* Speicher fÅr ein Terminal anfordern */
	if( !( term_str.arr = ( ULONG * ) realloc( ( VOID * ) term_str.arr, ( term_str.anz_elem + 1 ) * sizeof( ULONG ) ) ) )
		/* Fehler */
		return( error( NOT_ENOUGH_MEMORY, 0 ) );

	/* ACCEPT eintragen */
	term_str.arr[term_str.anz_elem++] = accept_hash_ind;

	/* In Zustand 2 / Endesymbol ACCEPT einsetzen */
	slr_tab[1][term_str.anz_elem - 1].value = 0;

	/* Ist ein Reduce-Eintrag */
	slr_tab[1][term_str.anz_elem - 1].state = REDUCE;

	/* Als erstes die ZustandsÅbergÑnge fÅr die Nichtterminale erzeugen. */
	for( h_count = 0; h_count <= anz_bel_hulle; h_count++ )
	{
		/* Alle Nichtterminale bearbeiten */
		for( prod_count = 1; prod_count <= prod_list_nr; prod_count++ )
		{
			/* GOTO( Ih_count, prod_count ) */
			if( ( _goto( ober_hulle.hulle[h_count], prod_info[prod_count].prod_nr, h_count, 0 ) ) != 0UL )
				/* Fehler */
				return( -1 );

			/* Ist Åberhaupt eine HÅlle erzeugt worden ? */
			if( anz_bel_hulle > temp_bel_hulle )
			{
				/* Die HÅlle mit den restlichen HÅllen vergleichen */
				for( p = 0; p <= temp_bel_hulle; p++ )
				{
					/* Ist die HÅlle schon vorhanden ? */
					if( !cmp_hulle( ober_hulle.hulle[p], ober_hulle.hulle[anz_bel_hulle] ) )
						/* HÅlle war vorhanden */
						goto_tab[h_count][prod_hash[prod_info[prod_count].prod_nr].index] = p;
				}

				/* Speicher fÅr die Items wieder freigeben */
				for( i = 0; i < ober_hulle.hulle[anz_bel_hulle]->anz_items; i++ )
					/* Speicher freigeben */
					free( ( VOID * ) ober_hulle.hulle[anz_bel_hulle]->items_array[i] );

				/* Speicher fÅr Itemsarray freigeben */
				free( ( VOID * ) ober_hulle.hulle[anz_bel_hulle]->items_array );

				/* HÅlle wieder lîschen */
				anz_bel_hulle--;
			}

			/* Alle Alternativen Åberspringen */
			while( prod_info[prod_count].prod_fl )
				prod_count++;
		}
	}

	/* Die Shift-EintrÑge erzeugen */

	/* Alle HÅllen durchlaufen */
	for( h_count = 0; h_count <= anz_bel_hulle; h_count++ )
	{
		/* Alle Terminale bearbeiten */
		for( prod_count = 0; prod_count < term_str.anz_elem; prod_count++ )
		{
			/* Alle Items durchlaufen */
			for( item_count = 0; item_count < ober_hulle.hulle[h_count]->anz_items; item_count++ )
			{
				/* Position des Punktes */
				pkt_pos = ober_hulle.hulle[h_count]->items_array[item_count]->pkt_pos;

				/*
					Ist der Punkt hinter dem letzten Element auf der
					rechten Seite ?
				*/
				if( pkt_pos < prod_info[ober_hulle.hulle[h_count]->items_array[item_count]->prod_list_nr].prod_ri_anz )
				{
					/* Ist das Terminal auf der rechten Seite vor dem Punkt ? */
					if( prod_info[ober_hulle.hulle[h_count]->items_array[item_count]->prod_list_nr].prod_ri[pkt_pos] == term_str.arr[prod_count] )
					{
						/* GOTO( Ih_count, prod_count ) */
						if( ( _goto( ober_hulle.hulle[h_count], term_str.arr[prod_count], h_count, 0 ) ) != 0UL )
							/* Fehler */
							return( -1 );

						/* Ist Åberhaupt eine HÅlle erzeugt worden ? */
						if( anz_bel_hulle > temp_bel_hulle )
						{
							/* Die HÅlle mit den restlichen HÅllen vergleichen */
							for( p = 0; p <= temp_bel_hulle; p++ )
							{
								/* Ist die HÅlle schon vorhanden ? */
								if( !cmp_hulle( ober_hulle.hulle[p], ober_hulle.hulle[anz_bel_hulle] ) )
								{
									/* HÅlle war vorhanden */
									slr_tab[h_count][prod_count].value = p;

									/* Ist ein Shift-Eintrag */
									slr_tab[h_count][prod_count].state = SHIFT;

									/* FÅr automatische Fehlerbehandlung
									shift_err[p] = h_count;
									*/

									/* NÑchstes Item testen */
									break;
								}
							}

							/* Speicher fÅr die Items wieder freigeben */
							for( i = 0; i < ober_hulle.hulle[anz_bel_hulle]->anz_items; i++ )
								/* Speicher freigeben */
								free( ( VOID * ) ober_hulle.hulle[anz_bel_hulle]->items_array[i] );

							/* Speicher fÅr Itemsarray freigeben */
							free( ( VOID * ) ober_hulle.hulle[anz_bel_hulle]->items_array );

							/* HÅlle wieder lîschen */
							anz_bel_hulle--;
						}
					}
				}
			}
		}
	}

	/* Die Reduce-Tabelle erzeugen */

	/* Alle HÅllen durchlaufen */
	for( h_count = 0; h_count <= anz_bel_hulle; h_count++ )
	{
		/* Alle Items durchlaufen */
		for( item_count = 0; item_count < ober_hulle.hulle[h_count]->anz_items; item_count++ )
		{
			/* Index der Produktion merken */
			pr_list_nr = ober_hulle.hulle[h_count]->items_array[item_count]->prod_list_nr;

			/* Ist es die Startproduktion ? */
			if( !pr_list_nr )
				/* ACCEPT einsetzen */
				slr_tab[h_count][term_str.anz_elem].value = 0;

			else
			{
				/* Position des Punktes */
				pkt_pos = ober_hulle.hulle[h_count]->items_array[item_count]->pkt_pos;

				/*
					Ist der Punkt hinter dem letzten Element auf der
					rechten Seite ?
				*/
				if( pkt_pos == prod_info[pr_list_nr].prod_ri_anz )
				{
					/* Zeiger auf FOLLOW-Menge holen */
					first = &prod_hash[prod_info[pr_list_nr].prod_nr].follow;

					/* Reduziere-Aktion in Tabelle eintragen */
					for( p = 0; p < first->anz_elem; p++ )
					{
						/* Terminal holen */
						prod_count = first->arr[p];

						/* Index des Terminals holen */
						for( i = 0; i < term_str.anz_elem; i++ )
						{
							/* Ist es das gesuchte Terminal ? */
							if( prod_count == term_str.arr[i] )
							{
								/* Ja, Index merken */
								prod_count = i;

								/* For-Schleife beenden */
								break;
							}
						}

						/* Reduce/Reduce- bzw. Shift/Reduce-Fehler ? */
						if( slr_tab[h_count][prod_count].state )
						{
							/*
								Es ist ein Shift/Reduce bzw. ein Reduce/-
								Reduce-Fehler aufgetreten.
							*/
							if( ( slr_tab[h_count][prod_count].state == REDUCE ) && ( prod_hash[prod_info[pr_list_nr].prod_nr].defined >= 0 ) )
							{
								/* Es ist ein Reduce/Reduce-Fehler aufgetreten*/
								fprintf( file, "\n\nReduce/Reduce-Konflikt in HÅlle %lu/Symbol %s !", h_count, prod_hash[term_str.arr[prod_count]].prod_name );

								/* Meldung soll nicht nocheinmal erscheinen */
								prod_hash[prod_info[pr_list_nr].prod_nr].defined = -1;
							}

							else
							{
								/* Es ist ein Shift/Reduce-Fehler aufgetreten */

								/* PrioritÑt des Terminals holen */
								term_priori = prod_hash[term_str.arr[prod_count]].priori;

								/* PrioritÑt der Produktion holen */
								prod_priori = prod_info[pr_list_nr].priori;

								/* Meldung ausgeben */
								fprintf( file, "\n\nShift/Reduce-Konflikt in HÅlle %lu/Symbol %s !", h_count, prod_hash[term_str.arr[prod_count]].prod_name );

								/* Schieben oder reduzieren ? */
								if( s_r( prod_priori, term_priori, prod_info[pr_list_nr].assoc ) == SHIFT )
								{
									/* Schieben */
									fprintf( file, "\nSchiebe %s...", prod_hash[term_str.arr[prod_count]].prod_name );
									break;
								}

								else
									/* Reduzieren */
									fprintf( file, "\nReduziere zu %s...", prod_hash[prod_info[pr_list_nr].prod_nr].prod_name );
							}
						}

						/* Index der Produktion merken */
						slr_tab[h_count][prod_count].value = pr_list_nr;

						/* Ist ein Reduce-Eintrag */
						slr_tab[h_count][prod_count].state = REDUCE;

						/* Merken */
						ziel_tab[h_count] = pr_list_nr;
					}
				}
			}
		}
	}

	/* Alle leeren EintrÑge in einem Reduce-ZustÑnde angleichen */
	test_of_reduce();

	/* Tabelleninformationen ausgeben ? */
	if( slr_info.slr_tab_1 == ON )
	{
		/* Meldung ausgeben */
		fprintf( file, "\n\nSLR-Tabelle" );

		/* Alle Terminale ausgeben */
		for( i = 0; i < term_str.anz_elem - 1; i++ )
		{
			/* Terminal ausgaben */
			fprintf( file, "\n\n\n\nTerminal: %s\n", prod_hash[term_str.arr[i]].prod_name );

			/* Alle HÅllen durchgehen */
			for( j = 0; j <= anz_bel_hulle; j++ )
			{
				/* Schieben oder reduzieren ? */
				if( slr_tab[j][i].state == SHIFT )
					/* Schieben */
					fprintf( file, "\nState %3lu: Schiebe '%s' ( Neuer State: %lu )", j, prod_hash[term_str.arr[i]].prod_name, slr_tab[j][i].value );

				else if( slr_tab[j][i].state == REDUCE )
				{
					/* Index der Produktion holen */
					pr_list_nr = slr_tab[j][i].value;

					/* Hat die Produktion Åberhaupt Alternativen ? */
					if( ( pr_list_nr ) && !( prod_info[pr_list_nr].prod_fl ) && !( prod_info[pr_list_nr - 1].prod_fl ) )
						/* Namen der Produktion ausgeben */
						fprintf( file, "\nState %3lu: Reduziere zu %s", j, prod_hash[prod_info[slr_tab[j][i].value].prod_nr].prod_name );

					else if( !( pr_list_nr ) && !( prod_info[pr_list_nr].prod_fl ) )
						/* Namen der Produktion ausgeben */
						fprintf( file, "\nState %3lu: Reduziere zu %s", j, prod_hash[prod_info[slr_tab[j][i].value].prod_nr].prod_name );

					else
					{
						/* Wenn es eine Alternative ist, Grungproduktion suchen */
						pr_list_nr--;

						/* Ist es eine Alternative ? */
						while( prod_info[pr_list_nr].prod_fl )
							/* Ja */
							pr_list_nr--;

						/* Dies ist die Grundproduktion */
						pr_list_nr = slr_tab[j][i].value - ( pr_list_nr + 1 );

						/* Namen der Produktion ausgeben */
						fprintf( file, "\nState %3lu: Reduziere zu %s_%lu", j, prod_hash[prod_info[slr_tab[j][i].value].prod_nr].prod_name, pr_list_nr );
					}
				}
			}

			/* Ausgabe auf Bildschirm ? */
			if( slr_info.dest == DEST_SCREEN )
				/* Auf Tastendruck warten */
				Cnecin();
		}
	}

	/* SLR_TAB ausgeben ? */
	if( slr_info.slr_tab_2 == ON )
	{
		/* Nun die SLR-Tabelle in Tabellenform ausgeben */
		fprintf( file, "\n\n\nTabellarische SLR-Tabelle\n" );

		/* Anzahl der Terminale, die auf dem Bildschirm passen wÅrden */
		max_term = ( WORD ) ( slr_info.max_char / ( nterm_length + 2 ) ) - 1;

		/* Terminal-ZÑhler */
		term_count = 0;

		/* Alle Terminale bearbeiten */
		while( term_count < term_str.anz_elem )
		{
			/* NÑchste Zeile */
			fprintf( file, "\n\n\n\t\t|" );

			/* Alle Terminale ausgeben */
			for( i = term_count; i < ( term_count + max_term ) && ( i < ( term_str.anz_elem - 1 ) ); i++ )
				/* Terminal ausgeben */
				fprintf( file, " %-*s |", ( WORD ) nterm_length + 2, prod_hash[term_str.arr[i]].prod_name );

			/* Ist es das letzte Terminal ? */
			if( i == ( term_str.anz_elem - 1 ) )
				/* ACCEPT ausgeben */
				fprintf( file, " %-*s |", ( WORD ) nterm_length + 2, "ACCEPT" );

			/* Alle HÅllen durchgehen */
			for( i = 0; i <= anz_bel_hulle; i++ )
			{
				/* Nummer der HÅlle ausgeben */
				fprintf( file, "\n\t%4lu|", i );

				/* Alle Terminale durchgehen */
				for( j = term_count; j < ( term_count + max_term ) && ( j < term_str.anz_elem ); j++ )
				{
					/* Shift ? */
					if( slr_tab[i][j].state == SHIFT )
						/* Wert ausgeben */
						fprintf( file, " s%-*lu |", ( WORD ) nterm_length + 1, slr_tab[i][j].value );

					else if( slr_tab[i][j].state == REDUCE )
					{
						/* Index der Produktion holen */
						pr_list_nr = slr_tab[i][j].value;

						/* Hat die Produktion Åberhaupt Alternativen ? */
						if( ( pr_list_nr ) && !( prod_info[pr_list_nr].prod_fl ) && !( prod_info[pr_list_nr - 1].prod_fl ) )
							/* Namen der Produktion ausgeben */
							fprintf( file, " %-*s |", ( WORD ) nterm_length + 2, prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name );

						else if( !( pr_list_nr ) && !( prod_info[pr_list_nr].prod_fl ) )
							/* Namen der Produktion ausgeben */
							fprintf( file, " %-*s |", ( WORD ) nterm_length + 2, prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name );

						else
						{
							/* Wenn es eine Alternative ist, Grundproduktion suchen */
							pr_list_nr--;

							/* Ist es eine Alternative ? */
							while( prod_info[pr_list_nr].prod_fl )
								/* Ja */
								pr_list_nr--;

							/* Dies ist die Grundproduktion */
							pr_list_nr = slr_tab[i][j].value - ( pr_list_nr + 1 );

							/* Namen der Produktion ausgeben */
							sprintf( nterm, "%s_%lu", prod_hash[prod_info[slr_tab[i][j].value].prod_nr].prod_name, pr_list_nr );
							fprintf( file, " %-*s |", ( WORD ) nterm_length + 2, nterm );
						}
					}

					else
						/* Fehlereintrag */
						fprintf( file, " -%-*lu |", ( WORD ) nterm_length + 1, slr_tab[i][j].value );
				}
			}

			/* NÑchsten Terminale ausgeben */
			term_count += max_term;

			/* Ausgabe auf Bildschirm ? */
			if( slr_info.dest == DEST_SCREEN )
				/* Auf Tastendruck warten */
				Cnecin();
		}
	}

	/* Ausgabe auf Bildschirm ? */
	if( slr_info.dest == DEST_SCREEN )
		/* Auf Tastendruck warten */
		Cnecin();

	/* HÅllen anzeigen */
	show_hulle();

	/* Alles OK */
	return( 0 );
}

WORD s_r( ULONG prod_priori, ULONG term_priori, ULONG prod_assoc )
{
	/*
		Hier wird festgestellt, ob bei einem s/r-Konflikt reduziert oder
		geschoben werden soll.
	*/

	/* Schieben oder reduzieren */
	if( prod_priori > term_priori )
		/* Reduzieren */
		return( REDUCE );

	else if( ( prod_priori == term_priori ) && ( prod_assoc == LEFT ) )
		/* Reduzieren */
		return( REDUCE );

	/* Schieben */
	return( SHIFT );
}

VOID test_of_reduce( VOID )
{
	/*
		Gibt es nur Reduces in der HÅlle, so werden auch alle LeereintrÑge
		auf Reduce gesetzt.
	*/
	REG ULONG	i, j, reduce;

	/* Alle HÅllen */
	for( j = 0; j <= anz_bel_hulle; j++ )
	{
		/* Alle Terminale durchgehen */
		for( i = 0; i < term_str.anz_elem; i++ )
		{
			/* Ist es ein Reduce-Eintrag ? */
			if( slr_tab[j][i].state == REDUCE )
			{
				/* Den Wert merken */
				reduce = slr_tab[j][i].value;

				/* Alle Terminale durchgehen */
				for( i = 0; i < term_str.anz_elem; i++ )
				{
					/* Leereintrag ? */
					if( !slr_tab[j][i].state )
					{
						/* Alle LeereintrÑge auf Reduce setzen */
						slr_tab[j][i].state = REDUCE;
						slr_tab[j][i].value = reduce;
					}
				}
			}
		}
	}
}