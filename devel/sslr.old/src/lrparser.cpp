/***************************************************************************
                          lrparser.cpp  -  description
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

#include "lrparser.h"


//Debug-Modus
#define SSLR_DEBUG
#undef SSLR_DEBUG



/*
	Es gibt zwei Möglichkeiten, die LALR(1)-Mengen zu erzeugen:
	1. Direkt gem. Algorithmus 4.13, S. 295/260 Drachenbuch I, oder
	2. erst durch die Erzeugung der LR(1)-Items, dann, durch Zusammenfassen gleicher
	Kerne.

	Ist LR2LALR definiert, wird Methode 2 angewandt.
*/
#define LR2LALR
#undef LR2LALR




//Für Debugging-Zwecke
char *scanner_token[] =
{
	/* 0*/ "IDENT",
	/* 1*/ "END",
	/* 2*/ "SEPERATOR",
	/* 3*/ "LEFT",
	/* 4*/ "RIGHT",
	/* 5*/ "TERM_TYPE",
	/* 6*/ "NTERM_TYPE",
	/* 7*/ "PREC",
	/* 8*/ "ERROR",
	/* 9*/ "PROD_END",
	/*10*/ "PROD_NEXT",
	/*11*/ "INCL",
	/*12*/ "DEF",
	/*13*/ "GRAMMAR",
	/*14*/ "SEM_TYPE",
	/*15*/ "BOPEN",
	/*16*/ "BCLOSE",
	/*17*/ "KLOPEN",
	/*18*/ "KLCLOSE",
	/*19*/ "EOPEN",
	/*20*/ "ECLOSE",
	/*21*/ "UNION",
	/*22*/ "NUMBER",
	/*23*/ "STRING",
	/*24*/ "SEM_ACT",
	/*25*/ "EXTENSION",
	/*27*/ "TERMINAL"
	/*28*/ "EXCLAMATION",
	/*29*/ "COMMA",
	/*30*/ "AST"
};


lrparser::lrparser( char *name, ARGV_STR *argv )
{
	//Initialisieren
	anzCheck = 0;
	check = 0L;
	prod_anz = 0;
	prod_str = 0L;
	prod2_anz = 0;
	prod2_str = 0L;
	error_anz = 0;
	error_str = 0L;
	hulle_str = 0L;
	hulle_anz =  0;
	coreAnz = 0;
	srError = 0;
	rrError = 0;
	semActCnt = 0;
	incl = 0L;
	ext = 0L;
	termArr = 0L;
	ntermArr = 0L;
	data = 0L;
	union_str = 0L;
	lrTab = 0L;
	jumpTab = 0L;
	hash_tab = 0L;
	s = 0L;

	//Argumente
	argv_str = argv;

	//Grammatik-Datei einlesen
	if( readFile( name ) < 0 )
		//Fehler
		return;

	//Speicher für die Hash-Tabelle anfordern
	hash_tab = new( hashClass );

	//Speicher für die %union-Angabe
	union_str = new( UNION_STR );
	memset( union_str, 0, sizeof( UNION_STR ) );

	//Scanner initialisieren
	s = new scanner( data, &att );

	//Grammatik-File analysieren
	if( analyze() < 0 )
		//Fehler
		return;
}


lrparser::~lrparser()
{
	//Speicher freigeben
	ERROR_STR	*errStr;
	PROD2_STR	*p2;
	HULLE_STR	*h;
	int			i;

	//Hash-Tabelle
	if( hash_tab )
		delete( hash_tab );

	//Scanner freigeben
	if( s )
		delete( s );

	//Grammatik-File
	if( data )
		delete[] ( data );

	//Include-Anweisungen im cpp-File
	if( incl )
		delete[] ( incl );

	//eigene Erweiterungen im cpp-FIle
	if( ext )
		delete[] ( ext );

	//Speicher für die Hüllen freigeben
	if( hulle_str )
	{
		//Hülle freigeben
		for( i = 0; i < hulle_anz; i++ )
		{
			//Zeiger auf Hülle
			h = hulle_str[i];

			//Hülle löschen
			deleteHulle( h );
		}

		delete[] ( hulle_str );
	}

	//union-Struktur
	if( union_str )
	{
		//Alle Einträge löschen
		for( i = 0; i < union_str->anz; i++ )
		{
			//Speicher freigeben
			delete[] ( union_str->type[i] );
			delete[] ( union_str->id[i] );
		}

		//Speicher freigeben
		delete[] ( union_str->id );
		delete[] ( union_str->type );
		delete[] ( union_str->text );
		delete( union_str );
	}

	//LR-Tabelle
	if( lrTab )
	{
		//Alle Zustände
		for( i = 0; i < hulle_anz; i++ )
			//Speicher freigeben
			delete[] ( lrTab[i] );

		delete[] ( lrTab );
	}

	//Sprung-Tabelle
	if( jumpTab )
	{
		//Alle Zustände
		for( i = 0; i < hulle_anz; i++ )
			//Speicher freigeben
			delete[] ( jumpTab[i] );

		delete[] ( jumpTab );
	}

	//Fehler-Strukturen
	if( error_str )
	{
		for( i = 0; i < error_anz; i++ )
		{
			//Zeiger auf ERROR_STR
			errStr = &error_str[i];

			//Fehlermeldung?
			if( errStr->msg )
				delete[] ( errStr->msg );

			//Terminal-gebundene Fehlermeldungen?
			if( errStr->errTermStr )
				delete[] ( errStr->errTermStr );
		}

		delete[] ( error_str );
	}

	//PROD_STR
	if( prod_str )
	{
		//PROD2_STR
		for( i = 0; i < prod2_anz; i++ )
		{
			//Zeiger auf PROD2_STR
			p2 = &prod2_str[i];

			//semantische Aktionen?
			if( p2->semAct )
				delete[] ( p2->semAct );

			//Elemente auf der rechten Seite
			if( p2->right )
				delete[] ( p2->right );
		}

		delete[] ( prod_str );
	}

	//PROD2_STR
	if( prod2_str )
		delete[] ( prod2_str );

	//für Check der Terminale
	if( termArr )
		delete[] ( termArr );

	//für Check der Nicht-Terminale
	if( ntermArr )
		delete[] ( ntermArr );

	//für den Check
	if( check )
		delete[] ( check );
}


int lrparser::readFile( char *name )
{
	//Datei öffnen
	FILE	*file;

	//Grammatik-Datei öffnen
	file = fopen( name, "r" );

	//Informationen über die Datei einholen
	if( stat( name, &st ) )
	{
		//Fehler
		cout << endl << "\nsslr: Unable to read grammar file " << name << "!\n";
		return( -1 );
	}

	//Speicher für die Datei anfordern
	data = new( char[st.st_size + 1] );

	//Mit Null-Byte abschliessen
	data[st.st_size] = 0;

	//Datei einlesen
	fread( data, sizeof( char ), st.st_size, file );

	//Datei schliessen
	fclose( file );

	//Alles OK
	return( 0 );
}


