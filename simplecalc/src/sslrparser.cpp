/***************************************************************************
                          sslrparser.cpp  -  description
                             -------------------
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

#include "sslrparser.h"
#include <iostream>


/****************************
* This is the debug version *
****************************/


//Terminals for debugging
char *SSLR_scannerToken[] =
{
	/*0*/ "SEMIKOLON",
	/*1*/ "DIV",
	/*2*/ "KLCLOSE",
	/*3*/ "UMINUS",
	/*4*/ "ZAHL",
	/*5*/ "MUL",
	/*6*/ "SUB",
	/*7*/ "KLOPEN",
	/*8*/ "ADD",
	/*9*/ "ACCEPT",
	0L
};


//syntax analyze table
SSLRParser::SSLR_LR_TAB SSLRParser::SSLR_lrTab[SSLRParser::SSLR_anzLRStates][SSLRParser::SSLR_anzTerm] = {
/* State 0 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ LR_SHIFT, 3 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 1 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, start_0 } },
/* State 2 */
{ { LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 10 },{ 0, 0 },{ LR_SHIFT, 9 },
{ 0, 0 } },
/* State 3 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ LR_SHIFT, 3 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 4 */
{ { LR_REDUCE, expr_3 },{ LR_SHIFT, 13 },{ LR_REDUCE, expr_3 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 12 },{ LR_REDUCE, expr_3 },{ 0, 0 },{ LR_REDUCE, expr_3 },
{ 0, 0 } },
/* State 5 */
{ { LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ 0, 0 },{ LR_REDUCE, term_2 },
{ 0, 0 } },
/* State 6 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ LR_SHIFT, 3 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 7 */
{ { LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ 0, 0 },{ LR_REDUCE, factor_1 },
{ 0, 0 } },
/* State 8 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, st_0 } },
/* State 9 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 10 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 11 */
{ { LR_REDUCE, expr_2 },{ 0, 0 },{ LR_REDUCE, expr_2 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_2 },{ 0, 0 },{ LR_REDUCE, expr_2 },
{ 0, 0 } },
/* State 12 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 13 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },
{ 0, 0 } },
/* State 14 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 19 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 10 },{ 0, 0 },{ LR_SHIFT, 9 },
{ 0, 0 } },
/* State 15 */
{ { LR_REDUCE, expr_0 },{ LR_SHIFT, 13 },{ LR_REDUCE, expr_0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 12 },{ LR_REDUCE, expr_0 },{ 0, 0 },{ LR_REDUCE, expr_0 },
{ 0, 0 } },
/* State 16 */
{ { LR_REDUCE, expr_1 },{ LR_SHIFT, 13 },{ LR_REDUCE, expr_1 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 12 },{ LR_REDUCE, expr_1 },{ 0, 0 },{ LR_REDUCE, expr_1 },
{ 0, 0 } },
/* State 17 */
{ { LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ 0, 0 },{ LR_REDUCE, term_0 },
{ 0, 0 } },
/* State 18 */
{ { LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ 0, 0 },{ LR_REDUCE, term_1 },
{ 0, 0 } },
/* State 19 */
{ { LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ 0, 0 },{ LR_REDUCE, factor_0 },
{ 0, 0 } }
};


//jump table
long SSLRParser::SSLR_jumpTab[SSLRParser::SSLR_anzLRStates][SSLRParser::SSLR_anzNTerm] = {
//start, factor, st, expr, term, 

/* State 0 */
{  0,  5,  1,  2,  4 },
/* State 1 */
{  0,  0,  0,  0,  0 },
/* State 2 */
{  0,  0,  0,  0,  0 },
/* State 3 */
{  0,  5,  0, 11,  4 },
/* State 4 */
{  0,  0,  0,  0,  0 },
/* State 5 */
{  0,  0,  0,  0,  0 },
/* State 6 */
{  0,  5,  0, 14,  4 },
/* State 7 */
{  0,  0,  0,  0,  0 },
/* State 8 */
{  0,  0,  0,  0,  0 },
/* State 9 */
{  0,  5,  0,  0, 15 },
/* State 10 */
{  0,  5,  0,  0, 16 },
/* State 11 */
{  0,  0,  0,  0,  0 },
/* State 12 */
{  0, 17,  0,  0,  0 },
/* State 13 */
{  0, 18,  0,  0,  0 },
/* State 14 */
{  0,  0,  0,  0,  0 },
/* State 15 */
{  0,  0,  0,  0,  0 },
/* State 16 */
{  0,  0,  0,  0,  0 },
/* State 17 */
{  0,  0,  0,  0,  0 },
/* State 18 */
{  0,  0,  0,  0,  0 },
/* State 19 */
{  0,  0,  0,  0,  0 }
};


