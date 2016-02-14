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

#include "coneoperandopcode.h"
#include "sslrtermdef.h"
#include "sslrparser.h"
#include "ssass.h"

using namespace std;


//Prototypen
int parseArgv( int, char **, ARGV_STR * );
int readFile( const char * );

//Variablen
struct stat		st;

//Grammatik-Datei-Daten
char			*data;


//names of the terminals
char *termName[] =
{ "BCC", "ASL", "ASR", "BCS", "BEQ", "BGE", "BGT", "BHI", "BLE", "BLS", "BLT", "BMI", "EXPORT", "BNE", "BPL", "BRA",
"BSR", "BVC", "BVS", "CLR", "CMP", "BYTE", "EXTB", "PLUS", "LOCAL", "STRING", "SINGLE", "LABEL_DEF", "MOVE", "DC", "EXT", "LINK",
"OR", "PC", "SR", "NAME", "KLCLOSE", "COMMA", "JMP", "JSR", "NEGX", "MULS", "MULU", "LEA", "MINUS", "ADDX", "LONG", "LSL",
"LSR", "SUBX", "ADRESS_REG", "NUMBER", "NEG", "NOP", "NOT", "UNLK", "DATA_REG", "PEA", "DATA_SEG", "STRING_CODE", "SCC", "SCS", "RTS", "SHARP",
"SEQ", "SGE", "SGT", "SHI", "SLE", "SLS", "SLT", "SMI", "SNE", "SPL", "SSF", "SST", "SUB", "DIVS", "DIVU", "SVC",
"SVS", "DOUBLE", "TST", "KLOPEN", "STACK", "EPSILON", "IMPORT", "ADD", "XOR", "TEXT_SEG", "AND", "WORD", "ACCEPT" };


int main( int argc, char *argv[] )
{
	//Argumente
	ARGV_STR	argv_str;

	//Meldung ausgeben
	cout << "\nSSASS V" << VERSION << "\n";
	cout << "(c) 2005 by Sven Schmidt\n";
	cout << "s.schmidt@lboro.ac.uk\n\n";

	//Argumente einlesen
	if( !parseArgv( argc, argv, &argv_str ) )
	{
		//keine Fehler in den Argumenten
		SSLEXScanner	*s;

		//Grammatik-Datei einlesen
		if( readFile( argv_str.name ) < 0 )
			//Fehler
			return( -1 );

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
		lr.giveArgs( &argv_str );
		lr.yyParse();

		//Speicher für den Scanner freigeben
		delete( s );

		//Speicher für das Grammatik-File freigeben
		delete[] ( data );
	}

/*
#include "copcode.h"


	COneOperandOpcode *abc = new COneOperandOpcode( COpcode::_MOVE );
	cout << *abc << endl;

	cout.operator<<( abc );
	cout << endl;

	operator<<( cout, *abc );
*/
	return EXIT_SUCCESS;
}


int parseArgv( int argc, char *argv[], ARGV_STR *argv_str )
{
	//Argumente parsen
	static const char	*of = "ssass.o";
	char				*pos;
	int					i, err = 1, argCnt = 0;

	//mit Standard-Werten initialisieren

	//Name des Output-Files
	argv_str->outputFileName = of;

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
							cerr << "ssass: Use -v, -v0, -v1 and -v2!" << endl;

						//Zahl löschen
						pos++;
					}
				}

				else if( !strcmp( pos, "-help" ) )
				{
					//Hilfe ausgeben
					cout << "\n";
					cout << "usage: ssass [OPTIONS]... -a SOURCE -o OBJECT...\n\n";
					cout << "-vX verbose mode (-v0, -v1, -v2)\n";
					cout << "\n";

					//nächstes Argument
					break;
				}

				else if( *pos == 'o' )
				{
					//den Namen für das Objektfile angeben
					argv_str->outputFileName = argv[i];

					//Outputfile überspringen
					i++;
				}

				else if( *pos == 'a' )
				{
					//den Namen für das zu assemblierende File
					argv_str->name = argv[i];

					//überspringen
					i++;

					//zu assemblierendes File wurde angegeben
					if( err == 1 )
						//zurücksetzen
						err = 0;
				}

				else
				{
					//unbekannte Option
					cerr << "ssass: Unknown argument line option " << pos << "!" << endl;
					cerr << "ssass: --help for more Information" << endl;

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
		{
			//unbekannte Option
			cerr << "ssass: Unknown argument line option " << pos << "!" << endl;
			cerr << "ssass: --help for more Information" << endl;

			//Fehler
			err++;
		}
	}

	//kein Assembler-File angegeben?
	if( err )
		//fehlt
		cout << "sslr: No assembler file specified!\n";

	//etwaige Fehler
	return( err );
}


int readFile( const char *name )
{
	//Datei öffnen
	FILE	*file;

	//Grammatik-Datei öffnen
	file = fopen( name, "r" );

	//Informationen über die Datei einholen
	if( stat( name, &st ) )
	{
		//Fehler
		cout << endl << "\nssass: Unable to read file " << name << "!\n";
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

