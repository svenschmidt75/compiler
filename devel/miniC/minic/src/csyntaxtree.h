/***************************************************************************
 *   Copyright (C) 2005 by Sven Schmidt   *
 *   s.schmidt@lboro.ac.uk   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CSYNTAXTREE_H
#define CSYNTAXTREE_H

#include <iostream>
#include <vector>

#include "csymboltable.h"

using namespace std;

/**
@author Sven Schmidt
*/
class CSyntaxTree
{
public:

	//Typ, Node oder Leaf
	enum STType{ NODE, LEAF };

	//wenn Operatorknoten, welcher Operatorknoten
	enum STOperator{ _ADD, _SUB, _MUL, _DIV, _MOD, _EQU, _GT, _GE, _LT, _LE, _NEQ, _EQ,
						_BIN_AND, _BIN_OR, _BIN_XOR, _BIN_NOT, _LEFT_SHIFT, _RIGHT_SHIFT,
						_LOG_AND, _LOG_OR, _LOG_NOT,
						_BOOL2CHAR, _BOOL2SHORT, _BOOL2INT, _BOOL2LONG,
						_SHORT2BOOL, _SHORT2CHAR, _SHORT2INT, _SHORT2LONG,
						_CHAR2BOOL, _CHAR2SHORT, _CHAR2INT, _CHAR2LONG,
						_INT2CHAR, _INT2BOOL, _INT2SHORT, _INT2LONG,
						_LONG2CHAR, _LONG2BOOL, _LONG2SHORT, _LONG2INT,
						_IF, _WHILE, _DOWHILE, _BLK_STMT, _LABEL, _DUMMY_STMT };

private:

	//Größe des Arrays für die SyntaxTree-Strukturen
	static const unsigned long	SyntaxTreeSize = 1000;

	struct _register
	{
		//Datenregister
		int							_reg;

		//Typ im Datenregister
		CSymbolTable::storageType	_storageType;
	};

	/*
		Hier werden Informationen für eine if/if else-Statement
		abgelegt.
	*/
	struct _ifStruct
	{
		/*
			if( expr )
				stmt1;

			else
				stmt2;
		*/

		//Wurzel zum Expression-Teil
		long	_expr;

		//Wurzel zu stmt1
		long	_stmt1;

		//Wurzel zu stmt2
		long	_stmt2;

		//Label zum Anfang von stmt2
		long	_label1;

		//Label zum Ende von stmt2
		long	_label2;
	};


	/*
		Statement müssen verkettet werden, weil sie verschachtelt auftreten können.
		Dies wird hier gemacht.
	*/
	struct _StmtChain
	{
		//nächstes Statement in der Verkettung (Wurzel des Systaxbaumes)
		long	_next;

		//vorheriges Statement in der Verkettung
		long	_prev;

		//erstes Statement in der Verkettung
		long	_root;
	};


	//Struktur für den eigentlichen Systaxbaum
	struct SyntaxTree
	{
		//Typ, entweder Knoten oder Leaf
		STType						_type;

		/*
			Wenn ein Operator-Knoten, welche Operation.
			Wenn ein Datentype in einen anderen umgewandelt werden soll, so
			steht der umzuwandelnde Typ in right, und die Operation kann z.B.
			INT2LONG sein, also ein Integer sol in einen Long-Typ umgewandelt werden.

			Blöcke können lokale Variablen enthalten, für die auf dem Laufzeit-Stack
			Platz geschaffen werden muss. Dazu wird der Wurzel des Syntaxbaumes einen
			entsprechener Node erzeugt, der die Anzahl der Bytes enthält, die alle
			lokalen Variablen benötigen.
		*/
		STOperator					_operator;

		/*
			Datentyp, der mit dem Operator verbunden ist.
			Wenn z.B.

				*, float
			   / \
              /   \
			 /     \
			a,int	b,float

			dann ist der Datentyp des *-Knotens float.
		*/
		CSymbolTable::storageType	_storageType;

		//Index in Symboltabelle, falls Variable oder Konstante
		long						_symTabIndex;

		//falls Zahlen-Leaf, steht hier der Zahlenwert
		long						_value;

		//Verkettung der Statements
		struct _StmtChain			_stmtChainStr;

		//wenn BEG_LOCALVAR, dann die Anzahl der Bytes für die lokalen Variablen
		long						_localVar;

		/*
			Beinhaltet die if-Anweisung.
			Der Knoten && (logisches und) benötigt ein Sprungziel, der Knoten
			|| (logisches oder) dagegen zwei. Für ersteren steht es in _ifStr.l1, für
			letzteren in _ifStr.l1 und _ifStr.l2.
		*/
		struct _ifStruct			_ifStr;

		/*
			LEAF

			Wenn auf eine lokale Vereinbarung zugegriffen wird, die in einer
			anderen Blocktiefe deklariert wurde, dann steht hier die
			Differenz zum Block, in dem auf sie zugegriffen wird.
		*/
		int							_blkDiff;

