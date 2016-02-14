/***************************************************************************
                          lrparser.h  -  description
                             -------------------
    begin                : Sam Aug 17 2002
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

#ifndef LRPARSER_H
#define LRPARSER_H


/**
  *@author Sven Schmidt
  */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <string>
#include <sys/stat.h>

#include "terminal.h"
#include "scanner.h"
#include "hashclass.h"


//Für Fehlerangaben (%error)
#define ESHIFT		-1
#define EDELETE		-2
#define EOVERREAD	-3
#define EEXIT		-4
#define ESEM		-5




//Kern oder normales LR-Item
#define CORE	0
#define ITEM	1


//Zustände in der LR-Tabelle
#define LR_ERROR	1
#define LR_SHIFT	2
#define LR_REDUCE	3
#define LR_ACCEPT	4



//Hier werden die Produktionen abgelegt, ohne die rechte Seite.
//Die rechten Seiten sind in PROD2_STR abgelegt.
typedef struct
{
	//Index in Hash-Tabelle
	long	hash_index;

	//Index der ersten Alternative in PROD2_STR
	long	prod2_index;

	//Anzahl der Alternativen
	int		altAnz;

}PROD_STR;


//Hier werden die zu den Produktionen gehörigen rechten Seiten abgelegt.
typedef struct
{
	//Index der Produktion in PROD_STR
	long			prod_index;

	//Anzahl der Elemente auf der rechten Seite
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

	//Falls in Folge eines Reduce-Reduce-Fehlers mit dieser ALternative nie reduziert wird
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

	//Typ
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
			EDELETE:				ein Terminal löschen
			EOVERREAD(TERMINAL):	bis TERMINAL erscheint, alles überlesen
			EEXIT:					Parsing beenden
			ESEM:					semantische Aktionen trotz Fehler zulassen
*/
typedef struct
{
	//Index der Produktion in PROD_STR
	long		prod_index;

	//Nummer der Alternative, also Index in der PROD2_STR
	long		prod2_index;

	//Position des Punktes
	int			dot;

	//ESHIFT(TERMINAL)
	long		shift;

	//EOVERREAD(TERMINAL)
	long		overread;

	//ESEM
	int			sem;

	//EEXIT
	int			exit;

	//EDELETE
	int			del;

	//Fehlermeldung
	char		*msg;

	//Zeile der Deklaration
	long		line;

	//Gibt an, für welche Terminale diese Fehlerangabe gilt
	int			*tArr;

	//Terminale, verbunden mit Fehleraktionen
	ERRTERM_STR	*errTermStr;

	//Anzahl der ERRTERM_STR-Strukturen
	int			errTermStrAnz;

}ERROR_STR;




//Stack bei der Analyse der Grammatik
typedef struct
{
	//Index der Produktion in PROD_STR
	long	prod_index;

	//Index der Alternative in PROD2_STR
	long	prod2_index;

	//Nummer der Alternativen
	int		altCount;

	//Priorität der Produktion
	int		pPriority;

	//EBNF-Error
	int		ebnf_err;

	//EBNF-Level, prüft die Verschachtelungen der EBNF-Symbole
	int		ebnf_level;

}STACK;



//Struktur für EIN LR(0/1)-Element
typedef struct
{
	//Index in PROD2_STR
	long		prod2_index;

	//Position des Punktes
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



class lrparser
{
private:

	//links-assoziativ
	static const int isLeftAssoc = 1;

	//rechts-assoziativ
	static const int isRightAssoc = 2;

	//nicht-assoziativ
	static const int isNonAssoc = 3;


	//Informationen über das Grammatik-File
	struct stat		st;

	//Attribute der vom Scanner gelieferten Tokens
	SCAN_STR		*att;

	//Daten mit der Grammatik-Datei
	char			*data;

	//Hash-Tabelle
	hashClass		*hash_tab;

	//Der Scanner
	scanner			*s;

	//Array für die Produktionen
	PROD_STR		*prod_str;

	//Anzahl der Produktionen
	int				prod_anz;

	//Array für alle rechten Seiten
	PROD2_STR		*prod2_str;

	//Anzahl aller rechten Seiten
	int				prod2_anz;

	//Array für die %error-Angaben
	ERROR_STR		*error_str;

	//Anzahl der Einträge in error_str
	int				error_anz;

	//Include-Anweisung vom Scanner
	char			*incl;

	//eigene Erweiterungen, vom Scanner
	char			*ext;

	/*
		Die Verwendung der Terminale/Nichtterminale wird geprüft. In diesem Array
		sind die Hash-Indices aufgereiht.
	*/
	long			*check;

	//Anzahl der Einträge in check
	long			anzCheck;

	//für %union-Definition
	UNION_STR		*union_str;

	//Argumente
	ARGV_STR		*argv_str;

	//Hash-Index von EPSILON
	long			epsHashIndex;

	//Hash-Index von ACCEPT
	long			acceptHashIndex;

	//Check-Terminal # bei Generierung von LALR-Parsern
	long			lalrHashIndex;

	//Array mit allen Hüllen
	HULLE_STR		**hulle_str;

	//Anzahl der Hüllen
	long			hulle_anz;

	//Anzahl aller Kerne
	long			coreAnz;

	//LR-Tabelle
	LR_TAB			**lrTab;

	//Sprung-Tabelle
	int				**jumpTab;

	//Array mit den Hash-Indices aller Terminale
	long			*termArr;

	//Anzahl der Einträge in termArr
	int				termArrCount;

	//Array mit den Hash-Indices aller Nichtterminale
	long			*ntermArr;

	//Anzahl der Einträge in ntermArr
	int				ntermArrCount;

	//Anzahl der Shift-Reduce-Fehler
	int				srError;

	//Anzahl der Reduce-Reduce-Fehler
	int				rrError;

	//Zähler für semantische Aktionen
	int				semActCnt;

	//Struktur für semantische Aktionen
	SEMACT_STR		semActStr;

	//in semActStr ist ein Eintrag
	bool			semAct;




	/*******************
	* private Methoden *
	*******************/

	//Grammatik-Datei analysieren
	int readFile( char * );

	//Grammatik-File analysieren
	int analyze( void );

	//Überprüfung der (Nicht)Terminals
	void setCheck( long );

	//Neues %union-Paar eintragen
	void insertUnion( char *, char * );

	//Index des Type der semantischen Aktion in union_str finden
	int getSemIndex( char * );

	//PROD_STR anfordern
	long getNewProd( void );

	//PROD2_STR anfordern
	long getNewProd2( long );

	//Ein Elemente in eine rechte Seite einer Alternative eintragen
	void insertElement( long, long, long );

	//Die Fehleraktionen auf doppelte EInträge prüfen
	int checkErrorStr( void );

	//PROD2_STR sortieren
	void sortProd2Str( void );

	//Grammatik in Datei ausgeben
	void outputGrammar( void );

	//Alle Alternativen ausgeben
	void outputAllAlt( void );

	//Eine Alternativen ausgeben
	void outputAlt( ostream &, long );

	//Eine Hülle ausgeben
	void outputHulle ( HULLE_STR * );

	//Ein Item ausgeben
	void outputItem( long, int, ofstream & );

	//Start-Produktion eintragen
	void insertStartProd( long &, long & );

	//(Nicht)Terminale überprüfen
	int checkAll( void );

	//prüfen, ob mit allen Alternativen reduziert wurde
	int checkAllReduce( void );

	//Alle FIRST-Mengen bilden
	void genAllFirst( void );

	//Hier werden die FIRST-Mengen eines Terminals/Nichtterminals erzeugt
	void genFirst( Terminal *, int, long * );

	//Alle FOLLOW-Mengen bilden
	void genAllFollow( void );

	//Prüft, ob die Hülle h schon in hulle_str enthalten ist
	int cmpLR0Hulle( HULLE_STR * );

	//Prüft, ob die Hülle h schon in hulle_str enthalten ist
	int cmpLR1Hulle( HULLE_STR * );

	//Alle Hüllen der SLR und LALR bilden
	void genLR0Hulle( void );

	//Alle Hüllen der LR
	void genLR1Hulle( void );

	//LR(0)-Kern in HULLE h eintragen
	void insertItem( HULLE_STR *, long, int, int );

	//LR(1)-Kern in HULLE h eintragen
	void insertItem( HULLE_STR *, long, int, int, Terminal * );

	//Eine HULLE_STR-Struktur erzeugen
	HULLE_STR *createHulle( void );

	//Eine HULLE_STR-Struktur löschen
	void deleteHulle( HULLE_STR * );

	//Prüft, ob in h der LR(0)-Kern schon enthalten ist
	int findItem( HULLE_STR *, long, int );

	//Prüft, ob in h der LR(1)-Kern schon enthalten ist
	int findItem( HULLE_STR *, long, int, Terminal * );

	//Eine Hülle in die Sammlung von Elementen eintragen.
	void insertHulle( HULLE_STR * );

	//LR(0)-Items der Hülle erzeugen
	void genLR0Items( HULLE_STR * );

	//LR(1)-Items der Hülle erzeugen
	void genLR1Items( HULLE_STR * );

	//Hier werden die Hüllen erzeugt, die sich durch Spring( h, X ) ergeben
	void makeLR0Jump( HULLE_STR * );

	//Hier werden die Hüllen erzeugt, die sich durch Spring( h, X ) ergeben
	void makeLR1Jump( HULLE_STR * );

	//Aus der Sammlung der LR(0)-Elemente die Lookaheads anfügen.
	void genLALR( void );

	//Die Hülle erzeugen, die sich durch Sprung( I0, index ) ergibt
	void genLR0Jump( HULLE_STR *, HULLE_STR *, long );

	//Die Hülle erzeugen, die sich durch Sprung( I0, index ) ergibt
	void genLR1Jump( HULLE_STR *, HULLE_STR *, long );

	//Terminale ausgeben
	void outputTerminal( Terminal * );

	//Index der Hülle in die entsprechenden Elemente von I0 eintragen, die durch Sprung( I0, elem )
	//erzeugt wird.
	void setJump( HULLE_STR *, long, long );

	//Hier werden die LR(0)-Kerne beider Hüllen verglichen
	bool cmp2LR0Hulle( HULLE_STR *, HULLE_STR * );

	//Wandelt die LR(1)-Hüllen in LR(0)-Hüllen
	void LR2LALR( void );

	//Hülle löschen
	void delHulle( HULLE_STR * );

	//LR-Tabelle erzeugen
	void LRTab( void );

	//Einen Reduce-Eintrag erzeugen
	void setReduce( int, long, Terminal * );

	//Einen Shift-Eintrag erzeugen und dabei Shift-Reduce-Fehler behandeln.
	void setShift( int, long, int );

	//Hier werden die Klassenfiles des Parsers erzeugt.
	void generateFiles( void );

	//LR-Tabelle in Datei ausgeben
	void outputLRTab( ofstream & );

	//Sprung-Tabelle ausgeben
	void outputJumpTab( ofstream & );

	//Header-File ausgeben
	void outputHFile( ofstream & );

	//Terminal-Defines als enum ausgeben
	void outputCPPTerminal( ofstream & );

	//Die Alternativen in die cpp-Datei ausgeben.
	void outputAlternatives( ofstream & );

	//Follow-Mengen in cpp-File ausgeben
	void outputFollow( ofstream & );

	//die NTERM_STR ausgeben
	void outputNtermStr( ofstream & );

	//Die Namen der Terminale ausgeben, nur im debug-mode
	void outputTermName( ofstream & );

	//Hier wird die komplette Fehlerbehandlung ausgegeben
	void outputErrorRecovery( ofstream & );

	//Den Header des cpp-Files ausgeben
	void outputCPPHeader( ofstream & );

	//alle übrigen Routinen des cpp-Files ausgeben
	void outputCPPEnd( ofstream & );

	//Erzeuge eine neue ERROR_STR und liefere den Index zurück
	int getErrorStr( void );

	//Fügt eine ERRTERM_STR zu einer ERROR_STR hinzu.
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





public:
	lrparser( char *, ARGV_STR * );
	~lrparser();
};

#endif
