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

#include "csymboltable.h"


//Debug-Meldungen ausgeben
#define DEBUG


/*
	Organisation der Symboltabelle.

	Es wird ein Array implementiert, welches für jeden Block (Verschachtelungstiefe)
	einen Zeiger auf ein Symboltabllen-Eintrag enthält.
	Dieses Array wird als Stack implementiert. Die Symboltabllen-Einträge Variabler
	gleichen Blocks werden dabei doppelt verkettet. Wird ein Block verlassen, so wird der
	Stack-Pointer um eins reduziert. Die Symboltabellen-Einträge gehen dabei nicht verloren,
	denn die Syntaxbäume für die Ausdrücke (und nur in denen wird ja auf Variablen/Konstanten
	zugegeriffen) enthalten für jede Variable einen Zeiger auf den zugehörigen Symboltabllen-
	Eintrag.
*/


CSymbolTable::CSymbolTable()
{
	unsigned long	i;

	//erzeuge Speicher für symTabSize Blöcke
	symTab = new( SYMBOL_TABLE *[symTabSize] );

	//alle initialisieren
	memset( symTab, 0, sizeof( SYMBOL_TABLE * ) * symTabSize );

	//Position in symTab, d.h. aktuelle Blocktiefe
	symTabPos = 0;

	//maximale Blocktiefe
	symTabMaxPos = symTabSize;


	//zum Sichern ALLER erzeugter Symboltabellen-Einträge
	saveSymTabPos = 0;
	saveSymTabMaxPos = 0;
}


CSymbolTable::~CSymbolTable()
{
	//wurden alle Blöcke abgemeldet?
	if( symTabPos )
	{
		//nein, Fehler
		cerr << "CSymbolTable::~symbolTable: " << symTabPos << " block(s) still open, missing call to decBlockDepth!" << endl;

		//Ende
		exit( 1 );
	}

	//Speicher für das Array freigeben
	delete[] ( symTab );
}


void CSymbolTable::incBlockDepth( void )
{
	//es wird ein neuer Block betreten
	symTabPos++;

	//Array vergrößern?
	if( symTabPos == symTabMaxPos )
	{
		//ja, vergrößern
		SYMBOL_TABLE	**tmp;

		//Speicher anfordern
		tmp = new( SYMBOL_TABLE *[symTabMaxPos + symTabSize] );

		//initialisieren
		memset( tmp, 0, sizeof( SYMBOL_TABLE * ) * ( symTabMaxPos + symTabSize ) );

		//alte Einträge kopieren
		memcpy( tmp, symTab, sizeof( SYMBOL_TABLE * ) * symTabPos );

		//maximale Blocktiefe
		symTabMaxPos += symTabSize;

		//Speicher freigeben
		delete[] ( symTab );

		//Zeiger kopieren
		symTab = tmp;
	}
}


unsigned long CSymbolTable::insertSaveSymTab( SYMBOL_TABLE *symTab )
{
	//einen neu erzeugten Eintrag speichern

	//genug Speicher vorhanden?
	if( saveSymTabPos == saveSymTabMaxPos )
	{
		//nein, anfordern
		SYMBOL_TABLE	**tmp;

		//neue maximale Größe anpassen
		saveSymTabMaxPos += symTabSize;

		//Speicher anfordern
		tmp = new( SYMBOL_TABLE * [saveSymTabMaxPos + saveSymTabPos] );

		//alte Einträge kopieren
		memcpy( tmp, saveSymTab, sizeof( SYMBOL_TABLE * ) * saveSymTabPos );

		//Speicher freigeben?
		if( saveSymTab )
			//ja, freigeben
			delete[] ( saveSymTab );

		//Zeiger kopieren
		saveSymTab = tmp;
	}

	//eintragen
	saveSymTab[saveSymTabPos] = symTab;

	//Index zurückliefern
	return( saveSymTabPos++ );
}


void CSymbolTable::decBlockDepth( void )
{
	//es wird ein Block verlassen

	//prüfen, ob auch alle Vereinbarungen der aktuellen Blocktiefe auch benutzt wurden
	SYMBOL_TABLE	*tmp = symTab[symTabPos];

	//gibt es überhaupt schon Einträge für diese Blocktiefe?
	while( tmp )
	{
		//Vereinbarung benutzt?
		if( tmp->_name && ( tmp->_used == false ) )
			//Warnung ausgeben
			cout << "miniC: Line " << tmp->_line << ": " << tmp->_name << " declared but never used!" << endl;

		//weiter
		tmp = tmp->_next;
	}

	//Eintrag zurücksetzen
	symTab[symTabPos--] = 0L;

	//Unterlauf?
	if( symTabPos < 0 )
	{
		//Fehler
		cerr << "CSymbolTable::decBlockDepth: Underflow!" << endl;

		//Ende
		exit( 1 );
	}
}


