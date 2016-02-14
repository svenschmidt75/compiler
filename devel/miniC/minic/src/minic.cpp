/***************************************************************************
 *   Copyright (C) 2005 by Sven Schmidt   *
 *   s.schmidt@lboro.ac.uk   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <sys/stat.h>

#include "csymboltable.h"
#include "csyntaxtree.h"
#include "sslrtermdef.h"
#include "sslrparser.h"
#include "minic.h"

using namespace std;


//Prototypen
int parseArgv( int, char **, ARGV_STR * );
int readFile( char * );

//Variablen
struct stat		st;

//Grammatik-Datei-Daten
char			*data;


//names of the terminals
char *termName[] =
{ "SEMIKOLON", "LESS", "TRUE", "DIV", "LESS_EQU", "EQU", "DO", "BIN_OR", "KLCLOSE", "COMMA", "LEFT_SHIFT", "SHORT", "INT", "IF_STMT", "LOG_NOT", "FALSE",
"BOPEN", "BIN_NOT", "LONG", "NUMBER", "MOD", "MUL", "LOG_OR", "BCLOSE", "BOOL", "RIGHT_SHIFT", "WHILE", "NOTEQUAL", "EQUAL", "SUB", "GREATER", "LOG_AND",
"ELSE_STMT", "KLOPEN", "BIN_XOR", "BIN_AND", "GREATER_EQU", "IDENT", "ADD", "CHAR", "ACCEPT" };


int main( int argc, char *argv[] )
{
	//Argumente
	ARGV_STR	argv_str;

	//Meldung ausgeben
	cout << "\nMiniC-Compiler V" << VERSION << "\n";
	cout << "(c) 2005 by Sven Schmidt\n";
	cout << "s.schmidt@lboro.ac.uk\n\n";

	//Argumente einlesen
	if( !parseArgv( argc, argv, &argv_str ) )
	{
		//keine Fehler in den Argumenten
		SSLEXScanner	*s;

		//Grammatik-Datei einlesen
		if( readFile( argv[argc - 1] ) < 0 )
			//Fehler
			return( -1 );

		//Symboltabelle anlegen
		CSymbolTable *symTab = new( CSymbolTable );

		//Syntaxbaum erzeugen
		CSyntaxTree *syntaxTree = new( CSyntaxTree );

		//Codeerzeugung erzeugen
		CCodeBase *codeBase = new CCodeBase( symTab, syntaxTree );

		//Scanner-Klasse erzeugen
		s = new SSLEXScanner( data, strlen( data ) );
/*
		int ret;
		SSLR_TYPE	tmp;

		while( ( ret = s->yyLex( &tmp ) ) != ACCEPT )
		{
			cerr << termName[ret] << endl;
		}
*/

		//Dateinamen des Grammatik-Files aus den Command-Line-Arguments
		SSLRParser	lr( s );
		lr.giveArgs( &argv_str, symTab, syntaxTree, codeBase );
		lr.yyParse();

		//Speicher für den Scanner freigeben
		delete( s );

		//Speicher freigeben
		delete( symTab );
		delete( syntaxTree );
		delete( codeBase );
		delete[] ( data );
	}

	return EXIT_SUCCESS;
}


int parseArgv( int argc, char *argv[], ARGV_STR *argv_str )
{
	//Argumente parsen
	char	*pos;
	int		i, err = 1, argCnt = 0;

	//mit Standard-Werten initialisieren

	//keine Meldungen
	argv_str->verbose = 0;

	//Zähler
	i = 1;

	while( i < argc )
	{
		//nun die Argumente lesen
		pos = argv[i++];

		//Schalter?
		if( *pos == '-' )
		{
			//ja, Optionen können von der Art -vd... sein
			pos++;

			while( *pos )
			{
				//welcher Schalter?
				if( *pos == 'v' )
				{
					//Level 1
					argv_str->verbose = 1;

					//höhere Ausführlichkeit?
					if( pos[1] )
					{
						//Zahl auswerten
						if( pos[1] == '0' )
							//keine Meldungen
							argv_str->verbose = 0;

						else if( pos[1] == '1' )
							//Level 1
							argv_str->verbose = 1;

						else if( pos[1] == '2' )
							//Level 2
							argv_str->verbose = 2;

						else
							//Fehlermeldung
							cerr << "minic: Use -v, -v0, -v1 and -v2!" << endl;

						//Zahl löschen
						pos++;
					}
				}

				else if( !strcmp( pos, "-help" ) )
				{
					//Hilfe ausgeben
					cout << "\n";
					cout << "usage: minic [OPTIONS]... [GRAMMAR-FILE] ..\n\n";
					cout << "-vX verbose mode (-v0, -v1, -v2)\n";
					cout << "\n";

					//nächstes Argument
					break;
				}

				else if( *pos == 'd' )
					//Debug-Modus
					argv_str->debug = true;

				else if( !strncmp( "f1", pos, 2 ) )
				{
					//FIRST-Mengen ausgeben
					argv_str->showFirst = true;

					//löschen
					pos++;
				}

				else if( !strncmp( "f2", pos, 2 ) )
				{
					//FOLLOW-Mengen ausgeben
					argv_str->showFollow = true;

					//löschen
					pos++;
				}

				//Hüllen ausgeben?
				else if( *pos == 'h' )
					//Hüllen ausgeben
					argv_str->showHulle = true;

				//Grammatik-Datei ausgeben?
				else if( *pos == 'o' )
					//Grammatik in Datei ausgeben
					argv_str->outputGrammar = true;

				else
				{
					//unbekannte Option
					cerr << "minic: Unknown argument line option " << pos << "!\n";
					cerr << "minic: --help for more Information\n";

					//Fehler
					err++;
				}

				//weiter
				pos++;
			}

			//Argumente zählen
			argCnt++;
		}

		else
			//ist keine Option, muss Grammatik-File sein
			err = 0;
	}

	//Kein Grammatik-File angegeben?
	if( err )
		//Fehlt
		cout << "minic: No grammar file specified!\n";

	//etwaige Fehler
	return( err );
}


int readFile( char *name )
{
	//Datei öffnen
	FILE	*file;

	//Grammatik-Datei öffnen
	file = fopen( name, "r" );

	//Informationen über die Datei einholen
	if( stat( name, &st ) )
	{
		//Fehler
		cout << endl << "\nminic: Unable to read file " << name << "!\n";
		return( -1 );
	}

	//Speicher für die Datei anfordern
	data = new( char[st.st_size + 1] );

	//Mit Null-Byte abschliessen
	data[st.st_size] = 0;

	//Datei einlesen
	fread( data, sizeof( char ), st.st_size, file );

	//Datei schliessen
	fclose( file );

	//alles OK
	return( 0 );
}

