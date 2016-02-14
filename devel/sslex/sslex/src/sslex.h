/***************************************************************************
                          sslex.h  -  description
                             -------------------
    begin                : Don Feb 6 2003
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

#ifndef SSLEX_H
#define SSLEX_H

//in config.h steht die Version
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//Debug-Version
#define DEBUG
#undef DEBUG




//7- oder 8-Bit-Scanner
#define _7BIT	128
#define _8BIT	256



//Übergabe-Argumente, um den Parser zu steuern
typedef struct
{
	//Ausführliche Meldungen
	bool		verbose;

	//Debug-Modus
	bool		debug;

	//7- oder 8-Bit-Scanner
	int			cclWidth;

	//Zeige Konflikte zwischen den regulären Ausdrücken
	bool		showConflicts;

}ARGV_STR;



//Struktur für NEA
typedef struct
{
	//Start-Zustand
	ulong	startState;

	//Finit-Zustand
	ulong	endState;

	//ist ein Lookahead-NEA oder nicht
	bool	laState;

	//ist ein <<EOF>-NEA
	bool	eof;

	//NEA darf nur am Anfang einer Zeile stehen
	bool	begLine;

	//Aktion, verbunden mit einer Regel
	char	**action;

	//Anzahl der Aktionen
	int		actionAnz;

	//regulärer Ausdruck in Text-Form
	char	*regText;

	//Zeile, in der der reg. Ausdruck definiert wurde
	ulong	line;

}NEA_STR;



//Struktur mit den regulären Ausdrücken, die über ihren Namen, {NAME}, angesprochen werden
typedef struct
{
	//Name des regulären Ausdrucks
	char	*name;

	//regulärer Ausdruck als Text
	char	*text;

	//Zeile, in der die Definition steht
	long	line;

	//gibt an, ob der reg. Ausdruck überhaupt benutzt wurde
	bool	used;

}NAME_NEA;


//inkludiere die Terminal-Definitionen sowie die union-Angabe
#include "sslrtermdef.h"


#endif

