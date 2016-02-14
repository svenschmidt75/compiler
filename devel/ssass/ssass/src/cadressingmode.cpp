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
#include "cadressingmode.h"

CAdressingMode::CAdressingMode( _adrMode _am ) : _am( _am )
{
	//initialisieren
	_sr = false;
	_pc = false;
	_nDisp = 0;
	_label = 0L;
	_setAdress = false;
}


CAdressingMode::~CAdressingMode()
{
}


void CAdressingMode::setDataRegister( unsigned short dr )
{
	//Datenregister eintragen
	_dataReg = dr;
}


void CAdressingMode::setAdressRegister( unsigned short ar )
{
	//Adressregister eintragen
	_adrReg = ar;
}


void CAdressingMode::setDisplacement( long disp )
{
	//numerisches Displacement eintragen
	_nDisp = disp;
}


void CAdressingMode::setDisplacement( CLabel *label )
{
	//Displacement wurde als Label angegeben
	_label = label;

	//Adresse muss noch berechnet werden
	_setAdress = true;
}


void CAdressingMode::setPC( void )
{
	//Programm Counter ist gemeint
	_pc = true;
}


void CAdressingMode::setSR( void )
{
	//Status Register ist gemeint
	_sr = true;
}


CAdressingMode::_adrMode CAdressingMode::getAdrMode( void ) const
{
	//Adressierungsart zurückliefern
	return( _am );
}


void CAdressingMode::checkDisp( long adr )
{
	/*
		Wenn als Displacement ein Label angegeben wurde,
		dann nun den Offset berechnen. adr ist die Adresse,
		an die der zugehörige Opcode steht.
	*/

	//überhaupt ein Displacement als Label angegeben?
	if( _label && ( _setAdress == true ) )
	{
		//ja, Adresse holen
		long	la = _label->getAdress();

		//Offset bilden
		_nDisp = la - adr;
	}
}


const char *CAdressingMode::isLabelExtern( void ) const
{
	/*
		Wenn als Displacement ein Label angegeben wurde,
		dann prüfen, ob es als .import deklariert wurde
	*/

	//überhaupt ein Displacement als Label angegeben?
	if( _label && ( _label->isImport() == true ) )
		//ja
		return( _label->getName() );

	//Label nicht .import, oder gar kein Label
	return( 0L );
}


short CAdressingMode::getSize( void ) const
{
	/*
		Für die Berechnung der Adressen der Befehle muss man wissen, wie viele
		Bytes die codierte Adressierungsart beträgt.
	*/
	short	size;

	//je nach Adressierungsart
	switch( _am )
	{
		case CAdressingMode::_DRD:
		case CAdressingMode::_ARD:
		case CAdressingMode::_ARI:
		case CAdressingMode::_ARIPOST:
		case CAdressingMode::_ARIPRE:
		{
			//nur ein short
			size = sizeof( _adrReg );
			break;
		}

		case CAdressingMode::_SR:
		{
			//Status Register
			size = 0;
			break;
		}

		case CAdressingMode::_ARID:
		{
			//Adressregister + Displacement
			size = sizeof( _adrReg ) + sizeof( _nDisp );
			break;
		}

		case CAdressingMode::_PCID:
		case CAdressingMode::_AA:
		case CAdressingMode::_ID:
		{
			//nur Displacement
			size = sizeof( _nDisp );
			break;
		}

		default:
		{
			//Fehler, unbekannte Adressierungsart
			std::cerr << "ssass: CAdressingMode::getSize: Unknown adressing mode!" << std::endl;

			//Ende
			exit( 1 );
		}
	}

	//für die Codierung der Adressierungsart
	size += sizeof( _am );

	//Größe in Bytes zurückliefern
	return( size );
}


