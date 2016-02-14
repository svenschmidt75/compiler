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
#ifndef CDCDATABYTE_H
#define CDCDATABYTE_H

#include <vector>

#include "cdcdata.h"


/*
	Hier werden die Daten aus dem DATA-Segment verwaltet. Pro Zeile wird per
	.dc.b eine CDCDataByte-Klasse angelegt, in der die Daten der Zeile
	gespeichert werden.
	Per << können sie dann ausgegeben werden.
*/



/**
@author Sven Schmidt
*/
class CDCDataByte : public CDCData
{
public:
	CDCDataByte();
	~CDCDataByte();

	//damit << auf die privaten Members zugreifen darf
	friend std::ostream &operator<<( std::ostream &, const CDCDataByte & );

	//die Größe dieses Bereichs zurückliefern
	virtual unsigned long getSize( void ) const;

	//Problem: Siehe COpcode-Klasse
	virtual _typeId getInternType( void ) const;

	//ein Element eintragen
	void insertElement( unsigned char );

	//einen ganzen String eintragen
	void insertString( char * );


private:

	//Array mit den Bytes
	std::vector<unsigned char>	_array;
};

#endif