int lrparser::analyze( void )
{
/*
	Zu den verschiedenen Status:
		0: es wird entweder %include, %definition oder %grammar erwartet
		1: %include erkannt, %definition oder %grammar erwartet
		2: %definition erkannt, erwartet %grammar
		3: %grammar erkannt, %extension möglich
*/

	//Hier wird die Grammatik-Datei analysiert
	SSLEXToken	token;
	HULLE_STR 	*h;
	ERROR_STR	*errorStr;
	PROD2_STR	*p2;
	PROD_STR	*p1;
	STACK		stack[100];
	long		line, hash_index;
	long		prod_index, prod2_index;
	char		*unionType, auxsym[11];
	char		*ebnf[] = { ")", "}", "]" }, *name;
	long		errorIndex, errorAct;
	bool		setNeg = false;
	int			state, oldState, ebnf_err = -1, err = 0;
	int			assoc, priority = 0, pPriority = 0, priLine = 0, unionDef = 0, unionCount = 0;
	int			semIndex = -1, altCount;
	int			errorNeg;
	int			auxsym_count = 1, sp = 0, ebnf_level = 0, pri;
	int			type, grammar = 0, i;


	//Status
	state = 0;

	//keine Assoziativität
	assoc = 0;

	//Falls kein semantischer Typ angegeben wurde
	insertUnion( "", "" );

	//keine semantische Aktion
	semAct = false;

	//Start-Produktion eintragen
	insertStartProd( prod_index, prod2_index );

	do
	{
		//Scanner aufrufen
		token = s->getToken();

		//Zeile
		line = s->getLine();

//Debug-Modus?
#ifdef SSLR_DEBUG
			cerr << "zeile: " << s->getLine() << "\n";
			cerr << "token: " << scanner_token[token] << "\n\n";

#endif

		//Token
		switch( token )
		{
			case AST:
			{
				//Zeiger in %union?
				if( state != 101 )
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": * unexpected!\n";

					//Fehler zählen
					err++;
				}

				//weiter
				break;
			}

			case EOPEN:
			{
				//EBNF-Symbol "optional"?
				if( state == 402 )
				{
					//Ja

					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja
						insertEmbeddedSemanticAction( prod_index, prod2_index, line, auxsym_count );

						//zurücksetzen
						semAct = false;
					}

					//alte Daten sichern
					stack[sp].prod_index = prod_index;
					stack[sp].prod2_index = prod2_index;
					stack[sp].altCount = altCount;
					stack[sp].pPriority = pPriority;
					stack[sp].ebnf_err = ebnf_err;
					stack[sp++].ebnf_level = ebnf_level++;

					//EBNF-Error setzen
					ebnf_err = 2;

					//auxsym erstellen
					sprintf( auxsym, "auxsym_%d", auxsym_count++ );


					//In Hash-Tabelle eintragen
					hash_index = hash_tab->insert( auxsym );

					//Produktion wurde definiert
					hash_tab->setProdDef( hash_index );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//Nichtterminal wird benutzt
					hash_tab->setUsed( hash_index );

					//Nichtterminal wird definiert
					hash_tab->setDefined( hash_index, line );

					//Ist ein Nichtterminal
					hash_tab->setType( hash_index, SSLR_NTERM );


					//auxsym-Produktion auf rechter Seite eintragen
					insertElement( prod_index, prod2_index, hash_index );

					//neue PROD_STR anfordern
					prod_index = getNewProd();

					//Priorität dieser Produktion initialisieren
					pPriority = 0;

					//prod_index dieser Produktion in die Hash-Tabelle eintragen
					hash_tab->setProdStr( hash_index, prod_index );

					//Hash-Index des Nichtterminals eintragen
					prod_str[prod_index].hash_index = hash_index;

					//PROD2_STR anfordern
					prod2_index = getNewProd2( prod_index );

					//den Index der ersten Alternative der Produktion merken
					prod_str[prod_index].prod2_index = prod2_index;

					//Anzahl der Alternativen
					altCount = 1;

					//analysieren
					state = 402;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": [ unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case ECLOSE:
			{
				//EBNF-Symbol "optional"?
				if( state == 402 )
				{
					//Ja

					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja, keine embedded semantic action

						//Zeiger auf PROD2_STR
						p2 = &prod2_str[prod2_index];

						//semantische Aktionen übertragen
						p2->semAct = semActStr.semAct;

						//Zeile merken
						p2->semActLine = semActStr.line;

						//ist keine embedded semantic action
						p2->isEmbeddedSemAct = false;

						//zurücksetzen
						semAct = false;
					}

					//Daten holen
					sp--;

					//Unterlauf?
					if( sp < 0 )
					{
						//Ja
						cerr << "sslr: ERROR: line " << line << ": Too many closing EBNF-Symbols!\n";

						//Ende
						return( -1 );
					}

					//darf es hier stehen?
					if( ebnf_err != 2 )
					{
						//Nein, Fehler
						cerr << "sslr: ERROR: line " << line << ": ] not allowed, " << ebnf[ebnf_err] << " expected!\n";

						//Fehler zählen
						err++;
					}

					//Priorität der Produktion eintragen
					prod2_str[prod2_index].priority = pPriority;

					//PROD2_STR anfordern
					prod2_index = getNewProd2( prod_index );

					//Priorität der Produktion eintragen
					prod2_str[prod2_index].priority = 0;

					//Terminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( "EPSILON" );

					//EPSILON auf rechter Seite eintragen
					insertElement( prod_index, prod2_index, hash_index );

					//Ist ein Terminal
					hash_tab->setType( hash_index, SSLR_TERM );

					//Anzahl der Alternativen
					altCount++;

					//Anzahl der Alternativen eintragen
					prod_str[prod_index].altAnz = altCount;

					//Alte Daten zurückholen
					prod_index = stack[sp].prod_index;
					prod2_index = stack[sp].prod2_index;
					altCount = stack[sp].altCount;
					pPriority = stack[sp].pPriority;
					ebnf_err = stack[sp].ebnf_err;
					ebnf_level = stack[sp].ebnf_level;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": } unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case STRING:
			{
				//%error-Anweisung
				if( ( state == 503 ) || ( state == 504 ) || ( state == 508 ) )
				{
					//Fehlermeldung
					errorStr->msg = new( char[strlen(att->name) + 1] );

					//kopieren
					strcpy( errorStr->msg, att->name );

					//Status
					state = 501;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": String \"" << att->name << "\" unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case KLCLOSE:
			{
				//%error-Angabe?
				if( state == 507 )
					//Ja
					state = 504;

				//Fehlerangabe?
				else if( ( state == 508 ) && setNeg )
					//negierte Terminale zu Ende
					setNeg = false;

				//EBNF-Symbol?
				else if( state == 402 )
				{
					//Ja

					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja, keine embedded semantic action

						//Zeiger auf PROD2_STR
						p2 = &prod2_str[prod2_index];

						//semantische Aktionen übertragen
						p2->semAct = semActStr.semAct;

						//Zeile merken
						p2->semActLine = semActStr.line;

						//ist keine embedded semantic action
						p2->isEmbeddedSemAct = false;

						//zurücksetzen
						semAct = false;
					}

					//Daten holen
					sp--;

					//Unterlauf?
					if( sp < 0 )
					{
						//Ja
						cerr << "sslr: ERROR: line " << line << ": Too many closing EBNF-Symbols!\n";

						//Ende
						return( -1 );
					}

					//darf es hier stehen?
					if( ebnf_err )
					{
						//Nein, Fehler
						cerr << "sslr: ERROR: line " << line << ": ) not allowed, " << ebnf[ebnf_err] << " expected!\n";

						//Fehler zählen
						err++;
					}

					//Anzahl der Alternativen eintragen
					prod_str[prod_index].altAnz = altCount;

					//Priorität der Produktion eintragen
					prod2_str[prod2_index].priority = pPriority;

					//Alte Daten zurückholen
					prod_index = stack[sp].prod_index;
					prod2_index = stack[sp].prod2_index;
					altCount = stack[sp].altCount;
					pPriority = stack[sp].pPriority;
					ebnf_err = stack[sp].ebnf_err;
					ebnf_level = stack[sp].ebnf_level;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": ) unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case KLOPEN:
			{
				//%error-Angabe?
				if( state == 505 )
					//Ja
					state = 506;

				//Fehleranweisung der Art !(TERMINAL,...
				else if( ( state == 503 ) && errorNeg )
					//es folgen Terminale, die alle als negiert aufzufassen sind
					setNeg = true;

				//EBNF-Symbol?
				else if( state == 402 )
				{
					//Ja

					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja
						insertEmbeddedSemanticAction( prod_index, prod2_index, line, auxsym_count );

						//zurücksetzen
						semAct = false;
					}

					//Alte Daten sichern
					stack[sp].prod_index = prod_index;
					stack[sp].prod2_index = prod2_index;
					stack[sp].altCount = altCount;
					stack[sp].pPriority = pPriority;
					stack[sp].ebnf_err = ebnf_err;
					stack[sp++].ebnf_level = ebnf_level++;

					//nächster Level
					ebnf_level++;

					//EBNF-Error setzen
					ebnf_err = 0;

					//auxsym erstellen
					sprintf( auxsym, "auxsym_%d", auxsym_count++ );

					//In Hash-Tabelle eintragen
					hash_index = hash_tab->insert( auxsym );

					//Produktion wurde definiert
					hash_tab->setProdDef( hash_index );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//Nichtterminal wird benutzt
					hash_tab->setUsed( hash_index );

					//Nichtterminal wird definiert
					hash_tab->setDefined( hash_index, line );

					//Ist ein Nichtterminal
					hash_tab->setType( hash_index, SSLR_NTERM );

					//auxsym-Produktion auf rechter Seite eintragen
					insertElement( prod_index, prod2_index, hash_index );

					//neue PROD_STR anfordern
					prod_index = getNewProd();

					//Priorität dieser Produktion initialisieren
					pPriority = 0;

					//prod_index dieser Produktion in die Hash-Tabelle eintragen
					hash_tab->setProdStr( hash_index, prod_index );

					//Hash-Index des Nichtterminals eintragen
					prod_str[prod_index].hash_index = hash_index;

					//PROD2_STR anfordern
					prod2_index = getNewProd2( prod_index );

					//den Index der ersten Alternative der Produktion merken
					prod_str[prod_index].prod2_index = prod2_index;

					//Anzahl der Alternativen
					altCount = 1;

					//analysieren
					state = 402;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": ( unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case NUMBER:
			{
				//%error-Modus?
				if( state == 501 )
				{
					//Aktion erwartet
					state = 502;

					//Eine ERROR_STR anfordern
					errorIndex = getErrorStr();

					//Zeiger auf die ERROR_STR
					errorStr = &error_str[errorIndex];

					//Position des Punktes
					errorStr->dot = att->l;

					//Index der Produktion eintragen
					errorStr->prod_index = prod_index;

					//Index der Alternative in PROD2_STR eintragen
					errorStr->prod2_index = prod2_index;

					//Die Zeile merken
					errorStr->line = line;

					//standardmäßig sind die folgenden Terminale nicht negiert
					errorNeg = 0;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": Unexpected number " << att->l << "!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case ERROR:
			{
				//%error-Angabe
				if( state == 402 )
					//OK, Fehleranweisung parsen
					state = 500;

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": %error unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case PREC:
			{
				//Priorität setzen erlaubt?
				if( state == 402 )
					//Ja
					state = 403;

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": %prec unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case PROD_END:
			{
				//Neue Produktion
				if( state == 402 )
				{
					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja, keine embedded semantic action

						//Zeiger auf PROD2_STR
						p2 = &prod2_str[prod2_index];

						//semantische Aktionen übertragen
						p2->semAct = semActStr.semAct;

						//Zeile merken
						p2->semActLine = semActStr.line;

						//ist keine embedded semantic action
						p2->isEmbeddedSemAct = false;

						//zurücksetzen
						semAct = false;
					}

					//Anzahl der Alternativen eintragen
					prod_str[prod_index].altAnz = altCount;

					//Priorität der Produktion eintragen
					prod2_str[prod2_index].priority = pPriority;

					//Wurden Elemente auf der rechten Seite eingetragen?
					if( !prod2_str[prod2_index].riAnz )
					{
						//Fehlermeldung
						cerr << "sslr: ERROR: line " << line - 1 << ": No elements on right side of alternative!\n";

						//Fehler
						err++;
					}

					//Status zurücksetzen
					state = 400;
				}

				//EBNF-Fehler?
				if( ebnf_level )
				{
					//Ja, fehlenden schließendes Symbol
					cerr << "sslr: ERROR: line " << line << ": Missing " << ebnf[ebnf_err] << "!\n";

					//Parsing zu Ende
					return( -1 );
				}

				break;
			}

			case PROD_NEXT:
			{
				//semantische Aktion vorher eintragen?
				if( semAct )
				{
					//Ja, keine embedded semantic action

					//Zeiger auf PROD2_STR
					p2 = &prod2_str[prod2_index];

					//semantische Aktionen übertragen
					p2->semAct = semActStr.semAct;

					//Zeile merken
					p2->semActLine = semActStr.line;

					//ist keine embedded semantic action
					p2->isEmbeddedSemAct = false;

					//zurücksetzen
					semAct = false;
				}

				//nächste Produktion
				altCount++;

				//Priorität der Produktion eintragen
				prod2_str[prod2_index].priority = pPriority;

				//Wurden Elemente auf der rechten Seite eingetragen?
				if( !prod2_str[prod2_index].riAnz )
				{
					//Fehlermeldung
					cerr << "sslr: ERROR: line " << line - 1 << ": No elements on right side of alternative!\n";

					//Fehler
					err++;
				}

				//PROD2_STR anfordern
				prod2_index = getNewProd2( prod_index );

				//die Alternative hat noch keine Priorität
				pPriority = 0;

				break;
			}

			case TERMINAL:
			{
				//Terminal

				//Ist es erlaubt?
				if( !strcmp( att->name, "LALR" ) )
				{
					//Nein, reserviert
					cerr << "sslr: ERROR: line " << line << ": Terminal LALR reserved!\n";

					//Fehler
					err++;
				}

				//Terminal in Hash-Tabelle eintragen
				hash_index = hash_tab->insert( att->name );

				//an dieser Stelle erlaubt?
				if( state == 402 )
				{
					//ist es ein EPSILON, dass nicht als erstes Element auf der rechten Seite steht?
					if( ( prod2_str[prod2_index].riAnz ) && ( hash_index == epsHashIndex ) )
					{
						//Fehler
						cerr << "sslr: ERROR: line " << line << ": EPSILON must always be the only element on the right side!\n";

						//Fehler
						err++;
					}

					else
					{
						//semantische Aktion vorher eintragen?
						if( semAct )
						{
							//Ja
							insertEmbeddedSemanticAction( prod_index, prod2_index, line, auxsym_count );

							//zurücksetzen
							semAct = false;
						}

						//für Fehlerbehandlung merken
						setCheck( hash_index );

						//Terminal benutzt
						hash_tab->setUsed( hash_index );

						//Terminal in rechte Seite eintragen
						insertElement( prod_index, prod2_index, hash_index );

						//Ist ein Terminal
						type = hash_tab->setType( hash_index, SSLR_TERM );

						//Priorität des Terminals holen
						pPriority = hash_tab->getPriority( hash_index );

						//Anders?
						if( type != SSLR_TERM )
						{
							//Fehler, Nichtterminal wurde schon als Terminal definiert

							//Name des Nichtterminals
							name = hash_tab->getName( hash_index );

							//Fehlermedlung
							cerr << "sslr: ERROR: line " << line << ": Terminal " << name << " already defined as nonterminal in line " << hash_tab->getDefLine( hash_index ) << "!\n";

							//Fehler
							err++;
						}

						//Zeiger auf PROD_STR
						p1 = &prod_str[prod_index];

						//Priorität übernehmen
						prod2_str[prod2_index].priority = hash_tab->getPriority( hash_index );
					}
				}

				else if( state == 401 )
				{
					//Es wird ein Seperator -> erwartet
					cerr << "sslr: ERROR: line " << line << ": Missing seperator before terminal " << att->name << "!\n";

					//Fehler
					err++;
				}

				else
				{
					//Nein, Fehler
					cerr << "sslr: ERROR: line " << line << ": Terminal unexpected!\n";

					//Fehler
					err++;
				}

				break;
			}

			case SEPERATOR:
			{
				//an der richtigen Stelle?
				if( state == 401 )
					//Ja
					state = 402;

				//Produktion nicht korrekt mit ; abgeschlossen?
				else if( state == 402 )
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": Production " << hash_tab->getName( prod_str[prod_index].hash_index ) << " doesn't end with ;!\n";

					//Fehler
					err++;
				}

				//%error
				else if( ( state == 502 ) || ( state == 508 ) )
					//Seperator
					state = 503;

				else if( state == 504 )
					//%error-Analyse: Terminal erwartet
					state = 503;

				else
					//Fehler
					cerr << "sslr: Warning: line " << line << ": ( unexpected!\n";

				break;
			}

			case GRAMMAR:
			{
				//Grammatik analysieren
				state = 400;

				//Schon erkannt?
				if( grammar )
				{
					//Ja
					cerr << "sslr: ERROR: line " << line << ": %grammar already defined!\n";

					//Fehler
					err++;
				}

				//Grammatik erkannt
				grammar++;

				break;
			}

			case SEM_TYPE:
			{
				//Typ für semantische Aktion, sollte in %union definiert sein.

				//%union schon angegeben?
				if( !unionDef )
				{
					//Nein
					cerr << "sslr: ERROR: line " << line << ": %union missing!\n";

					//Fehler
					err++;
				}

				else
				{
					//Den Index in union_str finden
					if( ( semIndex = getSemIndex( att->name ) ) < 0 )
					{
						//Typ nicht in %union definiert
						cerr << "sslr: ERROR: line " << line << ": Type for semantic action " << att->name << " not defined in %union!\n";

						//Fehler
						err++;
					}
				}

				break;
			}

			case TERM_TYPE:
			{
				//Terminale werden definiert
				state = 200;

				break;
			}

			case NTERM_TYPE:
			{
				//Terminale werden definiert
				state = 300;

				break;
			}

			case UNION:
			{
				//%union schon definiert?
				if( unionDef )
				{
					//schon definiert
					cerr << "sslr: ERROR: line " << line << ": %union already definined!\n";

					//Fehler zählen
					err++;
				}

				else
				{
					//Nein, noch nicht
					unionDef++;

					//Status: Erkennen von %union
					oldState = state;
					state = 100;
				}

				break;
			}

			case BOPEN:
			{
				//%union erkennen?
				if( !state )
				{
					//EBNF-Symbol
				}

				else if( state == 500 )
					//OK
					state = 501;

				//%union-Angabe beginnen?
				else if( state == 100 )
					//OK
					state = 101;

				//EBNF-Symbol?
				else if( state == 402 )
				{
					//Ja

					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja
						insertEmbeddedSemanticAction( prod_index, prod2_index, line, auxsym_count );

						//zurücksetzen
						semAct = false;
					}

					//Alte Daten sichern
					stack[sp].prod_index = prod_index;
					stack[sp].prod2_index = prod2_index;
					stack[sp].altCount = altCount;
					stack[sp].pPriority = pPriority;
					stack[sp].ebnf_err = ebnf_err;
					stack[sp++].ebnf_level = ebnf_level++;

					//nächster Level
					ebnf_level++;

					//EBNF-Error setzen
					ebnf_err = 1;

					//auxsym erstellen
					sprintf( auxsym, "auxsym_%d", auxsym_count++ );


					//In Hash-Tabelle eintragen
					hash_index = hash_tab->insert( auxsym );

					//Produktion wurde definiert
					hash_tab->setProdDef( hash_index );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//Nichtterminal wird benutzt
					hash_tab->setUsed( hash_index );

					//Nichtterminal wird definiert
					hash_tab->setDefined( hash_index, line );

					//Ist ein Nichtterminal
					hash_tab->setType( hash_index, SSLR_NTERM );


					//auxsym-Produktion auf rechter Seite eintragen
					insertElement( prod_index, prod2_index, hash_index );

					//neue PROD_STR anfordern
					prod_index = getNewProd();

					//Priorität dieser Produktion initialisieren
					pPriority = 0;

					//prod_index dieser Produktion in die Hash-Tabelle eintragen
					hash_tab->setProdStr( hash_index, prod_index );

					//Hash-Index des Nichtterminals eintragen
					prod_str[prod_index].hash_index = hash_index;

					//PROD2_STR anfordern
					prod2_index = getNewProd2( prod_index );

					//den Index der ersten Alternative der Produktion merken
					prod_str[prod_index].prod2_index = prod2_index;

					//Anzahl der Alternativen
					altCount = 1;

					//analysieren
					state = 402;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": { unexpected!\n";

					//Fehler
					err++;
				}

				break;
			}

			case BCLOSE:
			{
				long	baseProd2Index, count;

				//%union-Erkennung abgeschlossen?
				if( state == 100 )
					//Ja
					state = 0;

				//%error-Angabe zu Ende
				else if( state == 506 )
					//OK
					state = 400;

				//%union-Angabe zu Ende?
				else if( state == 101 )
				{
					//OK
					state = oldState;

					//in att->name steht der union-Text
					union_str->text = att->name;
				}

				//%error-Anweisung zu Ende?
				else if( state == 501 )
					//OK
					state = 402;

				//EBNF-Symbol?
				else if( state == 402 )
				{
					//Ja

					//semantische Aktion vorher eintragen?
					if( semAct )
					{
						//Ja, keine embedded semantic action

						//Zeiger auf PROD2_STR
						p2 = &prod2_str[prod2_index];

						//semantische Aktionen übertragen
						p2->semAct = semActStr.semAct;

						//Zeile merken
						p2->semActLine = semActStr.line;

						//ist keine embedded semantic action
						p2->isEmbeddedSemAct = false;

						//zurücksetzen
						semAct = false;
					}

					//Daten holen
					sp--;

					//Unterlauf?
					if( sp < 0 )
					{
						//Ja
						cerr << "sslr: ERROR: line " << line << ": Too many closing EBNF-Symbols!\n";

						//Ende
						return( -1 );
					}

					//darf es hier stehen?
					if( ebnf_err != 1 )
					{
						//Nein, Fehler
						cerr << "sslr: ERROR: line " << line << ": } not allowed, " << ebnf[ebnf_err] << " expected!\n";

						//Fehler
						err++;
					}

					//Priorität der Alternative eintragen
					prod2_str[prod2_index].priority = pPriority;


					//prod2_index der ersten Alternative der Produktion
					baseProd2Index = prod_str[prod_index].prod2_index;

					//nun das auxsym selbst als letztes Element auf rechter Seite eintragen,
					//damit beliebig Ausführen möglich ist
					for( count = baseProd2Index; count < ( baseProd2Index + altCount ); count ++ )
						//auxsym auf rechter Seite eintragen
						insertElement( prod_index, count, prod_str[prod_index].hash_index );


					//PROD2_STR anfordern
					prod2_index = getNewProd2( prod_index );

					//Priorität der Produktion eintragen
					prod2_str[prod2_index].priority = 0;

					//Terminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( "EPSILON" );

					//EPSILON in rechte Seite eintragen
					insertElement( prod_index, prod2_index, hash_index );


					//Ist ein Terminal
					hash_tab->setType( hash_index, SSLR_TERM );

					//Anzahl der Alternativen
					altCount++;

					//Anzahl der Alternativen eintragen
					prod_str[prod_index].altAnz = altCount;

					//Alte Daten zurückholen
					prod_index = stack[sp].prod_index;
					prod2_index = stack[sp].prod2_index;
					altCount = stack[sp].altCount;
					pPriority = stack[sp].pPriority;
					ebnf_err = stack[sp].ebnf_err;
					ebnf_level = stack[sp].ebnf_level;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": } unexpected!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case COMMA:
			{
				//ein Komma zum Trennen der Terminale in den Fehlerangaben
				if( state == 508 )
					//OK
					state = 503;

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": Unexpected comma!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case EXCLAMATION:
			{
				//Ausrufezeichen, dient der Negation von Terminalen bei den Fehlerangaben

				//erlaubt?
				if( ( state == 503 ) || ( state == 508 ) )
				{
					//Ja
					errorNeg++;

					//neuer Status
					state = 503;
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": Unexpected exlamation mark!\n";

					//Fehler zählen
					err++;
				}

				break;
			}

			case IDENT:
			{
				//Terminal/Nichtterminal

				//Terminal in %definition definiert?
				if( state == 2 )
				{
					//Terminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( att->name );

					//Terminal wird definiert
//					hash_tab->setDefined( hash_index, line );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//Ist ein Terminal
					hash_tab->setType( hash_index, SSLR_TERM );

					//Assoziativität eintragen
					hash_tab->setAssociation( hash_index, assoc );

					//Priorität erhöhen
					if( line > priLine )
					{
						//Ja, erhöhen
						priority++;
						priLine = line;
					}

					//Priorität eintragen
					hash_tab->setPriority( hash_index, priority );
				}

				else if( state == 100 )
				{
					//Fehler, nach %union muss ein { folgen!
					cerr << "sslr: ERROR: line " << line << ": { must follow %union!\n";

					//Fehler zählen
					err++;

					//Zustand setzen
					state = 101;

					//ist hoffentlich ein Typ
					unionType = new( char[strlen( att->name ) + 1] );
					strcpy( unionType, att->name );

					//Zeigt an, dass der Typ erfolgreich gescannt wurde
					unionCount++;
				}

				else if( state == 101 )
				{
					//%union-Paar
					unionCount++;

					//eintragen?
					if( unionCount == 2 )
					{
						//Ja
						insertUnion( unionType, att->name );

						//Speicher wieder freigeben
						delete[] ( unionType );

						//Zähler zurücksetzen
						unionCount = 0;
					}

					else
					{
						//Typ merken
						unionType = new( char[strlen( att->name ) + 1] );
						strcpy( unionType, att->name );
					}
				}

				else if( state == 200 )
				{
					//Terminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( att->name );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//%term_type: Terminal wird definiert
					hash_tab->setDefined( hash_index, line );

					//Index in union_str eintragen
					hash_tab->setUnionType( hash_index, semIndex );

					//Ist ein Terminal
					type = hash_tab->setType( hash_index, SSLR_TERM );

					//Anders?
					if( type != SSLR_TERM )
					{
						//Fehler, Nichtterminal wurde schon als Terminal definiert

						//Name des Nichtterminals
						name = hash_tab->getName( hash_index );

						//Fehlermedlung
						cerr << "sslr: ERROR: line " << line << ": Terminal " << name << " already defined as nonterminal in line " << hash_tab->getDefLine( hash_index ) << "!\n";

						//Fehler
						err++;
					}
				}

				else if( state == 300 )
				{
					//Nichtterminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( att->name );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//%nterm_type: Nichtterminal wird definiert
					hash_tab->setDefined( hash_index, line );

					//Index in union_str eintragen
					hash_tab->setUnionType( hash_index, semIndex );

					//Ist ein Nichtterminal
					type = hash_tab->setType( hash_index, SSLR_NTERM );

					//Anders?
					if( type != SSLR_NTERM )
					{
						//Fehler, Nichtterminal wurde schon als Terminal definiert

						//Name des Nichtterminals
						name = hash_tab->getName( hash_index );

						//Fehlermedlung
						cerr << "sslr: ERROR: line " << line << ": Nonterminal " << name << " already defined as terminal in line " << hash_tab->getDefLine( hash_index ) << "!\n";

						//Fehler
						err++;
					}
				}

				else if( state == 503 )
				{
					//%error-Anweisung, es folgt eine Aktion
					if( !strcmp( att->name, "SHIFT" ) )
					{
						//ein Terminal einschieben
						errorAct = ESHIFT;

						//als nächstes Terminal erwartet
						state = 505;
					}

					else if( !strcmp( att->name, "DELETE" ) )
					{
						//ein Terminal löschen
						errorAct = EDELETE;

						//als nächstes Terminal erwartet
						state = 504;
					}

					else if( !strcmp( att->name, "OVERREAD" ) )
					{
						//Terminale überlesen
						errorAct = EOVERREAD;

						//als nächstes Terminal erwartet
						state = 505;
					}

					else if( !strcmp( att->name, "EXIT" ) )
					{
						//Parsing abbrechen
						errorAct = EEXIT;

						//als nächstes Terminal erwartet
						state = 504;
					}

					else if( !strcmp( att->name, "SEM" ) )
					{
						//semantische Aktionen trotz Fehler erlauben
						errorAct = ESEM;

						//als nächstes Terminal erwartet
						state = 504;
					}

					else
					{
						//ein Terminal

						//Index des Terminals holen
						hash_index = hash_tab->insert( att->name );

						//für Fehlerbehandlung merken
						setCheck( hash_index );

						//Negieren?
						if( ( setNeg == true ) || errorNeg )
							//ERRTERM_STR in errorStr eintragen
							insertErrTermStr ( errorStr, hash_index, 1 );

						else
							//nicht negieren
							insertErrTermStr ( errorStr, hash_index, 0 );

						//nicht negieren
						errorNeg = 0;

						//TERM1, TERM2 : Fehleraktion erwartet, oder Fehlermeldung
						state = 508;
					}

					//sofort eintragen?
					if( state == 504 )
					{
						//in die ERROR_STR eintragen
						switch( errorAct )
						{
							case EDELETE:
							{
								//das nächste Terminal löschen, mehrfaches Auftreten von EDELETE
								//ist erlaubt
								errorStr->del++;

								break;
							}

							case EEXIT:
							{
								//das Parsen abbrechen
								if( errorStr->exit )
									//schon eingetragen
									cerr << "sslr: Warning: line " << line << ": EXIT already used!\n";

								else
									//eintragen
									errorStr->exit++;

								break;
							}

							case ESEM:
							{
								//semantische Aktionen trotz Fehlers weiterführen
								if( errorStr->sem )
									//schon eingetragen
									cerr << "sslr: Warning: line " << line << ": SEM already used!\n";

								else
									//eintragen
									errorStr->sem++;

								break;
							}

							default:
							{
								//Fehler
								cerr << "sslr: ERROR: line " << line << ": unknown error-action!\n";

								//Fehler
								err++;
							}
						}
					}
				}

				else if( state == 506 )
				{
					//%error-Anweisung, es folgt ein Terminal

					//Terminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( att->name );

					//Shift-Aktion?
					if( errorAct == ESHIFT )
					{
						//schon in der ERROR_STR enthalten?
						if( errorStr->shift )
							//Es war schon eine SHIFT(TERMINAL)-Aktion eingetragen worden
							cerr << "sslr: Warning: line: " << line << ": Double used shift(terminal)-error-actions make no sense!\n";

						else
							//eintragen
							errorStr->shift = hash_index;
					}

					else if( errorAct == EOVERREAD )
					{
						//schon in der ERROR_STR enthalten?
						if( errorStr->overread )
							//Es war schon eine OVERREAD(TERMINAL)-Aktion eingetragen worden
							cerr << "sslr: Warning: line: " << line << ": Double used overread(terminal)-error-actions make no sense!\n";

						else
							//eintragen
							errorStr->overread = hash_index;
					}

					//weiter
					state = 507;
				}

				else
				{
					//(Nicht)Terminal in Hash-Tabelle eintragen
					hash_index = hash_tab->insert( att->name );

					//für Fehlerbehandlung merken
					setCheck( hash_index );

					//Beginn der Analyse einer Produktion?
					if( state == 400 )
					{
						//Produktion wurde definiert
						hash_tab->setProdDef( hash_index );

						//Nichtterminal auf rechter Seite der Start-Produktion eintragen?
						if( !prod_index )
						{
							//Ja, eintragen
							insertElement( prod_index, prod2_index, hash_index );

							//Eine Alternative
							prod_str[prod_index].altAnz = 1;

							//Produktion wurde benutzt
							hash_tab->setUsed( hash_index );
						}

						//Start-Produktion "start" nicht erlaubt
						if( !strcmp( hash_tab->getName( hash_index ), "start" ) )
						{
							//Fehler
							err++;

							//Meldung
							cerr << "sslr: ERROR: line " << line << ": Production start reserved!\n";
						}

						//Ja, neue PROD_STR anfordern
						prod_index = getNewProd();

						//prod_index dieser Produktion in die Hash-Tabelle eintragen
						hash_tab->setProdStr( hash_index, prod_index );

						//Hash-Index des Nichtterminals eintragen
						prod_str[prod_index].hash_index = hash_index;

						//Ist ein Nichtterminal
						type = hash_tab->setType( hash_index, SSLR_NTERM );

						//Anders?
						if( type != SSLR_NTERM )
						{
							//Fehler, Nichtterminal wurde schon als Terminal definiert

							//Name des Nichtterminals
							name = hash_tab->getName( hash_index );

							//Fehlermedlung
							cerr << "sslr: ERROR: line " << line << ": Nonterminal " << name << " already defined as terminal in line " << hash_tab->getDefLine( hash_index ) << "!\n";

							//Fehler
							err++;
						}

						//PROD2_STR anfordern
						prod2_index = getNewProd2( prod_index );

						//den Index der ersten Alternative der Produktion merken
						prod_str[prod_index].prod2_index = prod2_index;

						//Anzahl der Alternativen
						altCount = 1;

						//Trenner -> erwartet
						state = 401;

						//Meldung ausgeben
						if( argv_str->verbose )
							cout << "sslr: line " << line << ": Analyze " << att->name << "...\n";
					}

					else if( state == 401 )
					{
						//Es wird ein Seperator -> erwartet
						cerr << "sslr: ERROR: line " << line << ": Missing Seperator before Nonterminal " << att->name << "!\n";

						//Fehler
						err++;
					}

					//Rechte Seite anylysieren?
					else if( state == 402 )
					{
						//semantische Aktion vorher eintragen?
						if( semAct )
						{
							//Ja
							insertEmbeddedSemanticAction( prod_index, prod2_index, line, auxsym_count );

							//zurücksetzen
							semAct = false;
						}

						//Ja, Nichtterminal eintragen
						insertElement( prod_index, prod2_index, hash_index );

						//Nichtterminal benutzt
						hash_tab->setUsed( hash_index );

						//ist ein Nichtterminal
						type = hash_tab->setType( hash_index, SSLR_NTERM );

						//anders?
						if( type != SSLR_NTERM )
						{
							//Fehler, Nichtterminal wurde schon als Terminal definiert

							//Name des Nichtterminals
							name = hash_tab->getName( hash_index );

							//Fehlermedlung
							cerr << "sslr: ERROR: line " << line << ": Nonterminal " << name << " already defined as terminal in line " << hash_tab->getDefLine( hash_index ) << "!\n";

							//Fehler
							err++;
						}
					}

					//Priorität der Produktion ändern?
					else if( state == 403 )
					{
						//Ja, die Prioriät des Terminals = Priorität der Alternative

						//Zeiger auf PROD_STR
						p1 = &prod_str[prod_index];

						//Hash-Index des Terminals holen
						hash_index = hash_tab->insert( att->name );

						//Terminal wird benutzt
						hash_tab->setUsed( hash_index );

						//Priorität der Produktion
						pri = prod2_str[prod2_index].priority;

						//Priorität der Alternative > Priorität des Terminals?
						if( pri <=  hash_tab->getPriority( hash_index ) )
						{
							//Nein, übernehmen
							pPriority = hash_tab->getPriority( hash_index );

							//Priorität in Alternative eintragen
							prod2_str[prod2_index].priority = pPriority;
						}

						else
							//Meldung
							cerr << "sslr: Warning: line " << line << ": Priority of " << hash_tab->getName( p1->hash_index ) << " > than priority of " << hash_tab->getName( hash_index ) << "!\n";

						//Status = rechte Seite analysieren
						state = 402;
					}
				}

				break;
			}

			case NONASSOC:
			{
				//Terminale sind nicht-assoziativ
				if( state == 2 )
					//setzen
					assoc = isNonAssoc;

				else
				{
					//darf hier nicht stehen
					cerr << "sslr: ERROR: line " << line << ": %nonassoc must stand in %definition!\n";

					//Fehler
					err++;
				}

				break;
			}

			case RIGHT:
			{
				//Terminale sind rechts-assoziativ
				if( state == 2 )
					//setzen
					assoc = isRightAssoc;

				else
				{
					//darf hier nicht stehen
					cerr << "sslr: ERROR: line " << line << ": %right must stand in %definition!\n";

					//Fehler
					err++;
				}

				break;
			}

			case LEFT:
			{
				//Terminale sind links-assoziativ
				if( state == 2 )
					//setzen
					assoc = isLeftAssoc;

				else
				{
					//darf hier nicht stehen
					cerr << "sslr: ERROR: line " << line << ": %left must stand in %definition!\n";

					//Fehler
					err++;
				}

				break;
			}

			case DEF:
			{
				//Definitions-Bereich
				/*
					Hier folgen nun Assoziationen der Terminals, Prioritäten, etc.
					%left, %right, %term_type, %nterm_type
				*/

				//An dieser Stelle erlaubt?
				if( ( !state ) || ( state == 1 ) )
					//OK, Status setzen
					state = 2;

				else
				{
					//Darf hier nicht stehen
					cerr << "sslr: ERROR: line " << line << ": %definition not allowed at this place!\n";

					//Fehler
					err++;
				}

				break;
			}

			case INCL:
			{
				//Include-Anweisung vom Scanner an dieser Stelle erlaubt?
				if( !state )
				{
					//Die Include-Anweisung kommt immer als erstes
					incl = new( char[strlen(att->name)+1] );

					//Include-Angaben kopieren
					strcpy( incl, att->name );

					//Status setzen
					state++;
				}

				else
				{
					//Darf hier nicht stehen
					cerr << "sslr: ERROR: line " << line << ": %include must be first!\n";

					//Fehler
					err++;
				}

				break;
			}

			case EXTENSION:
			{
				//Eigene Erweiterungen?
				if( state == 400 )
				{
					//Die Include-Anweisung kommt immer als erstes
					ext = new( char[strlen(att->name)+1] );

					//Include-Angaben kopieren
					strcpy( ext, att->name );

					//Status setzen
					state++;
				}

				else
				{
					//Darf hier nicht stehen
					cerr << "sslr: ERROR: line " << line << ": %extension must be the latest!\n";

					//Fehler
					err++;
				}

				break;
			}

			case END:
			{
				//Eingabe beendet
				break;
			}

			case SEM_ACT:
			{
				//eine semantische Aktion, in PROD2_STR anhängen
				if( state == 402 )
				{
					//%union schon angegeben?
					if( !unionDef )
					{
						//Nein
						cerr << "sslr: ERROR: line " << line << ": Semantic action useless without %union!\n";

						//Fehler
						err++;
					}

					else
					{
						//Schon eine semantische Aktion in Struktur, d.h. doppeltes Auftreten?
						if( semAct )
						{
							//doppelte semantische Aktion
							cerr << "sslr: ERROR: line " << line << ": Double Semantic action!\n";

							//Fehler
							err++;
						}

						else
						{
							//Speicher anfordern
							semActStr.semAct = new( char[strlen(att->name)+1] );

							//semantische Aktionen übertragen
							strcpy( semActStr.semAct, att->name );

							//Zeile merken
							semActStr.line = line;

							//semantische Aktion eingetragen
							semAct = true;

							//Zähler
							semActCnt++;
						}
					}
				}

				else
				{
					//Fehler
					cerr << "sslr: ERROR: line " << line << ": Semantic action not allowed!\n";

					//Fehler
					err++;
				}

				break;
			}

			default:
			{
				//Fehler
				cerr << "sslr: Warning: line " << line << ": Undefined token!\n";

				//weiter
				break;
			}
		}

	}while( token != END );

	//Meldung?
	if( argv_str->verbose )
		//Ja, start-Produktion nicht mitzählen
		cout << "sslr: " << prod_anz - 1 << " productions analyzed!\n";

	//(Nicht)Terminale checken
	err += checkAll();

	//Die PROD2_STR sortieren
	sortProd2Str();

	//finde doppelte Alternativen innerhalb einer Produktion
	err += findDoubleAlt();

	//Alles OK?
	if( !err )
	{
		//Ja, die ERROR_STR prüfen
		checkErrorStr();

		//Grammatik ausgeben
		outputGrammar();

		//FIRST-Mengen berechnen
		genAllFirst();

		//FOLLOW-Mengen berechnen
		genAllFollow();

		//Welchen Parser erzeugen?
		if( argv_str->type == SLR )
			//SLR-Parser erzeugen
			genLR0Hulle();

		else if( argv_str->type == LALR )
		{
#ifndef LR2LALR
			//LALR-Parser erzeugen
			genLR0Hulle();

			//Die Lookaheads erzeugen
			genLALR();
#else
			/*
				Es gibt zu Debuggingzwecken auch die Möglichkeit, erst die LR(1)-Items zu erzeugen,
				und dann diese zu den LALR-Items zusammenzufassen.
			*/

			//LR(1)-Hüllen erzeugen
			genLR1Hulle();

			//LR(1)-Hüllen in LR(0)-Hüllen mit Lookaheads
			LR2LALR();
#endif
		}

		else if( argv_str->type == LR )
			//LR-Parser erzeugen
			genLR1Hulle();

		//LR-Tabelle erzeugen
		LRTab();

		//Hüllen ausgeben?
		if( argv_str->showHulle )
		{
			//Ja, Hüllen ausgeben
			for( i = 0; i < hulle_anz; i++ )
			{
				//Zeiger auf Hülle
				h = hulle_str[i];

				//Ausgeben
				outputHulle( h );
			}
		}

		//Quell-Files des Parsers erzeugen
		generateFiles();
	}

	else
		//Abbruch-Meldung ausgeben
		cerr << "\n\nsslr: Aborting...\n";

	//prüfen, ob mit allen Alternativen reduziert wurde
	checkAllReduce();

	//Das war's
	return( err );
}


void lrparser::insertEmbeddedSemanticAction( long prod_index, long prod2_index, long line, int &auxsymCount )
{
	//fügt ein auxsym-Nichtterminal auf rechter Seite von prod2_index ein für eine embedded semantic
	//action.
	PROD2_STR	*p2;
	long		hash_index;
	char		auxsym[11];

	//auxsym erstellen
	sprintf( auxsym, "auxsym_%d", auxsymCount++ );

	//in Hash-Tabelle eintragen
	hash_index = hash_tab->insert( auxsym );

	//Produktion wurde definiert
	hash_tab->setProdDef( hash_index );

	//für Fehlerbehandlung merken
	setCheck( hash_index );

	//Nichtterminal wird benutzt
	hash_tab->setUsed( hash_index );

	//Nichtterminal wird definiert
	hash_tab->setDefined( hash_index, line );

	//Ist ein Nichtterminal
	hash_tab->setType( hash_index, SSLR_NTERM );


	//auxsym-Produktion auf rechter Seite eintragen
	insertElement( prod_index, prod2_index, hash_index );


	//neue PROD_STR anfordern
	prod_index = getNewProd();

	//prod_index dieser Produktion in die Hash-Tabelle eintragen
	hash_tab->setProdStr( hash_index, prod_index );

	//Hash-Index des Nichtterminals eintragen
	prod_str[prod_index].hash_index = hash_index;

	//PROD2_STR anfordern
	prod2_index = getNewProd2( prod_index );

	//Zeiger auf PROD2_STR
	p2 = &prod2_str[prod2_index];

	//den Index der ersten Alternative der Produktion merken
	prod_str[prod_index].prod2_index = prod2_index;

	//Priorität der Produktion eintragen
	p2->priority = 0;

	//Terminal in Hash-Tabelle eintragen
	hash_index = hash_tab->insert( "EPSILON" );

	//EPSILON auf rechter Seite eintragen
	insertElement( prod_index, prod2_index, hash_index );

	//Ist ein Terminal
	hash_tab->setType( hash_index, SSLR_TERM );

	//Anzahl der Alternativen eintragen
	prod_str[prod_index].altAnz = 1;



	//semantische Aktion eintragen
	p2->semAct = semActStr.semAct;

	//Zeile merken
	p2->semActLine = semActStr.line;

	//ist eine embedded semantic action
	p2->isEmbeddedSemAct = true;
}


int lrparser::findDoubleAlt( void )
{
	//doppelte Alternativen finden
	PROD_STR	*p1;
	PROD2_STR	*p2, *p3;
	char		*name;
	int			i, j, k, l, err = 0, ri;

	//Alle Produktionen
	for( i = 0; i < prod_anz; i++ )
	{
		//Zeiger auf PROD_STR
		p1 = &prod_str[i];

		//Namen der Produktion holen
		name = hash_tab->getName( p1->hash_index );

		//Alle rechten Seiten
		ri = p1->altAnz;

		//Alle Alternativen
		for( j = p1->prod2_index; j < ( p1->prod2_index + ri ); j++ )
		{
			//Zeiger auf PROD2_STR
			p2 = &prod2_str[j];

			//Alle aneren Alternativen
			for( k = j + 1; k < ( p1->prod2_index + ri ); k++ )
			{
				//Zeiger auf PROD2_STR
				p3 = &prod2_str[k];

				//Anzahl der Elemente gleich?
				if( p2->riAnz == p3->riAnz )
				{
					//die rechten Seiten von p2 und p3 vergleichen
					for( l = 0; l < p2->riAnz; l++ )
					{
						//Elemente gleich?
						if( p2->right[l] != p3->right[l] )
							//Nein
							break;
					}

					//gleich, wenn l = p2->riAnz
					if( l == p2->riAnz )
					{
						//Alternativen sind gleich
						cerr << "sslr: ERROR: Alternativen " << ( j - p1->prod2_index  + 1 ) << " und " << ( k - p1->prod2_index + 1 ) << " gleich in Produktion " << name << "!\n";

						//Fehler
						err++;
					}
				}
			}
		}
	}

	//zurück
	return( err );
}


int lrparser::getErrorStr( void )
{
	//Erzeuge eine neue ERROR_STR und liefere den Index zurück
	ERROR_STR	*tmp;

	//eine neue ERROR_STR anfordern
	tmp = new( ERROR_STR[error_anz + 1] );

	//Inhalt kopieren
	memmove( tmp, error_str, sizeof( ERROR_STR ) * error_anz );

	//Initalisieren
	memset( &tmp[error_anz], 0, sizeof( ERROR_STR ) );

	//Adressen
	error_str = tmp;

	//Index zurückliefern
	return( error_anz++ );
}


int lrparser::insertErrTermStr( ERROR_STR *errStr, long terminal, int neg )
{
	//fügt eine ERRTERM_STR zu einer ERROR_STR hinzu
	ERRTERM_STR		*tmp, *tmp2;
	int				i;

	//Alle ERRTERM_STR prüfen, ob das Terminal schon da ist
	for( i = 0; i < errStr->errTermStrAnz; i++ )
	{
		//Zeiger auf die ERRTERM_STR holen
		tmp = &errStr->errTermStr[i];

		//Terminle gleich?
		if( tmp->terminal == terminal )
		{
			//gleich
			cerr << "sslr: Warning: line " << errStr->line << ": Terminal " << hash_tab->getName( terminal ) << " already used in user-defined error-action!\n";

			//zurück
			return( -1 );
		}
	}

	//ERRTERM_STR erzeugen und zu errStr hinzufügen

	//eine neue ERRTERM_STR anfordern
	tmp = new( ERRTERM_STR[errStr->errTermStrAnz + 1] );

	//eintragen
	tmp2 = &tmp[errStr->errTermStrAnz];
	tmp2->terminal = terminal;
	tmp2->neg = neg;

	//Inhalt kopieren
	memmove( tmp, errStr->errTermStr, sizeof( ERRTERM_STR ) * errStr->errTermStrAnz );

	//Adressen
	errStr->errTermStr = tmp;

	//Index zurückliefern
	return( errStr->errTermStrAnz++ );
}


void lrparser::insertStartProd( long &prod_index, long &prod2_index )
{
	//Start-Produktion eintragen
	long	hash_index;

	//EPSILON eintragen
	epsHashIndex = hash_tab->insert( "EPSILON" );

	//Ist ein Terminal
	hash_tab->setType( epsHashIndex, SSLR_TERM );

	//ACCEPT eintragen
	acceptHashIndex = hash_tab->insert( "ACCEPT" );

	//Ist ein Terminal
	hash_tab->setType( acceptHashIndex, SSLR_TERM );

	//Check-Index für LALR-Parser
	lalrHashIndex = hash_tab->insert( "LALR" );

	//Ist ein Terminal
	hash_tab->setType( lalrHashIndex, SSLR_TERM );



	//Startproduktion in Hash-Tabelle eintragen
	hash_index = hash_tab->insert( "start" );

	//Ist ein Nichtterminal
	hash_tab->setType( hash_index, SSLR_NTERM );

	//Nichtterminal wird definiert
	hash_tab->setDefined( hash_index, 0 );

	//für Fehlerbehandlung merken
//	setCheck( hash_index );

	//neue PROD_STR anfordern
	prod_index = getNewProd();

	//Hash-Index des Nichtterminals eintragen
	prod_str[prod_index].hash_index = hash_index;

	//PROD2_STR anfordern
	prod2_index = getNewProd2( prod_index );

	//den Index der ersten Alternative der Produktion merken
	prod_str[prod_index].prod2_index = prod2_index;
}


void lrparser::setCheck( long hash_index )
{
	//Hash-Index in Liste eintragen zur allgemeinen Überprüfung
	long	i, *tmp;

	//Alle durchgehen
	for( i = 0; i < anzCheck; i++ )
	{
		//Gleich?
		if( check[i] == hash_index )
			//Ja
			return;

		else if( check[i] > hash_index )
		{
			//Mehr Speicher
			tmp = new( long[anzCheck + 1] );

			//umkopieren
			memcpy( tmp, check, sizeof( long ) * i );
			tmp[i] = hash_index;
			memcpy( &tmp[i+1], &check[i], sizeof( long ) * ( anzCheck - i ) );

			//ein Eintrag mehr
			anzCheck++;

			//Speicher freigeben
			if( check )
				delete[] ( check );

			//Zeiger
			check = tmp;

			//Schleife verlassen
			break;
		}
	}

	//Leer?
	if( i == anzCheck )
	{
		//Ja, anhängen

		//Mehr Speicher
		tmp = new( long[anzCheck + 1] );

		//umkopieren
		memcpy( tmp, check, sizeof( long ) * anzCheck );
		tmp[anzCheck] = hash_index;

		//ein Eintrag mehr
		anzCheck++;

		//Speicher freigeben
		if( check )
			delete[] ( check );

		//Zeiger
		check = tmp;
	}
}


int lrparser::checkAllReduce( void )
{
	//prüfen, ob mit allen Alternativen reduziert wurde
	/*
		Hier wird geprüft, ob
		- jemals mit einer Alternative reduziert wird
	*/
	PROD2_STR	*pStr;
	int			i, err = 0;

	//prüfen, ob mit allen Alternativen auch reduziert wurde
	for( i = 0; i < prod2_anz; i++ )
	{
		//Zeiger auf Struktur
		pStr = &prod2_str[i];

		//wurde reduziert?
		if( pStr->reduce <= 0 )
		{
			//Nein, Warnung ausgeben
			cerr << "sslr: ERROR: Never reduced with ";

			//rechte Seite ausgeben
			outputAlt( cerr, i );

			//Meldung
			cerr << "!" << endl;

			//Fehler
			err++;
		}
	}

	//Wenn Fehler, zurückgeben
	return( err );
}


int lrparser::checkAll( void )
{
	/*
		Hier wird geprüft, ob
		- (Nicht)Terminale definiert wurden
		- (Nicht)Terminale benutzt wurden
		- (Nicht)Terminale schon als Terminale benutzt wurden
		- Mehrfachdefinitionen
	*/
	char	*id[3] = { "", "Terminal", "Nonterminal" }, *name;
	int		type, i, err = 0;
	int		tc = 0, nc = 0;

	//Alle eingetragenen Elemente
	for( i = 0; i < anzCheck; i++ )
	{
		//Typ holen
		type = hash_tab->getType( check[i] );

		//Terminal?
		if( type == SSLR_TERM )
			//Ja
			tc++;

		//Nichtterminal?
		else if( type == SSLR_NTERM )
			//Ja
			nc++;

		//Name holen
		name = hash_tab->getName( check[i] );

		//Definiert?
		if( !hash_tab->getDefined( check[i] ) )
		{
			//Element wurde nicht definiert
			cerr << "sslr: Warning: " << id[type] << " " << name << " not defined!\n";

			//Fehler
//			err++;
		}

		//benutzt?
		if( hash_tab->getUsed( check[i] ) == false )
		{
			//Element wurde nicht benutzt
			cerr << "sslr: ERROR: " << id[type] << " " << name << " not used!\n";

			//Fehler
			err++;
		}

		//mehrfach benutzt?
		if( hash_tab->getDefined( check[i] ) > 1 )
		{
			//Element wurde nicht benutzt
			cerr << "sslr: ERROR: " << id[type] << " " << name << " more than once defined!\n";

			//Fehler
			err++;
		}

		//Produktion definiert?
		if( ( type == SSLR_NTERM ) && ( hash_tab->getProdDef( check[i] ) == false ) )
		{
			//Produktion wurde nicht definiert
			cerr << "sslr: ERROR: Production " << name << " not defined!\n";

			//Fehler
			err++;
		}
	}

	//Speicher für das Array für die Terminale
	termArr = new( long[tc + 1] );

	//Speicher für das Array für die Nichtterminale
	ntermArr = new( long[nc + 1] );

	//Anazhl der Einträge merken
	termArrCount = tc + 1;
	ntermArrCount = nc + 1;

	//start-Produktion als erste Produktion eintragen
	ntermArr[0] = prod_str[0].hash_index;

	//Index in der LR-Tabelle eintragen
	hash_tab->setLRTabIndex( ntermArr[0], 0 );

	//Hash-Indices aus checkArr
	for( i = 0, tc = 0, nc = 1; i < anzCheck; i++ )
	{
		//Name holen
		name = hash_tab->getName( check[i] );

		//Typ holen
		type = hash_tab->getType( check[i] );

		//Terminal?
		if( type == SSLR_TERM )
		{
			//Ja, eintragen
			termArr[tc] = check[i];

			//Index in der LR-Tabelle eintragen
			hash_tab->setLRTabIndex( check[i], tc++ );
		}

		//Nichtterminal?
		else if( type == SSLR_NTERM )
		{
			//Ja
			ntermArr[nc] = check[i];

			//Index in der LR-Tabelle eintragen
			hash_tab->setLRTabIndex( check[i], nc++ );
		}
	}

	//ACCEPT als letztes Terminal eintragen
	termArr[tc] = acceptHashIndex;

	//Index in der LR-Tabelle eintragen
	hash_tab->setLRTabIndex( acceptHashIndex, tc );

	//Wenn Fehler, zurückgeben
	return( err );
}


void lrparser::insertUnion( char *type, char *id )
{
	//Neuen Union-Eintrag
	char	**t1, **t2;
	int		i;

	//Speicher für die Typen
	t1 = new( char *[union_str->anz + 1] );

	//Speicher für die Bezeichner
	t2 = new( char *[union_str->anz + 1] );

	//kopieren
	for( i = 0; i < union_str->anz; i++ )
	{
		//Nur die Adressen kopieren
		t1[i] = union_str->type[i];
		t2[i] = union_str->id[i];
	}

	//den alten Speicher freigeben
	if( union_str->type )
		delete[] ( union_str->type );

	if( union_str->id )
		delete[] ( union_str->id );

	//Speicher für neue Einträge
	t1[union_str->anz] = new( char[strlen( type ) + 1] );
	t2[union_str->anz] = new( char[strlen( id ) + 1] );

	//Strings kopieren
	strcpy( t1[union_str->anz], type );
	strcpy( t2[union_str->anz], id );

	//ein weiterer Eintrag
	union_str->anz++;

	//Adressen kopieren
	union_str->type = t1;
	union_str->id = t2;
}


int lrparser::getSemIndex( char *name )
{
	//In name steht der Typ zur semantischen Aktion. Den Index in union_str suchen.
	/*
		Beispiel:
			%union
			{
				int		a;
			}

		In name steht nun a, den Index in union_str zurückliefern.
	*/
	UNION_STR	*h = union_str;
	int			i;

	//Alle Einträge durchgehen
	for( i = 0; i < h->anz; i++ )
	{
		//Vergleichen
		if( !strcmp( h->id[i], name ) )
			//gefunden
			return( i );
	}

	//nicht gefunden
	return( -1 );
}


long lrparser::getNewProd( void )
{
	//PROD_STR anfordern
	PROD_STR	*tmp;

	//Speicher für PROD_STR anfordern
	tmp = new( PROD_STR[prod_anz + 1] );

	//kopieren
	memmove( tmp, prod_str, sizeof( PROD_STR ) * prod_anz );

	//alten Speicher freigeben
	if( prod_str )
		delete[] ( prod_str );

	//Adressen
	prod_str = tmp;

	//Initialisieren
	memset( &prod_str[prod_anz], 0, sizeof( PROD_STR ) );

	//Index der ersten Alternative in PROD2_STR eintragen
	prod_str[prod_anz].prod2_index = prod2_anz;

	//Index zurückliefern
	return( prod_anz++ );
}


long lrparser::getNewProd2( long prod_index )
{
	//PROD2_STR anfordern
	PROD2_STR	*tmp;

	//Speicher für PROD2_STR anfordern
	tmp = new( PROD2_STR[prod2_anz + 1] );

	//kopieren
	memmove( tmp, prod2_str, sizeof( PROD2_STR ) * prod2_anz );

	//alten Speicher freigeben
	if( prod2_str )
		delete[] ( prod2_str );

	//Adressen
	prod2_str = tmp;

	//Initialisieren
	memset( &prod2_str[prod2_anz], 0, sizeof( PROD2_STR ) );

	//Index der Produktion in PROD_STR
	prod2_str[prod2_anz].prod_index = prod_index;

	//zurück setzen
	prod2_str[prod2_anz].reduce = 0;

	//Index zurückliefern
	return( prod2_anz++ );
}


void lrparser::insertElement( long prod_index, long prod2_index, long hash_index )
{
	//Element auf der rechten Seite der Produktion eintragen
//	PROD_STR	*p1 = &prod_str[prod_index];
	PROD2_STR	*p2 = &prod2_str[prod2_index];
	long		*tmp;

	//Index der Produktion eintragen
	p2->prod_index = prod_index;

	//Speicher für die Elemente anfordern
	tmp = new( long[p2->riAnz + 1] );

	//kopieren
	memmove( tmp, p2->right, sizeof( long ) * p2->riAnz );

	//alten Speicher freigeben
	if( p2->right )
		delete[] ( p2->right );

	//Adressen
	p2->right = tmp;

	//Index des Elemente in der rechten Seite eintragen
	p2->right[p2->riAnz++] = hash_index;
}


int lrparser::checkErrorStr( void )
{
	//Die ERROR_STR prüfen
	ERRTERM_STR	*errTermStr;
	PROD2_STR	*p2;
	ERROR_STR	*a, *b;
	long		hashSize;
	char		*name;
	int			i, j, k, l, m, termIndex, pass = 0;
	int			*errArr1, *errArr2, errArr1Cnt = 0, errArr2Cnt = 0;
	int			*tArr, err = 0;

	//Gibt es überhaupt Fehlerangaben?
	if( error_anz )
	{
		//Die Größe der Hash-Tabelle holen
		hashSize = hash_tab->getHashTabSize();

		//Speicher für die schon bereits behandelten ERROR_STR
		errArr1 = new( int[error_anz] );
		errArr2 = new( int[error_anz] );

		//Initialisieren
		memset( errArr2, 0, sizeof( int ) * error_anz );

		//Speicher für die Terminal-Prüfung
		tArr = new( int[termArrCount] );


		//Alle ERROR_STR
		for( i = 0; i < error_anz; i++ )
		{
			//Zeiger auf ERROR_STR
			a = &error_str[i];

			//Speicher für die Terminal-Prüfung
			a->tArr = new( int[termArrCount] );

			//Array für die Prüfung der Doppeldeutigkeit bei den Terminalen
			memset( a->tArr, 0, sizeof( int ) * termArrCount );
		}


		//Alle ERROR_STR
		for( i = 0; i < error_anz; i++ )
		{
			//Zeiger auf ERROR_STR
			a = &error_str[i];

			//Schon bearbeitet?
			if( ( errArr2Cnt ) && ( errArr2[i] ) )
				//Ja
				continue;

			//aufnehmen
			errArr1[errArr1Cnt++] = i;

			//verhindert ein doppeltes Prüfen dieser ERROR_STR
			errArr2[i] = 1;
			errArr2Cnt++;

			//Alle anderen ERROR_STR aufnehmen, die daselbe Item behandeln
			for( j = i + 1; j < error_anz; j++ )
			{
				//Zeiger auf ERROR_STR
				b = &error_str[j];

				//Gleiche Items?
				if( ( a->prod2_index == b->prod2_index ) && ( a->dot == b->dot ) )
				{
					//Ja, aufnehmen
					errArr1[errArr1Cnt++] = j;

					//verhindert ein doppeltes Prüfen dieser ERROR_STR
					errArr2[j] = 1;
					errArr2Cnt++;
				}
			}

			/*
				Zwei Passes: beim ersten werden die negierten Terminale untersucht, beim zweiten die
				nicht-negierten.
				Es werden doppelte Terminale überprüft, etc.
			*/
			pass = 0;

			//Alle ERROR_STR durchgehen
			for( k = 0, pass = 0; ( k < errArr1Cnt ) && ( pass < 2 ); k++ )
			{
				//Zeiger auf ERROR_STR
				b = &error_str[errArr1[k]];

				//mit Terminalen assoziierten Fehleraktionen?
				if( b->errTermStrAnz )
				{
					//Ja, die betreffenden Terminale eintragen
					for( l = 0; l < b->errTermStrAnz; l++ )
					{
						//Zeiger auf ERRTERM_STR holen
						errTermStr = &b->errTermStr[l];

						//nur bei Pass 1
						if( !pass )
						{
							//negiertes Terminal?
							if( errTermStr->neg )
							{
								//Namen des Terminals
								name = hash_tab->getName( errTermStr->terminal );

								//Ja, alle ausser dieses
								for( m = 0; m < termArrCount; m++ )
								{
									//Ist es das nicht-negierte Terminal?
									if( hash_tab->getLRTabIndex( errTermStr->terminal ) != m )
									{
										//schon gesetzt?
										if( b->tArr[m] != -1 )
											//Nein, setzen
											b->tArr[m] = 1;
									}

									else
										//Ist das negierte Terminal, deselektieren
										b->tArr[m] = -1;
								}
							}
						}

						//nur bei Pass 2
						else if( pass == 1 )
						{
							//Nun all die Terminale prüfen, die nicht negiert sind

							//negiertes Terminal?
							if( !errTermStr->neg )
							{
								//Nein
								termIndex = hash_tab->getLRTabIndex( errTermStr->terminal );

								//Namen des Terminals
								name = hash_tab->getName( errTermStr->terminal );

								//schon besetzt?
								if( b->tArr[termIndex] )
								{
									//Ja, Warnung ausgeben
									cerr << "sslr: ERROR: line " << b->line << ": Terminal " << hash_tab->getName( errTermStr->terminal ) << " already used in used-defined error-action!\n";

									//Fehler
									err++;
								}

								//setzen
								b->tArr[termIndex] = 1;
							}
						}
					}
				}

				//alle durch im ersten Pass?
				if( k == ( errArr1Cnt - 1 ) )
				{
					//Ja
					pass++;
					k = -1;
				}
			}


			/*
				Hier werden Fehlerangaben gefunden, die nie ausgeführt werden. Beim folgenden Beispiel wird
				die letzte Fehlerangabe nicht ausgeführt.
				1:!ADD:			"Fehlermeldung1"
				1:ADD			"Fehlermeldung2"
				1:SHIFT(ADD)	"Fehlermeldung3"
			*/

			//Array für die Prüfung der Doppeldeutigkeit bei den Terminalen
			memset( tArr, 0, sizeof( int ) * termArrCount );

			//Alle ERROR_STR durchgehen
			for( k = 0; k < errArr1Cnt; k++ )
			{
				//Zeiger auf ERROR_STR
				b = &error_str[errArr1[k]];

				//benutzte Terminale übertragen
				for( m = 0; m < termArrCount; m++ )
				{
					//gesetzt?
					if( b->tArr[m] > 0 )
						//Ja, übertragen
						tArr[m] = 1;
				}
			}

			//Nun stehen in tArr alle benutzten Terminale der ERROR_STR, die alle daselbe Item behandeln

			//Alle ERROR_STR durchgehen
			for( k = 0; k < errArr1Cnt; k++ )
			{
				//Zeiger auf ERROR_STR
				b = &error_str[errArr1[k]];

				//Ist es eine Fehlerangabe ohne terminal-gebundene Aktion?
				if( !b->errTermStrAnz )
				{
					//Ja, prüfen, ob sie jemals ausgeführt werden kann
					for( m = 0, l = 0; m < termArrCount; m++ )
					{
						//noch frei?
						if( !tArr[m] )
							//Ja
							l++;
					}

					//Waren noch Terminale frei?
					if( !l )
					{
						//Nein, Fehlermeldung
						cerr << "sslr: ERROR: line " << b->line << ": Error-action will never be used!\n";

						//Fehler
						err++;
					}
				}
			}





			//Hier wird geprüft, ob zwischen den ERROR_STR sich Terminale überschneiden.
			//Alle ERROR_STR durchgehen
			for( k = 0; k < errArr1Cnt; k++ )
			{
				//Zeiger auf ERROR_STR
				a = &error_str[errArr1[k]];

				//hat a überhaupt terminal-gebundene Fehlerangaben?
				if( !a->errTermStrAnz )
				//Nein, nächste ERROR_STR
					continue;

				//Alle anderen ERROR_STR prüfen
				for( l = k + 1; l < errArr1Cnt; l++ )
				{
					//Zeiger auf ERROR_STR
					b = &error_str[errArr1[l]];

					//sich überschneidende Terminale finden
					for( m = 0; m < termArrCount; m++ )
					{
						//In beiden ERROR_STR genutzt?
						if( ( a->tArr[m] > 0 ) && ( b->tArr[m] > 0 ) )
						{
							//Den Namen des Terminals holen
							name = hash_tab->getName( termArr[m] );

							//Meldung ausgeben
							cerr << "sslr: ERROR: line " << a->line << ": Collision in Terminal " << name << " in error-action in line " << b->line << "!\n";

							//Fehler
							err++;
						}
					}
				}
			}





			/*
				Hier wird geprüft, ob Fehlerangaben der folgenden Form auftauchen:

				1:SHIFT(ADD)	"Fehlermeldung 1"
				1:OVERREAD(SUB)	"Fehlermeldung 2"

				Dies geht nicht, da nicht beide Fehlermeldungen gleichzeitig ausgegeben werden, besser:
				1:SHIFT(ADD):OVERREAD(SUB)	"Fehlermeldung 1 und 2"
			*/

			//Alle ERROR_STR durchgehen
			for( k = 0; k < errArr1Cnt; k++ )
			{
				//Zeiger auf ERROR_STR
				a = &error_str[errArr1[k]];

				//hat a überhaupt terminal-gebundene Fehlerangaben?
				if( a->errTermStrAnz )
					//Ja, nächste ERROR_STR
					continue;

				//Alle anderen ERROR_STR prüfen
				for( l = k + 1; l < errArr1Cnt; l++ )
				{
					//Zeiger auf ERROR_STR
					b = &error_str[errArr1[l]];

					//hat b überhaupt terminal-gebundene Fehlerangaben?
					if( b->errTermStrAnz )
						//Ja, nächste ERROR_STR
						continue;

					//Aktionen?
					if( ( a->del || a->exit || a->overread || a->sem || a->shift ) && ( b->del || b->exit || b->overread || b->sem || b->shift  ) )
					{
						//Bei beiden eine Fehlermeldung?
						if( a->msg && b->msg )
						{
							//nicht gut
							cerr << "sslr: ERROR: line " << a->line << ": Both error-actions with an error message in line " << b->line << "!\n";

							//Fehler
							err++;
						}
					}
				}
			}

			//zurücksetzen
			errArr1Cnt = 0;
		}







		//Nun prüfen, ob EXIT mit etwas anderem zusammen steht
		for( k = 0, pass = 0; ( k < error_anz ) && ( pass < 2 ); k++ )
		{
			//Zeiger auf ERROR_STR
			a = &error_str[k];

			//Ist EXIT aktiviert?
			if( a->exit )
			{
				//mit SHIFT?
				if( a->shift || a->del || a->overread || a->sem )
					//unsinnig
					cerr << "sslr: Warning: line " << a->line << ": It makes no sense to use EXIT together with other user-defined error-actions!\n";
			}
		}






		//Nun prüfen, ob eine Fehlerangabe jemals ausgeführt wird
		for( k = 0, pass = 0; ( k < error_anz ) && ( pass < 2 ); k++ )
		{
			//Zeiger auf ERROR_STR
			a = &error_str[k];

			//Zeiger auf PROD2_STR
			p2 = &prod2_str[a->prod2_index];

			//Ist die Position des Punktes zu groß?
			if( a->dot > p2->riAnz )
			{
				//wird niemals ausgeführt
				cerr << "sslr: ERROR: line " << a->line << ": Position of dot " << a->dot << " to big!\n";

				//Fehler
				err++;
			}
		}



		//Speicher freigeben
		delete[] ( errArr1 );
		delete[] ( errArr2 );
		delete[] ( tArr );
	}

	//Anzahl der Fehler zurückgeben
	return( err );
}


void lrparser::sortProd2Str( void )
{
	//Die PROD2_STR sortieren
	PROD2_STR	*a, *b, *source, *dest, tmp;
	PROD_STR	*p1;
	char		*name1, *name2;
	int			i, j, k, anz;

	//Initialisieren
	i = 0;

	//alle Alternativen
	while( i < ( prod2_anz - 1 ) )
	{
		//Zeiger auf PROD2_STR
		a = &prod2_str[i];

		//Den Namen holen
		name1 = hash_tab->getName( prod_str[a->prod_index].hash_index );

		//Index
		j = i++;

		do
		{
			//nächsten
			j++;

			//Zeiger auf PROD2_STR
			b = &prod2_str[j];

			//Zeiger auf PROD_STR
			p1 = &prod_str[b->prod_index];

			//Den Namen holen
			name2 = hash_tab->getName( p1->hash_index );

		}while( ( a->prod_index == b->prod_index ) && ( j < ( prod2_anz - 1 ) ) );

		//Alle Alternativen
		for( k = j++; j < prod2_anz; j++ )
		{
			//Zeiger auf PROD2_STR
			b = &prod2_str[j];

			//Den Namen holen
			name2 = hash_tab->getName( prod_str[b->prod_index].hash_index );

			//gleiche Produktionen, d.h. prod_index gleich?
			if( a->prod_index == b->prod_index )
			{
				//Ja

				//Zeiger auf PROD2_STR
				source = &prod2_str[j];

				//Zeiger auf PROD2_STR
				dest = &prod2_str[k];

				//j sichern
				tmp.prod_index = source->prod_index;
				tmp.riAnz = source->riAnz;
				tmp.right = source->right;
				tmp.priority = source->priority;
				tmp.semAct = source->semAct;
				tmp.semActLine = source->semActLine;

				//verschieben
				memmove( &prod2_str[k+1], &prod2_str[k], sizeof( PROD2_STR ) * ( j - k ) );

				//eintragen
				dest->prod_index = tmp.prod_index;
				dest->riAnz = tmp.riAnz;
				dest->right = tmp.right;
				dest->priority = tmp.priority;
				dest->semAct = tmp.semAct;
				dest->semActLine = tmp.semActLine;

				//nächste Position
				k++;
			}
		}

		//ab k + 1
		i = k;
	}

	//Indices anpassen
	for( i = 0, k = 0; i < prod_anz; i++ )
	{
		//Anzahl der Alternativen
		anz = prod_str[i].altAnz;

		//prod2_index anpassen
		prod_str[i].prod2_index = k;

		//Alle zugehörigen Alternativen
		for( j = k; j < ( k + anz ); j++ )
			//PROD_STR-Index eintragen
			prod2_str[j].prod_index = i;

		//überspringen
		k += anz;
	}
}


void lrparser::outputGrammar( void )
{
	//Grammatik in Datei ausgeben
	PROD2_STR	*p2;
	PROD_STR	*p1;
	char		*name;
	int			i, j, k, r;

	//Grammatik in Datei ausgeben?
	if( argv_str->outputGrammar )
	{
		//Stream öffnen
		ofstream	out( argv_str->name );

		//Header ausgeben
		out << "/*\n";
		out << "Grammatik in BNF-Form. Generiert vom SSLR-Parser-Generator V" << VERSION << ".\n";
		out << "(c) 2002-2005 by Sven Schmidt\n";
		out << "s.schmidt@lboro.ac.uk\n";
		out << "*/\n\n";

		//Alle Produktionen
		for( i = 1; i < prod_anz; i++ )
		{
			//Zeiger auf die PROD_STR holen
			p1 = &prod_str[i];

			//Den Namen der Produktion holen
			name = hash_tab->getName( p1->hash_index );

			//Produktion ausgeben
			out << name << "\n";

			//Seperator ausgeben
			out << "\t->\t";

			//alle Alternativen ausgeben
			for( j = p1->prod2_index, k = 0; j < ( p1->prod2_index + p1->altAnz ); j++, k++ )
			{
				//Zeiger auf die PROD2_STR holen
				p2 = &prod2_str[j];

				//neue Alternative?
				if( k )
					//Ja
					out << "\t|\t";

				//rechte Seite ausgeben
				for( r = 0; r < p2->riAnz; r++ )
				{
					//Namen holen
					name = hash_tab->getName( p2->right[r] );

					//Terminal?
					if( hash_tab->getType( p2->right[r] ) == SSLR_TERM )
						//Ja
						out << " '" << name << "' ";

					else
						//Nichtterminal
						out << " " << name << " ";
				}

				//Priorität ausgeben
				out << "\t/* Priority: " << p2->priority << " */\n";
			}

			//Produktion abschliessen
			out << "\t;\n\n";
		}
	}
}


void lrparser::outputAlt( ostream &s, long prod2_index )
{
	//Alle Alternativen der Produktion ausgeben, zu der die Alternative prod2_index gehört
	PROD_STR	*p1;
	PROD2_STR	*p2;
	char		*name;
	int			r;

	//Zeiger auf die PROD2_STR holen
	p2 = &prod2_str[prod2_index];

	//Zeiger auf die PROD_STR holen
	p1 = &prod_str[p2->prod_index];

	//Namen der Produktion holen
	name = hash_tab->getName( p1->hash_index );

	//ausgeben
	s << " " << name << " -> ";

	//rechte Seite ausgeben
	for( r = 0; r < p2->riAnz; r++ )
	{
		//Namen holen
		name = hash_tab->getName( p2->right[r] );

		//Terminal?
		if( hash_tab->getType( p2->right[r] ) == SSLR_TERM )
			//Ja
			s << " '" << name << "' ";

		else
			//Nichtterminal
			s << " " << name << " ";
	}

	//nächste Zeile
	s << "\n";
}


void lrparser::outputItem( long prod2_index, int dot, ofstream &file )
{
	//Ein Item ausgeben
	PROD_STR	*p1;
	PROD2_STR	*p2;
	char		*name;
	int			r;

	//Zeiger auf die PROD2_STR holen
	p2 = &prod2_str[prod2_index];

	//Zeiger auf die PROD_STR holen
	p1 = &prod_str[p2->prod_index];

	//Namen der Produktion holen
	name = hash_tab->getName( p1->hash_index );

	//ausgeben
	file << " " << name << " -> ";

	//rechte Seite ausgeben
	for( r = 0; r < p2->riAnz; r++ )
	{
		//Namen holen
		name = hash_tab->getName( p2->right[r] );

		//Position des Punktes?
		if( dot == r )
			//Ja
			file << ".";

		//Terminal?
		if( hash_tab->getType( p2->right[r] ) == SSLR_TERM )
			//Ja
			file << " '" << name << "' ";

		else
			//Nichtterminal
			file << " " << name << " ";
	}

	//Position des Punktes?
	if( dot == r )
		//Ja
		file << ".";
}


void lrparser::outputAllAlt( void )
{
	//Alle Alternativen ausgeben
	PROD_STR	*p1;
	PROD2_STR	*p2;
	ofstream	out( argv_str->name );
	char		*name;
	int			j, r;

	//alle Alternativen ausgeben
	for( j = 0; j < prod2_anz; j++ )
	{
		//Zeiger auf die PROD2_STR holen
		p2 = &prod2_str[j];

		//Zeiger auf die PROD_STR holen
		p1 = &prod_str[p2->prod_index];

		//Namen der Produktion holen
		name = hash_tab->getName( p1->hash_index );

		//ausgeben
		out << " " << name << " -> ";

		//rechte Seite ausgeben
		for( r = 0; r < p2->riAnz; r++ )
		{
			//Namen holen
			name = hash_tab->getName( p2->right[r] );

			//Terminal?
			if( hash_tab->getType( p2->right[r] ) == SSLR_TERM )
				//Ja
				out << " '" << name << "' ";

			else
				//Nichtterminal
				out << " " << name << " ";
		}

		//nächste Zeile
		out << "\n";
	}
}


void lrparser::genAllFirst( void )
{
	//Alle FIRST-Mengen bilden
	PROD2_STR	*p2;
	PROD_STR	*p1;
	Terminal	*f1;
	long		anzFirst;
	char		*name;
	int			i, j, changed;
	int			pass = 0;
	int			*pArr;

	//keine doppelten Ausgaben
	pArr = new( int[prod_anz] );

	//Initialisieren
	memset( pArr, 0, sizeof( int ) * prod_anz );

	do
	{
		//Prüft, ob weitere Terminale zur FIRST-Menge hinzugefügt wurden
		changed = 0;

		//ein weiterer Pass
		pass++;

		//Alle Produktionen
		for( i = 0; i < prod_anz; i++ )
		{
			//Zeiger auf PROD_STR
			p1 = &prod_str[i];

			//Ja, Name holen
			name = hash_tab->getName( p1->hash_index );

			//Ausführliche Ausgaben?
			if( ( argv_str->verbose ) && !( pArr[i] ) )
			{
				//Ja
				cout << "sslr: Generate FIRST(" << name << ")...\n";

				//Meldung nicht nocheinmal ausgeben
				pArr[i]++;
			}

			//Anzahl der FIRST-Elemente holen
			anzFirst = hash_tab->getFirstAnz( p1->hash_index );

			//Zeiger auf FIRST-Mengen holen
			f1 = hash_tab->getFirst( p1->hash_index );

			//Alle Alternativen
			for( j = p1->prod2_index; j < ( p1->prod2_index + p1->altAnz ); j++ )
			{
				//Zeiger auf PROD2_STR
				p2 = &prod2_str[j];

				//FIRST-Menge berechnen
				genFirst( f1, p2->riAnz, p2->right );
			}

			//geändert?
			if( anzFirst != hash_tab->getFirstAnz( p1->hash_index ) )
				//Ja
				changed++;
		}

	}while( changed );

	//Speicher wieder freigeben
	delete[] ( pArr );

	//Ausführliche Ausgaben?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: " << pass << " pass(es) to generate FIRST-Sets!\n";

	//FIRST-Mengen ausgeben?
	if( argv_str->showFirst )
	{
		//Alle Produktionen
		for( i = 0; i < prod_anz; i++ )
		{
			//Zeiger auf PROD_STR
			p1 = &prod_str[i];

			//Ja, Name holen
			name = hash_tab->getName( p1->hash_index );

			//Meldung
			cout << "\nFIRST(" << name << ")=\n";

			//FIRST-Menge ausgeben
			hash_tab->printFirst( p1->hash_index );
		}
	}
}


void lrparser::genFirst( Terminal *f, int anzElem, long *arr )
{
	//Hier werden die FIRST-Mengen eines Terminals/Nichtterminals erzeugt
	Terminal	*f2, *f3;
	char		*name;
	int			i, eps = 0;

	//für jeden Durchgang eine eigene FIRST-Menge
	f3 = new Terminal( epsHashIndex );

	//Gibt es überhaupt noch Elemente
	if( anzElem > 0 )
	{
		//Alle Elemente
		for( i = 0; i < anzElem; i++ )
		{
			//Namen holen
			name = hash_tab->getName( arr[i] );

			//Zeiger auf die FIRST-Menge holen
			f2 = hash_tab->getFirst( arr[i] );

			//hinzufügen
			f3->add( f2 );

			//beinhaltet die FIRST-Menge EPSILON?
			if( f2->isEpsilon() )
				//Ja, hochzählen
				eps++;

			else
				//Nein, for-Schleife verlassen
				break;
		}

		//Epsilon zu dieser Menge hinzufügen?
		if( eps && ( eps < anzElem ) )
		{
			//Nein, aus der FIRST-Menge löschen

			//EPSILON aus FIRST-Menge löschen
			f3->del( epsHashIndex );
		}
	}

	else
		//leere Menge, EPSILON hinzufügen
		f3->ins( epsHashIndex );

	//FIRST-Menge hinzufügen
	f->add( f3 );

	//Speicher freigeben
	delete( f3 );
}


void lrparser::genAllFollow( void )
{
	//Alle FOLLOW-Mengen bilden
	PROD2_STR	*p2;
	PROD_STR	*p1, *p3;
	Terminal	*f1, *f2, *first;
	long		anzFollow;
	char		*name, *tmpName;
	int			i, j, r, changed;
	int			pass = 0;
	int			*pArr;

	//keine doppelten Ausgaben
	pArr = new( int[prod_anz] );

	//Initialisieren
	memset( pArr, 0, sizeof( int ) * prod_anz );

	//ACCEPT in die Start-Produktion aufnehmen
	f1 = hash_tab->getFollow( prod_str[0].hash_index );

	//ACCEPT eintragen
	f1->ins( acceptHashIndex );

	do
	{
		//Prüft, ob weitere Terminale zur FOLLOW-Menge hinzugefügt wurden
		changed = 0;

		//ein weiterer Pass
		pass++;

		//Alle Produktionen
		for( i = 1; i < prod_anz; i++ )
		{
			//Nein, Zeiger auf PROD_STR
			p1 = &prod_str[i];

			//Ja, Name holen
			name = hash_tab->getName( p1->hash_index );

			//Ausführliche Ausgaben?
			if( ( argv_str->verbose ) && !( pArr[i] ) )
			{
				//Ja
				cout << "sslr: Generate FOLLOW(" << name << ")...\n";

				//Meldung nicht nocheinmal ausgeben
				pArr[i]++;
			}

			//Anzahl der FOLLOW-Elemente holen
			anzFollow = hash_tab->getFollowAnz( p1->hash_index );

			//Zeiger auf FOLLOW-Mengen holen
			f1 = hash_tab->getFollow( p1->hash_index );

			//Alle Alternativen
			for( j = 0; j < prod2_anz; j++ )
			{
				//Zeiger auf PROD2_STR
				p2 = &prod2_str[j];

//Debug-Modus?
#ifdef SSLR_DEBUG
				//Alternative ausgeben
				outputAlt( j );
#endif

				//Nichtterminal check[i] auf rechter Seite suchen
				for( r = 0; r < p2->riAnz; r++ )
				{
					//Name holen
					tmpName = hash_tab->getName( p2->right[r] );

					//Gefunden?
					if( p2->right[r] == p1->hash_index )
					{
						//FIRST-Mengen bilden
						first = new Terminal( epsHashIndex );

						//FIRST-Mengen bilden
						genFirst( first, p2->riAnz - r - 1, &p2->right[r+1] );

						//EPSILON in FIRST-Menge?
						if( first->isEpsilon() )
						{
							//Ja, Zeiger auf PROD_STR
							p3 = &prod_str[p2->prod_index];

							//Namen holen
							tmpName = hash_tab->getName( p3->hash_index );

//							cerr << "FOLLOW(" << name << ") += FOLLOW(" << tmpName << ")\n";

							//Ja, FOLLOW(p2->prodindex) = FOLLOW(check[i])
							f2 = hash_tab->getFollow( p3->hash_index );

							//hinzufügen
							f1->add( f2 );

							//EPSILON aus FIRST-Menge löschen
							first->del( epsHashIndex );
						}

						//hinzufügen
						f1->add( first );

						//FIRST-Mengen löschen
						delete( first );
					}
				}
			}

			//geändert?
			if( anzFollow != hash_tab->getFollowAnz( p1->hash_index ) )
				//Ja
				changed++;
		}

	}while( changed );

	//Speicher wieder freigeben
	delete[] ( pArr );

	//Ausführliche Ausgaben?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: " << pass << " pass(es) to generate FOLLOW-Sets!\n";

	//FOLLOW-Mengen ausgeben?
	if( argv_str->showFollow )
	{
		//Alle Produktionen
		for( i = 0; i < prod_anz; i++ )
		{
			//Zeiger auf PROD_STR
			p1 = &prod_str[i];

			//Ja, Name holen
			name = hash_tab->getName( p1->hash_index );

			//Meldung
			cout << "\nFOLLOW(" << name << ")=\n";

			//FOLLOW-Menge ausgeben
			hash_tab->printFollow( p1->hash_index );
		}
	}
}


void lrparser::genLR0Hulle( void )
{
	//Generiere alle Hüllen der LR(0)-Elemente für SLR und LALR
	HULLE_STR	*h;
	int			changed = 0, pass = 0, i;

	//1. Hülle generieren
	h = createHulle();

	//ersten Kern eintragen
	insertItem( h, 0, 0, CORE );

	//Hülle eintragen
	insertHulle( h );

	//Wurde nicht durch Sprung-Funktion erzeugt
	h->from = 0;
	h->elem = 0;

	//bei Hülle 0 anfangen
	i = 0;

	do
	{
		//Passe zählen
		pass++;

		//Keine Änderungen
//		changed = 0;

		//Anzahl der Hüllen merken
//		anzH = hulle_anz;

		//Alle Hüllen
		for( i = 0; i < hulle_anz; i++ )
		{
			//Zeiger auf Hülle
			h = hulle_str[i];

			//Alle Elemente dieser Hülle erzeugen
			genLR0Items( h );

			//Ausgeben
//			outputHulle( h );

			//Anhand der Items die Hüllen erzeugen, die sich durch Spring( h, X ) ergeben
			makeLR0Jump( h );
/*
			//Anzahl der Hüllen geändert?
			if( hulle_anz != anzH )
				//Ja
				changed++;
*/
		}

	}while( changed );

	//Meldung ausgeben?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: " << pass << " pass(es) to generate " << hulle_anz << " LR(0)-Items!\n";
}


void lrparser::genLR1Hulle( void )
{
	//Generiere alle Hüllen der LR(1)-Elemente für LR
	HULLE_STR	*h;
	Terminal	*la;
	int			changed = 0, pass = 0, i;

	//1. Hülle generieren
	h = createHulle();

	//Speicher für die Lookaheads
	la = new Terminal( epsHashIndex );

	//Accept eintragen
	la->ins( acceptHashIndex );

	//ersten Kern eintragen
	insertItem( h, 0, 0, CORE, la );

	//Hülle eintragen
	insertHulle( h );

	//Wurde nicht durch Sprung-Funktion erzeugt
	h->from = 0;
	h->elem = 0;

	//bei Hülle 0 anfangen
	i = 0;

	do
	{
		//Passe zählen
		pass++;

		//Keine Änderungen
//		changed = 0;

		//Anzahl der Hüllen merken
//		anzH = hulle_anz;

		//Alle Hüllen
		for( i = 0; i < hulle_anz; i++ )
		{
			//Zeiger auf Hülle
			h = hulle_str[i];

			//Alle Elemente dieser Hülle erzeugen
			genLR1Items( h );

			//Ausgeben
//			outputHulle( h );

			//Anhand der Items die Hüllen erzeugen, die sich durch Spring( h, X ) ergeben
			makeLR1Jump( h );
/*
			//Anzahl der Hüllen geändert?
			if( hulle_anz != anzH )
				//Ja
				changed++;
*/
		}

	}while( changed );

	//Speicher freigeben
	delete( la );

	//Meldung ausgeben?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: " << pass << " pass(es) to generate " << hulle_anz << " LR(1)-Items!\n";
}


void lrparser::makeLR0Jump( HULLE_STR *h )
{
	//Hier werden die Hüllen erzeugt, die sich durch Sprung( h, X ) ergeben
	HULLE_STR	*tmp;
	PROD2_STR	*p2;
	LR_STR		*lr;
	char		*name;
	long		elem, hInd;
	int			i;

	//Alle LR(0)-Elemente der Hülle
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0)-Element
		lr = &h->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Ist der Punkt am Ende?
		if( lr->dot < p2->riAnz )
		{
			//Punkt eine Position weiter und Element eintragen

			//Das (Nicht)Terminal holen, das nach dem Punkt kommt
			/*
				Bsp: expr
							->	.expr ADD expr
							|	.expr SUB expr
							|	term
							;

				Hier also ist expr vor dem Punkt.
			*/
			elem = p2->right[lr->dot];

			/*
				Die hier erzeugte Hülle ergibt sich also aus:

				tmp = Sprung( h, elem )
			*/

			//darf nicht .EPSILON sein
			if( elem != epsHashIndex )
			{
				//Den Namen holen
				name = hash_tab->getName( elem );

				//Hülle anfordern
				tmp = createHulle();

				//Hülle erzeugen
				genLR0Jump( h, tmp, elem );

				//Hülle schon in hulle_str?
				if( ( hInd = cmpLR0Hulle( tmp ) ) < 0 )
				{
					//Nein

					// this = Sprung( h, elem ) merken
					tmp->from = h->hulleIndex;
					tmp->elem = elem;

					//In den Elementen von h, aus denen Sprung(h,elem) entsteht, den Index von tmp
					//eintragen.
					setJump( h, elem, hulle_anz );

					//Meldung ausgeben?
					if( argv_str->verbose )
						//Ja
						cout << "sslr: Generate Jump(" << name << ")...\n";

					//Hülle eintragen
					insertHulle( tmp );

					//Ausgeben
//					outputHulle( tmp );
				}

				else
				{
					//Die Hülle existiert schon
					//In den Elementen von h, aus denen Sprung(h,elem) entsteht, den Index von tmp
					//eintragen.
					setJump( h, elem, hInd );

					//Hülle löschen
					deleteHulle( tmp );
				}
			}
		}
	}
}


void lrparser::makeLR1Jump( HULLE_STR *h )
{
	//Hier werden die Hüllen erzeugt, die sich durch Sprung( h, X ) ergeben
	HULLE_STR	*tmp;
	PROD2_STR	*p2;
	LR_STR		*lr;
	char		*name;
	long		elem, hInd;
	int			i;

	//Alle LR(1)-Elemente der Hülle
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0)-Element
		lr = &h->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Ist der Punkt am Ende?
		if( lr->dot < p2->riAnz )
		{
			//Punkt eine Position weiter und Element eintragen

			//Das (Nicht)Terminal holen, das nach dem Punkt kommt
			/*
				Bsp: expr
							->	.expr ADD expr
							|	.expr SUB expr
							|	.term
							;

				Hier sind expr und term vor dem Punkt.
			*/
			elem = p2->right[lr->dot];

			/*
				Die hier erzeugte Hülle ergibt sich also aus:

				tmp = Sprung( h, elem )
			*/

			//darf nicht .EPSILON sein
			if( elem != epsHashIndex )
			{
				//Den Namen holen
				name = hash_tab->getName( elem );

				//Hülle anfordern
				tmp = createHulle();

				//Hülle erzeugen
				genLR1Jump( h, tmp, elem );

				//Hülle schon in hulle_str?
				if( ( hInd = cmpLR1Hulle( tmp ) ) < 0 )
				{
					//Nein

					// this = Sprung( h, elem ) merken
					tmp->from = h->hulleIndex;
					tmp->elem = elem;

					//In den Elementen von h, aus denen Sprung(h,elem) entsteht, den Index von tmp
					//eintragen.
					setJump( h, elem, hulle_anz );

					//Meldung ausgeben?
					if( argv_str->verbose )
						//Ja
						cout << "sslr: " << hulle_anz << " Generate Jump(" << h->hulleIndex << ", " << name << ")...\n";

					//Hülle eintragen
					insertHulle( tmp );

					//Ausgeben
//					outputHulle( tmp );
				}

				else
				{
					//Die Hülle existiert schon
					//In den Elementen von h, aus denen Sprung(h,elem) entsteht, den Index von tmp
					//eintragen.
					setJump( h, elem, hInd );

					//Hülle löschen
					deleteHulle( tmp );
				}
			}
		}
	}
}


void lrparser::genLR0Items( HULLE_STR *h )
{
	//Hier werden alle LR(0)-Items einer Hülle erzeugt.
	PROD2_STR	*p2;
	PROD_STR	*p1;
	LR_STR		*lr;
	char		*name, *tmpName;
	long		pi;
	int			j, k;

	//Alle Elemente dieser Hülle
	for( j = 0; j < h->anzElem; j++ )
	{
		/*
			Bsp:	expr	->	.expr ADD expr
							|	.expr SUB expr
							|	.term
		*/
		//Index der ersten Alternative der Produktion nach dem Punkt holen

		//Zeiger auf LR(0)-Element
		lr = &h->lr_str[j];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Ist der Punkt am Ende?
		if( lr->dot < p2->riAnz )
		{
			//Namen holen
			name = hash_tab->getName( p2->right[lr->dot] );

			//Ist es ein Nichtterminal?
			if( hash_tab->getType( p2->right[lr->dot] ) == SSLR_NTERM )
			{
				//Ja, diese Produktion als Elemente mit dot = 0 aufnehmen

				//prod_index dieses Nichtterminals
				pi = hash_tab->getProdStr( p2->right[lr->dot] );

				//Zeiger auf PROD_STR
				p1 = &prod_str[pi];

				//Namen holen
				tmpName = hash_tab->getName( p1->hash_index );

				//Index der ersten Alternative in der PROD2_STR
				for( k = p1->prod2_index; k < ( p1->prod2_index + p1->altAnz ); k++ )
					//Als LR(0)-Item hinzufügen
					insertItem( h, k, 0, ITEM );
			}
		}
	}
}


void lrparser::genLR1Items( HULLE_STR *h )
{
	//Hier werden alle LR(1)-Items einer Hülle erzeugt.
	PROD2_STR	*p2;
	PROD_STR	*p1;
	LR_STR		*lr, *lr2;
	Terminal	*la;
	char		*name, *tmpName;
	long		pi, cInd;
	int			j, k, changed, anzElem;

	do
	{
		//solange etwas hinzugefügt wurde
		changed = 0;

		//Alle Elemente dieser Hülle
		for( j = 0; j < h->anzElem; j++ )
		{
			/*
				Bsp:	expr	->	.expr ADD expr
								|	.expr SUB expr
								|	.term
			*/
			//Index der ersten Alternative der Produktion nach dem Punkt holen

			//Zeiger auf LR(0)-Element
			lr = &h->lr_str[j];

			//Zeiger auf PROD2_STR
			p2 = &prod2_str[lr->prod2_index];

			//Ist der Punkt am Ende?
			if( lr->dot < p2->riAnz )
			{
				//Namen holen
				name = hash_tab->getName( p2->right[lr->dot] );

				//Ist es ein Nichtterminal?
				if( hash_tab->getType( p2->right[lr->dot] ) == SSLR_NTERM )
				{
					//Ja, diese Produktion als Elemente mit dot = 0 aufnehmen

					//prod_index dieses Nichtterminals
					/*
						Bsp:
							term	->	.term MUL factor

						Also PROD_STR-Index von term
					*/
					pi = hash_tab->getProdStr( p2->right[lr->dot] );

					//Zeiger auf PROD_STR
					p1 = &prod_str[pi];

					//Namen holen
					tmpName = hash_tab->getName( p1->hash_index );

					//Speicher für die Lookaheads holen
					la = new Terminal( epsHashIndex );

					//FIRST-Menge bilden nach dot
					genFirst( la, p2->riAnz - lr->dot - 1, &p2->right[lr->dot + 1] );

					//EPSILON in FIRST-Menge?
					if( la->isEpsilon() )
					{
						//Ja, alle Lookaheads des LR(1)-Items hinzufügen
						la->add( lr->la );

						//EPSILON aus FIRST-Menge löschen
						la->del( epsHashIndex );
					}

					//Index der ersten Alternative in der PROD2_STR
					for( k = p1->prod2_index; k < ( p1->prod2_index + p1->altAnz ); k++ )
					{
						//Ist dieses Item schon in der Hülle, also nur die Lookaheads ergänzen?
						if( ( cInd = findItem( h, k, 0 ) ) < 0 )
							//Nein, noch nicht da, als LR(1)-Item hinzufügen
							insertItem( h, k, 0, ITEM, la );

						else
						{
							//Ja, Item schon da

							//Zeiger auf Item
							lr2 = &h->lr_str[cInd];

							//Anzahl der Lookaheads merken
							anzElem = lr2->la->getAnzElem();

							//Lookaheads ergänzen
							insertItem( h, k, 0, ITEM, la );

							//Wurden Terminale hinzugefügt?
							if( anzElem != lr2->la->getAnzElem() )
								//Ja
								changed++;


							/*

								statement_list
									:	statement
									|	statement_list statement
									;

								Wegen solchen Produktionen muss man mehrmals über die Items hinweggehen,
								da im zweiten Schritt .statement_list statement FIRST(statement)
								als Lookaheads hinzukommt, die im ersten Schritt .statement nicht
								erfasst wurden!
							*/
						}
					}

					//Speicher wieder freigeben
					delete( la );
				}
			}
		}

		//Solange Änderungen vorgenommen wurden

	}while( changed );
}


HULLE_STR *lrparser::createHulle( void )
{
	//Eine HULLE_STR-Struktur erzeugen
	HULLE_STR	*h;

	//Speicher anfordern
	h = new HULLE_STR;

	//Initailisieren
	h->anzCore = 0;
	h->anzElem = 0;
	h->lr_str = 0;

	//Zeiger auf Hülle zurückliefern
	return( h );
}


void lrparser::deleteHulle( HULLE_STR *h )
{
	//Eine HULLE_STR-Struktur löschen
	LR_STR	*lr;
	int		i;

	//Alle Elemente einer Hülle
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0/1)-Item
		lr = &h->lr_str[i];

		//Lookahead-Terminale freigeben
		if( lr->la )
			delete( lr->la );

		//Lookahead-Terminale freigeben
		if( lr->lalr )
			delete( lr->lalr );
	}

	//LR_STR freigeben
	delete[] ( h->lr_str );

	//HULLE_STR löschen
	delete( h );
}


void lrparser::delHulle( HULLE_STR *h )
{
	//Hülle löschen
	int	i;

	//Alle Elemente der Hülle
	for( i = 0; i < h->anzElem; i++ )
	{
		//Terminale freigeben
		if( h->lr_str[i].la )
		{
			delete( h->lr_str[i].la );
			h->lr_str[i].la = 0L;
		}

		//Lookaheads freigeben
		if( h->lr_str[i].lalr )
		{
			delete( h->lr_str[i].lalr );
			h->lr_str[i].lalr = 0L;
		}
	}

	//Item-Struktur freigeben
	delete[] ( h->lr_str );

	//zurücksetzen
	h->lr_str = 0L;

	//Speicher freigeben
	delete( h );
}


void lrparser::insertItem( HULLE_STR *h, long prod2_index, int dot, int type )
{
	//LR(0)-Element in HULLE h eintragen
	LR_STR	*tmp;

	//Item schon enthalten?
	if( findItem( h, prod2_index, dot ) < 0 )
	{
		//Nein, Speicher für das LR(0)-Element
		tmp = new( LR_STR[h->anzElem + 1] );

		//kopieren
		memmove( tmp, h->lr_str, sizeof( LR_STR ) * h->anzElem );

		//Alten Speicher freigeben
		if( h->lr_str )
			delete[] ( h->lr_str );

		//Zeiger
		h->lr_str = tmp;

		//eintragen
		h->lr_str[h->anzElem].dot = dot;
		h->lr_str[h->anzElem].prod2_index = prod2_index;

		//Keine Lookaheads
		h->lr_str[h->anzElem].la = new Terminal( epsHashIndex );

		//Lookaheads eintragen
		h->lr_str[h->anzElem++].lalr = new Terminal( epsHashIndex );

		//ein weiterer Kern hinzugefügt?
		if( type == CORE )
			//Ja
			h->anzCore++;

		//Warnung?
		if( ( type == CORE ) && ( h->anzElem > h->anzCore ) )
			cerr << "sslr: Es wurde ein Kern eingetragen, obwohl schon alle Items berechnet sein sollten!\n";
	}
}


void lrparser::insertItem( HULLE_STR *h, long prod2_index, int dot, int type, Terminal *la )
{
	//LR(1)-Element in HULLE h eintragen
	LR_STR	*tmp;
	long	hInd;

	//Item schon enthalten?
	if( ( hInd = findItem( h, prod2_index, dot ) ) < 0 )
	{
		//Nein, Speicher für das LR(1)-Element
		tmp = new( LR_STR[h->anzElem + 1] );

		//kopieren
		memmove( tmp, h->lr_str, sizeof( LR_STR ) * h->anzElem );

		//Alten Speicher freigeben
		if( h->lr_str )
			delete[] ( h->lr_str );

		//Zeiger
		h->lr_str = tmp;

		//eintragen
		h->lr_str[h->anzElem].dot = dot;
		h->lr_str[h->anzElem].prod2_index = prod2_index;

		//Lookaheads eintragen
		h->lr_str[h->anzElem].la = new Terminal( epsHashIndex );
		h->lr_str[h->anzElem].la->add( la );

		//keine Lookahead-Terminale
		h->lr_str[h->anzElem++].lalr = 0L;

		//ein weiterer Kern hinzugefügt?
		if( type == CORE )
			//Ja
			h->anzCore++;

		//Warnung?
		if( ( type == CORE ) && ( h->anzElem > h->anzCore ) )
			cerr << "sslr: Es wurde ein Kern eingetragen, obwohl schon alle Items berechnet sein sollten!\n";
	}

	else
	{
		//Item schon in der Hülle enthalten, also nur die Lookaheads la hinzufügen
		tmp = &h->lr_str[hInd];

		//Lookaheads hinzufügen
		tmp->la->add( la );
	}
}


int lrparser::cmpLR0Hulle( HULLE_STR *h )
{
	//Prüft, ob die Hülle h schon in hulle_str enthalten ist
	HULLE_STR	*tmp;
	LR_STR		*lr;
	int			i, j, elemCnt = 0;
	int			anzCore = h->anzCore, cInd;

	//Alle Hüllen
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zähler
		elemCnt = 0;

		//Zeiger auf Hülle
		tmp = hulle_str[i];

		//Anzahl der Items gleich?
		if( ( h->anzElem == tmp->anzElem ) && ( h->anzCore == tmp->anzCore ) )
		{
			//Alle LR(0)-Kerne von tmp
			for( j = 0; j < tmp->anzCore; j++ )
			{
				//Zeiger auf LR(0)-Item
				lr = &tmp->lr_str[j];

				//Item in h enthalten?
				if( ( cInd = findItem( h, lr->prod2_index, lr->dot ) ) < 0 )
					//Nein
					break;

				//Ist es ein Kern-Item von h?
				else if( cInd <= ( anzCore - 1 ) )
					//Ja
					elemCnt++;
			}

			//gefunden?
			if( elemCnt == anzCore )
				//Ja
				break;
		}
	}

	//h noch nicht in hulle_str enthalten
	return( anzCore == elemCnt ? i : -1 );
}


int lrparser::cmpLR1Hulle( HULLE_STR *h )
{
	//Prüft, ob die Hülle h schon in hulle_str enthalten ist
	HULLE_STR	*tmp;
	LR_STR		*lr;
	int			i, j, elemCnt = 0;
	int			anzCore = h->anzCore, cInd;

	//Alle Hüllen
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zähler
		elemCnt = 0;

		//Zeiger auf Hülle
		tmp = hulle_str[i];

		//Anzahl der Items gleich?
		if( ( h->anzElem == tmp->anzElem ) && ( h->anzCore == tmp->anzCore ) )
		{
			//Alle LR(1)-Kerne
			for( j = 0; j < tmp->anzCore; j++ )
			{
				//Zeiger auf LR(0)-Item
				lr = &tmp->lr_str[j];

				//Item in h enthalten?
				if( ( cInd = findItem( h, lr->prod2_index, lr->dot, lr->la ) ) < 0 )
					//Nein
					break;

				//Ist es ein Kern-Item von h?
				else if( cInd <= ( anzCore - 1 ) )
					//Ja
					elemCnt++;
			}

			//gefunden?
			if( elemCnt == anzCore )
				//Ja
				break;
		}
	}

	//h noch nicht in hulle_str enthalten
	return( anzCore == elemCnt ? i : -1 );
}


int lrparser::findItem( HULLE_STR *h, long prod2_index, int dot )
{
	//Prüft, ob in h der Kern schon enthalten ist
	LR_STR	*lr;
	int		i;

	//Alle bisherigen Elemente
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0)-Item
		lr = &h->lr_str[i];

		//gleich?
		if( ( prod2_index == lr->prod2_index ) && ( dot == lr->dot ) )
			//Ja
			return( i );
	}

	//Nicht enthalten
	return( -1 );
}


