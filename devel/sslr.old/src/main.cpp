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


#include "sslr.h"
#include "lrparser.h"



/*
	Für eine public version entsprechendes Flag setzen.
*/
#define PUBLIC
//#undef PUBLIC



//Prototypen
int parseArgv( int, char **, ARGV_STR * );




int main( int argc, char *argv[] )
{
	//Argumente
	ARGV_STR	argv_str;

	//Meldung ausgeben
	cout << "\nSSLR V" << VERSION << "\n";
	cout << "(c) 2002-2005 by Sven Schmidt\n";
	cout << "s.schmidt@lboro.ac.uk\n\n";

	//Argumente einlesen
	if( !parseArgv( argc, argv, &argv_str ) )
	{
		//Keine Fehler in den Argumenten

		//Los geht's
#ifdef PUBLIC
		//Dateinamen des Grammatik-Files aus den Command-Line-Arguments
		lrparser	lr( argv[argc - 1], &argv_str );
#else
		//meine Version, Grammtikdatei ist test.gra
		lrparser	lr( "/home/sschmidt/Develop/compilerbau/sslrExamples/oberon0.s", &argv_str );
#endif
	}

	//Speicher freigeben
	delete[] ( argv_str.name );

	//Alles OK
	return EXIT_SUCCESS;
}


int parseArgv( int argc, char *argv[], ARGV_STR *argv_str )
{
	//Argumente Parsen
	char	*pos;
	int		i, err = 1, argCnt = 0;

	//Mit Standard-Werten initialisieren

	//Ausführliche Meldungen
	argv_str->verbose = false;

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
	argv_str->type = LALR;

	//Name der Grammatik-Datei
	argv_str->name = new( char[9] );
	strcpy( argv_str->name, "sslr.gra" );

	//Zähler
	i = 1;

	while( i < argc )
	{
		//Nun die Argumente lesen
		pos = argv[i++];

		//Schalter?
		if( *pos == '-' )
		{
			//Ja, Optionen können von der Art -vd... sein
			pos++;

			while( *pos )
			{
				//Welcher Schalter?
				if( *pos == 'v' )
					//Ausführliche Meldungen
					argv_str->verbose = true;

				else if( !strcmp( pos, "-help" ) )
				{
					//Hilfe ausgeben
					cout << "\n";
					cout << "usage: sslex [OPTIONS]... [GRAMMAR-FILE] ..\n\n";
					cout << "-v verbose mode\n";
					cout << "-d generate debug-version of cpp-file\n";
					cout << "-f1 output FIRST set\n";
					cout << "-f2 output FOLLOW set\n";
					cout << "-h output closure\n";
					cout << "-o output grammar in BNF-form in file sslr.gra\n";
					cout << "-1 generate SLR-parser\n";
					cout << "-2 generate LALR-parser\n";
					cout << "-3 generate LR-parser\n";
					cout << "\n";

					//nächstes Argument
					break;
				}

				else if( *pos == 'd' )
					//Debug-Modus
					argv_str->debug = true;

				else if( !strncmp( "f1", pos, 2 ) )
					//FIRST-Mengen ausgeben
					argv_str->showFirst = true;

				else if( !strncmp( "f2", pos, 2 ) )
					//FOLLOW-Mengen ausgeben
					argv_str->showFollow = true;

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
					argv_str->type = SLR;

				else if( *pos == '2' )
					//LALR-Parser erzeugen
					argv_str->type = LALR;

				else if( *pos == '3' )
					//LR-Parser erzeugen
					argv_str->type = LR;

				else
				{
					//unbekannte Option
					cerr << "sslr: Unknown argument line option " << *pos << "!\n";
					cout << "sslr: --help for more Information\n";

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
		cout << "sslr: No grammar file specified!\n";

	//etwaige Fehler
	return( err );
}

