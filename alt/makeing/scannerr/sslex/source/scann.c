/*
	Projekt		:	Scanner-Generator
	Modul		:	SCANN.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	18.07.1997
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

WORD scanner( VOID );


/*****************/
/* Deklarationen */
/*****************/

/* Aktuelle Zeile */
ULONG	zeile = 1;

LONG	value = 0;

/* Zaehler */
WORD	string_count;

/* Array mit String */
BYTE	string[256];


/****************/
/* Definitionen */
/****************/



/***********/
/* Imports */
/***********/

MGLOBAL LR_TYPE	scan_val;
MGLOBAL BYTE	*grammar;
MGLOBAL WORD	analyze;



WORD scanner( VOID )
{
	/* Hier wird die Eingabe gescannt. */
	BYTE	string[256];
	WORD	comment = 0, i;

	/* Endlosschleife */
	do
	{
		/* Anhand des Zeichens die Massnahmen einleiten */
		switch( *grammar )
		{
			/* Space, Tab */
			case ' ':
			case '\t':
			{
				/* Zeichen Åberlesen */
				grammar++;

				/* NÑchstes Zeichen bearbeiten */
				break;
			}

			/* Neue Zeile */
			case 0x0d:
			{
				/* Zeichen ueberlesen */
				grammar++;
			}

			/* LineFeed */
			case '\n':
			{
				/* Naechste Zeile */
				zeile++;

				/* Linefeed ueberlesen */
				grammar++;

				/* Eine komplette Line wurde geparst */
				return( ACCEPT );
			}

			case ';':
			{
				/* Kommentar */
				while( ( *grammar != 0x0a ) && ( *grammar != 0x0d ) )
					/* Zeichen ueberlesen */
					grammar++;

				/* NÑchstes Zeichen bearbeiten */
				break;
			}

			case '/':
			{
				/* Beginn eines C-Kommentares ? */
				if( grammar[1] == '*' )
					/* Flag fuer Kommentar setzen */
					comment++;

				/* Ende eines C-Kommentares ? */
				else if( grammar[-1] == '*' )
					/* Flag fuer Kommentar loeschen */
					comment--;

				/* Naechstes Zeichen bearbeiten */
				grammar++;
				break;
			}

			/* Eingabe zu Ende ? */
			case 0x00:
				/* Eingabe ist zu Ende */
				return( ACCEPT );

			/* Alles andere */
			default:
			{
				/* Noch im Kommentar ? */
				if( comment )
				{
					/* Ja, mit dem naechsten Zeichen weitermachen */
					grammar++;
					break;
				}

				else
				{
					/* Kein Kommentar */
					switch( *grammar )
					{
						case 'A':
						case 'B':
						case 'C':
						case 'D':
						case 'E':
						case 'F':
						case 'G':
						case 'H':
						case 'I':
						case 'J':
						case 'K':
						case 'L':
						case 'M':
						case 'N':
						case 'O':
						case 'P':
						case 'Q':
						case 'R':
						case 'S':
						case 'T':
						case 'U':
						case 'V':
						case 'W':
						case 'X':
						case 'Y':
						case 'Z':
						case 'a':
						case 'b':
						case 'c':
						case 'd':
						case 'e':
						case 'f':
						case 'g':
						case 'h':
						case 'i':
						case 'j':
						case 'k':
						case 'l':
						case 'm':
						case 'n':
						case 'o':
						case 'p':
						case 'q':
						case 'r':
						case 's':
						case 't':
						case 'u':
						case 'v':
						case 'w':
						case 'x':
						case 'y':
						case 'z':
						case '_':
						case ' ':
						{
							/* Zeichen erkannt */
							scan_val.b = *grammar++;
							return( CHARACTER );
						}

						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
						{
							/* Eine Zahl einscannen */

							/* Zaehler zuruecksetzen */
							i = 0;

							/* Solange eine Ziffer folgt */
							while( isdigit( *grammar ) )
								/* Ziffer eintragen */
								string[i++] = *grammar++;

							/* Mit Null-Byte abschliessen */
							string[i] = 0;

							/* Den nummerischen Wert holen */
							scan_val.l = atol( string );

							/* Eine Zahl wurde erkannt */
							return( NUMBER );
						}

						case '.':
						{
							/* Beliebiges Zeichen */
							grammar++;
							return( ANY_CHAR );
						}

						case ',':
						{
							/* Ist ein Komma */
							grammar++;
							return( COMMA );
						}

						case '~':
						{
							/* Negierte Zeichen */
							grammar++;
							return( PREVCCL );
						}

						case '$':
						{
							/* RegulÑrer Ausdruck am Ende einer Zeile */
							grammar++;
							return( END_SYM );
						}

						case '[':
						{
							/* Beginn eines Bereichs */
							grammar++;
							return( AROPEN );
						}

						case ']':
						{
							/* Ende eines Bereichs */
							grammar++;
							return( ARCLOSE );
						}

						case '-':
						{
							/* Bereich */
							grammar++;
							return( TO );
						}

						case '*':
						{
							/* Null bis beliebig oft */
							grammar++;
							return( MORE );
						}

						case '+':
						{
							/* Einmal bis beliebig oft */
							grammar++;
							return( ONE_OR_MORE );
						}

						case '?':
						{
							/* Optional */
							grammar++;
							return( ONE_OR_ZERO );
						}

						case '{':
						{
							/* Beginn Anzahl der REs */
							grammar++;
							return( BROPEN );
						}

						case '}':
						{
							/* Ende Anzahl der REs */
							grammar++;
							return( BRCLOSE );
						}

						case '|':
						{
							/* Oder */
							grammar++;
							return( OR );
						}

						case '(':
						{
							/* Klammer */
							grammar++;
							return( KLOPEN );
						}

						case ')':
						{
							/* Klammer */
							grammar++;
							return( KLCLOSE );
						}

						case '/':
						{
							/* Lookahead-Symbol */
							grammar++;
							return( LOOKAHEAD_SYM );
						}

						case '\\':
						{
							/* Ist es ein \ oder folgt ein Zeichen ? */
							switch( grammar[1] )
							{
								case 't':
								{
									scan_val.b = '\t';
									break;
								}

								case 'a':
								{
									scan_val.b = '\a';
									break;
								}

								case 'b':
								{
									scan_val.b = '\b';
									break;
								}

								case 'f':
								{
									scan_val.b = '\f';
									break;
								}

								case 'n':
								{
									scan_val.b = '\n';
									break;
								}

								case 'r':
								{
									scan_val.b = '\r';
									break;
								}

								case 'v':
								{
									scan_val.b = '\v';
									break;
								}

								default:
									scan_val.b = grammar[1];
							}

							grammar += 2;
							return( CHARACTER );
						}

						case '^':
						{
							/* Am Anfang einer Zeile */
							grammar++;

							if( grammar[-2] == '[' )
								return( EXP );

							return( BEG_LINE );
						}

						case '\"':
						{
							/* Anfang eines Strings */
							grammar++;
							return( STRING_SYM );
						}

						case '<':
						{
							/* Anfang eines Startsymbols */
							grammar++;
							return( BEG_NAME_LIST );
						}

						case '>':
						{
							/* Ende eines Startsymbols */
							grammar++;
							return( END_NAME_LIST );
						}

						case '%':
						{
							/* Definition der inclusive-Startconditions ? */
							if( !strncmp( grammar, "%incl_sc", 8 ) )
								/* Ja, auswerten */
								return( DO_INCL_SC );

							/* Definition der exclusive-Startconditions ? */
							else if( !strncmp( grammar, "%excl_sc", 8 ) )
								/* Ja, auswerten */
								return( DO_EXCL_SC );

							/* Definition der vordefinierten NEAen ? */
							else if( !strncmp( grammar, "%predef_nea", 11 ) )
								/* Ja, auswerten */
								return( DO_PREDEF );

							/* Definition der erklaubten Zeichen ? */
							else if( !strncmp( grammar, "%def_char", 9 ) )
								/* Ja, auswerten */
								return( DO_DEFCHAR );

							break;
						}

						default:
						{
							/* Unbekanntes Zeichen */
							fprintf( stderr, "\nUnbekannte Angabe in Zeile %ld!", zeile );
							exit( 1 );
						}
					}
				}
			}
		}

	}while( 1 );
}


