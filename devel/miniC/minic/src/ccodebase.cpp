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
#include "ccodebase.h"

/*
	Statische Datenelemente MÜSSEN definiert werden!
*/
CCodeBase::Register	CCodeBase::dataReg[maxRegister] = { 0 };
CCodeBase::Register	CCodeBase::adressReg[maxRegister] = { 0 };


CCodeBase::CCodeBase()
{
}


CCodeBase::CCodeBase( CSymbolTable *_symTab, CSyntaxTree *_syntaxTree )
{
	//Zeiger auf den Syntaxbaum
	syntaxTree = _syntaxTree;

	//Zeiger auf die Symboltabelle
	symTab = _symTab;
}


CCodeBase::~CCodeBase()
{
}


CCodeBase::Register CCodeBase::getFreeAdressRegister( void )
{
	Register	i = 0;

	//nächstes freies Register holen
	while( ( i < maxRegister ) && ( adressReg[i] != 0 ) )
		//nächstes Register früfen
		i++;

	//kein freies Register mehr vorhanden?
	if( i == maxRegister )
	{
		//nein, Ende
		cerr << "CCodeBase::getFreeAdressRegister: Number of available adress registers exeeded!" << endl;

		//Ende
		exit( 1 );
	}

	//Adressregister i ist nun belegt
	adressReg[i] = 1;

	//freies Datenregister zurückliefern
	return( i );
}


void CCodeBase::freeAdressRegister( CCodeBase::Register reg )
{
	//gibt es das Adressregister überhaupt?
	if( ( reg >= 0 ) && ( reg < maxRegister ) )
	{
		//ja, ist es überhaupt belegt?
		if( adressReg[reg] )
			//ja, freigeben
			adressReg[reg] = 0;

		else
			//Meldung
			cerr << "CCodeBase::freeAdressRegister: Adress register " << reg << " not occupied!" << endl;
	}

	else
	{
		//nein, Ende
		cerr << "CCodeBase::freeAdressRegister: Adress register " << reg << " not existent!" << endl;

		//Ende
		exit( 1 );
	}
}


CCodeBase::Register CCodeBase::getFreeDataRegister( void )
{
	Register	i = 0;

	//nächstes freies Register holen
	while( ( i < maxRegister ) && ( dataReg[i] != 0 ) )
		//nächstes Register früfen
		i++;

	//kein freies Register mehr vorhanden?
	if( i == maxRegister )
	{
		//nein, Ende
		cerr << "CCodeBase::getFreeDataRegister: Number of available data registers exeeded!" << endl;

		//Ende
		exit( 1 );
	}

	//Datenregister i ist nun belegt
	dataReg[i] = 1;

	//freies Datenregister zurückliefern
	return( i );
}


void CCodeBase::freeDataRegister( CCodeBase::Register reg )
{
	//gibt es das Datenregister überhaupt?
	if( ( reg >= 0 ) && ( reg < maxRegister ) )
	{
		//ja, ist es überhaupt belegt?
		if( dataReg[reg] )
			//ja, freigeben
			dataReg[reg] = 0;

		else
			//Meldung
			cerr << "CCodeBase::freeDataRegister: Data register " << reg << " not occupied!" << endl;
	}

	else
	{
		//nein, Ende
		cerr << "CCodeBase::freeDataRegister: Data register " << reg << " not existent!" << endl;

		//Ende
		exit( 1 );
	}
}


