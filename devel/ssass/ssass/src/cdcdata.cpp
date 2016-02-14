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
#include "cdcdata.h"

CDCData::CDCData()
{
	//in Type steht der Typ für die Spezialisierung
}


CDCData::~CDCData()
{
}


void CDCData::setLine( unsigned long line )
{
	//Zeile eintragen
	_line = line;
}


unsigned long CDCData::getLine( void ) const
{
	//Zeile zurückliefern
	return( _line );
}


unsigned long CDCData::getSize( void ) const
{
	//die Größe dieses Bereichs zurückliefern
	return( 0 );
}


CDCData::_typeId CDCData::getInternType( void ) const
{
	//Problem: Siehe COpcode-Klasse
	return( _CDCData );
}