long CSymbolTable::insert( char *name, int blkDepth, long line )
{
	/*
		Es soll ein Element in die Symboltabelle eingetragen werden. Um einen Eintrag eindeutig
		zuordnen zu können, ist z.e. der Name, zum anderen die Blocktiefe notwenig.
	*/
	SYMBOL_TABLE	*_symTab;
	long			ret;

	//wurde die aktuelle Blocktiefe bereits initialisiert?
	if( blkDepth > symTabPos )
	{
		//nein, Fehler
		cerr << "CSymbolTable::insert: Forgotten call to incBlockDepth!" << endl;
		cerr << "CSymbolTable::insert: Name: " << name << endl;
		cerr << "CSymbolTable::insert: blkDepth: " << blkDepth << endl;

		//Ende
		exit( 1 );
	}

	//schauen, ob der Eintrag bereits in der Symboltabelle eingetragen wurde
	if( lookup( name, blkDepth ) )
		//der Eintrag existiert schon
		return( -1 );

	//eine neue Symboltabellen-Struktur erzeugen
	_symTab = new( SYMBOL_TABLE );

	//den Eintrag merken
	ret = insertSaveSymTab( _symTab );

	//initialisieren
	memset( _symTab, 0, sizeof( SYMBOL_TABLE ) );

	//ist schon ein Eintrag für diese Blocktiefe vorhanden?
	if( symTab[blkDepth] )
	{
		//ja, diesen an den letzten Eintrag anhängen
		SYMBOL_TABLE	*tmp = symTab[blkDepth];

		//frei?
		while( tmp->_next )
			//nächster Eintrag
			tmp = tmp->_next;

		//doppelt-verkettet anhängen
		tmp->_next = _symTab;
		_symTab->_prev = tmp;
	}

	else
		//es gibt noch keinen Eintrag für diese Blocktiefe
		symTab[blkDepth] = _symTab;

	//Namen eintragen
	_symTab->_name = name;

	//Index im saveSymTab Array
	_symTab->_saveSymTabIndex = ret;

	//Verschachtelungstiefe
	_symTab->_blk = blkDepth;

	//Vereinbarung wurde noch nicht benutzt
	_symTab->_used = false;

	//Zeile im Quelltext
	_symTab->_line = line;

#ifdef DEBUG
	//Meldung ausgeben
	cerr << "CSymbolTable::insert: Trage " << name << " mit Blocktiefe " << blkDepth << " ein." << endl;
#endif

	//Index zurückliefern
	return( ret );
}


CSymbolTable::SYMBOL_TABLE *CSymbolTable::lookup( char *name, int blkDepth ) const
{
	//Suchen eines Eintrags
	SYMBOL_TABLE	*tmp = symTab[blkDepth];

	//überhaupt ein Name angegeben?
	if( name )
	{
		//gibt es überhaupt schon Einträge für diese Blocktiefe?
		while( tmp && tmp->_name )
		{
			//gesuchter Eintrag?
			if( !strcmp( tmp->_name, name ) )
				//ja, gefunden
				return( tmp );

			//weiter
			tmp = tmp->_next;
		}
	}

	//nicht gefunden
	return( 0L );
}










/*
	Hier folgen die öffentlichen Routinen, die genutzt werden, um die
	diversen Attribute der eingetragenen Alemente nachzutragen.
*/


void CSymbolTable::setStorageType( long index, CSymbolTable::storageType _storageType )
{
	//den Typ einer Variablen/Konstanten eintragen, also CHAR, INT, SHORT, ...

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Typ eintragen
		tmp->_storageType = _storageType;
	}

	else
	{
		//Eintrag existiert gar nicht
		char	*storageTypeName[] = { "", "CHAR", "INT", "SHORT", "LONG" };

		//Meldung ausgeben
		cerr << "CSymbolTable::setStorageType: Entry not existent!" << endl;
		cerr << "CSymbolTable::setStorageType: Storage type: " << storageTypeName[_storageType] << endl;
		cerr << "CSymbolTable::setStorageType: Index: " << index;

		//Ende
		exit( 1 );
	}
}


CSymbolTable::storageType CSymbolTable::getStorageType( long index )
{
	//den Typ einer Variablen/Konstanten eintragen, also CHAR, INT, SHORT, ...

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Typ zurückliefern
		return( tmp->_storageType );
	}

	else
	{
		//Meldung ausgeben
		cerr << "CSymbolTable::getStorageType: Entry not existent!" << endl;
		cerr << "CSymbolTable::getStorageType: Index: " << index;

		//Ende
		exit( 1 );
	}
}


bool CSymbolTable::isLocal( long index )
{
	/*
		Wenn die Blocktiefe dieser Vereinbarung 0 ist, dann ist es eine globalen
		Variable.
	*/

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//lokale Variable?
		return( tmp->_blk ? true : false );
	}

	else
	{
		//Eintrag existiert gar nicht

		//Meldung ausgeben
		cerr << "CSymbolTable::isLocal: Entry not existent!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSymbolTable::setOffset( long index, int offset )
{
	/*
		Lokale Variablen werden auf den Laufzeit-Stack abgelegt. Nun wird der Offset
		zu dieser Variablen eingetragen.
		Bei globalen Variablen ist es ähnlich.
	*/

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Offset eintragen
		tmp->_offset = offset;
	}

	else
	{
		//Eintrag existiert gar nicht

		//Meldung ausgeben
		cerr << "CSymbolTable::setOffset: Entry not existent!" << endl;
		cerr << "CSymbolTable::setOffset: Offset: " << offset << endl;

		//Ende
		exit( 1 );
	}
}


