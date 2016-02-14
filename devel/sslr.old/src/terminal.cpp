/***************************************************************************
                          terminal.cpp  -  description
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

#include "terminal.h"

Terminal::Terminal( long epsilonHashIndex )
{
	//Daten-Strukturen initialisieren
	term = 0L;
	anz = maxAnz = 0;
	epsilon = 0;

	//Hash-Index von EPSILON
	epsHashIndex = epsilonHashIndex;
}


Terminal::~Terminal()
{
	//Speicher freigeben?
	if( term )
		//Ja, bei term = 0L gibt's SEGFAULT
		delete[] ( term );
}


long Terminal::ins( long id )
{
	//Es soll ein Terminal eingetragen werden.
	long	*tmp, i;

	//Terminal schon eingetragen?
	for( i = 0; i < anz; i++ )
	{
		//Terminal schon enthalten?
		if( term[i] == id )
			//Ja
			return( epsilon );
	}

	//Epsilon?
	if( id == epsHashIndex )
		//Ja
		epsilon++;

	//Ist genügend Speicher vorhanden?
	if( anz >= maxAnz )
	{
		//Nein, anfordern

		//Genügend Speicher anfordern
		tmp = new( long[maxAnz + ANZTERM] );

		//Speicher übertragen
		memcpy( tmp, term, sizeof( long ) * anz );

		//neue maximale Terminal-Zahl
		maxAnz += ANZTERM;

		//alten Speicher freigeben
		if( term )
			delete[] ( term );

		//Zeiger anpassen
		term = tmp;
	}

	//Terminal der Reihenfolge nach eintragen
	for( i = 0; i < anz; i++ )
	{
		//Terminal schon enthalten?
		if( term[i] == id )
			//Ja
			break;

		//kleiner?
		else if( term[i] > id )
		{
			//Ja, umkopieren
			memmove( &term[i+1], &term[i], sizeof( long ) * ( anz - i ) );

			//neues eintragen
			term[i] = id;

			//ein Terminal mehr
			anz++;

			//Schleife verlassen
			break;
		}
	}

	//Alle verglichen?
	if( i == anz )
		//Ja, am Ende anhängen
		term[anz++] = id;

	//wegen EPSILON
	return( epsilon );
}


void Terminal::del( long id )
{
	//Es soll ein Terminal aus der Liste entfernt werden.
	long	i;

	//Terminal der Reihenfolge nach eintragen
	for( i = 0; i < anz; i++ )
	{
		//gleich?
		if( term[i] == id )
		{
			//Epsilon?
			if( id == epsHashIndex )
				//Ja
				epsilon = 0;

			//Ja, umkopieren
			memmove( &term[i], &term[i+1], sizeof( long ) * ( anz - i - 1 ) );

			//ein Terminal weniger
			anz--;

			//Schleife verlassen
			break;
		}
	}
}


bool Terminal::find( long id )
{
	//Ist das Element id enthalten?
	long	i;

	//Terminal der Reihenfolge nach eintragen
	for( i = 0; i < anz; i++ )
	{
		//gleich?
		if( term[i] == id )
			//Ja
			return( true );
	}

	//Nicht enthalten
	return( false );
}


Terminal &Terminal::operator+=( const Terminal &a )
{
	//Die Terminale von a zu diesem hinzufügen
	long	i;

	//Alle Terminale hier eintragen
	for( i = 0; i < a.anz; i++ )
		//HIER eintragen
		ins( a.term[i] );

	//zurückliefern
	return( *this );
}


Terminal &Terminal::operator-=( const Terminal &a )
{
	//Die Terminale von a aus diesem entfernen
	long	i;

	//Alle Terminale hier löschen
	for( i = 0; i < a.anz; i++ )
		//HIER löschen
		del( a.term[i] );

	//zurückliefern
	return( *this );
}


long Terminal::operator[]( const int index )
{
	//Operator [] überladen

	//gibt es überhaupt Terminale?
	if( term )
	{
		//Index ok?
		if( ( index < 0 ) || ( index > anz ) )
		{
			//nein

			//Fehlermeldung
			cerr << "sslr: ERROR: Terminal::operator[]: Index out of range!\n";
			return( -1 );
		}

		//Index des Terminals in der Hash-Tabelle zurückliefern
		return( term[index] );
	}

	else
	{
		//nein

		//Fehlermeldung
		cerr << "sslr: ERROR: Terminal::operator[]: Index out of range!\n";
		return( -1 );
	}
}


bool Terminal::cmp( const Terminal *a )
{
	//Keine Ahnung, wie man den Operator == überlädt.
	//Terminale dieses Objekts und das von a gleich?
	long	anzA = a->anz;

	//Anzahl der Terminale gleich?
	if( anz == anzA )
		//Vergleichen
		return( !memcmp( term, a->term, sizeof( long ) * anz ) ? true : false );

	else
		//nicht gleich
		return( false );
}


void Terminal::add( const Terminal *a )
{
	//Die Terminale von a zu diesem hinzufügen
	long	i;

	//Alle Terminale hier eintragen
	for( i = 0; i < a->anz; i++ )
		//HIER eintragen
		ins( a->term[i] );
}


long Terminal::getAnzElem( void )
{
	//Anzahl der eingetragenen Terminale zurückliefern
	return( anz );
}


bool Terminal::isEpsilon( void )
{
	//Liefert true, wenn EPSILON in dieser Terminal-Menge enthalten ist
	return( epsilon ? true : false );
}


void Terminal::print( void )
{
	//Nur für Debugging-Zwecke, Ausgabe der Terminals
	long	i;

	//newline
	cerr << "\n";

	for( i = 0; i < anz; i++ )
		//Terminal ausgeben
		cerr << i << ": " << term[i] << "\n";

	//newline
	cerr << "\n";
}


