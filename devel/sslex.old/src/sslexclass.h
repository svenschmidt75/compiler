/***************************************************************************
                          sslexclass.h  -  description
                             -------------------
    begin                : Mon Feb 3 2003
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

#ifndef SSLEXCLASS_H
#define SSLEXCLASS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/**
  *@author Sven Schmidt
  */


//wichtig
using namespace std;

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <string>
#include <assert.h>

#include "sslex.h"
#include "_sslexscanner.h"


//define debug-macro
#define SSLR_DEBUG
#undef SSLR_DEBUG


//eigene Defines


//NEA darf nur am Zeilenanfang stehen
#define BEGLINE_NEA	0
#define NORM_NEA	1




//Number of States
#define	SSLR_STATE	91

//Number of Terminals = Number of columns in lr_tab
#define	TERM	45


//Number of Productios = Number of rows in jump_tab
#define	NTERM	24


//Number of Alternatives
#define	PROD	68


//Actions in lr_tab
#define	LR_ERROR	1
#define	LR_SHIFT	2
#define	LR_REDUCE	3
#define	LR_ACCEPT	4

//maximum stack size
#define MAX_STACK 100


//error routines
#define DELETE	errDelete
#define SHIFT	errShift
#define OVERREAD	errOverRead
#define SEM	semAct = 0
#define EXIT	exit( 1 )


//Nonterminal-Defines for Jump-Table
#define start	0
#define sconname	1
#define scon	2
#define rule	3
#define re2	4
#define sc2	5
#define flexrule	6
#define re	7
#define goal	8
#define fullccl	9
#define string	10
#define initforrule	11
#define initlex	12
#define series	13
#define ccl	14
#define startconddecl	15
#define sect1	16
#define sect2	17
#define scsect1	18
#define ccl_expr	19
#define singleton	20
#define namelist1	21
#define namelist2	22
#define scon_stk_ptr	23


//Nontermials for lr-Table
#define start_0	0
#define goal_0	1
#define initlex_0	2
#define sect1_0	3
#define sect1_1	4
#define scsect1_0	5
#define scsect1_1	6
#define startconddecl_0	7
#define startconddecl_1	8
#define namelist1_0	9
#define namelist1_1	10
#define sect2_0	11
#define sect2_1	12
#define sc2_0	13
#define sc2_1	14
#define sc2_2	15
#define initforrule_0	16
#define flexrule_0	17
#define flexrule_1	18
#define flexrule_2	19
#define scon_stk_ptr_0	20
#define scon_0	21
#define scon_1	22
#define scon_2	23
#define namelist2_0	24
#define namelist2_1	25
#define sconname_0	26
#define rule_0	27
#define rule_1	28
#define rule_2	29
#define rule_3	30
#define re_0	31
#define re_1	32
#define re2_0	33
#define series_0	34
#define series_1	35
#define singleton_0	36
#define singleton_1	37
#define singleton_2	38
#define singleton_3	39
#define singleton_4	40
#define singleton_5	41
#define singleton_6	42
#define singleton_7	43
#define singleton_8	44
#define singleton_9	45
#define singleton_10	46
#define singleton_11	47
#define fullccl_0	48
#define fullccl_1	49
#define ccl_0	50
#define ccl_1	51
#define ccl_2	52
#define ccl_3	53
#define ccl_expr_0	54
#define ccl_expr_1	55
#define ccl_expr_2	56
#define ccl_expr_3	57
#define ccl_expr_4	58
#define ccl_expr_5	59
#define ccl_expr_6	60
#define ccl_expr_7	61
#define ccl_expr_8	62
#define ccl_expr_9	63
#define ccl_expr_10	64
#define ccl_expr_11	65
#define string_0	66
#define string_1	67




//für das Wiederholen von NEAen
#define _INFINITY	-1

//Zustände setzen
#define MARK_AS_NORMAL	0
#define MARK_AS_FINIT	1
#define MARK_AS_LA		2




//Schreibarbeit ersparen
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;



