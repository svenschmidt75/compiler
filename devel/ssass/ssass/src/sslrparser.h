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
#include <fstream>
#include <iomanip>
#include <vector>
#include "ctwooperandopcode.h"
#include "coneoperandopcode.h"
#include "cnooperandopcode.h"
#include "cdcdatasingle.h"
#include "cdcdatadouble.h"
#include "sslexscanner.h"
#include "cdcdatabyte.h"
#include "cdcdataword.h"
#include "cdcdatalong.h"
#include "copcode.h"
#include "clabel.h"
#include "ssass.h"


//Nonterminal defines for the jump table
#define start	0
#define extSize	1
#define numList	2
#define section2	3
#define section3	4
#define section4	5
#define adrMode	6
#define auxsym_1	7
#define auxsym_2	8
#define auxsym_3	9
#define section2List	10
#define section3List	11
#define memnonic	12
#define initSSASS	13
#define program	14
#define section32	15
#define section11	16
#define section12	17


//Nontermials for the LR table
#define start_0	0
#define program_0	1
#define initSSASS_0	2
#define section11_0	3
#define auxsym_1_0	4
#define auxsym_1_1	5
#define section12_0	6
#define auxsym_2_0	7
#define auxsym_2_1	8
#define auxsym_2_2	9
#define auxsym_2_3	10
#define section2_0	11
#define section2List_0	12
#define section2List_1	13
#define memnonic_0	14
#define memnonic_1	15
#define memnonic_2	16
#define memnonic_3	17
#define memnonic_4	18
#define memnonic_5	19
#define memnonic_6	20
#define memnonic_7	21
#define memnonic_8	22
#define memnonic_9	23
#define memnonic_10	24
#define memnonic_11	25
#define memnonic_12	26
#define memnonic_13	27
#define memnonic_14	28
#define memnonic_15	29
#define memnonic_16	30
#define memnonic_17	31
#define memnonic_18	32
#define memnonic_19	33
#define memnonic_20	34
#define memnonic_21	35
#define memnonic_22	36
#define memnonic_23	37
#define memnonic_24	38
#define memnonic_25	39
#define memnonic_26	40
#define memnonic_27	41
#define memnonic_28	42
#define memnonic_29	43
#define memnonic_30	44
#define memnonic_31	45
#define memnonic_32	46
#define memnonic_33	47
#define memnonic_34	48
#define memnonic_35	49
#define memnonic_36	50
#define memnonic_37	51
#define memnonic_38	52
#define memnonic_39	53
#define memnonic_40	54
#define memnonic_41	55
#define memnonic_42	56
#define memnonic_43	57
#define memnonic_44	58
#define memnonic_45	59
#define memnonic_46	60
#define memnonic_47	61
#define memnonic_48	62
#define memnonic_49	63
#define memnonic_50	64
#define memnonic_51	65
#define memnonic_52	66
#define memnonic_53	67
#define memnonic_54	68
#define memnonic_55	69
#define memnonic_56	70
#define memnonic_57	71
#define memnonic_58	72
#define memnonic_59	73
#define memnonic_60	74
#define memnonic_61	75
#define memnonic_62	76
#define memnonic_63	77
#define memnonic_64	78
#define extSize_0	79
#define extSize_1	80
#define extSize_2	81
#define extSize_3	82
#define extSize_4	83
#define extSize_5	84
#define section3_0	85
#define section3_1	86
#define section3List_0	87
#define section3List_1	88
#define section32_0	89
#define section32_1	90
#define section32_2	91
#define auxsym_3_0	92
#define numList_0	93
#define numList_1	94
#define section4_0	95
#define adrMode_0	96
#define adrMode_1	97
#define adrMode_2	98
#define adrMode_3	99
#define adrMode_4	100
#define adrMode_5	101
#define adrMode_6	102
#define adrMode_7	103
#define adrMode_8	104
#define adrMode_9	105
#define adrMode_10	106
#define adrMode_11	107
#define adrMode_12	108


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
	static const unsigned long	SSLR_anzLRStates = 296;

	//Number of terminals = number of columns in lrTab
	static const unsigned long	SSLR_anzTerm = 93;

	//Number of productions = number of rows in jumpTab
	static const unsigned long	SSLR_anzNTerm = 18;

	//Number of alternatives
	static const unsigned long	SSLR_anzProd = 109;


	//Actions in lrTab
	static const int			LR_ERROR = 1;
	static const int			LR_SHIFT = 2;
	static const int			LR_REDUCE = 3;
	static const int			LR_ACCEPT = 4;


	//maximum stack size
	static const int			SSLR_maxStackSize = 100;

