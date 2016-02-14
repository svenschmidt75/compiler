/***************************************************************************
                          sslr.h  -  description
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


#ifndef SSLR_H
#define SSLR_H


//in config.h steht die Version
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//Terminal oder Nicht-Terminal
#define SSLR_TERM 1
#define SSLR_NTERM 2



//Größe der Hash-Tabelle
#define HASH_SIZE 6000


//Parser-Typ
#define SLR		0
#define LALR	1
#define	LR		2



//Übergabe-Argumente, um den Parser zu steuern
typedef struct
{
	//Ausführliche Meldungen
	bool		verbose;

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