void CCodeBase::print( long index )
{
	/*
		Dieser Routine wird der Index der Wurzel des Syntaxbaumes übergeben, für
		den hier Code erzeugt werden soll.
	*/
	long	left, right, next;

	//undefiniert?
	if( index == -1 )
		//ja, Ende
		return;

	//linkes Kind
	left = syntaxTree->getLeftChild( index );

	//rechtes Kind
	right = syntaxTree->getRightChild( index );

	//nächstes Statement in der Verkettung holen
	next = syntaxTree->getNextStmt( index );


	/*
		Es wird von rechts nach linke ausgewertet!!!
		Also wird zuerst alle rechten Kinder Code erzeugt, und erste DANACH
		für alle linken!!!
	*/


	//Zeile im Quelltext
	line = syntaxTree->getLine( index );

	/*
		Hier können jetzt LEAFs stehen oder NODEs.
	*/

	//Knoten?
	if( syntaxTree->isNode( index ) )
	{
		//ist ein Knoten
		CSyntaxTree::STOperator op = syntaxTree->getOperator( index );

		//welcher Operator?
		switch( op )
		{
			case CSyntaxTree::_LOG_AND:
			{
				//logisches und, &&
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr, dr;
				long						l1, dummy, gen;

				//in der _ifStr-Struktur steht das Sprungziel
				syntaxTree->getIfStmt( index, dummy, dummy, dummy, l1, dummy );

				//in parent steht der Index des Labels
				gen = syntaxTree->getParent( l1 );



				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );


				//Datenregister für das Ergebnis holen
				dr = getFreeDataRegister();

				//das Eregbnis auf false setzen
				cout << "\tmove.b\t#0,d" << dr << endl;

				//linkes Kind == false?
				cout << "\tcmp.b\t#0,d" << ldr << endl;

				//Sprung
				cout << "\tbeq\t\tL" << gen << endl;

				//rechtes Kind == false?
				cout << "\tcmp.b\t#0,d" << rdr << endl;

				//Sprung
				cout << "\tbeq\t\tL" << gen << endl;

				//beide true, das Eregbnis auf true setzen
				cout << "\tmove.b\t#1,d" << dr << endl;

				//Label erzeugen
				print( l1 );

				//das Ergenbis steht in dr, also ldr und rdr freigeben
				freeDataRegister( ldr );
				freeDataRegister( rdr );

				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( dr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Expression has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den &&-Knoten eintragen
					syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_LOG_OR:
			{
				//logisches oder, ||
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr, dr;
				long						l1, l2, dummy, gen1, gen2;

				//in der _ifStr-Struktur steht das Sprungziel
				syntaxTree->getIfStmt( index, dummy, dummy, dummy, l1, l2 );

				//in parent steht der Index des Labels
				gen1 = syntaxTree->getParent( l1 );
				gen2 = syntaxTree->getParent( l2 );



				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );


				//Datenregister für das Ergebnis holen
				dr = getFreeDataRegister();

				//das Eregbnis auf false setzen
				cout << "\tmove.b\t#0,d" << dr << endl;

				//linkes Kind == false?
				cout << "\tcmp.b\t#0,d" << ldr << endl;

				//Sprung
				cout << "\tbne\t\tL" << gen2 << endl;

				//rechtes Kind == false?
				cout << "\tcmp.b\t#0,d" << rdr << endl;

				//Sprung
				cout << "\tbne\t\tL" << gen2 << endl;

				//beide false
				cout << "\tbra\t\tL" << gen1 << endl;

				//Label erzeugen
				print( l2 );

				//beide true, das Eregbnis auf true setzen
				cout << "\tmove.b\t#1,d" << dr << endl;

				//Label erzeugen
				print( l1 );

				//das Ergenbis steht in dr, also ldr und rdr freigeben
				freeDataRegister( ldr );
				freeDataRegister( rdr );

				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( dr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Expression has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den ||-Knoten eintragen
					syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_LOG_NOT:
			{
				//logisches nicht, !
				CSymbolTable::storageType	rightType;
				Register					dr, rdr;

				/*
					Dieser Knoten hat nur ein rechtes Kind!
				*/


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );


				//Datenregister für das Ergebnis holen
				dr = getFreeDataRegister();

				//prüfen auf false und entsprechend setzen
				cout << "\tcmp.b\t#0,d" << rdr << endl;
				cout << "\tseq.b\td" << dr << endl;

				//das Ergenbis steht in dr, also rdr freigeben
				freeDataRegister( rdr );

				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( dr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Expression has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den !-Knoten eintragen
					syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_IF:
			{
				//if-Statement ausgeben
				CSymbolTable::storageType	leftType, rightType;
				Register					dr, rdr;
				long						expr, stmt1, stmt2, l1, l2, gen1, gen2;

				//Informationen holen
				syntaxTree->getIfStmt( index, expr, stmt1, stmt2, l1, l2 );

				//die Label holen
				if( l1 != -1 )
					//es gibt einen else-Teil
					gen1 = syntaxTree->getParent( l1 );

				//Sprung hinter else-Teil
				gen2 = syntaxTree->getParent( l2 );

				//Code für expr ausgeben
				print( expr );

				//Datenregister des Ergebnis holen
				dr = syntaxTree->getDataRegister( expr );

				//Ergebnis auf true oder false testen
				cout << "\ttst.b\td" << dr << endl;

				//gibt es einen else-Teil?
				if( stmt2 == -1 )
					//nein, gibt es nicht
					cout << "\tbne\t\tL" << gen2 << endl;

				else
					//ja, gibt es
					cout << "\tbne\t\tL" << gen1 << endl;

				//if-Teil ausgeben (true-Teil)
				print( stmt1 );

				//gibt es einen else-Teil?
				if( stmt2 == -1 )
					//nein, Label erzeugen
					print( l2 );

				else
				{
					//ja, Sprung hinter den else-Teil
					cout << "\tbra\t\tL" << gen2 << endl;

					//Label erzeugen
					print( l1 );

					//else-Teil ausgeben
					print( stmt2 );

					//Label ausgeben
					print( l2 );
				}

				//das Datenregister für die Auswertung von expr freigeben
				freeDataRegister( dr );
				break;
			}

			case CSyntaxTree::_WHILE:
			{
				//while-Statement ausgeben
				CSymbolTable::storageType	leftType, rightType;
				Register					dr, rdr;
				long						expr, stmt, dummy, l1, l2, gen1, gen2;

				//Informationen holen
				syntaxTree->getIfStmt( index, expr, stmt, dummy, l1, l2 );

				//in parent steht der Index des Labels
				gen1 = syntaxTree->getParent( l1 );
				gen2 = syntaxTree->getParent( l2 );

				//das Label am Anfang des expr-Teils ausgeben
				print( l1 );

				//Code für expr ausgeben
				print( expr );

				//Datenregister des Ergebnis holen
				dr = syntaxTree->getDataRegister( expr );

				//Ergebnis auf true oder false testen
				cout << "\ttst.b\td" << dr << endl;

				//Ergebnis ist false
				cout << "\tbne\t\tL" << gen2 << endl;

				//stmt-Teil ausgeben (true-Teil)
				print( stmt );

				//Sprung zum Anfang des expr-Teils
				cout << "\tbra\t\tL" << gen1 << endl;

				//Label am Ende des stmt-Teils erzeugen
				print( l2 );

				//das Datenregister für die Auswertung von expr freigeben
				freeDataRegister( dr );
				break;
			}

			case CSyntaxTree::_DOWHILE:
			{
				//do-while-Statement ausgeben
				CSymbolTable::storageType	leftType, rightType;
				Register					dr, rdr;
				long						expr, stmt, dummy, l1, l2, gen1, gen2;

				//Informationen holen
				syntaxTree->getIfStmt( index, expr, stmt, dummy, l1, l2 );

				//in parent steht der Index des Labels
				gen1 = syntaxTree->getParent( l1 );
				gen2 = syntaxTree->getParent( l2 );

				//das Label am Anfang des stmt-Teils ausgeben
				print( l1 );

				//stmt-Teil ausgeben
				print( stmt );

				//Code für expr ausgeben
				print( expr );

				//Datenregister des Ergebnis holen
				dr = syntaxTree->getDataRegister( expr );

				//Ergebnis auf true oder false testen
				cout << "\ttst.b\td" << dr << endl;

				//Ergebnis ist false, Schleife verlassen
				cout << "\tbne\t\tL" << gen2 << endl;

				//Schleifenbody nochmal ausführen
				cout << "\tbra\t\tL" << gen1 << endl;

				//Label am Ende des stmt-Teils erzeugen
				print( l2 );

				//das Datenregister für die Auswertung von expr freigeben
				freeDataRegister( dr );
				break;
			}

			case CSyntaxTree::_LABEL:
			{
				//ein Label wird sofort ausgegeben
				long	lab;

				//die Nummer des Labels steht in parent
				lab = syntaxTree->getParent( index );

				//Label ausgeben
				cout << "L" << lab << ":" << endl;
				break;
			}

			case CSyntaxTree::_BLK_STMT:
			{
				//Block-Statement
				vector<long>	stmt;
				long			localVarSize, stmtSize, i;

				//Länge der lokalen Variablen dieses Blocks
				localVarSize = syntaxTree->getLocalVar( index );

				//Platz für lokale Variablen auf dem Laufzeit-Stack schaffen
				cout << "\tlink.l\ta6,#-" << localVarSize << endl;

				//Code für Block ausgeben
				print( right );

				//Platz für lokale Variablen auf dem Laufzeit-Stack entfernen
				cout << "\tunlk.l\ta6" << endl;
				break;
			}

			case CSyntaxTree::_EQ:
			{
				//gleich, equal
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Verleichs-Operator ausgeben
				cout << "\tcmp";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//nimm das Register rdr um das Ergebnis zu speichern
				cout << "\tseq.b\td" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Comparision has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den ==-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_NEQ:
			{
				//ungleich, not equal
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Verleichs-Operator ausgeben
				cout << "\tcmp";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//nimm das Register rdr um das Ergebnis zu speichern
				cout << "\tsne.b\td" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Comparision has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den !=-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_LT:
			{
				//kleiner als, less than
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Verleichs-Operator ausgeben
				cout << "\tcmp";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//nimm das Register rdr um das Ergebnis zu speichern
				cout << "\tslt.b\td" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Comparision has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den <-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_LE:
			{
				//kleiner gleich, less equal
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Verleichs-Operator ausgeben
				cout << "\tcmp";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//nimm das Register rdr um das Ergebnis zu speichern
				cout << "\tsle.b\td" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Comparision has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den <=-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_GT:
			{
				//größer als, greater than
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Verleichs-Operator ausgeben
				cout << "\tcmp";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//nimm das Register rdr um das Ergebnis zu speichern
				cout << "\tsgt.b\td" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Comparision has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den >-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_GE:
			{
				//größer gleich, greater equal
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Verleichs-Operator ausgeben
				cout << "\tcmp";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//nimm das Register rdr um das Ergebnis zu speichern
				cout << "\tsge.b\td" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Comparision has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den <=-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, CSymbolTable::_BOOL );

				break;
			}

			case CSyntaxTree::_BIN_AND:
			{
				//binäres und
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//binäres und ausgeben
				cout << "\tand";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Binary and has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den &-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_BIN_OR:
			{
				//binäres oder
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//binäres oder ausgeben
				cout << "\tor";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Binary or has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den |-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_BIN_XOR:
			{
				//binäres x-oder
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//binäres x-oder ausgeben
				cout << "\txor";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Binary xor has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den ^-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_BIN_NOT:
			{
				//binäres nicht
				CSymbolTable::storageType	type;
				Register					dr;

				/*
					Dieser Knoten hat nur ein rechtes Kind!
				*/


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				dr = syntaxTree->getDataRegister( right );



				//binäres nicht ausgeben
				cout << "\tnot";

				//Breite ausgeben
				printStorageType( type );

				//Register ausgeben
				cout << "\td" << dr << endl;


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( dr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Binary not has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den ~-Knoten eintragen
					syntaxTree->setDataRegister( index, dr, type );

				break;
			}

			case CSyntaxTree::_LEFT_SHIFT:
			{
				//links shift
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//>> ausgeben
				cout << "\tlsl";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << rdr << ",d" << ldr << endl;

				//das Ergenbis steht in ldr, also rdr freigeben
				freeDataRegister( rdr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( ldr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Left shift has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den <<-Knoten eintragen
					syntaxTree->setDataRegister( index, ldr, leftType );

				break;
			}

			case CSyntaxTree::_RIGHT_SHIFT:
			{
				//rechts shift
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//>> ausgeben
				cout << "\tlsr";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << rdr << ",d" << ldr << endl;

				//das Ergenbis steht in ldr, also rdr freigeben
				freeDataRegister( rdr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( ldr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Right shift has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den >>-Knoten eintragen
					syntaxTree->setDataRegister( index, ldr, leftType );

				break;
			}

			case CSyntaxTree::_MUL:
			{
				//Multiplikation
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Multiplikation ausgeben
				cout << "\tmul";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Multiplication has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den *-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_DIV:
			{
				//Division
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Division ausgeben
				cout << "\tdiv";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Division has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den /-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_ADD:
			{
				//Addition
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Addition ausgeben
				cout << "\tadd";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Addition has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den +-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_SUB:
			{
				//Subtraktion
				CSymbolTable::storageType	leftType, rightType;
				Register					ldr, rdr;

				//Code für das linke Kind ausgeben
				print( left );

				//storage type des linken Kindes
				leftType = syntaxTree->getDataRegisterStorageType( left );

				//Datenregister des linken Kindex holen
				ldr = syntaxTree->getDataRegister( left );


				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindex holen
				rdr = syntaxTree->getDataRegister( right );



				//Subtraktion ausgeben
				cout << "\tsub";

				//Breite ausgeben
				printStorageType( leftType );

				//Register ausgeben
				cout << "\t";
				cout << "d" << ldr << ",d" << rdr << endl;

				//das Ergenbis steht in rdr, also ldr freigeben
				freeDataRegister( ldr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
				{
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Ausdruck hat keinen Effekt
					cerr << "CCodeBase::print: Line " << line << ": Subtraction has no effect!" << endl;
				}

				else
					//Knoten wird noch benötigt, Register in den --Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, leftType );

				break;
			}

			case CSyntaxTree::_EQU:
			{
				//Zuweisung
				CSymbolTable::storageType	leftType, rightType, type;
				Register					rdr;

				//ist das linke Kind ein Leaf?
				if( syntaxTree->isNode( left ) )
				{
					//nein, ist ein Knoten
					cerr << "CCodeBase::print: Left child is node in equ-expression!" << endl;

					//Ende
					exit( 1 );
				}

				//Index in Symboltabelle holen
				long symTabIndex = syntaxTree->getSymTabIndex( left );

				//Storage type holen
				leftType = symTab->getStorageType( symTabIndex );



				//Code für das rechte Kind ausgeben
				print( right );

				//storage type des rechten Kindes
				rightType = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister des rechten Kindes holen
				rdr = syntaxTree->getDataRegister( right );


				//Inhalt des Datenregisters rdr speichern
				SaveLValue( left, rdr );


				//Wurzel?
				if( syntaxTree->isRoot( index ) )
					//ja, ist die Wurzel, Datenregister freigeben
					freeDataRegister( rdr );

				else
				{
					//Knoten wird noch benötigt, Register in den =-Knoten eintragen
					syntaxTree->setDataRegister( index, rdr, type );

					/*
						Wenn eine Zuweisung nicht Root ist, dann könnte es sich um
						einen Fehler handeln!
					*/
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Warnung ausgeben
					cerr << "CCodeBase::print: line " << line << ": Possible wrong assignment!" << endl;
				}

				break;
			}

			case CSyntaxTree::_SHORT2BOOL:
			{
				/*
					Eine Typkonvertierung von SHORT zu BOOL durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ SHORT sein
				if( type != CSymbolTable::_SHORT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				//Code ausgeben
				cout << "\ttst.w\td" << dr << endl;

				//nun in 0 oder 1 wandeln
				cout << "\tsne.b\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_SHORT2CHAR:
			{
				/*
					Eine Typkonvertierung von SHORT zu CHAR durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ SHORT sein
				if( type != CSymbolTable::_SHORT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_CHAR );

				/*
					Hier muss kein Code erzeugt werden, da einfach statt move.w
					move.b benutzt wird.
				*/
				break;
			}

			case CSyntaxTree::_SHORT2INT:
			{
				/*
					Eine Typkonvertierung von SHORT zu INT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ SHORT sein
				if( type != CSymbolTable::_SHORT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_INT );

				//Code ausgeben
				cout << "\text.l\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_SHORT2LONG:
			{
				/*
					Eine Typkonvertierung von SHORT zu LONG durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ SHORT sein
				if( type != CSymbolTable::_SHORT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_LONG );

				//Code ausgeben
				cout << "\text.l\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_INT2BOOL:
			{
				/*
					Eine Typkonvertierung von INT zu BOOL durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ INT sein
				if( type != CSymbolTable::_INT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				//Code ausgeben
				cout << "\ttst.l\td" << dr << endl;

				//nun in 0 oder 1 wandeln
				cout << "\tsne.b\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_INT2LONG:
			{
				/*
					Eine Typkonvertierung von INT zu LONG durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ INT sein
				if( type != CSymbolTable::_INT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_LONG );

				/*
					Hier muss kein Code erzeugt werden, da intern LONG und INT
					äquivalent sind.
				*/
				break;
			}

			case CSyntaxTree::_INT2SHORT:
			{
				/*
					Eine Typkonvertierung von INT zu SHORT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ INT sein
				if( type != CSymbolTable::_INT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_SHORT );

				/*
					Hier muss kein Code erzeugt werden, da einfach statt move.l
					move.w benutzt wird.
				*/
				break;
			}

			case CSyntaxTree::_INT2CHAR:
			{
				/*
					Eine Typkonvertierung von INT zu CHAR durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ INT sein
				if( type != CSymbolTable::_INT )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_CHAR );

				/*
					Hier muss kein Code erzeugt werden, da einfach statt move.l
					move.b benutzt wird.
				*/
				break;
			}

			case CSyntaxTree::_BOOL2CHAR:
			{
				/*
					Eine Typkonvertierung von BOOL zu CHAR durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ BOOL sein
				if( type != CSymbolTable::_BOOL )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_CHAR );

				/*
					Hier muss kein Code ausgegeben werden, da BOOL und CHAR beide
					8 Bit-Datentypen sind.
				*/
				break;
			}

			case CSyntaxTree::_BOOL2SHORT:
			{
				/*
					Eine Typkonvertierung von BOOL zu SHORT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ BOOL sein
				if( type != CSymbolTable::_BOOL )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_SHORT );

				//Code ausgeben
				cout << "\text.w\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_BOOL2INT:
			{
				/*
					Eine Typkonvertierung von BOOL zu INT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ BOOL sein
				if( type != CSymbolTable::_BOOL )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_INT );

				//Code ausgeben
				cout << "\textb.l\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_BOOL2LONG:
			{
				/*
					Eine Typkonvertierung von BOOL zu LONG durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ BOOL sein
				if( type != CSymbolTable::_BOOL )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_LONG );

				//Code ausgeben
				cout << "\textb.l\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_CHAR2BOOL:
			{
				/*
					Eine Typkonvertierung von CHAR zu BOOL durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ CHAR sein
				if( type != CSymbolTable::_CHAR )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				//Code ausgeben
				cout << "\ttst.b\td" << dr << endl;

				//nun in 0 oder 1 wandeln
				cout << "\tsne.b\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_CHAR2SHORT:
			{
				/*
					Eine Typkonvertierung von CHAR zu SHORT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ CHAR sein
				if( type != CSymbolTable::_CHAR )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_SHORT );

				//Code ausgeben
				cout << "\text.w\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_CHAR2INT:
			{
				/*
					Eine Typkonvertierung von CHAR zu INT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ CHAR sein
				if( type != CSymbolTable::_CHAR )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_INT );

				//Code ausgeben
				cout << "\textb.l\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_CHAR2LONG:
			{
				/*
					Eine Typkonvertierung von CHAR zu LONG durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ CHAR sein
				if( type != CSymbolTable::_CHAR )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_LONG );

				//Code ausgeben
				cout << "\textb.l\td" << dr << endl;
				break;
			}

			case CSyntaxTree::_LONG2INT:
			{
				/*
					Eine Typkonvertierung von LONG zu INT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ LONG sein
				if( type != CSymbolTable::_LONG )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_INT );

				/*
					Hier muss kein Code erzeugt werden, da intern LONG und INT
					äquivalent sind.
				*/
				break;
			}

			case CSyntaxTree::_LONG2SHORT:
			{
				/*
					Eine Typkonvertierung von LONG zu SHORT durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ LONG sein
				if( type != CSymbolTable::_LONG )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_SHORT );

				/*
					Hier muss kein Code erzeugt werden, da einfach statt move.l
					move.w benutzt wird.
				*/
				break;
			}

			case CSyntaxTree::_LONG2CHAR:
			{
				/*
					Eine Typkonvertierung von LONG zu CHAR durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ LONG sein
				if( type != CSymbolTable::_LONG )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_CHAR );

				/*
					Hier muss kein Code erzeugt werden, da einfach statt move.l
					move.b benutzt wird.
				*/
				break;
			}

			case CSyntaxTree::_LONG2BOOL:
			{
				/*
					Eine Typkonvertierung von LONG zu BOOL durchführen.
					Dieser Knoten hat nur ein rechtes Kind!
				*/
				CSymbolTable::storageType	type;
				Register					dr;

				//zuerst das rechte Kind auswerten
				print( right );

				//im Knoten des rechten Kindes steht jetzt das Register

				//storage type des rechten Kindes
				type = syntaxTree->getDataRegisterStorageType( right );

				//Datenregister holen, das gecastet werden soll
				dr = syntaxTree->getDataRegister( right );

				//das Datenregister muss vom Typ LONG sein
				if( type != CSymbolTable::_LONG )
				{
					//Zeile holen
					long line = syntaxTree->getLine( index );

					//Fehler
					cerr << "CCodeBase::print: Line " << line << ": Wrong type in data register!" << endl;

					//Ende
					exit( 1 );
				}

				//Register eintragen
				syntaxTree->setDataRegister( index, dr, CSymbolTable::_BOOL );

				//Code ausgeben
				cout << "\ttst.l\td" << dr << endl;

				//nun in 0 oder 1 wandeln
				cout << "\tsne.b\td" << dr << endl;
				break;
			}
/*
			default:
			{
				//unbekannter Operator
				cerr << "CCodeBase::print: Unknown operator " << op << "!" << endl;

				//Ende
				exit( 1 );
			}
*/
		}

		//nächstes Statement in der Stement-Verkettung ausgeben
		print( next );
	}

	else
	{
		//ist ein Leaf
		CSymbolTable::storageType	type;
		Register					dr;

		//Index in Symboltabelle holen
		long symTabIndex = syntaxTree->getSymTabIndex( index );

		//Storage type holen
		type = symTab->getStorageType( symTabIndex );

		//den Inhalt der Vereinbarung in ein Datenregister laden
		dr = LoadRValue( index );

		//Register eintragen
		syntaxTree->setDataRegister( index, dr, type );
	}
}


void CCodeBase::printStorageType( CSymbolTable::storageType type ) const
{
	//Storage Type ausgeben

	//welcher storage type?
	switch( type )
	{
		case CSymbolTable::_CHAR:
		case CSymbolTable::_BOOL:
		{
			//Bytes und Bools benötigen ein Byte
			cout << ".b";
			break;
		}

		case CSymbolTable::_SHORT:
		{
			//Shorts benötigen zwei Bytes
			cout << ".w";
			break;
		}

		case CSymbolTable::_INT:
		case CSymbolTable::_LONG:
		{
			//Integers benötigen vier Bytes
			cout << ".l";
			break;
		}

		default:
		{
			//Fehler
			cerr << "CCodeBase::printStorageType: Unknown storage type!" << endl;

			//Ende
			exit( 1 );
		}
	}
}


CCodeBase::Register CCodeBase::LoadRValue( long index )
{
	//lädt eine lokale/globale Vereinbarung in ein Datenregister
	CSymbolTable::varType	varType;
	CCodeBase::Register		reg;
	long					symTabIndex;
	int						offset;

	//wenn index ein Node ist, dann wurde das Ergebnis ja schon in ein Datenregister
	//abgelegt
	if( syntaxTree->isNode( index ) )
		//ja, ist ein Node
		return( syntaxTree->getDataRegister( index ) );

	//hole ein freies Datenregister
	reg = getFreeDataRegister();

	//Index in Symboltabelle holen
	symTabIndex = syntaxTree->getSymTabIndex( index );

	//Typ holen
	varType = symTab->getType( symTabIndex );

	//Offset der lokalen Variablen zum lokalen Stackframe holen
	offset = symTab->getOffset( symTabIndex );

	//Storage type holen
	CSymbolTable::storageType storageType = symTab->getStorageType( symTabIndex );

	//Offset berechnen
	offset = offset + symTab->getWidthInBytes( storageType );


	//Register eintragen
	syntaxTree->setDataRegister( index, reg, storageType );


	//Name der Variablen
	char *name = symTab->getName( symTabIndex );

	//Zahl, nicht Variable?
	if( varType == CSymbolTable::_NUMBER )
	{
		long	val = syntaxTree->getNumber( index );

		//ist eine Zahl
		cout << "\tmove";

		//storage Type ausgeben
		printStorageType( storageType );

		//Zahl direkt angeben
		cout << "\t#" << val << ",d" << reg;
	}

	//lokale oder globale Variable?
	else if( symTab->isLocal( symTabIndex ) )
	{
		//lokale Variable

		//ist die lokale Variable in einem anderen Block deklariert?
		int	diff = syntaxTree->getBlkDiff( index );

		//anderer Block?
		if( diff )
		{
			CCodeBase::Register	adrReg;
			int					i;

			//hole ein freies Adressregister
			adrReg = getFreeAdressRegister();

			//ja, den Stackframe laden, in dem die lokale Variable deklariert wurde
			cout << "\tmove.l\t(a6),a" << adrReg << endl;

			//alle weiteren Level abarbeiten
			for( i = 1; i < diff; i++ )
				//Zeiger auf Stackframe für lokale Variablen holen
				cout << "\tmove.l\t(a" << adrReg << "),a" << adrReg << endl;

			//lokale Variable in Datenregister laden
			cout << "\tmove";

			//storage Type ausgeben
			printStorageType( storageType );

			//von adrReg aus
			cout << "\t-" << offset << "(a" << adrReg << "),d" << reg;

			//Adressregister freigeben
			freeAdressRegister( adrReg );
		}

		else
		{
			//nein, im gleichen Block

			//lokale Variable in Datenregister laden
			cout << "\tmove";

			//storage Type ausgeben
			printStorageType( storageType );

			//von adrReg aus
			cout << "\t-" << offset << "(a6),d" << reg;
		}
	}

	else
	{
		//ist eine globale Variable
		cout << "\tmove";

		//storage Type ausgeben
		printStorageType( storageType );

		//in Datenregister laden
		cout << "\t" << name << ",d" << reg;
	}

	//Name der Variablen ausgeben
	if( name )
		//als Kommentar anhängen
		cout << "\t;" << name << endl;

	else
		//neue Zeile
		cout << endl;

	//Datenregister zurückliefern
	return( reg );
}


void CCodeBase::SaveLValue( long index, CCodeBase::Register reg )
{
	//speichert den Inhalt des Datenregisters reg in eine Vereinbarung

	//Index in Symboltabelle holen
	long symTabIndex = syntaxTree->getSymTabIndex( index );

	//Offset zum Stackframe holen
	int offset = symTab->getOffset( symTabIndex );

	//Storage type holen
	CSymbolTable::storageType storageType = symTab->getStorageType( symTabIndex );

	//Offset berechnen
	offset = offset + symTab->getWidthInBytes( storageType );


	//Name der Variablen
	char *name = symTab->getName( symTabIndex );


	//lokale oder globale Variable?
	if( symTab->isLocal( symTabIndex ) )
	{
		//lokale Variable

		//ist die lokale Variable in einem anderen Block deklariert?
		int	diff = syntaxTree->getBlkDiff( index );

		//anderer Block?
		if( diff )
		{
			CCodeBase::Register	adrReg;
			int					i;

			//hole ein freies Adressregister
			adrReg = getFreeAdressRegister();

			//ja, den Stackframe laden, in dem die lokale Variable deklariert wurde
			cout << "\tmove.l\t(a6),a" << adrReg << endl;

			//alle weiteren Level abarbeiten
			for( i = 1; i < diff; i++ )
				//Zeiger auf Stackframe für lokale Variablen holen
				cout << "\tmove.l\t(a" << adrReg << "),a" << adrReg << endl;

			//Datenregister eintragen
			cout << "\tmove";

			//storage Type ausgeben
			printStorageType( storageType );

			//von adrReg aus
			cout << "\td" << reg << ",-" << offset << "(a" << adrReg << ")";

			//Adressregister freigeben
			freeAdressRegister( adrReg );
		}

		else
		{
			//nein, im gleichen Block

			//von Datenregister aus
			cout << "\tmove";

			//storage Type ausgeben
			printStorageType( storageType );

			//Datenregister
			cout << "\td" << reg << ",-" << offset << "(a6)";
		}
	}

	else
	{
		//ist eine globale Variable

		//von Datenregister aus
		cout << "\tmove";

		//storage Type ausgeben
		printStorageType( syntaxTree->getDataRegisterStorageType( index ) );

		//Datenregister
		cout << "\td" << reg << "," << name;
	}

	//Kommentar
	cout << "\t;" << name << endl;
}

