/***************************************************************************
                          scanner.cpp  -  description
                             -------------------
    begin                : Fre Aug 16 2002
    copyright            : (C) 2002 by Sven Schmidt
    email                : s.schmidt@lboro.ac.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "scanner.h"

scanner::scanner()
{
	//Scanner-Klasse initialisieren

	//Die Attribute der Token hängen vom Typ ab, d.h. ein Identfiert hat als Attribut den Namen, eine Zahl
	//den Zahlenwert.
	attribut_str = new SCAN_STR;

	//zurücksetzen
	unionFlag = false;
	unionStr = 0L;

	//Zeile
	line = 1;

	//Zeile für semantische Aktionen zurücksetzen
	semActLine = 0;

	//Speicher für den String
	string = new( char[512+1] );
}


scanner::scanner( char *data, SCAN_STR **att )
{
	//Scanner-Klasse initialisieren

	//Die Attribute der Token hängen vom Typ ab, d.h. ein Identfiert hat als Attribut den Namen, eine Zahl
	//den Zahlenwert.
	attribut_str = new SCAN_STR;

	//zurücksetzen
	unionFlag = false;
	unionStr = 0L;

	//merken
	*att = attribut_str;

	//Zeile
	line = 1;

	//Zeile für semantische Aktionen zurücksetzen
	semActLine = 0;

	//Speicher für den String
	string = new( char[512+1] );

	//keine Fehler
	err = 0;

	//Anfang der Daten
	pos = data;

	//Speicher für die semantischen Aktionen
	sem = new( char[strlen(data) + 1] );

	//Speicher für Include-Anweisungen
	incl = new( char[strlen(data) + 1] );

	//Speicher für eigene Erweiterungen
	extension = new( char[strlen(data) + 1] );
}


scanner::~scanner()
{
	//Speicher freigeben

	//Erweiterungen?
	if( extension )
		delete[] ( extension );

	//Includes
	if( incl )
		delete[] ( incl );

	//semantische Aktionen?
	if( sem )
		delete[] ( sem );

	//String
	if( string )
		delete[] ( string );

	//Attribute
	if( attribut_str )
		delete( attribut_str );
}


unsigned long scanner::getLine( void )
{
	//aktuelle Zeilen-Nummer
	unsigned long	tmp = line;

	//gab es eine semantische Aktion?
	if( semActLine )
	{
		//merken
		tmp = semActLine;

		//zurücksetzen
		semActLine = 0;
	}

	//Zeilennummer zurückgeben
	return( tmp );
}


SSLEXToken scanner::getToken( void )
{
	//Hier werden die Tokens erkannt und zurückgeliefert.
	char			*tmp;
	int				cc;

	//Default-Status
	state = DEFAULT_STATE;

	//nächstes Zeichen
	while( *pos )
	{
		//Blanks löschen
		switch( state )
		{
			case DEFAULT_STATE:
			{
				//Leerzeichen löschen
				while( isspace( *pos ) && ( *pos != '\n' ) )
				//nächstes Zeichen
					pos++;

				//Datei zu Ende?
				if( !*pos )
					//Ja, alles OK
					continue;

				break;
			}
		}

		//Neue Zeile?
		if( ( *pos == '\n' ) && ( state != SEMACT_STATE ) )
		{
			//Neue Zeile
			line++;

			//Status: COMENT_CPP_STATE?
			if( state != COMENT_CPP_STATE )
			{
				//Status COMENT_CPP_STATE ist Kommentar //
				pos++;
				continue;
			}
		}

		//Abbhängig vom aktuellen Zustand analysieren
		switch( state )
		{
			//default-Zustand
			case DEFAULT_STATE:
			{
				//Der Scanner ist im default-Zustand, erkennt in diesem Zustand also Kommentare, Identifier,
				//etc.

				//Kommentar?
				if( !strncmp( pos, "/*", 2 ) )
					//Ja
					state = COMENT_C_STATE;

				//Zeiger in %union
				else if( *pos == '*' )
				{
					//Ist ein Zeiger in %union (hoffentlich)
					pos++;
					return( AST );
				}

				else if( *pos == ';' )
				{
					//Ende einer Produktion
					pos++;
					return( PROD_END );
				}

				else if( *pos == ',' )
				{
					//ein einfaches Komme zum Trennen der Terminale in den Fehlerangaben
					pos++;
					return( COMMA );
				}

				else if( *pos == '!' )
				{
					//dient zum Negieren der Terminale in den Fehlerangaben
					pos++;
					return( EXCLAMATION );
				}

				else if( *pos == '|' )
				{
					//Eine Alternative zu einer Produktion
					pos++;
					return( PROD_NEXT );
				}

				else if( !strncmp( pos, "//", 2 ) )
					//Kommentar
					state = COMENT_CPP_STATE;

				else if( isdigit( *pos ) )
				{
					//Eine Zahl

					//Index zurücksetzen
					cc = 0;

					//Eine Zahl erkennen
					state = DIGIT_STATE;
				}

				else if( isalnum( *pos ) || ( *pos == '_' ) )
				{
					//Hier darf dann nur ein Identifier folgen

					//Index zurücksetzen
					cc = 0;

					//Zustand setzen
					state = IDENT_STATE;
				}

				else if( *pos == '"' )
				{
					//Einen String erkennen

					//Index zurücksetzen
					cc = 0;

					//Zustand setzen
					state = STRING_STATE;

					//löschen
					pos++;
				}

				else if( *pos == '\'' )
				{
					//Terminal
					state = TERMINAL_STATE;

					//Index zurücksetzen
					cc = 0;

					//löschen
					pos++;
				}

				else if( *pos == ':' )
				{
					//Seperator
					pos++;
					return( SEPERATOR );
				}

				else if( *pos == '{' )
				{
					//Brace
					pos++;

					//gehört das zu union?
					if( unionFlag == true )
						//Ja, Position merken
						unionStr = pos;

					return( BOPEN );
				}

				else if( *pos == '}' )
				{
					//EBNF, etc.

					//gehört das zu union?
					if( unionFlag == true )
					{
						//Ja, Text merken
						unionText();

						//als Attribut merken
						attribut_str->name = unionStr;

						//zurücksetzen
						unionFlag = false;
					}

					//löschen
					pos++;
					return( BCLOSE );
				}

				else if( *pos == '[' )
				{
					//EBNF
					pos++;
					return( EOPEN );
				}

				else if( *pos == ']' )
				{
					//EBNF
					pos++;
					return( ECLOSE );
				}

				else if( *pos == '(' )
				{
					//EBNF
					pos++;
					return( KLOPEN );
				}

				else if( *pos == ')' )
				{
					//EBNF
					pos++;
					return( KLCLOSE );
				}

				else if( !strncmp( pos, "::=", 3 ) )
				{
					//Seperator
					pos += 3;
					return( SEPERATOR );
				}

				else if( !strncmp( pos, "\\{", 2 ) )
				{
					//Seperator
					pos += 2;

					//Zähler
					cc = 0;

					//hierhin kopieren
					tmp = sem;

					//Zeile merken
					semActLine = line;

					//semantische Aktion
					state = SEMACT_STATE;
				}

				else if( !strncmp( pos, ":=", 2 ) )
				{
					//Seperator
					pos += 2;
					return( SEPERATOR );
				}

				else if( !strncmp( pos, "->", 2 ) )
				{
					//Seperator
					pos += 2;
					return( SEPERATOR );
				}

				else if( *pos == '%' )
				{
					//left, right, term_type, nterm_type, error, prec
					if( !strncmp( pos, "%left", 5 ) )
					{
						//aus der Eingabe entfernen
						pos += 5;
						return( LEFT );
					}

					else if( !strncmp( pos, "%right", 6 ) )
					{
						//aus der Eingabe entfernen
						pos += 6;
						return( RIGHT );
					}

					else if( !strncmp( pos, "%nonassoc", 9 ) )
					{
						//aus der Eingabe entfernen
						pos += 9;
						return( NONASSOC );
					}

					else if( !strncmp( pos, "%termType", 9 ) )
					{
						//aus der Eingabe entfernen
						pos += 10;
						return( TERM_TYPE );
					}

					else if( !strncmp( pos, "%ntermType", 10 ) )
					{
						//aus der Eingabe entfernen
						pos += 11;
						return( NTERM_TYPE );
					}

					else if( !strncmp( pos, "%prec", 5 ) )
					{
						//aus der Eingabe entfernen
						pos += 5;
						return( PREC );
					}

					else if( !strncmp( pos, "%error", 6 ) )
					{
						//aus der Eingabe entfernen
						pos += 6;
						return( ERROR );
					}

					else if( !strncmp( pos, "%include", 8 ) )
					{
						//aus der Eingabe entfernen
						pos += 8;

						//Zähler
						cc = 0;

						//Ziel
						tmp = incl;

						//Include-Anweisungen
						state = INCL_STATE;
					}

					else if( !strncmp( pos, "%extension", 10 ) )
					{
						//aus der Eingabe entfernen
						pos += 10;

						//Zähler
						cc = 0;

						//Ziel
						tmp = extension;

						//eigene Erweiterungen
						state = EXTENSION_STATE;
					}

					else if( !strncmp( pos, "%definition", 11 ) )
					{
						//aus der Eingabe entfernen
						pos += 11;
						return( DEF );
					}

					else if( !strncmp( pos, "%grammar", 8 ) )
					{
						//aus der Eingabe entfernen
						pos += 8;
						return( GRAMMAR );
					}

					else if( !strncmp( pos, "%union", 6 ) )
					{
						//aus der Eingabe entfernen
						pos += 6;

						//Flag setzen
						unionFlag = true;
						return( UNION );
					}

					//Fehler!
					else
					{
						//unbekanntes Steuerzeichen

						//extrahieren
						pos++;
						cc = 0;

						while( !isspace( *pos ) )
							string[cc++] = *pos++;

						//abschließen
						string[cc] = 0;

						//Meldung ausgeben
						cerr << "sslr: line " << line << ": Unknown command %" << string << "!\n";

						//Fehler
						err++;

						//Zeile löschen
						discard( '\n' );
					}
				}

				else if( *pos == '<' )
				{
					//Für semmantische Aktionen: hier wird der Typ angegeben, der sich in der
					//Anweisung %union wiederinden läßt.
					pos++;

					//Zähler
					cc = 0;

					//bis >
					while( *pos != '>' )
						//merken
						string[cc++] = *pos++;

					//Identifier zu Ende
					string[cc] = 0;

					//> löschen
					pos++;

					//Typ für semmantische Aktion
					attribut_str->name = string;
					return( SEM_TYPE );
				}

				else if( *pos != '\n' )
				{
					//unerkanntes Zeichen
					cerr << "sslr: line " << line << ": Illegal character " << *pos << "!\n";

					//Fehler
					err++;

					//bis zum Ende der Zeile alles löschen
					discard( '\n' );
				}

				break;
			}

			case IDENT_STATE:
			{
				//Identifier

				while( isalnum( *pos ) || ( *pos == '_' ) )
					//OK, eintragen
					string[cc++] = *pos++;

				//Identifier zu Ende
				string[cc] = 0;

				//Attribut(Name) übergeben
				attribut_str->name = string;

				//Identifier erkannt
				return( IDENT );
			}

			case TERMINAL_STATE:
			{
				//Terminal

				//Das erste Zeichen eines Identifiers darf keine Nummer sein
				if( !isalpha( *pos ) && ( *pos != '_' ) )
				{
					//fängt mit Nummer an
					cerr << "sslr: line " << line << ": Terminals must begin with a letter!" << "\n";

					//Fehler
					err++;
				}

				do
				{
					//Erlaubtes Zeichen?
					if( isalnum( *pos ) || ( *pos == '_' ) )
						//Eintragen
						string[cc++] = *pos++;

					else
					{
						//Fehler
						string[cc] = 0;

						//Space?
						if( isspace( *pos ) )
							//Terminal nicht abgeschlossen
							cerr << "sslr: line " << line << ": Terminal " << string << " doesn't end with '!\n";

						else
							//nicht erlaubtes Zeichen
							cerr << "sslr: line " << line << ": Illegal character " << *pos << " in Terminal " << string << "!\n";

						//Fehler
						err++;

						//bis newline löschen
						discard( '\n' );

						//default-Zustand
						state = DEFAULT_STATE;

						//while-Schleife verlassen
						break;
					}

				}while( *pos != '\'' );

				//Identifier zu Ende
				string[cc] = 0;

				//Attribut(Name) übergeben
				attribut_str->name = string;

				//Zeichen löschen?
				if( *pos == '\'' )
					//Ja
					pos++;

				//Identifier erkannt
				return( TERMINAL );
			}

			case DIGIT_STATE:
			{
				//Zahl
				while( isdigit( *pos ) )
					//OK, eintragen
					string[cc++] = *pos++;

				//Zahl zu Ende
				string[cc] = 0;

				//Attribut(Name) übergeben
				attribut_str->l = strtol( string, 0L, 10 );

				//Zahl erkannt
				return( NUMBER );
			}

			case STRING_STATE:
			{
				//String
				while( *pos != '"' )
					//OK, eintragen
					string[cc++] = *pos++;

				//Zahl zu Ende
				string[cc] = 0;

				// " löschen
				pos++;

				//Attribut(Name) übergeben
				attribut_str->name = string;

				//Einen String erkannt
				return( STRING );
			}

			case SEMACT_STATE:
			{
				//semantische Angabe in der Grammatik-Datei
				while( !( ( pos[0] == '\\' ) && ( pos[1] == '}' ) ) )
				{
					//Newline?
					if( *pos == '\n' )
						//Ja
						line++;

					//eintragen
					tmp[cc++] = *pos++;
				}

				//Null-Byte
				tmp[cc] = 0;

				//\} löschen
				pos += 2;

				//Attribut(Name) übergeben
				attribut_str->name = tmp;

				//Einen String erkannt
				return( SEM_ACT );
			}

			case COMENT_C_STATE:
			{
				//Kommentar
				if( !strncmp( pos, "*/", 2 ) )
				{
					//Ja
					state = DEFAULT_STATE;

					//löschen
					pos++;
				}

				//ein Zeichen löschen
				pos++;

				break;
			}

			case COMENT_CPP_STATE:
			{
				//Kommentar
				if( *pos++ == '\n' )
					//Ja
					state = DEFAULT_STATE;

				break;
			}

			case INCL_STATE:
			{
				//Include-Anweisungen

				// \{ löschen
				pos += 2;

				//Alles kopieren
				while( !( ( pos[0] == '\\' ) && ( pos[1] == '}' ) ) )
				{
					//Newline?
					if( *pos == '\n' )
						//Ja
						line++;

					//merken
					tmp[cc++] = *pos++;
				}

				//ende
				tmp[cc] = 0;

				// " löschen
				pos += 2;

				//Attribut(Name) übergeben
				attribut_str->name = tmp;

				//Einen String erkannt
				return( INCL );
			}

			case EXTENSION_STATE:
			{
				//eigene Erweiterungen

				// \{ löschen
				pos += 2;

				//Alles kopieren
				while( !( ( pos[0] == '\\' ) && ( pos[1] == '}' ) ) )
				{
					//Newline?
					if( *pos == '\n' )
						//Ja
						line++;

					//merken
					tmp[cc++] = *pos++;
				}

				//ende
				tmp[cc] = 0;

				// " löschen
				pos += 2;

				//Attribut(Name) übergeben
				attribut_str->name = tmp;

				//Einen String erkannt
				return( EXTENSION );
			}

			default:
			{
				//Undefiniert
				cerr << "sslr: line " << line << ": Undefined state in scanner!\n";
				break;
			}
		}
	}

	//Alles gescannt
	attribut_str->b = err ? true : false;
	return( END );
}


void scanner::discard( char c )
{
	//Bis zum Zeichen c alles löschen
	while( *pos != c )
	{
		//Ende?
		if( !*pos )
		{
			//unerwarteter Fehler
			cerr << "sslr: line " << line << ": Unexpected end of file!\n";

			//Ende
			exit( 1 );
		}

		//Zeichen überspringen
		pos++;
	}
}


void scanner::unionText( void )
{
	//union-Bereich scannen
	char	*tmp = unionStr;
	long	len;

	//Fehler?
	if( unionStr )
	{
		//Länge
		len = pos - tmp;

		//Speicher anfordern
		unionStr = new( char[len + 1] );

		//kopieren
		strncpy( unionStr, tmp, len );

		//Null-Byte
		unionStr[len] = 0;
	}
}

