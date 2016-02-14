/***************************************************************************
                          sslexscanner.cpp  -  description
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

#include "sslexscanner.h"
#include <iostream>


/********************************
* This is the non-debug-version *
********************************/




//Define macro ECHO
#define ECHO	cout << yyText

//set a new start condition in user-defined action
#define BEGIN	setStartCondition

//current start condition
#define YY_START	baseState

//yyTerminate
#define yyTerminate()	return( ACCEPT )


//concatenate lexem
#define yyMore()	concatenate = true


//Size of start condition stack
#define SCStackSize	100


//enlargement for yyText if neccesary
#define SSLEX_YYTEXTSIZE	(8*1024)



//Transition-Table
long SSLEXScanner::ssLexTable[SSLEX_STATE][CCLWIDTH] = 
{
/*
  \0,  \1,  \2,  \3,  \4,  \5,  \6,  \a,  \b,  \t,  \n,  \v,  \f,  \r,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
    ,   !,   ",   #,   $,   %,   &,   ',   (,   ),   *,   +,   ,,   -,   .,   /,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   :,   ;,   <,   =,   >,   ?,
   @,   A,   B,   C,   D,   E,   F,   G,   H,   I,   J,   K,   L,   M,   N,   O,   P,   Q,   R,   S,   T,   U,   V,   W,   X,   Y,   Z,   [,   \,   ],   ^,   _,
   `,   a,   b,   c,   d,   e,   f,   g,   h,   i,   j,   k,   l,   m,   n,   o,   p,   q,   r,   s,   t,   u,   v,   w,   x,   y,   z,   {,   |,   },   ~, DEL
*/

//Startcondition: INITIAL
/* 0 */
{
   2,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   3,   3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   4,   5,   6,   7,  -1,   8,  -1,   9,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  -1,  11,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 1 */
{
   2,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   3,   3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   4,   5,   6,   7,  -1,   8,  -1,   9,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  -1,  11,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 2 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 3 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   3,   3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   3,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 4 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 5 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 6 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 7 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 8 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 9 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 10 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  12,  -1,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  14,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 11 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 12 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 13 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  14,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 14 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  16,  -1,  16,  -1,  -1,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 15 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
},
/* 16 */
{
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1
}

};

//Accept-Array
ulong SSLEXScanner::ssLexAccept[SSLEX_STATE] = 
{
/* 0 */
0,
/* 1 */
0,
/* 2 */

//Line 34: \\0
2,
/* 3 */

//Line 19: (([ \\t\\n])+)
3,
/* 4 */

//Line 36: \"(\"
4,
/* 5 */

//Line 38: \")\"
5,
/* 6 */

//Line 30: \"*\"
6,
/* 7 */

//Line 28: \"+\"
7,
/* 8 */

//Line 26: \"-\"
8,
/* 9 */

//Line 32: \"/\"
9,
/* 10 */

//Line 22: (([0-9])+(\\.([0-9])+)?(E[+\\-]?([0-9])+)?)
10,
/* 11 */

//Line 24: \";\"
11,
/* 12 */
0,
/* 13 */

//Line 22: (([0-9])+(\\.([0-9])+)?(E[+\\-]?([0-9])+)?)
13,
/* 14 */
0,
/* 15 */

//Line 22: (([0-9])+(\\.([0-9])+)?(E[+\\-]?([0-9])+)?)
15,
/* 16 */
0
};


//Table for error handling
int SSLEXScanner::err[scSize][CCLWIDTH] = { 0 };



//state of start conditions
ulong SSLEXScanner::scIndex[] = {
	0, //INITIAL
	1 //NL_INITIAL
};


SSLEXScanner::SSLEXScanner( char *data, ulong size )
{
	//initialize SSLEX

	//no data?
	if( !data )
	{
		//error
		cerr << "sslex: SSLEXScanner::SSLEXScanner: No data!\n";

		//exit
		exit( 1 );
	}

	//save length of text
	basePosLength = size;

	//copy text to analyze due to unput
	pos = basePos = new( char[basePosLength + 1] );

	//copy to analyze string. This is nesessary as there are zero-bytes allowed in scanning
	//text. But this would couse strlen to return only the length till the next zero-byte!
	memcpy( pos, data, sizeof( char ) * basePosLength );
	pos[basePosLength] = 0;

	//end of scanning
	end = pos + basePosLength;

	//start-condition INITIAL
	state = baseState = INITIAL;

	//init with SSLEX_YYTEXTSIZE bytes
	yyTextLen = SSLEX_YYTEXTSIZE;
	yyText = new( char[yyTextLen + 1] );
	yyLeng = 0;

	//SSLEX automatically counts line numbers
	yyLineno = 1;

	//last accepting state
	lastAcceptingState = -1;

	//do not concatenate token
	concatenate = false;

	//no start conditions on stack
	scStackSize = 0;
	scStackPos = 0;
	scStack = 0L;


#ifdef SSLEX_INIT

	//inizialize user defined variables
	init();
#endif
}


SSLEXScanner::~SSLEXScanner()
{
	//free memory
	if( yyText )
		delete[] ( yyText );

	if( scStack )
		delete[] ( scStack );

	//free old text
	if( basePos )
		delete[] ( basePos );


#ifdef SSLEX_INIT

	//deinizialize user defined variables
	deinit();
#endif
}


void SSLEXScanner::reset( void )
{
	//reset the whole scanner

	//start position
	pos = basePos;

	//end of scanning
	end = pos + basePosLength;

	//need memory for yyText?
	if( !yyText )
	{
		//init with SSLEX_YYTEXTSIZE bytes
		yyTextLen = SSLEX_YYTEXTSIZE;
		yyText = new( char[yyTextLen + 1] );
	}

	//start-condition INITIAL
	state = baseState = INITIAL;

	//no token
	yyLeng = 0;

	//SSLEX automatically counts line numbers
	yyLineno = 1;

	//last accepting state
	lastAcceptingState = -1;

	//do not concatenate token
	concatenate = false;

	//no start conditions on stack
	scStackPos = 0;


#ifdef SSLEX_INIT

	//inizialize user defined variables
	init();
#endif
}


void SSLEXScanner::reset( char *data, ulong size )
{
	//reset the whole scanner and give him new input

	//no data?
	if( !data )
	{
		//error
		cerr << "sslex: SSLEXScanner::reset: No data!\n";

		//exit
		exit( 1 );
	}

	//free old text
	if( basePos )
		delete[] ( basePos );

	//save length of text
	basePosLength = size;

	//copy text to analyze due to unput
	pos = basePos = new( char[basePosLength + 1] );

	//copy to analyze string. This is nesessary as there are zero-bytes allowed in scanning
	//text. But this would couse strlen to return only the length till the next zero-byte!
	memcpy( pos, data, sizeof( char ) * basePosLength );
	pos[basePosLength] = 0;

	//end of scanning
	end = pos + basePosLength;

	//start-condition INITIAL
	state = baseState = INITIAL;

	//need memory for yyText?
	if( !yyText )
	{
		//init with SSLEX_YYTEXTSIZE bytes
		yyTextLen = SSLEX_YYTEXTSIZE;
		yyText = new( char[yyTextLen + 1] );
	}

	//no token
	yyLeng = 0;

	//SSLEX automatically counts line numbers
	yyLineno = 1;

	//last accepting state
	lastAcceptingState = -1;

	//do not concatenate token
	concatenate = false;

	//no start conditions on stack
	scStackPos = 0;


#ifdef SSLEX_INIT

	//inizialize user defined variables
	init();
#endif
}


int SSLEXScanner::yyLex( SSLR_TYPE *attributeStr )
{
	//start scanning
	unsigned char	c;

	//Basis-Position für das Lexem
	startPos = pos;

	//setup some stuff
	yyDoBeforeRule();

	while( pos <= end )
	{
		//Zeichen
		c = *pos;

		//when we use a 7 bit scanner
		if( c > 127 )
		{
			//we need a 8 bit scanner
			cerr << "SSLEXScanner::yyLex: Line " << yyLineno << ": You need a 8 bit scanner to match the character " << c << "!\n";

			//skip character
			pos++;
			continue;
		}

		//schieben?
		if( ssLexTable[state][c] >= 0 )
		{
			//akzeptieren?
			if( ssLexAccept[state] )
			{
				//Ja, merken
				lastAcceptingState = state;
				lastAcceptingPos = pos;
			}

			//shift new state
			state = ssLexTable[state][c];

			//nächstes Zeichen
			pos++;
		}

		else
		{
			//akzeptieren
			do
			{
				switch( ssLexAccept[state] )
				{
					//\0
					case 2:
					{
						//setup yyText
						setupyyText();

return( ACCEPT );

						break;
					}

					//(([ \t\n])+)
					case 3:
					{
						//setup yyText
						setupyyText();

;

						break;
					}

					//"("
					case 4:
					{
						//setup yyText
						setupyyText();

return( KLOPEN );

						break;
					}

					//")"
					case 5:
					{
						//setup yyText
						setupyyText();

return( KLCLOSE);

						break;
					}

					//"*"
					case 6:
					{
						//setup yyText
						setupyyText();

return( MUL );

						break;
					}

					//"+"
					case 7:
					{
						//setup yyText
						setupyyText();

return( ADD );

						break;
					}

					//"-"
					case 8:
					{
						//setup yyText
						setupyyText();

return( SUB );

						break;
					}

					//"/"
					case 9:
					{
						//setup yyText
						setupyyText();

return( DIV );

						break;
					}

					//(([0-9])+(\.([0-9])+)?(E[+\-]?([0-9])+)?)
					case 10:
					case 13:
					case 15:
					{
						//setup yyText
						setupyyText();

attributeStr->f = atof( yyText );	return( ZAHL );

						break;
					}

					//";"
					case 11:
					{
						//setup yyText
						setupyyText();

return( SEMIKOLON );

						break;
					}


					default:
					{
						//unable to accept, backtracking or error
						if( lastAcceptingState >= 0 )
						{
							//set back to last accept position
							state = lastAcceptingState;
							pos = lastAcceptingPos;

							//now accept with last accept position
							continue;
						}

						else
						{
							//error
							error( *startPos );

							//remove character
							pos++;
						}

						break;
					}
				}

				//setup some stuff
				yyDoBeforeRule();

				//exit while-loop
				break;

			}while( 1 );

			//Ende der Eingabe erreicht?
			if( pos == end )
				//Ja
				break;
		}
	}

	//call eof-rules
	return( endOfLine() );
}


void SSLEXScanner::yyDoBeforeRule( void )
{
	//set some stuff before a new rule is scanned

	//reset
	lastAcceptingState = -1;

	//base position for lexem
	startPos = pos;

	//are we at the beginning of a line?
	if( ( pos == basePos ) || ( pos[-1] == '\n' ) )
		//setup state
		state = setupState( baseState );

	else
		//base-State
		state = baseState;
}


void SSLEXScanner::error( uchar c )
{
	//simple error handling routine
	ulong		i;
	char		*transChar[] = {
	  "\\0",   "\\1",   "\\2",   "\\3",   "\\4",   "\\5",   "\\6",  "\\a",  "\\b",  "\\t",  "\\n",  "\\v",
	 "\\f",  "\\r", "0x0e", "0x0f", "0x10", "0x11", "0x12", "0x13", "0x14", "0x15", "0x16", "0x17",
	"0x18", "0x19", "0x1a", "0x1b", "0x1c", "0x1d", "0x1e", "0x1f",    " ",    "!",   "\"",    "#",
	   "$",    "%",    "&",    "'",    "(",    ")",    "*",    "+",    ",",    "-",    ".",    "/",
	   "0",    "1",    "2",    "3",    "4",    "5",    "6",    "7",    "8",    "9",    ":",    ";",
	   "<",    "=",    ">",    "?",    "@",    "A",    "B",    "C",    "D",    "E",    "F",    "G",
	   "H",    "I",    "J",    "K",    "L",    "M",    "N",    "O",    "P",    "Q",    "R",    "S",
	   "T",    "U",    "V",    "W",    "X",    "Y",    "Z",    "[",    "\\", "]",    "^",    "_",
	   "`",    "a",    "b",    "c",    "d",    "e",    "f",    "g",    "h",    "i",    "j",    "k",
	   "l",    "m",    "n",    "o",    "p",    "q",    "r",    "s",    "t",    "u",    "v",    "w",
	   "x",    "y",    "z",    "{",    "|",    "}",    "~",    "DEL",
	   "128",  "129",  "130",  "131",  "132",  "133",  "134",  "135",  "136",  "137",
	   "138",  "139",  "140",  "141",  "142",  "143",  "144",  "145",  "146",  "147",
	   "148",  "149",  "150",  "151",  "152",  "153",  "154",  "155",  "156",  "157",
	   "158",  "159",  "160",  "161",  "162",  "163",  "164",  "165",  "166",  "167",
	   "168",  "169",  "170",  "171",  "172",  "173",  "174",  "175",  "176",  "177",
	   "178",  "179",  "180",  "181",  "182",  "183",  "184",  "185",  "186",  "187",
	   "188",  "189",  "190",  "191",  "192",  "193",  "194",  "195",  "196",  "197",
	   "198",  "199",  "200",  "201",  "202",  "203",  "204",  "205",  "206",  "207",
	   "208",  "209",  "210",  "211",  "212",  "213",  "214",  "215",  "216",  "217",
	   "218",  "219",  "220",  "221",  "222",  "223",  "224",  "225",  "226",  "227",
	   "228",  "229",  "230",  "231",  "232",  "233",  "234",  "235",  "236",  "237",
	   "238",  "239",  "240",  "241",  "242",  "243",  "244",  "245",  "246",  "247",
	   "248",  "249",  "250",  "251",  "252",  "253",  "254" };

	//get current sc
	for( i = 0; i < scSize; i++ )
	{
		//equal?
		if( scIndex[i] == baseState )
			break;
	}

	//did already an error occur in that sc?
	if( !err[i][c] )
	{
		//No, print an error message
		cerr << "line " << yyLineno << ": Illegal character " << transChar[c] << "!\n";

		//mark to avoid ever lasting loops
		err[i][c]++;
	}
}


void SSLEXScanner::setupYYLineno( char *beg, char *end )
{
	//setup yyLineno
	ulong	len = end - beg, i;
	int		nl = 0;

	//count newlines
	for( i = 0; i < len; i++ )
	{
		//newline?
		if( beg[i] == '\n' )
			nl++;
	}

	//add to yyLineno
	yyLineno += nl;
}


void SSLEXScanner::setupyyText( void )
{
	//insert text in yytext
	ulong	len;
	char	*tmp;

	//reg. expression one-character?
	if( pos == startPos )
		//yes, move to next character
		pos++;

	//get length of lexem
	yyLeng = pos - startPos;

	//concatenate token? (yyMore)
	if( concatenate == true )
	{
		//yes, append
		len = strlen( yyText );

		//enough space?
		if( yyTextLen <= ( yyLeng + len ) )
		{
			//extend memory

			//fix yyTextLen
			yyTextLen = yyLeng + len;

			//get enough memory für lexem
			tmp = new( char[yyTextLen + SSLEX_YYTEXTSIZE] );

			//copy string
			strcpy( tmp, yyText );

			//release memory
			if( yyText )
				delete[] ( yyText );

			//copy adress
			yyText = tmp;
		}

		//append
		strncat( yyText, startPos, yyLeng );

		//zero
		yyText[yyLeng + len] = 0;

		//set up yyLeng
		yyLeng += len;

		//next time we want to set up yyText as usual
		concatenate = false;
	}

	else
	{
		//enough space?
		if( yyTextLen <= yyLeng )
		{
			//extend memory
			if( yyText )
				delete[] ( yyText );

			//fix yyTextLen
			yyTextLen = yyLeng;

			//get enough memory für lexem
			yyText = new( char[yyTextLen + SSLEX_YYTEXTSIZE] );
		}

		//insert
		strncpy( yyText, startPos, yyLeng );

		//end with zere
		yyText[yyLeng] = 0;
	}

	//setup yyLineno
	setupYYLineno( startPos, pos );
}


ulong SSLEXScanner::setupState( ulong baseState )
{
	//setup state for begin-of-line-rules
	ulong	state = baseState, i;

	//test if the current state is valid
	switch( baseState )
	{
		case INITIAL:
		{
			//it is right the next state;
			state++;
			break;
		}

		default:
		{
			//ups, unknown start condition
			cerr << "SSLEXScanner::setupState: Line " << yyLineno << ": Unknown start condition!\n";
			exit( 1 );
		}
	}

	//return state
	return( state );
}


void SSLEXScanner::setStartCondition( ulong state )
{
	//set a new start condition
	switch( state )
	{
		case INITIAL:
		{
			//set new start condition
			baseState = INITIAL;
			break;
		}


		default:
		{
			//unknown start condition
			cerr << "SSLEXScanner::setStartCondition: Line " << yyLineno << ": Unknown start condition!\n";

			//ignore call
			break;
		}
	}
}


ulong SSLEXScanner::getYYLeng( void )
{
	//return length of yyText
	return( yyLeng );
}


ulong SSLEXScanner::getYYLineno( void )
{
	//return line number
	return( yyLineno );
}


char *SSLEXScanner::getYYText( void )
{
	//return yyText
	return( yyText );
}


char *SSLEXScanner::yyStrCpy( char *txt )
{
	//return txt in it's own memory
	ulong	len;
	char	*tmp;

	//length of txt
	len = strlen( txt );

	//get memory for txt
	tmp = new( char[len + 1] );

	//copy
	strcpy( tmp, txt );

	//zero
	tmp[len] = 0;

	//return
	return( tmp );
}


void SSLEXScanner::yyLess( int n )
{
	//returns all but the first n characters of yyText back to the input

	//check
	if( ( n < 0 ) || ( n > yyLeng ) )
	{
		//bad n
		cerr << "SSLEXScanner::yyLess: Line " << yyLineno << ": Bad argument!\n";

		//ignore
		return;
	}

	//zero
	yyText[n] = 0;

	//return to input spring
	pos -= ( yyLeng - n );
}


void SSLEXScanner::yyPushState( ulong newState )
{
	/*
		Pushes the current start condition onto the top of the start condition stack
		and switches to newState as though you have used BEGIN( newState ).
	*/

	//enough memory
	if( scStackPos == scStackSize )
		//enlarge start condition stack
		enlargeSCStack();

	//put on stack
	scStack[scStackPos++] = baseState;

	//switch to new state
	BEGIN( newState );
}


void SSLEXScanner::yyPopState( void )
{
	//pops the top of the start condition stack and switches to it

	//underflow?
	if( scStackPos <= 0 )
		//error
		cerr << "SSLEXScanner::yyPopState: Line " << yyLineno << ": Start condition stack underflow!\n";

	else
	{
		//get new SC state
		ulong	newState = scStack[--scStackPos];

		//switch to it
		BEGIN( newState );

		//set new state
		state = newState;
	}
}


ulong SSLEXScanner::yyTopState( void )
{
	//returns the top of the start condition stack without altering its contents
	return( scStack[scStackPos - 1] );
}


void SSLEXScanner::enlargeSCStack( void )
{
	//enlarge start condition stack
	ulong	*tmp;

	//get enough memory
	tmp = new( ulong[scStackSize + SCStackSize] );

	//copy old content
	memcpy( tmp, scStack, sizeof( ulong ) * scStackSize );

	//free old memory?
	if( scStack )
		//yes
		delete[] ( scStack );

	//set new adress
	scStack = tmp;

	//add size
	scStackSize += SCStackSize;
}


void SSLEXScanner::yyUnput( unsigned char c )
{
	//puts the character c back to the input stream
	ulong	len = basePosLength++;
	int		diff = pos - basePos;
	char	*txt;

	//get enough memory to push back character
	txt = new( char[len + 2] );

	//copy to current position
	memcpy( txt, basePos, sizeof( char ) * diff );

	//now insert c
	txt[diff] = c;

	//append rest of the input
	memcpy( &txt[diff + 1], pos, sizeof( char ) * ( len - diff ) );

	//zero
	txt[len + 1] = 0;

	//free old memory
	delete[] ( basePos );

	//set new position
	pos = startPos = &txt[diff];

	//set new end
	end = &txt[len + 1];

	//set new beginning
	basePos = txt;
}


void SSLEXScanner::yyUnput( char *str )
{
	//puts the string str back to the input stream
	ulong	len = basePosLength, strLen = strlen( str );
	int		diff = pos - basePos;
	char	*txt;

	//get enough memory to push back character
	txt = new( char[len + strLen + 1] );

	//copy to current position
	memcpy( txt, basePos, sizeof( char ) * diff );

	//now append str
	strcpy( &txt[diff], str );

	//append rest of the input
	memcpy( &txt[strLen + diff], pos, sizeof( char ) * ( len - diff ) );

	//zero
	txt[len + strLen] = 0;

	//free old memory
	delete[] ( basePos );

	//set new position
	pos = startPos = &txt[diff];

	//set new end
	end = &txt[len + strLen + 1];

	//set new beginning
	basePos = txt;

	//correct basePosLength
	basePosLength += strLen;
}


char SSLEXScanner::yyInput( void )
{
	//return the next character to be scanned
	return( *pos++ );
}


int SSLEXScanner::endOfLine( void )
{
	//call eof-rules

	//action depends on start condition
	switch( baseState )
	{
		case INITIAL:
		{
			//default action
			yyTerminate();

			break;
		}


		default:
		{
			//Unknown start condition
			cerr << "SSLEXScanner::endOfLine: Line " << yyLineno << ": Unknown start condition!\n";

			//ignore
			break;
		}
	}

	//this should never be executed!
	return( 0 );
}