//LR-Table
typedef struct
{
	int			state;
	long		value;

}LR_TAB;



//all prodctions for debug-mode
typedef struct
{
	//name of alternative
	char	*name;

	//pointer to right side
	char	**ri;

	//index in jump_tab
	int		jump_index;

	//number of elements on the right side
	int		elem;

	//pointer to follow-sets
	long	*follow;

	//number of follow-terminals
	long	followCnt;

}NTERM_STR;




//this structure saves important values for the error-recovery
typedef struct
{
	//stackpointer position
	int		sp;

	//state after reduction
	int		state;

	//number of alternative the reduction was done
	int		alt;

}ERR_STR;


//Struktur für der Erzeugung der Debug-Informationen im generierten Scanner
typedef struct
{
	//Zeiger auf NEA
	NEA_STR	*nea;

	//Index im Array mit den entsprechenden reg. Ausdrücken in Textform
	ulong	index;

}DEBUG_STR;




//Struktur für die Zustandsübergänge
typedef struct
{
	//Zeichen-Klasse, bestimmt den Übergang
	int		*cclClass;

	//jeweiliger Zustand ist ein Epsilon-Übergang
	bool	eps;

	//Index des Ziel-Zustandes
	ulong	bra;

}BRA_STR;


//Struktur für die NEA-Zustände
typedef struct stateStr
{
	//Nummer des Zustandes im Zustands-Array
	//-1: kein Übergang!
	ulong			state;

	//entsprechender NEA, nur bei Start-, End- und Lookahead-Zuständen
	NEA_STR			*nea;

	//hat epsilon-Übergänge
	bool			eps;

	//Ist Finit-Zustand
	bool			finit;

	//Mithilfe des folgenden Flags wird geprüft, ob die Regel jemals ausgeführt werden kann
	bool			isUseful;

	//ist Lookahead-End-Zustand
	bool			la;

	//Array mit den Übergängen
	BRA_STR			**braStr;

	//Anzahl der Übergänge von diesem NEA
	ulong			braStrCnt;

}STATE_STR;




//Struktur für die DEA-Zustandsübergangs-Tabelle
typedef struct
{
	//DEA-Index
	ulong	index;

	//Array mit der Breite cclWidth mit den Ziel-Zuständen
	ulong	*to;
//vorher:	long	*to;
}DTRAN;



//Struktur zur Implementierung der Gruppen für die Zustands-Minimierung, s. rache S. 172
typedef struct
{
	//Index dieser Gruppe
	long	index;

	//Index der Gruppe der Ziel-Zustände
	long	destIndex;

	//die zu dieser Gruppe gehörenden DEA-Zustände
	ulong	*DEAIndex;

	//Anazhl der DEA-Zustände in dieser Gruppe
	ulong	DEAIndexCnt;

	//Gruppe ist valid
	bool	valid;

	//enthält Finit-Zustände
	bool	finit;

	//enthält Lookahead-Zustände
	bool	la;

	//enthält den Start-Zustand
	bool	startGroup;

	//Zeiger auf den NEA, zu dem die akzeptierenden Zustände gehören, wenn akzeptierende Gruppe
	NEA_STR	*finitNEA;

	//DEA-Index eines der Lookahead-Zustände
	ulong	laIndex;

}GROUP_STR;



