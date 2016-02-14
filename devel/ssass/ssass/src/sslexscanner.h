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
#define SC_COMMENT	373


//Number of DEA-States
#define	SSLEX_STATE	379

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
#define regTextSize	103
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
	static ulong	regTextIndex92[];
	static ulong	regTextIndex93[];
	static ulong	regTextIndex94[];
	static ulong	regTextIndex95[];
	static ulong	regTextIndex96[];
	static ulong	regTextIndex97[];
	static ulong	regTextIndex98[];
	static ulong	regTextIndex99[];
	static ulong	regTextIndex100[];
	static ulong	regTextIndex101[];
	static ulong	regTextIndex102[];
	static ulong	regTextIndex103[];
	static ulong	regTextIndex104[];
	static ulong	regTextIndex105[];
	static ulong	regTextIndex106[];
	static ulong	regTextIndex107[];
	static ulong	regTextIndex108[];
	static ulong	regTextIndex109[];
	static ulong	regTextIndex110[];
	static ulong	regTextIndex111[];
	static ulong	regTextIndex112[];
	static ulong	regTextIndex113[];
	static ulong	regTextIndex114[];
	static ulong	regTextIndex115[];
	static ulong	regTextIndex116[];
	static ulong	regTextIndex117[];
	static ulong	regTextIndex118[];
	static ulong	regTextIndex119[];
	static ulong	regTextIndex120[];
	static ulong	regTextIndex121[];
	static ulong	regTextIndex122[];
	static ulong	regTextIndex123[];
	static ulong	regTextIndex124[];
	static ulong	regTextIndex125[];
	static ulong	regTextIndex126[];
	static ulong	regTextIndex127[];
	static ulong	regTextIndex128[];
	static ulong	regTextIndex129[];
	static ulong	regTextIndex130[];
	static ulong	regTextIndex131[];
	static ulong	regTextIndex132[];
	static ulong	regTextIndex133[];
	static ulong	regTextIndex134[];
	static ulong	regTextIndex135[];
	static ulong	regTextIndex136[];
	static ulong	regTextIndex137[];
	static ulong	regTextIndex138[];
	static ulong	regTextIndex139[];
	static ulong	regTextIndex140[];
	static ulong	regTextIndex141[];
	static ulong	regTextIndex142[];
	static ulong	regTextIndex143[];
	static ulong	regTextIndex144[];
	static ulong	regTextIndex145[];
	static ulong	regTextIndex146[];
	static ulong	regTextIndex147[];
	static ulong	regTextIndex148[];
	static ulong	regTextIndex149[];
	static ulong	regTextIndex150[];
	static ulong	regTextIndex151[];
	static ulong	regTextIndex152[];
	static ulong	regTextIndex153[];
	static ulong	regTextIndex154[];
	static ulong	regTextIndex155[];
	static ulong	regTextIndex156[];
	static ulong	regTextIndex157[];
	static ulong	regTextIndex158[];
	static ulong	regTextIndex159[];
	static ulong	regTextIndex160[];
	static ulong	regTextIndex161[];
	static ulong	regTextIndex162[];
	static ulong	regTextIndex163[];
	static ulong	regTextIndex164[];
	static ulong	regTextIndex165[];
	static ulong	regTextIndex166[];
	static ulong	regTextIndex167[];
	static ulong	regTextIndex168[];
	static ulong	regTextIndex169[];
	static ulong	regTextIndex170[];
	static ulong	regTextIndex171[];
	static ulong	regTextIndex172[];
	static ulong	regTextIndex173[];
	static ulong	regTextIndex174[];
	static ulong	regTextIndex175[];
	static ulong	regTextIndex176[];
	static ulong	regTextIndex177[];
	static ulong	regTextIndex178[];
	static ulong	regTextIndex179[];
	static ulong	regTextIndex180[];
	static ulong	regTextIndex181[];
	static ulong	regTextIndex182[];
	static ulong	regTextIndex183[];
	static ulong	regTextIndex184[];
	static ulong	regTextIndex185[];
	static ulong	regTextIndex186[];
	static ulong	regTextIndex187[];
	static ulong	regTextIndex188[];
	static ulong	regTextIndex189[];
	static ulong	regTextIndex190[];
	static ulong	regTextIndex191[];
	static ulong	regTextIndex192[];
	static ulong	regTextIndex193[];
	static ulong	regTextIndex194[];
	static ulong	regTextIndex195[];
	static ulong	regTextIndex196[];
	static ulong	regTextIndex197[];
	static ulong	regTextIndex198[];
	static ulong	regTextIndex199[];
	static ulong	regTextIndex200[];
	static ulong	regTextIndex201[];
	static ulong	regTextIndex202[];
	static ulong	regTextIndex203[];
	static ulong	regTextIndex204[];
	static ulong	regTextIndex205[];
	static ulong	regTextIndex206[];
	static ulong	regTextIndex207[];
	static ulong	regTextIndex208[];
	static ulong	regTextIndex209[];
	static ulong	regTextIndex210[];
	static ulong	regTextIndex211[];
	static ulong	regTextIndex212[];
	static ulong	regTextIndex213[];
	static ulong	regTextIndex214[];
	static ulong	regTextIndex215[];
	static ulong	regTextIndex216[];
	static ulong	regTextIndex217[];
	static ulong	regTextIndex218[];
	static ulong	regTextIndex219[];
	static ulong	regTextIndex220[];
	static ulong	regTextIndex221[];
	static ulong	regTextIndex222[];
	static ulong	regTextIndex223[];
	static ulong	regTextIndex224[];
	static ulong	regTextIndex225[];
	static ulong	regTextIndex226[];
	static ulong	regTextIndex227[];
	static ulong	regTextIndex228[];
	static ulong	regTextIndex229[];
	static ulong	regTextIndex230[];
	static ulong	regTextIndex231[];
	static ulong	regTextIndex232[];
	static ulong	regTextIndex233[];
	static ulong	regTextIndex234[];
	static ulong	regTextIndex235[];
	static ulong	regTextIndex236[];
	static ulong	regTextIndex237[];
	static ulong	regTextIndex238[];
	static ulong	regTextIndex239[];
	static ulong	regTextIndex240[];
	static ulong	regTextIndex241[];
	static ulong	regTextIndex242[];
	static ulong	regTextIndex243[];
	static ulong	regTextIndex244[];
	static ulong	regTextIndex245[];
	static ulong	regTextIndex246[];
	static ulong	regTextIndex247[];
	static ulong	regTextIndex248[];
	static ulong	regTextIndex249[];
	static ulong	regTextIndex250[];
	static ulong	regTextIndex251[];
	static ulong	regTextIndex252[];
	static ulong	regTextIndex253[];
	static ulong	regTextIndex254[];
	static ulong	regTextIndex255[];
	static ulong	regTextIndex256[];
	static ulong	regTextIndex257[];
	static ulong	regTextIndex258[];
	static ulong	regTextIndex259[];
	static ulong	regTextIndex260[];
	static ulong	regTextIndex261[];
	static ulong	regTextIndex262[];
	static ulong	regTextIndex263[];
	static ulong	regTextIndex264[];
	static ulong	regTextIndex265[];
	static ulong	regTextIndex266[];
	static ulong	regTextIndex267[];
	static ulong	regTextIndex268[];
	static ulong	regTextIndex269[];
	static ulong	regTextIndex270[];
	static ulong	regTextIndex271[];
	static ulong	regTextIndex272[];
	static ulong	regTextIndex273[];
	static ulong	regTextIndex274[];
	static ulong	regTextIndex275[];
	static ulong	regTextIndex276[];
	static ulong	regTextIndex277[];
	static ulong	regTextIndex278[];
	static ulong	regTextIndex279[];
	static ulong	regTextIndex280[];
	static ulong	regTextIndex281[];
	static ulong	regTextIndex282[];
	static ulong	regTextIndex283[];
	static ulong	regTextIndex284[];
	static ulong	regTextIndex285[];
	static ulong	regTextIndex286[];
	static ulong	regTextIndex287[];
	static ulong	regTextIndex288[];
	static ulong	regTextIndex289[];
	static ulong	regTextIndex290[];
	static ulong	regTextIndex291[];
	static ulong	regTextIndex292[];
	static ulong	regTextIndex293[];
	static ulong	regTextIndex294[];
	static ulong	regTextIndex295[];
	static ulong	regTextIndex296[];
	static ulong	regTextIndex297[];
	static ulong	regTextIndex298[];
	static ulong	regTextIndex299[];
	static ulong	regTextIndex300[];
	static ulong	regTextIndex301[];
	static ulong	regTextIndex302[];
	static ulong	regTextIndex303[];
	static ulong	regTextIndex304[];
	static ulong	regTextIndex305[];
	static ulong	regTextIndex306[];
	static ulong	regTextIndex307[];
	static ulong	regTextIndex308[];
	static ulong	regTextIndex309[];
	static ulong	regTextIndex310[];
	static ulong	regTextIndex311[];
	static ulong	regTextIndex312[];
	static ulong	regTextIndex313[];
	static ulong	regTextIndex314[];
	static ulong	regTextIndex315[];
	static ulong	regTextIndex316[];
	static ulong	regTextIndex317[];
	static ulong	regTextIndex318[];
	static ulong	regTextIndex319[];
	static ulong	regTextIndex320[];
	static ulong	regTextIndex321[];
	static ulong	regTextIndex322[];
	static ulong	regTextIndex323[];
	static ulong	regTextIndex324[];
	static ulong	regTextIndex325[];
	static ulong	regTextIndex326[];
	static ulong	regTextIndex327[];
	static ulong	regTextIndex328[];
	static ulong	regTextIndex329[];
	static ulong	regTextIndex330[];
	static ulong	regTextIndex331[];
	static ulong	regTextIndex332[];
	static ulong	regTextIndex333[];
	static ulong	regTextIndex334[];
	static ulong	regTextIndex335[];
	static ulong	regTextIndex336[];
	static ulong	regTextIndex337[];
	static ulong	regTextIndex338[];
	static ulong	regTextIndex339[];
	static ulong	regTextIndex340[];
	static ulong	regTextIndex341[];
	static ulong	regTextIndex342[];
	static ulong	regTextIndex343[];
	static ulong	regTextIndex344[];
	static ulong	regTextIndex345[];
	static ulong	regTextIndex346[];
	static ulong	regTextIndex347[];
	static ulong	regTextIndex348[];
	static ulong	regTextIndex349[];
	static ulong	regTextIndex350[];
	static ulong	regTextIndex351[];
	static ulong	regTextIndex352[];
	static ulong	regTextIndex353[];
	static ulong	regTextIndex354[];
	static ulong	regTextIndex355[];
	static ulong	regTextIndex356[];
	static ulong	regTextIndex357[];
	static ulong	regTextIndex358[];
	static ulong	regTextIndex359[];
	static ulong	regTextIndex360[];
	static ulong	regTextIndex361[];
	static ulong	regTextIndex362[];
	static ulong	regTextIndex363[];
	static ulong	regTextIndex364[];
	static ulong	regTextIndex365[];
	static ulong	regTextIndex366[];
	static ulong	regTextIndex367[];
	static ulong	regTextIndex368[];
	static ulong	regTextIndex369[];
	static ulong	regTextIndex370[];
	static ulong	regTextIndex371[];
	static ulong	regTextIndex372[];
	static ulong	regTextIndex373[];
	static ulong	regTextIndex374[];
	static ulong	regTextIndex375[];
	static ulong	regTextIndex376[];
	static ulong	regTextIndex377[];
	static ulong	regTextIndex378[];

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
