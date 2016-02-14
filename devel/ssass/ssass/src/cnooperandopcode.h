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
#ifndef CNOOPERANDOPCODE_H
#define CNOOPERANDOPCODE_H

#include <copcode.h>
#include <iostream>

/**
@author Sven Schmidt
*/
class CNoOperandOpcode : public COpcode
{
public:
	CNoOperandOpcode( _opcode );
	virtual ~CNoOperandOpcode();

	//damit << auf die privaten Members zugreifen darf
	friend std::ostream &operator<<( std::ostream &, const CNoOperandOpcode & );

	//wer bin ich
	virtual _typeId getInternType( void ) const;

	//codierte Größe in Bytes dieses Befehls
	virtual short getSize( void ) const;

private:

	/*
		Für die Offsetberechnung ist es am Einfachsten, wenn die Länge der
		assemblierten Befehle im Objektfile vorher bekannt ist.
	*/
	const static int	_width = sizeof( int );
};

#endif