int lrparser::findItem( HULLE_STR *h, long prod2_index, int dot, Terminal *la )
{
	//Prüft, ob in h der LR(1)-Kern schon enthalten ist
	LR_STR	*lr;
	int		i;

	//Alle bisherigen Elemente
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0)-Item
		lr = &h->lr_str[i];

		//gleich?
		if( ( prod2_index == lr->prod2_index ) && ( dot == lr->dot ) && ( lr->la->cmp( la ) ) )
			//Ja
			return( i );
	}

	//Nicht enthalten
	return( -1 );
}


void lrparser::outputHulle ( HULLE_STR *h )
{
	//Eine Hülle ausgeben
	Terminal	*la;
	PROD2_STR	*p2;
	PROD_STR	*p1;
	LR_STR		*lr;
	char		*name = "";
	long		*tArr, *nArr, elem, LRTabIndex;
	int			i, r, v, anz, typ;
	bool		draw;

	//Namen der Produktion holen
	name = hash_tab->getName( h->elem );

	//erster name ist leer
	if( name )
		//Ausgeben
		cout << "\n\nHülle " << h->hulleIndex << " = Sprung( " << h->from << ", " << name << " )\n";

	else
		//ohne "Vererbung"
		cout << "\n\nHülle " << h->hulleIndex << "\n";

	//Alle LR(0)-Elemente der Hülle
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0)-Element
		lr = &h->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Zeiger auf PROD_STR
		p1 = &prod_str[p2->prod_index];

		//Kern?
		if( i < h->anzCore )
			//Ja
			cout << "Kern:\t";

		else
			//Kein Kern-Element
			cout << "     \t";

		//Namen der Produktion holen
		name = hash_tab->getName( p1->hash_index );

		//ausgeben
		cout << " " << name << " -> ";

		//rechte Seite ausgeben
		for( r = 0; r < p2->riAnz; r++ )
		{
			//Position des Punktes?
			if( lr->dot == r )
				//Ja
				cout << ".";

			//Namen holen
			name = hash_tab->getName( p2->right[r] );

			//Terminal?
			if( hash_tab->getType( p2->right[r] ) == SSLR_TERM )
				//Ja
				cout << " '" << name << "' ";

			else
				//Nichtterminal
				cout << " " << name << " ";
		}

		//Position des Punktes?
		if( lr->dot == r )
			//Ja
			cout << ".";

		//Lookaheads ausgeben?
		if( lr->la )
		{
			//Ja, Lookaheads ausgeben
			la = lr->la;

			//Anzahl der Lookaheads
			anz = lr->la->getAnzElem();

			//Alle Lookaheads ausgeben
			for( v = 0; v < anz; v++ )
				//Terminal ausgeben
				cout << "," << hash_tab->getName( (*la)[v] );
		}

		//nächste Zeile
		cout << "\n";
	}

	//Alle Sprung-Übergänge nur einmal zeigen
	tArr = new( long[termArrCount + 1] );
	nArr = new( long[ntermArrCount] );

	//initialisieren
	memset( tArr, 0, sizeof( long ) * ( termArrCount + 1 ) );
	memset( nArr, 0, sizeof( long ) * ( ntermArrCount ) );

	//Alle LR(0)-Elemente der Hülle
	for( i = 0; i < h->anzElem; i++ )
	{
		//Zeiger auf LR(0)-Element
		lr = &h->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Zeiger auf PROD_STR
		p1 = &prod_str[p2->prod_index];

		//Position des Punktes?
		if( lr->dot < p2->riAnz )
		{
			//Sprung-Hülle ausgeben
			elem = p2->right[lr->dot];

			//Index des Element in der LR-Tabelle
			LRTabIndex = hash_tab->getLRTabIndex( elem );

			//Namen holen
			name = hash_tab->getName( elem );

			//Typ holen
			typ = hash_tab->getType( elem );

			//Terminal?
			if( typ == SSLR_TERM )
			{
				//hinter dem Punkt folgt ein Terminal
				if( tArr[LRTabIndex] )
					//wurde schon ausgegeben
					draw = false;

				else
				{
					//ausgeben
					draw = true;

					//markieren, damit es nicht mehr ausgegeben wird
					tArr[LRTabIndex]++;
				}
			}

			else
			{
				//hinter dem Punkt folgt ein Nichtterminal
				if( nArr[LRTabIndex] )
					//wurde schon ausgegeben
					draw = false;

				else
				{
					//ausgeben
					draw = true;

					//markieren, damit es nicht mehr ausgegeben wird
					nArr[LRTabIndex]++;
				}
			}

			//Epsilon?
			if( ( draw == true ) && ( p2->right[lr->dot] != epsHashIndex ) )
				//Jump ausgeben
				cout << "Sprung(" << name << ") = " << lr->jumpTo << "\n";
		}
	}

	//Speicher freigeben
	delete[] ( tArr );
	delete[] ( nArr );

	//nächste Zeile
	cout << "\n\n";
}


