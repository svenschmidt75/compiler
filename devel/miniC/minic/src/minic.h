/***************************************************************************
                          minic.h  -  description
                             -------------------
    begin                : Fre Aug 16 2002
    copyright            : (C) 2002 by Sven Schmidt
    email                : s.schmidt@lboro.ac.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef MINIC_H
#define MINIC_H


//in config.h steht die Version
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif




//Übergabe-Argumente, um den Parser zu steuern
typedef struct
{
	//Ausführlichkeit der Meldungen
	int			verbose;

	//Debug-Modus
	bool		debug;

	//Grammatik-Datei als File ausgeben
	bool		outputGrammar;

	//Dateiname
	char		*name;

	//LR-Parser-Typ: SLR,LALR,LR
	int			type;

	//FIRST-Mengen ausgeben
	bool		showFirst;

	//FOLLOW-Mengen ausgeben
	bool		showFollow;

	//Hüllen zeigen
	bool		showHulle;

}ARGV_STR;



#endif
