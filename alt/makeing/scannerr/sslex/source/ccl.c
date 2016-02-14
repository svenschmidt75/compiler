/*
	Projekt		:	Scanner-Generator
	Modul		:	CCL.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	25.07.1997
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

VOID del_line( BYTE *ch );
WORD get_char( VOID );



/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/


/* EnthÑlt alle Zeichen, die erlaubt/genutzt werden als CCL(Bit-kodiert). */
CCL_STR		all_bchar;

/* EnthÑlt alle Zeichen, die erlaubt/genutzt werden als Array(Zeichenweise). */
UBYTE		all_char[255];

/* Anzahl der EintrÑge in all_char */
WORD		all_char_count;




/***********/
/* Imports */
/***********/

MGLOBAL	NEA_STR	**nea_str_arr;
MGLOBAL ULONG	zeile;
MGLOBAL	LONG	nea_str_arr_count;
MGLOBAL BYTE	*grammar;




VOID insert_ccl( CCL_STR *ccl, BYTE from, BYTE to )
{
	/* Hier wird eine Zeichenklasse erzeugt. */
	REG UBYTE	i;

	/* Auf Fehler prÅfen */
	if( ( from < 0 ) || ( to < 0 ) || ( from > to ) )
		/* Fehler */
		fprintf( stderr, "\nZeile %lu: Fehler in Zeichenbereich !" );

	/* Positive Zeichenklasse */
	for( i = from; i <= to; i++ )
		/* Bit setzen */
		ccl->ccl[i / CHAR_BIT] |= 1 << ( i % CHAR_BIT );

	/* Niedrigstes Zeichen */
	ccl->lw_char = from;

	/* Letztes Zeichen */
	ccl->hi_char = to;
}


VOID negate_ccl( CCL_STR *ccl )
{
	/* Zeichenklasse negieren */
	REG WORD	i;

	/* Alle Bytes negieren */
	for( i = 0; i < CCL_WIDTH; i++ )
	{
		/* XOR */
		ccl->ccl[i] ^= 0xff;

		/* Nun die ungÅltigen Zeichen ausblenden */
		ccl->ccl[i] &= all_bchar.ccl[i];
	}

	/* Das erste gesetzte Bit finden */
	for( i = all_bchar.lw_char; i < all_bchar.hi_char; i++ )
	{
		/* Bit gesetzt ? */
		if( ccl->ccl[i / CHAR_BIT] & ( 1 << ( i % CHAR_BIT ) ) )
		{
			/* Ja, Position merken */
			ccl->lw_char = i;
			break;
		}
	}

	/* Das letzte gesetzte Bit suchen */
	for( i = all_bchar.hi_char; i >= all_bchar.lw_char; i-- )
	{
		/* Bit gesetzt ? */
		if( ccl->ccl[i / CHAR_BIT] & ( 1 << ( i % CHAR_BIT ) ) )
		{
			/* Ja, Position merken */
			ccl->hi_char = i;
			break;
		}
	}
}


VOID check_ccl( CCL_STR *ccl )
{
	/* Hier wird eine CCL auf ungÅltige Zeichen untersucht. */
	REG WORD	i, j;

	/* Alle Zeichen */
	for( i = ccl->lw_char; i < ccl->hi_char; i++ )
	{
		/* Ist das Zeichen gesetzt ? */
		if( ccl->ccl[i / CHAR_BIT] & ( 1 << ( i % CHAR_BIT ) ) )
		{
			/* Ja, darf es das auch ? */
			if( !( all_bchar.ccl[i / CHAR_BIT] & ( 1 << ( i % CHAR_BIT ) ) ) )
			{
				/* Nein, Meldung ausgeben */
				fprintf( stderr, "\nZeile %lu: Zeichen %c ist undefiniert !", zeile, ( BYTE ) i );

				/* Zeichen ab jetzt erlauben */
				all_bchar.ccl[i / CHAR_BIT] |= ( 1 << ( i % CHAR_BIT ) );

				/* Zeichen in all_char eintragen */
				for( j = 0; j < all_char_count; j++ )
				{
					/* Ist es das gesuchte Zeichen ? */
					if( i < all_char[j] )
					{
						/* Zeichen um einen Eintrag verschieben */
						memcpy( ( VOID * ) &all_char[j + 1], ( VOID * ) &all_char[j], ( ULONG ) ( all_char_count - j ) );

						/* Zeichen eintragen */
						all_char[j] = i;
						all_char_count++;

						/* Die Schleife verlassen */
						break;
					}
				}

				/* Wurde das Zeichen eingetragen ? */
				if( j == all_char_count )
					/* Nein */
					all_char[all_char_count++] = i;
			}
		}
	}
}