//Struktur für die SC
typedef struct
{
	//Name der SC
	char		*name;

	//true: exclusive, sonst inclusive
	bool		type;

	//Array mit allen NEAen, die zu dieser SC gehören
	NEA_STR		**neaArr;

	//Anzahl der NEA_STR in neaArr
	long		neaArrCnt;

	//Anzahl der NEAen, die überall stehen dürfen
	long		nea1Anz;

	//Anzahl der NEAen, die nur am Zeilenanfang stehen dürfen
	long		nea2Anz;

	//DEA-Zustandsübergangstabelle für die NEAen, die überall stehen dürfen
	DTRAN		**DTran1;

	//Anzahl der Zeilen in der DEA-Übergangstabelle
	ulong		DTran1Cnt;

	//DEA-Zustandsübergangstabelle für die NEAen, die nur am Anfang stehen dürfen
	DTRAN		**DTran2;

	//Anzahl der Zeilen in der DEA-Übergangstabelle
	ulong		DTran2Cnt;

	//bei Zustandsminimierung: Array mit den Gruppen der DEA-Zustände
	GROUP_STR	**grp;

	//Anzahl der Gruppen
	ulong		grpCnt;

	//Start-Zustand des DEA mit NEAen, die überall stehen dürfen
	long		DEAIndex;

	//Start-Zustand der NEAen, die NUR am Zeilenanfang stehen dürfen
	long		begLineDEAIndex;

	//DEA-Zustand der NEAen, die überall stehen dürfen (nach Zustandsminimierung)
	long		firstStart;

	//DEA-Zustand der NEAen, die nur am Zeilenanfang stehen dürfen (nach Zustandsminimierung)
	long		secondStart;

}SC_STR;



//Struktur für die Erzeugung von e-Closure
typedef struct
{
	//Array mit den Indices, die einen Epsilon-Übergang zu to haben
	ulong	*from;

	//Anzahl der Indices in from
	ulong	fromCnt;

	//Array mit den Zuständen, die von from über Epsilon führen
	ulong	*to;

	//Anzahl der Indices in to
	ulong	toCnt;

}ECLOS_STR;


//Struktur für die Erzeugung von move (Drachenbuch S. 145)
typedef struct
{
	//Übergangszeichen
	uchar	jump;

	//Array mit den Indices, die einen Übergang mit jump nach to haben
	ulong	*from;

	//Anzahl der Indices in from
	ulong	fromCnt;

	//Array mit den Zuständen, die von from über jump führen
	ulong	*to;

	//Anzahl der Indices in to
	ulong	toCnt;

}MOVE_STR;



//Struktur für eine Menge von NEA-Zuständen, die einem DEA-Zustand entspricht
typedef struct
{
	//Index dieser Zustandsmenge bzw. Index dieses DEA-Zustandes
	ulong	deaIndex;

	//markiert/unmarkiert
	bool	marked;

}DSTATES;


//Struktur für ALLE DEA-Zustände
typedef struct
{
	//Index des DEA-Zustands
	ulong	index;

	//Index der Gruppe, in der dieser DEA-Zustand ist
	ulong	groupIndex;

	//die zu diesem DEA-Zustand gehörenden NEA-Zustände
	ulong	*neaIndex;

	//Anzahl der NEA-Zustände
	ulong	neaIndexCnt;

	//ist Finit-Zustand
	bool	finit;

	//ist Lookahead-Zustand
	bool	la;

	//toter Zustand, benötigt für die DEA-Zustandsminimierung
	bool	dummy;

	//Wenn es ein End-Zustand ist, dann die zugehörige NEA_STR eintragen
	NEA_STR	*finitNEA;

	//Wenn es ein Lookahead-Zustand ist, dann die zugehörige NEA_STR eintragen
	NEA_STR	*laNEA;

}DEA_STATES;




class ssLexClass
{
private:

	//Daten mit der Grammatik-Datei
	char			*data;

	//command-line Argumente
	ARGV_STR		*argv_str;

	//Scanner
	SSLEXScanner	*s;

	//true: exclusive SC, sonst inclusive
	bool			xcluSC;

	//Stack mit den SC, die mit dem aktuellen NEA verbunden werden sollen
	SC_STR			**scStack;

	//Position auf dem SC-Stack
	long			scStackPos;

	//Struktur mit den in %definition definierten SC
	SC_STR			*scStr;

	//Anzahl der SC
	long			scStrAnz;

	//Fehler
	int				err;

	//trailing context
	bool			trailingContext;

	/*
		Wenn eine Aktion | als erstes Zeichen hat, dann sind die Aktionen gleich.
		Dazu werden die entsprechenden NEA_STR auf einen extra Stack gelegt.
	*/
	NEA_STR			**neaStack;