//follow sets for the error recovery
long SSLRParser::SSLR_startFollow[1] = { ACCEPT };
long SSLRParser::SSLR_stFollow[1] = { ACCEPT };
long SSLRParser::SSLR_exprFollow[4] = { SEMIKOLON, KLCLOSE, SUB, ADD };
long SSLRParser::SSLR_termFollow[6] = { SEMIKOLON, DIV, KLCLOSE, MUL, SUB, ADD };
long SSLRParser::SSLR_factorFollow[6] = { SEMIKOLON, DIV, KLCLOSE, MUL, SUB, ADD };

//all alternatives
char *SSLRParser::SSLR_start0Ri[1] = { "st" };
char *SSLRParser::SSLR_st0Ri[2] = { "expr", "\'SEMIKOLON\'" };
char *SSLRParser::SSLR_expr0Ri[3] = { "expr", "\'ADD\'", "term" };
char *SSLRParser::SSLR_expr1Ri[3] = { "expr", "\'SUB\'", "term" };
char *SSLRParser::SSLR_expr2Ri[2] = { "\'SUB\'", "expr" };
char *SSLRParser::SSLR_expr3Ri[1] = { "term" };
char *SSLRParser::SSLR_term0Ri[3] = { "term", "\'MUL\'", "factor" };
char *SSLRParser::SSLR_term1Ri[3] = { "term", "\'DIV\'", "factor" };
char *SSLRParser::SSLR_term2Ri[1] = { "factor" };
char *SSLRParser::SSLR_factor0Ri[3] = { "\'KLOPEN\'", "expr", "\'KLCLOSE\'" };
char *SSLRParser::SSLR_factor1Ri[1] = { "\'ZAHL\'" };


//all alternatives
SSLRParser::SSLR_NTERM_STR SSLRParser::SSLR_ntermStr[SSLRParser::SSLR_anzProd] = {
{ "start", SSLRParser::SSLR_start0Ri, start, 1, SSLRParser::SSLR_startFollow, 1 },
{ "st", SSLRParser::SSLR_st0Ri, st, 2, SSLRParser::SSLR_stFollow, 1 },
{ "expr", SSLRParser::SSLR_expr0Ri, expr, 3, SSLRParser::SSLR_exprFollow, 4 },
{ "expr", SSLRParser::SSLR_expr1Ri, expr, 3, SSLRParser::SSLR_exprFollow, 4 },
{ "expr", SSLRParser::SSLR_expr2Ri, expr, 2, SSLRParser::SSLR_exprFollow, 4 },
{ "expr", SSLRParser::SSLR_expr3Ri, expr, 1, SSLRParser::SSLR_exprFollow, 4 },
{ "term", SSLRParser::SSLR_term0Ri, term, 3, SSLRParser::SSLR_termFollow, 6 },
{ "term", SSLRParser::SSLR_term1Ri, term, 3, SSLRParser::SSLR_termFollow, 6 },
{ "term", SSLRParser::SSLR_term2Ri, term, 1, SSLRParser::SSLR_termFollow, 6 },
{ "factor", SSLRParser::SSLR_factor0Ri, factor, 3, SSLRParser::SSLR_factorFollow, 6 },
{ "factor", SSLRParser::SSLR_factor1Ri, factor, 1, SSLRParser::SSLR_factorFollow, 6 }};


#ifdef SSLR_DEBUG

//names of the terminals
char *SSLRParser::SSLR_termName[SSLRParser::SSLR_anzTerm] =
{ "SEMIKOLON", "DIV", "KLCLOSE", "UMINUS", "ZAHL", "MUL", "SUB", "KLOPEN", "ADD", "ACCEPT" };