short CAdressingMode::getLabelOffset( void ) const
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
	unsigned short	_offset = 0;

	//Offset ist abhängig von der Adressierungsart
	switch( _am )
	{
		case CAdressingMode::_ARID:
		{
			//Adress Register Indirect with Displacement
			_offset = sizeof( CAdressingMode::_adrMode ) + sizeof( unsigned short );
			break;
		}

		case CAdressingMode::_PCID:
		case CAdressingMode::_AA:
		case CAdressingMode::_ID:
		{
			//hier ist der Offset +2
			_offset = sizeof( unsigned short );
			break;
		}

		default:
		{
			//hier darf nicht hingesprungen werden
			std::cerr << "ssass: CAdressingMode::getLabelOffset: Adressing mode not listed!" << std::endl;
			break;
		}
	}

	//Offset zurückliefern
	return( _offset );
}


std::ostream &operator<<( std::ostream &os, const CAdressingMode &cls )
{
	//Ausgabe der Adressierungsart

	//je nach Adressierungsart
	switch( cls._am )
	{
		//Adressierunsart
		CAdressingMode::_adrMode	_am;

		case CAdressingMode::_DRD:
		{
			//Data Register Direct
			_am = CAdressingMode::_DRD;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Nummer des Datenregisters ausgeben
			os.write( ( char * ) &cls._dataReg, sizeof( cls._dataReg ) );
			break;
		}

		case CAdressingMode::_SR:
		{
			//Status Register
			_am = CAdressingMode::_SR;
			os.write( ( char * ) &_am, sizeof( _am ) );
			break;
		}

		case CAdressingMode::_ARD:
		{
			//Adress Register Direct
			_am = CAdressingMode::_ARD;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Nummer des Adressregisters ausgeben
			os.write( ( char * ) &cls._adrReg, sizeof( cls._adrReg ) );
			break;
		}

		case CAdressingMode::_ARI:
		{
			//Adress Register Indirect
			_am = CAdressingMode::_ARI;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Nummer des Adressregisters ausgeben
			os.write( ( char * ) &cls._adrReg, sizeof( cls._adrReg ) );
			break;
		}

		case CAdressingMode::_ARIPOST:
		{
			//Adress Register Indirect with Postincrement
			_am = CAdressingMode::_ARIPOST;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Nummer des Adressregisters ausgeben
			os.write( ( char * ) &cls._adrReg, sizeof( cls._adrReg ) );
			break;
		}

		case CAdressingMode::_ARIPRE:
		{
			//Adress Register Indirect with Predecrement
			_am = CAdressingMode::_ARIPRE;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Nummer des Adressregisters ausgeben
			os.write( ( char * ) &cls._adrReg, sizeof( cls._adrReg ) );
			break;
		}

		case CAdressingMode::_ARID:
		{
			//Adress Register Indirect with Displacement
			_am = CAdressingMode::_ARID;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Nummer des Adressregisters ausgeben
			os.write( ( char * ) &cls._adrReg, sizeof( cls._adrReg ) );

			//Displacement ausgeben
			os.write( ( char * ) &cls._nDisp, sizeof( cls._nDisp ) );
			break;
		}

		case CAdressingMode::_PCID:
		{
			//Programm Counter Indirect with Displacement
			_am = CAdressingMode::_PCID;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Displacement ausgeben
			os.write( ( char * ) &cls._nDisp, sizeof( cls._nDisp ) );
			break;
		}

		case CAdressingMode::_AA:
		{
			//Absolute Adressing
			_am = CAdressingMode::_AA;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Adresse ausgeben
			os.write( ( char * ) &cls._nDisp, sizeof( cls._nDisp ) );
			break;
		}

		case CAdressingMode::_ID:
		{
			//Immediate Data
			_am = CAdressingMode::_ID;
			os.write( ( char * ) &_am, sizeof( _am ) );

			//Immediate Data ausgeben
			os.write( ( char * ) &cls._nDisp, sizeof( cls._nDisp ) );
			break;
		}

		default:
		{
			//Fehler, unbekannte Adressierungsart
			std::cerr << "ssass: std::ostream &operator<<: Unknown adressing mode!" << std::endl;

			//Ende
			exit( 1 );
		}
	}

std::cout << "CAdressingMode" << std::endl;

	//Referenz auf den ostream zurückliefern
	return( os );
}