	//Anzahl der Elemente in neaStack
	ulong			neaStackCnt;

	//Größe des Stacks
	ulong			neaStackMax;

	//DSTATES-Stack
	DSTATES			**dstatesStack;

	//Anzahl der Einträge in dstatesStack
	ulong			dstatesStackPos;

	//Array mit den Zeigern auf alle DEA-Zustände
	DEA_STATES		**DEAStates;

	//alle DEA-Zustände werden fortlaufend nummeriert
	ulong			DEAStatesCnt;

	//Zahl der erzeugten Gruppen bei DEA-Zustandsminimierung
	ulong			numberOfGroups;

	//7- oder 8-Bit Scanner erzeugen
	int				cclWidth;

	//Array mit den Zuständen
	STATE_STR		**stateStr;

	//Anzahl der Einträge in stateStr
	ulong			stateStrCnt;

	//den maximalen Gruppenindex merken
	ulong			maxGroupIndex;

	//zeigt an, ob Lookahead-Rules benutzt wurden
	bool			laUsed;

	//zeigt an, ob begin-of-line-Rules benutzt wurden
	bool			begLineUsed;

	//wird in outputCPPMinDEA erzeugt und dient zur Ausgabe der Aktionen
	NEA_STR			**finitNEA, **laNEA;

	//wird bei der Ausgabe der Debug-Infos benötigt
	ulong			debugStrPos;

	//Regeln zählen
	ulong			ruleCnt;




	//Stack
	long				stack[MAX_STACK];

	//stack-counter
	long				sp;


	//LR-Table
	static LR_TAB		lr_tab[SSLR_STATE][TERM];

	//Jump-Table
	static long			jump_tab[SSLR_STATE][NTERM];

	//Array with all alternatives, for debug-mode only
	static NTERM_STR	nterm_str[PROD];

	//array with the names of the terminals
	static char			*term_name[TERM];

	//current terminal
	int					terminal;

	//attributes returned form scanner
	SSLR_TYPE			attributeStr;

	//line number
	unsigned long		line;

	//current state
	int					state;

	//number of errors
	int					anzError;

	//structure for error-recovery
	ERR_STR				errStr;

	//semantische Aktionen steuern
	int					semAct;

	//stack for semantic actions
	SSLR_TYPE			semStack[MAX_STACK];

	//counter for stack
	int					semStackCnt;

	/*
		Nach einem Fehler müssen mind. ERR_SHIFT Terminale geschoben werden, damit keine
		Fehlerlawinen auftreten.
	*/
	bool				regShift;

	//Alternatives
	static char	*start0Ri[1];
	static char	*goal0Ri[4];
	static char	*initlex0Ri[1];
	static char	*sect10Ri[2];
	static char	*sect11Ri[1];
	static char	*scsect10Ri[3];
	static char	*scsect11Ri[1];
	static char	*startconddecl0Ri[1];
	static char	*startconddecl1Ri[1];
	static char	*namelist10Ri[2];
	static char	*namelist11Ri[1];
	static char	*sect20Ri[2];
	static char	*sect21Ri[1];
	static char	*sc20Ri[5];
	static char	*sc21Ri[5];
	static char	*sc22Ri[1];
	static char	*initforrule0Ri[1];
	static char	*flexrule0Ri[2];
	static char	*flexrule1Ri[1];
	static char	*flexrule2Ri[1];
	static char	*scon_stk_ptr0Ri[1];
	static char	*scon0Ri[4];
	static char	*scon1Ri[3];
	static char	*scon2Ri[1];
	static char	*namelist20Ri[3];
	static char	*namelist21Ri[1];
	static char	*sconname0Ri[1];
	static char	*rule0Ri[2];
	static char	*rule1Ri[3];
	static char	*rule2Ri[2];
	static char	*rule3Ri[1];
	static char	*re0Ri[3];
	static char	*re1Ri[1];
	static char	*re20Ri[2];
	static char	*series0Ri[2];
	static char	*series1Ri[1];
	static char	*singleton0Ri[2];
	static char	*singleton1Ri[2];
	static char	*singleton2Ri[2];
	static char	*singleton3Ri[6];
	static char	*singleton4Ri[5];
	static char	*singleton5Ri[4];
	static char	*singleton6Ri[1];
	static char	*singleton7Ri[1];
	static char	*singleton8Ri[1];
	static char	*singleton9Ri[3];
	static char	*singleton10Ri[3];
	static char	*singleton11Ri[1];
	static char	*fullccl0Ri[3];
	static char	*fullccl1Ri[4];
	static char	*ccl0Ri[4];
	static char	*ccl1Ri[2];
	static char	*ccl2Ri[2];
	static char	*ccl3Ri[1];
	static char	*ccl_expr0Ri[1];
	static char	*ccl_expr1Ri[1];
	static char	*ccl_expr2Ri[1];
	static char	*ccl_expr3Ri[1];
	static char	*ccl_expr4Ri[1];
	static char	*ccl_expr5Ri[1];
	static char	*ccl_expr6Ri[1];
	static char	*ccl_expr7Ri[1];
	static char	*ccl_expr8Ri[1];
	static char	*ccl_expr9Ri[1];
	static char	*ccl_expr10Ri[1];
	static char	*ccl_expr11Ri[1];
	static char	*string0Ri[2];
	static char	*string1Ri[1];