#endif



void SSLRParser::SSLR_errPop( long termi )
{
	//get current state
	long state = SSLR_state;

	//pop states from the parse stack untill termi may be shifted
	while( SSLR_lrTab[state][termi].state != LR_SHIFT )
	{
		//pop one element
		state = SSLR_pop( 1 );

		//are we in state 1, that is could'n termi be shifted?
		if( state == 1 )
		{
			//all states poped from parse stack, exit
			SSLR_state = state;

			//set current terminal to ACCEPT to simulate successful parsing
			SSLR_terminal = ACCEPT;
			return;
		}
	}

	//set new state
	SSLR_state = state;
}


void SSLRParser::SSLR_errShift( long termi )
{
	//can we reduce here?
	while( SSLR_lrTab[SSLR_state][termi].state == LR_REDUCE )
		//yes, that might allow recovery
		SSLR_reduce( &SSLR_lrTab[SSLR_state][termi] );

	//shift a new state
	if( SSLR_lrTab[SSLR_state][termi].state == LR_SHIFT )
		//push new state onto the parse stack
		SSLR_push( SSLR_lrTab[SSLR_state][termi].value );
}


void SSLRParser::SSLR_errDelete( void )
{
	//delete current terminal
	SSLR_terminal = SSLR_getTerminal();
}


void SSLRParser::SSLR_errOverRead( long termi )
{
	//delete all terminals untill termi is found
	do
	{
		//get next terminal
		SSLR_terminal = SSLR_getTerminal();

		//equal?
		if( SSLR_terminal == termi )
			//yes, return
			return;

		else if( SSLR_terminal == ACCEPT )
		{
			//abort parsing
			SSLR_state = 1;
			return;
		}

	}while( 1 );
}


void SSLRParser::SSLR_error( void )
{
	//error handling
	SSLR_NTERM_STR	*n;
	long			*f, tmpState;
	int				fCnt = 0, i, rec = 0;

	//delete all terminals untill one that follows the last nonterminal
	//the reduction was done with
	SSLR_anzError++;

	//no semantic actions are allowed after errors
	SSLR_semAct++;

	//state after reduction
	tmpState = SSLR_errorStr.state;

	//get a pointer to the SSLR_NTERM_STR of the nonterminal
	n = &SSLR_ntermStr[SSLR_errorStr.alt];

	//debugging message
	cout << "SSLRParser::SSLR_error: Looking for terminal that may follow " << n->name << "..\n";

	//get the follow terminals
	f = n->follow;

	//number of follow terminals
	fCnt = n->followCnt;


	switch( SSLR_state )
	{
		// st ->  expr  'SEMIKOLON' .
		case 8:
		{
				//shift terminal SEMIKOLON
				SSLR_ERROR_SHIFT( SEMIKOLON );

				//delete current terminal
				SSLR_ERROR_DELETE();

				//skip automatic error recovery
				rec++;

			break;
		}

		// factor ->  'KLOPEN'  expr . 'KLCLOSE' 
		case 14:
		{
			if( ( SSLR_terminal == KLCLOSE ) || ( SSLR_terminal == KLOPEN ) )
			{
				//show user-defined error message
				cerr << "line: " << SSLR_line << ": ) expected!" << endl;

				//shift Terminal KLOPEN
				SSLR_ERROR_SHIFT( KLOPEN );

				//delete current terminal
				SSLR_ERROR_DELETE();

				//skip automatic error recovery
				rec++;

			}

			break;
		}

		default:
		{
			//default error message
			cerr << "line: " << SSLR_line << ": Syntax error!" << endl;
			break;
		}
	}


	//do SSLRs intern error recovery?
	if( !rec )
	{
		//yes, no user defined error routines given

		do
		{
			//is the current terminal ACCEPT?
			if( SSLR_terminal == ACCEPT )
			{
				//abort parsing
				SSLR_state = 1;
				return;
			}

			//may the terminal SSLR_terminal follow?
			for( i = 0; i < SSLR_anzTerm; i++ )
			{
				//shift OK?
				if( SSLR_lrTab[tmpState][SSLR_terminal].state == LR_SHIFT )
				{

#ifdef SSLR_DEBUG
					//inform the user
					cout << SSLR_termName[SSLR_terminal] << " may follow " << n->name << "\n\n";
#endif

					//set new state
					SSLR_state = SSLR_errorStr.state;

					//get stack position
					SSLR_sp = SSLR_errorStr.sp;
					return;
				}
			}

			//get next terminal
			SSLR_terminal = SSLR_getTerminal();

		}while( 1 );
	}

	return;
}


