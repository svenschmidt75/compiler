/*
	Projekt		:	LR-Parser-Generator V1.00û
	Modul		:	LR.H
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1
*/

/* Deklarationen */


/* Copyright-Meldung */
#define INFO	"\nLR-Parser-Generator V1.00û"\
				"\n(C) by Sven Schmidt"


/*******************/
/* Fehlermeldungen */
/*******************/


#define NO_GRAMMAR_FILE_NAME		0
#define	GRAMMAR_FILE_NOT_FOUND		1
#define	CANNOT_OPEN_GRAMMAR_FILE	2
#define	CANNOT_READ_GRAMMAR_FILE	3
#define	CANNOT_CLOSE_GRAMMAR_FILE	4
#define	CANNOT_OPEN_INFO_FILE		5
#define	CANNOT_READ_INFO_FILE		6
#define	CANNOT_CLOSE_INFO_FILE		7
#define CANNOT_CLOSE_FILE			8
#define NOT_ENOUGH_MEMORY			9
#define INVALID_CHAR				10
#define UNKNOWN_INFO_FILE_STATEMENT	11
#define HASH_TAB_ERROR				12
#define PREC_ERR					13
#define NOT_USED_TERM				14
#define DOUBLE_ERROR_MSG			15
#define PROD_SEP_ERR				16
#define PROD_BEGINN					17
#define PREC_PRIORI					18
#define MAX_ANZ_RI_ERR				19
#define EBNF_SYM_ERR				20
#define ERROR_ERR					21
#define LR_ERR						22
#define TOO_MANY_EBNF_SYMB			23
#define NOT_DEFINED_NTERM			24
#define NOT_USED_NTERM				25
#define NOT_DEFINED_PREC_TERM		26
#define YET_DEFINED_TERM			27
#define	CANNOT_CLOSE_BNF_FILE		28
#define UNTERMINATED_TERMINAL		29
#define BAD_ALTERNATIVE				30
#define	CANNOT_OPEN_H_FILE			31
#define CANNOT_CLOSE_H_FILE			32
#define	CANNOT_OPEN_PARSER_FILE		33
#define CANNOT_CLOSE_PARSER_FILE	34



/* Parser-Typ */
#define LR							0	/* LR-Parser */
#define LALR						1	/* LALR-Parser */
#define SLR							2	/* SLR-Parser */


/* Typ des Elements */
#define TERM						0	/* Terminal */
#define NTERM						1	/* Nichtterminal */
#define OK							2	/* Eingabe war OK */
#define END							3	/* Ende der Eingabe */


/* FÅr EPSILON */
#define E_ON						1	/* EPSILON */
#define E_OFF						0	/* Kein EPSILON */


/* AssoziativitÑt */
#define LEFT						0	/* Links-Assoziativ */
#define RIGHT						1	/* Rechts-Assoziativ */



/* ZustÑnde in der LR-Tabelle */
#define ERROR						0	/* Fehler */
#define SHIFT						1	/* Schieben */
#define REDUCE						2	/* Reduzieren */


/* FÅr die Info-Datei */
#define ON							0	/* Ein */
#define OFF							1	/* Aus */
#define DSCREEN						0	/* Ausgabe auf den Bildschirm */
#define DFILE						1	/* Ausgabe in Datei */
#define DPRINTER					2	/* Ausgabe auf den Drucker */



/* Max. zulÑssige Anzahl der Elemente auf der rechten Seite */
#define MAX_ANZ_RI_ELEM				20



/****************/
/* Definitionen */
/****************/


typedef struct
{
	/*	0: Error, 1: Shift; 2: Reduce */
	WORD	state;

	/*
		Zu schiebendes Terminal, bei Reduktion Anzahl der vom Stack zu
		lîschenden Terminale.
	*/
	ULONG	value;

}LR_TAB;


typedef struct
{
	/* Anzahl der Elemente */
	ULONG	anz_elem;

	/* 1: Epsilon ist in term_arr */
	WORD	epsilon;

	/* Array der Terminale */
	LONG	*term_arr;

}FIRST_STR;


typedef struct
{
	/* Index der Nichtterminals in der NTERM_STR */
	LONG		nterm_index;

	/* FIRST-Struktur */
	FIRST_STR	first;

}NTERM_FIRST;


