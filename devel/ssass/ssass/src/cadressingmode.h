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
#ifndef CADRESSINGMODE_H
#define CADRESSINGMODE_H


//warum hier nicht #include "clabel.h"?
//class CLabel;
#include "clabel.h"

/**
@author Sven Schmidt
*/
class CAdressingMode
{
public:

	//Adressierungart
	enum _adrMode{ _DRD, _ARD, _SR, _ARI, _ARIPOST, _ARIPRE, _ARID, _PCID, _AA, _ID };

	//damit << auf die privaten Members zugreifen darf
	friend std::ostream &operator<<( std::ostream &, const CAdressingMode & );

	//Kon- und Destruktor
	CAdressingMode( _adrMode );
	~CAdressingMode();

	//Datenregister eintragen
	void setDataRegister( unsigned short );

	//Adressregister eintragen
	void setAdressRegister( unsigned short );

	//numerisches Displacement eintragen, auch Immediate Data
	void setDisplacement( long );

	//Displacement wurde als Label angegeben
	void setDisplacement( CLabel * );

	//Programm Counter ist gemeint
	void setPC( void );

	//Status Register ist gemeint
	void setSR( void );

	//Adressierungsart zurückliefern
	_adrMode getAdrMode( void ) const;

	/*
		Wenn als Displacement ein Label angegeben wurde,
		dann nun den Offset berechnen. adr ist die Adresse,
		an die der zugehörige Opcode steht.
	*/
	void checkDisp( long adr );

	/*
		Wenn als Displacement ein Label angegeben wurde,
		dann prüfen, ob es als .import deklariert wurde
	*/
	const char *isLabelExtern( void ) const;

	/*
		Für die Berechnung der Adressen der Befehle muss man wissen, wie viele
		Bytes die codierte Adressierungsart benötigt.
	*/
	short getSize( void ) const;

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
	short getLabelOffset( void ) const;


private:

	//Adressierungsart
	const _adrMode		_am;

	//Datenregister
	unsigned short		_dataReg;

	//Adressregister
	unsigned short		_adrReg;

	//wenn 12(a0), dann hier displacement
	long				_nDisp;

	//Displacement wurde als Label angegeben
	CLabel				*_label;

	/*
		Wenn ein Label als Displacement angegeben wurde, dann muss später,
		wenn die Adressen der Labels bekannt sind, noch der Offset in den
		Opcode eingetragen werden.
	*/
	bool				_setAdress;

	//true, wenn Status Register
	bool				_sr;

	//true, wenn Programm Counter
	bool				_pc;
};

#endif