		/*
			Hier steht das Register, in dem das Ergebnis gespeichert wird.
			Bspw: c = a + b;
			wird so etwas wie
			move.x b,d0
			move.x a,d1
			add.x  d0,d1
			move.x d1,c

			Für das LEAF b steht dann hier d0, für den Operator-Knoten ADD steht
			hier d1, denn in d1 steht das Ergebnis der Addition.
		*/
		struct _register			_reg;

		//Zeilennummer im Quelltext
		long						_line;


		//Parent
		long						_parent;

		//linkes Child
		long						_left;

		//rechtes Child
		long						_right;
	};


	//private Variablen


	/*
		Die Routinen zur Codeerzeugung und zum Aufbau des Syntaxbaumes
		sollen keine direkte Kontrolle über Strukturen auf Syntaxbaum-Eintäge
		erhalten. Deshalb liefern makeNode und makeLeaf jeweils eine Zahl zurück,
		die eine eindeutige Zuordnung der verschiedenen Syntaxbaum-Einträge ermöglicht.
		Dazu werden ALLE erzeugten Syntaxbaum-Strukturen in einem Array
		gespeichert und der Index wird von makeNode/makeLeaf zurückgeliefert.
	*/
	SyntaxTree				**saveSyntaxTree;

	//Anzahl der Einträge in saveSyntaxTree
	long					saveSyntaxTreePos;

	//maximal mögliche Anzahl in saveSyntaxTree
	unsigned long			saveSyntaxTreeMaxPos;




	//hier wird eine neue Struktur für ein LEAF oder NODE erzeugt und der Index in
	//saveSyntaxTree wird zurückgeliefert.
	long createStruct( void );

	//den Typ eines Eintrages, also NODE oder LEAF, setzen
	void setType( long, STType );

	//den Operator eines NODE setzen
	void setOperator( long, STOperator );

	//Eintrag in Symboltabelle dieses Leafs
	void setSymTabIndex( long, long );


public:

	CSyntaxTree();
	~CSyntaxTree();

	/*
		Hier wird ein neuer Knoten für einen Syntaxbaum erzeugt, wobei der Typ in
		type steht. Ist kein Parent/Child vorhanden, steht dort -1.
	*/
	long makeNode( STOperator, long, long, long );

	/*
		Hier wird ein neues Leaf für einen Syntaxbaum erzeugt, wobei der Eintrag
		in der Symboltabelle in symTabIndex steht. Ist kein Parent/Child vorhanden,
		steht dort -1.
	*/
	long makeLeaf( long, long parent );

	//Children eintragen
	void setChildren( long, long, long );

	//das Elternteil eintragen (doppelt verkettet)
	void setParent( long, long );

	//Eintrag der Anzahl der Bytes für lokale Variablen
	void setLocalVar( long, long );

	//einen Syntaxbaum ausgeben
	void printSyntaxTree( long ) const;

	//linkes Kind zurückliefern
	long getLeftChild( long );

	//rechtes Kind zurückliefern
	long getRightChild( long );

	//Eltern zurückliefern
	long getParent( long );

	//prüft, ob dieser Teil des Sytntaxbaumes ein Knoten ist
	bool isNode( long );

	//den Operator eines NODE zurückliefern
	STOperator getOperator( long );

	//Eintrag der Anzahl der Bytes für lokale Variablen zurückliefern
	long getLocalVar( long );

	//Register eintragen
	void setDataRegister( long, int, CSymbolTable::storageType );

	//Register holen
	int getDataRegister( long );

	//Breite des Datenregisters holen
	CSymbolTable::storageType getDataRegisterStorageType( long );

	//Eintrag in Symboltabelle dieses Leafs zurückliefern
	long getSymTabIndex( long );

	//Blockdifferenz eintragen
	void setBlkDiff( long, int );

	//Blockdifferenz zurückliefern
	int getBlkDiff( long );

	//prüft, ob dieser Teil des Sytntaxbaumes die Wurzel ist
	bool isRoot( long index );

	//Zeilennummer im Quelltext
	void setLine( long, long );

	//Zeilennummer im Quelltext
	long getLine( long );

	//Datentyp eines Knotens/Leafs holen
	CSymbolTable::storageType getStorageType( long );

	//Datentyp eines Knotens/Leafs setzen
	void setStorageType( long, CSymbolTable::storageType );

	//die Informationen zur Auswertung eines if-Statements eintragen
	void setIfStmt( long, long, long, long, long, long );

	//die Informationen zur Auswertung eines if-Statements zurückliefern
	void getIfStmt( long, long &, long &, long &, long &, long & );

	/*
		Hier wird für den Sprung-Node eine eindeutige Zahl
		erzeugt und in parent abgelegt.
	*/
	void genJumpLabel( long index );

	//index ist ein Zahlen-Leaf, hier wird der Zahlenwert eingetragen
	void setNumber( long, long );

	//index ist ein Zahlen-Leaf, hier wird der Zahlenwert zurückgeliefert
	long getNumber( long );

	//Statements verketten
	void linkStmt( long, long );
	long getNextStmt( long );

	//das erste Statement in der Verkettung zurückliefern
	long getRootStmt( long );
};

#endif