void lrparser::insertHulle( HULLE_STR *h )
{
	//Eine Hülle in die Sammlung von Elementen eintragen.
	HULLE_STR	**tmp;

	//Hülle schon eingetragen?
//	if( cmpLR0Hulle( h ) == -1 )
	{
		//Speicher anfordern
		tmp = new( HULLE_STR *[hulle_anz + 1] );

		//kopieren
		memmove( tmp, hulle_str, sizeof( HULLE_STR * ) * hulle_anz );

		//Alten Speicher freigeben
		if( hulle_str )
			delete[] ( hulle_str );

		//Zeiger
		hulle_str = tmp;

		//eintragen
		hulle_str[hulle_anz] = h;
		h->hulleIndex = hulle_anz++;

		//Anzahl der Kerne
		coreAnz += h->anzCore;
	}
}


void lrparser::genLALR( void )
{
	//Aus der Sammlung der LR(0)-Elemente die Lookaheads anfügen.
	HULLE_STR	*h, *checkHulle, *hTmp, *hTmp2;
	PROD2_STR	*p2;
	Terminal	*checkTerm, *la;
	LR_STR		*lr, *lr2, *lr3;
	long		elem;
	char		*name;
	int			i, j, k, v, hInd, cInd, laAnz, changed;
	int			pass = 0;

	//Check-Terminal # (s. Drache S. 295)
	checkTerm = new Terminal( epsHashIndex );
	checkTerm->ins( lalrHashIndex );

	/******************************************/
	/* spontan generierte Lookaheads erzeugen */
	/******************************************/


	//Alle Hüllen, I
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zeiger auf die Hülle
		h = hulle_str[i];

		//für jeden Kern, B -> gamma .delta in K
		for( j = 0; j < h->anzCore; j++ )
		{
			//Zeiger auf das LR(0)-Item
			lr = &h->lr_str[j];

			//neue Hülle anfordern, erzeuge J' := Hülle({[B -> gamma .delta, #]});
			checkHulle = createHulle();

			//Diesen Kern mit Check-Terminal # in checkHulle eintragen
			insertItem( checkHulle, lr->prod2_index, lr->dot, CORE, checkTerm );

			//Hülle bilden
			genLR1Items( checkHulle );

			//Hülle ausgeben
//			outputHulle( checkHulle );

			//Alle LR(1)-Elemente dieser Hülle
			for( k = 0; k < checkHulle->anzElem; k++ )
			{
				//Zeiger auf das LR(1)-Item, [A -> alpha .X beta, a]
				lr2 = &checkHulle->lr_str[k];

				//Zeiger auf die Terminale
				la = lr2->la;

				//Alle Terminale
				for( v = 0; v < la->getAnzElem(); v++ )
				{
					//lalrHashIndex?
					if( (*la)[v] == lalrHashIndex )
						//Ja, Lookaheads breiten sich aus; werden später behandelt
						continue;

					//Zeiger auf die Alternative
					p2 = &prod2_str[lr2->prod2_index];

					//Position des Punktes < maximale Position?
					if( lr2->dot < p2->riAnz )
					{
						//.index holen
						elem = p2->right[lr2->dot];

						//darf nicht Epsilon sein
						if( elem != epsHashIndex )
						{
							//Namen holen
							name = hash_tab->getName( elem );

							//Das LR(0)-Item k in h suchen
							cInd = findItem( h, lr2->prod2_index, lr2->dot );

							//Kern nicht gefunden?
							if( cInd < 0 )
							{
								//Kern nicht gefunden
								cerr << "sslr: lrparser::genLALR: Fatal internal error!\n";

								//Ende
								exit( 1 );
							}

							//Zeiger auf Item
							lr3 = &h->lr_str[cInd];

							//Index der Sprung-Hülle
							hInd = lr3->jumpTo;

							//Hülle nicht gefunden?
							if( hInd < 0 )
							{
								//Hülle nicht gefunden
								cerr << "sslr: lrparser::genLALR: Fatal internal error!\n";

								//Ende
								exit( 1 );
							}

							//Zeiger auf Hülle
							hTmp2 = hulle_str[hInd];

							//Kern k in hTmp2 finden
							cInd = findItem( hTmp2, lr2->prod2_index, lr2->dot + 1 );

							//Kern nicht gefunden?
							if( cInd < 0 )
							{
								//Kern nicht gefunden
								cerr << "sslr: lrparser::genLALR: Fatal internal error!\n";

								//Ende
								exit( 1 );
							}

							//Zeiger auf LR(1)-Item, A -> alpha X .beta in Sprung(I,X)
							lr3 = &hTmp2->lr_str[cInd];

							//spontan generierte Lookaheads aufnehmen
							lr3->lalr->add( la );

							//das checkTerminal löschen
							lr3->lalr->del( lalrHashIndex );

							//Speicher freigeben
//							delete( hTmp );
						}
					}
				}
			}

			//Speicher wieder freigeben
			deleteHulle( checkHulle );
		}
	}

