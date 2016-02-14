/***************************************************************************
                          hashclass.h  -  description
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

#ifndef HASHCLASS_H
#define HASHCLASS_H


/**Hier wird die Hash-Tabelle implementiert.

  *@author Sven Schmidt
  */

#include "terminal.h"
#include "sslr.h"



//Struktur, die die Hash-Tabelle repräsentiert
typedef struct
{
	//String des Terminals/Nicht-Terminals
	char		*name;

	//Bei Nichtterminal Index der Produktion in PROD_STR
	long		prod_index;

	//Typ: NTERM/TERM
	int			type;

	//Spalten-Index dieses Elements in der LR-Tabelle
	int			lrTabIndex;

	//FIRST-Mengen
	Terminal	*first;

	//FOLLOW-Mengen
	Terminal	*follow;

	//Assoziativität, mit %left/%right/%nonassoc festgelegt
	int			assoc;

	//Priorität des Terminals
	int			priority;

	//(Nicht)Terminale wurde definiert
	int			defined;

	//Zeile, in der das Terminal definiert wurde
	long		defLine;

	//Produktion wurde definiert
	bool		prodDef;

	//(Nicht)Terminal wurde benutzt
	bool		used;

	//%union-Typ
	int			unionIndex;


}HASH_TAB;



class hashClass
{
private:
	//Private Variablen

	//Hash-Tabelle
	HASH_TAB	*hash_tab;

	//Hash-Index von EPSILON
	long		epsHashIndex;


	//private Methoden

	//Den Index in der Hash-Tabelle berechnen
	long hashpjw( char * );

	//Kollisionen abfangen
	long collision( char * );

	//Prüfen, ob Eintrag bereits vorhanden
	bool find( char *, long );


public: 
	hashClass();
	~hashClass();


	//Namen eintragen
	long insert( char * );

	//Assoziativität eintragen
	void setAssociation( long, int );

	//Priorität eintragen
	void setPriority( long, int );

	//Typ: Terminal oder Nichtterminal
	int setType( long, int );

	//(Nicht)Terminal wurde definiert
	void setDefined( long, long );

	//Typ für semantische Aktionen
	void setUnionType( long, int );

	//Typ für semantische Aktionen, aus %union
	int getUnionType( long index );

	//Index der Produktion in PROD_STR
	void setProdStr( long, long );

	//Index der Produktion in PROD_STR
	long getProdStr( long );

	//Priorität des (Nicht)Terminals
	int getPriority( long );

	//Assoziativität des (Nicht)Terminals
	int getAssociation( long );

	//Größe der Hash-Tabelle
	long getHashTabSize( void );

	//(Nicht)Terminal definiert in Zeile
	long getDefLine( long );

	//Den Namen zurückliefern
	char *getName( long );

	//Liefert den Typ des Elements zurck
	int getType( long );

	//(Nicht)Terminal wurde definiert
	int getDefined( long );

	//(Nicht)Terminal wurde benutzt
	void setUsed( long );

	//(Nicht)Terminal wurde benutzt?
	bool getUsed( long );

	//Die FIRST-Elemente zurückliefern
	Terminal *getFirst( long );

	//Die FOLLOW-Elemente zurückliefern
	Terminal *getFollow( long );

	//Die Anzahl der FIRST-Elemente zurückliefern
	long getFirstAnz( long );

	//Die Anzahl der FOLLOW-Elemente zurückliefern
	long getFollowAnz( long );

	//FIRST-Menge ausgeben
	void printFirst( long );

	//FOLLOW-Menge ausgeben
	void printFollow( long );

	//Produktion wurde definiert
	void setProdDef( long );

	//Produktion wurde definiert?
	bool getProdDef( long );

	//Den Spalten-Index dieses Elements in der LR-Tabelle eintragen
	void setLRTabIndex( long, int );

	//Den Spalten-Index dieses Elements in der LR-Tabelle zurckgeben
	int getLRTabIndex( long );
};

#endif