SSLRParser::SSLRParser( SSLEXScanner *scan )
{
	//call reset to initialize the parser
	reset();

	//scanner class
	SSLR_scanner = scan;

#ifdef SSLR_INIT
	//inizialize user defined variables
	init();
#endif
}


SSLRParser::~SSLRParser()
{
#ifdef SSLR_INIT
	//uninizialize user defined variables
	deinit();
#endif
}


void SSLRParser::reset( void )
{
	//resets the whole parser

	//parse stack pointer
	SSLR_sp = 0;

	//start with state 0
	SSLR_push( 0 );
	SSLR_state = 0;

	//no errors yet
	SSLR_anzError = 0;

	//defualt values for error recovery
	SSLR_errorStr.alt = start_0;
	SSLR_errorStr.sp = SSLR_sp;
	SSLR_errorStr.state = 0;

	//semantic actions
	SSLR_semAct = 0;

	//stack counter for semantic action stack
	SSLR_semStackCnt = 0;
}


int SSLRParser::SSLR_getTerminal( void )
{
	//semantic actions allowed?
	if( !SSLR_semAct )
		//push attribute on stack for semantic actions
		SSLR_semStack[SSLR_semStackCnt++] = SSLR_attributeStr;

	//get line
	SSLR_line = SSLR_scanner->getYYLineno();

	//get next terminal
	SSLR_terminal = SSLR_scanner->yyLex( &SSLR_attributeStr );

	//return terminal
	return( SSLR_terminal );
}


int SSLRParser::yyParse( void )
{
	//start parsing

	//get first terminal
	SSLR_terminal = SSLR_getTerminal();

	//start parsing
	return( SSLR_parse() );
}


void SSLRParser::SSLR_push( long ast )
{
	//push a new state on the parse stack
	SSLR_stack[SSLR_sp++] = ast;

	//stack overflow?
	if( SSLR_sp > SSLR_maxStackSize )
	{
		//stack-overflow
		cerr << "SSLRParser::SSLR_push: stack overflow!\nExiting..." << endl;
		exit( 1 );
	}

	//set new state
	SSLR_state = ast;
}


long SSLRParser::SSLR_pop( int anz )
{
	//pop anz elements from the parse stack
	SSLR_sp -= anz;

	//stack underflow?
	if( SSLR_sp < 0 )
	{
		//stack underflow
		cerr << "SSLRParser::SSLR_pop: stack underflow!\nExiting..." << endl;
		exit( 1 );
	}

	//return new state
	return( SSLR_stack[SSLR_sp - 1] );
}


int SSLRParser::SSLR_parse( void )
{
	//begin parsing

	while( 1 )
	{
		//end of parsing?
		if( SSLR_state == 1 )
		{
			//yes, is terminal ACCEPT
			if( SSLR_terminal == ACCEPT )
				//return number of errors
				return( SSLR_anzError );

			else
			{
				//error
				cerr << "SSLRParser::SSLR_parse: bailing out..." << endl;
				return( SSLR_anzError );
			}
		}

		//shift terminal SSLR_terminal?
		else if( SSLR_lrTab[SSLR_state][SSLR_terminal].state == LR_SHIFT )
		{
			//yes
			SSLR_shift( &SSLR_lrTab[SSLR_state][SSLR_terminal] );

			//get new terminal
			SSLR_terminal = SSLR_getTerminal();
		}

		//reduce with production?
		else if( SSLR_lrTab[SSLR_state][SSLR_terminal].state == LR_REDUCE )
			//reduce
			SSLR_reduce( &SSLR_lrTab[SSLR_state][SSLR_terminal] );

		//otherwise an error occured
		else
			//call error recovery
			SSLR_error();
	}
}


