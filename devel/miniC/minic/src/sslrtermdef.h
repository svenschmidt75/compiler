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


#include "csymboltable.h"

//Terminals returned by the scanner
#define SEMIKOLON	0
#define LESS	1
#define TRUE	2
#define DIV	3
#define LESS_EQU	4
#define EQU	5
#define DO	6
#define BIN_OR	7
#define KLCLOSE	8
#define COMMA	9
#define LEFT_SHIFT	10
#define SHORT	11
#define INT	12
#define IF_STMT	13
#define LOG_NOT	14
#define FALSE	15
#define BOPEN	16
#define BIN_NOT	17
#define LONG	18
#define NUMBER	19
#define MOD	20
#define MUL	21
#define LOG_OR	22
#define BCLOSE	23
#define BOOL	24
#define RIGHT_SHIFT	25
#define WHILE	26
#define NOTEQUAL	27
#define EQUAL	28
#define SUB	29
#define GREATER	30
#define LOG_AND	31
#define ELSE_STMT	32
#define KLOPEN	33
#define BIN_XOR	34
#define BIN_AND	35
#define GREATER_EQU	36
#define IDENT	37
#define ADD	38
#define CHAR	39
#define ACCEPT	40



//Type for semantic actions
typedef union
{

	//Attribut
	int							_a;

	//Fließpunkt-Zahl
	double						_float;

	//Integer-Zahl
	long						_integer;

	//Index für Symboltabellen-Eintrag
	long						_symTabIndex;

	//Index für Node/Leaf im Systaxbaum
	long						_syntaxTreeIndex;

	//den Namen eines Bezeichners
	char						*_name;

	//Typ für eine Variablenvereinbarung
	CSymbolTable::storageType	_type;

}SSLR_TYPE;


#endif

