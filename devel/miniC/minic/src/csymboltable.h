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
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>

using namespace std;


/**
@author Sven Schmidt
*/

class CSymbolTable
{
public:

	//Typ der möglichen Variablen
	enum storageType{ _CHAR = 1, _BOOL, _INT, _SHORT, _LONG };

	//Variable, Konstante, (später Funktion)
	enum varType{ _VAR = 1, _NUMBER };


private:

	//Größe der Symboltabelle
	static const unsigned long	symTabSize = 1000;


	//Struktur für die Verwendung von Symboltabellen-Einträgen
	struct SYMBOL_TABLE
	{
		//Name der Variablen
		char				*_name;

		/*
			Jeder Block hat ein display. Hier steht der Offset zur Basis, in der
			die Varaible gespeichert ist.
			z.B.

			int a,b;
			{
				int a,b,c;
			}
			int d,e;

			Verschachtelungstiefe 0 (global):
				a: 0
				b: sizeof(int)
				d: 2*sizeof(int)
				e: 3*sizeof(int)

			Verschachtelungstiefe 1:
				a: 0
				b: sizeof(int)
				c: 2*sizeof(int)
		*/
		int					_offset;

		//Typ der Variablen, also CHAR, INT, SHORT, ...
		storageType			_storageType;

		//Variable/Konstante (später Funktion)
		varType				_type;

		//Verschachtelungstiefe bei Verwendung von Blöcken, 0 für global definierte Variablen
		int					_blk;

		//Index im saveSymTab Array
		long				_saveSymTabIndex;

		//Vereinbarungen wurde benutzt
		bool				_used;

		//Zeile der Deklaration
		long				_line;

		//Vereinbarung ist konstant
		bool				_const;

		//Vereinbarung ist signed
		bool				_signed;

		//Zeiger auf nächste Symboltabellen-Struktur deselben Blocks
		struct SYMBOL_TABLE	*_next;

		//Zeiger auf nächste Symboltabellen-Struktur
		struct SYMBOL_TABLE	*_prev;
	};


	//private Variablen

	/*
		Die Symboltabelle besteht im Wesentlichen aus einem statischen Array, welches Zeiger auf
		SYMBOL_TABLE enthält. Der Array-Index ist ein aus dem Namen des Eintrags generierter
		Hash-Index. Die Position in der Verkettung (durch next) wird in einem Integer gespeichert.
	*/
	SYMBOL_TABLE	**symTab;

	//Position in symTab, d.h. aktuelle Blocktiefe
	long			symTabPos;

	//maximale Blocktiefe
	unsigned long	symTabMaxPos;

	/*
		Die Routinen zur Codeerzeugung und zum Aufbau des Syntaxbaumes
		sollen keine direkte Kontrolle über Symboltabellen-Einträge
		erhalten. Deshalb liefert insert eine Zahl zurück, die eine
		eindeutige Zuordnung der verschiedenen Variablen etc. ermöglicht.
		Dazu werden ALLE erzeugten Symboltabellen-Einträge in einem Array
		gespeichert und der Index wird von insert zurückgeliefert.
	*/
	SYMBOL_TABLE	**saveSymTab;

	//Anzahl der Einträge in saveSymTab
	long			saveSymTabPos;

	//maximal mögliche Anzahl in saveSymTab
	unsigned long	saveSymTabMaxPos;







	//Suchen eines Eintrags
	SYMBOL_TABLE *lookup( char *, int ) const;

	//einen neu erzeugten Eintrag speichern
	unsigned long insertSaveSymTab( SYMBOL_TABLE * );




public:

	CSymbolTable();
    ~CSymbolTable();

	//es soll ein Element in die Symboltabelle eingetragen werden
	long insert( char *, int, long );

	//den Typ einer Variablen/Konstanten eintragen
	void setStorageType( long, storageType );
	storageType getStorageType( long );

	//es wird ein neuer Block betreten
	void incBlockDepth( void );

	//es wird ein Block verlassen
	void decBlockDepth( void );

	/*
		Lokale Variablen werden auf den Laufzeit-Stack abgelegt. Nun wird der Offset
		zu dieser Variablen eingetragen.
		Bei globalen Variablen ist es ähnlich.
	*/
	void setOffset( long, int );
	int getOffset( long );

	//Vereinbarung ist entweder eine Variable oder eine Zahl
	void setType( long, varType );
	varType getType( long );

	/*
		Wenn in einer expression auf eine Variable zugegriffen wird, dann wird hier der
		entsprechende Eintrag in der Symboltabelle gesucht.
		Dazu wird zuerst im aktuellen Block gesucht. Wird der Eintrag nicht gefunden, wird in einem
		vorherigen Block gesucht (Verschachtelungstiefe).
		Der letztmögliche Block ist der, in dem die globalen Vereinbarungen getroffwn wurden.
	*/
	long find( char * );

	/*
		Wenn die Blocktiefe dieser Vereinbarung 0 ist, dann ist es eine globale
		Variable.
	*/
	bool isLocal( long );

	//liefert die Breite in Bytes eines jeden storage types zurück
	int getWidthInBytes( storageType );

	//liefert einen Zeiger auf den Namen der Vereinbarung zurück
	char *getName( long );

	//liefert die Blocktiefe der Vereinbarung zurück
	int getBlkDepth( long );
};

#endif
