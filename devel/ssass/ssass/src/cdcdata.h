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
#ifndef CDCDATA_H
#define CDCDATA_H

#include <iostream>

/**
@author Sven Schmidt
@TODO Dies ist wahrscheinlich ein awful class design bur at the moment I don#t
know better. Maybe a template class would be nicer.
*/

class CDCData
{
public:

	//Kon- und Destruktor
	CDCData();
	virtual ~CDCData();

	//wird für die Identifizierung der jeweiligen (abgeleiteten) Klasse benötigt
	enum _typeId { _CDCData, _CDCDataByte, _CDCDataWord, _CDCDataLong, _CDCDataSingle, _CDCDataDouble };

	//Zeile eintragen
	void setLine( unsigned long );

	//Zeile zurückliefern
	unsigned long getLine( void ) const;

	//die Größe dieses Bereichs zurückliefern
	virtual unsigned long getSize( void ) const;

	//Problem: Siehe COpcode-Klasse
	virtual _typeId getInternType( void ) const;


private:

	//Zeile im Modul
	unsigned long		_line;
};

#endif
