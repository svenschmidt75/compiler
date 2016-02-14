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
#include <fstream>
#include <sys/stat.h>

#include "sslexscanner.h"
#include "terminal.h"
#include "hashclass.h"


//error routines
#define SSLR_ERROR_POP			SSLR_errPop
#define SSLR_ERROR_DELETE		SSLR_errDelete
#define SSLR_ERROR_SHIFT		SSLR_errShift
#define SSLR_ERROR_OVERREAD		SSLR_errOverRead
#define SSLR_ERROR_SEM			SSLR_semAct = true
#define SSLR_ERROR_EXIT			exit( 1 )


//Nonterminal defines for the jump table
#define start	0
#define semType	1
#define errorMsg	2
#define newProduction	3
#define ebnfEClose	4
#define rhs	5
#define sc2	6
#define error	7
#define associativity	8
#define ebnfEOpen	9
#define ebnfKLOpen	10
#define rightSide	11
#define unionBody2	12
#define goal	13
#define definition1	14
#define definition2	15
#define ebnfBOpen	16
#define termList	17
#define sslrRule	18
#define semanticAction	19
#define errorAction2	20
#define startUnion	21
#define errorBody	22
#define sect1	23
#define sect2	24
#define precedence	25
#define ebnfKLClose	26
#define negTerm	27
#define item1	28
#define item2	29
#define item3	30
#define initBeforeNumber	31
#define ntermDef	32
#define unionBody	33
#define initSSLR	34
#define union	35
#define newAlternative	36
#define ebnfBClose	37
#define errorAction	38
#define errorBody2	39
#define namelist1	40
#define namelist2	41


//Nontermials for the LR table
#define start_0	0
#define goal_0	1
#define initSSLR_0	2
#define sect1_0	3
#define definition1_0	4
#define definition1_1	5
#define definition1_2	6
#define union_0	7
#define union_1	8
#define startUnion_0	9
#define unionBody_0	10
#define unionBody_1	11
#define unionBody2_0	12
#define unionBody2_1	13
#define definition2_0	14
#define definition2_1	15
#define ntermDef_0	16
#define ntermDef_1	17
#define semType_0	18
#define semType_1	19
#define associativity_0	20
#define associativity_1	21
#define associativity_2	22
#define namelist1_0	23
#define namelist1_1	24
#define namelist2_0	25
#define namelist2_1	26
#define sect2_0	27
#define sect2_1	28
#define sc2_0	29
#define sc2_1	30
#define sslrRule_0	31
#define newProduction_0	32
#define rightSide_0	33
#define rightSide_1	34
#define newAlternative_0	35
#define item1_0	36
#define item1_1	37
#define rhs_0	38
#define rhs_1	39
#define rhs_2	40
#define rhs_3	41
#define item2_0	42
#define item2_1	43
#define item3_0	44
#define item3_1	45
#define item3_2	46
#define semanticAction_0	47
#define ebnfKLOpen_0	48
#define ebnfBOpen_0	49
#define ebnfEOpen_0	50
#define ebnfKLClose_0	51
#define ebnfBClose_0	52
#define ebnfEClose_0	53
#define precedence_0	54
#define precedence_1	55
#define error_0	56
#define error_1	57
#define errorBody_0	58
#define errorBody_1	59
#define errorBody2_0	60
#define errorBody2_1	61
#define errorBody2_2	62
#define errorBody2_3	63
#define errorBody2_4	64
#define errorBody2_5	65
#define initBeforeNumber_0	66
#define negTerm_0	67
#define termList_0	68
#define termList_1	69
#define errorAction_0	70
#define errorAction_1	71
#define errorAction2_0	72
#define errorAction2_1	73
#define errorAction2_2	74
#define errorAction2_3	75
#define errorAction2_4	76
#define errorAction2_5	77
#define errorMsg_0	78
#define errorMsg_1	79


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
	static const unsigned long	SSLR_anzLRStates = 134;

	//Number of terminals = number of columns in lrTab
	static const unsigned long	SSLR_anzTerm = 41;

	//Number of productions = number of rows in jumpTab
	static const unsigned long	SSLR_anzNTerm = 42;

	//Number of alternatives
	static const unsigned long	SSLR_anzProd = 80;


	//Actions in lrTab
	static const int			LR_ERROR = 1;
	static const int			LR_SHIFT = 2;
	static const int			LR_REDUCE = 3;
	static const int			LR_ACCEPT = 4;


	//maximum stack size
	static const int			SSLR_maxStackSize = 100;

