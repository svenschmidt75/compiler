/*
	Projekt		:	LR-Parser-Generator
	Modul		:	LR.H
	Sprache		:	ANSI-C
	Compiler	:	ATARI: PureC V1.1
					   PC: Borland C++ 5.0
*/




/************/
/* Includes */
/************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ext.h>
#include <tos.h>
#include <portab.h>


/*****************/
/* Deklarationen */
/*****************/


/* Copyright-Meldung */
#define INFO	"\nLR-Parser-Generator SSLR V1.00û"\
				"\n(C) 1997,1998 by Sven Schmidt"


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
#define PROD_SEP_ERR				15
#define PROD_BEGINN					16
#define PREC_PRIORI					17
#define MAX_ANZ_RI_ERR				18
#define EBNF_SYM_ERR				19
#define ERROR_ERR					20
#define LR_ERR						21
#define TOO_MANY_EBNF_SYMB			22
#define NOT_DEFINED_NTERM			23
#define NOT_USED_NTERM				24
#define NOT_DEFINED_PREC_TERM		25
#define YET_DEFINED_TERM			26
#define	CANNOT_CLOSE_BNF_FILE		27
#define UNTERMINATED_TERMINAL		28
#define BAD_ALTERNATIVE				29
#define	CANNOT_OPEN_H_FILE			30
#define CANNOT_CLOSE_H_FILE			31
#define	CANNOT_OPEN_PARSER_FILE		32
#define CANNOT_CLOSE_PARSER_FILE	33
#define	CANNOT_CLOSE_ERR_FILE		34
#define	UNION_ERR					35
#define UNION_DBL					36
#define TERM_TYPE_ERR				37
#define NTERM_TYPE_ERR				38
#define INIT_ERR					39
#define INCL_ERR					40
#define TERM_IS_NTERM				41
#define NTERM_IS_TERM				42
#define INIT_DBL					43
#define INCL_DBL					44
#define SEM_ACT_ERR					45
#define UNKNOWN_ATTRIBUTE			46
#define MISSING_NTERM_ATTRIBUTE		47
#define SX_ATTRIBUTE_ERROR			48
#define MISSING_UNION_STRUCT		49
#define BAD_SEM_ACT					50
#define	CANNOT_OPEN_BNF_FILE		51
#define	ALL_ERR						52
#define	OR_OPERATOR					53
#define	PROD_END					54



/* LR-Parser */
#define LR							0

/* LALR-Parser */
#define LALR						1

/* SLR-Parser */
#define SLR							2


/* Typ des Elements */
#define TERM						0	/* Terminal */
#define NTERM						1	/* Nichtterminal */
#define OK							2	/* Eingabe war OK */
#define END							3	/* Ende der Eingabe */


/* fÅr EPSILON */
#define E_ON						1	/* EPSILON */
#define E_OFF						0	/* Kein EPSILON */


/* AssoziativitÑt */
#define LEFT						0	/* Links-Assoziativ */
#define RIGHT						1	/* Rechts-Assoziativ */



/* ZustÑnde in der LR-Tabelle */
#define ERROR						0	/* Fehler */
#define SHIFT						1	/* Schieben */
#define REDUCE						2	/* Reduzieren */


/* fÅr die Info-Datei */
#define ON							0	/* Ein */
#define OFF							1	/* Aus */
#define DSCREEN						0	/* Ausgabe auf den Bildschirm */
#define DFILE						1	/* Ausgabe in Datei */
#define DPRINTER					2	/* Ausgabe auf den Drucker */



/* Max. zulÑssige Anzahl der Elemente auf der rechten Seite */
#define MAX_ANZ_RI					20



/****************/
/* Definitionen */
/****************/


typedef struct
{
	/*
		0: Error
		1: Shift
		2: Reduce
	*/
	WORD	state;

	/*
		Shift	: zu schiebendes Terminal
		Reduce	: Index der Produktion
	*/
	ULONG	value;

}LR_TAB;


typedef struct
{
	/* Anzahl der Elemente */
	ULONG	anz_elem;

	/*
		0: Epsilon ist nicht in term_arr
		1: Epsilon ist in term_arr
	*/
	WORD	epsilon;

	/* Array der Terminale */
	LONG	*term_arr;

}FIRST_STR;


typedef struct
{
	/* Zeile der Definition */
	LONG	zeile;

	/* Inder in der NTERM_STR */
	LONG	nterm_index;

	/* Position der semmantischen Aktion auf der rechten Seite */
	WORD	right_pos;

	/* 1, wenn EBNF-Symbol { } */
	WORD	ebnf;

	LONG	term;
	
	/* Zeiger auf Text der semmantischen Aktion */
	BYTE	*sem_act_text;

}SEM_ACT;


