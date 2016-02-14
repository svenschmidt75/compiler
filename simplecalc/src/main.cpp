/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Die Apr  8 19:35:28 CEST 2003
    copyright            : (C) 2003 by Sven Schmidt
    email                : sschmidt@physik.uni-bremen.de
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

#include <iostream.h>
#include <stdlib.h>
#include <string.h>

#include "sslrparser.h"


int main(int argc, char *argv[])
{
	char	arg[1024];

	//Scanner-Klasse
	SSLEXScanner	sslex( "", 0 );

	//Parser-Klasse
	SSLRParser		sslr( &sslex );

	strcpy( arg, "integer 90" );

	//Schleife
	do
	{
		//Zeile einlesen
		cout << "\nBitte um Eingabe (quit zum Anhalten): ";
		cin >> arg;

		//Ende der Eingabe?
		if( !strcmp( arg, "quit" ) )
			//Ja
			break;

		//Eingabe an Scanner berliefern
		sslex.reset( arg, strlen( arg ) );

		//parsen
		sslr.reset();
		sslr.yyParse();
//		break;

	}while( arg );

	return EXIT_SUCCESS;
}