/*
	//Alle Hüllen
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zeiger auf die Hülle
		h = hulle_str[i];

		//Alle Kerne
		for( j = 0; j < h->anzCore; j++ )
		{
			//Zeiger auf das LR(0)-Item
			lr = &h->lr_str[j];

			//Meldung
			cout << "\nKern " << j << ": spontan generierte Lookaheads für " << i << ":\n";

			//spontan generierte Lookaheads ausgeben
			outputTerminal( lr->lalr );

			//Absatz
			cout << "\n";
		}
	}
*/

	/********************************************/
	/* spontan generierte Lookaheads verbreiten */
	/********************************************/

	//Im ersten Kern ist ACCEPT ein spontan generiertes Lookahead
	hulle_str[0]->lr_str[0].lalr->ins( acceptHashIndex );


	//Ab hier sollten alle spontan generierten Lookaheads verfügbar sein,
	//nun auf die übrigen Kerne verbreiten.

	do
	{
		//keine Änderungen
		changed = 0;

		//Anzahl der Pässe
		pass++;

		//Alle Hüllen, I
		for( i = 0; i < hulle_anz; i++ )
		{
			//Zeiger auf die Hülle
			h = hulle_str[i];

			//Alle Kerne dieser Hülle, B -> gamma .delta in K
			for( j = 0; j < h->anzCore; j++ )
			{
				//Zeiger auf das LR(0)-Item des Kerns
				lr = &h->lr_str[j];

				//Position des Punktes < maximale Position?
				if( lr->dot < prod2_str[lr->prod2_index].riAnz )
				{
					//Hülle erzeugen, erzeuge J' := Hülle({[B -> gamma .delta, #]});
					hTmp = createHulle();

					//Hülle erzeugen, wo die Terminale hinvererbt werden
					insertItem( hTmp, lr->prod2_index, lr->dot, CORE, checkTerm );

					//alle Items erzeugen
					genLR1Items( hTmp );

					//Hülle ausgeben
//					outputHulle( hTmp );

					/*
						Nun alle Elemente dieser Hülle untersuchen, die # als Lookahead haben.
						Von diesen Elementen die Sprung-Hüllen bilden und an die Kerne
						die Looakheads verbreiten.
					*/
					for( k = 0; k < hTmp->anzElem; k++ )
					{
						//Zeiger auf Item, [A -> alpha .X beta, #]
						lr2 = &hTmp->lr_str[k];

						//Hat dieses Element # als Lookahead?
						if( lr2->la->find( lalrHashIndex ) )
						{
							//Ja, die Sprung-Hülle dieses Elements bilden
							p2 = &prod2_str[lr2->prod2_index];

							//Punkt am Ende?
							if( lr2->dot < p2->riAnz )
							{
								//.index holen
								elem = p2->right[lr2->dot];

								//darf nicht Epsilon sein
								if( elem != epsHashIndex )
								{
									//Namen holen
									name = hash_tab->getName( elem );

									//Das LR(0)-Item k in h suchen
									cInd = findItem( h, lr2->prod2_index, lr2->dot );

									//Kern nicht gefunden?
									if( cInd < 0 )
									{
										//Kern nicht gefunden
										cerr << "sslr: lrparser::genLALR: Fatal internal error!\n";

										//Ende
										exit( 1 );
									}

									//Zeiger auf Item
									lr3 = &h->lr_str[cInd];

									//Index der Spring-Hülle
									hInd = lr3->jumpTo;

									//Hülle nicht gefunden?
									if( hInd < 0 )
									{
										//Hülle nicht gefunden
										cerr << "sslr: lrparser::genLALR: Fatal internal error!\n";

										//Ende
										exit( 1 );
									}

									//Zeiger auf die Hülle
									hTmp2 = hulle_str[hInd];

									//Kern k in hTmp2 finden
									cInd = findItem( hTmp2, lr2->prod2_index, lr2->dot + 1 );

									//Kern nicht gefunden?
									if( cInd < 0 )
									{
										//Kern nicht gefunden
										cerr << "sslr: lrparser::genLALR: Fatal internal error!\n";

										//Ende
										exit( 1 );
									}

									//Zeiger auf Item, A -> alpha X .beta in Sprung(I,X)
									lr3 = &hTmp2->lr_str[cInd];

									//Anzahl der Terminale
									laAnz = lr3->lalr->getAnzElem();

									//Lookaheads hinzufügen
									lr3->lalr->add( lr->lalr );

									//# löschen
									lr3->lalr->del( lalrHashIndex );

									//Wurde etwas hinzugefügt?
									if( laAnz != lr3->lalr->getAnzElem() )
										//Ja
										changed++;
								}
							}
						}
					}

					//Speicher freigeben
					deleteHulle( hTmp );
				}
			}
		}

/*
		//Alle Hüllen
		for( i = 0; i < hulle_anz; i++ )
		{
			//Zeiger auf die Hülle
			h = hulle_str[i];

			//Alle Kerne
			for( j = 0; j < h->anzCore; j++ )
			{
				//Zeiger auf das LR(0)-Item
				lr = &h->lr_str[j];

				//Meldung
				cout << "\nspontan generierte Lookaheads für " << i << ":\n";

				//spontan generierte Lookaheads ausgeben
				outputTerminal( lr->lalr );

				//Absatz
				cout << "\n";
			}
		}
*/


	}while( changed );

	//Meldung?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: " << pass << " pass(es) to generate LALR-Items!\n";

	//Speicher für Check-Terminal # freigeben(s. Drache S. 295)
	delete( checkTerm );


	//Überall die Hüllen neu generieren
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zeiger auf die Hülle
		h = hulle_str[i];

		//Hülle ausgeben
//		outputHulle( h );

		//Alle Kerne
		for( j = 0; j < h->anzCore; j++ )
		{
			//Zeiger auf das LR(0)-Item
			lr = &h->lr_str[j];

			//Alten Speicher freigeben
			delete( lr->la );

			//Lookaheads eintragen
			lr->la = lr->lalr;

			//zurücksetzen
			lr->lalr = 0L;
		}

		//Hüllen erzeugen
		genLR1Items( h );
	}

/*
	//Alle Hüllen
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zeiger auf die Hülle
		h = hulle_str[i];

		//Alle Kerne
		for( j = 0; j < h->anzCore; j++ )
		{
			//Zeiger auf das LR(0)-Item
			lr = &h->lr_str[j];

			//Meldung
			cout << "\nspontan generierte Lookaheads für " << i << ":\n";

			//spontan generierte Lookaheads ausgeben
			outputTerminal( lr->lalr );

			//Absatz
			cout << "\n";
		}
	}
*/
}


void lrparser::genLR0Jump( HULLE_STR *I0, HULLE_STR *I1, long index )
{
	/*
		Bsp:
			expr	->	.expr ADD expr
					|	.expr SUB expr
					|	.term
					;

		Hier werden also die ersten beiden Elemente bei index = expr bzw.
		das letzte bei index = term hinzugefügt.
	*/
	PROD2_STR	*p2;
	LR_STR		*lr;
	char		*name, *tmpName;
	int			i;

	//Name von index
	name = hash_tab->getName( index );

	//Alle Elemente von I0
	for( i = 0; i < I0->anzElem; i++ )
	{
		//Zeiger auf LR-Item
		lr = &I0->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Punkt em Ende?
		if( lr->dot < p2->riAnz )
		{
			//Name von Element nach Punkt
			tmpName = hash_tab->getName( p2->right[lr->dot] );

			//Ja, index nach Punkt, d.h. .expr?
			if( p2->right[lr->dot] == index )
				//Ja, als Kern in I1 eintragen
				insertItem( I1, lr->prod2_index, lr->dot + 1, CORE );
		}
	}

	//Alle LR(0)-Elemente dieser Hülle erzeugen
	genLR0Items( I1 );
}


void lrparser::genLR1Jump( HULLE_STR *I0, HULLE_STR *I1, long index )
{
	/*
		Bsp:
			expr	->	.expr ADD expr
					|	.expr SUB expr
					|	.term
					;

		Hier werden also die ersten beiden Elemente bei index = expr bzw.
		das letzte bei index = term hinzugefügt.
	*/
	PROD2_STR	*p2;
	LR_STR		*lr;
	char		*name;
	int			i;

	//Name von index
	name = hash_tab->getName( index );

	//Alle Elemente von I0
	for( i = 0; i < I0->anzElem; i++ )
	{
		//Zeiger auf LR-Item
		lr = &I0->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Punkt em Ende?
		if( lr->dot < p2->riAnz )
		{
			//Ja, index nach Punkt, d.h. .expr?
			if( p2->right[lr->dot] == index )
				//Ja, als Kern in I1 eintragen
				insertItem( I1, lr->prod2_index, lr->dot + 1, CORE, lr->la );
		}
	}

	//Alle LR(1)-Elemente dieser Hülle erzeugen
	genLR1Items( I1 );
}


void lrparser::outputTerminal( Terminal *la )
{
	//Terminale ausgeben
	int	i, anz;

	//Anzahl der Terminale
	anz = la->getAnzElem();

	//Alle Terminale ausgeben
	for( i = 0; i < anz; i++ )
		//ausgaben
		cout << hash_tab->getName( (*la)[i] ) << ", ";
}


void lrparser::setJump( HULLE_STR *I0, long elem, long HIindex )
{
	/*
		Bsp:
			expr	->	.expr ADD expr
					|	.expr SUB expr
					|	.term
					;

		Hier werden also die ersten beiden Elemente bei index = expr bzw.
		das letzte bei index = term hinzugefügt.
	*/
	PROD2_STR	*p2;
	LR_STR		*lr;
	char		*name;
	int			i;

	//Name von index
	name = hash_tab->getName( elem );

	//Alle Elemente von I0
	for( i = 0; i < I0->anzElem; i++ )
	{
		//Zeiger auf LR-Item
		lr = &I0->lr_str[i];

		//Zeiger auf PROD2_STR
		p2 = &prod2_str[lr->prod2_index];

		//Punkt am Ende?
		if( lr->dot < p2->riAnz )
		{
			//Ja, index nach Punkt, d.h. .expr?
			if( p2->right[lr->dot] == elem )
				//Ja, den Index der Hülle eintragen, die durch Sprung( I0, elem ) erzeugt wird
				lr->jumpTo = HIindex;
		}
	}
}


bool lrparser::cmp2LR0Hulle( HULLE_STR *a, HULLE_STR *b )
{
	//Hier werden die LR(0)-Kerne beider Hüllen verglichen
	LR_STR	*lr;
	int		i, cInd;

	//Kerne gleich?
	if( ( a->anzElem == b->anzElem ) && ( a->anzCore == b->anzCore ) )
	{
		//Ja, gleich
		for( i = 0; i < a->anzCore; i++ )
		{
			//Zeiger auf Kern von a
			lr = &a->lr_str[i];

			//Ist dieses Item in b?
			if( ( cInd = findItem( b, lr->prod2_index, lr->dot ) ) < 0 )
				//Nein
				return( false );

			//Ist es ein Kern?
			else if( cInd > ( a->anzCore - 1 ) )
				//Ist kein Kern
				return( false );
		}

		//gleich
		return( true );
	}

	//nicht gleich
	return( false );
}


void lrparser::LR2LALR( void )
{
	//Wandelt die LR(1)-Hüllen in LR(0)-Hüllen
	HULLE_STR	*a, *b, **tmp;
	LR_STR		*lr;
	int			i, j, k;

	//Meldung?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: Making LR to LALR...\n";

	//Alle Hüllen
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zeiger auf Hülle
		a = hulle_str[i];

		//Alle anderen Hüllen mit dieser vergleichen
		for( j = i + 1; j < hulle_anz; j++ )
		{
			//Zeiger auf Hülle
			b = hulle_str[j];

			//beiden Hüllen vergleichen
			if( cmp2LR0Hulle( a, b ) == true )
			{
				//Meldung?
				if( argv_str->verbose )
					//Ja
					cout << "Closure " << i << " and " << j << " equal...\n";

				//Die Terminale der Kerne von b in a eintragen, dann b löschen
				for( k = 0; k < b->anzCore; k++ )
				{
					//Zeiger auf Kern
					lr = &b->lr_str[k];

					//Kern in a eintragen
					insertItem( a, lr->prod2_index, lr->dot, CORE, lr->la );
				}

				//Hülle nochmal bilden, da Lookaheads möglicherweise anders
				genLR1Items( a );

				//Den Speicher der Hülle freigeben
				delHulle( b );

				//Speicher für alle Hüllen
				tmp = new( HULLE_STR *[hulle_anz - 1] );

				//Hüllen kopieren
				memmove( tmp, hulle_str, sizeof( HULLE_STR * ) * j );
				memmove( &tmp[j], &hulle_str[j + 1], sizeof( HULLE_STR * ) * ( hulle_anz - j - 1 ) );

				//Hülle b löschen

				//eine Hülle weniger
				hulle_anz--;

				//Alten Speicher freigeben
				delete[] ( hulle_str );

				//Zeiger
				hulle_str = tmp;

				//Index erniedrigen
				j--;
			}
		}

		//Hüllennummerierung
		a->hulleIndex = i;
	}
}


void lrparser::LRTab( void )
{
	//LR-Tabelle erzeugen
	PROD2_STR	*p2;
	HULLE_STR	*h;
	PROD_STR	*p1;
	Terminal	*la;
	LR_STR		*lr;
	char		*name;
	long		elem;
	int			i, j, type, lrTabIndex;

	//Meldugn ausgeben?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: Generate LR-Table...\n";

	//Speicher für die LR-Tabelle
	lrTab = new( LR_TAB *[hulle_anz] );

	//Speicher für die Sprung-Tabelle
	jumpTab = new( int *[hulle_anz] );

	//Für jeden Zustand die Spalten anfordern
	for( i = 0; i < hulle_anz; i++ )
	{
		//Speicher für Spalte
		lrTab[i] = new( LR_TAB[termArrCount + 1] );

		//Speicher für Spalte
		jumpTab[i] = new( int[ntermArrCount] );

		//Initialisieren
		memset( lrTab[i], 0, sizeof( LR_TAB ) * termArrCount );
		memset( jumpTab[i], 0, sizeof( int ) * ntermArrCount );
	}


	//Alle Hüllen
	for( i = 0; i < hulle_anz; i++ )
	{
		//Zeiger auf die Hülle
		h = hulle_str[i];

		//Alle LR(0)/LR(1)-Elemente der Hülle
		for( j = 0; j < h->anzElem; j++ )
		{
			//Zeiger auf das LR(0)/LR(1)-Item
			lr = &h->lr_str[j];

			//Zeiger auf die Alternative
			p2 = &prod2_str[lr->prod2_index];

			//Zeiger auf die Alternative
			p1 = &prod_str[p2->prod_index];

			//Ist es ein Element der Art .expr?
			if( lr->dot < p2->riAnz )
			{
				//Element nach Punkt, hier z.B. expr
				elem = p2->right[lr->dot];

				//Index des Elements in der LR-Tabelle
				lrTabIndex = hash_tab->getLRTabIndex( elem );

				//Typ holen
				type = hash_tab->getType( elem );

				//Name holen
				name = hash_tab->getName( elem );

				//Nichtterminal vor Punkt?
				if( type == SSLR_NTERM )
				{
					//Ja, Eintrag in Sprung-Hülle
					jumpTab[i][lrTabIndex] = lr->jumpTo;

					//Meldung?
					if( argv_str->verbose )
						//Ja
						cout << "sslr: LRTab: jump[" << i << "][" << name << "] = " << lr->jumpTo << "...\n";
				}

				else if( elem == epsHashIndex )
				{
					//Spezial-Fall, reduzieren

					//SLR-Parser?
					if( argv_str->type == SLR )
						//Follow-Menge
						la = hash_tab->getFollow( p1->hash_index );

					else
						//bei LALR und LR sind es die Lookaheads
						la = lr->la;

					//Reduzieren eintragen
					setReduce( i, lr->prod2_index, la );
				}

				else
					//Terminal schieben
					setShift( i, elem, lr->jumpTo );

			}

			else
			{
				//Reduce-Zustand

				//SLR-Parser?
				if( argv_str->type == SLR )
					//Follow-Menge
					la = hash_tab->getFollow( p1->hash_index );

				else
					//bei LALR und LR sind es die Lookaheads
					la = lr->la;

				//Reduzieren eintragen
				setReduce( i, lr->prod2_index, la );
			}
		}
	}

	//ltTab[1][ACCEPT] = Akzeptiere
	lrTab[1][termArrCount].state = LR_ACCEPT;

	//Shift-Reduce-Fehler aufgetreten?
	if( srError )
		//Meldung
		cerr << "sslr: " << srError << " shift-reduce error(s) occured!\n";

	//Reduce-Reduce-Fehler aufgetreten?
	if( rrError )
		//Meldung
		cerr << "sslr: " << rrError << " reduce-reduce error(s) occured!\n";
}