#ifdef SSLR_DEBUG
	//longest right side of all alternatives
	static const int			SSLR_maxAltRightSideWidth = 117;

	//longest terminal or nonterminal
	static const int			SSLR_maxElemWidth = 19;

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
	unsigned long				SSLR_sp;


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
	static char *				SSLR_goal0Ri[3];
	static char *				SSLR_initSSLR0Ri[1];
	static char *				SSLR_sect10Ri[3];
	static char *				SSLR_definition10Ri[2];
	static char *				SSLR_definition11Ri[1];
	static char *				SSLR_definition12Ri[1];
	static char *				SSLR_union0Ri[4];
	static char *				SSLR_union1Ri[1];
	static char *				SSLR_startUnion0Ri[1];
	static char *				SSLR_unionBody0Ri[3];
	static char *				SSLR_unionBody1Ri[2];
	static char *				SSLR_unionBody20Ri[2];
	static char *				SSLR_unionBody21Ri[3];
	static char *				SSLR_definition20Ri[3];
	static char *				SSLR_definition21Ri[2];
	static char *				SSLR_ntermDef0Ri[1];
	static char *				SSLR_ntermDef1Ri[1];
	static char *				SSLR_semType0Ri[3];
	static char *				SSLR_semType1Ri[1];
	static char *				SSLR_associativity0Ri[1];
	static char *				SSLR_associativity1Ri[1];
	static char *				SSLR_associativity2Ri[1];
	static char *				SSLR_namelist10Ri[2];
	static char *				SSLR_namelist11Ri[1];
	static char *				SSLR_namelist20Ri[2];
	static char *				SSLR_namelist21Ri[1];
	static char *				SSLR_sect20Ri[2];
	static char *				SSLR_sect21Ri[1];
	static char *				SSLR_sc20Ri[3];
	static char *				SSLR_sc21Ri[2];
	static char *				SSLR_sslrRule0Ri[3];
	static char *				SSLR_newProduction0Ri[1];
	static char *				SSLR_rightSide0Ri[3];
	static char *				SSLR_rightSide1Ri[5];
	static char *				SSLR_newAlternative0Ri[1];
	static char *				SSLR_item10Ri[2];
	static char *				SSLR_item11Ri[1];
	static char *				SSLR_rhs0Ri[3];
	static char *				SSLR_rhs1Ri[3];
	static char *				SSLR_rhs2Ri[3];
	static char *				SSLR_rhs3Ri[1];
	static char *				SSLR_item20Ri[2];
	static char *				SSLR_item21Ri[1];
	static char *				SSLR_item30Ri[1];
	static char *				SSLR_item31Ri[1];
	static char *				SSLR_item32Ri[1];
	static char *				SSLR_semanticAction0Ri[1];
	static char *				SSLR_ebnfKLOpen0Ri[1];
	static char *				SSLR_ebnfBOpen0Ri[1];
	static char *				SSLR_ebnfEOpen0Ri[1];
	static char *				SSLR_ebnfKLClose0Ri[1];
	static char *				SSLR_ebnfBClose0Ri[1];
	static char *				SSLR_ebnfEClose0Ri[1];
	static char *				SSLR_precedence0Ri[2];
	static char *				SSLR_precedence1Ri[1];
	static char *				SSLR_error0Ri[4];
	static char *				SSLR_error1Ri[1];
	static char *				SSLR_errorBody0Ri[2];
	static char *				SSLR_errorBody1Ri[1];
	static char *				SSLR_errorBody20Ri[5];
	static char *				SSLR_errorBody21Ri[7];
	static char *				SSLR_errorBody22Ri[8];
	static char *				SSLR_errorBody23Ri[10];
	static char *				SSLR_errorBody24Ri[5];
	static char *				SSLR_errorBody25Ri[3];
	static char *				SSLR_initBeforeNumber0Ri[1];
	static char *				SSLR_negTerm0Ri[1];
	static char *				SSLR_termList0Ri[3];
	static char *				SSLR_termList1Ri[1];
	static char *				SSLR_errorAction0Ri[3];
	static char *				SSLR_errorAction1Ri[1];
	static char *				SSLR_errorAction20Ri[4];
	static char *				SSLR_errorAction21Ri[4];
	static char *				SSLR_errorAction22Ri[4];
	static char *				SSLR_errorAction23Ri[1];
	static char *				SSLR_errorAction24Ri[1];
	static char *				SSLR_errorAction25Ri[1];
	static char *				SSLR_errorMsg0Ri[1];
	static char *				SSLR_errorMsg1Ri[1];

	//all follow sets for the error recovery
	static long					SSLR_startFollow[1];
	static long					SSLR_goalFollow[1];
	static long					SSLR_initSSLRFollow[1];
	static long					SSLR_sect1Follow[2];
	static long					SSLR_definition1Follow[7];
	static long					SSLR_unionFollow[7];
	static long					SSLR_startUnionFollow[1];
	static long					SSLR_unionBodyFollow[2];
	static long					SSLR_unionBody2Follow[1];
	static long					SSLR_definition2Follow[7];
	static long					SSLR_ntermDefFollow[2];
	static long					SSLR_semTypeFollow[1];
	static long					SSLR_associativityFollow[1];
	static long					SSLR_namelist1Follow[8];
	static long					SSLR_namelist2Follow[8];
	static long					SSLR_sect2Follow[1];
	static long					SSLR_sc2Follow[2];
	static long					SSLR_sslrRuleFollow[1];
	static long					SSLR_newProductionFollow[1];
	static long					SSLR_rightSideFollow[5];
	static long					SSLR_newAlternativeFollow[6];
	static long					SSLR_item1Follow[13];
	static long					SSLR_rhsFollow[13];
	static long					SSLR_item2Follow[13];
	static long					SSLR_item3Follow[13];
	static long					SSLR_semanticActionFollow[13];
	static long					SSLR_ebnfKLOpenFollow[6];
	static long					SSLR_ebnfBOpenFollow[6];
	static long					SSLR_ebnfEOpenFollow[6];
	static long					SSLR_ebnfKLCloseFollow[13];
	static long					SSLR_ebnfBCloseFollow[13];
	static long					SSLR_ebnfECloseFollow[13];
	static long					SSLR_precedenceFollow[6];
	static long					SSLR_errorFollow[5];
	static long					SSLR_errorBodyFollow[2];
	static long					SSLR_errorBody2Follow[2];
	static long					SSLR_initBeforeNumberFollow[1];
	static long					SSLR_negTermFollow[1];
	static long					SSLR_termListFollow[3];
	static long					SSLR_errorActionFollow[1];
	static long					SSLR_errorAction2Follow[1];
	static long					SSLR_errorMsgFollow[2];


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