	//all Follow-Sets
	static long	startFollow[1];
	static long	goalFollow[1];
	static long	initlexFollow[3];
	static long	sect1Follow[2];
	static long	scsect1Follow[4];
	static long	startconddeclFollow[1];
	static long	namelist1Follow[5];
	static long	sect2Follow[1];
	static long	sc2Follow[12];
	static long	initforruleFollow[9];
	static long	flexruleFollow[1];
	static long	scon_stk_ptrFollow[1];
	static long	sconFollow[9];
	static long	namelist2Follow[2];
	static long	sconnameFollow[2];
	static long	ruleFollow[1];
	static long	reFollow[5];
	static long	re2Follow[6];
	static long	seriesFollow[11];
	static long	singletonFollow[15];
	static long	fullcclFollow[15];
	static long	cclFollow[14];
	static long	ccl_exprFollow[14];
	static long	stringFollow[2];




	/*******************
	* private Methoden *
	*******************/


	//Push a new state on stack
	void Push( long );

	//pop anz elements from stack
	long Pop( int );

	//begin parsing
	int Parse( void );

	//Shift
	void Shift( LR_TAB * );

	//Reduce
	void Reduce( LR_TAB * );

	//error-handling
	void error( void );

	//shift a new state
	void errShift( long );

	//delete next terminal
	void errDelete( void );

	//delete all terminals till term is found
	void errOverRead( long );

	//output an alternative
	void outputAlt( NTERM_STR * );

	//do semantic actions
	void doSemAct( LR_TAB * );

	//get next Terminal
	int getTerminal( void );

	//einen neuen NEA anfordern
	NEA_STR *getNewNEA( void );

	//einen neuen Zustand anfordern
	ulong getNewState( NEA_STR * );

	//neue Zeichenklasse anfordern
	int *cclInit( void );

	//eine Zeichenklasse gem. . (any-char) erzeugen
	int *cclAny( void );

	//einen Bereich wie [a-z] eintragen
	int *cclAddRange( int *, uchar, uchar );

	//ein Zeichen zu einer Zeichenklasse hinzufügen
	int *cclAdd( int *, uchar );

	//eine Zeichenklasse an eine andere anfügen
	int *cclAdd( int *, int * );

	//eine Zeichenklasse negieren
	int *cclNegate( int * );

	//eine vordefinierte Zeichenklasse installieren
	int *cclClass( int *, int );

	//eine Zeichenklasse kopieren
	int *cclCopy( int * );

	//einen Zustandsübergang mit cclStr erzeugen
	void mkBranch( ulong, ulong, int *, bool );

	//einen Zustand an einen NEA anhängen
	void mkBranch( NEA_STR *, int *, bool );

