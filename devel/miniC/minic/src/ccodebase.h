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
#ifndef CCODEBASE_H
#define CCODEBASE_H

#include <iostream>
#include "csyntaxtree.h"
#include "csymboltable.h"

using namespace std;

/**
@author Sven Schmidt
*/


/*
	Dies ist die Basisklasse für die Codeerzeugung. Alle Assembler-Befehle
	werden von dieser Klasse abgeleitet.
*/


class CCodeBase
{
public:

	CCodeBase();
	CCodeBase( CSymbolTable *, CSyntaxTree * );
	~CCodeBase();


	//Typ für Register
	typedef int Register;




	/*
		Dieser Routine wird der Index der Wurzel des Syntaxbaumes übergeben, für
		den hier Code erzeugt werden soll.
	*/
	void print( long );


private:

	//Maximale Zahl der Register
	static const int	maxRegister = 256;

	/*
		Datenregister. Unsere virtuelle Maschine hat eine fest vorgegebene
		Anzahl an Datenregistern. Dadurch ist es möglich, in der Auswertung
		von Ausdrücken die Ergebnisse in Datenregistern zu speichern.
	*/
	static Register		dataReg[maxRegister];

	/*
		Adressregister. Unsere virtuelle Maschine hat eine fest vorgegebene
		Anzahl an Adressregistern.
	*/
	static Register		adressReg[maxRegister];


	//Zeiger auf den Syntaxbaum
	CSyntaxTree			*syntaxTree;

	//Zeiger auf die Symboltabelle
	CSymbolTable 		*symTab;

	//Zeile im Quelltext
	long				line;







	//lädt eine lokale/globale Vereinbarung in ein Datenregister
	Register LoadRValue( long );

	//speichert den Inhalt des Datenregisters reg in eine Vereinbarung
	void SaveLValue( long, CCodeBase::Register );


	/*
		Nächstes freies Register holen.

		static sorgt dafür, dass in allen abgeleiteten Klassen immer auf
		dieselbe Elementfunktion zugegriffen wird.
	*/
	static Register getFreeDataRegister( void );

	//ein Datenregister freigeben
	static void freeDataRegister( Register );

	//nächstes freies Adressregister holen
	static Register getFreeAdressRegister( void );

	//Adressregister freigeben
	void freeAdressRegister( Register );

	//Storage Type ausgeben
	void printStorageType( CSymbolTable::storageType ) const;
};

#endif