WORD delete( VOID )
{
	/* In dieser Routine werden alle unnuetzen Dinge geloescht. */
	WORD	comment = 0;

	/* Endlosschleife */
	do
	{
		/* Anhand des Zeichens die Massnahmen einleiten */
		switch( *grammar )
		{
			/* Space, Tab */
			case ' ':
			case '\t':
			{
				/* Zeichen ueberlesen */
				grammar++;

				/* Naechstes Zeichen bearbeiten */
				break;
			}

			/* Neue Zeile */
			case 0x0d:
			{
				/* Zeichen ueberlesen */
				grammar++;
			}

			/* LineFeed */
			case '\n':
			{
				/* Naechste Zeile */
				zeile++;

				/* Naechstes Zeichen bearbeiten */
				grammar++;
				break;
			}

			case '/':
			{
				/* Beginn eines C-Kommentares ? */
				if( grammar[1] == '*' )
					/* Flag fuer Kommentar setzen */
					comment++;

				/* Ende eines C-Kommentares ? */
				else if( grammar[-1] == '*' )
					/* Flag fuer Kommentar loeschen */
					comment--;

				/* Naechstes Zeichen bearbeiten */
				grammar++;
				break;
			}

			/* Eingabe zu Ende ? */
			case 0x00:
				/* Eingabe ist zu Ende */
				return( 1 );

			/* Alles andere */
			default:
			{
				/* Noch im Kommentar ? */
				if( comment )
				{
					/* Ja, mit dem naechsten Zeichen weitermachen */
					grammar++;
					break;
				}

				else
					/* Zurueck */
					return( 0 );
			}
		}

	}while( 1 );
}


WORD extract_word( VOID )
{
	/* Hier wird ein Wort extrahiert. */
	WORD	type = -1;
	BYTE	*temp_grammar = grammar;

	/* Endlosschleife */
	do
	{
		switch( *grammar )
		{
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '_':
			case 'ss':
			{
				/* Mit dem naechsten Zeichen weitermachen */
				grammar++;
				break;
			}

			case '(':
			case ')':
			case ':':
			case '=':
			case '-':
			case ' ':
			case '\t':
			case '\n':
			case 0x0d:
			case 0x00:
			{
				/* Nichtterminal merken */
				strncpy( string, temp_grammar, ( grammar - temp_grammar ) );

				/* Null-Byte anhaengen */
				string[grammar - temp_grammar] = 0;

				/* Ist die Eingabe zu Ende ? */
				if( !( *grammar ) && ( type < 0 ) )
					/* Eingabe ist zu Ende */
					return( 1 );

				else
					/* Alles OK */
					return( 0 );
			}

			/* Unerlaubtes Zeichen */
			default:
			{
				/* Fehlermeldung ausgeben */
				fprintf( stderr, "\nZeile %lu: Unerlaubtes Zeichen %c !", zeile, ( ULONG ) *grammar );

				/* Wort merken */
				strncpy( string, temp_grammar, ( grammar - temp_grammar ) );

				/* Null-Byte anhaengen */
				string[grammar - temp_grammar] = 0;

				/* Weitermachen */
				return( 0 );
			}
		}

	}while( 1 );
}