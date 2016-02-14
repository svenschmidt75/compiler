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
#include "csyntaxtree.h"

/*
	Hier werden die Strukturen für die Darstellung von Syntaxbäumen
	implementiert. Es wird dabei intern eine Struktur vom Typ SystaxTree
	angelegt, es wird aber nur ein Index auf den Eintrag in einem private-Array
	zurückgeliefert, um direkten Zugriff zu vermeiden.
*/


CSyntaxTree::CSyntaxTree()
{
	//initialisieren
	saveSyntaxTree = 0L;
	saveSyntaxTreePos = 0;
	saveSyntaxTreeMaxPos = 0;
}


CSyntaxTree::~CSyntaxTree()
{
	//Speicher freigeben
	if( saveSyntaxTree )
		delete[] ( saveSyntaxTree );
}


long CSyntaxTree::makeNode( CSyntaxTree::STOperator op, long parent, long left, long right )
{
	/*
		Hier wird ein neuer Knoten für einen Syntaxbaum erzeugt, wobei der Typ in
		type steht. Ist kein Parent/Child vorhanden, steht dort -1.
	*/
	long	index;

	//einen neuen Eintrag erzeugen
	index = createStruct();

	//Typ eintragen
	setType( index, NODE );

	//Operator eintragen
	setOperator( index, op );

	//Parent eintragen
	setParent( index, parent );

	//Children eintragen
	setChildren( index, left, right );

	//Index zurückliefern
	return( index );
}


long CSyntaxTree::makeLeaf( long symTabIndex, long parent )
{
	/*
		Hier wird ein neues Leaf für einen Syntaxbaum erzeugt, wobei der Eintrag
		in der Symboltabelle in symTabIndex steht. Ist kein Parent/Child vorhanden,
		steht dort -1.
	*/
	long	index;

	//einen neuen Eintrag erzeugen
	index = createStruct();

	//Typ eintragen
	setType( index, LEAF );

	//Symboltabellen-Eintrag eintragen
	setSymTabIndex( index, symTabIndex );

	//Parent eintragen
	setParent( index, parent );

	//Index zurückliefern
	return( index );
}


