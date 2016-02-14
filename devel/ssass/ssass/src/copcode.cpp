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
#include "copcode.h"

#include <iostream>

COpcode::COpcode( _opcode op ) : _op( op )
{
	//noch keine Extension
	_ext = _NOEXT;

	//Zeile
	_line = 0;
}


COpcode::~COpcode()
{
}


COpcode &COpcode::operator=( const COpcode &op )
{
	/*
		Assignment-Konstruktor, also
		COpcode a, b;
		a = b;
		ist
		a.operator=( b );
	*/
	this->_ext = op._ext;

std::cout << "Ich werde aufgerufen!" << std::endl;

	//Referenz auf dieses Objekt zurückliefern
	return( *this );
}


void COpcode::setExt( COpcode::_extension ext )
{
	//Extension setzen
	_ext = ext;
}


void COpcode::setLine( unsigned long line )
{
	//Zeile eintragen
	_line = line;
}


unsigned long COpcode::getLine( void ) const
{
	//Zeile zurückliefern
	return( _line );
}


COpcode::_typeId COpcode::getInternType( void ) const
{
	/*
		Problem: Für jeden Opcode wird eine COpcode-Klasse angelegt und in einem
		Array abgelegt (vector<COpcode *>). Wenn später darauf zugegriffen wird,
		dann weiss der Compiler nicht, dass auch CNoOperandOpcode-, COneOperandOpcode-
		und CTwoOperandOpcode-Objekte dabei sein können, da die ja alle von
		COpcode ableiten. Mit einem typeid und/oder einem dynamic_cast ist es
		auch nicht getan. Abhilfe schafft hier eine virtuelle Funktion, die einemID je
		nach Klassentyp zurückliefert.
		Siehe SSLRParser::insertLabel!
	*/
	return( _COPCODE );
}


long COpcode::getAdress( void ) const
{
	//Adresse des Opcodes zurückliefern
	return( _adr );
}


void COpcode::setAdress( long adr )
{
	//Adresse des Opcodes setzen
	_adr = adr;
}


short COpcode::getSize( void ) const
{
	//codierte Größe in Bytes dieses Befehls
	return( 0 );
}



