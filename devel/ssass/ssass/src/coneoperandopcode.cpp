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
#include "coneoperandopcode.h"

COneOperandOpcode::COneOperandOpcode( _opcode op ) : COpcode( op )
{
	//initialisieren
	_adrMode = 0L;
}


COneOperandOpcode::~COneOperandOpcode()
{
}


void COneOperandOpcode::setSource( CAdressingMode *adrM )
{
	//Adressierunsart setzen
	_adrMode = adrM;
}


CAdressingMode::_adrMode COneOperandOpcode::getAdrMode( void ) const
{
	//Adressierungsart zurückliefern
	return( _adrMode->getAdrMode() );
}


COpcode::_typeId COneOperandOpcode::getInternType( void ) const
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
	return( _CONE );
}


void COneOperandOpcode::setDispAdress( void ) const
{
	/*
		Die Adresse eines Labels eintragen, wenn das Displacement
		als Label angegeben wurde.
	*/
	_adrMode->checkDisp( _adr );
}


const char *COneOperandOpcode::isLabelExtern( void ) const
{
	/*
		Prüft, ob die Adressierungsart ein Label benutzt, dass
		als .import deklariert wurde.
	*/
	return( _adrMode->isLabelExtern() );
}


short COneOperandOpcode::getSize( void ) const
{
	//codierte Größe in Bytes dieses Befehls
	short	size = _adrMode->getSize() + sizeof( int );
	return( size );
}


short COneOperandOpcode::getLabelOffset( void ) const
{
	/*
		Wird auf Labels zugegriffen, die als .import deklariert wurden,
		so muss der Linker später die richtigen Adressen eintragen.
		Dazu genügt es, ihm zu sagen, an welcher Adresse der zugreifende Befehl liegt
		plus Offset zur Ablage des Labels.
		Also:

		0x00: short: Befehlsnummer
		0x02: short: Adressierungsart
		0x04:  long: Offset
		0x08:
		Hier wird also als Offset sizeof( short ) zurückgeliefert, da die Adresse des
		Labels immer am Offset 2 bzgl. der Adressierungsart liegt.
	*/
	return( _adrMode->getLabelOffset() );
}


std::ostream &operator<<( std::ostream &os, const COneOperandOpcode &cls )
{
	//Ausgabe eines Opcodes
	os.write( ( char * ) &cls._op, sizeof( cls._op ) );

	//Adressierungsart ausgeben
	os << *(cls._adrMode);

std::cout << "One" << std::endl;

	//Referenz auf den ostream zurückliefern
	return( os );
}

