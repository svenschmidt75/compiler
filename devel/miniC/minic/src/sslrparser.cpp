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
	/*1*/ "LESS",
	/*2*/ "TRUE",
	/*3*/ "DIV",
	/*4*/ "LESS_EQU",
	/*5*/ "EQU",
	/*6*/ "DO",
	/*7*/ "BIN_OR",
	/*8*/ "KLCLOSE",
	/*9*/ "COMMA",
	/*10*/ "LEFT_SHIFT",
	/*11*/ "SHORT",
	/*12*/ "INT",
	/*13*/ "IF_STMT",
	/*14*/ "LOG_NOT",
	/*15*/ "FALSE",
	/*16*/ "BOPEN",
	/*17*/ "BIN_NOT",
	/*18*/ "LONG",
	/*19*/ "NUMBER",
	/*20*/ "MOD",
	/*21*/ "MUL",
	/*22*/ "LOG_OR",
	/*23*/ "BCLOSE",
	/*24*/ "BOOL",
	/*25*/ "RIGHT_SHIFT",
	/*26*/ "WHILE",
	/*27*/ "NOTEQUAL",
	/*28*/ "EQUAL",
	/*29*/ "SUB",
	/*30*/ "GREATER",
	/*31*/ "LOG_AND",
	/*32*/ "ELSE_STMT",
	/*33*/ "KLOPEN",
	/*34*/ "BIN_XOR",
	/*35*/ "BIN_AND",
	/*36*/ "GREATER_EQU",
	/*37*/ "IDENT",
	/*38*/ "ADD",
	/*39*/ "CHAR",
	/*40*/ "ACCEPT",
	0L
};



	//miniC initialisieren
	#define SSLR_INIT