#ifdef SSLR_DEBUG
	//longest right side of all alternatives
	static const int			SSLR_maxAltRightSideWidth = 82;

	//longest terminal or nonterminal
	static const int			SSLR_maxElemWidth = 15;

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
	long						SSLR_dbgSp;
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
	static char *				SSLR_program0Ri[6];
	static char *				SSLR_initSSASS0Ri[1];
	static char *				SSLR_section110Ri[1];
	static char *				SSLR_auxsym_10Ri[2];
	static char *				SSLR_auxsym_11Ri[1];
	static char *				SSLR_section120Ri[1];
	static char *				SSLR_auxsym_20Ri[3];
	static char *				SSLR_auxsym_21Ri[3];
	static char *				SSLR_auxsym_22Ri[3];
	static char *				SSLR_auxsym_23Ri[1];
	static char *				SSLR_section20Ri[2];
	static char *				SSLR_section2List0Ri[1];
	static char *				SSLR_section2List1Ri[2];
	static char *				SSLR_memnonic0Ri[5];
	static char *				SSLR_memnonic1Ri[5];
	static char *				SSLR_memnonic2Ri[5];
	static char *				SSLR_memnonic3Ri[5];
	static char *				SSLR_memnonic4Ri[5];
	static char *				SSLR_memnonic5Ri[5];
	static char *				SSLR_memnonic6Ri[5];
	static char *				SSLR_memnonic7Ri[5];
	static char *				SSLR_memnonic8Ri[5];
	static char *				SSLR_memnonic9Ri[5];
	static char *				SSLR_memnonic10Ri[5];
	static char *				SSLR_memnonic11Ri[5];
	static char *				SSLR_memnonic12Ri[3];
	static char *				SSLR_memnonic13Ri[3];
	static char *				SSLR_memnonic14Ri[5];
	static char *				SSLR_memnonic15Ri[5];
	static char *				SSLR_memnonic16Ri[5];
	static char *				SSLR_memnonic17Ri[5];
	static char *				SSLR_memnonic18Ri[3];
	static char *				SSLR_memnonic19Ri[3];
	static char *				SSLR_memnonic20Ri[3];
	static char *				SSLR_memnonic21Ri[3];
	static char *				SSLR_memnonic22Ri[3];
	static char *				SSLR_memnonic23Ri[3];
	static char *				SSLR_memnonic24Ri[3];
	static char *				SSLR_memnonic25Ri[3];
	static char *				SSLR_memnonic26Ri[3];
	static char *				SSLR_memnonic27Ri[3];
	static char *				SSLR_memnonic28Ri[3];
	static char *				SSLR_memnonic29Ri[3];
	static char *				SSLR_memnonic30Ri[3];
	static char *				SSLR_memnonic31Ri[3];
	static char *				SSLR_memnonic32Ri[3];
	static char *				SSLR_memnonic33Ri[3];
	static char *				SSLR_memnonic34Ri[3];
	static char *				SSLR_memnonic35Ri[3];
	static char *				SSLR_memnonic36Ri[5];
	static char *				SSLR_memnonic37Ri[3];
	static char *				SSLR_memnonic38Ri[3];
	static char *				SSLR_memnonic39Ri[2];
	static char *				SSLR_memnonic40Ri[2];
	static char *				SSLR_memnonic41Ri[5];
	static char *				SSLR_memnonic42Ri[3];
	static char *				SSLR_memnonic43Ri[5];
	static char *				SSLR_memnonic44Ri[3];
	static char *				SSLR_memnonic45Ri[1];
	static char *				SSLR_memnonic46Ri[1];
	static char *				SSLR_memnonic47Ri[3];
	static char *				SSLR_memnonic48Ri[3];
	static char *				SSLR_memnonic49Ri[3];
	static char *				SSLR_memnonic50Ri[3];
	static char *				SSLR_memnonic51Ri[3];
	static char *				SSLR_memnonic52Ri[3];
	static char *				SSLR_memnonic53Ri[3];
	static char *				SSLR_memnonic54Ri[3];
	static char *				SSLR_memnonic55Ri[3];
	static char *				SSLR_memnonic56Ri[3];
	static char *				SSLR_memnonic57Ri[3];
	static char *				SSLR_memnonic58Ri[3];
	static char *				SSLR_memnonic59Ri[3];
	static char *				SSLR_memnonic60Ri[3];
	static char *				SSLR_memnonic61Ri[3];
	static char *				SSLR_memnonic62Ri[3];
	static char *				SSLR_memnonic63Ri[3];
	static char *				SSLR_memnonic64Ri[1];
	static char *				SSLR_extSize0Ri[1];
	static char *				SSLR_extSize1Ri[1];
	static char *				SSLR_extSize2Ri[1];
	static char *				SSLR_extSize3Ri[1];
	static char *				SSLR_extSize4Ri[1];
	static char *				SSLR_extSize5Ri[1];
	static char *				SSLR_section30Ri[2];
	static char *				SSLR_section31Ri[1];
	static char *				SSLR_section3List0Ri[1];
	static char *				SSLR_section3List1Ri[2];
	static char *				SSLR_section320Ri[4];
	static char *				SSLR_section321Ri[2];
	static char *				SSLR_section322Ri[1];
	static char *				SSLR_auxsym_30Ri[1];
	static char *				SSLR_numList0Ri[1];
	static char *				SSLR_numList1Ri[3];
	static char *				SSLR_section40Ri[1];
	static char *				SSLR_adrMode0Ri[1];
	static char *				SSLR_adrMode1Ri[1];
	static char *				SSLR_adrMode2Ri[1];
	static char *				SSLR_adrMode3Ri[3];
	static char *				SSLR_adrMode4Ri[4];
	static char *				SSLR_adrMode5Ri[4];
	static char *				SSLR_adrMode6Ri[4];
	static char *				SSLR_adrMode7Ri[4];
	static char *				SSLR_adrMode8Ri[4];
	static char *				SSLR_adrMode9Ri[4];
	static char *				SSLR_adrMode10Ri[2];
	static char *				SSLR_adrMode11Ri[2];
	static char *				SSLR_adrMode12Ri[2];

	//all follow sets for the error recovery
	static long					SSLR_startFollow[1];
	static long					SSLR_programFollow[1];
	static long					SSLR_initSSASSFollow[5];
	static long					SSLR_section11Follow[4];
	static long					SSLR_auxsym_1Follow[4];
	static long					SSLR_section12Follow[1];
	static long					SSLR_auxsym_2Follow[1];
	static long					SSLR_section2Follow[2];
	static long					SSLR_section2ListFollow[67];
	static long					SSLR_memnonicFollow[67];
	static long					SSLR_extSizeFollow[76];
	static long					SSLR_section3Follow[1];
	static long					SSLR_section3ListFollow[4];
	static long					SSLR_section32Follow[4];
	static long					SSLR_auxsym_3Follow[1];
	static long					SSLR_numListFollow[5];
	static long					SSLR_section4Follow[1];
	static long					SSLR_adrModeFollow[68];


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
//Command line arguments
ARGV_STR				*argvStr;
//Array mit den Opcodes
std::vector<COpcode *>	_opcodeArray;
//Array mit allen verwendeten Labels
std::vector<CLabel *>	_labelArray;
//prüft, ob während der Assemblierung Fehler aufgetreten sind
bool					_assErr;
//assemblierte Opcodes zählen
unsigned long			_opcodeCnt;
//Extension für einen Opcode oder für .dc. ...
COpcode::_extension		_extension;
/*
Jedes Objektfile hat einen Header, der wichtige Informationen
für den Linker bereithält.
*/
struct _SSASSHeader
{
//Länge des folgenden Strings
unsigned short	_versionLength;
//String mit der Version von SSASS
const char		*_version;
//Länge des folgenden Strings
unsigned short	_fileNameLength;
//Dateiname des Moduls
const char		*_fileName;
//Anfang des Textsegemnts
long			_text;
//Länge des Textsegments
long			_textLength;
//Anfang des DATA-Segemnts
long			_data;
//Länge des DATA-Segments
long			_dataLength;
//Anfang des BSS-Segemnts
long			_bss;
//Länge des BSS-Segments
long			_bssLength;
/*
Beginn des Auflistung der Adressen der Befehle in diesem Modul,
die auf externe Labels zugreifen und deshalb angepasst werden müssen.
*/
long			_opcodeFix;
//Länge dieses Bereichs
long			_opcodeFixLength;
/*
Wenn dieses Modul Labels exportiert, denn Name, Adresse und Zeilennummer
der zu exportierenden Labels.
*/
long			_exportLabel;
//Länge dieses Bereichs
long			_exportLabelLength;
};
/*
Beginn des Auflistung der Adressen der Befehle in diesem Modul,
die auf externe Labels zugreifen und deshalb angepasst werden müssen.
*/
struct _opcodeFixStruct
{
//Adresse des Opcodes
long		_adr;
//Zeile im Source-File
long		_line;
//Name des benötigten Labels
const char	*_labelName;
};
/*
Beginn des Auflistung der Adressen der Befehle in diesem Modul,
die auf externe Labels zugreifen und deshalb angepasst werden müssen.
*/
std::vector<struct _opcodeFixStruct>	_opcodeFixStr;
/*
Wenn dieses Modul Labels exportiert, denn Name, Adresse und Zeilennummer
der zu exportierenden Labels.
*/
struct _exportLabelStruct
{
//Name des Labels
const char	*_labelName;
//Dateiname, in dem das Label definiert wurde
const char	*_fileName;
//Zeile, in der das Label definiert wurde
long		_line;
//Adresse des Labels
long		_adr;
};
/*
In diesem Array stehen die Strukturen mit den zu exportierenden
Labels.
*/
std::vector<struct _exportLabelStruct>	_exportLabelStr;
//Dateiname des Moduls
const char								*moduleName;
//Header für das Objektfile
struct _SSASSHeader						_header;
//Adresse des nächsten Opcodes
long									_opcodeAdress;
/*
Im DATA-Segment werden mittels .dc.b, ..., .dc.d und .string statische Daten abgelegt.
Pro Zeile werden diese in CDCData abgelegt, welche wiederum in diesem Array
abgelegt werden (für die Code-Ausgabe).
*/
std::vector<CDCData *>					_CDCDataArray;
//nach dem Erkennen der Extension die entsprechende CDCData-Klasse erzeugen
CDCData									*_CDCDataDATA;
//Adress-Offset im Daten-Segment
long									_dataAdress;
//initialisieren
void init( void );
//deinitialisieren
void deinit( void );
//prüft, ob das Label bereits eingetragen wurde
CLabel *findLabel( const char * );
//prüft alle Labels auf korrekte Verwendung
void checkLabel( void );
//hier wird die Adresse eines Labels in die Opcodes eingetragen
void insertLabels( void );
/*
Opcodes, die per .import auf externe Labels zugreifen, können erst
vom Linker verarbeitet werden, weil jetzt die Adresse noch gar nicht
bekannt sein kann. Dazu wird eine Struktur aufgefüllt, die alle nötigen
Informationen enthält und im Objektfile hinter den Opcodes aufgelistet
wird, so dass der Linker SSLINK problemlos darauf zugreifen kann.
*/
void fillOpcodeFixStruct( void );
/*
Wenn dieses Modul Labels exportiert, also Labels per .export deklariert wurden,
dann deren Adresse, Zeile im Modul und den Namen exportieren.
*/
void fillExportLabelStruct( void );
//Header des Objektfiles erzeugen
void fillHeader( void );
//Objekt-File ausgeben
void writeObjectFile( void );
//eine COpcode-Klasse ins Array eintragen und Adressen etc. setzen
void insertOpcode( COpcode *, long );
public:
//dem Parser einige Variablen übergeben
void giveArgs( ARGV_STR * );
};


#endif

