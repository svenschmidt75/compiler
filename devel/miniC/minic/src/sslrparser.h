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
#include <iomanip>
#include <vector>
#include "sslexscanner.h"
#include "ccodebase.h"
#include "minic.h"


//Nonterminal defines for the jump table
#define start	0
#define declaration	1
#define factor	2
#define auxsym_1	3
#define type_specifier	4
#define expr	5
#define definitions	6
#define stmt_list	7
#define definition	8
#define declarations	9
#define stmt	10
#define program	11
#define term	12
#define declarator_list	13


//Nontermials for the LR table
#define start_0	0
#define program_0	1
#define definitions_0	2
#define definitions_1	3
#define definition_0	4
#define declarations_0	5
#define declarations_1	6
#define declaration_0	7
#define declarator_list_0	8
#define declarator_list_1	9
#define type_specifier_0	10
#define type_specifier_1	11
#define type_specifier_2	12
#define type_specifier_3	13
#define type_specifier_4	14
#define stmt_list_0	15
#define stmt_list_1	16
#define stmt_0	17
#define stmt_1	18
#define stmt_2	19
#define stmt_3	20
#define stmt_4	21
#define stmt_5	22
#define stmt_6	23
#define stmt_7	24
#define auxsym_1_0	25
#define expr_0	26
#define expr_1	27
#define expr_2	28
#define expr_3	29
#define expr_4	30
#define expr_5	31
#define expr_6	32
#define expr_7	33
#define expr_8	34
#define expr_9	35
#define expr_10	36
#define expr_11	37
#define expr_12	38
#define expr_13	39
#define expr_14	40
#define expr_15	41
#define expr_16	42
#define expr_17	43
#define term_0	44
#define term_1	45
#define term_2	46
#define term_3	47
#define factor_0	48
#define factor_1	49
#define factor_2	50
#define factor_3	51
#define factor_4	52


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


#ifdef SSLR_DEBUG
	//stack for debugging
	typedef struct
	{
		//true: terminal, false: nonterminal
		bool	elem;

		//terminal: index in SSLR_termName, nonterminal: index in SSLR_NTERM_STR
		int		index;

	}SSLR_debugStack;
#endif




	//private class variables



	//Number of lr states
	static const unsigned long	SSLR_anzLRStates = 94;

	//Number of terminals = number of columns in lrTab
	static const unsigned long	SSLR_anzTerm = 41;

	//Number of productions = number of rows in jumpTab
	static const unsigned long	SSLR_anzNTerm = 14;

	//Number of alternatives
	static const unsigned long	SSLR_anzProd = 53;


	//Actions in lrTab
	static const int			LR_ERROR = 1;
	static const int			LR_SHIFT = 2;
	static const int			LR_REDUCE = 3;
	static const int			LR_ACCEPT = 4;


	//maximum stack size
	static const int			SSLR_maxStackSize = 100;

#ifdef SSLR_DEBUG
	//longest right side of all alternatives
	static const int			SSLR_maxAltRightSideWidth = 72;

	//longest terminal or nonterminal
	static const int			SSLR_maxElemWidth = 18;

	//longest state
	static const int			SSLR_maxStateWidth = 8;
#endif

	//number of seccessful reduce - and shift actions
	//neccassary after an error occured
	static const int			SSLR_anzSuppressErrorMsg = 3;

	//number of seccessful reduce - and shift actions after an error occured
	int							SSLR_anzSuppressError;


	//parse stack
	long						SSLR_stack[SSLR_maxStackSize];

#ifdef SSLR_DEBUG
	//debug stack
	SSLR_debugStack				SSLR_dbgStack[SSLR_maxStackSize];

	//debug stack pointer
	unsigned long				SSLR_dbgSp;
