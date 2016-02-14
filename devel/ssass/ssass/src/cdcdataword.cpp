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
#include "cdcdataword.h"

CDCDataWord::CDCDataWord() : CDCData()
{
}


CDCDataWord::~CDCDataWord()
{
}


unsigned long CDCDataWord::getSize( void ) const
{
	//die Größe dieses Bereichs zurückliefern
	unsigned long	_size = _array.size();
	unsigned char	_typeSize = sizeof( unsigned short );

	//Anzahl der Elemente
	return( _size * _typeSize );
}


CDCData::_typeId CDCDataWord::getInternType( void ) const
{
	//Problem: Siehe COpcode-Klasse
	return( _CDCDataWord );
}


void CDCDataWord::insertElement( unsigned short elem )
{
	//ein Element eintragen
	_array.push_back( elem );
}


std::ostream &operator<<( std::ostream &os, const CDCDataWord &cls )
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