typedef struct
{
	/* Index der Produktion in NTERM_STR */
	LONG		nterm_index;

	/* Position des Punktes */
	WORD		pkt_pos;

	/* Terminal, bei dem reduziert wird */
	FIRST_STR	term;

}ITEM_STR;


typedef struct
{
	/* Anzahl der Kerne im Item */
	ULONG		anz_kitems;

	/* Anzahl der Items */
	ULONG		anz_items;

	/* Array mit den Items */
	ITEM_STR	*item_str;

	/* Index in der ERROR_STR -> fÅr Fehlerbehandlung */
	LONG		*error_index;

	/* Anzahl der Indizes fÅr die Fehlerbehandlung */
	WORD		anz_error_index;

	/*
		Nichtterminal/Terminal, durch das die HÅlle erzeugt wurde.
		 -> GOTO( ..., Nterm/Term )
	*/
	LONG		nterm;

	/*
		Index der HÅlle, durch die diese HÅlle erzeugt wurde.
		-> GOTO( Ix, ... )
	*/
	ULONG		index;

}HULLE_STR;


typedef struct error_str
{
	/* Index der Produktion in NTERM_STR */
	LONG				nterm_index;

	/* Position des Punktes */
	WORD				pkt_pos;

	/* Zeiger auf Fehlertext */
	BYTE				*err_msg;

	/* Terminal fÅr Use x instead of y */
	LONG				term;

	/* Zeiger auf ERROR_STR mit der gleichen Produktion */
	struct error_str	*error_str;

}ERROR_STR;


typedef struct
{
	/* Index in der Hash-Tabelle */
	LONG		hash_tab_index;

	/* PrioritÑt der Produktion */
	WORD		priori;

	/* AssoziativitÑt der Produktion */
	WORD		assoc;

	/* Nummer der Alternative */
	WORD		alt;

	/* Index der NTERM_STR, in dem die folgende Alternative steht */
	LONG		post_index;

	/* Nummer der Produktion( ohne Alternativen ) */
	ULONG		prod_index;

	/* Rechte Seite der Produktion */
	LONG		ri[MAX_ANZ_RI_ELEM];

	/* Anzahl der Elemente auf der rechten Seite */
	WORD		anz_ri;

	/* Beinhaltet die FOLLOW-Mengen */
	FIRST_STR	follow;

}NTERM_STR;


typedef struct
{
	/* Index in der Hash-Tabelle */
	LONG		hash_tab_index;

	/* PrioritÑt der Produktion */
	WORD		priori;

	/* AssoziativitÑt der Produktion */
	WORD		assoc;

}TERM_STR;


typedef struct
{
	/* Name der Produktion / des Terminals */
	BYTE		*name;

	/* Nichtterminal wurde definiert */
	WORD		left_def;

	/* Nichtterminal / Terminal wurde benutzt */
	WORD		def;

	/* Index des Nichtterminals in der nterm_str */
	LONG		nterm_index;

	/* Zeiger auf TERM_STR */
	TERM_STR	*term_str;

}HASH_TAB;


/* Struktur fÅr die Info-Datei */

typedef struct
{
	/* Anzeigen auf Bildschirm bzw. Datei */
	WORD	dest;

	/* Anzeigen der FOLLOW-Mengen */
	WORD	follow;

	/* Anzeigen der FIRST-Mengen */
	WORD	first;

	/* LR-/LALR-/SLR-Parser erzeugen */
	WORD	type;

	/* Grammatik ausgeben */
	WORD	output_grammar;

	/* Anzeigen der LR-Items */
	WORD	lr_items;

	/* Analyse-Prozess-Meldungen ausgeben */
	WORD	analyze;

	/* LR-Tabelle komprimieren */
	WORD	compress;

	/* Ausgabe der LR-Tabelle */
	WORD	lr_tab;

	/* DEBUG-Version des LR-Parser erzeugen */
	WORD	debug;

	/* Max. Anzahl der Zeichen pro Zeile */
	WORD	max_char;

	/* Speicher fÅr Grammatik-Hash-Tabelle */
	ULONG	hash_tab_size;

}LR_INFO;