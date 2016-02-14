/***************************************************************************
                          sslex.h  -  description
                             -------------------
    begin                : Don Feb 6 2003
    copyright            : (C) 2003 by Sven Schmidt
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

#ifndef SSLEX_H
#define SSLEX_H


//Debug-Version
#define DEBUG
#undef DEBUG




//7- oder 8-Bit-Scanner
#define _7BIT	128
#define _8BIT	256




//Terminal-Defines
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



//Übergabe-Argumente, um den Parser zu steuern
typedef struct
{
	//Ausführliche Meldungen
	bool		verbose;

	//Debug-Modus
	bool		debug;

	//7- oder 8-Bit-Scanner
	int			cclWidth;

	//Zeige Konflikte zwischen den regulären Ausdrücken
	bool		showConflicts;

}ARGV_STR;



//Struktur für NEA
typedef struct
{
	//Start-Zustand
	ulong	startState;

	//Finit-Zustand
	ulong	endState;

	//ist ein Lookahead-NEA oder nicht
	bool	laState;

	//ist ein <<EOF>-NEA
	bool	eof;

	//NEA darf nur am Anfang einer Zeile stehen
	bool	begLine;

	//Aktion, verbunden mit einer Regel
	char	**action;

	//Anzahl der Aktionen
	int		actionAnz;

	//regulärer Ausdruck in Text-Form
	char	*regText;

	//Zeile, in der der reg. Ausdruck definiert wurde
	ulong	line;

}NEA_STR;


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



//Struktur mit den regulären Ausdrücken, die über ihren Namen, {NAME}, angesprochen werden
typedef struct
{
	//Name des regulären Ausdrucks
	char	*name;

	//regulärer Ausdruck als Text
	char	*text;

	//Zeile, in der die Definition steht
	long	line;

	//gibt an, ob der reg. Ausdruck überhaupt benutzt wurde
	bool	used;

}NAME_NEA;

#endif

