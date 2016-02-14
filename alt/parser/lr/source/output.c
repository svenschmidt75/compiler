/*
	Projekt		:	LR-Parser-Generator V1.00û
	Modul		:	OUTPUT.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/



/************/
/* Includes */
/************/

#include <portab.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tos.h>

/* Include-Datei des Parser-Generators */
#include "LR.H"



/**************/
/* Prototypen */
/**************/

WORD generate_first( FIRST_STR *first, LONG term, LONG index, WORD e_state );
WORD error( WORD error_number, ULONG para1, ULONG para2  );
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

	/* Dateiendung Ñndern */
	temp_file_name = strrchr( file_name, ( WORD ) '.' );
	strcpy( temp_file_name, ".GRA" );

	/* Datei zum Schreiben îffnen */
	bnf_file = freopen( file_name, "w", 0L );

	/* Namen der Grammatik ausgeben */
	fprintf( bnf_file,	";Grammatik %s in BNF-Form"
						"\n;Generiert von LR-Parser-Generator V0.00û"
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
			fprintf( bnf_file, "\n\t\t|\t" );

			/* Rechte Seite ausgeben */
			output_right( bnf_file, i );
		}
	}

	/* Datei schliessen */
	if( fclose( bnf_file ) < 0 )
		/* Fehler */
		return( error( CANNOT_CLOSE_BNF_FILE, 0, 0 ) );

	/* ZurÅcksetzen */
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
	fprintf( file, "\n\n\n****************\n* FIRST-Mengen *\n"
				   "****************\n" );

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
	fprintf( file, "\n\n\n*****************\n* FOLLOW-Mengen *\n"
				   "*****************\n" );

	/* Alle NTERM_STRs */
	for( i = 0; i <= nterm_count; )
	{
		/* Nichtterminal optisch trennen */
		for( j = 0; j < ( WORD ) strlen( hash_tab[nterm_str[i]->hash_tab_index].name ); j++ )
			/* - schreiben */
			sep[j] = '-';

		/* Null-Byte */
		sep[j] = 0;

		/* Nichtterminal ausgeben */
		fprintf( file, "\n\n%s\n%s\n\n", hash_tab[nterm_str[i]->hash_tab_index].name, sep );

		/* FOLLOW-Menge ausgeben */
		for( j = 0; j < nterm_str[i]->follow.anz_elem; j++ )
			/* Terminal ausgeben */
			fprintf( file, "%s\n", hash_tab[nterm_str[i]->follow.term_arr[j]].name );

		/* Die Alternativen Åberspringen */
		while( ( ++i <= nterm_count ) && ( nterm_str[i]->alt ) )
			;
	}

	/* Alles OK */
	return( 0 );
}


WORD output_items( VOID )
{
	/* Hier werden die SLR- bzw. LR-Items ausgegeben. */
	REG LONG	i;

	/* Meldung ausgeben */
	if( !lr_info.analyze )
	{
		/* SLR-Parser ? */
		if( lr_info.type == SLR )
			/* Meldung ausgeben */
			fprintf( file, "\n\n\n***************\n"
						   "* LR(0)-Items *\n***************\n" );

		else
			/* Meldung ausgeben */
			fprintf( file, "\n\n\n***************\n"
						   "* LR(1)-Items *\n***************\n" );
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
			fprintf( file, "\nKern:\t" );

		else
			/* Nein */
			fprintf( file, "\n\t\t" );

		/* Ist es ein Reduce-Item ? */
		if( item->pkt_pos < nterm->anz_ri )
		{
			/*
				Nun den Zustand ausgeben, den der Parser erreicht, wenn er das
				Element nach dem Punkt auf der rechten Seite geschoben hat.
			*/
			/* Ist es ein Terminal ? */
			if( hash_tab[nterm->ri[item->pkt_pos]].term_str )
			{
				/* Ja, ist ein Terminal */

				/* Index des Terminals holen */
				if( ( index = get_term_index( nterm->ri[item->pkt_pos] ) ) < 0 )
					/* Fehler */
					return( -1 );

				/* Aufgrund Shift-/Reduce-Error reduzieren ? */
				if( lr_tab[hulle_index][index].state == REDUCE )
					/* Ja */
					fprintf( file, "\t\t" );

				else
					/* Zustand ausgeben */
					fprintf( file, "%3ld:\t", lr_tab[hulle_index][index].value );
			}

			else
				/* Zustand ausgeben */
				fprintf( file, "%3ld:\t", jump_tab[hulle_index][nterm_str[hash_tab[nterm->ri[item->pkt_pos]].nterm_index]->prod_index - 1] );
		}

		else
			/* Ist ein Reduce-Item */
			fprintf( file, "\t\t" );

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

		for( j = 0; j < nterm->anz_ri; j++ )
		{
			/* Punkt ausgeben ? */
			if( item->pkt_pos == j )
				/* Punkt ausgeben */
				fprintf( file, "." );

			/* Ist es ein Terminal ? */
			if( hash_tab[nterm->ri[j]].term_str )
				/* Ja, ist ein Terminal */
				fprintf( file, " \'%s\'", hash_tab[nterm->ri[j]].name );

			else
				/* Ist ein Nichtterminal */
				fprintf( file, " %s", hash_tab[nterm->ri[j]].name );
		}

		/* Punkt ausgeben ? */
		if( item->pkt_pos == j )
			/* Punkt ausgeben */
			fprintf( file, "." );

		/* Terminal ausgeben ? */
		if( item->term.anz_elem )
		{
			/* Ja */
			for( j = 0; j < item->term.anz_elem; j++ )
				/* Terminal ausgeben */
				fprintf( file, ", %s", hash_tab[item->term.term_arr[j]].name );
		}
	}

	/* Ausgabe auf Bildschirm ? */
	if( lr_info.dest == DSCREEN )
		/* Auf Tastendruck warten */
		Cnecin();

	/* Alles OK */
	return( 0 );
}


WORD output_lr_tab( VOID )
{
	/* Hier wird die LR-Tabelle ausgegeben. */
	WORD		max_term;
	REG ULONG	t_count, eps_flg, i, j, nterm_index;
	BYTE		nterm[256], sep[256];

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
	fprintf( file, "\n\n\n**************\n* LR-Tabelle *\n**************" );

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
		fprintf( file, "\n\n\n\t\t|" );

		/* Alle Terminale bearbeiten */
		for( i = t_count; ( i <= ( t_count + max_term ) ) && ( i <= term_count ); i++ )
			/* Terminal ausgeben */
			fprintf( file, " %-*s |", ( WORD ) nterm_length, hash_tab[term_str[i].hash_tab_index].name );

		/* Optische Trennung */
		fprintf( file, "\n\t\t|" );

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
		t_count += max_term;
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
	REG ULONG	i, j, k;

	/* Meldung ausgeben */
	fprintf( file, "\n\n\n****************\n* GOTO-Tabelle *\n"
				   "****************\n\n" );

	/* Alle HÅllen */
	for( i = 0; i <= hulle_count; i++ )
	{
		/* Alle  NTERM_STRs */
		for( j = 1, k = 1; k <= prod_count; k++ )
		{
			/* Gibt es einen Sprung ? */
			if( jump_tab[i][k] )
				/* Ja */
				fprintf( file, "\nGOTO( I%lu, %*s ) = I%lu", i, ( WORD ) nterm_length, hash_tab[nterm_str[j]->hash_tab_index].name, jump_tab[i][k] );

			/* Alternativen Åberspringen */
			while( ( ++j <= nterm_count ) && ( nterm_str[j]->alt ) )
				;
		}
	}
}