	//zwei NEAen verodern
	NEA_STR *mkor( NEA_STR *, NEA_STR * );

	//zwei NEAen verketten
	NEA_STR *mkLink( NEA_STR *, NEA_STR * );

	//einen NEA beliebig oft ausführbar machen
	NEA_STR *mkClos( NEA_STR * );

	//einen NEA mind. einmal ausführbar machen
	NEA_STR *mkPosClos( NEA_STR * );

	//einen NEA optional machen
	NEA_STR *mkOpt( NEA_STR * );

	//einen NEA mndestens min-mal ausführen, maximal max-mal
	NEA_STR *mkRepeat( NEA_STR *, int, int );

	//einen NEA genau min-mal ausführen
	NEA_STR *mkRepeat( NEA_STR *, int );

	//eine Kopie eines NEA erzeugen
	NEA_STR *mkCopy( NEA_STR * );

	//einen NEa mitsamt seinen Zuständen löschen
	void mkDelete( NEA_STR * );

	//einen NEA ausgeben
	void outputNEA( NEA_STR * );

	//einen Zustand setzen
	void setState( NEA_STR *, ulong, int );





	//eine in %definition definierte SC eintragen
	void insertSC( char *, bool );

	//eine in %definition definierte SC suchen
	int lookupSC( char * );

	//einen NEA an eine SC einfügen
	void insertSC( SC_STR *, NEA_STR * );

	//einen NEA an alle inclusive SC einfügen
	void insertInclSC( NEA_STR *nea );

	//prüfen, ob mit der SC bereits ein EOF-NEA verbunden ist
	bool insertEOFSC( SC_STR * );

	/*
		den NEA-Stack vergrößern, wird benötig, falls mehrere NEAen diegleiche
		Aktion ausführen sollen (erstes Zeichen |).
	*/
	void extendNEAStack( void );

	//einen NEA auf den NEA-Stack packen
	void insertNEAStack( NEA_STR * );

	//eine ECLOS_STR-Struktur vorbereiten
	ECLOS_STR *createEClos( void );

	//ECLOS_STR freigeben
	void removeEClos( ECLOS_STR * );

	/*
		einen Zustand in eine ECLOS_STR-Struktur eintragen, von dem aus die Epsilon-Übergänge berechnet
		werden sollen.
	*/
	void insertFromEClos( ECLOS_STR *, ulong );

	/*
		mehrere Zustände in eine ECLOS_STR-Struktur eintragen, von denen aus die Epsilon-Übergänge
		berechnet werden sollen.
	*/
	void insertFromEClos( ECLOS_STR *, ulong *, ulong );

	/*
		einen Zustand in eine ECLOS_STR-Struktur eintragen, der von from aus über einen Epsilon-Übergänge
		erreicht wird.
	*/
	void insertToEClos( ECLOS_STR *, ulong );

	/*
		Nun die Zustände bestimmen, die über Epsilon-Übergänge von from aus erreichbar sind und
		in to eintragen.
	*/
	void generateEClos( ECLOS_STR * );

	//sucht einen Index in to
	bool findToEClos( ECLOS_STR *, ulong );


	//sucht einen Index in to
	bool findToMove( MOVE_STR *, ulong );

	/*
		einen Zustand in eine MOVE_STR-Struktur eintragen, der von from aus über einen Übergänge
		erreicht wird.
	*/
	void insertToMove( MOVE_STR *, ulong );

	/*
		mehrere Zustände in eine MOVE_STR-Struktur eintragen, von denen aus die Übergänge
		berechnet werden sollen.
	*/
	void insertFromMove( MOVE_STR *, ulong *index, ulong );

	/*
		einen Zustand in eine MOVE_STR-Struktur eintragen, von dem aus die Übergänge berechnet
		werden sollen.
	*/
	void insertFromMove( MOVE_STR *, ulong );

	//eine MOVE_STR-Struktur vorbereiten
	MOVE_STR *createMove( uchar c );