int CSymbolTable::getOffset( long index )
{
	/*
		Lokale Variablen werden auf den Laufzeit-Stack abgelegt. Hier wird der Offset
		zu dieser Variablen zurückgeliefert.
		Bei globalen Variablen ist es ähnlich.
	*/

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Offset zurückliefern
		return( tmp->_offset );
	}

	else
	{
		//Eintrag existiert gar nicht

		//Meldung ausgeben
		cerr << "CSymbolTable::getOffset: Entry not existent!" << endl;

		//Ende
		exit( 1 );
	}
}


void CSymbolTable::setType( long index, CSymbolTable::varType type )
{
	//Variable oder Zahl

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Typ eintragen
		tmp->_type = type;
	}

	else
	{
		//Eintrag existiert gar nicht
		char	*varTypeName[] = { "", "VAR", "NUMBER" };

		//Meldung ausgeben
		cerr << "CSymbolTable::setType: Entry not existent!" << endl;
		cerr << "CSymbolTable::setType: Type: " << varTypeName[type] << endl;
		cerr << "CSymbolTable::setType: Index: " << index;

		//Ende
		exit( 1 );
	}
}


CSymbolTable::varType CSymbolTable::getType( long index )
{
	//Variable oder Zahl

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Typ zurückliefern
		return( tmp->_type );
	}

	else
	{
		//Meldung ausgeben
		cerr << "CSymbolTable::getType: Entry not existent!" << endl;
		cerr << "CSymbolTable::getType: Index: " << index;

		//Ende
		exit( 1 );
	}
}


long CSymbolTable::find( char *name )
{
	/*
		Wenn in einer expression auf eine Variable zugegriffen wird, dann wird hier der
		entsprechende Eintragin der Symboltabelle gesucht.
		Dazu wird zuerst im aktuellen Block gesucht. Wird der Eintrag nicht gefunden, wird in einem
		vorherigen Block gesucht (Verschachtelungstiefe).
		Der letztmögliche Block ist der, in dem die globalen Vereinbarungen getroffwn wurden.
	*/
	SYMBOL_TABLE	*tmp;
	long			pos = symTabPos;

	//solange noch Blöcke da sind
	while( pos >= 0 )
	{
		//Zeiger auf den ersten Eintrag holen
		tmp = symTab[pos];

		//sind noch Einträge da?
		while( tmp )
		{
			//ja, Eintrag gefunden?
			if( tmp->_name && !strcmp( tmp->_name, name ) )
				//ja
				return( tmp->_saveSymTabIndex );

			else
				//nein, nächster
				tmp = tmp->_next;
		}

		//nächsthöheren Block prüfen
		pos--;
	}

	//Vereinbarung nicht gefunden
	return( -1 );
}


int CSymbolTable::getWidthInBytes( storageType type )
{
	//liefert die Breite in Bytes eines jeden storage types zurück

	//welcher storage type?
	switch( type )
	{
		case _CHAR:
		case _BOOL:
			//Bytes und Bools benötigen zwei Bytes
			return( 2 );

		case _SHORT:
			//Shorts benötigen zwei Bytes
			return( 2 );

		case _INT:
		case _LONG:
			//Integers benötigen vier Bytes
			//Longs benötigen vier Bytes
			return( 4 );

		default:
		{
			//Fehler
			cerr << "CSymbolTable::getWidthInBytes: Unknown storage type!" << endl;

			//Ende
			exit( 1 );
		}
	}
}


char *CSymbolTable::getName( long index )
{
	//liefert einen Zeiger auf den Namen der Vereinbarung zurück

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Name zurückliefern
		return( tmp->_name );
	}

	else
	{
		//Eintrag existiert gar nicht

		//Meldung ausgeben
		cerr << "CSymbolTable::getName: Entry not existent!" << endl;
		cerr << "CSymbolTable::getName: Index: " << index;

		//Ende
		exit( 1 );
	}
}


int CSymbolTable::getBlkDepth( long index )
{
	//liefert die Blocktiefe der Vereinbarung zurück

	//existiert der Eintrag?
	if( saveSymTabPos > index )
	{
		//Zeiger holen
		SYMBOL_TABLE	*tmp = saveSymTab[index];

		//Blocktiefe zurückliefern
		return( tmp->_blk );
	}

	else
	{
		//Eintrag existiert gar nicht

		//Meldung ausgeben
		cerr << "CSymbolTable::getBlkDepth: Entry not existent!" << endl;
		cerr << "CSymbolTable::getBlkDepth: Index: " << index;

		//Ende
		exit( 1 );
	}
}
