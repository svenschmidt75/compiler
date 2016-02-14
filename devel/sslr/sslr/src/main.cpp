/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Don Aug 15 18:00:33 CEST 2002
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <sys/stat.h>


#include "sslr.h"
#include "_sslrparser.h"



/*
	Für eine public version entsprechendes Flag setzen.
*/
#define PUBLIC
//#undef PUBLIC


//Prototypen
int parseArgv( int, char **, ARGV_STR * );
int readFile( char * );

//Variablen
struct stat		st;

//Grammatik-Datei-Daten
char			*data;




int main( int argc, char *argv[] )
{
	//Argumente
	ARGV_STR	argv_str;

	//Meldung ausgeben
	cout << "\nSSLR V" << VERSION << "\n";
	cout << "(c) 2002-2005 by Sven Schmidt\n";
	cout << "s.schmidt@lboro.ac.uk\n\n";

#ifdef PUBLIC
	//Argumente einlesen
	if( !parseArgv( argc, argv, &argv_str ) )
	{
		//keine Fehler in den Argumenten
		SSLEXScanner	*s;

		//Grammatik-Datei einlesen
		if( readFile( argv[argc - 1] ) < 0 )
			//Fehler
			return( -1 );
#else
	{
		//keine Fehler in den Argumenten
		SSLEXScanner	*s;

		parseArgv( argc, argv, &argv_str );

		//Grammatik-Datei einlesen
//		if( readFile( "/home/sschmidt/Develop/compilerbau/devel/sslr/src/sslr.sslr" ) < 0 )
		if( readFile( "/home/sschmidt/Develop/compilerbau/devel/miniC/miniC.sslr" ) < 0 )
			//Fehler
			return( -1 );
#endif

		//Scanner-Klasse erzeugen
		s = new SSLEXScanner( data, strlen( data ) );

		//Dateinamen des Grammatik-Files aus den Command-Line-Arguments
		SSLRParser	lr( s );
		lr.giveArgs( &argv_str );
		lr.yyParse();

		//Speicher für den Scanner freigeben
		delete( s );

		//Speicher für das Grammatik-File freigeben
		delete[] ( data );
	}

	//Speicher freigeben
	delete[] ( argv_str.name );

	//alles OK
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

	//FIRST-Mengen ausgeben
	argv_str->showFirst = false;

	//FOLLOW-Mengen ausgeben
	argv_str->showFollow = false;

	//Hüllen ausgeben
	argv_str->showHulle = false;

	//Debug-Modus
	argv_str->debug = false;

	//Grammatik-File ausgeben
	argv_str->outputGrammar = false;

	//LALR-Parser
	argv_str->type = SSLR_LALR;

	//Name der Grammatik-Datei
	argv_str->name = new( char[9] );
	strcpy( argv_str->name, "sslr.gra" );

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
							cerr << "sslr: Use -v, -v0, -v1 and -v2!" << endl;

						//Zahl löschen
						pos++;
					}
				}

				else if( !strcmp( pos, "-help" ) )
				{
					//Hilfe ausgeben
					cout << "\n";
					cout << "usage: sslex [OPTIONS]... [GRAMMAR-FILE] ..\n\n";
					cout << "-vX verbose mode (-v0, -v1, -v2)\n";
					cout << "-d generate debug version of cpp-file\n";
					cout << "-f1 output FIRST set\n";
					cout << "-f2 output FOLLOW set\n";
					cout << "-h output closure\n";
					cout << "-o output grammar in BNF form in file sslr.gra\n";
					cout << "-1 generate SLR parser\n";
					cout << "-2 generate LALR parser\n";
					cout << "-3 generate LR parser\n";
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

				else if( *pos == '1' )
					//SLR-Parser erzeugen
					argv_str->type = SSLR_SLR;

				else if( *pos == '2' )
					//LALR-Parser erzeugen
					argv_str->type = SSLR_LALR;

				else if( *pos == '3' )
					//LR-Parser erzeugen
					argv_str->type = SSLR_LR;

				else
				{
					//unbekannte Option
					cerr << "sslr: Unknown argument line option " << pos << "!\n";
					cerr << "sslr: --help for more Information\n";

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

	//kein Grammatik-File angegeben?
	if( err )
		//Fehlt
		cout << "sslr: No grammar file specified!" << endl;

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
		cout << endl << "\nsslr: Unable to read grammar file " << name << "!" << endl;
		return( -1 );
	}

	//Speicher für die Datei anfordern
	data = new( char[st.st_size + 1] );

	//mit Null-Byte abschliessen
	data[st.st_size] = 0;

	//Datei einlesen
	fread( data, sizeof( char ), st.st_size, file );

	//Datei schliessen
	fclose( file );

	//alles OK
	return( 0 );
}