void lrparser::setReduce( int hInd, long prod2_index, Terminal *la )
{
	//Einen Reduce-Eintrag erzeugen und dabei Reduce-Reduce- und Shift-Reduce-Fehler behandeln.
	PROD2_STR	*p2, *p3, *p4;
	PROD_STR	*p1;
	long		elem;
	char		*name;
	int			i, laAnz, lrTabIndex;
	int			tAssoc, pPriority, tPriority, state;

	//Hier mit der Produktion prod2_index reduzieren
//	h = hulle_str[hInd];

	//Zeiger auf die Alternative
	p2 = &prod2_str[prod2_index];

	//Zeiger auf die Alternative
	p1 = &prod_str[p2->prod_index];

	//Anzahl der Lookaheads
	laAnz = la->getAnzElem();

	//Für jedes Lookahead
	for( i = 0; i < laAnz; i++ )
	{
		//nächstes Lookahead holen
		elem = (*la)[i];

		//Namen
		name = hash_tab->getName( elem );

		//Spalten-Index in LR-Tabelle holen
		lrTabIndex = hash_tab->getLRTabIndex( elem );

		//Ist dieser Eintrag in der Hash-Tabelle schon belegt?
		if( lrTab[hInd][lrTabIndex].state == LR_REDUCE )
		{
			//Reduce-Reduce-Konflikt

			//Zeiger auf die PROD2_STR
			p3 = &prod2_str[lrTab[hInd][lrTabIndex].value];
			p4 = &prod2_str[prod2_index];

			//Ist ein "echter" Reduce-Reduce-Konflikt
			cout << "sslr: Reduce-Reduce conflict in closure " << hInd << " between\n";

			//eingetragene Produktion ausgeben
			outputAlt( cout, lrTab[hInd][lrTabIndex].value );

			cout << "and\n";

			//eingetragene Produktion ausgeben
			outputAlt( cout, prod2_index );

			//neue Zeile
			cout << "\n";

			//Fehler
			rrError++;
		}

		else if( lrTab[hInd][lrTabIndex].state == LR_SHIFT )
		{
			//Shift-Reduce-Fehler

			//Fehler ausgeben
			cout << "sslr: Shift-Reduce conflict in closure " << hInd << " between reducing\n";

			//Alternative ausgeben, mit der reduziert werden soll
			outputAlt( cout, prod2_index ),

			//Schieben mit Terminal
			name = hash_tab->getName( lrTab[hInd][lrTabIndex].termHashIndex );
			cout << "and shifting " << name << "!\n\n";

			//Priorität des geschobenen Terminals
			tPriority = hash_tab->getPriority( lrTab[hInd][lrTabIndex].termHashIndex );

			//Priorität der Produktion holen
			pPriority = p2->priority;

			//Assoziation des Terminals holen
			tAssoc = hash_tab->getAssociation( lrTab[hInd][lrTabIndex].termHashIndex );

			//Wurde die Priorität überhaupt angegeben?
			if( ( tPriority > 0 ) && ( pPriority > 0 ) )
			{
				//Ja, Priorität der Produktion > Priorität des Terminals?
				if( pPriority > tPriority )
					//Ja, reduzieren
					state = LR_REDUCE;

				else if( ( tPriority == pPriority ) && ( tAssoc == isLeftAssoc ) )
					//beide Prioritäten gleich, zu schiebendes Terminal ist links-assoziativ
					state = LR_REDUCE;

				//nicht-assoziatives Terminal?
				else if( ( tPriority == pPriority ) && ( tAssoc == isNonAssoc ) )
				{
					//Ja, Fehlereintrag
					lrTab[hInd][lrTabIndex].state = 0;
					lrTab[hInd][lrTabIndex].value = 0;

					//ausgeben
					state = LR_ERROR;
				}

				else
					//nichts von alledem
					state = LR_SHIFT;
			}

			else
				//Schieben
				state = LR_SHIFT;

			//Schieben oder reduzieren?
			if( state == LR_REDUCE )
			{
				//Reduzieren
				lrTab[hInd][lrTabIndex].state = LR_REDUCE;
				lrTab[hInd][lrTabIndex].value = prod2_index;

				//Meldung
				cout << "reduce...\n";

				//merken, dass reduziert wird
				prod2_str[prod2_index].reduce--;

				//Meldung ausgeben?
				if( argv_str->verbose )
				{
					//Ja
//					cout << "sslr: LRTab: lrTab[" << hInd << "][" << name << "] = reduce " << hash_tab->getName( p1->hash_index ) << "...\n";
					cout << "sslr: LRTab: lrTab[" << hInd << "][" << name << "] = reduce with ";

					//rechte Seite ausgeben
					outputAlt( cout, prod2_index );
				}
			}

			else if( state == LR_SHIFT )
				//geschobenen beibehalten
				cout << "shift...\n\n";

			else if( state == LR_ERROR )
				//Terminal ist nicht-assoziativ
				cout << "Terminal is non-associative!\n";

			//Fehler
			srError++;
		}

		else
		{
			//kein Fehler aufgetreten
			lrTab[hInd][lrTabIndex].state = LR_REDUCE;
			lrTab[hInd][lrTabIndex].value = prod2_index;

			//merken, dass reduziert wird
			prod2_str[prod2_index].reduce++;

			//Meldung ausgeben?
			if( argv_str->verbose )
			{
				//Ja
//				cout << "sslr: LRTab: lrTab[" << hInd << "][" << name << "] = reduce " << hash_tab->getName( p1->hash_index ) << "...\n";
				cout << "sslr: LRTab: lrTab[" << hInd << "][" << name << "] = reduce with ";

				//rechte Seite ausgeben
				outputAlt( cout, prod2_index );
			}
		}
	}
}


void lrparser::setShift( int hInd, long termHashIndex, int jumpTo )
{
	//Einen Shift-Eintrag erzeugen und dabei Shift-Reduce-Fehler behandeln.
	PROD2_STR	*p2;
	PROD_STR	*p1;
	long		prod2_index;
	char		*name;
	int			lrTabIndex;
	int			tAssoc, pPriority, tPriority, state;

	//Spalten-Index in LR-Tabelle holen
	lrTabIndex = hash_tab->getLRTabIndex( termHashIndex );

	//Name holen
	name = hash_tab->getName( termHashIndex );

	//Ist dieser Eintrag in der Hash-Tabelle schon belegt?
	if( lrTab[hInd][lrTabIndex].state == LR_REDUCE )
	{
		//Shift-Reduce-Fehler

		//Index der Alternative in PROD2_STR
		prod2_index = lrTab[hInd][lrTabIndex].value;

		//Zeiger auf die Alternative
		p2 = &prod2_str[prod2_index];

		//Zeiger auf die Alternative
		p1 = &prod_str[p2->prod_index];

		//Fehler ausgeben
		cout << "sslr: Shift-Reduce conflict in closure " << hInd << " between reducing\n";

		//Alternative ausgeben, mit der reduziert werden soll
		outputAlt( cout, prod2_index ),

		//Schieben mit Terminal
		cout << "and shifting " << name << "!\n";

		//Priorität des geschobenen Terminals
		tPriority = hash_tab->getPriority( termHashIndex );

		//Priorität der Produktion holen
		pPriority = p2->priority;

		//Assoziation des Terminals holen
		tAssoc = hash_tab->getAssociation( termHashIndex );

		//Wurde die Priorität überhaupt angegeben?
		if( ( tPriority > 0 ) && ( pPriority > 0 ) )
		{
			//Ja, Priorität der Produktion > Priorität des Terminals?
			if( pPriority > tPriority )
				//Ja, reduzieren
				state = LR_REDUCE;

			else if( ( tPriority == pPriority ) && ( tAssoc == isLeftAssoc ) )
				//beide Prioritäten gleich, Terminal ist links-assoziativ
				state = LR_REDUCE;

			//nicht-assoziatives Terminal?
			else if( ( tPriority == pPriority ) && ( tAssoc == isNonAssoc ) )
			{
				//Ja, Fehlereintrag
				lrTab[hInd][lrTabIndex].state = 0;
				lrTab[hInd][lrTabIndex].value = 0;

				//ausgeben
				state = LR_ERROR;
			}

			else
				//nichts von alledem
				state = LR_SHIFT;
		}

		else
			//Schieben
			state = LR_SHIFT;

		//Schieben oder reduzieren?
		if( state == LR_SHIFT )
		{
			//Schieben
			lrTab[hInd][lrTabIndex].state = LR_SHIFT;
			lrTab[hInd][lrTabIndex].value = jumpTo;
			lrTab[hInd][lrTabIndex].termHashIndex = termHashIndex;

			//merken, dass nicht reduziert, sondern geschoben wird
			prod2_str[prod2_index].reduce--;

			//Meldung
			cout << "shift...\n\n";

			//Meldung ausgeben?
			if( argv_str->verbose )
				//Ja
				cout << "sslr: LRTab: lrTab[" << hInd << "][" << name << "] = shift " << jumpTo << "...\n";
		}

		else if( state == LR_REDUCE )
			//Reduktion mit Alternative beibehalten
			cout << "reduce...\n\n";

		else if( state == LR_ERROR )
			//Terminal ist nicht-assoziativ
			cout << "Terminal is non-associative!\n";

		//Fehler
		srError++;
	}

	else
	{
		//Kein Fehler aufgetreten
		lrTab[hInd][lrTabIndex].state = LR_SHIFT;
		lrTab[hInd][lrTabIndex].value = jumpTo;
		lrTab[hInd][lrTabIndex].termHashIndex = termHashIndex;

		//Meldung ausgeben?
		if( argv_str->verbose )
			//Ja
			cout << "sslr: LRTab: lrTab[" << hInd << "][" << name << "] = shift " << jumpTo << "...\n";
	}
}


void lrparser::generateFiles( void )
{
	//Hier werden die Klassenfiles des Parsers erzeugt.
	ofstream	cppFile( "sslrparser.cpp" );
	ofstream	hFile( "sslrparser.h" );

	//Meldugn ausgeben?
	if( argv_str->verbose )
		//Ja
		cout << "sslr: Generate parser file...\n";

	//Das Header-File generieren
	outputHFile( hFile );

	//Den Header des cpp-Files ausgeben
	outputCPPHeader( cppFile );

	//Terminal-Defines als enum ausgeben
	outputCPPTerminal( cppFile );

	//etwaige Include-Anweisungen einfügen
	outputIncl( cppFile );

	//LR-Tabelle ausgeben
	outputLRTab( cppFile );

	//Sprung-Tabelle ausgeben
	outputJumpTab( cppFile );

	//Die Follow-Sets ausgeben
	outputFollow( cppFile );

	//Die rechten Seiten ausgeben
	outputAlternatives( cppFile );

	//alle NTERM_STR ausgeben
	outputNtermStr( cppFile );

	//Debug-Mode?
	if( argv_str->debug == true )
		//Ja
		outputTermName( cppFile );

	//Fehlerbehandlung ausgeben
	outputErrorRecovery( cppFile );

	//Alle übrigen Routinen ausgeben
	outputCPPEnd( cppFile );

	//semantische Aktionen ausgeben
	outputSemAct( cppFile );

	//etwaige eigene Erweiterungen einfügen
	outputExt( cppFile );
}


void lrparser::outputCPPHeader( ofstream &file )
{
	//Den Header des cpp-Files ausgeben

	//Header des cpp-Files
file << "/***************************************************************************\n";
file << "                          sslrparser.cpp  -  description\n";
file << "                             -------------------\n";
file << "    copyright            : (C) 2002 by Sven Schmidt\n";
file << "    email                : s.schmidt@lboro.ac.uk\n";
file << " ***************************************************************************/\n";
file << "\n";
file << "/***************************************************************************\n";
file << " *                                                                         *\n";
file << " *   This program is free software; you can redistribute it and/or modify  *\n";
file << " *   it under the terms of the GNU General Public License as published by  *\n";
file << " *   the Free Software Foundation; either version 2 of the License, or     *\n";
file << " *   (at your option) any later version.                                   *\n";
file << " *                                                                         *\n";
file << " ***************************************************************************/\n";
file << "\n";
file << "#include \"sslrparser.h\"\n";
file << "#include <iostream>\n";
file << "\n";
file << "\n";

	//Debug-Version?
	if( argv_str->debug == true )
	{
		//Debug-Version
		file << "/****************************\n";
		file << "* This is the debug version *\n";
		file << "****************************/\n\n\n";
	}

	else
	{
		//ist nicht die Debug-Version
		file << "/********************************\n";
		file << "* This is the non-debug version *\n";
		file << "********************************/\n\n\n";
	}
}


void lrparser::outputCPPEnd( ofstream &file )
{
	//alle übrigen Routinen des cpp-Files ausgeben

file << "\n";
file << "\n";
file << "SSLRParser::SSLRParser( SSLEXScanner *scan )\n";
file << "{\n";
file << "	//call reset to initialize the parser\n";
file << "	reset();\n";
file << "\n";
file << "	//scanner class\n";
file << "	SSLR_scanner = scan;\n";
file << "}\n";
file << "\n";
file << "\n";
file << "SSLRParser::~SSLRParser()\n";
file << "{\n";
file << "}\n";
file << "\n";
file << "\n";
file << "void SSLRParser::reset( void )\n";
file << "{\n";
file << "	//resets the whole parser\n";
file << "\n";
file << "	//parse stack pointer\n";
file << "	SSLR_sp = 0;\n";
file << "\n";
file << "	//start with state 0\n";
file << "	SSLR_push( 0 );\n";
file << "	SSLR_state = 0;\n";
file << "\n";
file << "	//no errors yet\n";
file << "	SSLR_anzError = 0;\n";
file << "\n";
file << "	//defualt values for error recovery\n";
file << "	SSLR_errorStr.alt = start_0;\n";
file << "	SSLR_errorStr.sp = SSLR_sp;\n";
file << "	SSLR_errorStr.state = 0;\n";
file << "\n";


	//semantische Aktionen?
	if( semActCnt && ( union_str->anz > 1 ) )
	{
		//Ja
		file << "	//semantic actions\n";
		file << "	SSLR_semAct = 0;\n";
		file << "\n";
		file << "	//stack counter for semantic action stack\n";
		file << "	SSLR_semStackCnt = 0;\n";
	}

file << "}\n";
file << "\n";
file << "\n";
file << "int SSLRParser::SSLR_getTerminal( void )\n";
file << "{\n";

	//semantische Aktionen?
	if( semActCnt && ( union_str->anz > 1 ) )
	{
		//Ja, Attribut des Scanners auf den Stack schieben
		file << "	//semantic actions allowed?\n";
		file << "	if( !SSLR_semAct )\n";
		file << "		//push attribute on stack for semantic actions\n";
		file << "		SSLR_semStack[SSLR_semStackCnt++] = SSLR_attributeStr;\n";
		file << "\n";
	}

file << "	//get next terminal\n";
file << "	SSLR_terminal = SSLR_scanner->yyLex( &SSLR_attributeStr );\n";
file << "\n";
file << "	//get line\n";
file << "	SSLR_line = SSLR_scanner->getYYLineno();\n";
file << "\n";
file << "	//return terminal\n";
file << "	return( SSLR_terminal );\n";
file << "}\n";
file << "\n";
file << "\n";
file << "int SSLRParser::yyParse( void )\n";
file << "{\n";
file << "	//start parsing\n";
file << "\n";
file << "	//get first terminal\n";
file << "	SSLR_terminal = SSLR_getTerminal();\n";
file << "\n";
file << "	//start parsing\n";
file << "	return( SSLR_parse() );\n";
file << "}\n";
file << "\n";
file << "\n";
file << "void SSLRParser::SSLR_push( long ast )\n";
file << "{\n";
file << "	//push a new state on the parse stack\n";
file << "	SSLR_stack[SSLR_sp++] = ast;\n";
file << "\n";
file << "	//stack overflow?\n";
file << "	if( SSLR_sp > SSLR_maxStackSize )\n";
file << "	{\n";
file << "		//stack-overflow\n";
file << "		cerr << \"SSLRParser::SSLR_push: stack overflow!\\nExiting...\\n\";\n";
file << "		exit( 1 );\n";
file << "	}\n";
file << "\n";
file << "	//set new state\n";
file << "	SSLR_state = ast;\n";
file << "}\n";
file << "\n";
file << "\n";
file << "long SSLRParser::SSLR_pop( int anz )\n";
file << "{\n";
file << "	//pop anz elements from the parse stack\n";
file << "	SSLR_sp -= anz;\n";
file << "\n";
file << "	//stack underflow?\n";
file << "	if( SSLR_sp < 0 )\n";
file << "	{\n";
file << "		//stack underflow\n";
file << "		cerr << \"SSLRParser::SSLR_pop: stack underflow!\\nExiting...\\n\";\n";
file << "		exit( 1 );\n";
file << "	}\n";
file << "\n";
file << "	//return new state\n";
file << "	return( SSLR_stack[SSLR_sp - 1] );\n";
file << "}\n";
file << "\n";
file << "\n";
file << "int SSLRParser::SSLR_parse( void )\n";
file << "{\n";
file << "	//begin parsing\n";
file << "\n";
file << "	while( 1 )\n";
file << "	{\n";
file << "		//end of parsing?\n";
file << "		if( SSLR_state == 1 )\n";
file << "		{\n";
file << "			//yes, is terminal ACCEPT\n";
file << "			if( SSLR_terminal == ACCEPT )\n";
file << "				//return number of errors\n";
file << "				return( SSLR_anzError );\n";
file << "\n";
file << "			else\n";
file << "			{\n";
file << "				//error\n";
file << "				cerr << \"SSLRParser::SSLR_parse: bailing out...\\n\";\n";
file << "				return( SSLR_anzError );\n";
file << "			}\n";
file << "		}\n";
file << "\n";
file << "		//shift terminal SSLR_terminal?\n";
file << "		else if( SSLR_lrTab[SSLR_state][SSLR_terminal].state == LR_SHIFT )\n";
file << "		{\n";
file << "			//yes\n";
file << "			SSLR_shift( &SSLR_lrTab[SSLR_state][SSLR_terminal] );\n";
file << "\n";
file << "			//get new terminal\n";
file << "			SSLR_terminal = SSLR_getTerminal();\n";
file << "		}\n";
file << "\n";
file << "		//reduce with production?\n";
file << "		else if( SSLR_lrTab[SSLR_state][SSLR_terminal].state == LR_REDUCE )\n";
file << "			//reduce\n";
file << "			SSLR_reduce( &SSLR_lrTab[SSLR_state][SSLR_terminal] );\n";
file << "\n";
file << "		//otherwise an error occured\n";
file << "		else\n";
file << "			//call error recovery\n";
file << "			SSLR_error();\n";
file << "	}\n";
file << "}\n";
	file << "\n";
	file << "\n";

	//Debug-Version
	if( argv_str->debug == true )
	{
		//Ja
		file << "void SSLRParser::SSLR_shift( SSLR_LR_TAB *tab )\n";
		file << "{\n";
		file << "	//shift a terminal onto the parse stack\n";

		//Debug-Makro ausgeben
		file << "\n#ifdef SSLR_DEBUG\n";

		file << "	//output\n";
		file << "	cout << \"Shift\\n\";\n";
		file << "	cout << \"current state: \" << SSLR_state << \"\\n\";\n";
		file << "	cout << \"current terminal: \" << SSLR_termName[SSLR_terminal] << \"\\n\";\n";

		//Debug-Makro ausgeben
		file << "#endif\n\n";

		file << "	//shift a new state onto the stack\n";
		file << "	SSLR_push( tab->value );\n";
		file << "\n";

		//Debug-Makro ausgeben
		file << "\n#ifdef SSLR_DEBUG\n";

		file << "	//output\n";
		file << "	cout << \"shift new state \" << SSLR_state << \"\\n\\n\";\n";

		//Debug-Makro ausgeben
		file << "#endif\n";

		file << "}\n";
		file << "\n";
		file << "\n";
		file << "void SSLRParser::SSLR_reduce( SSLR_LR_TAB *tab )\n";
		file << "{\n";
		file << "	//reduce with production\n";
		file << "	SSLR_NTERM_STR	*n;\n";
		file << "\n";

		//semantische Aktionen?
		if( semActCnt )
		{
			//Ja
			file << "	//perform a semantic action accociated with the reduced production\n";
			file << "	SSLR_doSemAct( tab );\n";
			file << "\n";
		}

		file << "	//get a pointer to the alternative\n";
		file << "	n = &SSLR_ntermStr[tab->value];\n";

		//Debug-Makro ausgeben
		file << "\n#ifdef SSLR_DEBUG\n";

		file << "	//output\n";
		file << "	cout << \"Reduce\\n\";\n";
		file << "	cout << \"current state: \" << SSLR_state << \"\\n\";\n";
		file << "	cout << \"current terminal: \" << SSLR_termName[SSLR_terminal] << \"\\n\";\n";
		file << "	cout << \"reduce with \";\n";
		file << "\n";
		file << "	//output alternative\n";
		file << "	SSLR_outputAlt( n );\n";
		file << "\n";
		file << "	//output\n";
		file << "	cout << \"\\n\";\n";

		//Debug-Makro ausgeben
		file << "#endif\n\n";

		file << "	//remove states form stack\n";
		file << "	SSLR_state = SSLR_pop( n->elem );\n";
		file << "\n";
		file << "	//get new state\n";
		file << "	SSLR_state = SSLR_jumpTab[SSLR_state][n->jump_index];\n";
		file << "\n";
		file << "	//push new state on stack\n";
		file << "	SSLR_push( SSLR_state );\n";


		//Debug-Makro ausgeben
		file << "\n#ifdef SSLR_DEBUG\n";

		file << "	//output\n";
		file << "	cout << \"new state \" << SSLR_state << \"\\n\\n\";\n";

		//Debug-Makro ausgeben
		file << "#endif\n\n";

		file << "	//for error recovery\n";
		file << "	SSLR_errorStr.alt = tab->value;\n";
		file << "	SSLR_errorStr.sp = SSLR_sp;\n";
		file << "	SSLR_errorStr.state = SSLR_state;\n";
		file << "}\n";
		file << "\n";

		//Debug-Makro ausgeben
		file << "\n#ifdef SSLR_DEBUG\n";

		file << "void SSLRParser::SSLR_outputAlt( SSLR_NTERM_STR *n )\n";
		file << "{\n";
		file << "	//output an alternative\n";
		file << "	int	i;\n";
		file << "\n";
		file << "	//name of alternative\n";
		file << "	cout << n->name << \" -> \";\n";
		file << "\n";
		file << "	//output right side\n";
		file << "	for( i = 0; i < n->elem; i++ )\n";
		file << "		cout << n->ri[i] << \" \";\n";
		file << "\n";
		file << "\t//EPSILON alternative?\n";
		file << "\tif( !n->elem )\n";
		file << "\t\t//yes\n";
		file << "\t\tcout << \"'EPSILON'\";\n";
		file << "}\n";

		//Debug-Makro ausgeben
		file << "#endif\n";
	}

	else
	{
		//ist nicht die Debug-Version
		file << "void SSLRParser::SSLR_shift( SSLR_LR_TAB *tab )\n";
		file << "{\n";
		file << "	//shift a new state onto the stack\n";
		file << "	SSLR_push( tab->value );\n";
		file << "}\n";
		file << "\n";
		file << "\n";
		file << "void SSLRParser::SSLR_reduce( SSLR_LR_TAB *tab )\n";
		file << "{\n";
		file << "	//reduce with production\n";
		file << "	SSLR_NTERM_STR	*n;\n";
		file << "\n";

		//semantische Aktionen?
		if( semActCnt )
		{
			//Ja
			file << "	//perform a semantic action accociated with the reduced production\n";
			file << "	SSLR_doSemAct( tab );\n";
			file << "\n";
		}

		file << "	//get a pointer to the alternative\n";
		file << "	n = &SSLR_ntermStr[tab->value];\n";
		file << "\n";
		file << "	//remove states form stack\n";
		file << "	SSLR_state = SSLR_pop( n->elem );\n";
		file << "\n";
		file << "	//get new state\n";
		file << "	SSLR_state = SSLR_jumpTab[SSLR_state][n->jump_index];\n";
		file << "\n";
		file << "	//push new state on stack\n";
		file << "	SSLR_push( SSLR_state );\n";
		file << "\n";
		file << "	//for error recovery\n";
		file << "	SSLR_errorStr.alt = tab->value;\n";
		file << "	SSLR_errorStr.sp = SSLR_sp;\n";
		file << "	SSLR_errorStr.state = SSLR_state;\n";
		file << "}\n";
		file << "\n";
		file << "\n";
	}
}


void lrparser::outputCPPTerminal( ofstream &file )
{
	//Terminal-Defines als enum ausgeben
	char	*name;
	int		i;

	//Header ausgeben
	file << "//Terminals for debugging\n";
	file << "char *SSLR_scannerToken[] =\n{\n";

	for( i = 0; i < termArrCount; i++ )
	{
		//Name des Terminals
		name = hash_tab->getName( termArr[i] );

		//Define-Anweisung ausgeben
		file << "\t/*" << i << "*/ \"" << name << "\",\n";
	}

	//Abschluss
	file << "\t0L\n};\n\n\n";
}


void lrparser::outputLRTab( ofstream &file )
{
	//LR-Tabelle in Datei ausgeben
	PROD2_STR	*p2;
	PROD_STR	*p1;
	LR_TAB		*lrtab;
	char		puf1[256+1], puf2[256+1], *name;
	int			i, j, k, id;


	//Überschrift
	file << "//syntax analyze table\n";

	//Definition der LR-Tabelle
	file << "SSLRParser::SSLR_LR_TAB SSLRParser::SSLR_lrTab[SSLRParser::SSLR_anzLRStates][SSLRParser::SSLR_anzTerm] = {\n";

	//Alle Zustände
	for( i = 0; i < hulle_anz; i++ )
	{
		//Öffnen
		file << "/* State " << i << " */\n{ ";

		//Alle Terminale
		for( j = 0, k = 0; j < termArrCount; j++, k++ )
		{
			//Zeiger auf LR_TAB-Struktur
			lrtab = &lrTab[i][j];

			//Zustand ausgeben
			switch( lrtab->state )
			{
				//Fehler?
				case LR_ERROR:
				{
					//Ja, Fehlerbehandlung
					sprintf( puf1, "%s", "LR_ERROR" );
					break;
				}

				//Shift?
				case LR_SHIFT:
				{
					//Ja, Zustand schieben
					sprintf( puf1, "%s", "LR_SHIFT" );
					break;
				}

				//Reduce?
				case LR_REDUCE:
				{
					//Ja, mit Produktion reduzieren
					sprintf( puf1, "%s", "LR_REDUCE" );

					//Produktion ausschreiben
					p2 = &prod2_str[lrtab->value];

					//Zeiger auf PROD_STR
					p1 = &prod_str[p2->prod_index];

					//Name der Produktion
					name = hash_tab->getName( p1->hash_index );

					//Nummer der Alternative
					id = lrtab->value - p1->prod2_index;

					//Namen generieren
					sprintf( puf2, "%s_%d", name, id );

					break;
				}

				//Akzeptieren?
				case LR_ACCEPT:
				{
					//Ja, akzeptieren
					sprintf( puf1, "%s", "LR_ACCEPT" );
					break;
				}

				default:
				{
					//anderer Wert
					sprintf( puf1, "%d", lrTab[i][j].state );
					break;
				}
			}

			//Werte-Paar ausgeben
			file << "{ " << puf1 << ", ";

			//Reduce?
			if( lrtab->state == LR_REDUCE )
				//Ja
				file << puf2 << " }";

			else
				//ausgeben
				file << lrtab->value << " }";

			//Kommt noch etwas?
			if( j < ( termArrCount - 1 ) )
				//Ja
				file << ",";

			//neue Zeile?
			if( k == 8 )
			{
				//Ja
				file << "\n";
				k = 0;
			}
		}

		//Nächster Zustand?
		if( i < ( hulle_anz - 1 ) )
			//Ja
			file << " },\n";

		else
			//Nein
			file << " }\n";
	}

	//abschliessen
	file << "};\n";
}