/**********************************************
** hier stehen Deklarationen, die später im **
** Header-File des erzeugten Parsers stehen **
**********************************************/
private:
//Hier werden die Produktionen abgelegt, ohne die rechte Seite.
//Die rechten Seiten sind in PROD2_STR abgelegt.
typedef struct
{
//Index der Produktion in der Hash-Tabelle
long	hash_index;
//Index der ersten Alternative in PROD2_STR
long	prod2_index;
//Anzahl der Alternativen dieser Produktion
int		altAnz;
}PROD_STR;
//Hier werden die zu den Produktionen gehörenden rechten Seiten abgelegt.
typedef struct
{
//Index der Produktion in PROD_STR
long			prod_index;
//Zeile, in der die Alternative steht
long			line;
//Anzahl der Elemente auf der rechten Seite dieser Alternative
int				riAnz;
//Index der Hash-Einträge
long			*right;
//Priorität der Alternative
int				priority;
//String mit semantischer Aktion
char			*semAct;
//Zeile, in der die semantische Aktion angegeben wurde
unsigned long	semActLine;
//false: normale semantische Aktion, true: embedded semantic action
bool			isEmbeddedSemAct;
//falls in Folge eines Reduce-Reduce-Fehlers mit dieser Alternative nie reduziert wird
int				reduce;
}PROD2_STR;
//%union
typedef struct
{
/*
Beispiel:
int		a;
long	b;
bool	c;
*/
//Typen
char		**type;
//Bezeichner
char		**id;
/*
Die union-Angabe wird zwar gescannt um die Richtigkeit der semantischen Typen zu bestimmen,
aber in den Quelltext wird die Angabe des Grammatik-Files unverändert übernommen, weil sonst
z.B. Pointer und Kommentare verschwinden würden.
*/
char		*text;
//Anzahl der Elemente
int			anz;
}UNION_STR;
//Hier stehen die Terminale, die mit einer bestimmten Fehleraktion verbunden sind
typedef struct
{
//Terminal, bei dem eine bestimmte Fehleraktion ausgeführt werden soll
long	terminal;
//0: Fehleraktion bei diesem Terminal, 1: bei allen anderen (Negation)
int		neg;
}ERRTERM_STR;
/*
Wenn eine Fehlermeldung für ein LR-Item ausgegeben werden soll, so stehen hier
die Hüllen-Indices, in denen sich ein LR-Item befindet, für das die Fehlermeldung
ausgegeben werden soll. Dies entspricht später in der SSLR_error-Routine des
erzeugten Parsers den case-Anweisungen.
*/
typedef struct
{
//Index der Hüllen
ulong	*arr;
//Anzahl der Einträge in arr
ulong	anzArr;
}HulleErrorIndexStr;
//%error
/*
Man hat die Möglichkeit, die Fehler-Behandlung beim Parsen zu beeinflussen.
Aufbau:
%error
{
Poition des Punktes eines Elementes:Aktion:Fehlermeldung
}
Aktion:
ESHIFT(TERMINAL):		TERMINAL einfügen
EPOP(TERMINAL):			solange Zustände vom Parse-Stack entfernen,
bis TERMINAL geschoben werden darf
EDELETE:				ein Terminal löschen
EOVERREAD(TERMINAL):	bis TERMINAL erscheint, alles überlesen
EEXIT:					Parsing beenden
ESEM:					semantische Aktionen trotz Fehler zulassen
*/
typedef struct
{
//Index der Produktion in PROD_STR
long				prod_index;
//Nummer der Alternative, also Index in der PROD2_STR
long				prod2_index;
//Position des Punktes (LR-Item)
int					dot;
//ESHIFT(TERMINAL)
long				shift;
//EPOP(TERMINAL)
long				pop;
//EOVERREAD(TERMINAL)
long				overread;
//ESEM
int					sem;
//EEXIT
int					exit;
//EDELETE
int					del;
//Fehlermeldung
char				*msg;
//Zeile der Deklaration der Fehleranweisung
long				line;
//hier stehen die termArr-Indices der Terminale, für die eine Aktion augeführt werden soll
int					*errTermArr;
//Terminale, verbunden mit Fehleraktionen
ERRTERM_STR			*errTermStr;
//Anzahl der ERRTERM_STR-Strukturen
int					errTermStrAnz;
/*
Wenn eine Fehlermeldung für ein LR-Item ausgegeben werden soll, so stehen hier
die Hüllen-Indices, in denen such dieses LR-Item befindet.
*/
HulleErrorIndexStr	hInd;
}ERROR_STR;
/*
Hier stehen die LR-Items und deren zugehöriger Hüllen-Index, die reduzieren
mit der richtigen Lookahead-Menge. Dies wird benötigt, um die Zustände
zu finden, die für die %error-Anweisung in Frage kommen.
*/
typedef struct
{
//Index der Produktion in PROD_STR des LR-Items
long	prod_index;
//Nummer der Alternative, also Index in der PROD2_STR des LR-Items
long	prod2_index;
//Index der Hülle, in der es steht
ulong	hulleIndex;
}LRReduceErrorItem;
//EBNF-Stack für die Analyse der Grammatik
typedef struct
{
//Index der Produktion in PROD_STR
long	prod_index;
//Index der Alternative in PROD2_STR
long	prod2_index;
//Nummer der Alternativen
int		altCount;
}EBNF_STACK;
//Struktur für EIN LR(0/1)-Element
typedef struct
{
//Index in PROD2_STR
long		prod2_index;
//Position des Punktes im LR-Item
int			dot;
//Hülle, die aus diesem Item durch Sprung erzeugt wird
long		jumpTo;
//Lookahead-Terminale
Terminal	*la;
//spontan generierte Terminale bei LALR
Terminal	*lalr;
}LR_STR;
//Hülle-Struktur
typedef struct
{
//Anzahl der Kerne
int		anzCore;
//Anzahl der LR(0)-Elemente
int		anzElem;
//LR(0/1)-Elemente
LR_STR	*lr_str;
/*
Wenn für diese Hülle gilt:
this = Sprung( i, X ),
dann ist from = i und elem = X.
*/
long	from;
long	elem;
//Index dieser Hülle in hulle_str
long	hulleIndex;
}HULLE_STR;
//LR-Tabelle
typedef struct
{
/*
1: Fehler
2: Shift
3: Reduce
4: Akzeptieren
*/
int		state;
/*
bei Shift: zu schiebender Zustand
bei Reduce: Index der Alternative, d.h. prod2_index
*/
long	value;
//bei Shift: Hash-Index des Terminals, bei dem geschoben wird
long	termHashIndex;
}LR_TAB;
//Struktur für semantische Aktionen u.a. wegen embedded actions wie bei YACC
typedef struct
{
//Text mit semantischer Aktion
char	*semAct;
//Zeilennummer
long	line;
}SEMACT_STR;
//links-assoziativ
static const int			isLeftAssoc = 1;
//rechts-assoziativ
static const int			isRightAssoc = 2;
//nicht-assoziativ
static const int			isNonAssoc = 3;
//Typ eines LR-Items
static const int			SSLR_CORE = 0;
static const int			SSLR_ITEM = 1;
//mögliche Zustände in der LR-Tabelle
static const int			SSLR_ERROR = 1;
static const int			SSLR_SHIFT = 2;
static const int			SSLR_REDUCE = 3;
static const int			SSLR_ACCEPT = 4;
//Informationen über das Grammatik-File
struct stat					st;
//Hash-Tabelle
hashClass					*hash_tab;
//Array für die Produktionen
PROD_STR					*prod_str;
//Anzahl der Produktionen
int							prod_anz;
//Array für alle rechten Seiten
PROD2_STR					*prod2_str;
//Anzahl aller rechten Seiten
int							prod2_anz;
//Array für die %error-Angaben
ERROR_STR					*error_str;
//Anzahl der Einträge in error_str
int							error_anz;
/*
Die Verwendung der Terminale/Nichtterminale wird geprüft. In diesem Array
sind die Hash-Indices aufgereiht.
*/
long						*check;
//Anzahl der Einträge in check
long						anzCheck;
//für %union-Definition
UNION_STR					*unionStr;
//Argumente
ARGV_STR					*argv_str;
//Hash-Index von EPSILON
long						epsHashIndex;
//Hash-Index von ACCEPT
long						acceptHashIndex;
//Check-Terminal # bei Generierung von LALR-Parsern
long						lalrHashIndex;
//Array mit allen Hüllen
HULLE_STR					**hulle_str;
//Anzahl der Hüllen
long						hulle_anz;
//Anzahl aller Kerne
long						coreAnz;
//LR-Tabelle
LR_TAB						**lrTab;
//Sprung-Tabelle
int							**jumpTab;
//Array mit den Hash-Indices aller Terminale
long						*termArr;
//Anzahl der Einträge in termArr
int							termArrCount;
//Array mit den Hash-Indices aller Nichtterminale
long						*ntermArr;
//Anzahl der Einträge in ntermArr
int							ntermArrCount;
//Anzahl der Shift-Reduce-Fehler
int							srError;
//Anzahl der Reduce-Reduce-Fehler
int							rrError;
//Zähler für semantische Aktionen
int							semActCnt;
//Struktur für semantische Aktionen
SEMACT_STR					semActStr;
//es gibt eine einzutragende semantische Aktion
bool						semAct;
//Priorität eines Terminals
int							priority;
//Assoziativität eines Terminals
int							assocDef;
//Definition einer %union-Struktur
bool						unionDef;
//Index der Produktion in PROD_STR
long						prod_index;
//Index der Alternative in PROD2_STR
long						prod2_index;
//%ntermDef, also für Nicht-Terminale, oder %termDef für Terminale in %definition
int							typeDef;
//Fehler währen des Parsens
int							err;
/*
Wenn %termDef/%ntermDef <semantischer Typ> (in der %definition-Sektion)
angegeben wurde, dann prüfen, ob dieser semantische Typ auch in der
%union-Struktur angegeben wurde.
*/
int							semIndex;
//Index des Elements in der HASH_TAB
long						hash_index;
/*
Die Priorität eines Terminals wird indirekt durch %left/%right angegeben: Je tiefer die
Angabe steht, desto größer ist die Priorität des Terminals.
*/
unsigned long				priLine;
//Anzahl der Alternativen der aktuellen Produktion prod_index
int							altCount;
/*
Bei der Verwendung von EBNF-Symbolen treten Hierachien auf,
für die diverse Informationen gesichert werden müssen.
*/
EBNF_STACK					*ebnfStack;
//Stack-Pointer für den EBNF-Stack
int							ebnfSP;
/*
Bei der Verwendung von eingebetteten semantischen Aktionen
sowie bei der verwendung von EBNF-Symbolen müssen
dummy-Nichtterminale eingeschoben werden, die hier
auxsym heissen.
*/
char						*auxsym;
//Zähler für die Anzahl der auxsym-Nichtterminale
int							auxsym_count;
/*
Bei der Angabe der Grammatik kann man Fehlerangeben machen, diese
Struktur enthält pro Alternative und Position des Punktes in einem
LR-Item diese Informationen.
*/
ERROR_STR					*errorStr;
//Index der aktuellen errorStr
long						errorIndex;
//gibt an, ob die Terminale negiert werden sollen
bool						errorNeg;
/*
Länge der längsten rechten Seite einer Alternative. Wird für die
formatierte Ausgabe der rechten Seite einer Alternative für die
Debug-Version des Parsers benötigt.
*/
int							maxAltRightSideWidth;
//längstes Terminal/Nichtterminal
int							maxElemWidth;
//Breite des größtes Zustands
int							maxStateWidth;
/*******************
* private Methoden *
*******************/
//Überprüfung der (Nicht)Terminals
void setCheck( long );
//Neues %union-Paar eintragen
void insertUnion( char *, char * );
//Index des Type der semantischen Aktion in unionStr finden
int getSemIndex( char * );
//eine neue Produktion anfordern
long getNewProd( void );
//eine neue Alternative anfordern
long getNewProd2( long );
//ein Elemente in eine rechte Seite einer Alternative eintragen
void insertElement( long, long, long );
//die Fehleraktionen auf doppelte EInträge prüfen
int checkErrorStr( void );
//PROD2_STR sortieren
void sortProd2Str( void );
//Grammatik in Datei ausgeben
void outputGrammar( void );
//alle Alternativen ausgeben
void outputAllAlt( void );
//eine Alternativen ausgeben
void outputAlt( ostream &, long );
//eine Hülle ausgeben
void outputHulle ( HULLE_STR * );
//ein LR-Item in ofstream file ausgeben
void outputItem( long, int, ofstream & );
//ein LR-Item auf cout ausgeben
void outputItem( long, int );
//Start-Produktion eintragen
void insertStartProd( long &, long & );
//(Nicht)Terminale überprüfen
int checkAll( void );
//prüfen, ob mit allen Alternativen reduziert wurde
int checkAllReduce( void );
//alle FIRST-Mengen bilden
void genAllFirst( void );
//hier werden die FIRST-Mengen eines Terminals/Nichtterminals erzeugt
void genFirst( Terminal *, int, long * );
//alle FOLLOW-Mengen bilden
void genAllFollow( void );
//prüft, ob die Hülle h schon in hulle_str enthalten ist
int cmpLR0Hulle( HULLE_STR * );
//prüft, ob die Hülle h schon in hulle_str enthalten ist
int cmpLR1Hulle( HULLE_STR * );
//alle Hüllen der SLR und LALR bilden
void genLR0Hulle( void );
//alle Hüllen der LR
void genLR1Hulle( void );
//LR(0)-Kern in HULLE h eintragen
void insertItem( HULLE_STR *, long, int, int );
//LR(1)-Kern in HULLE h eintragen
void insertItem( HULLE_STR *, long, int, int, Terminal * );
//eine HULLE_STR-Struktur erzeugen
HULLE_STR *createHulle( void );
//eine HULLE_STR-Struktur löschen
void deleteHulle( HULLE_STR * );
//prüft, ob in der Hülle h das LR-Item schon enthalten ist
int findItem( HULLE_STR *, long );
//prüft, ob in der Hülle h das LR-Item schon enthalten ist
int findItem( HULLE_STR *, long, int );
//prüft, ob in der Hülle h das LR-Item schon enthalten ist
int findItem( HULLE_STR *, long, int, Terminal * );
//eine Hülle in die Sammlung von Elementen eintragen
void insertHulle( HULLE_STR * );
//LR(0)-Items der Hülle erzeugen
void genLR0Items( HULLE_STR * );
//LR(1)-Items der Hülle erzeugen
void genLR1Items( HULLE_STR * );
//hier werden die Hüllen erzeugt, die sich durch Spring( h, X ) ergeben
void makeLR0Jump( HULLE_STR * );
//hier werden die Hüllen erzeugt, die sich durch Spring( h, X ) ergeben
void makeLR1Jump( HULLE_STR * );
//aus der Sammlung der LR(0)-Elemente die Lookaheads anfügen
void genLALR( void );
//die Hülle erzeugen, die sich durch Sprung( I0, index ) ergibt
void genLR0Jump( HULLE_STR *, HULLE_STR *, long );
//die Hülle erzeugen, die sich durch Sprung( I0, index ) ergibt
void genLR1Jump( HULLE_STR *, HULLE_STR *, long );
//Terminale ausgeben
void outputTerminal( Terminal * );
//Index der Hülle in die entsprechenden Elemente von I0 eintragen, die durch Sprung( I0, elem )
//erzeugt wird.
void setJump( HULLE_STR *, long, long );
//hier werden die LR(0)-Kerne beider Hüllen verglichen
bool cmp2LR0Hulle( HULLE_STR *, HULLE_STR * );
//wandelt die LR(1)-Hüllen in LR(0)-Hüllen
void LR2LALR( void );
//LR-Tabelle erzeugen
void LRTab( void );
//einen Reduce-Eintrag erzeugen
void setReduce( int, long, Terminal * );
//einen Shift-Eintrag erzeugen und dabei Shift-Reduce-Fehler behandeln
void setShift( int, long, int );
//hier werden die Source-Files des Parsers erzeugt
void generateFiles( void );
//LR-Tabelle in Datei ausgeben
void outputLRTab( ofstream & );
//Sprung-Tabelle ausgeben
void outputJumpTab( ofstream & );
/*
Der Scanner liefert ja Tokens zum Parser zurück, die im Prinzip
für die Auswahl der nächsten Aktion (schieben/reduzieren) benötigt
werden. In diesem File werden die #defines der Terminale geschrieben,
sowie die Definition der %union-Angabe, auf die im Aktionsteil einer
Regel in SSLEX zugegriffen werden kann/muss.
*/
void outputTermDefHFile( ofstream & );
//Header-File ausgeben
void outputHFile( ofstream & );
//Terminal-Defines als enum ausgeben
void outputCPPTerminal( ofstream & );
//die Alternativen in die cpp-Datei ausgeben.
void outputAlternatives( ofstream & );
//Follow-Mengen in cpp-File ausgeben
void outputFollow( ofstream & );
//die SSLR_NTERM_STR ausgeben
void outputNtermStr( ofstream & );
//die Namen der Terminale ausgeben, nur im debug-mode
void outputTermName( ofstream & );
//hier wird die komplette Fehlerbehandlung ausgegeben
void outputErrorRecovery( ofstream & );
//den Header des cpp-Files ausgeben
void outputCPPHeader( ofstream & );
//alle übrigen Routinen des cpp-Files ausgeben
void outputCPPEnd( ofstream & );
//erzeuge eine neue ERROR_STR und liefere den Index zurück
int getErrorStr( void );
//fügt eine ERRTERM_STR zu einer ERROR_STR hinzu.
int insertErrTermStr( ERROR_STR *, long, int );
//semantische Aktionen ausgeben
void outputSemAct( ofstream & );
//Include-Anweisungen in das cpp-File ausgeben
void outputIncl( ofstream & );
//eigene Erweiterungen in das cpp-File ausgeben
void outputExt( ofstream & );
//doppelte Alternativen finden
int findDoubleAlt( void );
//fügt ein auxsym-Nichtterminal auf rechter Seite von prod2_index ein für eine embedded semantic
//action.
void insertEmbeddedSemanticAction( long, long, long, int & );
/*
Diese Routine wird vom Konstruktor der SSLRParser-Klasse aufgerufen, wenn
im %include-Teil das Makro SSLR_INIT definiert wurde.
*/
void init( void );
/*
Diese Routine wird vom Destruktor der SSLRParser-Klasse aufgerufen, wenn
im %include-Teil das Makro SSLR_INIT definiert wurde.
*/
void deinit( void );
/*
Innerhalb der Grammatik kann man mittels %error Fehleranweisungen
in den erzeugten Parser einbauen.
Ein LR-Parser kann aber nötige Reduktionen verweigern, wenn das
darauffolgend zu schiebende Terminal nich als Lookahead vorliegt.
Also werden hier die Zustände gefunden, die vor dem Item mit der
Fehleranweisung stehen und die reduzieren.
*/
void processErrorStr( void );
/*
Hier wird in der Hülle hulleIndex nach einem LR-Item gesucht,
das reduziert, wobei seine LA-Symbole mind. t umfassen müssen.
*/
bool findReduceItem( unsigned long, unsigned long &, Terminal * );
/*
Hier wird geprüft, ob auf der rechten Seite des LR-Item item
der Hülle hStr das Nicht-Terminal hash_index auftaucht.
*/
int findNTERMRightSide( HULLE_STR *, int, unsigned long );
//liefere den Index des Terminals in termArr zurück
int getTermArrIndex( unsigned long );
public:
/*
Diese Funktion dient der Übergabe der Kommandozeilen-Argumente, die
SSLR steuern.
*/
void giveArgs( ARGV_STR * );
};


#endif