	//Speicher für moevStr freigeben
	void removeMove( MOVE_STR * );

	/*
		Nun die Zustände bestimmen, die über c-Übergänge von from aus erreichbar sind und
		in to eintragen.
	*/
	void generateMove( MOVE_STR * );



	//DSTATES wird als Stack implementiert.
	void initDStatesStack( void );

	//eine neue NEA-Zustandsmenge in DSTATES eintragen, d.h. einen neuen DEA-Zustand erzeugen
	long insertDStates( ulong *, ulong );

	//eine Menge von NEA-Zuständen, d.h. einen DEA-Zustand suchen
	long lookupDStates( ulong *, ulong );

	//DSTATES deinitialisieren
	void removeDStatesStack( void );

	//einen unmarkierten DEA-Zustand holen
	long getUnmarkedDStates( void );

	//einen unmarkierten DEA-Zustand markieren
	void markDStates( ulong );

	//einen Zustandsübergang in die DEA-Übergangstabelle eintragen
	void insertDTran( SC_STR *, int, long, long, uchar );

	//die DEA-Zustände ausgeben mit den zugehörigen NEA-Zuständen
	void outputDEAStates( void );

	//Hier werden die Klassenfiles des Parsers erzeugt.
	void generateFiles( void );

	//die Übergangstabelle ausgeben
	void outputCPPDTran( ofstream & );

	//die Übergangstabelle ausgeben, mit Minimierung der DEA-Zustandsmenge
	void outputCPPMinDEA( ofstream & );

	//die Start-Zustände der einzelnen DEAen der SC ausgeben
	void outputHHeader( ofstream & );

	//einen neuen DEA-Zustand
	ulong insertDEAState( ulong *, ulong, bool );

	//finde Regeln, die nie ausgeführt werden
	void findNoRules( void );

	//gibt ein Zeichen aus, insbesondere die nicht-druckbaren in lesbarer Form
	void outputChar( uchar, char * );

	//aus den NEAen DEAen machen
	void NEA2DEA( void );

	//eine neue Gruppe für die Zustandsminimierung erzeugen
	GROUP_STR *createGROUP( void );

	//eine Gruppe löschen
	void deleteGROUP( GROUP_STR * );

	//einen DEA-Zustand aus einer Gruppe löschen
	void removeGROUP( GROUP_STR *, ulong );

	//einen DEA-Index zu einer Gruppe hinzufügen
	void insertGROUP( GROUP_STR *, ulong );

	//die speziellen Eigenschaften einer Gruppe übernehmen
	void fixGROUP( GROUP_STR *, GROUP_STR * );

	//eine Gruppe ausgeben
	void outputGROUP( GROUP_STR * );

	//tote Zustände finden
	void checkGROUP( SC_STR *, GROUP_STR **, ulong );

	//Hier wird die DEA-Zustandsmenge minimiert, wie es im Drachen, S. 172 beschrieben ist.
	void minDEA( void );

	//den Header des CPP-Files ausgeben
	void outputCPPHeader( ofstream & );

	//Aktionen ausgeben
	void outputAction( ofstream & );

	//<<EOF>>-NEAen ausgeben
	void outputEOF( ofstream & );

	//die Debug-Strukturen ausgeben
	void outputCPPDebug( ofstream & );

	//wandelt die Steuerzeichen in druckbare Zeichen
	char *toString( char * );

	//Informationen ausgeben
	void outputInfo( void );

	//Include-Anweisungen in das cpp-File ausgeben
	void outputIncl( ofstream & );

	//eigene Erweiterungen in das cpp-File ausgeben
	void outputExt( ofstream & );

	//den Speicher der Aktionen eines NEA freigeben
	void deleteAction( NEA_STR * );

	//weil es nicht möglich ist, einen Breakpoint im Destructor zu setzen
	void _temp( void );


public:
	ssLexClass( SSLEXScanner *, ARGV_STR * );
	~ssLexClass();

	//resets the whole parser
	void reset( void );

	//start parsing
	int yyParse( void );
};

#endif