VOID do_defchar( WORD flag )
{
	/* Hier werden die erlaubten Zeichen ausgewertet. */
	REG WORD	i, j;
	WORD		beg = -1, end = -1;

	/* Default oder User-Defined ? */
	if( !flag )
	{
		/* User-Defined */
		grammar += 9;

		/* Blîdsinn lîschen */
		delete();

		/* { lîschen */
		grammar++;

		/* Blîdsinn lîschen */
		delete();

		/* Solange nicht das Ende der Definition erreicht wurde */
		while( *grammar != '}' )
		{
			/* Zeichen extrahieren */
			beg = get_char();

			/* Zeichenbereich ? */
			if( *grammar == '-' )
			{
				/* Ja, nÑchsten Wert holen */
				grammar++;
				end = get_char();

				/* Ist ein Wert kleiner als der andere ? */
				if( end < beg )
				{
					/* Warnung ausgeben */
					fprintf( stderr, "\nZeile %lu: Bereichsangabe Fehlerhaft !(Wert1 grîûer Wert2)." );

					/* Werte vertauschen */
					i = end;
					end = beg;
					beg = i;
				}
			}

			else if( *grammar == ',' )
			{
				/* Nun den Wert eintragen */
				grammar++;
				end = beg;
			}

			else if( ( *grammar == '\n' ) || ( *grammar == 0x0d ) )
				/* Ein Wert */
				end = beg;

			else
			{
				/* Fehler */
				fprintf( stderr, "\nZeile %lu: Fehler in der Zeichenbereichsangabe !", zeile );

				/* Zeile lîschen */
				del_line( grammar );
			}

			/* Wert setzen */
			for( i = beg; i <= end; i++ )
				/* Bit setzen */
				all_bchar.ccl[i / CHAR_BIT] |= ( 1 << ( i % CHAR_BIT ) );

			/* Blîdsinn lîschen */
			delete();
		}
	}

	else
	{
		/* Default setzen */
		for( i = 0; i < 127; i++ )
			/* Default-Werte setzen */
			all_bchar.ccl[i / CHAR_BIT] |= ( 1 << ( i % CHAR_BIT ) );

		/* Warnung ausgeben */
		fprintf( stderr, "\nEr wurde keine Zeichenbereichsdefinition vorgenommen !(Default: 0-127)" );
	}

	/* Nun die Bit-Codierung in Zeichen umsetzen */
	for( i = j = 0; i < 255; i++ )
	{
		/* Ist das Bit in all_bchar hesetzt ? */
		if( all_bchar.ccl[i / CHAR_BIT] & ( 1 << ( i % CHAR_BIT ) ) )
			/* Ja, in all_char eintragen */
			all_char[j++] = i;
	}

	/* Anzahl der EintrÑge */
	all_char_count = j;

	/* Den niedrigsten Wert eines Zeichen merken */
	all_bchar.lw_char = all_char[0];

	/* Den hîchsten Wert eines Zeichen merken */
	all_bchar.hi_char = all_char[j - 1];

	/* } lîschen */
	grammar++;
}


WORD get_char( VOID )
{
	/* Hier wird eine Angabe extrahiert. */
	REG WORD	i;
	WORD		base;
	BYTE		buff[10];

	/* Ist es \1, \x1 oder \a ? */
	if( *grammar == '\\' )
	{
		/* lîschen */
		grammar++;

		/* x, Zeichen oder Zahl ? */
		i = 0;

		/* Ist es eine hexadezimale Angabe ? */
		if( *grammar == 'x' )
		{
			/* Ja, Basis ist 16 */
			grammar++;
			base = 16;
		}

		else
			/* Basis ist 10 */
			base = 10;
		
		while( isdigit( *grammar ) )
			/* Zahl lesen */
			buff[i++] = *grammar++;

		/* War es eine Zahl oder ein Buchstabe ? */
		if( i )
		{
			/* Ja, Zahl extrahieren */
			buff[i] = 0;

			/* Wert zurÅckliefern */
			return( ( WORD ) strtol( buff, 0L, base ) );
		}

		else
		{
			/* War ein Buchstabe */
			switch( *grammar )
			{
				case 'a':
				{
					/* Bell */
					base = 0x07;
					break;
				}

				case 'b':
				{
					/* Backspace */
					base = 0x08;
					break;
				}

				case 'f':
				{
					/* FormFeed */
					base = 0x0c;
					break;
				}

				case 'n':
				{
					/* LineFeed */
					base = 0x0a;
					break;
				}

				case 'r':
				{
					/* Carriage Return */
					base = 0x0d;
					break;
				}

				case 't':
				{
					/* Tabulator */
					base = 0x09;
					break;
				}

				case 'v':
				{
					/* Tabulator */
					base = 0x0b;
					break;
				}
			}

			/* Zeichen lîschen */
			grammar++;

			/* Wert zurÅckliefern */
			return( base );
		}
	}

	else if( *grammar == '\'' )
	{
		/* ASCII-Angabe */
		grammar++;

		/* Zeichen extrahieren */
		base = *grammar++;

		/* ' lîschen */
		grammar++;

		/* Wert zurÅckliefern */
		return( base );
	}

	else if ( isdigit( *grammar ) )
	{
		/* Ist eine Zahl */
		i = 0;

		while( isdigit( *grammar ) )
			/* Zahl lesen */
			buff[i++] = *grammar++;

		/* Zahl extrahieren */
		buff[i] = 0;

		/* Wert zurÅckliefern */
		return( ( WORD ) strtol( buff, 0L, 10 ) );
	}

	/* Fehler */
	return( -1 );
}


VOID del_line( BYTE *ch )
{
	/* Zeichen bis zum Zeilenende lîschen */
	while( *ch != '\n' && *ch != 0x0d )
		ch++;
}