typedef struct
{
	/* Index der Produktion in der NTERM_STR */
	LONG		nterm_index;

	/* FIRST-Struktur zu der Produktion */
	FIRST_STR	first;

}NTERM_FIRST;


typedef struct
{
	/* Index der Produktion in NTERM_STR */
	LONG		nterm_index;

	/* Position des Punktes */
	WORD		pkt_pos;

	/* Hash-Index des Elements nach dem Punkt */
	LONG		elem;

	/* Folgezustand des Items */
	LONG		next_state;

	/* Die Lookaheads(LALR/LR) des Items */
	FIRST_STR	lookahead;

}ITEM_STR;


typedef struct
{
	/* Anzahl der Kerne in der HÅlle */
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

	/* Position des Punktes im Item */
	WORD				pkt_pos;

	/* Zeiger auf Fehlertext */
	BYTE				*err_msg;

	/* Terminal fÅr Use x instead of y */
	LONG				term;

	/*
		Beeinflussung der Fehlerbehandlung durch den User
		type[0] : |=  1: INSERT(Terminal)
		type[0] : |=  2: OVERREAD(Terminal)
		type[0] : |=  4: DELETE
		type[0] : |=  8: EXIT
		type[0] : |= 16: SEM
		type[1] : Hash-Index des Terminals
	*/
	LONG				type[2];

	/* Zeile der Definition */
	ULONG				zeile;

	/* Zeiger auf ERROR_STR mit der gleichen Produktion */
	struct error_str	*error_str;

}ERROR_STR;


/* Die Struktur fÅr die Grammatik-Fehler */
typedef struct
{
	/*
		SHIFT : Shift-/Reduce-Fehler
		REDUCE: Reduce-/Reduce-Fehler
	*/
	WORD	type;

	/*
		SHIFT : Schieben
		REDUCE: Reduzieren
	*/
	WORD	state;

	/* Nummer der HÅlle, in dem der Fehler auftrat */
	LONG	hulle;

	/* Terminale, bei denen der Fehler auftrat */
	LONG	*term;

	/* Anzahl der Terminale */
	WORD	term_count;

	/* Produktion, bei der der Fehler auftritt */
	LONG	prod1;

	/* Nummer der Alternative, -1 hat keine Alternative */
	WORD	prod1_alt;

	/* Zweite Produktion bei Reduce-/Reduce-Fehler */
	LONG	prod2;

	/* Nummer der Alternative, -1 hat keine Alternative */
	WORD	prod2_alt;

}GRA_ERR_STR;


typedef struct
{
	/* Index der Produktion in der Hash-Tabelle */
	LONG		hash_tab_index;

	/* PrioritÑt der Produktion */
	WORD		priori;

	/* AssoziativitÑt der Produktion */
	WORD		assoc;

	/* Nummer der Alternative */
	WORD		alt;

	/* Index der NTERM_STR, in dem die folgende Alternative steht */
	LONG		post_index;

	/* Nummer der Produktion( ohne Alternativen, laufende Nummer ) */
	ULONG		prod_index;

	/* Rechte Seite der Produktion */
	LONG		ri[MAX_ANZ_RI];

	/* Zeiger auf die semmantischen Aktionen */
	BYTE		**sem_act;

	/* Anzahl der Elemente auf der rechten Seite */
	WORD		anz_ri;

	/* Beinhaltet die FOLLOW-Menge der Produktion */
	FIRST_STR	follow;

}NTERM_STR;


typedef struct
{
	/* Index des Terminals in der Hash-Tabelle */
	LONG		hash_tab_index;

	/* PrioritÑt des Terminals */
	WORD		priori;

	/* AssoziativitÑt des Terminals */
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

	/*
		Typ des Terminals/Nichtterminals
		%term_type, %nterm_type
	*/
	LONG		type;

	/* Zeiger auf TERM_STR */
	TERM_STR	*term_str;

	/* HÅllen, die mit name erzeugt wurden */
	LONG		*hulle_arr;

	/* Anzahl der HÅllen in hulle_arr */
	LONG		hulle_arr_count;

	/*
		Zeilennummer, in der der Typ des Terminals/Nichtterminals fest-
		gelegt wurde.
		Wird fÅr die Fehlermeldung benîtigt.
	*/
	ULONG		zeile;

}HASH_TAB;


/* Struktur fÅr die Info-Datei */

typedef struct
{
	/* Anzeigen auf Bildschirm, Drucker oder Datei */
	WORD	dest;

	/* Anzeigen auf Bildschirm, Drucker oder Datei */
	WORD	err_dest;

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
	LONG	hash_tab_size;

}LR_INFO;