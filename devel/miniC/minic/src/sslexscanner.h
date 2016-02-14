/***************************************************************************
                          sslexscanner.h  -  description
                             -------------------
    begin                : Mon Feb 24 2003
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

#ifndef SSLEXSCANNER_H
#define SSLEXSCANNER_H


/**
	@author SSLEX V0.5a
	(c) 2002-2005 by Sven Schmidt
	eMail: s.schmidt@lboro.ac.uk
  */


/****************************
* This is the debug-version *
****************************/


//define debug-macro
#define SSLEX_DEBUG
#undef SSLEX_DEBUG



using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "sslrtermdef.h"


//type definition
typedef unsigned long	ulong;
typedef unsigned char	uchar;



//Start-States of the different start conditions
#define INITIAL	0
#define SC_COMMENT	86


//Number of DEA-States
#define	SSLEX_STATE	92

//7- or 8-bit scanner
#define	CCLWIDTH	128

//current start condition
#define	YY_START	baseState
#define	YYSTATE		YY_START


//number of start conditions
#define scSize	4

//do not inizialize user defined variables per default
#undef SSLEX_INIT


//in case of the debug version
#ifdef SSLEX_DEBUG
//size of array with regular expressions
#define regTextSize	50
#endif

class SSLEXScanner
{
public:

//in case of the debug version
#ifdef SSLEX_DEBUG
	//Structure with debugging information
	typedef struct
	{
		//DEA-State
		ulong	state;

		//assocciated regular expressions in this state
		ulong	*index;

		//number of assocciated regular expressions
		ulong	indexCnt;

	}SSLEXDebugStr;
#endif

	//initialize SSLEX
	SSLEXScanner( char *, ulong );
	~SSLEXScanner();

	//Methods for public access

	//start scanning
	int yyLex( SSLR_TYPE *attributeStr = 0L );

	//return line number
	ulong getYYLineno( void );

	//return length of yyText
	ulong getYYLeng( void );

	//return yyText
	char *getYYText( void );

	//reset the whole scanner
	void reset( void );

	//reset the whole scanner and give him new input
	void reset( char *, ulong );


private:

	//Base state of SSLEX, i.e. beginning state of a start condition
	ulong					baseState;

	//current state
	ulong					state;

	//Begin of scanning text
	char					*basePos;

	//length of scanning text
	ulong					basePosLength;

	//current scanning position
	char					*pos;

	//end of scanning text
	char					*end;

	//Position of the beginning of a lexem
	char					*startPos;

	//last accepting position in scanning text
	char					*lastAcceptingPos;

	//last accepting state in scanning text
	long					lastAcceptingState;

	//Table for error handling
	static int				err[scSize][CCLWIDTH];

	//transition table
	static long				ssLexTable[SSLEX_STATE][CCLWIDTH];

	//Accept-Array
	static ulong			ssLexAccept[SSLEX_STATE];

#ifdef SSLEX_DEBUG
	//structure with debugging info
	static SSLEXDebugStr	debugStr[SSLEX_STATE];

	//array with the regular expressions in text form
	static char				*regText[regTextSize];

	//Array with indices in regText
	static ulong			*regTextIndex[SSLEX_STATE];

