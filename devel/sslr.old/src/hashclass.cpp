/***************************************************************************
                          hashclass.cpp  -  description
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

#include "hashclass.h"


hashClass::hashClass()
{
	//Klasse initialisieren

	//Speicher für die Hash-Tabelle anfordern
	hash_tab = new( HASH_TAB[HASH_SIZE] );

	//Initialisieren
	memset( hash_tab, 0, sizeof( HASH_TAB ) * HASH_SIZE );

	//Hash-Index holen
	epsHashIndex = hashpjw( "EPSILON" );

	//Hash-Index von EPSILON eintragen
	insert( "EPSILON" );
}


hashClass::~hashClass()
{
	//Speicher freigeben
	unsigned long	i;
	HASH_TAB		*h;

	//Alle Speicherbereiche freigeben
	for( i = 0; i < HASH_SIZE; i++ )
	{
		//Zeiger auf Hash-Struktur
		h = &hash_tab[i];

		//Speicher belegt?
		if( h->name )
			delete[] ( h->name );

		//FIRST-Mengen?
		if( h->first )
			delete( h->first );

		//FOLLOW-Mengen?
		if( h->follow )
			delete( h->follow );
	}

	delete[] ( hash_tab );
}


long hashClass::insert( char *n )
{
	//Den Namen eintragen
	HASH_TAB	*h;
	long		index;
	
	//Hash-Index holen
	index = collision( n );

	//Zeiger auf Hash-Eintrag
	h = &hash_tab[index];

	//schon eingetragen?
	if( ( h->name ) && !strcmp( h->name, n ) )
		//ja
		return( index );

	//eintragen
	h->name = new( char[strlen(n)+1] );
	strcpy( h->name, n );

	//FIRST-Menge initialisieren
	hash_tab[index].first = new Terminal( epsHashIndex );

	//FOLLOW-Menge initialisieren
	hash_tab[index].follow = new Terminal( epsHashIndex );

	//Produktion nicht definiert
	hash_tab[index].prodDef = false;
	
	//Hash-Index zurückliefern
	return( index );
}


long hashClass::collision( char *name )
{
	//Falls ein Eintrag in der Hash-Tabelle bereits besetzt ist, muss ein anderer,
	//freier gefunden werden.
	long	up, down, index;

	//Hash-Index holen
	index = hashpjw( name );

	//Eintrag bereits belegt?
	if( hash_tab[index].name )
	{
		//ist es dieser String?
		if( !strcmp( hash_tab[index].name, name ) )
			//ja
			return( index );

		//Index
		up = index;
		down = index;

		//den nächsten freien Platz suchen
		do
		{
			//freien Eintrag finden
			up += 2;
			down -= 2;

			//Überlauf?
			if( up >= HASH_SIZE )
				//Zu groß
				up = 0;

			//Unterlauf?
			if( down < 0 )
				down = HASH_SIZE;

			//Ist es dieser String?
			if( ( hash_tab[up].name ) && !strcmp( hash_tab[up].name, name ) )
				//Ja
				return( up );

			//Ist es dieser String?
			else if( ( hash_tab[down].name ) && !strcmp( hash_tab[down].name, name ) )
				//Ja
				return( down );

		}while( hash_tab[up].name && hash_tab[down].name );

		//up oder down egal
		if( !hash_tab[up].name )
			//dieser ist es
			index = up;

		else
			//dann muss es dieser sein
			index = down;
	}

	//Hash-Index zurückliefern
	return( index );
}


long hashClass::hashpjw( char *name )
{
	//Hash-Index erzeugen
	register char	*p;
	unsigned long	h = 0, g;
	long			length = HASH_SIZE;

	for( p = name; *p; p = p + 1 )
	{
		h = ( h << 4 ) + *p;

		if( g = h & 0xf0000000L )
		{
			h = h ^ ( g >> 24 );
			h = h ^ g;
		}
	}

	return( ( long ) ( ( h % length ) + 1 ) );
}


bool hashClass::find( char *name, long index )
{
	//Platz schon belegt?
	if( hash_tab[index].name )
		return( !strcmp( name, hash_tab[index].name ) ? true : false );

	else
		return( false );
}


void hashClass::setAssociation( long index, int assoc )
{
	//Assoziativität eintragen
	hash_tab[index].assoc = assoc;
}


void hashClass::setPriority( long index, int priority )
{
	//Priorität eintragen
	hash_tab[index].priority = priority;
}


int hashClass::setType( long index, int type )
{
	//Typ: Terminal oder Nichtterminal
	int	thisType = hash_tab[index].type;

	//schon gesetzt?
	if( ( thisType ) && ( thisType != type ) )
		//Fehler, Typ schon eingetragen und anders
		return( thisType );

	else
	{
		//Alles OK
		hash_tab[index].type = type;

		//Terminal?
		if( type == SSLR_TERM )
		{
			//Terminal ist seine eigene FIRST-Menge
			if( hash_tab[index].first )
				//OK, eintragen
				hash_tab[index].first->ins( index );

			else
				//FIRST-Menge noch nicht initialisiert
				cerr << "sslr: hashClass::setType: FIRST-Set not yet initialized!\n";
		}

		//Typ zurückliefern
		return( type );
	}
}


void hashClass::setDefined( long index, long line )
{
	//(Nicht)Terminal wurde definiert
	hash_tab[index].defined++;

	//Zeile schon eingetragen?
	if( !hash_tab[index].defLine )
		//Nein, eintragen
		hash_tab[index].defLine = line;
}


void hashClass::setUsed( long index )
{
	//(Nicht)Terminal wurde benutzt
	hash_tab[index].used = true;
}


void hashClass::setUnionType( long index, int unionIndex )
{
	//Typ für semantische Aktionen, aus %union
	hash_tab[index].unionIndex = unionIndex;
}


int hashClass::getUnionType( long index )
{
	//Typ für semantische Aktionen, aus %union
	return( hash_tab[index].unionIndex );
}


int hashClass::getPriority( long index )
{
	//Priorität des (Nicht)Terminals
	return( hash_tab[index].priority );
}


int hashClass::getAssociation( long index )
{
	//Assoziativität des (Nicht)Terminals
	return( hash_tab[index].assoc );
}


long hashClass::getHashTabSize( void )
{
	//Größe der Hash-Tabelle
	return( HASH_SIZE );
}


char *hashClass::getName( long index )
{
	//Den Namen zurückliefern
	return( hash_tab[index].name );
}


int hashClass::getType( long index )
{
	//Typ, also Terminal oder Nichtterminal
	return( hash_tab[index].type );
}


long hashClass::getDefLine( long index )
{
	//Zeile der Definition zurückliefern
	return( hash_tab[index].defLine );
}


int hashClass::getDefined( long index )
{
	//(Nicht)Terminal definiert?
	return( hash_tab[index].defined );
}


bool hashClass::getUsed( long index )
{
	//(Nicht)Terminal benutzt?
	return( hash_tab[index].used );
}


Terminal *hashClass::getFirst( long index )
{
	//Die FIRST-Elemente zurückliefern
	return( hash_tab[index].first );
}


Terminal *hashClass::getFollow( long index )
{
	//Die FOLLOW-Elemente zurückliefern
	return( hash_tab[index].follow );
}


long hashClass::getFirstAnz( long index )
{
	//Die Anzahl der FIRST-Elemente zurückliefern
	return( hash_tab[index].first->getAnzElem() );
}


long hashClass::getFollowAnz( long index )
{
	//Die Anzahl der FOLLOW-Elemente zurückliefern
	return( hash_tab[index].follow->getAnzElem() );
}


void hashClass::printFirst( long index )
{
	//FIRST-Menge ausgeben
	Terminal	*la;
	HASH_TAB	*h;
	long		anz;
	long		i;

	//Zeiger auf die Hash-Struktur
	h = &hash_tab[index];

	//Anzahl der Terminale
	anz = h->first->getAnzElem();

	//Zeiger auf die Terminale
	la = h->first;

	//Alle Terminale
	for( i = 0; i < anz; i++ )
		//ausgeben
		cout << i << ": " << hash_tab[(*la)[i]].name << "\n";
}


void hashClass::printFollow( long index )
{
	//FOLLOW-Menge ausgeben
	Terminal	*la;
	HASH_TAB	*h;
	long		anz;
	long		i;

	//Zeiger auf die Hash-Struktur
	h = &hash_tab[index];

	//Anzahl der Terminale
	anz = h->follow->getAnzElem();

	//Zeiger auf die Terminale
	la = h->follow;

	//Alle Terminale
	for( i = 0; i < anz; i++ )
		//ausgeben
		cout << i << ": " << hash_tab[(*la)[i]].name << "\n";
}

void hashClass::setProdStr( long index, long prod_index )
{
	//Index der Produktion in PROD_STR
	hash_tab[index].prod_index = prod_index;
}


long hashClass::getProdStr( long index )
{
	//Index der Produktion in PROD_STR
	return( hash_tab[index].prod_index );
}


void hashClass::setProdDef( long index )
{
	//Produktion wurde definiert
	hash_tab[index].prodDef = true;
}


bool hashClass::getProdDef( long index )
{
	//Produktion wurde definiert?
	return( hash_tab[index].prodDef );
}


void hashClass::setLRTabIndex( long index, int column )
{
	//Den Spalten-Index dieses Elements in der LR-Tabelle eintragen
	hash_tab[index].lrTabIndex = column;
}


int hashClass::getLRTabIndex( long index )
{
	//Den Spalten-Index dieses Elements in der LR-Tabelle zurückgeben
	return( hash_tab[index].lrTabIndex );
}


