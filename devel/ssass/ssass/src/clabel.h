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
#ifndef CLABEL_H
#define CLABEL_H

#include <vector>

#include "sslexscanner.h"

/**
@author Sven Schmidt
*/
class CLabel
{
public:

	//Kon- und Destructor
	CLabel();
	~CLabel();

	//den Namen eines Labels eintragen
	void setName( char * );

	//das Label wird in einem anderen Moduls deklariert
	void setImport( void );

	//das Label wird auch in anderen Modulen zugänglich gemacht
	void setExport( void );

	//das Label ist nur in diesem Modulen zugänglich
	void setLocal( void );

	//prüft, ob das Label exportiert werden soll
	bool isExport( void );

	//prüft, ob das Label importiert werden muss
	bool isImport( void );

	//prüft, ob das Label nur im aktuellen Modul gültig ist
	bool isLocal( void );

	//liefert den Namen des Labels zurück
	const char *getName( void );

	//Adresse des Labels eintragen
	void setAdress( long );

	//Adresse des Labels erfragen
	long getAdress( void );

	//Label wurde definiert
	void setDefined( void );

	//prüft, ob das Label definiert wurde
	bool getDefined( void );

	//dieses Label wird benutzt
	void setUsed( void );

	//prüft, ob dieses Label benutzt wird
	bool isUsed( void ) const;

	//Zeile eintragen, in der das Label definiert wurde
	void setLine( unsigned long );

	//Zeile, in der das Label definiert wurde, zurückgeben
	unsigned long getLine( void ) const;


private:

	//Adresse des Labels
	long				_adr;

	//Name des Labels
	char				*_name;

	//Zeile, in der ein Label definiert wurde
	unsigned long		_line;

	//true: Label in anderen Modulen zugänglich machen
	bool				_export;

	//true: Label ist in einem anderen Modul definiert
	bool				_import;

	//true: Label ist nur in dem aktuellen Modul erlaubt
	bool				_local;

	//true: Label wurde definiert
	bool				_setDefined;

	//Label ist .local und wird benutzt, wenn true
	bool				_isUsed;
};

#endif
