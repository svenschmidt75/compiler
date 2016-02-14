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
#define CCE_GRAPH	0
#define PLUS	1
#define DEF	2
#define SOPEN	3
#define LA	4
#define OR	5
#define NAME	6
#define KLCLOSE	7
#define SPEC	8
#define CCE_CNTRL	9
#define XSCDECL	10
#define SCALL	11
#define CCE_UPPER	12
#define COPEN	13
#define CCE_XDIGIT	14
#define CCLOSE	15
#define BOPEN	16
#define MINUS	17
#define ENDLINE	18
#define NUMBER	19
#define MUL	20
#define EOF_OP	21
#define CCE_DIGIT	22
#define CCE_ALPHA	23
#define NEGATE	24
#define KOMMA	25
#define BCLOSE	26
#define SCOPEN	27
#define SCDECL	28
#define CCE_ALNUM	29
#define CCE_PUNCT	30
#define NEWLINE	31
#define CCE_PRINT	32
#define CCE_SPACE	33
#define CCE_LOWER	34
#define BEGLINE	35
#define CCE_BLANK	36
#define KLOPEN	37
#define EPSILON	38
#define QUEST	39
#define PREVCCL	40
#define ANY	41
#define CHAR	42
#define SCCLOSE	43
#define ACCEPT	44



//Type for semantic actions
typedef union
{

	//Strings
	char	*name;

	int		i;

	//Charakter-Klassen
	int		*cclClass;

	//simples Zeichen
	char	character;

	//long
	long	l;

	//NEA_STR
	NEA_STR	*neaStr;

}SSLR_TYPE;


#endif