void SSLRParser::SSLR_shift( SSLR_LR_TAB *tab )
{
	//shift a terminal onto the parse stack

#ifdef SSLR_DEBUG
	//output
	cout << "Shift" << endl;
	cout << "current state: " << SSLR_state << endl;
	cout << "current terminal: " << SSLR_termName[SSLR_terminal] << endl;
#endif

	//shift a new state onto the stack
	SSLR_push( tab->value );


#ifdef SSLR_DEBUG
	//output
	cout << "shift new state " << SSLR_state << endl << endl;
#endif
}


void SSLRParser::SSLR_reduce( SSLR_LR_TAB *tab )
{
	//reduce with production
	SSLR_NTERM_STR	*n;

	//perform a semantic action accociated with the reduced production
	SSLR_doSemAct( tab );

	//get a pointer to the alternative
	n = &SSLR_ntermStr[tab->value];

#ifdef SSLR_DEBUG
	//output
	cout << "Reduce\n";
	cout << "current state: " << SSLR_state << "\n";
	cout << "current terminal: " << SSLR_termName[SSLR_terminal] << "\n";
	cout << "reduce with ";

	//output alternative
	SSLR_outputAlt( n );

	//output
	cout << "\n";
#endif

	//remove states form stack
	SSLR_state = SSLR_pop( n->elem );

	//get new state
	SSLR_state = SSLR_jumpTab[SSLR_state][n->jump_index];

	//push new state on stack
	SSLR_push( SSLR_state );

#ifdef SSLR_DEBUG
	//output
	cout << "new state " << SSLR_state << "\n\n";
#endif

	//for error recovery
	SSLR_errorStr.alt = tab->value;
	SSLR_errorStr.sp = SSLR_sp;
	SSLR_errorStr.state = SSLR_state;
}


#ifdef SSLR_DEBUG
void SSLRParser::SSLR_outputAlt( SSLR_NTERM_STR *n )
{
	//output an alternative
	int	i;

	//name of alternative
	cout << n->name << " -> ";

	//output right side
	for( i = 0; i < n->elem; i++ )
		cout << n->ri[i] << " ";

	//EPSILON alternative?
	if( !n->elem )
		//yes
		cout << "'EPSILON'";
}
#endif
void SSLRParser::SSLR_doSemAct( SSLR_LR_TAB *tab )
{
	//do semantic actions before reduction
	SSLR_NTERM_STR	*n;
	SSLR_TYPE		tmp;

	//semantic actions allowed?
	if( !SSLR_semAct )
	{
		//get a pointer to the alternative
		n = &SSLR_ntermStr[tab->value];

		//Alternative to reduce with
		switch( tab->value )
		{
			// st ->  expr  'SEMIKOLON' .
			case st_0:
			{
				 printf( "%f", SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (1)].f );
				 break;
			}

			// expr ->  expr  'ADD'  term .
			case expr_0:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)].f + SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)].f;
				break;
			}

			// expr ->  expr  'SUB'  term .
			case expr_1:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)].f - SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)].f;
				break;
			}

			// expr ->  'SUB'  expr .
			case expr_2:
			{
				 tmp.f = -SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)].f;
				 break;
			}

			// expr ->  term .
			case expr_3:
			{
				tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)].f;
				break;
			}

			// term ->  term  'MUL'  factor .
			case term_0:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)].f * SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)].f;
				 break;
			}

			// term ->  term  'DIV'  factor .
			case term_1:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)].f / SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)].f;
				 break;
			}

			// term ->  factor .
			case term_2:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)].f;
				 break;
			}

			// factor ->  'KLOPEN'  expr  'KLCLOSE' .
			case factor_0:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)].f;
				 break;
			}

			// factor ->  'ZAHL' .
			case factor_1:
			{
				 tmp.f = SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)].f;
				 break;
			}

			default:
				//error
				cerr << "SSLRParser::SSLR_doSemAct: Unknown alternative " << tab->value << "!" << endl;
		}

		//correct stack pointer
		SSLR_semStackCnt -= n->elem;

		//push last value on stack
		SSLR_semStack[SSLR_semStackCnt++] = tmp;
	}
}