	//Array with indices in regText
	static ulong	regTextIndex0[];
	static ulong	regTextIndex1[];
	static ulong	regTextIndex2[];
	static ulong	regTextIndex3[];
	static ulong	regTextIndex4[];
	static ulong	regTextIndex5[];
	static ulong	regTextIndex6[];
	static ulong	regTextIndex7[];
	static ulong	regTextIndex8[];
	static ulong	regTextIndex9[];
	static ulong	regTextIndex10[];
	static ulong	regTextIndex11[];
	static ulong	regTextIndex12[];
	static ulong	regTextIndex13[];
	static ulong	regTextIndex14[];
	static ulong	regTextIndex15[];
	static ulong	regTextIndex16[];
	static ulong	regTextIndex17[];
	static ulong	regTextIndex18[];
	static ulong	regTextIndex19[];
	static ulong	regTextIndex20[];
	static ulong	regTextIndex21[];
	static ulong	regTextIndex22[];
	static ulong	regTextIndex23[];
	static ulong	regTextIndex24[];
	static ulong	regTextIndex25[];
	static ulong	regTextIndex26[];
	static ulong	regTextIndex27[];
	static ulong	regTextIndex28[];
	static ulong	regTextIndex29[];
	static ulong	regTextIndex30[];
	static ulong	regTextIndex31[];
	static ulong	regTextIndex32[];
	static ulong	regTextIndex33[];
	static ulong	regTextIndex34[];
	static ulong	regTextIndex35[];
	static ulong	regTextIndex36[];
	static ulong	regTextIndex37[];
	static ulong	regTextIndex38[];
	static ulong	regTextIndex39[];
	static ulong	regTextIndex40[];
	static ulong	regTextIndex41[];
	static ulong	regTextIndex42[];
	static ulong	regTextIndex43[];
	static ulong	regTextIndex44[];
	static ulong	regTextIndex45[];
	static ulong	regTextIndex46[];
	static ulong	regTextIndex47[];
	static ulong	regTextIndex48[];
	static ulong	regTextIndex49[];
	static ulong	regTextIndex50[];
	static ulong	regTextIndex51[];
	static ulong	regTextIndex52[];
	static ulong	regTextIndex53[];
	static ulong	regTextIndex54[];
	static ulong	regTextIndex55[];
	static ulong	regTextIndex56[];
	static ulong	regTextIndex57[];
	static ulong	regTextIndex58[];
	static ulong	regTextIndex59[];
	static ulong	regTextIndex60[];
	static ulong	regTextIndex61[];
	static ulong	regTextIndex62[];
	static ulong	regTextIndex63[];
	static ulong	regTextIndex64[];
	static ulong	regTextIndex65[];
	static ulong	regTextIndex66[];
	static ulong	regTextIndex67[];
	static ulong	regTextIndex68[];
	static ulong	regTextIndex69[];
	static ulong	regTextIndex70[];
	static ulong	regTextIndex71[];
	static ulong	regTextIndex72[];
	static ulong	regTextIndex73[];
	static ulong	regTextIndex74[];
	static ulong	regTextIndex75[];
	static ulong	regTextIndex76[];
	static ulong	regTextIndex77[];
	static ulong	regTextIndex78[];
	static ulong	regTextIndex79[];
	static ulong	regTextIndex80[];
	static ulong	regTextIndex81[];
	static ulong	regTextIndex82[];
	static ulong	regTextIndex83[];
	static ulong	regTextIndex84[];
	static ulong	regTextIndex85[];
	static ulong	regTextIndex86[];
	static ulong	regTextIndex87[];
	static ulong	regTextIndex88[];
	static ulong	regTextIndex89[];
	static ulong	regTextIndex90[];
	static ulong	regTextIndex91[];

	//Array with regular expression at accepting
	static char				*acceptRegText[SSLEX_STATE];
#endif

	//name of the start conditions
	static char				*scName[SSLEX_STATE];

	//state of start conditions
	static ulong			scIndex[];

	//memory for yyText
	ulong					yyTextLen;

	//yyText: holds the accepted lexem
	char					*yyText;

	//length of yyText
	ulong					yyLeng;

	//SSLEX automatically counts line numbers
	ulong					yyLineno;

	//concatenate token
	bool					concatenate;

	//size of start condition stack
	ulong					scStackSize;

	//numer of start xonditions on stack
	long					scStackPos;

	//start condition stack
	ulong					*scStack;


private:
//werden unverändert ins Header-File des erzeugten Scanners übernommen
//Kommantar scannen
bool		cComment;
//Methoden
//eigene Variablen initialisieren
void init( void );
//eigene Variablen deinitialisieren
void deinit( void );
public:


private:

	//insert text in yytext
	void setupyyText( void );

	//setup yyLineno
	void setupYYLineno( char *, char * );

	//setup state for begin-of-line-rules
	ulong setupState( ulong );

	//set a new start condition
	void setStartCondition( ulong );

	//return txt in it's own memory
	char *yyStrCpy( char *txt );

	//returns all but the first n characters of yyText back to the input
	void yyLess( int );

	//enlarge start condition stack
	void enlargeSCStack( void );

	/*
		Pushes the current start condition onto the top of the start condition stack
		and switches to newState as though you have used BEGIN( newState ).
	*/
	void yyPushState( ulong );

	//pops the top of the start condition stack and switches to it
	void yyPopState( void );

	//returns the top of the start condition stack without altering its contents
	ulong yyTopState( void );

	//puts the character c back to the input stream
	void yyUnput( unsigned char );
	void yyUnput( char * );

	//return the next character to be scanned
	char yyInput( void );

	//call eof-rules
	int endOfLine( void );

	//handle simple errors
	void error( uchar c );

	//set some stuff before a new rule is scanned
	void yyDoBeforeRule( void );

#ifdef SSLEX_DEBUG
	//print debugging information
	void mainDebug( unsigned char );

	//print debugging information
	void shiftDebug( unsigned char );

	//print debugging information
	void acceptDebug( void );

	//copy cnt bytes from from to to, but transform escape sequences into
	//readable text form
	void txtFilter( char *, char *, ulong );
#endif
};

#endif