//syntax analyze table
SSLRParser::SSLR_LR_TAB SSLRParser::SSLR_lrTab[SSLRParser::SSLR_anzLRStates][SSLRParser::SSLR_anzTerm] = {
/* State 0 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 1 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, start_0 }
 },
/* State 2 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ LR_REDUCE, program_0 }
 },
/* State 3 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },
{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_0 },
{ 0, 0 },{ LR_REDUCE, definitions_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, definitions_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_0 },{ 0, 0 },{ LR_REDUCE, definitions_0 },{ LR_REDUCE, definitions_0 }
 },
/* State 4 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ LR_REDUCE, definition_0 }
 },
/* State 5 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },
{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 },
{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, stmt_list_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ 0, 0 },{ LR_REDUCE, stmt_list_0 },{ LR_REDUCE, stmt_list_0 }
 },
/* State 6 */
{ { LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_SHIFT, 30 },{ 0, 0 },{ LR_REDUCE, factor_4 },{ 0, 0 },
{ 0, 0 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, factor_4 },{ 0, 0 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },
{ 0, 0 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ 0, 0 }
 },
/* State 7 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 8 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 9 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 10 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_5 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_5 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_REDUCE, stmt_5 },{ LR_REDUCE, stmt_5 },{ LR_REDUCE, stmt_5 },{ LR_REDUCE, stmt_5 },
{ LR_REDUCE, stmt_5 },{ LR_SHIFT, 23 },{ LR_REDUCE, stmt_5 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_5 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_REDUCE, stmt_5 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_5 },
{ LR_REDUCE, stmt_5 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_5 },{ 0, 0 },{ LR_SHIFT, 19 },{ LR_REDUCE, stmt_5 }
 },
/* State 11 */
{ { LR_SHIFT, 36 },{ LR_SHIFT, 37 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 48 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 45 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 46 },{ 0, 0 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ 0, 0 },{ LR_SHIFT, 50 },{ 0, 0 },{ 0, 0 }
 },
/* State 12 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },
{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },{ LR_REDUCE, auxsym_1_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },
{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, auxsym_1_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },{ 0, 0 },{ LR_REDUCE, auxsym_1_0 },{ 0, 0 }
 },
/* State 13 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_1 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },
{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 },
{ 0, 0 },{ LR_REDUCE, declarations_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_1 },
{ LR_REDUCE, declarations_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_1 },{ 0, 0 },{ LR_REDUCE, declarations_1 },{ LR_REDUCE, declarations_1 }
 },
/* State 14 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 15 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 16 */
{ { LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_SHIFT, 56 },{ LR_REDUCE, expr_17 },{ 0, 0 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },
{ 0, 0 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },
{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_SHIFT, 57 },{ LR_SHIFT, 55 },{ LR_REDUCE, expr_17 },{ 0, 0 },{ LR_REDUCE, expr_17 },
{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ 0, 0 },
{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ LR_REDUCE, expr_17 },{ 0, 0 }
 },
/* State 17 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 59 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 18 */
{ { LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ 0, 0 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },
{ 0, 0 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },
{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ 0, 0 },{ LR_REDUCE, term_3 },
{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ 0, 0 },
{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ LR_REDUCE, term_3 },{ 0, 0 }
 },
/* State 19 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, type_specifier_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 20 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, type_specifier_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 21 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, type_specifier_2 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 22 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, type_specifier_3 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 23 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, type_specifier_4 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 24 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 25 */
{ { LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ 0, 0 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },
{ 0, 0 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },
{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ 0, 0 },{ LR_REDUCE, factor_1 },
{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ 0, 0 },
{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ LR_REDUCE, factor_1 },{ 0, 0 }
 },
/* State 26 */
{ { LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ 0, 0 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },
{ 0, 0 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },
{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ 0, 0 },{ LR_REDUCE, factor_2 },
{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ 0, 0 },
{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ LR_REDUCE, factor_2 },{ 0, 0 }
 },
/* State 27 */
{ { LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ 0, 0 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },
{ 0, 0 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },
{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ 0, 0 },{ LR_REDUCE, factor_3 },
{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ 0, 0 },
{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ LR_REDUCE, factor_3 },{ 0, 0 }
 },
/* State 28 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_1 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },
{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_1 },
{ 0, 0 },{ LR_REDUCE, definitions_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, definitions_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, definitions_1 },{ 0, 0 },{ LR_REDUCE, definitions_1 },{ LR_REDUCE, definitions_1 }
 },
/* State 29 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },
{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 },
{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, stmt_list_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ 0, 0 },{ LR_REDUCE, stmt_list_1 },{ LR_REDUCE, stmt_list_1 }
 },
/* State 30 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 31 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 62 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 32 */
{ { 0, 0 },{ LR_SHIFT, 37 },{ LR_SHIFT, 26 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_SHIFT, 7 },{ LR_SHIFT, 48 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ LR_SHIFT, 20 },
{ LR_SHIFT, 46 },{ LR_SHIFT, 8 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ LR_SHIFT, 24 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_SHIFT, 6 },{ LR_SHIFT, 50 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 33 */
{ { LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },
{ 0, 0 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },
{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },{ LR_REDUCE, factor_4 },
{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 },
{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ LR_REDUCE, factor_4 },{ 0, 0 }
 },
/* State 34 */
{ { 0, 0 },{ LR_SHIFT, 37 },{ LR_SHIFT, 26 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_SHIFT, 7 },{ LR_SHIFT, 48 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ LR_SHIFT, 20 },
{ LR_SHIFT, 46 },{ LR_SHIFT, 8 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ LR_SHIFT, 24 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_SHIFT, 6 },{ LR_SHIFT, 50 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 35 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },
{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 },
{ 0, 0 },{ LR_REDUCE, declarations_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },
{ LR_REDUCE, declarations_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },{ 0, 0 },{ LR_REDUCE, declarations_0 },{ LR_REDUCE, declarations_0 }
 },
/* State 36 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_6 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_6 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },
{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 },
{ 0, 0 },{ LR_REDUCE, stmt_6 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_6 },
{ LR_REDUCE, stmt_6 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_6 },{ 0, 0 },{ LR_REDUCE, stmt_6 },{ LR_REDUCE, stmt_6 }
 },
/* State 37 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 38 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 39 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 40 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 41 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 42 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 43 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 44 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 45 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 46 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 47 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 48 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 49 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 50 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 27 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 51 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 27 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 52 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 53 */
{ { LR_REDUCE, expr_8 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_8 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_8 },{ LR_SHIFT, 48 },{ LR_REDUCE, expr_8 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },
{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },{ LR_REDUCE, expr_8 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ LR_REDUCE, expr_8 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_8 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ LR_REDUCE, expr_8 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_8 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_8 },{ 0, 0 }
 },
/* State 54 */
{ { LR_REDUCE, expr_14 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_14 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },
{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_14 },{ 0, 0 },{ LR_REDUCE, expr_14 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_14 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_14 },{ 0, 0 },
{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_REDUCE, expr_14 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_14 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_14 },{ 0, 0 }
 },
/* State 55 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 27 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 56 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 27 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 57 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 27 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 58 */
{ { LR_SHIFT, 84 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 85 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 59 */
{ { LR_REDUCE, declarator_list_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, declarator_list_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 60 */
{ { 0, 0 },{ LR_SHIFT, 37 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 48 },{ LR_SHIFT, 86 },
{ 0, 0 },{ LR_SHIFT, 45 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 46 },{ 0, 0 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ 0, 0 },{ LR_SHIFT, 50 },{ 0, 0 },{ 0, 0 }
 },
/* State 61 */
{ { LR_SHIFT, 87 },{ LR_SHIFT, 37 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 48 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 45 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 46 },{ 0, 0 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ 0, 0 },{ LR_SHIFT, 50 },{ 0, 0 },{ 0, 0 }
 },
/* State 62 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ 0, 0 },
{ LR_SHIFT, 15 },{ 0, 0 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 33 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 63 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_2 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_2 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },
{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 },
{ 0, 0 },{ LR_REDUCE, stmt_2 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_2 },
{ LR_REDUCE, stmt_2 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_2 },{ 0, 0 },{ LR_REDUCE, stmt_2 },{ LR_REDUCE, stmt_2 }
 },
/* State 64 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_4 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_4 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },
{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 },
{ 0, 0 },{ LR_REDUCE, stmt_4 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 89 },
{ LR_REDUCE, stmt_4 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_4 },{ 0, 0 },{ LR_REDUCE, stmt_4 },{ LR_REDUCE, stmt_4 }
 },
/* State 65 */
{ { LR_REDUCE, expr_0 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_0 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },
{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_0 },{ 0, 0 },{ LR_REDUCE, expr_0 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_0 },{ 0, 0 },
{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_REDUCE, expr_0 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_0 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_0 },{ 0, 0 }
 },
/* State 66 */
{ { LR_REDUCE, expr_1 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_1 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },
{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_1 },{ 0, 0 },{ LR_REDUCE, expr_1 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_1 },{ 0, 0 },
{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_REDUCE, expr_1 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_1 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_1 },{ 0, 0 }
 },
/* State 67 */
{ { LR_REDUCE, expr_2 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_2 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },
{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_2 },{ 0, 0 },{ LR_REDUCE, expr_2 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_2 },{ 0, 0 },
{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_REDUCE, expr_2 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_2 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_2 },{ 0, 0 }
 },
/* State 68 */
{ { LR_REDUCE, expr_3 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_3 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },
{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_3 },{ 0, 0 },{ LR_REDUCE, expr_3 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_3 },{ 0, 0 },
{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_REDUCE, expr_3 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_3 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_3 },{ 0, 0 }
 },
/* State 69 */
{ { LR_REDUCE, expr_4 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_4 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },
{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_4 },{ 0, 0 },{ LR_REDUCE, expr_4 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_4 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_4 },{ 0, 0 },
{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_REDUCE, expr_4 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_4 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_4 },{ 0, 0 }
 },
/* State 70 */
{ { LR_REDUCE, expr_5 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_5 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },
{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_5 },{ 0, 0 },{ LR_REDUCE, expr_5 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_5 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_5 },{ 0, 0 },
{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_REDUCE, expr_5 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_5 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_5 },{ 0, 0 }
 },
/* State 71 */
{ { LR_REDUCE, expr_6 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_6 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_6 },{ LR_SHIFT, 48 },{ LR_REDUCE, expr_6 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },
{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },{ LR_REDUCE, expr_6 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_6 },{ 0, 0 },{ LR_REDUCE, expr_6 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_6 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ LR_REDUCE, expr_6 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_6 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_6 },{ 0, 0 }
 },
/* State 72 */
{ { LR_REDUCE, expr_7 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_7 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_7 },{ LR_SHIFT, 48 },{ LR_REDUCE, expr_7 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },
{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },{ LR_REDUCE, expr_7 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ LR_REDUCE, expr_7 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_7 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ LR_REDUCE, expr_7 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_7 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_7 },{ 0, 0 }
 },
/* State 73 */
{ { LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ 0, 0 },{ LR_REDUCE, expr_9 },{ 0, 0 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },
{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_9 },{ 0, 0 },{ LR_REDUCE, expr_9 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_SHIFT, 51 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ 0, 0 },
{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_REDUCE, expr_9 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_9 },{ 0, 0 }
 },
/* State 74 */
{ { LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ 0, 0 },{ LR_REDUCE, expr_10 },{ 0, 0 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },
{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_10 },{ 0, 0 },{ LR_REDUCE, expr_10 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_SHIFT, 51 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ 0, 0 },
{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_REDUCE, expr_10 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_10 },{ 0, 0 }
 },
/* State 75 */
{ { LR_REDUCE, expr_11 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_11 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },
{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_11 },{ 0, 0 },{ LR_REDUCE, expr_11 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_11 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_11 },{ 0, 0 },
{ LR_REDUCE, expr_11 },{ LR_REDUCE, expr_11 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_11 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_11 },{ 0, 0 }
 },
/* State 76 */
{ { LR_REDUCE, expr_12 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_12 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_12 },{ LR_SHIFT, 48 },{ LR_REDUCE, expr_12 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },
{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },{ LR_REDUCE, expr_12 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_12 },{ 0, 0 },{ LR_REDUCE, expr_12 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_12 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_12 },{ 0, 0 },
{ LR_REDUCE, expr_12 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_12 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_12 },{ 0, 0 }
 },
/* State 77 */
{ { LR_REDUCE, expr_13 },{ LR_SHIFT, 37 },{ LR_REDUCE, expr_13 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },
{ 0, 0 },{ LR_SHIFT, 45 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },
{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ LR_REDUCE, expr_13 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, expr_13 },{ 0, 0 },{ LR_REDUCE, expr_13 },
{ LR_SHIFT, 46 },{ LR_REDUCE, expr_13 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_REDUCE, expr_13 },{ 0, 0 },
{ LR_REDUCE, expr_13 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ LR_REDUCE, expr_13 },{ LR_SHIFT, 50 },{ LR_REDUCE, expr_13 },{ 0, 0 }
 },
/* State 78 */
{ { LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_SHIFT, 56 },{ LR_REDUCE, expr_15 },{ 0, 0 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },
{ 0, 0 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },
{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_SHIFT, 57 },{ LR_SHIFT, 55 },{ LR_REDUCE, expr_15 },{ 0, 0 },{ LR_REDUCE, expr_15 },
{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ 0, 0 },
{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ LR_REDUCE, expr_15 },{ 0, 0 }
 },
/* State 79 */
{ { LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_SHIFT, 56 },{ LR_REDUCE, expr_16 },{ 0, 0 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },
{ 0, 0 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },
{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_SHIFT, 57 },{ LR_SHIFT, 55 },{ LR_REDUCE, expr_16 },{ 0, 0 },{ LR_REDUCE, expr_16 },
{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ 0, 0 },
{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ LR_REDUCE, expr_16 },{ 0, 0 }
 },
/* State 80 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 90 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 81 */
{ { LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ 0, 0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },
{ 0, 0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },
{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ 0, 0 },{ LR_REDUCE, term_0 },
{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ 0, 0 },
{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ LR_REDUCE, term_0 },{ 0, 0 }
 },
/* State 82 */
{ { LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ 0, 0 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },
{ 0, 0 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },
{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ 0, 0 },{ LR_REDUCE, term_1 },
{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ 0, 0 },
{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ LR_REDUCE, term_1 },{ 0, 0 }
 },
/* State 83 */
{ { LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ 0, 0 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },
{ 0, 0 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },
{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ 0, 0 },{ LR_REDUCE, term_2 },
{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ 0, 0 },
{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ LR_REDUCE, term_2 },{ 0, 0 }
 },
/* State 84 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },
{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 },
{ 0, 0 },{ LR_REDUCE, declaration_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },
{ LR_REDUCE, declaration_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },{ 0, 0 },{ LR_REDUCE, declaration_0 },{ LR_REDUCE, declaration_0 }
 },
/* State 85 */
{ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 91 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 86 */
{ { LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ 0, 0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },
{ 0, 0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },
{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ 0, 0 },{ LR_REDUCE, factor_0 },
{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ 0, 0 },
{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ LR_REDUCE, factor_0 },{ 0, 0 }
 },
/* State 87 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },
{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 },
{ 0, 0 },{ LR_REDUCE, stmt_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },
{ LR_REDUCE, stmt_0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },{ 0, 0 },{ LR_REDUCE, stmt_0 },{ LR_REDUCE, stmt_0 }
 },
/* State 88 */
{ { LR_SHIFT, 92 },{ LR_SHIFT, 37 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 39 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 48 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 45 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 44 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 46 },{ 0, 0 },{ LR_SHIFT, 42 },{ LR_SHIFT, 41 },{ LR_SHIFT, 51 },{ LR_SHIFT, 38 },{ LR_SHIFT, 43 },{ 0, 0 },
{ 0, 0 },{ LR_SHIFT, 49 },{ LR_SHIFT, 47 },{ LR_SHIFT, 40 },{ 0, 0 },{ LR_SHIFT, 50 },{ 0, 0 },{ 0, 0 }
 },
/* State 89 */
{ { 0, 0 },{ 0, 0 },{ LR_SHIFT, 26 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 21 },{ LR_SHIFT, 22 },{ LR_SHIFT, 9 },{ LR_SHIFT, 14 },{ LR_SHIFT, 27 },{ LR_SHIFT, 12 },
{ LR_SHIFT, 15 },{ LR_SHIFT, 23 },{ LR_SHIFT, 25 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 20 },
{ 0, 0 },{ LR_SHIFT, 8 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_SHIFT, 24 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_SHIFT, 6 },{ 0, 0 },{ LR_SHIFT, 19 },{ 0, 0 }
 },
/* State 90 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_7 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_7 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },
{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 },
{ 0, 0 },{ LR_REDUCE, stmt_7 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_7 },
{ LR_REDUCE, stmt_7 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_7 },{ 0, 0 },{ LR_REDUCE, stmt_7 },{ LR_REDUCE, stmt_7 }
 },
/* State 91 */
{ { LR_REDUCE, declarator_list_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ LR_REDUCE, declarator_list_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 }
 },
/* State 92 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_1 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },
{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 },
{ 0, 0 },{ LR_REDUCE, stmt_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_1 },
{ LR_REDUCE, stmt_1 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_1 },{ 0, 0 },{ LR_REDUCE, stmt_1 },{ LR_REDUCE, stmt_1 }
 },
/* State 93 */
{ { 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_3 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_3 },{ 0, 0 },{ 0, 0 },
{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },
{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 },
{ 0, 0 },{ LR_REDUCE, stmt_3 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_3 },
{ LR_REDUCE, stmt_3 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ LR_REDUCE, stmt_3 },{ 0, 0 },{ LR_REDUCE, stmt_3 },{ LR_REDUCE, stmt_3 }
 }
};


//jump table
long SSLRParser::SSLR_jumpTab[SSLRParser::SSLR_anzLRStates][SSLRParser::SSLR_anzNTerm] = {
//start, declaration, factor, auxsym_1, type_specifier, expr, definitions, stmt_list, definition, declarations, 
//stmt, program, term, declarator_list, 

/* State 0 */
{  0, 13, 18,  0, 17, 11,  2,  4,  3, 10,  5,  1, 16,  0 },
/* State 1 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 2 */
{  0, 13, 18,  0, 17, 11,  0,  4, 28, 10,  5,  0, 16,  0 },
/* State 3 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 4 */
{  0, 13, 18,  0, 17, 11,  0,  0,  0, 10, 29,  0, 16,  0 },
/* State 5 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 6 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 7 */
{  0, 13, 18,  0, 17, 11,  0,  0,  0, 10, 31,  0, 16,  0 },
/* State 8 */
{  0,  0, 18,  0,  0, 32,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 9 */
{  0,  0, 18,  0,  0, 34,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 10 */
{  0, 35,  0,  0, 17,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 11 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 12 */
{  0,  0,  0, 52,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 13 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 14 */
{  0,  0, 18,  0,  0, 53,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 15 */
{  0,  0, 18,  0,  0, 54,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 16 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 17 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 58 },
/* State 18 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 19 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 20 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 21 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 22 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 23 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 24 */
{  0,  0, 18,  0,  0, 60,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 25 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 26 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 27 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 28 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 29 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 30 */
{  0,  0, 18,  0,  0, 61,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 31 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 32 */
{  0, 13, 18,  0, 17, 11,  0,  0,  0, 10, 63,  0, 16,  0 },
/* State 33 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 34 */
{  0, 13, 18,  0, 17, 11,  0,  0,  0, 10, 64,  0, 16,  0 },
/* State 35 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 36 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 37 */
{  0,  0, 18,  0,  0, 65,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 38 */
{  0,  0, 18,  0,  0, 66,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 39 */
{  0,  0, 18,  0,  0, 67,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 40 */
{  0,  0, 18,  0,  0, 68,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 41 */
{  0,  0, 18,  0,  0, 69,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 42 */
{  0,  0, 18,  0,  0, 70,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 43 */
{  0,  0, 18,  0,  0, 71,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 44 */
{  0,  0, 18,  0,  0, 72,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 45 */
{  0,  0, 18,  0,  0, 73,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 46 */
{  0,  0, 18,  0,  0, 74,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 47 */
{  0,  0, 18,  0,  0, 75,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 48 */
{  0,  0, 18,  0,  0, 76,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 49 */
{  0,  0, 18,  0,  0, 77,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 50 */
{  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0, 78,  0 },
/* State 51 */
{  0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0, 79,  0 },
/* State 52 */
{  0, 13, 18,  0, 17, 11,  0, 80,  0, 10,  5,  0, 16,  0 },
/* State 53 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 54 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 55 */
{  0,  0, 81,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 56 */
{  0,  0, 82,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 57 */
{  0,  0, 83,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 58 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 59 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 60 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 61 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 62 */
{  0,  0, 18,  0,  0, 88,  0,  0,  0,  0,  0,  0, 16,  0 },
/* State 63 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 64 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 65 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 66 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 67 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 68 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 69 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 70 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 71 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 72 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 73 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 74 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 75 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 76 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 77 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 78 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 79 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 80 */
{  0, 13, 18,  0, 17, 11,  0,  0,  0, 10, 29,  0, 16,  0 },
/* State 81 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 82 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 83 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 84 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 85 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 86 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 87 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 88 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 89 */
{  0, 13, 18,  0, 17, 11,  0,  0,  0, 10, 93,  0, 16,  0 },
/* State 90 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 91 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 92 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* State 93 */
{  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};


//follow sets for the error recovery
long SSLRParser::SSLR_startFollow[1] = { ACCEPT };
long SSLRParser::SSLR_programFollow[1] = { ACCEPT };
long SSLRParser::SSLR_definitionsFollow[17] = { TRUE, DO, ACCEPT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BOOL, WHILE, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_definitionFollow[17] = { TRUE, DO, ACCEPT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BOOL, WHILE, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_declarationsFollow[19] = { TRUE, DO, ACCEPT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BCLOSE, BOOL, WHILE, ELSE_STMT, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_declarationFollow[19] = { TRUE, DO, ACCEPT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BCLOSE, BOOL, WHILE, ELSE_STMT, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_declarator_listFollow[2] = { SEMIKOLON, COMMA };
long SSLRParser::SSLR_type_specifierFollow[1] = { IDENT };
long SSLRParser::SSLR_stmt_listFollow[18] = { TRUE, DO, ACCEPT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BCLOSE, BOOL, WHILE, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_stmtFollow[19] = { TRUE, DO, ACCEPT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BCLOSE, BOOL, WHILE, ELSE_STMT, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_auxsym_1Follow[16] = { TRUE, DO, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, BOOL, WHILE, KLOPEN, IDENT, CHAR };
long SSLRParser::SSLR_exprFollow[33] = { SEMIKOLON, LESS, TRUE, LESS_EQU, DO, BIN_OR, KLCLOSE, LEFT_SHIFT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, LOG_OR, BOOL, RIGHT_SHIFT, WHILE, NOTEQUAL, EQUAL, SUB, GREATER, LOG_AND, KLOPEN, BIN_XOR, BIN_AND, GREATER_EQU, IDENT, ADD, CHAR };
long SSLRParser::SSLR_termFollow[36] = { SEMIKOLON, LESS, TRUE, DIV, LESS_EQU, DO, BIN_OR, KLCLOSE, LEFT_SHIFT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, MOD, MUL, LOG_OR, BOOL, RIGHT_SHIFT, WHILE, NOTEQUAL, EQUAL, SUB, GREATER, LOG_AND, KLOPEN, BIN_XOR, BIN_AND, GREATER_EQU, IDENT, ADD, CHAR };
long SSLRParser::SSLR_factorFollow[36] = { SEMIKOLON, LESS, TRUE, DIV, LESS_EQU, DO, BIN_OR, KLCLOSE, LEFT_SHIFT, SHORT, INT, IF_STMT, LOG_NOT, FALSE, BOPEN, BIN_NOT, LONG, NUMBER, MOD, MUL, LOG_OR, BOOL, RIGHT_SHIFT, WHILE, NOTEQUAL, EQUAL, SUB, GREATER, LOG_AND, KLOPEN, BIN_XOR, BIN_AND, GREATER_EQU, IDENT, ADD, CHAR };

//all alternatives
char *SSLRParser::SSLR_start0Ri[1] = { "program" };
char *SSLRParser::SSLR_program0Ri[1] = { "definitions" };
char *SSLRParser::SSLR_definitions0Ri[1] = { "definition" };
char *SSLRParser::SSLR_definitions1Ri[2] = { "definitions", "definition" };
char *SSLRParser::SSLR_definition0Ri[1] = { "stmt_list" };
char *SSLRParser::SSLR_declarations0Ri[2] = { "declarations", "declaration" };
char *SSLRParser::SSLR_declarations1Ri[1] = { "declaration" };
char *SSLRParser::SSLR_declaration0Ri[3] = { "type_specifier", "declarator_list", "\'SEMIKOLON\'" };
char *SSLRParser::SSLR_declarator_list0Ri[1] = { "\'IDENT\'" };
char *SSLRParser::SSLR_declarator_list1Ri[3] = { "declarator_list", "\'COMMA\'", "\'IDENT\'" };
char *SSLRParser::SSLR_type_specifier0Ri[1] = { "\'CHAR\'" };
char *SSLRParser::SSLR_type_specifier1Ri[1] = { "\'BOOL\'" };
char *SSLRParser::SSLR_type_specifier2Ri[1] = { "\'SHORT\'" };
char *SSLRParser::SSLR_type_specifier3Ri[1] = { "\'INT\'" };
char *SSLRParser::SSLR_type_specifier4Ri[1] = { "\'LONG\'" };
char *SSLRParser::SSLR_stmt_list0Ri[1] = { "stmt" };
char *SSLRParser::SSLR_stmt_list1Ri[2] = { "stmt_list", "stmt" };
char *SSLRParser::SSLR_stmt0Ri[4] = { "\'IDENT\'", "\'EQU\'", "expr", "\'SEMIKOLON\'" };
char *SSLRParser::SSLR_stmt1Ri[5] = { "\'DO\'", "stmt", "\'WHILE\'", "expr", "\'SEMIKOLON\'" };
char *SSLRParser::SSLR_stmt2Ri[3] = { "\'WHILE\'", "expr", "stmt" };
char *SSLRParser::SSLR_stmt3Ri[5] = { "\'IF_STMT\'", "expr", "stmt", "\'ELSE_STMT\'", "stmt" };
char *SSLRParser::SSLR_stmt4Ri[3] = { "\'IF_STMT\'", "expr", "stmt" };
char *SSLRParser::SSLR_stmt5Ri[1] = { "declarations" };
char *SSLRParser::SSLR_stmt6Ri[2] = { "expr", "\'SEMIKOLON\'" };
char *SSLRParser::SSLR_stmt7Ri[4] = { "\'BOPEN\'", "auxsym_1", "stmt_list", "\'BCLOSE\'" };
char *SSLRParser::SSLR_auxsym_10Ri[1] = { "\'EPSILON\'" };
char *SSLRParser::SSLR_expr0Ri[3] = { "expr", "\'LESS\'", "expr" };
char *SSLRParser::SSLR_expr1Ri[3] = { "expr", "\'GREATER\'", "expr" };
char *SSLRParser::SSLR_expr2Ri[3] = { "expr", "\'LESS_EQU\'", "expr" };
char *SSLRParser::SSLR_expr3Ri[3] = { "expr", "\'GREATER_EQU\'", "expr" };
char *SSLRParser::SSLR_expr4Ri[3] = { "expr", "\'EQUAL\'", "expr" };
char *SSLRParser::SSLR_expr5Ri[3] = { "expr", "\'NOTEQUAL\'", "expr" };
char *SSLRParser::SSLR_expr6Ri[3] = { "expr", "\'LOG_AND\'", "expr" };
char *SSLRParser::SSLR_expr7Ri[3] = { "expr", "\'LOG_OR\'", "expr" };
char *SSLRParser::SSLR_expr8Ri[2] = { "\'LOG_NOT\'", "expr" };
char *SSLRParser::SSLR_expr9Ri[3] = { "expr", "\'LEFT_SHIFT\'", "expr" };
char *SSLRParser::SSLR_expr10Ri[3] = { "expr", "\'RIGHT_SHIFT\'", "expr" };
char *SSLRParser::SSLR_expr11Ri[3] = { "expr", "\'BIN_AND\'", "expr" };
char *SSLRParser::SSLR_expr12Ri[3] = { "expr", "\'BIN_OR\'", "expr" };
char *SSLRParser::SSLR_expr13Ri[3] = { "expr", "\'BIN_XOR\'", "expr" };
char *SSLRParser::SSLR_expr14Ri[2] = { "\'BIN_NOT\'", "expr" };
char *SSLRParser::SSLR_expr15Ri[3] = { "expr", "\'ADD\'", "term" };
char *SSLRParser::SSLR_expr16Ri[3] = { "expr", "\'SUB\'", "term" };
char *SSLRParser::SSLR_expr17Ri[1] = { "term" };
char *SSLRParser::SSLR_term0Ri[3] = { "term", "\'MUL\'", "factor" };
char *SSLRParser::SSLR_term1Ri[3] = { "term", "\'DIV\'", "factor" };
char *SSLRParser::SSLR_term2Ri[3] = { "term", "\'MOD\'", "factor" };
char *SSLRParser::SSLR_term3Ri[1] = { "factor" };
char *SSLRParser::SSLR_factor0Ri[3] = { "\'KLOPEN\'", "expr", "\'KLCLOSE\'" };
char *SSLRParser::SSLR_factor1Ri[1] = { "\'NUMBER\'" };
char *SSLRParser::SSLR_factor2Ri[1] = { "\'TRUE\'" };
char *SSLRParser::SSLR_factor3Ri[1] = { "\'FALSE\'" };
char *SSLRParser::SSLR_factor4Ri[1] = { "\'IDENT\'" };


//all alternatives
SSLRParser::SSLR_NTERM_STR SSLRParser::SSLR_ntermStr[SSLRParser::SSLR_anzProd] = {
{ "start", SSLRParser::SSLR_start0Ri, start, 1, SSLRParser::SSLR_startFollow, 1 },
{ "program", SSLRParser::SSLR_program0Ri, program, 1, SSLRParser::SSLR_programFollow, 1 },
{ "definitions", SSLRParser::SSLR_definitions0Ri, definitions, 1, SSLRParser::SSLR_definitionsFollow, 17 },
{ "definitions", SSLRParser::SSLR_definitions1Ri, definitions, 2, SSLRParser::SSLR_definitionsFollow, 17 },
{ "definition", SSLRParser::SSLR_definition0Ri, definition, 1, SSLRParser::SSLR_definitionFollow, 17 },
{ "declarations", SSLRParser::SSLR_declarations0Ri, declarations, 2, SSLRParser::SSLR_declarationsFollow, 19 },
{ "declarations", SSLRParser::SSLR_declarations1Ri, declarations, 1, SSLRParser::SSLR_declarationsFollow, 19 },
{ "declaration", SSLRParser::SSLR_declaration0Ri, declaration, 3, SSLRParser::SSLR_declarationFollow, 19 },
{ "declarator_list", SSLRParser::SSLR_declarator_list0Ri, declarator_list, 1, SSLRParser::SSLR_declarator_listFollow, 2 },
{ "declarator_list", SSLRParser::SSLR_declarator_list1Ri, declarator_list, 3, SSLRParser::SSLR_declarator_listFollow, 2 },
{ "type_specifier", SSLRParser::SSLR_type_specifier0Ri, type_specifier, 1, SSLRParser::SSLR_type_specifierFollow, 1 },
{ "type_specifier", SSLRParser::SSLR_type_specifier1Ri, type_specifier, 1, SSLRParser::SSLR_type_specifierFollow, 1 },
{ "type_specifier", SSLRParser::SSLR_type_specifier2Ri, type_specifier, 1, SSLRParser::SSLR_type_specifierFollow, 1 },
{ "type_specifier", SSLRParser::SSLR_type_specifier3Ri, type_specifier, 1, SSLRParser::SSLR_type_specifierFollow, 1 },
{ "type_specifier", SSLRParser::SSLR_type_specifier4Ri, type_specifier, 1, SSLRParser::SSLR_type_specifierFollow, 1 },
{ "stmt_list", SSLRParser::SSLR_stmt_list0Ri, stmt_list, 1, SSLRParser::SSLR_stmt_listFollow, 18 },
{ "stmt_list", SSLRParser::SSLR_stmt_list1Ri, stmt_list, 2, SSLRParser::SSLR_stmt_listFollow, 18 },
{ "stmt", SSLRParser::SSLR_stmt0Ri, stmt, 4, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt1Ri, stmt, 5, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt2Ri, stmt, 3, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt3Ri, stmt, 5, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt4Ri, stmt, 3, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt5Ri, stmt, 1, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt6Ri, stmt, 2, SSLRParser::SSLR_stmtFollow, 19 },
{ "stmt", SSLRParser::SSLR_stmt7Ri, stmt, 4, SSLRParser::SSLR_stmtFollow, 19 },
{ "auxsym_1", SSLRParser::SSLR_auxsym_10Ri, auxsym_1, 0, SSLRParser::SSLR_auxsym_1Follow, 16 },
{ "expr", SSLRParser::SSLR_expr0Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr1Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr2Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr3Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr4Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr5Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr6Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr7Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr8Ri, expr, 2, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr9Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr10Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr11Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr12Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr13Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr14Ri, expr, 2, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr15Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr16Ri, expr, 3, SSLRParser::SSLR_exprFollow, 33 },
{ "expr", SSLRParser::SSLR_expr17Ri, expr, 1, SSLRParser::SSLR_exprFollow, 33 },
{ "term", SSLRParser::SSLR_term0Ri, term, 3, SSLRParser::SSLR_termFollow, 36 },
{ "term", SSLRParser::SSLR_term1Ri, term, 3, SSLRParser::SSLR_termFollow, 36 },
{ "term", SSLRParser::SSLR_term2Ri, term, 3, SSLRParser::SSLR_termFollow, 36 },
{ "term", SSLRParser::SSLR_term3Ri, term, 1, SSLRParser::SSLR_termFollow, 36 },
{ "factor", SSLRParser::SSLR_factor0Ri, factor, 3, SSLRParser::SSLR_factorFollow, 36 },
{ "factor", SSLRParser::SSLR_factor1Ri, factor, 1, SSLRParser::SSLR_factorFollow, 36 },
{ "factor", SSLRParser::SSLR_factor2Ri, factor, 1, SSLRParser::SSLR_factorFollow, 36 },
{ "factor", SSLRParser::SSLR_factor3Ri, factor, 1, SSLRParser::SSLR_factorFollow, 36 },
{ "factor", SSLRParser::SSLR_factor4Ri, factor, 1, SSLRParser::SSLR_factorFollow, 36 }};


#ifdef SSLR_DEBUG

//names of the terminals
char *SSLRParser::SSLR_termName[SSLRParser::SSLR_anzTerm] =
{ "SEMIKOLON", "LESS", "TRUE", "DIV", "LESS_EQU", "EQU", "DO", "BIN_OR", "KLCLOSE", "COMMA", "LEFT_SHIFT", "SHORT", "INT", "IF_STMT", "LOG_NOT", "FALSE", 
"BOPEN", "BIN_NOT", "LONG", "NUMBER", "MOD", "MUL", "LOG_OR", "BCLOSE", "BOOL", "RIGHT_SHIFT", "WHILE", "NOTEQUAL", "EQUAL", "SUB", "GREATER", "LOG_AND", 
"ELSE_STMT", "KLOPEN", "BIN_XOR", "BIN_AND", "GREATER_EQU", "IDENT", "ADD", "CHAR", "ACCEPT" };

#endif



void SSLRParser::SSLR_error( void )
{
	//error handling
	long	tmpState, tmpSp;
	int		rec = 0;

	//no more semantic actions allowed
	SSLR_semAct = false;

	//count error
	SSLR_anzError++;


	//default error message
	cerr << "line: " << SSLR_line << ": Syntax error!" << endl;

	//supress further error messages
	SSLR_anzSuppressError = SSLR_anzSuppressErrorMsg;

	//skip intern error recovery?
	if( rec )
		//yes, return
		return;

	//save current stack position
	tmpSp = SSLR_sp;

	//pop states from the parse stack until a valid transition is found
	do
	{
		//pop one state from the parse stack
		tmpState = SSLR_pop( 1 );

		//is there a transition?
		if( ( SSLR_sp > 0 ) && ( SSLR_lrTab[tmpState][SSLR_terminal].state == LR_SHIFT ) )
		{
			//we found a valid transition, consider recovered
			SSLR_state = tmpState;
			return;
		}

		//did we check the whole stack?
		else if( SSLR_sp < 1 )
		{
			//the whole stack has been visited without success
			SSLR_terminal = SSLR_getTerminal();

			//end of input, that is ACCEPT?
			if( SSLR_terminal == ACCEPT )
			{
				//yes, stop parsing
				SSLR_state = 1;
				return;
			}

			//set stack pointer
			SSLR_sp = tmpSp;
		}

	}while( 1 );
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

#ifdef SSLR_DEBUG
	//output
	cout << setw( SSLR_maxStateWidth ) << " state |"
	<< setw( SSLR_maxElemWidth ) << " stack |" << setw( SSLR_maxAltRightSideWidth )
	<< " action |" << setw( SSLR_maxElemWidth ) << " next |" << endl;

	//separator
	cout << setw( SSLR_maxStateWidth ) << setfill( '-' ) << "|"
	<< setw( SSLR_maxElemWidth ) << "|" << setw( SSLR_maxAltRightSideWidth )
	<< "|" << setw( SSLR_maxElemWidth ) << "|" << endl;

	//initialize
	cout << setw( SSLR_maxStateWidth ) << setfill( ' ' ) << " 0 |"
	<< setw( SSLR_maxElemWidth ) << "|" << setw( SSLR_maxAltRightSideWidth )
	<< "|" << setw( SSLR_maxElemWidth ) << "|" << endl;

	//set debug stack pointer
	SSLR_dbgSp = 0;
#endif

	//parse stack pointer
	SSLR_sp = 0;

	//start with state 0
	SSLR_push( 0 );
	SSLR_state = 0;

	//no errors yet
	SSLR_anzError = 0;
	SSLR_anzSuppressError = 0;

	//semantic actions
	SSLR_semAct = true;

	//stack counter for semantic action stack
	SSLR_semStackCnt = 0;
}


int SSLRParser::SSLR_getTerminal( void )
{
	//semantic actions allowed?
	if( SSLR_semAct == true )
	{
		//semantic stack overflow?
		if( SSLR_semStackCnt >= SSLR_maxStackSize )
		{
			//yes, print an error message
			cerr << "SSLRParser::SSLR_getTerminal: Semantic stack overflow!" << endl;

			//no more semantic actions allowed
			SSLR_semAct = false;
		}

		else
			//push attribute on stack for semantic actions
			SSLR_semStack[SSLR_semStackCnt++] = SSLR_attributeStr;
	}

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
		if( ( SSLR_state == 1 ) && ( SSLR_terminal == ACCEPT ) )
			//return number of errors
			return( SSLR_anzError );

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

	//reduce number of error messages to supress?
	if( SSLR_anzSuppressError )
		//yes, until it is zero
		SSLR_anzSuppressError--;

#ifdef SSLR_DEBUG
	//debug stack overflow?
	if( SSLR_dbgSp >= SSLR_maxStackSize )
	{
		//debug stack overflow
		cerr << "SSLRParser::SSLR_shift: Debug stack overflow!" << endl;

		//exit
		exit( 1 );
	}

	//set debug stack
	SSLR_dbgStack[SSLR_dbgSp].elem = true;
	SSLR_dbgStack[SSLR_dbgSp++].index = SSLR_terminal;

	//output
	SSLR_outputDbgStack( SSLR_state, LR_SHIFT, SSLR_terminal, SSLR_terminal );
#endif

	//shift a new state onto the stack
	SSLR_push( tab->value );
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
	//pop elements from debug stack
	SSLR_dbgSp -= n->elem;

	//debug stack overflow?
	if( SSLR_dbgSp >= SSLR_maxStackSize )
	{
		//debug stack overflow
		cerr << "SSLRParser::SSLR_reduce: Debug stack overflow!" << endl;

		//exit
		exit( 1 );
	}

	//set debug stack
	SSLR_dbgStack[SSLR_dbgSp].elem = false;
	SSLR_dbgStack[SSLR_dbgSp++].index = tab->value;

	//output
	SSLR_outputDbgStack( SSLR_state, LR_REDUCE, tab->value, SSLR_terminal );
#endif

	//remove states form stack
	SSLR_state = SSLR_pop( n->elem );

	//get new state
	SSLR_state = SSLR_jumpTab[SSLR_state][n->jump_index];

	//push new state on stack
	SSLR_push( SSLR_state );
}


#ifdef SSLR_DEBUG
void SSLRParser::SSLR_outputDbgStack( int state, int action, int index, int next )
{
	//print the debugging stack
	char	statePuf[SSLR_maxStateWidth + 1];
	char	stackPuf[SSLR_maxElemWidth + 1];
	char	actionPuf[SSLR_maxAltRightSideWidth + 1];
	char	nextPuf[SSLR_maxElemWidth + 1];
	int		stackIndex, i;

	//curent state in string
	sprintf( statePuf, "%d", SSLR_state );

	//print first element
	stackIndex = SSLR_dbgStack[0].index;

	//terminal?
	if( SSLR_dbgStack[0].elem == true )
		//terminal
		sprintf( stackPuf, "%s", SSLR_termName[stackIndex] );

	else
		//is nonterminal
		sprintf( stackPuf, "%s", SSLR_ntermStr[stackIndex].name );

	//action
	if( action == LR_SHIFT )
	{
		//shift state
		sprintf( actionPuf, "push %s", SSLR_termName[index] );


		//next is empty
		sprintf( nextPuf, "%s", " " );
	}

	else
	{
		SSLR_NTERM_STR	*str = &SSLR_ntermStr[index];
		int				i;

		//reduce with alternative
		sprintf( actionPuf, "reduce with " );

		//print name of alternative
		strcat( actionPuf, str->name );
		strcat( actionPuf, " -> " );

		//print all elements on the right side
		for( i = 0; i < str->elem; i++ )
		{
			//print element
			strcat( actionPuf, str->ri[i] );

			//more elements?
			if( i < ( str->elem - 1 ) )
				//yes
				strcat( actionPuf, " " );
		}

		//alternatives with zero elements have EPSILON on the right side
		if( !str->elem )
			//print EPSILON
			strcat( actionPuf, str->ri[0] );

		//next is the current terminal
		sprintf( nextPuf, "%s", SSLR_termName[next] );
	}

	//separator
	cout << setw( SSLR_maxStateWidth ) << setfill( '-' ) << "|"
	<< setw( SSLR_maxElemWidth ) << "|" << setw( SSLR_maxAltRightSideWidth )
	<< "|" << setw( SSLR_maxElemWidth ) << "|" << endl;

	//initialize
	cout << setw( SSLR_maxStateWidth ) << setfill( ' ' )
	<< ( string( statePuf ) + " |" )
	<< setw( SSLR_maxElemWidth ) << ( string( stackPuf ) + " |" )
	<< setw( SSLR_maxAltRightSideWidth ) << ( string( actionPuf ) + " |" )
	<< setw( SSLR_maxElemWidth ) << ( string( nextPuf ) + " |" ) << endl;



	//print all other debug stack entries
	for( i = 1; i < SSLR_dbgSp; i++ )
	{
		//print element
		stackIndex = SSLR_dbgStack[i].index;

		//terminal?
		if( SSLR_dbgStack[i].elem == true )
			//terminal
			sprintf( stackPuf, "%s", SSLR_termName[stackIndex] );

		else
			//is nonterminal
			sprintf( stackPuf, "%s", SSLR_ntermStr[stackIndex].name );

		cout << setw( SSLR_maxStateWidth ) << setfill( ' ' )
		<< "|" << setw( SSLR_maxElemWidth ) << ( string( stackPuf ) + " |" )
		<< setw( SSLR_maxAltRightSideWidth ) << "|"
		<< setw( SSLR_maxElemWidth ) << "|" << endl;
	}
}
#endif


void SSLRParser::SSLR_doSemAct( SSLR_LR_TAB *tab )
{
	//do semantic actions before reduction
	SSLR_NTERM_STR	*n;
	SSLR_TYPE		tmp;

	//semantic actions allowed?
	if( SSLR_semAct == true )
	{
		//get a pointer to the alternative
		n = &SSLR_ntermStr[tab->value];

		//Alternative to reduce with
		switch( tab->value )
		{
			// definition ->  stmt_list .
			case definition_0:
			{
				
	//Codeerzeugung aufrufen
	codeBase->print( SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._syntaxTreeIndex );
				break;
			}

			// declarator_list ->  'IDENT' .
			case declarator_list_0:
			{
				
	/*
		Eine Vereinbarung treffen, z.B. eine Variable.
		Der Typ steht in stType.
	*/

	//in Symboltabelle eintragen
	tmp._symTabIndex = symTab->insert( SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._name, blkNum, SSLR_line );

	//schon eingetragen?
	if( tmp._symTabIndex == -1 )
	{
		//ja, Redeklaration
		cerr << "miniC: Line " << SSLR_line << ": " << SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._name << " already declared!" << endl;
	}

	else
	{
		//Typ eintragen
		symTab->setType( tmp._symTabIndex, CSymbolTable::_VAR );

		//Storage type eintragen
		symTab->setStorageType( tmp._symTabIndex, stType );

		//den Offset eintragen
		symTab->setOffset( tmp._symTabIndex, localVariableOffsetCnt );

		//für die nächste Variable anpassen
		adjustVarOffset( stType );
	}
				break;
			}

			// declarator_list ->  declarator_list  'COMMA'  'IDENT' .
			case declarator_list_1:
			{
				
	/*
		Eine Vereinbarung treffen, z.B. eine Variable.
		Der Typ steht in stType.
	*/

	//in Symboltabelle eintragen
	tmp._symTabIndex = symTab->insert( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._name, blkNum, SSLR_line );

	//schon eingetragen?
	if( tmp._symTabIndex == -1 )
	{
		//ja, Redeklaration
		cerr << "miniC: Line " << SSLR_line << ": " << SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._name << " already declared!" << endl;
	}

	else
	{
		//Typ eintragen
		symTab->setType( tmp._symTabIndex, CSymbolTable::_VAR );

		//Storage type eintragen
		symTab->setStorageType( tmp._symTabIndex, stType );

		//den Offset eintragen
		symTab->setOffset( tmp._symTabIndex, localVariableOffsetCnt );

		//für die nächste Variable anpassen
		adjustVarOffset( stType );
	}
				break;
			}

			// type_specifier ->  'CHAR' .
			case type_specifier_0:
			{
				
	//zu deklarierende Bezeichner sind vom Typ char
	stType = CSymbolTable::_CHAR;
				break;
			}

			// type_specifier ->  'BOOL' .
			case type_specifier_1:
			{
				
	//zu deklarierende Bezeichner sind vom Typ bool
	stType = CSymbolTable::_BOOL;
				break;
			}

			// type_specifier ->  'SHORT' .
			case type_specifier_2:
			{
				
	//zu deklarierende Bezeichner sind vom Typ short
	stType = CSymbolTable::_SHORT;
				break;
			}

			// type_specifier ->  'INT' .
			case type_specifier_3:
			{
				
	//zu deklarierende Bezeichner sind vom Typ int
	stType = CSymbolTable::_INT;
				break;
			}

			// type_specifier ->  'LONG' .
			case type_specifier_4:
			{
				
	//zu deklarierende Bezeichner sind vom Typ long
	stType = CSymbolTable::_LONG;
				break;
			}

			// stmt_list ->  stmt .
			case stmt_list_0:
			{
				
	long	index = SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._syntaxTreeIndex;

	//Wurzel des Syntaxbaumes für das statement hier ablegen?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._syntaxTreeIndex != -1 )
	{
		//Syntaxbaum ausgeben
//		syntaxTree->printSyntaxTree( $1 );

		//merken
		tmp._syntaxTreeIndex = index;
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;







	cout << "stmt:" << endl;
	cout << "tmp._syntaxTreeIndex: " << tmp._syntaxTreeIndex << endl;
	cout << "1: " << SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._syntaxTreeIndex << endl;
	cout << endl;
				break;
			}

			// stmt_list ->  stmt_list  stmt .
			case stmt_list_1:
			{
				
	long	index1 = SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (1)]._syntaxTreeIndex, index2 = SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex;

	//Wurzel des Syntaxbaumes für das statement hier ablegen?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex != -1 ) )
	{
		//Statements verketten
		syntaxTree->linkStmt( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex );

		//Syntaxbaum ausgeben
//		syntaxTree->printSyntaxTree( $2 );

		//merken
		tmp._syntaxTreeIndex = SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex;
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;







	cout << "stmt_list stmt:" << endl;
	cout << "tmp._syntaxTreeIndex: " << tmp._syntaxTreeIndex << endl;
	cout << "1: " << SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (1)]._syntaxTreeIndex << endl;
	cout << "2: " << SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex << endl;
	cout << endl;
				break;
			}

			// stmt ->  'IDENT'  'EQU'  expr  'SEMIKOLON' .
			case stmt_0:
			{
				
	//$1 = $3;
	CSymbolTable::varType	varType;

	//der Identifier muss hier bereits deklariert sein
	long index = symTab->find( SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (1)]._name );

	/*
		Auch Zahlen werden in die Symboltabelle eingetragen.
		Zuweisungen können NUR an Variablen vorgenommen werden, nicht
		Zahlen.
	*/

	//Typ holen
	varType = symTab->getType( index );

	//nicht Variable?
	if( varType != CSymbolTable::_VAR )
	{
		//Zuweisung nicht an Variable!
		cerr << "miniC: Line " << SSLR_line << ": " << SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (1)]._name << " not an lvalue!" << endl;

		//Zustand merken
		tmp._syntaxTreeIndex = -1;
	}

	//Variable nicht deklariert?
	else if( index < 0 )
	{
		//nein, Fehler
		cerr << "miniC: Line " << SSLR_line << ": " << SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (1)]._name << " undeclared!" << endl;

		//Zustand merken
		tmp._syntaxTreeIndex = -1;
	}

	else if( SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;

		//ein Leaf für den LValue erzeugen
		long lval = syntaxTree->makeLeaf( index, -1 );

		//Knoten für Zuweisung erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_EQU, -1, lval, SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( lval, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		/*
			Wenn auf lokale Variablen ausserhalb des aktuellen Blocks zugegriffen wird,
			dann muss man sich anhand der link-Befehle zum Stackframe "hangeln", in dem
			die Vereinbarung getroffen wurde.
		*/

		//die Blocktiefe der Vereinbarung holen
		int	localVarBlkNum = symTab->getBlkDepth( index );

		//Differenz eintragen
		syntaxTree->setBlkDiff( lval, blkNum - localVarBlkNum );



		//den Datentyp der Vereinbarung der Zuweisung holen
		lt = symTab->getStorageType( index );

		//den Datentyp ins Leaf eintragen
		syntaxTree->setStorageType( lval, lt );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex );

		//die rechte Seite muss auf den Typ der linken Seite gecastet werden
		if( forceCast( lt, rt, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
				Es iwrd IMMER das rechte King gecastet!
			*/
			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Possible loss of information in assignment!" << endl;

			//das rechte Kind casten, Cast-Knoten erzeugen
			castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex );

			//das rechte Kind des =-Knoten korrigieren
			syntaxTree->setChildren( tmp._syntaxTreeIndex, lval, castNode );

			//dem Cast-Knoten IMMER den Datentyp der Zuweisung zuweisen
			syntaxTree->setStorageType( castNode, lt );

			//Parent des rechten Kindes ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex, castNode );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem =-Knoten IMMER einen Datentyp der Zuweisung zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, lt );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;







	cout << "'IDENT' 'EQU' expr 'SEMIKOLON': " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// stmt ->  'DO'  stmt  'WHILE'  expr  'SEMIKOLON' .
			case stmt_1:
			{
				
	//Fehler?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						l1, l2, castNode = SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (4)]._syntaxTreeIndex;

		//Knoten für DO-WHILE-Statement erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_DOWHILE, -1, -1, -1 );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//die expr-Anweisung in einem while-Statement muss bool als Ergebnis zurückliefern
		lt = CSymbolTable::_BOOL;

		//Datentyp des expr-Teils holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (4)]._syntaxTreeIndex );

		//die expr-Anweisung muss auf BOOL gecastet werden?
		if( forceCast( lt, rt, op ) == true )
		{
			//expr-Anweisung auf BOOL-Datentyp casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Expression in while statement must be of type boolean!" << endl;

			//expr casten
			castNode = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (4)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( castNode, CSymbolTable::_BOOL );

			//Parent von expr ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (4)]._syntaxTreeIndex, castNode );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}


		//der expr-Teil hat als parent den WHILE-Node
		syntaxTree->setParent( castNode, tmp._syntaxTreeIndex );

		//Sprung-Ziel vor dem stmt-Teil
		l1 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l1 );

		//Sprung-Ziel hinter der Schleife
		l2 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l2 );

		//Informationen eintragen
		syntaxTree->setIfStmt( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex, -1, l1, l2 );
	}

	else
		//keine Code-Erzeugung
		tmp._syntaxTreeIndex = -1;







	cout << "'DO' stmt 'WHILE' expr 'SEMIKOLON': " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// stmt ->  'WHILE'  expr  stmt .
			case stmt_2:
			{
				
	//Fehler?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						l1, l2, castNode = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex;

		//Knoten für WHILE-Statement erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_WHILE, -1, -1, -1 );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//die expr-Anweisung in einem while-Statement muss bool als Ergebnis zurückliefern
		lt = CSymbolTable::_BOOL;

		//Datentyp des expr-Teils holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex );

		//die expr-Anweisung muss auf BOOL gecastet werden?
		if( forceCast( lt, rt, op ) == true )
		{
			//expr-Anweisung auf BOOL-Datentyp casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Expression in while statement must be of type boolean!" << endl;

			//expr casten
			castNode = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( castNode, CSymbolTable::_BOOL );

			//Parent von expr ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex, castNode );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}


		//der expr-Teil hat als parent den WHILE-Node
		syntaxTree->setParent( castNode, tmp._syntaxTreeIndex );

		//Sprung-Ziel vor dem expr-Teil
		l1 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l1 );

		//Sprung-Ziel hinter dem stmt-Teil
		l2 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l2 );

		//Informationen eintragen
		syntaxTree->setIfStmt( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, -1, l1, l2 );
	}

	else
		//keine Code-Erzeugung
		tmp._syntaxTreeIndex = -1;







	cout << "'WHILE' expr stmt: " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// stmt ->  'IF_STMT'  expr  stmt  'ELSE_STMT'  stmt .
			case stmt_3:
			{
				
	//Fehler?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						l1, l2, castNode = SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex;

		//Knoten für IF-Statement erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_IF, -1, -1, -1 );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//die expr-Anweisung in einem if-Statement muss bool als Ergebnis zurückliefern
		lt = CSymbolTable::_BOOL;

		//Datentyp des expr-Teils holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex );

		//die expr-Anweisung muss auf BOOL gecastet werden?
		if( forceCast( lt, rt, op ) == true )
		{
			//expr-Anweisung auf BOOL-Datentyp casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Expression in if statement must be of type boolean!" << endl;

			//expr casten
			castNode = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( castNode, CSymbolTable::_BOOL );

			//Parent von expr ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (2)]._syntaxTreeIndex, castNode );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}


		//der expr-Teil hat als parent den IF-Node
		syntaxTree->setParent( castNode, tmp._syntaxTreeIndex );

		//Sprung-Ziel vor dem stmt2-Teil
		l1 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l1 );

		//Sprung-Ziel hinter dem stmt2-Teil
		l2 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l2 );

		//Informationen eintragen
		syntaxTree->setIfStmt( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (3)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 5 + (5)]._syntaxTreeIndex, l1, l2 );
	}

	else
		//keine Code-Erzeugung
		tmp._syntaxTreeIndex = -1;







	cout << "'IF_STMT' expr stmt 'ELSE_STMT' stmt: " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// stmt ->  'IF_STMT'  expr  stmt .
			case stmt_4:
			{
				
	//Fehler?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						l1, castNode = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex;

		//Knoten für IF-Statement erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_IF, -1, -1, -1 );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//die expr-Anweisung in einem if-Statement muss bool als Ergebnis zurückliefern
		lt = CSymbolTable::_BOOL;

		//Datentyp des expr-Teils holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex );

		//die expr-Anweisung muss auf BOOL gecastet werden?
		if( forceCast( lt, rt, op ) == true )
		{
			//expr-Anweisung auf BOOL-Datentyp casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Expression in if statement must be of type boolean!" << endl;

			//expr casten
			castNode = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( castNode, CSymbolTable::_BOOL );

			//Parent von expr ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex, castNode );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}


		//der expr-Teil hat als parent den IF-Node
		syntaxTree->setParent( castNode, tmp._syntaxTreeIndex );

		//Sprung-Ziel hinter dem stmt1-Teil
		l1 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l1 );

		//Informationen eintragen
		syntaxTree->setIfStmt( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, -1, -1, l1 );
	}

	else
		//keine Code-Erzeugung
		tmp._syntaxTreeIndex = -1;







	cout << "'IF_STMT' expr stmt: " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// stmt ->  declarations .
			case stmt_5:
			{
				
	/*
		Damit Deklarationen an beliebiger Stelle in einem compound statement
		stehen können, müssen auch für Deklarationen Syntaxbaum-Nodes
		angelegt werden, die aber nur für die Aufrechterhaltung der
		Verkettung der Syntaxbäume angelegt werden. Das Problem:

		Da diese Alternative in der stmt-Produktion auftritt, tauchen
		diese in
		stmt_list	-> stmt_list stmt
		auf der rechten Seite auf. Aber genau hier wird die Verkettung
		der Statements durchgeführt, also muss bei $2 ein Syntaxbaum-Node
		stehen.
	*/

	//Knoten für die Vekettung der Statements erzeugen
	tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_DUMMY_STMT, -1, -1, -1 );




	cout << "declarations: " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// stmt ->  expr  'SEMIKOLON' .
			case stmt_6:
			{
				
	//Knoten für die Vekettung der Statements erzeugen
	tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_DUMMY_STMT, -1, -1, -1 );

	//Warnung ausgeben
	cerr << "miniC: Line " << SSLR_line << ": Expression has no effect!" << endl;
				break;
			}

			// stmt ->  'BOPEN'  auxsym_1  stmt_list  'BCLOSE' .
			case stmt_7:
			{
				
	/*
		Ein Block kann lokale Variablen haben, die auf dem Laufzeit-Stack abgelegt werden.
		Diese müssen am Ende des Blocks gelöscht werden.
	*/

	/*
		Die Statements sind i.A. verkettet. In die Wurzel von _BLK_STMT wird
		die Wurzel, d.h. das erste Statement in der Verkettung eingetragen.
	*/
	long root = syntaxTree->getRootStmt( SSLR_semStack[SSLR_semStackCnt - 1 - 4 + (3)]._syntaxTreeIndex );

	//Knoten für einen Block erzeugen
	tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_BLK_STMT, -1, -1, root );

	//Informationen eintragen
	syntaxTree->setLocalVar( tmp._syntaxTreeIndex, localVariableOffsetCnt );


	/* Verschachtelungstiefe - 1*/
	symTab->decBlockDepth();
	blkNum--;

	//den Offset für die lokalen Variablen zurückholen
	popVarOffsetStack();







	cout << "'BOPEN' stmt_list 'BCLOSE': " << tmp._syntaxTreeIndex << endl;
				break;
			}

			// auxsym_1 ->  'EPSILON' .
			case auxsym_1_0:
			{
				

	/* Verschachtelungstiefe + 1 */
	symTab->incBlockDepth();
	blkNum++;

	//den Offset für die lokalen Variablen merken
	pushVarOffsetStack();

	//zurücksetzen
	localVariableOffsetCnt = 0;

				break;
			}

			// expr ->  expr  'LESS'  expr .
			case expr_0:
			{
				
	//$1 < $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für "kleiner" erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_LT, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//der <-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'GREATER'  expr .
			case expr_1:
			{
				
	//$1 > $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für "größer" erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_GT, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//der <-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'LESS_EQU'  expr .
			case expr_2:
			{
				
	//$1 <= $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für "kleiner gleich" erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_LE, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//der <-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'GREATER_EQU'  expr .
			case expr_3:
			{
				
	//$1 >= $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für "größer gleich" erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_GE, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//der <-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'EQUAL'  expr .
			case expr_4:
			{
				
	//$1 == $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für "gleich" erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_EQ, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//der ==-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'NOTEQUAL'  expr .
			case expr_5:
			{
				
	//$1 != $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für "ungleich" erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_NEQ, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//der !=-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'LOG_AND'  expr .
			case expr_6:
			{
				
	//logisches und, &&

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						left = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, right = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, l1;

		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//$1 muss BOLL sein
		if( forceCast( CSymbolTable::_BOOL, lt, op ) == true )
		{
			//$1 auf BOOL casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Left expression in == must be of type boolean!" << endl;

			//$1 auf BOOL casten
			left = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( left, CSymbolTable::_BOOL );

			//Parent von $1 ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, left );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( left, SSLR_line  );
		}

		//$3 muss BOLL sein
		if( forceCast( CSymbolTable::_BOOL, rt, op ) == true )
		{
			//$1 auf BOOL casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Right expression in == must be of type boolean!" << endl;

			//$3 auf BOOL casten
			right = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( right, CSymbolTable::_BOOL );

			//Parent von $3 ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, right );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( right, SSLR_line  );
		}

		//Knoten für && erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_LOG_AND, -1, left, right );

		//Eltern eintragen
		syntaxTree->setParent( left, tmp._syntaxTreeIndex );
		syntaxTree->setParent( right, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );

		//der &&-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );


		//Sprung-Ziel generieren
		l1 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l1 );

		//das Sprungziel wird in die _ifStr-Struktur eingetragen
		syntaxTree->setIfStmt( tmp._syntaxTreeIndex, -1, -1, -1, l1, -1 );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'LOG_OR'  expr .
			case expr_7:
			{
				
	//logisches oder, ||

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						left = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, right = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, l1, l2;

		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//$1 muss BOLL sein
		if( forceCast( CSymbolTable::_BOOL, lt, op ) == true )
		{
			//$1 auf BOOL casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Left expression in == must be of type boolean!" << endl;

			//$1 auf BOOL casten
			left = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( left, CSymbolTable::_BOOL );

			//Parent von $1 ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, left );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( left, SSLR_line  );
		}

		//$3 muss BOLL sein
		if( forceCast( CSymbolTable::_BOOL, rt, op ) == true )
		{
			//$1 auf BOOL casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Right expression in == must be of type boolean!" << endl;

			//$3 auf BOOL casten
			right = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( right, CSymbolTable::_BOOL );

			//Parent von $3 ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, right );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( right, SSLR_line  );
		}

		//Knoten für || erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_LOG_OR, -1, left, right );

		//Eltern eintragen
		syntaxTree->setParent( left, tmp._syntaxTreeIndex );
		syntaxTree->setParent( right, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );

		//der ||-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );


		//Sprung-Ziel generieren
		l1 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l1 );

		//Sprung-Ziel generieren
		l2 = syntaxTree->makeNode( CSyntaxTree::_LABEL, -1, -1, -1 );
		syntaxTree->genJumpLabel( l2 );

		//das Sprungziel wird in die _ifStr-Struktur eingetragen
		syntaxTree->setIfStmt( tmp._syntaxTreeIndex, -1, -1, -1, l1, l2 );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  'LOG_NOT'  expr .
			case expr_8:
			{
				
	//logisches nicht, !

	//Fehler?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		long						left = SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex;

		//Datentyp von $2 holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex );

		//$2 muss BOLL sein
		if( forceCast( CSymbolTable::_BOOL, lt, op ) == true )
		{
			//$2 auf BOOL casten

			//Warnung ausgeben, da bei einem Zuweisungscast meist Informationen verloren gehen
			cerr << "miniC: Line " << SSLR_line << ": Expression in ! must be of type boolean!" << endl;

			//$2 auf BOOL casten
			left = syntaxTree->makeNode( op, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex );

			//Ergebnis muss BOOL sein
			syntaxTree->setStorageType( left, CSymbolTable::_BOOL );

			//Parent von $2 ist der Cast-Knoten
			syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex, left );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( left, SSLR_line  );
		}

		//Knoten für ! erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_LOG_NOT, -1, -1, left );

		//Eltern eintragen
		syntaxTree->setParent( left, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );

		//der !-Knoten ist immer vom Datentyp BOOL
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'LEFT_SHIFT'  expr .
			case expr_9:
			{
				
	//links shift, <<

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für << erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_LEFT_SHIFT, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//dem <<-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, lt );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'RIGHT_SHIFT'  expr .
			case expr_10:
			{
				
	//rechts shift, >>

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für >> erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_RIGHT_SHIFT, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//dem >>-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, lt );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'BIN_AND'  expr .
			case expr_11:
			{
				
	//binäres und, &

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für & erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_BIN_AND, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem &-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'BIN_OR'  expr .
			case expr_12:
			{
				
	//binäres oder, |

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für | erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_BIN_OR, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem &-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'BIN_XOR'  expr .
			case expr_13:
			{
				
	//binäres x-oder, ^

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für ^ erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_BIN_XOR, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem &-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  'BIN_NOT'  expr .
			case expr_14:
			{
				
	//binäres nicht, ~

	//Fehler?
	if( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex != -1 )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für ~ erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_BIN_NOT, -1, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 2 + (2)]._syntaxTreeIndex );

		//dem &-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, lt );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'ADD'  term .
			case expr_15:
			{
				
	//$1 * $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für Addition erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_ADD, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem +-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  expr  'SUB'  term .
			case expr_16:
			{
				
	//$1 - $3

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für Subtraktion erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_SUB, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem --Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// expr ->  term .
			case expr_17:
			{
				
	//weitergeben
	tmp._syntaxTreeIndex = SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._syntaxTreeIndex;
				break;
			}

			// term ->  term  'MUL'  factor .
			case term_0:
			{
				
	//Multiplikation

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für Multiplikation erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_MUL, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem *-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// term ->  term  'DIV'  factor .
			case term_1:
			{
				
	//Division

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		CSymbolTable::storageType	lt, rt, type;
		CSyntaxTree::STOperator		op;
		int							which;

		//Knoten für Division erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_DIV, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );



		//Datentyp des linken Knotens holen
		lt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

		//Datentyp des rechten Knotens holen
		rt = syntaxTree->getStorageType( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//casten?
		if( cast( lt, rt, type, which, op ) == true )
		{
			long	castNode;

			/*
				Die mit beiden Kindern verbundene Typen sind inkompatibel, es muss eine
				implizite Typkonvertierung vorgenommen werden.
			*/

			//linkes Kind casten?
			if( which == LEFT )
			{
				//ja, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, castNode, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//Parent des rechten Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );
			}

			else
			{
				//das rechte Kind casten, Cast-Knoten erzeugen
				castNode = syntaxTree->makeNode( op, tmp._syntaxTreeIndex, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

				//das rechte Kind des *-Knoten korrigieren
				syntaxTree->setChildren( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, castNode );

				//Parent des linken Kindes ist der Cast-Knoten
				syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, castNode );
			}

			//dem Cast-Knoten einen Datentyp zuweisen
			syntaxTree->setStorageType( castNode, type );

			//Zeilennummer in den Knoten eintragen
			syntaxTree->setLine( castNode, SSLR_line  );
		}

		//dem /-Knoten einen Datentyp zuweisen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// term ->  term  'MOD'  factor .
			case term_2:
			{
				
	//Modulo

	//Fehler?
	if( ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex != -1 ) && ( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex != -1 ) )
	{
		//Knoten für Modulo erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeNode( CSyntaxTree::_MOD, -1, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex );

		//Eltern eintragen
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (1)]._syntaxTreeIndex, tmp._syntaxTreeIndex );
		syntaxTree->setParent( SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (3)]._syntaxTreeIndex, tmp._syntaxTreeIndex );

		//Zeilennummer in den Knoten eintragen
		syntaxTree->setLine( tmp._syntaxTreeIndex, SSLR_line  );
	}

	else
		//Fehler
		tmp._syntaxTreeIndex = -1;
				break;
			}

			// term ->  factor .
			case term_3:
			{
				
	//weitergeben
	tmp._syntaxTreeIndex = SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._syntaxTreeIndex;
				break;
			}

			// factor ->  'KLOPEN'  expr  'KLCLOSE' .
			case factor_0:
			{
				
	//weitergeben
	tmp._syntaxTreeIndex = SSLR_semStack[SSLR_semStackCnt - 1 - 3 + (2)]._syntaxTreeIndex;
				break;
			}

			// factor ->  'NUMBER' .
			case factor_1:
			{
				
	//in Symboltabelle eintragen
	long symTabIndex = symTab->insert( 0L, blkNum, SSLR_line );

	//Typ eintragen
	symTab->setType( symTabIndex, CSymbolTable::_NUMBER );

	//Storage type eintragen
	symTab->setStorageType( symTabIndex, CSymbolTable::_LONG );


	//ein LEAF erzeugen
	tmp._syntaxTreeIndex = syntaxTree->makeLeaf( symTabIndex, -1 );

	//Zahlenwert eintragen
	syntaxTree->setNumber( tmp._syntaxTreeIndex, SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._integer );

	//den Datentyp ins Leaf eintragen
	syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_LONG );
				break;
			}

			// factor ->  'TRUE' .
			case factor_2:
			{
				
	//in Symboltabelle eintragen
	long symTabIndex = symTab->insert( 0L, blkNum, SSLR_line );

	/*
		Der Datentyp bool wird intern als Byte dargestellt, der 0 oder 1
		sein kann. Also ist true eine Zahl mit Zahlenwert +1.
	*/
	symTab->setType( symTabIndex, CSymbolTable::_NUMBER );

	//Storage type eintragen
	symTab->setStorageType( symTabIndex, CSymbolTable::_BOOL );


	//ein LEAF erzeugen
	tmp._syntaxTreeIndex = syntaxTree->makeLeaf( symTabIndex, -1 );

	//Zahlenwert eintragen
	syntaxTree->setNumber( tmp._syntaxTreeIndex, 1 );

	//den Datentyp ins Leaf eintragen
	syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
				break;
			}

			// factor ->  'FALSE' .
			case factor_3:
			{
				
	//in Symboltabelle eintragen
	long symTabIndex = symTab->insert( 0L, blkNum, SSLR_line );

	/*
		Der Datentyp bool wird intern als Byte dargestellt, der 0 oder 1
		sein kann. Also ist false eine Zahl mit Zahlenwert 0.
	*/
	symTab->setType( symTabIndex, CSymbolTable::_NUMBER );

	//Storage type eintragen
	symTab->setStorageType( symTabIndex, CSymbolTable::_BOOL );


	//ein LEAF erzeugen
	tmp._syntaxTreeIndex = syntaxTree->makeLeaf( symTabIndex, -1 );

	//Zahlenwert eintragen
	syntaxTree->setNumber( tmp._syntaxTreeIndex, 0 );

	//den Datentyp ins Leaf eintragen
	syntaxTree->setStorageType( tmp._syntaxTreeIndex, CSymbolTable::_BOOL );
				break;
			}

			// factor ->  'IDENT' .
			case factor_4:
			{
				
	//der Identifier muss hier bereits deklariert sein
	long index = symTab->find( SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._name );

	//Variable nicht deklariert?
	if( index < 0 )
	{
		//nein, Fehler
		cerr << "miniC: Line " << SSLR_line << ": " << SSLR_semStack[SSLR_semStackCnt - 1 - 1 + (1)]._name << " undeclared!" << endl;

		//Zustand merken
		tmp._syntaxTreeIndex = -1;
	}

	else
	{
		CSymbolTable::storageType	type;

		//ein Leaf erzeugen
		tmp._syntaxTreeIndex = syntaxTree->makeLeaf( index, -1 );

		//den Datentyp der Vereinbarung holen
		type = symTab->getStorageType( index );

		//den Datentyp ins Leaf eintragen
		syntaxTree->setStorageType( tmp._syntaxTreeIndex, type );

		//Vereinbarung ist eine Variable
		symTab->setType( index, CSymbolTable::_VAR );

		/*
			Wenn auf lokale Variablen ausserhalb des aktuellen Blocks zugegriffen wird,
			dann muss man sich anhand der link-Befehle zum Stackframe "hangeln", in dem
			die Vereinbarung getroffen wurde.
		*/

		//die Blocktiefe der Vereinbarung holen
		int	localVarBlkNum = symTab->getBlkDepth( index );

		//Differenz eintragen
		syntaxTree->setBlkDiff( tmp._syntaxTreeIndex, blkNum - localVarBlkNum );
	}
				break;
			}

//			default:
				//error
//				cerr << "SSLRParser::SSLR_doSemAct: Unknown alternative " << tab->value << "!" << endl;
		}

		//correct stack pointer
		SSLR_semStackCnt -= n->elem;

		//semantic stack overflow?
		if( SSLR_semStackCnt >= SSLR_maxStackSize )
		{
			//yes, print an error message
			cerr << "SSLRParser::SSLR_doSemAct: Semantic stack overflow!" << endl;

			//no more semantic actions allowed
			SSLR_semAct = false;
		}

		else
			//push last value on stack
			SSLR_semStack[SSLR_semStackCnt++] = tmp;
	}
}



void SSLRParser::init( void )
{
	//initialisieren

	//Verschachtelungstiefe für globale Vereinbarungen setzen
	blkNum = 0;

	//Symboltabelle
	symTab = 0L;

	//Syntax-Baum
	syntaxTree = 0L;

	//für Variablen auf Laufzeit-Stack
	varOffsetStack = 0L;
	varOffsetStackPos = 0;
	varOffsetStackMaxPos = 0;
	localVariableOffsetCnt = 0;

	//Array mit den zu korrigierenden Offsets für die lokalen Variablen
	localVarStr.clear();
}


void SSLRParser::deinit( void )
{
	//deinitialisieren

	//Speicher freigeben?
	if( varOffsetStack )
		delete[] ( varOffsetStack );
}


void SSLRParser::giveArgs( ARGV_STR *_argvStr, CSymbolTable *_symTab, CSyntaxTree *_syntaxTree, CCodeBase *_codeBase )
{
	//dem Parser einige Variablen übergeben

	//Zeiger auf die Symboltabelle
	symTab = _symTab;

	//Zeiger auf den Syntaxbaum
	syntaxTree = _syntaxTree;

	//Zeiger auf die Codeerzeugung
	codeBase = _codeBase;

	//Command line arguments
	argvStr = _argvStr;
}


void SSLRParser::popVarOffsetStack( void )
{
	//Offset für lokale Variablen vom Stack holen

	//aktuelle Größe der lokalen Variablen des aktuellen Blocks zurückholen
	localVariableOffsetCnt = localVarStr.back();

	//letzen Eintrag popen
	localVarStr.pop_back();
}


void SSLRParser::pushVarOffsetStack( void )
{
	//Offset für lokale Variablen merken

	//auf Stack schieben
	localVarStr.push_back( localVariableOffsetCnt );
}


void SSLRParser::adjustVarOffset( CSymbolTable::storageType type )
{
	//Offset für lokale Variablen anpassen
	int	offset;

	//welcher storage type?
	switch( type )
	{
		case CSymbolTable::_CHAR:
		case CSymbolTable::_BOOL:
		{
			//Bytes und Bools benötigen ein Byte
			offset = _CHAR;
			break;
		}

		case CSymbolTable::_SHORT:
		{
			//Shorts benötigen zwei Bytes
			offset = _SHORT;
			break;
		}

		case CSymbolTable::_INT:
		{
			//Integers benötigen vier Bytes
			offset = _INT;
			break;
		}

		case CSymbolTable::_LONG:
		{
			//Longs benötigen vier Bytes
			offset = _LONG;
			break;
		}

		default:
		{
			//Fehler
			cerr << "SSLRParser::pushVarOffsetStack: Unknown storage type!" << endl;

			//Ende
			exit( 1 );
		}
	}

	//anpassen
	localVariableOffsetCnt += offset;
}


bool SSLRParser::cast( CSymbolTable::storageType lt, CSymbolTable::storageType rt, CSymbolTable::storageType &newType,
int &which, CSyntaxTree::STOperator &op )
{
	/*
		Wenn zwei Operanden inkompatibel sind, dann müssen sie durch einen cast kompatibel gemacht werden.
		     lt: Typ des linken Operanden
		     rt: Typ des rechten Operanden
		newType: der gemeinsame Typ beider Operanden nach dem cast
		  which: LEFT:  linker Operand muss gecastet werden
				 RIGHT: rechter Operand muss gecastet werden
		     op: Cast-Operator
	*/

	//Typen vergleichen
	if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//gemeinsamer Datentyp ist CHAR
		newType = CSymbolTable::_CHAR;

		//nicht casten
		return( false );
	}

	if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//gemeinsamer Datentyp ist BOOL
		newType = CSymbolTable::_BOOL;

		//nicht casten
		return( false );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_INT ) )
	{
		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//nicht casten
		return( false );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_LONG ) )
	{
		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//nicht casten
		return( false );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//gemeinsamer Datentyp ist SHORT
		newType = CSymbolTable::_SHORT;

		//nicht casten
		return( false );
	}




	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//BOOL in SHORT wandeln
		op = CSyntaxTree::_BOOL2SHORT;

		//gemeinsamer Datentyp ist SHORT
		newType = CSymbolTable::_SHORT;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//BOOL in CHAR wandeln
		op = CSyntaxTree::_BOOL2CHAR;

		//gemeinsamer Datentyp ist CHAR
		newType = CSymbolTable::_CHAR;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_INT ) )
	{
		//BOOL in INT wandeln
		op = CSyntaxTree::_BOOL2INT;

		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_LONG ) )
	{
		//BOOL in LONG wandeln
		op = CSyntaxTree::_BOOL2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}




	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in CHAR wandeln
		op = CSyntaxTree::_BOOL2CHAR;

		//gemeinsamer Datentyp ist CHAR
		newType = CSymbolTable::_CHAR;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//CHAR in SHORT wandeln
		op = CSyntaxTree::_CHAR2SHORT;

		//gemeinsamer Datentyp ist SHORT
		newType = CSymbolTable::_SHORT;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_INT ) )
	{
		//CHAR in INT wandeln
		op = CSyntaxTree::_CHAR2INT;

		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_LONG ) )
	{
		//CHAR in LONG wandeln
		op = CSyntaxTree::_CHAR2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}



	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in SHORT wandeln
		op = CSyntaxTree::_BOOL2SHORT;

		//gemeinsamer Datentyp ist SHORT
		newType = CSymbolTable::_SHORT;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in SHORT wandeln
		op = CSyntaxTree::_CHAR2SHORT;

		//gemeinsamer Datentyp ist SHORT
		newType = CSymbolTable::_SHORT;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_INT ) )
	{
		//SHORT in INT wandeln
		op = CSyntaxTree::_SHORT2INT;

		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_LONG ) )
	{
		//SHORT in LONG wandeln
		op = CSyntaxTree::_SHORT2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}




	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in INT wandeln
		op = CSyntaxTree::_BOOL2INT;

		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in INT wandeln
		op = CSyntaxTree::_CHAR2INT;

		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//SHORT in INT wandeln
		op = CSyntaxTree::_SHORT2INT;

		//gemeinsamer Datentyp ist INT
		newType = CSymbolTable::_INT;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_LONG ) )
	{
		//INT in LONG wandeln
		op = CSyntaxTree::_INT2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den linken Operand casten
		which = LEFT;

		//casten
		return( true );
	}



	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in LONG wandeln
		op = CSyntaxTree::_BOOL2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in LONG wandeln
		op = CSyntaxTree::_CHAR2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//SHORT in LONG wandeln
		op = CSyntaxTree::_SHORT2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_INT ) )
	{
		//INT in LONG wandeln
		op = CSyntaxTree::_INT2LONG;

		//gemeinsamer Datentyp ist LONG
		newType = CSymbolTable::_LONG;

		//den rechten Operand casten
		which = RIGHT;

		//casten
		return( true );
	}

	else
		//Warnung ausgeben
		cerr << "SSLRParser::cast: Nothing to cast!" << endl;

	//nicht casten
	return( false );
}


bool SSLRParser::forceCast( CSymbolTable::storageType lt, CSymbolTable::storageType rt, CSyntaxTree::STOperator &op )
{
	/*
		Bei einer Zuweisung
		a = b;

		muss die rechte Seite auf den Typ von a gecastet werden, auch, wenn dann Informationen
		verloren gehen.
		lt: Typ des linken Operanden
		rt: Typ des rechten Operanden
		op: Cast-Operator
	*/

	//Typen vergleichen
	if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_CHAR ) )
		//nicht casten
		return( false );

	if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_BOOL ) )
		//nicht casten
		return( false );

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_INT ) )
		//nicht casten
		return( false );

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_LONG ) )
		//nicht casten
		return( false );

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_SHORT ) )
		//nicht casten
		return( false );






	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in BOOL wandeln
		op = CSyntaxTree::_CHAR2BOOL;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//SHORT in BOOL wandeln
		op = CSyntaxTree::_SHORT2BOOL;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_INT ) )
	{
		//INT in BOOL wandeln
		op = CSyntaxTree::_INT2BOOL;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_BOOL ) && ( rt == CSymbolTable::_LONG ) )
	{
		//LONG in BOOL wandeln
		op = CSyntaxTree::_LONG2BOOL;

		//casten
		return( true );
	}




	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in CHAR wandeln
		op = CSyntaxTree::_BOOL2CHAR;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//SHORT in CHAR wandeln
		op = CSyntaxTree::_SHORT2CHAR;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_INT ) )
	{
		//INT in CHAR wandeln
		op = CSyntaxTree::_INT2CHAR;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_CHAR ) && ( rt == CSymbolTable::_LONG ) )
	{
		//LONG in CHAR wandeln
		op = CSyntaxTree::_LONG2CHAR;

		//casten
		return( true );
	}



	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in SHORT wandeln
		op = CSyntaxTree::_BOOL2SHORT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in SHORT wandeln
		op = CSyntaxTree::_CHAR2SHORT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_INT ) )
	{
		//INT in SHORT wandeln
		op = CSyntaxTree::_INT2SHORT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_SHORT ) && ( rt == CSymbolTable::_LONG ) )
	{
		//LONG in SHORT wandeln
		op = CSyntaxTree::_LONG2SHORT;

		//casten
		return( true );
	}




	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in INT wandeln
		op = CSyntaxTree::_BOOL2INT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in INT wandeln
		op = CSyntaxTree::_CHAR2INT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//SHORT in INT wandeln
		op = CSyntaxTree::_SHORT2INT;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_INT ) && ( rt == CSymbolTable::_LONG ) )
	{
		//LONG in INT wandeln
		op = CSyntaxTree::_LONG2INT;

		//casten
		return( true );
	}



	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_BOOL ) )
	{
		//BOOL in LONG wandeln
		op = CSyntaxTree::_BOOL2LONG;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_CHAR ) )
	{
		//CHAR in LONG wandeln
		op = CSyntaxTree::_CHAR2LONG;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_SHORT ) )
	{
		//SHORT in LONG wandeln
		op = CSyntaxTree::_SHORT2LONG;

		//casten
		return( true );
	}

	else if( ( lt == CSymbolTable::_LONG ) && ( rt == CSymbolTable::_INT ) )
	{
		//INT in LONG wandeln
		op = CSyntaxTree::_INT2LONG;

		//casten
		return( true );
	}

	else
		//Warnung ausgeben
		cerr << "SSLRParser::forceCast: Nothing to cast!" << endl;

	//nicht casten
	return( false );
}


