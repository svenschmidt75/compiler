/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Mon Feb  3 14:01:43 CET 2003
    copyright            : (C) 2003 by Sven Schmidt
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
#include <stdlib.h>
#include <sys/stat.h>


//cout definieren
using namespace std;


//#include "sslex.h"
#include "sslrparser.h"


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


/*

	Dies ist in dem generierten Scanner-Header-File einzutragen!

#include "sslex.h"

*/


int main( int argc, char *argv[] )
{
	//Scanner-Klasse aufrufen
	//Argumente
	ARGV_STR		argv_str;
	SSLEXScanner	*s;

	//Meldung ausgeben
	cout << "\nSSLEX V" << VERSION << "\n";
	cout << "(c) 2002-2005 by Sven Schmidt\n";
	cout << "s.schmidt@lboro.ac.uk\n\n";

#ifdef PUBLIC

	//Argumente einlesen
	if( !parseArgv( argc, argv, &argv_str ) )
	{
		//keine Fehler in den Argumenten

		//Grammatik-Datei einlesen
		if( readFile( argv[argc - 1] ) < 0 )
			//Fehler
			return( -1 );
#else
	{
		parseArgv( argc, argv, &argv_str );

		//Grammatik-Datei einlesen
		if( readFile( "/home/sschmidt/Develop/compilerbau/devel/miniC/miniC.sslex" ) < 0 )
			//Fehler
			return( -1 );
#endif

		//los geht's
		s = new SSLEXScanner( data, strlen( data ) );

		//Dateinamen der scanner specification file aus den Command-Line-Arguments
		SSLRParser	lr( s );
		lr.giveArgs( &argv_str );
		lr.yyParse();

		//löschen
		delete( s );
		delete[] ( data );
	}

	return( EXIT_SUCCESS );
}



int parseArgv( int argc, char *argv[], ARGV_STR *argv_str )
{
	//Argumente Parsen
	char	*pos;
	int		i, err = 1, argCnt = 0;

	//mit Standard-Werten initialisieren

	//ausführliche Meldungen
	argv_str->verbose = false;

	//Debug-Modus
	argv_str->debug = false;
//	argv_str->debug = true;

	//7-Bit-Scanner per default
	argv_str->cclWidth = _7BIT;

	//zeige keine Konflikte zwischen den regulären Ausdrücken
	argv_str->showConflicts = false;

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
					//ausführliche Meldungen
					argv_str->verbose = true;

				else if( !strcmp( pos, "-help" ) )
				{
					//Hilfe ausgeben
					cout << "\n";
					cout << "usage: sslex [OPTIONS]... [GRAMMAR-FILE] ..\n\n";
					cout << "-v verbose mode\n";
					cout << "-s show conflicts between rules\n";
					cout << "-d generate debug-version of cpp-file\n";
					cout << "-7 generate a 7 bit scanner (default)\n";
					cout << "-8 generate a 8 bit scanner\n";
					cout << "\n";

					//nächstes Argument
					break;
				}

				else if( *pos == 'd' )
					//Debug-Modus
					argv_str->debug = true;

				else if( *pos == '7' )
					//7-Bit-Scanner
					argv_str->cclWidth = _7BIT;

				else if( *pos == '8' )
					//8-Bit-Scanner
					argv_str->cclWidth = _8BIT;

				else if( *pos == 's' )
					//zeige Konflikte zwischen reg. Ausdrücken
					argv_str->showConflicts = true;

				else
				{
					//unbekannte Option
					cerr << "sslex: Unknown argument line option " << *pos << "!\n";
					cout << "sslex: --help for more Information\n";

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
			//ist keine Option, muss scanner specification file sein
			err = 0;
	}

	//kein scanner specification file angegeben?
	if( err )
		//Fehlt
		cout << "sslex: No scanner specification file specified!" << endl;

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
		cout << endl << "\nsslex: Couldn't read specification file " << name << "!" << endl;
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


