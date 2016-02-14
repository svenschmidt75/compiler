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
#include <cstring>

#include "clabel.h"

CLabel::CLabel()
{
	//initialisieren
	_name = 0L;
	_adr = 0L;
	_export = false;
	_import = false;
	_local = false;
	_setDefined = false;
}


CLabel::~CLabel()
{
	//Speicher freigeben?
	if( _name )
		//ja
		delete[] ( _name );
}


void CLabel::setName( char *name )
{
	//den Namen eines Labels eintragen

	/*
		Problem: Wenn eine Deklaration der Art
		.import label1
		gefunden wird, dann wird es als extern deklariert. Wenn nun aber ein Label der Art
		label1:
			nop
		benutzt wird, dann muss eine Fehlermeldung auftauchen, da nicht klar ist,
		welches benutzt werden soll.
	*/
	if( !_name )
	{
		//noch nicht eingetragen
		int	len = strlen( name );

		//Speicher anfordern
		_name = new( char[len + 1] );

		//kopieren
		strcpy( _name, name );
	}
}


void CLabel::setExport( void )
{
	//das Label in anderen Modulen zugänglich machen
	_export = true;
}


void CLabel::setImport( void )
{
	//das Label wurde in einem anderen Modul deklariert
	_import = true;
}


void CLabel::setLocal( void )
{
	//das Label ist nur im aktuellen Modul erlaubt
	_local = true;
}


bool CLabel::isExport( void )
{
	//prüft, ob das Label exportiert werden soll
	return( _export );
}


bool CLabel::isImport( void )
{
	//prüft, ob das Label importiert werden muss
	return( _import );
}


bool CLabel::isLocal( void )
{
	//prüft, ob das Label nur im aktuellen Modul gültig ist
	return( _local );
}


const char *CLabel::getName( void )
{
	//liefert den Namen des Labels zurück
	return( _name );
}


void CLabel::setAdress( long adr )
{
	//Adresse des Labels eintragen
	_adr = adr;
}


long CLabel::getAdress( void )
{
	//Adresse des Labels erfragen
	return( _adr );
}


void CLabel::setDefined( void )
{
	//Label wurde definiert
	_setDefined = true;
}


bool CLabel::getDefined( void )
{
	//prüft, ob das Label definiert wurde
	return( _setDefined );
}


void CLabel::setUsed( void )
{
	//dieses Label wird benutzt
	_isUsed = true;
}


bool CLabel::isUsed( void ) const
{
	//prüft, ob dieses Label benutzt wird
	return( _isUsed );
}


void CLabel::setLine( unsigned long line )
{
	//Zeile eintragen, in der das Label definiert wurde
	_line = line;
}


unsigned long CLabel::getLine( void ) const
{
	//Zeile, in der das Label definiert wurde, zurückgeben
	return( _line );
}
