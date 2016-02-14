/***************************************************************************
                          sslrparser.h  -  description
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

#ifndef SSLRPARSER_H
#define SSLRPARSER_H


/**
	@author SSLR V0.5a
	(c) 2002-2005 by Sven Schmidt
	eMail: s.schmidt@lboro.ac.uk
  */


/****************************
* This is the debug version *
****************************/


//define debug macro
#define SSLR_DEBUG
#undef SSLR_DEBUG


//Include-Files
#include <iostream>
#include "sslexscanner.h"


//error routines
#define SSLR_ERROR_POP			SSLR_errPop
#define SSLR_ERROR_DELETE		SSLR_errDelete
#define SSLR_ERROR_SHIFT		SSLR_errShift
#define SSLR_ERROR_OVERREAD		SSLR_errOverRead
#define SSLR_ERROR_SEM			SSLR_semAct = 0
#define SSLR_ERROR_EXIT			exit( 1 )


//Nonterminal defines for the jump table
#define start	0
#define factor	1
#define st	2
#define expr	3
#define term	4


//Nontermials for the LR table
#define start_0	0
#define st_0	1
#define expr_0	2
#define expr_1	3
#define expr_2	4
#define expr_3	5
#define term_0	6
#define term_1	7
#define term_2	8
#define factor_0	9
#define factor_1	10


class SSLRParser
{
private:

	//LR table
	typedef struct
	{
		int			state;
		long		value;

	}SSLR_LR_TAB;


	//all productions
	typedef struct
	{
		//name of alternative
		char	*name;

		//pointer to right side of alternative
		char	**ri;

		//index in jumpTab
		int		jump_index;

		//number of elements on the right side
		int		elem;

		//pointer to follow sets
		long	*follow;

		//number of follow terminals
		long	followCnt;

	}SSLR_NTERM_STR;


	//this structure saves important values for the error recovery
	typedef struct
	{
		//stack pointer position
		int		sp;

		//state after reduction
		int		state;

		//number of alternative the reduction was done
		int		alt;

	}SSLR_ERROR_STR;


	//private class variables

	//Number of lr states
	static const unsigned long	SSLR_anzLRStates = 20;

	//Number of terminals = number of columns in lrTab
	static const unsigned long	SSLR_anzTerm = 10;

	//Number of productions = number of rows in jumpTab
	static const unsigned long	SSLR_anzNTerm = 5;

	//Number of alternatives
	static const unsigned long	SSLR_anzProd = 11;


	//Actions in lrTab
	static const int			LR_ERROR = 1;
	static const int			LR_SHIFT = 2;
	static const int			LR_REDUCE = 3;
	static const int			LR_ACCEPT = 4;


	//maximum stack size
	static const int			SSLR_maxStackSize = 100;


	//parse stack
	long						SSLR_stack[SSLR_maxStackSize];

	//stack counter
	long						SSLR_sp;


	//LR table
	static SSLR_LR_TAB			SSLR_lrTab[SSLR_anzLRStates][SSLR_anzTerm];

	//jump table
	static long					SSLR_jumpTab[SSLR_anzLRStates][SSLR_anzNTerm];

	//scanner class
	SSLEXScanner *				SSLR_scanner;

	//Array with all alternatives, for debug-mode only
	static SSLR_NTERM_STR		SSLR_ntermStr[SSLR_anzProd];

#ifdef SSLR_DEBUG
	//array with the names of the terminals
	static char *				SSLR_termName[SSLR_anzTerm];
#endif

	//current terminal
	int							SSLR_terminal;

	//attributes returned form scanner
	SSLR_TYPE					SSLR_attributeStr;

	//line number
	unsigned long				SSLR_line;

	//current state
	int							SSLR_state;

	//number of errors
	int							SSLR_anzError;

	//structure for error-recovery
	SSLR_ERROR_STR				SSLR_errorStr;

	//allow/deny semantic actions
	int							SSLR_semAct;

	//stack for semantic actions
	SSLR_TYPE					SSLR_semStack[SSLR_maxStackSize];

	//stack counter for semantic action stack
	int							SSLR_semStackCnt;

	//Alternatives
	static char *				SSLR_start0Ri[1];
	static char *				SSLR_st0Ri[2];
	static char *				SSLR_expr0Ri[3];
	static char *				SSLR_expr1Ri[3];
	static char *				SSLR_expr2Ri[2];
	static char *				SSLR_expr3Ri[1];
	static char *				SSLR_term0Ri[3];
	static char *				SSLR_term1Ri[3];
	static char *				SSLR_term2Ri[1];
	static char *				SSLR_factor0Ri[3];
	static char *				SSLR_factor1Ri[1];

	//all follow sets for the error recovery
	static long					SSLR_startFollow[1];
	static long					SSLR_stFollow[1];
	static long					SSLR_exprFollow[4];
	static long					SSLR_termFollow[6];
	static long					SSLR_factorFollow[6];


	//private methods


	//push a new state on parse stack
	void SSLR_push( long );

	//pop elements from parse stack
	long SSLR_pop( int );

	//begin parsing
	int SSLR_parse( void );

	//shift a new state onto the parse stack
	void SSLR_shift( SSLR_LR_TAB * );

	//reduce with a production
	void SSLR_reduce( SSLR_LR_TAB * );

	//error handling
	void SSLR_error( void );

#ifdef SSLR_DEBUG
	//output an alternative
	void SSLR_outputAlt( SSLR_NTERM_STR * );
#endif

	//shift a new state onto the parse stack
	void SSLR_errShift( long );

	//pop elements from the parse stack untill term may be shifted
	void SSLR_errPop( long );

	//delete next terminal
	void SSLR_errDelete( void );

	//delete all terminals untill term is found
	void SSLR_errOverRead( long );

	//perform semantic actions associated with the production
	//to be reduced
	void SSLR_doSemAct( SSLR_LR_TAB * );

	//get next terminal
	int SSLR_getTerminal( void );

public:

	//public methods 
	SSLRParser( SSLEXScanner * );
	~SSLRParser();

	//resets the whole parser
	void reset( void );

	//start parsing
	int yyParse( void );


};


#endif

