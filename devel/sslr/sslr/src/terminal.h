/***************************************************************************
                          terminal.h  -  description
                             -------------------
    begin                : Don Aug 15 2002
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

#ifndef TERMINAL_H
#define TERMINAL_H


/**In dieser Klasse werden die Terminale verwaltet, also FOLLOW, FIRST und die LOOKAHEADS.
  *@author Sven Schmidt
  */

#include <cstdio>
#include <iostream>
#include <cstring>

//WICHTIG
using namespace std;



//Anzahl der Terminale, um die term erweitert werden soll
#define ANZTERM 100


class Terminal
{

private:

	//private Variablen

	//Array mit den Terminalen. Dies sind die Hash-Indices, die dann auf einen Eintrag für jedes
	//Terminal verweist.
	//Die Hash-Indices werden der Reihenfolge nach sortiert, damit ein vergleich effizienter ist.
	long	*term;

	//Anzahl der Terminale in term
	long	anz;

	//Anzahl der maximal möglichen Terminale in term
	long	maxAnz;

	//diese Menge enthält EPSILON
	int		epsilon;

	//Hash-Index von EPSILON
	long	epsHashIndex;




public: 

	//public Methoden
	Terminal( long );
	~Terminal();

	//Operator += überladen
	Terminal &operator+=( const Terminal & );

	//Operator += überladen
	Terminal &operator-=( const Terminal & );

	//Operator [] überladen
	long operator[]( const int );

	/*
		Hier wird geprüft, ob die Terminale in a auch in this enthalten
		sind.
	*/
	bool bgt( const Terminal * );

	//die Terminale beider Objekte vergleichen
	bool cmp( const Terminal * );

	//Operator = überladen
	//Operator == überladen
	//Operator != überladen

	//Die Terminale von a zu diesem hinzufügen
	void add( const Terminal * );

	//Terminal eintragen
	long ins( long );

	//Terminal löschen
	void del( long );

	//Terminals printen
	void print( void );

	//Anzahl der eingetragenen Terminale zurückliefern
	long getAnzElem( void );

	//Liefert true, wenn EPSILON in dieser Terminal-Menge enthalten ist
	bool isEpsilon( void );

	//Element suchen
	bool find( long );
};

#endif
