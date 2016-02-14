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
#ifndef CDCDATASINGLE_H
#define CDCDATASINGLE_H

#include <vector>

#include <cdcdata.h>

/**
@author Sven Schmidt
*/
class CDCDataSingle : public CDCData
{
public:
	CDCDataSingle();
	~CDCDataSingle();

	//damit << auf die privaten Members zugreifen darf
	friend std::ostream &operator<<( std::ostream &, const CDCDataSingle & );

	//die Größe dieses Bereichs zurückliefern
	virtual unsigned long getSize( void ) const;

	//Problem: Siehe COpcode-Klasse
	virtual _typeId getInternType( void ) const;

	//ein Element eintragen
	void insertElement( float );


private:

	//Array mit den Floats
	std::vector<float>	_array;
};

#endif
