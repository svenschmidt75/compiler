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


/********************************
* This is the non-debug-version *
********************************/



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


//Number of DEA-States
#define	SSLEX_STATE	17

//7- or 8-bit scanner
#define	CCLWIDTH	128

//current start condition
#define	YY_START	baseState
#define	YYSTATE		YY_START


//number of start conditions
#define scSize	2

//do not inizialize user defined variables per default
#undef SSLEX_INIT



class SSLEXScanner
{
public:

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
};

#endif
