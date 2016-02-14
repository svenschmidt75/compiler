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
#define AST	0
#define UNION	1
#define SEMIKOLON	2
#define SHIFT	3
#define EXIT	4
#define STRING	5
#define TERMINAL	6
#define DB	7
#define EOPEN	8
#define LEFT_ASSOC	9
#define KLCLOSE	10
#define COMMA	11
#define PREC	12
#define END_OF_PRODUCTION	13
#define NON_TERMINAL	14
#define ERROR	15
#define TYPE	16
#define BOPEN	17
#define NUMBER	18
#define SEMACT	19
#define NOT	20
#define NEW_ALTERNATIVE	21
#define DELETE	22
#define RIGHT_ASSOC	23
#define POP	24
#define BCLOSE	25
#define semTypeOpen	26
#define OVERREAD	27
#define SEM	28
#define SEP	29
#define NON_ASSOC	30
#define NTERM_TYPE	31
#define DEFINITION	32
#define KLOPEN	33
#define semTypeClose	34
#define TERM_TYPE	35
#define EPSILON	36
#define ECLOSE	37
#define IDENT	38
#define GRAMMAR	39
#define ACCEPT	40



//Type for semantic actions
typedef union
{

	//Strings
	char		*name;

	//einfache Zahl
	int			integer;

}SSLR_TYPE;


#endif