long CSyntaxTree::getParent( long index )
{
	//Eltern zurückliefern

	//Eintrag überhaupt vorhanden?
	if( ( index != -1 ) && ( index < saveSyntaxTreePos ) )
		//zurückliefern
		return( saveSyntaxTree[index]->_parent );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getParent: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getLeftChild( long index )
{
	//linkes Kind zurückliefern

	//Eintrag überhaupt vorhanden?
	if( ( index != -1 ) && ( index < saveSyntaxTreePos ) )
		//zurückliefern
		return( saveSyntaxTree[index]->_left );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getLeftChild: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getRightChild( long index )
{
	//rechtes Kind zurückliefern

	//Eintrag überhaupt vorhanden?
	if( ( index != -1 ) && ( index < saveSyntaxTreePos ) )
		//zurückliefern
		return( saveSyntaxTree[index]->_right );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getRightChild: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


bool CSyntaxTree::isNode( long index )
{
	//prüft, ob dieser Teil des Sytntaxbaumes ein Knoten ist

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//zurückliefern
		return( ( saveSyntaxTree[index]->_type == NODE ) ? true : false );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::isNode: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


bool CSyntaxTree::isRoot( long index )
{
	//prüft, ob dieser Teil des Sytntaxbaumes die Wurzel ist

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//zurückliefern
		return( ( saveSyntaxTree[index]->_parent == -1 ) ? true : false );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::isRoot: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setSymTabIndex( long index, long symTabIndex )
{
	//Eintrag in Symboltabelle dieses Leafs

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_symTabIndex = symTabIndex;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setSymTabIndex: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setLine( long index, long line )
{
	//Zeilennummer im Quelltext

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_line = line;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setLine: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getLine( long index )
{
	//Zeilennummer im Quelltext zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//zurückliefern
		return( saveSyntaxTree[index]->_line );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getLine: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getSymTabIndex( long index )
{
	//Eintrag in Symboltabelle dieses Leafs zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		return( saveSyntaxTree[index]->_symTabIndex );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getSymTabIndex: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setLocalVar( long index, long localVarSize )
{
	//Eintrag der Anzahl der Bytes für lokale Variablen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_localVar = localVarSize;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setLocalVar: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getLocalVar( long index )
{
	//Eintrag der Anzahl der Bytes für lokale Variablen zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		return( saveSyntaxTree[index]->_localVar );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getLocalVar: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setChildren( long index, long left, long right )
{
	//Children eintragen

	//Eintrag überhaupt vorhanden?
	if( ( index != -1 ) && ( index < saveSyntaxTreePos ) )
	{
		//eintragen
		saveSyntaxTree[index]->_left = left;
		saveSyntaxTree[index]->_right = right;
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setChildren: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setParent( long index, long parent )
{
	//das Elternteil eintragen (doppelt verkettet)

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_parent = parent;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setParent: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setOperator( long index, CSyntaxTree::STOperator op )
{
	//den Operator eines NODE setzen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_operator = op;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setOperator: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


CSyntaxTree::STOperator CSyntaxTree::getOperator( long index )
{
	//den Operator eines NODE zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		return( saveSyntaxTree[index]->_operator );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getOperator: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setType( long index, CSyntaxTree::STType type )
{
	//den Typ eines Eintrages, also NODE oder LEAF, setzen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_type = type;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setType: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::createStruct( void )
{
	//hier wird eine neue Struktur für ein LEAF oder NODE erzeugt und der Index in
	//saveSyntaxTree wird zurückgeliefert.
	SyntaxTree	*str;

	//Speicher für Struktur anfordern
	str = new( SyntaxTree );

	//initialisieren
	memset( str, 0, sizeof( SyntaxTree ) );

	//genug Speicher vorhanden?
	if( saveSyntaxTreePos == saveSyntaxTreeMaxPos )
	{
		//nein, anfordern
		SyntaxTree	**tmp;

		//neue maximale Größe anpassen
		saveSyntaxTreeMaxPos += SyntaxTreeSize;

		//Speicher anfordern
		tmp = new( SyntaxTree * [saveSyntaxTreeMaxPos + saveSyntaxTreePos] );

		//alte Einträge kopieren
		memcpy( tmp, saveSyntaxTree, sizeof( SyntaxTree * ) * saveSyntaxTreePos );

		//Speicher freigeben?
		if( saveSyntaxTree )
			//ja, freigeben
			delete[] ( saveSyntaxTree );

		//Zeiger kopieren
		saveSyntaxTree = tmp;
	}

	//noch keine Eltern und Kinder
	str->_left = -1;
	str->_right = -1;
	str->_parent = -1;
	str->_stmtChainStr._next = -1;
	str->_stmtChainStr._prev = -1;
	str->_stmtChainStr._root = -1;
	str->_blkDiff = 0;

	//noch keine Datenregister
	str->_reg._reg = -1;

	//eintragen
	saveSyntaxTree[saveSyntaxTreePos] = str;

	//Index zurückliefern
	return( saveSyntaxTreePos++ );
}


void CSyntaxTree::printSyntaxTree( long index ) const
{
	//einen Syntaxbaum ausgeben
	char	*type[2] = { "NODE", "LEAF" };
	char	*op[] = { "_ADD", "_SUB", "_MUL", "_DIV", "_MOD", "_EQU", "_GT",
						"_GE", "_LT", "_LE", "_NEQ", "_EQ",
						"_BIN_AND", "_BIN_OR", "_BIN_XOR", "_BIN_NOT",
						"_LEFT_SHIFT", "_RIGHT_SHIFT",
						"_LOG_AND", "_LOG_OR", "_LOG_NOT",
						"_BOOL2CHAR", "_BOOL2SHORT", "_BOOL2INT", "_BOOL2LONG",
						"_SHORT2BOOL", "_SHORT2CHAR", "_SHORT2INT", "_SHORT2LONG",
						"_CHAR2BOOL", "_CHAR2SHORT", "_CHAR2INT", "_CHAR2LONG",
						"_INT2CHAR", "_INT2BOOL", "_INT2SHORT", "_INT2LONG",
						"_LONG2CHAR", "_LONG2BOOL", "_LONG2SHORT", "_LONG2INT",
						"_IF", "_WHILE", "_DOWHILE", "_BLK_STMT", "_LABEL", "_DUMMY_STMT" };

	//gibt es den Baum überhaupt?
	if( index < saveSyntaxTreePos )
	{
		//Zeiger auf Struktur
		struct SyntaxTree	*tree = saveSyntaxTree[index];

		//Index ausgeben
		cout << "  Index: " << index << endl;

		//Type ausgeben
		cout << "    Typ: " << type[tree->_type] << endl;

		//Datentyp ausgeben
		cout << "Datentyp: ";

		//Datentyp
		switch( tree->_storageType )
		{
			case CSymbolTable::_BOOL:
			{
				//ausgeben
				cout << "BOOL" << endl;
				break;
			}

			case CSymbolTable::_CHAR:
			{
				//ausgeben
				cout << "CHAR" << endl;
				break;
			}

			case CSymbolTable::_SHORT:
			{
				//ausgeben
				cout << "SHORT" << endl;
				break;
			}

			case CSymbolTable::_INT:
			{
				//ausgeben
				cout << "INT" << endl;
				break;
			}

			case CSymbolTable::_LONG:
			{
				//ausgeben
				cout << "LONG" << endl;
				break;
			}

			default:
				//es gibt keinen
				cerr << "CSyntaxTree::printSyntaxTree: Unknown storage type!" << endl;
		}

		//Operator ausgeben?
		if( tree->_type == NODE )
			//ja, nur bei Knoten
			cout << "     Op: " << op[tree->_operator] << endl;

		//Index des Elterns
		cout << " Parent: " << tree->_parent << endl;

		//Index beider Kinder
		cout << "   left: " << tree->_left << endl;
		cout << "  right: " << tree->_right << endl;
		cout << endl;

		//linkes Kind ausgeben?
		if( tree->_left != -1 )
			//ja
			printSyntaxTree( tree->_left );

		//rechtes Kind ausgeben?
		if( tree->_right != -1 )
			//ja
			printSyntaxTree( tree->_right );

		//if-Statement?
		if( tree->_operator == _IF )
		{
			//Zeiger auf die if-Struktur
			struct _ifStruct	*tmp = &tree->_ifStr;

			//expr ausgeben
			printSyntaxTree( tmp->_expr );

			//stmt1 ausgeben
			printSyntaxTree( tmp->_stmt1 );

			//stmt2 ausgeben?
			if( tmp->_stmt2 != -1 )
				//ja, ausgeben
				printSyntaxTree( tmp->_stmt2 );
		}
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::printSyntaxTree: Entry " << index << " not available!" << endl;

		//Ende
//		exit( 1 );
	}
}


void CSyntaxTree::setDataRegister( long index, int dr, CSymbolTable::storageType type )
{
	//Register eintragen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
	{
		//eintragen
		saveSyntaxTree[index]->_reg._reg = dr;
		saveSyntaxTree[index]->_reg._storageType = type;
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setDataRegister: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


int CSyntaxTree::getDataRegister( long index )
{
	//Register holen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		return( saveSyntaxTree[index]->_reg._reg );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getDataRegister: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


CSymbolTable::storageType CSyntaxTree::getDataRegisterStorageType( long index )
{
	//Breite in Bytes des Daten-Registers holen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//zurückliefern
		return( saveSyntaxTree[index]->_reg._storageType );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getDataRegisterStorageType: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setBlkDiff( long index, int diff )
{
	//Blockdifferenz eintragen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//eintragen
		saveSyntaxTree[index]->_blkDiff = diff;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setBlkDiff: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


int CSyntaxTree::getBlkDiff( long index )
{
	//Blockdifferenz zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//zurückliefern
		return( saveSyntaxTree[index]->_blkDiff );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getBlkDiff: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


CSymbolTable::storageType CSyntaxTree::getStorageType( long index )
{
	//Datentyp eines Knotens/Leafs holen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//zurückliefern
		return( saveSyntaxTree[index]->_storageType );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getStorageType: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setStorageType( long index, CSymbolTable::storageType type )
{
	//Datentyp eines Knotens/Leafs setzen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//setzen
		saveSyntaxTree[index]->_storageType = type;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setStorageType: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setIfStmt( long index, long expr, long stmt1, long stmt2, long l1, long l2 )
{
	//die Informationen zur Auswertung eines if-Statements eintragen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
	{
		//Zeiger auf _ifStr-Struktur
		struct _ifStruct	*tmp = &saveSyntaxTree[index]->_ifStr;

		//Wurzel des expr-Teil
		tmp->_expr = expr;

		//Wurzel des stmt1-Teil
		tmp->_stmt1 = stmt1;

		//Wurzel des stmt2-Teil
		tmp->_stmt2 = stmt2;

		//Wurzel des Sprungziels vor dem stmt2-Teil
		tmp->_label1 = l1;

		//Wurzel des Sprungziels hinter dem stmt2-Teil
		tmp->_label2 = l2;
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setIfStmt: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::getIfStmt( long index, long &expr, long &stmt1, long &stmt2, long &l1, long &l2 )
{
	//die Informationen zur Auswertung eines if-Statements zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
	{
		//Zeiger auf _ifStr-Struktur
		struct _ifStruct	*tmp = &saveSyntaxTree[index]->_ifStr;

		//Wurzel des expr-Teil
		expr = tmp->_expr;

		//Wurzel des stmt1-Teil
		stmt1 = tmp->_stmt1;

		//Wurzel des stmt2-Teil
		stmt2 = tmp->_stmt2;

		//Wurzel des Sprungziels vor dem stmt2-Teil
		l1 = tmp->_label1;

		//Wurzel des Sprungziels hinter dem stmt2-Teil
		l2 = tmp->_label2;
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getIfStmt: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::genJumpLabel( long index )
{
	/*
		Hier wird für den Sprung-Node eine eindeutige Zahl
		erzeugt und in parent abgelegt.
	*/
	static long	cnt = 0;

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//Label eintragen
		saveSyntaxTree[index]->_parent = cnt++;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::genJumpLabel: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::setNumber( long index, long val )
{
	//index ist ein Zahlen-Leaf, hier wird der Zahlenwert eingetragen

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//Zahl eintragen
		saveSyntaxTree[index]->_value = val;

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::setNumber: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getNumber( long index )
{
	//index ist ein Zahlen-Leaf, hier wird der Zahlenwert zurückgeliefert

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//Zahl zurückliefern
		return( saveSyntaxTree[index]->_value );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getNumber: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSyntaxTree::linkStmt( long index, long stmt )
{
	//Statements verketten

	//Eintrag überhaupt vorhanden?
	if( ( ( index != -1 ) && ( index < saveSyntaxTreePos ) ) && ( ( stmt != -1 ) && ( stmt < saveSyntaxTreePos ) ) )
	{
		//das Statement stmt als nächstes von index eintragen
		saveSyntaxTree[index]->_stmtChainStr._next = stmt;

		//index als vorheriges von stmt eintragen
		saveSyntaxTree[stmt]->_stmtChainStr._prev = index;
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::linkStmt: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getNextStmt( long index )
{
	//Statements verketten

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
		//Index der Wurzel des Statements zurückliefern
		return( saveSyntaxTree[index]->_stmtChainStr._next );

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getNextStmt: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSyntaxTree::getRootStmt( long index )
{
	//das erste Statement in der Verkettung zurückliefern

	//Eintrag überhaupt vorhanden?
	if( index < saveSyntaxTreePos )
	{
		struct _StmtChain	*tmp;
		long				old = index;

		//Zeiger auf die Verkettung
		tmp = &saveSyntaxTree[index]->_stmtChainStr;

		//solange es ein prov-Element hat
		while( tmp->_prev != -1 )
		{
			//Index merken
			old = tmp->_prev;

			//Zeiger auf nächste Verkettung
			tmp = &saveSyntaxTree[tmp->_prev]->_stmtChainStr;
		}

		//zurückliefern
		return( old );
	}

	else
	{
		//Eintrag existiert nicht
		cerr << "CSyntaxTree::getRootStmt: Entry " << index << " not available!" << endl;

		//Ende
		exit( 1 );
	}
}