#endif

	//stack counter
	long						SSLR_sp;


	//LR table
	static SSLR_LR_TAB			SSLR_lrTab[SSLR_anzLRStates][SSLR_anzTerm];

	//jump table
	static long					SSLR_jumpTab[SSLR_anzLRStates][SSLR_anzNTerm];

	//scanner class
	SSLEXScanner *				SSLR_scanner;

	//array with all alternatives
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

	//allow/deny semantic actions
	bool						SSLR_semAct;

	//stack for semantic actions
	SSLR_TYPE					SSLR_semStack[SSLR_maxStackSize];

	//stack counter for semantic action stack
	unsigned int				SSLR_semStackCnt;

	//Alternatives
	static char *				SSLR_start0Ri[1];
	static char *				SSLR_program0Ri[1];
	static char *				SSLR_definitions0Ri[1];
	static char *				SSLR_definitions1Ri[2];
	static char *				SSLR_definition0Ri[1];
	static char *				SSLR_declarations0Ri[2];
	static char *				SSLR_declarations1Ri[1];
	static char *				SSLR_declaration0Ri[3];
	static char *				SSLR_declarator_list0Ri[1];
	static char *				SSLR_declarator_list1Ri[3];
	static char *				SSLR_type_specifier0Ri[1];
	static char *				SSLR_type_specifier1Ri[1];
	static char *				SSLR_type_specifier2Ri[1];
	static char *				SSLR_type_specifier3Ri[1];
	static char *				SSLR_type_specifier4Ri[1];
	static char *				SSLR_stmt_list0Ri[1];
	static char *				SSLR_stmt_list1Ri[2];
	static char *				SSLR_stmt0Ri[4];
	static char *				SSLR_stmt1Ri[5];
	static char *				SSLR_stmt2Ri[3];
	static char *				SSLR_stmt3Ri[5];
	static char *				SSLR_stmt4Ri[3];
	static char *				SSLR_stmt5Ri[1];
	static char *				SSLR_stmt6Ri[2];
	static char *				SSLR_stmt7Ri[4];
	static char *				SSLR_auxsym_10Ri[1];
	static char *				SSLR_expr0Ri[3];
	static char *				SSLR_expr1Ri[3];
	static char *				SSLR_expr2Ri[3];
	static char *				SSLR_expr3Ri[3];
	static char *				SSLR_expr4Ri[3];
	static char *				SSLR_expr5Ri[3];
	static char *				SSLR_expr6Ri[3];
	static char *				SSLR_expr7Ri[3];
	static char *				SSLR_expr8Ri[2];
	static char *				SSLR_expr9Ri[3];
	static char *				SSLR_expr10Ri[3];
	static char *				SSLR_expr11Ri[3];
	static char *				SSLR_expr12Ri[3];
	static char *				SSLR_expr13Ri[3];
	static char *				SSLR_expr14Ri[2];
	static char *				SSLR_expr15Ri[3];
	static char *				SSLR_expr16Ri[3];
	static char *				SSLR_expr17Ri[1];
	static char *				SSLR_term0Ri[3];
	static char *				SSLR_term1Ri[3];
	static char *				SSLR_term2Ri[3];
	static char *				SSLR_term3Ri[1];
	static char *				SSLR_factor0Ri[3];
	static char *				SSLR_factor1Ri[1];
	static char *				SSLR_factor2Ri[1];
	static char *				SSLR_factor3Ri[1];
	static char *				SSLR_factor4Ri[1];

	//all follow sets for the error recovery
	static long					SSLR_startFollow[1];
	static long					SSLR_programFollow[1];
	static long					SSLR_definitionsFollow[17];
	static long					SSLR_definitionFollow[17];
	static long					SSLR_declarationsFollow[19];
	static long					SSLR_declarationFollow[19];
	static long					SSLR_declarator_listFollow[2];
	static long					SSLR_type_specifierFollow[1];
	static long					SSLR_stmt_listFollow[18];
	static long					SSLR_stmtFollow[19];
	static long					SSLR_auxsym_1Follow[16];
	static long					SSLR_exprFollow[33];
	static long					SSLR_termFollow[36];
	static long					SSLR_factorFollow[36];


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

	//print an error message, but only when it is allowed
	void SSLR_printErrorMsg( bool, char * );

#ifdef SSLR_DEBUG
	//print the debug stack SSLR_dbgStack
	void SSLR_outputDbgStack( int, int, int, int );
#endif

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


private:
/*
In einem Block können lokale Bezeichner vereinbart werden. Für diese muss
auf dem Laufzeit-Stack Platz geschaffen werden (mittels des link-Befehls).
*/
vector<long>				localVarStr;
//Offset für die lokalen Variablen
enum{ _CHAR = 2, _BOOL = 2, _SHORT = 2, _INT = 4, _LONG = 4 };
//fürs Casten
enum{ LEFT, RIGHT };
/* Verschachtelungstiefe */
int							blkNum;
//den Typ der Vereinbarung merken
CSymbolTable::storageType	stType;
//Zeiger auf die Symboltabelle
CSymbolTable				*symTab;
//Zeiger auf den Syntaxbaum
CSyntaxTree					*syntaxTree;
//Zeiger auf die Codeerzeugung
CCodeBase					*codeBase;
//Command line arguments
ARGV_STR					*argvStr;
/*
In jedem Block (0=global definierte Variablen) bzw. Verschachtelungstiefe können
Variablen definiert werden, für die später auf dem Laufzeit-Stack Platz
geschaffen werden muss. Hier wird für jeden Block eine entsprechende Variable
geführt, die für den jeweiligen Block den Offset relativ zur Start-Adresse des
Laufzeit-Stacks führt.
*/
unsigned long				*varOffsetStack;
//Zähler für den Offset
unsigned long 				varOffsetStackPos;
//Anzahl der maximalen Elemente von varOffsetStack
unsigned long				varOffsetStackMaxPos;
//Zähler für den Offset für die lokalen Variablen
long						localVariableOffsetCnt;
//initialisieren
void init( void );
//deinitialisieren
void deinit( void );
//Offset für lokale Variablen merken
void pushVarOffsetStack( void );
//Offset für lokale Variablen vom Stack holen
void popVarOffsetStack( void );
//Offset für lokale Variablen anpassen
void adjustVarOffset( CSymbolTable::storageType );
/*
Wenn zwei Operanden inkompatibel sind, dann müssen sie durch einen cast kompatibel gemacht werden.
lt: Typ des linken Operanden
rt: Typ des rechten Operanden
newType: der gemeinsame Typ beider Operanden nach dem cast
which: LEFT:  linker Operand muss gecastet werden
RIGHT: rechter Operand muss gecastet werden
op: Cast-Operator
*/
bool cast( CSymbolTable::storageType, CSymbolTable::storageType, CSymbolTable::storageType &, int &,
CSyntaxTree::STOperator & );
/*
Bei einer Zuweisung
a = b;
muss die rechte Seite auf den Typ von a gecastet werden, auch, wenn dann Informationen
verloren gehen.
lt: Typ des linken Operanden
rt: Typ des rechten Operanden
op: Cast-Operator
*/
bool forceCast( CSymbolTable::storageType, CSymbolTable::storageType, CSyntaxTree::STOperator & );
public:
//dem Parser einige Variablen übergeben
void giveArgs( ARGV_STR *, CSymbolTable *, CSyntaxTree *, CCodeBase * );
};


#endif

