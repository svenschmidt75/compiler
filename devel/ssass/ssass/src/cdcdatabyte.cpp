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
#include "cdcdatabyte.h"

CDCDataByte::CDCDataByte() : CDCData()
{
}


CDCDataByte::~CDCDataByte()
{
}


unsigned long CDCDataByte::getSize( void ) const
{
	//die Größe dieses Bereichs zurückliefern
	unsigned long	_size = _array.size();
	unsigned char	_typeSize = sizeof( unsigned char );

	//Anzahl der Elemente
	return( _size * _typeSize );
}


CDCData::_typeId CDCDataByte::getInternType( void ) const
{
	//Problem: Siehe COpcode-Klasse
	return( _CDCDataByte );
}


void CDCDataByte::insertElement( unsigned char elem )
{
	//ein Element eintragen
	_array.push_back( elem );
}


void CDCDataByte::insertString( char *elem )
{
	//einen String eintragen
	unsigned int strSize = strlen( elem ), i;

	//alle Zeichen einfach eintragen ins Array
	for( i = 1; i < ( strSize - 1 ); i++ )
		_array.push_back( elem[i] );

	//mit Null-Byte abschließen
	_array.push_back( 0 );
}


std::ostream &operator<<( std::ostream &os, const CDCDataByte &cls )
{
	//die Daten ausgeben
	unsigned long	_size = cls._array.size(), i;

	//alle Elemente ausgeben
	for( i = 0; i < _size; i++ )
		//mit Standard-<< ausgeben
		os << cls._array[i];

	//Referenz auf den ostream zurückliefern
	return( os );
}