void lrparser::outputJumpTab( ofstream &file )
{
	//Sprung-Tabelle ausgeben
	PROD_STR	*p1;
	long		index, prod_index;
	char		*name, widthChar[128];
	int			i, j, k, width;

	//Breite des größten Zustands
	sprintf( widthChar, "%d", hulle_anz );
	width = strlen( widthChar );

	//Definition der Sprung-Tabelle
	file << "\n\n//jump table\n";
	file << "long SSLRParser::SSLR_jumpTab[SSLRParser::SSLR_anzLRStates][SSLRParser::SSLR_anzNTerm] = {\n";

	//zuerst alle Produktionen ausgeben, dient der Orientierung
	file << "//";
	for( i = 0, k = 0; i < ntermArrCount; i++, k++ )
	{
		//neue Zeile?
		if( k == 10 )
			//Ja
			file << "\n//";

		//Produktion ausgeben
		file << hash_tab->getName( ntermArr[i] ) << ", ";
	}

	//neue Zeile
	file << "\n\n";

	//Alle Zustände
	for( i = 0; i < hulle_anz; i++ )
	{
		//Öffnen
		file << "/* State " << i << " */\n{ ";

		//Alle Produktionen
		for( j = 0, k = 0; j < ntermArrCount; j++, k++ )
		{
			//Name der Produktion
			name = hash_tab->getName( ntermArr[j] );

			//prod_index holen
			prod_index = hash_tab->getProdStr( ntermArr[j] );

			//Zeiger auf PROD_STR
			p1 = &prod_str[prod_index];

			//Index in Jump-Tabelle
			index = hash_tab->getLRTabIndex( p1->hash_index );

			//Wert ausgeben
			file.width( width );
			file << jumpTab[i][index];

			//Folgt noch etwas?
			if( j < ( prod_anz - 1 ) )
				//Ja
				file << ", ";

			//neue Zeile?
			if( k == 15 )
			{
				//Ja
				file << "\n";
				k = 0;
			}
		}

		//Nächster Zustand?
		if( i < ( hulle_anz - 1 ) )
			//Ja
			file << " },\n";

		else
			//Nein
			file << " }\n";
	}

	//abschliessen
	file << "};\n\n";
}


void lrparser::outputAlternatives( ofstream &file )
{
	//Die Alternativen in die cpp-Datei ausgeben.
	PROD2_STR	*p2;
	PROD_STR	*p1;
	char		*name, puf1[256+1];
	int			i, j, id, type;

	//Nur bei Debug-Version
	if( argv_str->debug == true )
	{
		//Header
		file << "\n//all alternatives\n";

		//Alle Alternativen
		for( i = 0; i < prod2_anz; i++ )
		{
			//Zeiger auf PROD2_STR
			p2 = &prod2_str[i];

			//Zeiger auf PROD_STR
			p1 = &prod_str[p2->prod_index];

			//Den Namen der Alternative holen
			name = hash_tab->getName( p1->hash_index );

			//Nummer der Alternative
			id = i - p1->prod2_index;

			//Namen generieren
			sprintf( puf1, "%s%d", name, id );

			//Initialisieren
			file << "char *SSLRParser::SSLR_" << puf1 << "Ri[" << p2->riAnz << "] = { ";

			//Alle Elemente auf der rechten Seite ausgeben
			for( j = 0; j < p2->riAnz; j++ )
			{
				//Namen holen
				name = hash_tab->getName( p2->right[j] );

				//Typ holen
				type = hash_tab->getType( p2->right[j] );

				//Terminal?
				if( type == SSLR_TERM )
					//Ja
					file << "\"\\\'" << name << "\\\'\"";

				else
					//Nichtterminal
					file << "\"" << name << "\"";

				//folgt noch ein Eintrag?
				if( j < ( p2->riAnz - 1 ) )
					//Ja
					file << ", ";
			}

			//abschließen
			file << " };\n";
		}
	}
}


void lrparser::outputNtermStr( ofstream &file )
{
	//die NTERM_STR ausgeben
	PROD2_STR	*p2;
	PROD_STR	*p1;
	char		*name, puf1[256+1];
	long		followAnz;
	int			i, id, riAnz;

	//Header
	file << "\n\n//all alternatives\n";
	file << "SSLRParser::SSLR_NTERM_STR SSLRParser::SSLR_ntermStr[SSLRParser::SSLR_anzProd] = {\n";

	//Alle Alternativen
	for( i = 0; i < prod2_anz; i++ )
	{
		//Zeiger auf PROD2_STR
		p2 = &prod2_str[i];

		//Zeiger auf PROD_STR
		p1 = &prod_str[p2->prod_index];

		//Den Namen der Alternative holen
		name = hash_tab->getName( p1->hash_index );

		//Anzahl der Follow-Elemente
		followAnz = hash_tab->getFollowAnz( p1->hash_index );

		//Nummer der Alternative
		id = i - p1->prod2_index;

		//Namen generieren
		sprintf( puf1, "%s%d", name, id );

		//ist es eine EPSILON-Produktion?
		if( p2->right[0] == epsHashIndex )
			//Ja
			riAnz = 0;

		else
			//Nein, ganz normal
			riAnz = p2->riAnz;

		//Debug-Version?
		if( argv_str->debug == true )
			//Nein, entsprechende SSLR_NTERM_STR ausgeben
			file << "{ \"" << name << "\", SSLRParser::SSLR_" << puf1 << "Ri, " << name << ", " << riAnz << ", SSLRParser::SSLR_" << name << "Follow, " << followAnz << " }";

		else
			//Ja, entsprechende SSLR_NTERM_STR ausgeben
			file << "{ " << name << ", " << riAnz << ", SSLRParser::SSLR_" << name << "Follow, " << followAnz << " }";

		//folgt noch ein Eintrag?
		if( i < ( prod2_anz - 1 ) )
			//Ja
			file << ",\n";
	}

	//abschließen
	file << "};\n\n";
}


void lrparser::outputFollow( ofstream &file )
{
	//Die Alternativen in die cpp-Datei ausgeben.
	PROD_STR	*p1;
	Terminal	*followSet;
	char		*name;
	long		followAnz, *followTerm;
	int			i, j;

	//Header
	file << "\n//follow sets for the error recovery\n";

	//Alle Produktionen
	for( i = 0; i < prod_anz; i++ )
	{
		//Zeiger auf PROD_STR
		p1 = &prod_str[i];

		//Den Namen der Produktion holen
		name = hash_tab->getName( p1->hash_index );

		//Zeiger auf die Follow-Elemente
		followSet = hash_tab->getFollow( p1->hash_index );

		//Anzahl der Follow-Elemente
		followAnz = hash_tab->getFollowAnz( p1->hash_index );

		//Initialisieren
		file << "long SSLRParser::SSLR_" << name << "Follow[" << followAnz << "] = { ";

		//Alle Follow-Elemente ausgeben
		for( j = 0; j < followAnz; j++ )
		{
			//Namen holen
			name = hash_tab->getName( (*followSet)[j] );

			//Terminal ausgeben
			file << name;

			//folgt noch ein Eintrag?
			if( j < ( followAnz - 1 ) )
				//Ja
				file << ", ";
		}

		//abschließen
		file << " };\n";
	}
}


void lrparser::outputTermName( ofstream &file )
{
	//Die Namen der Terminale ausgeben, nur im debug-mode
	char	*name;
	int		i, k;

	//Debug-Makro ausgeben
	file << "\n#ifdef SSLR_DEBUG\n";

	//Header
	file << "\n//names of the terminals\n";
	file << "char *SSLRParser::SSLR_termName[SSLRParser::SSLR_anzTerm] =\n{ ";

	//Alle Terminale
	for( i = k = 0; i < termArrCount; i++, k++ )
	{
		//Name des Terminals
		name = hash_tab->getName( termArr[i] );

		//neue Zeile?
		if( k == 16 )
		{
			//Ja
			file << "\n";
			k = 0;
		}

		//Define-Anweisung ausgeben
		file << "\"" << name << "\"";

		//Folgt noch ein Terminal?
		if( i < ( termArrCount - 1 ) )
			//Ja
			file << ", ";
	}

	//abschliessen
	file << " };\n\n";
	file << "#endif\n\n";
}


void lrparser::outputSemAct( ofstream &file )
{
	//semantische Aktionen ausgeben
	PROD2_STR	*p2;
	PROD_STR	*p1;
	char		*name, *pos, *tmp, *semTypeName;
	long		_line, LRTabIndex;
	bool		semTypeOK = false;
	int			i, state, depth, semIndex, semTypeIndex, id;
	int			*nArr, *tArr, type, sign, err;

	//Gibt es überhaupt welche?
	if( semActCnt && ( union_str->anz > 1 ) )
	{
		//dient der Vermeidung von Fehlerlawinen
		nArr = new( int[ntermArrCount] );
		tArr = new( int[termArrCount] );

		//Initialisieren
		memset( nArr, 0, sizeof( int ) * ntermArrCount );
		memset( tArr, 0, sizeof( int ) * termArrCount );

		//Funktions-Header ausgeben
		file << "\n";
		file << "\n";
		file << "void SSLRParser::SSLR_doSemAct( SSLR_LR_TAB *tab )\n";
		file << "{\n";
		file << "	//do semantic actions before reduction\n";
		file << "	SSLR_NTERM_STR	*n;\n";
		file << "	SSLR_TYPE		tmp;\n";
		file << "\n";
		file << "	//semantic actions allowed?\n";
		file << "	if( !SSLR_semAct )\n";
		file << "	{\n";
		file << "		//get a pointer to the alternative\n";
		file << "		n = &SSLR_ntermStr[tab->value];\n";
		file << "\n";
		file << "		//Alternative to reduce with\n";
		file << "		switch( tab->value )\n";
		file << "		{\n";

		//Alle Alternativen
		for( i = 0; i < prod2_anz; i++ )
		{
			//Fehlerflag zurücksetzen
			err = 0;

			//Zeiger auf PROD2_STR
			p2 = &prod2_str[i];

			//Zeiger auf PROD_STR
			p1 = &prod_str[p2->prod_index];

			//Gibt es semantische Aktionen?
			if( p2->semAct )
			{
				//Ja, ausgeben
				name = hash_tab->getName( p1->hash_index );

				//das entsprechende Item ausgeben
				file << "\t\t\t//";
				outputItem( i, p2->riAnz, file );
				file << "\n";

				//case ausgeben
				file << "\t\t\tcase " << name << "_" << i - p1->prod2_index << ":\n";
				file << "\t\t\t{\n\t\t\t\t";

				//Zustand setzen
				state = 0;
				_line = p2->semActLine;

				//Verschachtelung von Kommentaren
				depth = 0;

				//Zeiger auf den text
				pos = p2->semAct;

				//den Text analysieren
				while( *pos && !err )
				{
					//Blancs löschen
					switch( state )
					{
						case 0:
						{
							//Leerzeichen löschen
							while( isspace( *pos ) && ( *pos != '\n' ) )
								//nächstes Zeichen
								file << *pos++;

							break;
						}
					}

					//Neue Zeile?
					if( *pos == '\n' )
					{
						//Neue Zeile
						_line++;

						//Status: 2?
						if( state != 2 )
						{
							//Status 2 ist Kommentar //
							file << *pos++;
							continue;
						}
					}

					//semantischer Typ?
					if( !state && ( *pos == '$' ) )
					{
						//Ja
						state = 10;
						pos++;
					}

					switch( state )
					{
						//Default-Zustand
						case 10:
						{
							//semantischer Typ?
							if( *pos == '$' )
							{
								//den semantischen Typ der Produktion holen
								semIndex = hash_tab->getUnionType( p1->hash_index );

								//gibt es einen semantischen Typ?
								if( semIndex )
								{
									//Ja, mit semantischem Typ ausgeben
									file << "tmp." << union_str->id[semIndex];

									//ist überflüssigerweise ein semantischer Typ in Form von <typ>
									//angegeben worden?
									if( semTypeOK )
									{
										//sind die Typen gleich?
										if( semIndex == semTypeIndex )
											//Ja, Angabe ist überflüssig
											cerr << "sslr: line " << _line << ": Explicit semantic type statement not necessary!\n";

										else
											//Nein, sind verschieden!
											cerr << "sslr: line " << _line << ": Explicit semantic type statement does not coincide with %union statement!\n";

										//Speicher für Namen freigeben
										delete( semTypeName );

										//Flag zurücksetzen
										semTypeOK = false;
									}
								}

								else
								{
									//nein, ohne semantischen Typ ausgeben
									file << "tmp";

									//ist ein semantischer Typ in Form von <typ> angegeben worden?
									if( semTypeOK )
									{
										//Typ ausgeben
										file << "." << union_str->id[semTypeIndex];

										//Speicher für Namen freigeben
										delete( semTypeName );

										//Flag zurücksetzen
										semTypeOK = false;
									}

									else
									{
										//nein, Fehler

										//den Namen der Produktion
										name = hash_tab->getName( p1->hash_index );

										//den Index in der LR-Tabelle holen
										LRTabIndex = hash_tab->getLRTabIndex( p1->hash_index );

										//Warnung ausgeben?
										if( !nArr[LRTabIndex] )
											//Ja, Warnung ausgeben
											cerr << "sslr: line " << _line << ": No semantic type defined for production " << name << "!\n";

										//Vermeidung von Fehlerlawinen
										nArr[LRTabIndex]++;
									}
								}

								//überspringen
								pos++;

								//Zustand zurücksetzen
								state = 0;
							}

							else if( *pos == '<' )
							{
								//ist vom Typ $<...>
								pos++;

								//Position merken
								tmp = pos;

								//solange kein >
								while( ( *pos != '>' ) && *pos )
									//übertragen
									pos++;

								//genügend Speicher anfordern
								semTypeName = new( char[pos - tmp + 1] );

								//kopieren
								strncpy( semTypeName, tmp, pos - tmp );
								semTypeName[pos - tmp] = 0;

								//den Index in union_str finden
								if( ( semTypeIndex = getSemIndex( semTypeName ) ) < 0 )
								{
									//Typ nicht in %union definiert
									cerr << "sslr: ERROR: line " << _line << ": Type " << semTypeName << " for semantic action not defined in %union!\n";

									//Speicher gleich wieder freigeben
									delete( semTypeName );

									//Fehler
									semTypeOK = false;
								}

								else
									//Flag setzen
									semTypeOK = true;

								//> löschen
								pos++;
							}

							else if( isdigit( *pos ) || ( *pos == '-' ) )
							{
								//bezieht sich auf ein Element auf der rechten Seite
								//semStack[semStackCnt-1] ist das letzte Element

								//negativer Index?
								if( *pos == '-' )
								{
									//Ja, negativ
									sign = -1;
									pos++;
								}

								else
									//ist positiv
									sign = 1;

								//Zahl einlesen
								id = strtol( pos, 0L, 10 );

								//Vorzeichen
								id *= sign;

								//Ziffern löschen
								while( isdigit( *pos ) )
									pos++;

								//gibt es überhaupt so viele Elemente auf der rechten Seite wie angegeben?
								if( ( id - 1 ) < p2->riAnz )
								{
									//Ist es der Form prod -> 'EPSILON'`?
									if( ( p2->riAnz == 1 ) && ( p2->right[0] == epsHashIndex ) )
									{
										//Warnung ausgeben
										cerr << "sslr: line " <<  _line << ": $" << id << " not allowed inside 'EPSILON'-alternative!\n";

										//nächste semantische Aktion behandelt
										err++;
										break;
									}

									else
									{
										//den semantischen Typ des Elements holen
										semIndex = hash_tab->getUnionType( p2->right[id - 1] );

										//gibt es einen semantischen Typ?
										if( semIndex )
										{
											//Ja, Stack-Zugriff
											file << "SSLR_semStack[SSLR_semStackCnt - 1 - " << p2->riAnz << " + (" << id << ")]." << union_str->id[semIndex];

											//ist überflüssigerweise ein semantischer Typ in Form von <typ>
											//angegeben worden?
											if( semTypeOK )
											{
												//sind die Typen gleich?
												if( semIndex == semTypeIndex )
													//Ja, Angabe ist überflüssig
													cerr << "sslr: line " << _line << ": Explicit semantic type statement not necessary!\n";

												else
													//Nein, sind verschieden!
													cerr << "sslr: line " << _line << ": Explicit semantic type statement does not coincide with %union-Statement!\n";

												//Speicher für Namen freigeben
												delete( semTypeName );

												//Flag zurücksetzen
												semTypeOK = false;
											}
										}

										else
										{
											//Nein, Stack-Zugriff
											file << "SSLR_semStack[SSLR_semStackCnt - 1 - " << p2->riAnz << " + (" << id << ")]";

											//ist ein semantischer Typ in Form von <typ> angegeben worden?
											if( semTypeOK )
											{
												//Typ ausgeben
												file << "." << union_str->id[semTypeIndex];

												//Speicher für Namen freigeben
												delete( semTypeName );

												//Flag zurücksetzen
												semTypeOK = false;

												//überspringen
												pos++;
											}

											else
											{
												//den Namen des Elements
												name = hash_tab->getName( p2->right[id - 1] );

												//den Index in der LR-Tabelle holen
												LRTabIndex = hash_tab->getLRTabIndex( p2->right[id - 1] );

												//Typ holen
												type = hash_tab->getType( p2->right[id - 1] );

												//Terminal?
												if( type == SSLR_TERM )
												{
													//Ja

													//Warnung ausgeben?
													if( !tArr[LRTabIndex] )
														//Warnung ausgeben
														cerr << "sslr: line " << _line << ": No semantic type defined for terminal " << name << "!\n";

													//Vermeidung von Fehlerlawinen
													tArr[LRTabIndex]++;
												}

												else
												{
													//Ist ein Nicht-Terminal

													//Warnung ausgeben?
													if( !nArr[LRTabIndex] )
														//Warnung ausgeben
														cerr << "sslr: line " << _line << ": No semantic type defined for nonterminal " << name << "!\n";

													//Vermeidung von Fehlerlawinen
													nArr[LRTabIndex]++;
												}
											}
										}
									}
								}

								else
								{
									//Indiziertes Element zu groß!
									cerr << "sslr: line " << _line << ": Number in semantic type statement too big!\n";

									//abbrechen
									err++;
									break;
								}

								//Zustand zurücksetzen
								state = 0;
							}

							break;
						}

						case 0:
						{
							//Kommentar?
							if( ( pos[0] == '/' ) && ( pos[1] == '*' ) )
							{
								//Ja, Kommentar
								state = 1;

								//Verschachtelung
								depth++;
								pos += 2;

								//ausgeben
								file << "/*";
							}

							else if( ( pos[0] == '/' ) && ( pos[1] == '/' ) )
							{
								//Kommentar
								pos += 2;
								file << "//";
								state = 2;
							}

							else
								//einfach übertragen
								file << *pos++;

							break;
						}

						case 1:
						{
							//Kommentar /*
							if( ( pos[0] == '*' ) && ( pos[1] == '/' ) )
							{
								//Kommentar schliessen
								file << "*/";
								pos += 2;

								//Verschachtelungstiefe
								depth--;

								//Default-Zustand?
								if( !depth )
									//Ja
									state = 0;
							}

							else if( ( pos[0] == '/' ) && ( pos[1] == '*' ) )
							{
								//Ja, Kommentar

								//Verschachtelung
								depth++;
								pos += 2;

								//ausgeben
								file << "/*";
							}

							else
								//einfach übertragen
								file << *pos++;

							break;
						}

						case 2:
						{
							//Kommentar //
							file << *pos;

							//Newline?
							if( *pos++ == '\n' )
								//Ja
								state = 0;

							break;
						}

						default:
							//einfach kopieren
							file << *pos++;
					}
				}

				//case abschliessen
				file << "\t\t\t\tbreak;\n";
				file << "\t\t\t}\n";
				file << "\n";
			}
		}

		file << "\t\t\tdefault:\n";
		file << "\t\t\t\t//error\n";
		file << "\t\t\t\tcerr << \"SSLRParser::SSLR_doSemAct: Unknown alternative \" << tab->value << \"!\\n\";\n";
		file << "\t\t}\n";
		file << "\n\t\t//correct stack pointer\n";
		file << "\t\tSSLR_semStackCnt -= n->elem;\n";
		file << "\n";
		file << "\t\t//push last value on stack\n";
		file << "\t\tSSLR_semStack[SSLR_semStackCnt++] = tmp;\n";
		file << "\t}\n";
		file << "}\n";
		file << "\n";

		//Speicher freigeben
		delete[] ( nArr );
		delete[] ( tArr );
	}
}


