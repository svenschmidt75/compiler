/***************************************************************************
                          scanner.h  -  description
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

#ifndef SCANNER_H
#define SCANNER_H


/**Hier wird der Scanner implementiert, der die Grammatik-Datei tokenisiert.
  *@author Sven Schmidt
  */


#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <sys/stat.h>

//WICHTIG
using namespace std;



//Die Token, die der Scanner erkennt
enum token
{
	IDENT = 0,
	END,
	SEPERATOR,
	LEFT,
	RIGHT,
	NONASSOC,
	TERM_TYPE,
	NTERM_TYPE,
	PREC,
	ERROR,
	PROD_END,
	PROD_NEXT,
	INCL,
	DEF,
	GRAMMAR,
	SEM_TYPE,
	BOPEN,
	BCLOSE,
	KLOPEN,
	KLCLOSE,
	EOPEN,
	ECLOSE,
	UNION,
	NUMBER,
	STRING,
	SEM_ACT,
	EXTENSION,
	TERMINAL,
	EXCLAMATION,
	COMMA,
	AST
};




//Scanner-Zustände
#define DEFAULT_STATE		0
#define INCL_STATE			100
#define SEMACT_STATE		101
#define COMENT_C_STATE		102
#define COMENT_CPP_STATE	103
#define DIGIT_STATE			104
#define IDENT_STATE			105
#define STRING_STATE		106
#define TERMINAL_STATE		107
#define EXTENSION_STATE		108


//Struktur für den Scanner
typedef union scanner_str
{
	//Die Attribute der Token, die der Scanner zurückliefert, hängen vom Token ab.
	//Dies wird mit einer union befriedigt.
	char	*name;

	//Zahl
	long	l;

	//Bool
	bool	b;

}SCAN_STR;


//Typ des zurückgelieferten Tokens
typedef int SSLEXToken;



class scanner
{
private:
	//private Veriablen
	SCAN_STR		*attribut_str;

	//aktuelle Zeile
	unsigned long	line;

	//Zeile, bei der die semantische Aktion beginnt
	unsigned long	semActLine;

	//Puffer, in dem die eingelesene Grammatik-Datei steht
	char			*base, *pos, *end;

	//Speicher für die union
	char			*unionStr;

	//String für die semmantischen Angaben in der Grammatik-Datei
	char			*sem;

	//String für Include-Anweisungen
	char			*incl;

	//String für eigene Erweiterungen
	char			*extension;

	//Zustand
	int				state;

	//Speicher für einen String
	char			*string;

	//Fehler
	int				err;

	//Flag für union-Erkennung
	bool			unionFlag;



	//private Methoden
	void discard( char );

	//union-Bereich scannen
	void unionText( void );



public: 
	scanner();
	scanner( char *, SCAN_STR ** );
	~scanner();


	//nächstes Token zurückliefern
	SSLEXToken getToken( void );

	//Zeilen-Nummer zurückgeben
	unsigned long getLine( void );
};

#endif
