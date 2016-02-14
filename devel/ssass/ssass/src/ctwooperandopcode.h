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
#ifndef CTWOOPERANDOPCODE_H
#define CTWOOPERANDOPCODE_H

#include <copcode.h>
#include <iostream>

#include "cadressingmode.h"

/**
@author Sven Schmidt
*/
class CTwoOperandOpcode : public COpcode
{
public:
	CTwoOperandOpcode( _opcode );
	virtual ~CTwoOperandOpcode();

	//damit << auf die privaten Members zugreifen darf
	friend std::ostream &operator<<( std::ostream &, const CTwoOperandOpcode & );

	//Adressierunsart Source
	void setSource( CAdressingMode * );

	//Adressierunsart Destination
	void setDest( CAdressingMode * );

	//Adressierungsart Source zurückliefern
	CAdressingMode::_adrMode getAdrModeS( void ) const;

	//Adressierungsart Destination zurückliefern
	CAdressingMode::_adrMode getAdrModeD( void ) const;

	//wer bin ich
	virtual _typeId getInternType( void ) const;

	/*
		Die Adresse eines Labels eintragen, wenn das Displacement
		als Label angegeben wurde.
	*/
	void setDispAdress( void ) const;

	/*
		Prüft, ob die Adressierungsart ein Label benutzt, dass
		als .import deklariert wurde.
	*/
	const char *isSLabelExtern( void ) const;

	/*
		Prüft, ob die Adressierungsart ein Label benutzt, dass
		als .import deklariert wurde.
	*/
	const char *isDLabelExtern( void ) const;

	//codierte Größe in Bytes dieses Befehls
	virtual short getSize( void ) const;

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
	short getLabelSOffset( void ) const;
	short getLabelDOffset( void ) const;


private:

	/*
		Für die Offsetberechnung ist es am Einfachsten, wenn die Länge der
		assemblierten Befehle im Objektfile vorher bekannt ist.
	*/
	const static int		_width = sizeof( int ) + 4 * sizeof( short ) + 2 * sizeof( long );

	//Adressierunsart Source
	CAdressingMode			*_adrModeS;

	//Adressierunsart Destination
	CAdressingMode			*_adrModeD;
};

#endif