void lrparser::outputErrorRecovery( ofstream &file )
{
	//Hier wird die komplette Fehlerbehandlung ausgegeben
	ERRTERM_STR		*errTermStr;
	ERROR_STR		*a, *b;
	HULLE_STR		*h;
	LR_STR			*lr;
	LR_TAB			*lrtab;
	long			LRTabIndex;
	char			*name;
	int				i, j, k, m;
	int				*errArr1, errArr1Cnt = 0, *errArr2, errArr2Cnt = 0;
	int				anzTerm, anznTerm, pass;
	int				*hulleArr, hulleArrCnt, autoErr;

	//Gibt es überhaupt Fehlerangaben?
	if( error_anz )
	{
		file << "\n";
		file << "\n";
		file << "void SSLRParser::SSLR_errShift( long termi )\n";
file << "{\n";
file << "	//shift a new state\n";
file << "	if( SSLR_lrTab[SSLR_state][termi].state == LR_SHIFT )\n";
file << "		//push new state\n";
file << "		SSLR_push( SSLR_lrTab[SSLR_state][termi].value );\n";
file << "}\n";
file << "\n";
file << "\n";
file << "void SSLRParser::SSLR_errDelete( void )\n";
file << "{\n";
file << "	//delete current terminal\n";
file << "	terminal = SSLR_getTerminal();\n";
file << "}\n";
file << "\n";
file << "\n";
file << "void SSLRParser::SSLR_errOverRead( long termi )\n";
file << "{\n";
file << "	//delete all terminals untill term is found\n";
file << "	do\n";
file << "	{\n";
file << "		//get next terminal\n";
file << "		terminal = SSLR_getTerminal();\n";
file << "\n";
file << "		//equal?\n";
file << "		if( terminal == termi )\n";
file << "			//yes, return\n";
file << "			return;\n";
file << "\n";
file << "		else if( terminal == ACCEPT )\n";
file << "		{\n";
file << "			//abort parsing\n";
file << "			state = 1;\n";
file << "			return;\n";
file << "		}\n";
file << "\n";
file << "	}while( 1 );\n";
file << "}\n";
	}

	//Fehler-Routine ausgeben
file << "\n";
file << "\n";
file << "void SSLRParser::SSLR_error( void )\n";
file << "{\n";
file << "	//error handling\n";
file << "	SSLR_NTERM_STR	*n;\n";
file << "	long			*f, tmpState;\n";
file << "	int				fCnt = 0, i, rec = 0;\n";
file << "\n";
file << "	//delete all terminals untill one that follows the last nonterminal\n";
file << "	//the reduction was done with\n";
file << "	SSLR_anzError++;\n";
file << "\n";

	//semantische Aktionen?
	if( semActCnt && ( union_str->anz > 1 ) )
	{
		//Ja, semantische Aktionen des Parsers stoppen
		file << "	//no semantic actions are allowed after errors\n";
		file << "	SSLR_semAct++;";
		file << "\n";
		file << "\n";
	}

file << "	//state after reduction\n";
file << "	tmpState = SSLR_errorStr.state;\n";
file << "\n";
file << "	//get a pointer to the SSLR_NTERM_STR of the nonterminal\n";
file << "	n = &SSLR_ntermStr[SSLR_errorStr.alt];\n";
file << "\n";

	//Debug-mode?
	if( argv_str->debug == true )
	{
		//Ja
		file << "	//debugging message\n";
		file << "	cout << \"SSLRParser::SSLR_error: Looking for terminal that may follow \" << n->name << \"..\\n\";\n";
		file << "\n";
	}

file << "	//get the follow terminals\n";
file << "	f = n->follow;\n";
file << "\n";
file << "	//number of follow terminals\n";
file << "	fCnt = n->followCnt;\n";
file << "\n";
file << "\n";



	//Gibt es überhaupt Fehlerangaben?
	if( error_anz )
	{
		//Ja, Header ausgeben
		file << "	switch( SSLR_state )\n";
		file << "	{\n";

		//Es gibt ERROR_STR, die dieselbe Alternative abdecken. Diese dürfen daher nicht doppelt auftreten.
		errArr1 = new( int[error_anz] );
		errArr2 = new( int[error_anz] );

		//Initialisieren
		memset( errArr2, 0, error_anz * sizeof( int ) );

		//Alle Hüllen, die auf ein Satz ERROR_STR passen, merken
		hulleArr = new( int[hulle_anz] );

		//Alle Fehler-Strukturen durchgehen
		for( i = 0; i < error_anz; i++ )
		{
			//schon bearbeitet?
			if( ( errArr2Cnt ) && ( errArr2[i] ) )
				//Ja
				continue;

			//Zeiger auf ERROR_STR
			a = &error_str[i];

			//aufnehmen
			errArr1[errArr1Cnt++] = i;

			//verhindert ein doppeltes Prüfen dieser ERROR_STR
			errArr2[i] = 1;
			errArr2Cnt++;

			//Alle anderen ERROR_STR aufnehmen, die daselbe Item behandeln
			for( j = i + 1; j < error_anz; j++ )
			{
				//Zeiger auf ERROR_STR
				b = &error_str[j];

				//Gleiche Items?
				if( ( a->prod2_index == b->prod2_index ) && ( a->dot == b->dot ) )
				{
					//Ja, aufnehmen
					errArr1[errArr1Cnt++] = j;

					//verhindert ein doppeltes Prüfen dieser ERROR_STR
					errArr2[j] = 1;
					errArr2Cnt++;
				}
			}


			//Das entsprechende Item ausgeben
			file << "		//";

			//Entsprechende Alternative ausgeben
			outputItem( a->prod2_index, a->dot, file );
			file << "\n";

			//Nun die Hüllen suchen, in denen dieses Item auftaucht
			for( j = 0, hulleArrCnt = 0; j < hulle_anz; j++ )
			{
				//Zeiger auf Hülle
				h = hulle_str[j];

				//Alle Items
				for( k = 0; k < h->anzElem; k++ )
				{
					//Zeiger auf LR_STR
					lr = &h->lr_str[k];

					//Sind die Items gleich?
					if( ( a->prod2_index == lr->prod2_index ) && ( a->dot == lr->dot ) )
					{
						//Ja, Fehlerbehandlung für diese Hülle installieren
						file << "		case " << j << ":\n";

						//Den Index der Hülle merken
						hulleArr[hulleArrCnt++] = j;
					}
				}
			}

			//Alle Aktionen in errStr->act_str abarbeiten
			file << "\t\t{\n";

			//1. Pass starten
			pass = 0;

			//Anzahl der terminal-gebundenen Fehleraktionen
			anzTerm = 0;

			//Anzahl der nicht-terminal-gebundenen Fehleraktionen
			anznTerm = 0;

			//Nun alle ERROR_STR durchlaufen,
			for( j = 0; ( j < errArr1Cnt ) && ( pass < 2 ); j++ )
			{
				//Zeiger auf die entsprechende ERROR_ANZ
				b = &error_str[errArr1[j]];

				//festhalten, ob die automatische Fehlerbehandlung gestartet werden soll
				autoErr = 0;

				//1. Pass?
				if( !pass )
				{
					//zuerst werden alle terminal-gebundenen Fehleraktionen ausgegeben
					if( b->errTermStr )
					{
						//if oder else if?
						if( !anzTerm )
							//das erste Auftreten
							file << "\t\t\tif( ";

						else
							//bei allen übrigen else if
							file << "\t\t\telse if( ";

						//Alle Terminal durchgehen
						for( m = 0; m < b->errTermStrAnz; m++ )
						{
							//Zeiger auf ERRTERM_STR
							errTermStr = &b->errTermStr[m];

							//den Namen des Terminals holen
							name = hash_tab->getName( errTermStr->terminal );

							//negiert?
							if( errTermStr->neg )
							{
								//Ja, alle ausser dieses

								//&& anfügen?
								if( m )
									//Ja
									file << " && ";

								//Bedingung ausgeben
								file << "( terminal != " << name << " )";
							}

							else
							{
								//Nein, nur bei diesem

								//&& anfügen?
								if( m )
									//Ja
									file << " || ";

								//Bedingung ausgeben
								file << "( terminal == " << name << " )";
							}
						}

						//neue Zeile
						file << " )\n";
						file << "\t\t\t{\n";

						//Fehlermeldung ausgeben?
						if( strcmp( b->msg, "" ) )
						{
							/*
								Ja, wenn die Fehlermeldung aus "" besteht, dann gar keine ausgeben.
								Die Fehlermeldung MUSS vor den Fehler-Aktionen ausgegeben werden, da,
								wenn z.B. EXIT verwendet wird, die Fehlermeldung nicht gezeigt
								würde!
							*/
							file << "\t\t\t\t//show user-defined error message\n";
							file << "\t\t\t\tcerr << \"line: \" << line << \": " << b->msg << "\";\n\n";
						}

						//Fehleraktionen ausgeben
						if( b->shift )
						{
							//den Namen des Terminals holen
							name = hash_tab->getName( b->shift );

							//Neuen Zustand schieben
							file << "\t\t\t\t//shift Terminal " << name << "\n";
							file << "\t\t\t\tSHIFT( " << name << " );\n\n";

							//Index des Terminals in der LR-Tabelle holen
							LRTabIndex = hash_tab->getLRTabIndex( b->shift );

							//Prüfen, ob das Terminal tatsächlich bei jeder Hülle, für die ein case
							//installiert wurde, geschoben werden kann
							for( m = 0; m < hulleArrCnt; m++ )
							{
								//Zeiger auf LR_TAB-Struktur
								lrtab = &lrTab[hulleArr[m]][LRTabIndex];

								//Kann das Terminal geschoben werden?
								if( lrtab->state != LR_SHIFT )
									//Nein, Warnung ausgeben
									cerr << "sslr: line " << b->line << ": Terminal " << name << " cannot be shifted in state " << hulleArr[m] << "!\n";
							}

							//automatische Fehlerbehandlung einschalten
							autoErr++;
						}

						if( b->overread )
						{
							//den Namen des Terminals holen
							name = hash_tab->getName( b->overread );

							//Terminale überlesen
							file << "\t\t\t\t//delete terminals untill " << name << " ist the current Terminal\n";
							file << "\t\t\t\tOVERREAD( " << name << " );\n\n";

							//automatische Fehlerbehandlung einschalten
							autoErr++;
						}

						if( b->del )
						{
							//ein Terminal löschen
							file << "\t\t\t\t//delete current terminal\n";

							//mehrfaches Auftreten von DELETE ist erlaubt
							for( m = 0; m < b->del; m++ )
								file << "\t\t\t\tDELETE();\n";

							//neue Zeile
							file << "\n";

							//automatische Fehlerbehandlung einschalten
							autoErr++;
						}

						if( b->exit )
						{
							//Parsen abbrechen
							file << "\t\t\t\t//stop parsing\n";
							file << "\t\t\t\tEXIT;\n";
						}

						if( b->sem )
						{
							//semantische Aktionen trotz Fehler zulassen

							//gibt es überhaupt semantische Aktionen?
							if( semActCnt && ( union_str->anz > 1 ) )
							{
								//Ja
								file << "\t\t\t\t//allow semantic actions despite the error\n";
								file << "\t\t\t\tSEM;\n\n";
							}

							else
								//Nein
								cerr << "sslr: line " << b->line << ": There are no semantic actions not to allow!\n";
						}

						//automatische Fehlerbehandlung überspringen, wenn keine Aktion gewält wurde oder nur SEM
						if( autoErr )
						{
							//Nein, automatische Fehlerbehandlung trotz eigener Fehler-Angaben ausführen
							file << "\t\t\t\t//skip automatic error recovery\n";
							file << "\t\t\t\trec++;\n\n";
						}

						//abschließen
						file << "\t\t\t}\n\n";

						//zählen
						anzTerm++;
					}
				}

				else
				{
					//hier werden alle nicht-terminal-gebundenen Fehleraktionen ausgeführt

					//nur nicht-terminal-gebundenen Fehleraktionen ausgegeben
					if( !b->errTermStr )
					{
						//Gab es terminal-gebundene Fehleraktionen?
						if( anzTerm && !anznTerm )
							//Ja, else ausgeben
							file << "\t\t\telse\n\t\t\t{\n";

						//Fehlermeldung ausgeben?
						if( strcmp( b->msg, "" ) )
						{
							/*
								Ja, wenn die Fehlermeldung aus "" besteht, dann gar keine ausgeben.
								Die Fehlermeldung MUSS vor den Fehler-Aktionen ausgegeben werden, da,
								wenn z.B. EXIT verwendet wird, die Fehlermeldung nicht gezeigt
								würde!
							*/
							file << "\t\t\t\t//show user-defined error message\n";
							file << "\t\t\t\tcerr << \"line: \" << line << \": " << b->msg << "\";\n\n";
						}

						//Fehleraktionen ausgeben
						if( b->shift )
						{
							//den Namen des Terminals holen
							name = hash_tab->getName( b->shift );

							//Neuen Zustand schieben
							file << "\t\t\t\t//shift terminal " << name << "\n";
							file << "\t\t\t\tSHIFT( " << name << " );\n\n";

							//Index des Terminals in der LR-Tabelle holen
							LRTabIndex = hash_tab->getLRTabIndex( b->shift );

							//Prüfen, ob das Terminal tatsächlich bei jeder Hülle, für die ein case
							//installiert wurde, geschoben werden kann
							for( m = 0; m < hulleArrCnt; m++ )
							{
								//Zeiger auf LR_TAB-Struktur
								lrtab = &lrTab[hulleArr[m]][LRTabIndex];

								//Kann das Terminal geschoben werden?
								if( lrtab->state != LR_SHIFT )
									//Nein, Warnung ausgeben
									cerr << "sslr: line " << b->line << ": Terminal " << name << " cannot be shifted in state " << hulleArr[m] << "!\n";
							}

							//automatische Fehlerbehandlung einschalten
							autoErr++;
						}

						if( b->overread )
						{
							//den Namen des Terminals holen
							name = hash_tab->getName( b->overread );

							//Terminale überlesen
							file << "\t\t\t\t//delete terminals untill " << name << " is the current terminal\n";
							file << "\t\t\t\tOVERREAD( " << name << " );\n\n";

							//automatische Fehlerbehandlung einschalten
							autoErr++;
						}

						if( b->del )
						{
							//ein Terminal löschen
							file << "\t\t\t\t//delete current terminal\n";

							//mehrfaches Auftreten von DELETE ist erlaubt
							for( m = 0; m < b->del; m++ )
								file << "\t\t\t\tDELETE();\n";

							//neue Zeile
							file << "\n";

							//automatische Fehlerbehandlung einschalten
							autoErr++;
						}

						if( b->exit )
						{
							//Parsen abbrechen
							file << "\t\t\t\t//stop parsing\n";
							file << "\t\t\t\tEXIT;\n";
						}

						if( b->sem )
						{
							//semantische Aktionen trotz Fehler zulassen

							//gibt es überhaupt semantische Aktionen?
							if( semActCnt && ( union_str->anz > 1 ) )
							{
								//Ja
								file << "\t\t\t\t//allow semantic actions despite the error\n";
								file << "\t\t\t\tSEM;\n\n";
							}

							else
								//Nein
								cerr << "sslr: line " << b->line << ": There are no semantic actions not to allow!\n";
						}

						//automatische Fehlerbehandlung überspringen, wenn keine Aktion gewält wurde oder nur SEM
						if( autoErr )
						{
							//Nein, automatische Fehlerbehandlung trotz eigener Fehler-Angaben ausführen
							file << "\t\t\t\t//skip automatic error recovery\n";
							file << "\t\t\t\trec++;\n\n";
						}

						//zählen
						anznTerm++;

						//abschliessen
						if( anzTerm && ( anzTerm + anznTerm == errArr1Cnt ) )
							//Ja
							file << "\t\t\t}\n\n";
					}
				}

				//neuen Pass starten?
				if( j == ( errArr1Cnt - 1 ) )
				{
					//Ja
					pass++;
					j = -1;
				}
			}

			//nächste ERROR_STR
			errArr1Cnt = 0;

			//case abschliessen
			file << "\t\t\tbreak;\n";
			file << "		}\n\n";
		}

		//default ausgeben
		file << "		default:\n";
		file << "		{\n";
		file << "			//default error message\n";
		file << "			cerr << \"line: \" << line << \": Syntax error!\\n\";\n";
		file << "			break;\n";
		file << "		}\n";

		//switch-Statement abschliessen
		file << "\t}\n\n";

		//Speicher wieder freigeben
		delete[] ( errArr1 );
		delete[] ( errArr2 );
		delete[] ( hulleArr );
	}

	else
	{
		//es gibt keine benutzter-definierten Fehleranweisungen, nun default-Fehlermeldung ausgeben
		file << "	//default error message\n";
		file << "	cerr << \"line: \" << SSLR_line << \": Syntax error!\\n\";\n";
	}


	//noch gewöhnliche Fehlerbehandlung
file << "\n";
file << "	//do SSLRs intern error recovery?\n";
file << "	if( !rec )\n";
file << "	{\n";
file << "		//yes, no user defined error routines given\n";
file << "\n";
file << "		do\n";
file << "		{\n";
file << "			//is the current terminal ACCEPT?\n";
file << "			if( SSLR_terminal == ACCEPT )\n";
file << "			{\n";
file << "				//abort parsing\n";
file << "				SSLR_state = 1;\n";
file << "				return;\n";
file << "			}\n";
file << "\n";
file << "			//may the terminal SSLR_terminal follow?\n";
file << "			for( i = 0; i < SSLR_anzTerm; i++ )\n";
file << "			{\n";
file << "				//shift OK?\n";
file << "				if( SSLR_lrTab[tmpState][SSLR_terminal].state == LR_SHIFT )\n";
file << "				{\n";

	//Debug-mode?
	if( argv_str->debug == true )
	{
		//Ja

		//Debug-Makro ausgeben
		file << "\n#ifdef SSLR_DEBUG\n";

		file << "					//inform the user\n";
		file << "					cout << SSLR_termName[SSLR_terminal] << \" may follow \" << n->name << \"\\n\\n\";\n";

		//Debug-Makro ausgeben
		file << "#endif\n\n";
	}

file << "					//set new state\n";
file << "					SSLR_state = SSLR_errorStr.state;\n";
file << "\n";
file << "					//get stack position\n";
file << "					SSLR_sp = SSLR_errorStr.sp;\n";
file << "					return;\n";
file << "				}\n";
file << "			}\n";
file << "\n";
file << "			//get next terminal\n";
file << "			SSLR_terminal = SSLR_getTerminal();\n";
file << "\n";
file << "		}while( 1 );\n";
file << "	}\n";
file << "\n";
file << "\treturn;\n";
file << "}\n";
file << "\n";
file << "\n";
}


void lrparser::outputIncl( ofstream &file )
{
	//Include-Anweisungen in das cpp-File ausgeben
	if( incl )
		//ausgeben
		file << incl;
}


void lrparser::outputExt( ofstream &file )
{
	//eigene Erweiterungen in das cpp-File ausgeben
	if( ext )
		//ausgeben
		file << ext;
}


void lrparser::outputHFile( ofstream &file )
{
	//Header-File ausgeben
	PROD2_STR	*p2;
	PROD_STR	*p1;
	string		a;
	char		*name, puf1[256+1];
	long		followAnz;
	int			i, id;

	//Kopf ausgeben
file << "/***************************************************************************\n";
file << "                          sslrparser.h  -  description\n";
file << "                             -------------------\n";
file << " ***************************************************************************/\n";
file << "\n";
file << "/***************************************************************************\n";
file << " *                                                                         *\n";
file << " *   This program is free software; you can redistribute it and/or modify  *\n";
file << " *   it under the terms of the GNU General Public License as published by  *\n";
file << " *   the Free Software Foundation; either version 2 of the License, or     *\n";
file << " *   (at your option) any later version.                                   *\n";
file << " *                                                                         *\n";
file << " ***************************************************************************/\n";
file << "\n";
file << "#ifndef SSLRPARSER_H\n";
file << "#define SSLRPARSER_H\n";
file << "\n";
file << "\n";
file << "/**\n";
file << "\t@author SSLR V" << VERSION << "\n\t(c) 2002-2005 by Sven Schmidt\n\teMail: s.schmidt@lboro.ac.uk\n";
file << "  */\n";
file << "\n";
file << "\n";


	//Debug-Version?
	if( argv_str->debug == true )
	{
		//Debug-Version
		file << "/****************************\n";
		file << "* This is the debug version *\n";
		file << "****************************/\n\n\n";

		//Debug-Makro
		file << "//define debug macro\n";
		file << "#define SSLR_DEBUG\n";
		file << "//#undef SSLR_DEBUG\n\n\n";
	}

	else
	{
		//ist nicht die Debug-Version
		file << "/********************************\n";
		file << "* This is the non-debug version *\n";
		file << "********************************/\n\n\n";
	}

	//Include-Files
	file << "//Include-Files\n";
	file << "#include <iostream>\n";
	file << "#include \"sslexscanner.h\"\n\n";

	//Allgemeine Definitionen

	//gibt es überhaupt Fehlerangaben?
	if( error_anz )
	{
		//Ja, die Fehlerroutinen als Defines ausgeben
		file << "//error routines\n";
		file << "#define POP\t\tSSLR_errPop\n";
		file << "#define DELETE\t\tSSLR_errDelete\n";
		file << "#define SHIFT\t\tSSLR_errShift\n";
		file << "#define OVERREAD\tSSLR_errOverRead\n";
		file << "#define SEM\t\tSSLR_semAct = 0\n";
		file << "#define EXIT\t\texit( 1 )\n";
		file << "\n\n\n";
	}


	//Terminal-Defines
	file << "//terminal defines\n";

	for( i = 0; i < termArrCount; i++ )
	{
		//Name des Terminals
		name = hash_tab->getName( termArr[i] );

		//Define-Anweisung ausgeben
		file << "#define " << name << "\t" << i << "\n";
	}


	//Nicht-Terminal-Defines für Sprung-Tabelle
	file << "\n\n//Nonterminal defines for the jump table\n";

	for( i = 0; i < ntermArrCount; i++ )
	{
		//Name des Nicht-Terminals
		name = hash_tab->getName( ntermArr[i] );

		//Define-Anweisung ausgeben
		file << "#define " << name << "\t" << i << "\n";
	}

	//Nicht-Terminale für die LR-Tabelle
	file << "\n\n//Nontermials for the LR table\n";

	for( i = 0; i < prod2_anz; i++ )
	{
		//Zeiger auf PROD2_STR
		p2 = &prod2_str[i];

		//Zeiger auf PROD_STR
		p1 = &prod_str[p2->prod_index];

		//Name der Produktion
		name = hash_tab->getName( p1->hash_index );

		//Nummer der Alternative
		id = i - p1->prod2_index;

		//Namen generieren
		sprintf( puf1, "%s_%d", name, id );

		//Define-Anweisung ausgeben
		file << "#define " << puf1 << "\t" << i << "\n";
	}


	//Struktur für die LR-Tabelle
	file << "\n\n//LR table\n";
	file << "typedef struct\n";
	file << "{\n";
	file << "\tint\t\t\tstate;\n";
	file << "\tlong\t\tvalue;\n";
	file << "\n}SSLR_LR_TAB;\n\n";


	//Struktur für alle Alternativen
	file << "\n";
	file << "\n";
	file << "//all prodctions for debug mode\n";
	file << "typedef struct\n";
	file << "{\n";

	//Debug-Mode?
	if( argv_str->debug == true )
	{
		file << "	//name of alternative\n";
		file << "	char *\tname;\n";
		file << "\n";
		file << "	//pointer to right side\n";
		file << "	char **\tri;\n";
		file << "\n";
	}

	//Zeiger auf die Follow-Terminale
	file << "	//index in jumpTab\n";
	file << "	int\t\tjump_index;\n";
	file << "\n";
	file << "	//number of elements on the right side\n";
	file << "	int\t\telem;\n";
	file << "\n";
	file << "	//pointer to follow sets\n";
	file << "	long *\tfollow;\n";
	file << "\n";
	file << "	//number of follow terminals\n";
	file << "	long\tfollowCnt;\n";
	file << "\n";
	file << "}SSLR_NTERM_STR;\n";
	file << "\n";
	file << "\n";


	//Struktur für die Standard-Fehlerbehandlung
	file << "\n";
	file << "\n";
	file << "//this structure saves important values for the error recovery\n";
	file << "typedef struct\n";
	file << "{\n";
	file << "	//stack pointer position\n";
	file << "	int\t\tsp;\n";
	file << "\n";
	file << "	//state after reduction\n";
	file << "	int\t\tstate;\n";
	file << "\n";
	file << "	//number of alternative the reduction was done\n";
	file << "	int\t\talt;\n";
	file << "\n";
	file << "}SSLR_ERROR_STR;\n";
	file << "\n";
	file << "\n";

	//wurde eine union definiert?
	if( union_str->anz > 1 )
	{
		//Ja, ausgeben
		file << "//Type for semantic actions\n";
		file << "typedef union\n{\n";

		//Text ausgeben
		file << union_str->text;

		//abschliessen
		file << "\n}SSLR_TYPE;\n\n\n";
	}


	//Typen für die semantischen Aktionen

	//Klassen-Deklaration
	file << "class SSLRParser\n";
	file << "{\n";
	file << "private:\n";
	file << "\t//private variables\n\n";

	//Anzahl der Zustände
	file << "\t//Number of lr states\n";
	file << "\tstatic const unsigned long\tSSLR_anzLRStates = " << hulle_anz << ";\n\n";

	//Anzahl der Terminale
	file << "\t//Number of terminals = number of columns in lrTab\n";
	file << "\tstatic const unsigned long\tSSLR_anzTerm = " << termArrCount << ";\n\n";

	//Anzahl der Produktionen
	file << "\t//Number of productions = number of rows in jumpTab\n";
	file << "\tstatic const unsigned long\tSSLR_anzNTerm = " << prod_anz << ";\n\n";

	//Anzahl der Alternativen
	file << "\t//Number of alternatives\n";
	file << "\tstatic const unsigned long\tSSLR_anzProd = " << prod2_anz << ";\n\n";

	//Zustände in der LR-Tabelle
	file << "\n\t//Actions in lrTab\n";
	file << "\tstatic const int\t\t\tLA_ERROR = 1;\n";
	file << "\tstatic const int\t\t\tLR_SHIFT = 2;\n";
	file << "\tstatic const int\t\t\tLR_REDUCE = 3;\n";
	file << "\tstatic const int\t\t\tLR_ACCEPT = 4;\n\n";

	//Stack-Größe
	file << "\n\t//maximum stack size\n";
	file << "\tstatic const int\t\t\tSSLR_maxStackSize = 100;\n\n";

	//Stack für Zustände
	file << "\n";
	file << "	//parse stack\n";
	file << "	long\t\t\t\t\t\tSSLR_stack[SSLR_maxStackSize];\n";
	file << "\n";
	file << "	//stack counter\n";
	file << "	long\t\t\t\t\t\tSSLR_sp;\n";
	file << "\n";
	file << "\n";

	//LR-Tabelle
	file << "\t//LR table\n";
	file << "\tstatic SSLR_LR_TAB\t\t\tSSLR_lrTab[SSLR_anzLRStates][SSLR_anzTerm];\n\n";

	//Sprung-Tabelle
	file << "\t//jump table\n";
	file << "\tstatic long\t\t\t\t\tSSLR_jumpTab[SSLR_anzLRStates][SSLR_anzNTerm];\n\n";

	//Zeiger auf die Scanner-Klasse
	file << "\t//scanner class\n";
	file << "\tSSLEXScanner *\t\t\t\tSSLR_scanner;\n\n";

	//Array with all alternatives, for debug mode only
	file << "\t//Array with all alternatives, for debug-mode only\n";
	file << "\tstatic SSLR_NTERM_STR\t\tSSLR_ntermStr[SSLR_anzProd];\n\n";

	//Debug-Mode?
	if( argv_str->debug == true )
	{
		//Debug-Makro ausgeben
		file << "#ifdef SSLR_DEBUG\n";

		//Array mit den Namen der Terminale
		file << "\t//array with the names of the terminals\n";
		file << "\tstatic char *\t\t\t\tSSLR_termName[SSLR_anzTerm];\n";

		//Debug-Makro ausgeben
		file << "#endif\n\n";
	}


	//Typ des Terminals, dass der Scanner zurückliefert
	file << "	//current terminal\n";
	file << "	int\t\t\t\t\t\t\tSSLR_terminal;\n";
	file << "\n";

	//Atrribut des Terminals wie z.B. Zahlenwert
	file << "	//attributes returned form scanner\n";
	file << "	SSLR_TYPE\t\t\t\t\tSSLR_attributeStr;\n";
	file << "\n";

	//Zeile, in der ein Fehler auftrat
	file << "	//line number\n";
	file << "	unsigned long\t\t\t\tSSLR_line;\n";
	file << "\n";

	//Aktueller Zustand des Parsers
	file << "	//current state\n";
	file << "	int\t\t\t\t\t\t\tSSLR_state;\n";
	file << "\n";

	//Anzahl der aufgetretenen Fehler
	file << "	//number of errors\n";
	file << "	int\t\t\t\t\t\t\tSSLR_anzError;\n";
	file << "\n";

	//Struktur für Fehlerbehandlung in der Standard-Fehlerbehandlung
	file << "	//structure for error-recovery\n";
	file << "	SSLR_ERROR_STR\t\t\t\tSSLR_errorStr;\n";
	file << "\n";

	//semantische Aktionen?
	if( semActCnt && ( union_str->anz > 1 ) )
	{
		//semantische Aktionen, für Fehlerbehandlung
		file << "	//allow/deny semantic actions\n";
		file << "	int\t\t\t\t\t\t\tSSLR_semAct;\n";
		file << "\n";
		file << "	//stack for semantic actions\n";
		file << "	SSLR_TYPE\t\t\t\t\tSSLR_semStack[SSLR_maxStackSize];\n";
		file << "\n";
		file << "	//stack counter for semantic action stack\n";
		file << "	int\t\t\t\t\t\t\tSSLR_semStackCnt;\n";
		file << "\n";
	}

	//Debug-Mode?
	if( argv_str->debug == true )
	{
		//die rechten Seiten
		file << "\t//Alternatives\n";

		//Alle rechten Seiten (nur Debug)
		for( i = 0; i < prod2_anz; i++ )
		{
			//Zeiger auf PROD2_STR
			p2 = &prod2_str[i];

			//Zeiger auf PROD_STR
			p1 = &prod_str[p2->prod_index];

			//Den Namen der Alternative holen
			name = hash_tab->getName( p1->hash_index );

			//Nummer der Alternative
			id = i - p1->prod2_index;

			//Namen generieren
			sprintf( puf1, "%s%d", name, id );

			//Initialisieren
			file << "\tstatic char *\t\t\t\tSSLR_" << puf1 << "Ri[" << p2->riAnz << "];\n";
		}
	}

	//Alle Follow-Mengen ausgeben
	file << "\n	//all follow sets for the error recovery\n";

	//Alle Follow-Mengen ausgeben
	for( i = 0; i < prod_anz; i++ )
	{
		//Zeiger auf PROD_STR
		p1 = &prod_str[i];

		//Den Namen der Alternative holen
		name = hash_tab->getName( p1->hash_index );

		//Anzahl der Follow-Elemente
		followAnz = hash_tab->getFollowAnz( p1->hash_index );

		//Initialisieren
		file << "\tstatic long\t\t\t\t\tSSLR_" << name << "Follow[" << followAnz << "];\n";
	}

	//Private Methoden
	file << "\n\n\t//private methods\n\n\n";


	//Methoden der Klasse
file << "\n";
file << "	//push a new state on parse stack\n";
file << "	void SSLR_push( long );\n";
file << "\n";
file << "	//pop elements from parse stack\n";
file << "	long SSLR_pop( int );\n";
file << "\n";
file << "	//begin parsing\n";
file << "	int SSLR_parse( void );\n";
file << "\n";
file << "	//shift a new state onto the parse stack\n";
file << "	void SSLR_shift( SSLR_LR_TAB * );\n";
file << "\n";
file << "	//reduce with a production\n";
file << "	void SSLR_reduce( SSLR_LR_TAB * );\n";
file << "\n";
file << "	//error handling\n";
file << "	void SSLR_error( void );\n";
file << "\n";

	//Debug-Version?
	if( argv_str->debug == true )
	{
		//Debug-Makro ausgeben
		file << "#ifdef SSLR_DEBUG\n";

		//Ausgaberoutine für Alternativen
		file << "	//output an alternative\n";
		file << "	void SSLR_outputAlt( SSLR_NTERM_STR * );\n";

		//Debug-Makro ausgeben
		file << "#endif\n\n";
	}

	//Gibt es überhaupt Fehlerangaben?
	if( error_anz )
	{
		//Ja, Hilfsroutinen ausgeben
		file << "	//shift a new state onto the parse stack\n";
		file << "	void SSLR_errShift( long );\n";
		file << "\n";
		file << "	//pop elements from the parse stack untill term may be shifted\n";
		file << "	void SSLR_errPop( long );\n";
		file << "\n";
		file << "	//delete next terminal\n";
		file << "	void SSLR_errDelete( void );\n";
		file << "\n";
		file << "	//delete all terminals untill term is found\n";
		file << "	void SSLR_errOverRead( long );\n";
		file << "\n";
	}

	//semantische Aktionen?
	if( semActCnt && ( union_str->anz > 1 ) )
	{
		//Ja, Prototypen ausgeben
		file << "\t//perform semantic actions associated with the production\n";
		file << "\t//to be reduced\n";
		file << "\tvoid SSLR_doSemAct( SSLR_LR_TAB * );\n";
		file << "\n";
	}

file << "	//get next terminal\n";
file << "	int SSLR_getTerminal( void );\n";
file << "\n";
file << "\n";
file << "\n";
file << "public:\n";
file << "\n";
file << "	//public methods \n";
file << "	SSLRParser( SSLEXScanner * );\n";
file << "	~SSLRParser();\n";
file << "\n";
file << "	//resets the whole parser\n";
file << "	void reset( void );\n";
file << "\n";
file << "	//start parsing\n";
file << "	int yyParse( void );\n";
file << "};\n";
file << "\n";
file << "\n";

	//Ende des Header-Files
	file << "#endif\n\n";
}

