/***************************************************************************
                          sslrtermdef.h  -  description
                             -------------------
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SSLRTERMDEF_H
#define SSLRTERMDEF_H


/**
	@author SSLR V0.5a
	(c) 2002-2005 by Sven Schmidt
	eMail: s.schmidt@lboro.ac.uk
  */


//Terminals returned by the scanner
#define SEMIKOLON	0
#define DIV	1
#define KLCLOSE	2
#define UMINUS	3
#define ZAHL	4
#define MUL	5
#define SUB	6
#define KLOPEN	7
#define ADD	8
#define ACCEPT	9



//Type for semantic actions
typedef union
{

	//Zahlen sind vom Typ float
	float	f;

}SSLR_TYPE;


#endif

