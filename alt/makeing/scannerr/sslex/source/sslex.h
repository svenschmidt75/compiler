/*
	Projekt		:	Scanner-Generator
	Modul		:	SSLEX.H
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	24.07.1997
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/



/************/
/* Includes */
/************/

#include <portab.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>


/******************
* Fehlermeldungen *
******************/

#define NO_MEMORY				0



/* Startsymbol ist vom Inclusive-Typ */
#define INCLUSIVE 				0

/* Startsymbol ist vom Exclusive-Typ */
#define EXCLUSIVE 				1


/* EPSILON */
#define _EPSILON			1


/********************
* ZustandsÅbergÑnge *
*********************/

/* Vom Start-Zustand aus */
#define START_STATE			-1

/* Vom Endzustand aus */
#define FINIT_STATE			-2

/* Einen neuen Start-Zustand erzeugen */
#define NEW_START_STATE		-3

/* Einen neuen Finit-Zustand erzeugen */
#define NEW_FINIT_STATE		-4


/**********************************************************************
* Breite des Array, das die Bits fÅr die öbergangszeichen beinhaltet. *
************************************************************************/

/* Anzahl der Bytes fÅr eine Zeichenklasse */
#define CCL_WIDTH			32


/* RÅckgabewerte vom Scanner */
#define DO_INCL_SC			10
#define DO_EXCL_SC			11
#define DO_PREDEF			12
#define DO_DEFCHAR			13


/* Typ der Zustandsmenge */
#define	FINIT				1
#define LOOKAHEAD			2



/*****************/
/* Deklarationen */
/*****************/


/* Struktur fÅr eine Zeichenklasse */
typedef struct
{
	/* Array mit den Bits fÅr die öbergangszeichen */
	UBYTE		ccl[CCL_WIDTH];

	/* Niedrigstes Bit, das in ccl gesetzt ist */
	UBYTE		lw_char;

	/* Hîchstes Bit, das in ccl gesetzt ist */
	UBYTE		hi_char;

}CCL_STR;


/* Struktur fÅr die Scanner-Tabelle */
typedef struct
{
	/* Index in DSTATES, bezeichnet die Zustandsmenge */
	WORD		dstates_index;

	/* Zeichenmenge, bei der ein ZustandsÅbergang zur Zustandsmenge dstates_index auftritt */
	CCL_STR		ccl_str;

}DTRAN;


/* Struktur fÅr die ZustÑnde */
typedef struct
{
	/* Das Bitfeld mit den ZustÑnden */
	UBYTE		*states;

	/* Anzahl der ZustÑnde in diesem Bit-Feld */
	WORD		count;

	/* Zustandsmenge markiert / unmarkiert */
	WORD		marked;

	/*
		1: Beinhaltet Finit-Zustand
		2: Beinhaltet Lookahead-Zustand
	*/
	WORD		type;

	/*
		Index in der Gruppe der Zustandsmengen.
		Dient der Zustandsminimierung in einem DEA((MIN_DFA)).
	*/
	WORD		group;

	/* Struktur fÅr die öbergÑnge der Zustandsmengen */
	DTRAN		*dtran;

	/* Anzahl der Elemente in dtran */
	WORD		dtran_count;

}DSTATES;


/* Sprung-Struktur */
typedef struct
{
	/* Zustand, der von diesem Zustand angesprungen wird */
	LONG		bra;

	/* Array mit den öbergangs-Zeichen dieses Zustands */
	CCL_STR		ccl_str;

	/* Negierte Zeichenklasse */
	WORD		neg_ccl;

	/* Wenn any > 0 ist, ist jedes Zeichen erlaubt */
	WORD		any;

	/* ZustandsÅbergang mit EPSILON */
	WORD		eps;

}BRA_STR;


/* Die Struktur eines Zustandes im NEA */
typedef struct
{
	/* Nummer des Zustandes */
	LONG		state;

	/* FÅr jeden ZustandsÅbergang eine BRA_STR */
	BRA_STR		*bra_str;

	/* Anzahl der ZustandsÅbergÑnge */
	WORD		bra_count;

}NEA_STATE;


/* Die Struktur eines NEAs */
typedef struct
{
	/* Startzustand des Automaten */
	LONG		start_state;

	/* Endzustand des Automaten */
	LONG		finit_state;

	/* Grîûter Zustand des NEAs */
	LONG		biggest_state;

	/* Kleinster Zustand des NEAs */
	LONG		least_state;

	/* Die ZustÑnde zu diesem NEA */
	LONG		*state_arr;

	/* Anzahl der ZustÑnde in diesem NEA */
	WORD		state_arr_count;

	/* NEA als Textform */
	BYTE		*nea_text;

	/*
		Accepting-Zustand bei verbundenen NEA_STRs.
		-> rule -> re 'LOOKAHEAD_SYM'
	*/
	LONG		lookahead_state;

	/* 1: NEA darf nur am Zeilenanfang stehen */
	WORD		beg_line;

}NEA_STR;


/* Struktur, die die Startsymbole beinhaltet */
typedef struct
{
	/* Startsymbol */
	BYTE		*start_sym;

	/* Alle NEAen, die mit der SC assoziiert sind */
	LONG		*ss_nea;

	/* Anzahl der NEAen in ss_nea */
	WORD		ss_nea_count;

	/* Array mit den Finit-ZustÑnden eines jeden NEAs in dieser SC */
	LONG		*finit_states;

	/* Anzahl der Finit-ZustÑnde in diesem NEA */
	WORD		finit_states_count;

	/* Diese NEAen dÅrfen am Zeilenanfang stehen. */
	LONG		nea_all;

	/* Diese NEAen mÅssen und dÅrfen am Zeilenanfang stehen. */
	LONG		nea_nall;

	/* Die Scanner-Tabelle fÅr diese SC */
	DTRAN		*dtran;

	/* Typ des Startsymbols.
		0: Inclusive
			- Es werden auch die NEAen hier eingetragen, bei denen keine Startsymbol angegeben wurde.

		1: Exclusive
			- Es werden nur die NEAen eingetragen, die in dem Startsymbol auftauchen.
	*/
	WORD		ss_type;

	/* Wenn EOF folgt */
	WORD		eof;

}SS_STR;


/* Struktur fÅr die vordefinierten Namen von NEAen */
typedef struct
{
	/* Name des vordefinierten NEA */
	BYTE	*name;

	/* Index in der NEA_STR */
	LONG	nea_ind;

}PREDEF_NEA_STR;



/* Typ-Definition fÅr die semmantischen Aktionen */
typedef union
{

	BYTE		b;
	BYTE		*string;
	WORD		w;

	/* Index */
	LONG		l;

	/* Index der NEA_STR */
	LONG		nea_ind;

	/* Zeiger auf Zeichenklasse */
	CCL_STR		*ccl_str;

}LR_TYPE;


/**************/
/* Prototypen */
/**************/

WORD nea_bra( LONG nea_ind, LONG from, LONG to, UBYTE ccl[], WORD neg, WORD any, WORD eps );
LR_TYPE do_sem_act( LONG prod, WORD lr_sem_count, LR_TYPE *lr_val, LR_TYPE *lr_sem_stack );
WORD insert_nea_sc( LONG nea_ind, LONG sc_ind, WORD type, WORD flag );
WORD e_closure( NEA_STR *nea_str, WORD *states, WORD states_count );
LONG insert_predef_nea( BYTE *nea_string, LONG nea_ind );
WORD insert_re( LONG nea_ind, BYTE *start, BYTE *end );
VOID insert_ccl( CCL_STR *ccl, BYTE from, BYTE to );
LONG rep_nea( LONG nea_ind1, LONG from, LONG to );
LONG link_nea( LONG nea_ind1, LONG nea_ind2 );
LONG or_nea( LONG nea_ind1, LONG nea_ind2 );
WORD make_nea_once_or_never( LONG nea_ind );
WORD read_scanner_file( BYTE *filename );
LONG get_predef_nea( BYTE *nea_string );
LONG insert_sc( BYTE s_c[], WORD type );
WORD make_nea_optional( LONG nea_ind );
LONG new_nea_state( LONG nea_ind );
WORD make_nea_once( LONG nea_ind );
WORD if_predef_nea( LONG nea_ind );
WORD output_error( WORD err_nr );
VOID negate_ccl( CCL_STR *ccl );
VOID check_ccl( CCL_STR *ccl );
VOID free_nea( LONG nea_ind );
LONG copy_nea( LONG nea_ind );
VOID do_defchar( WORD flag );
LONG get_sc_ind( BYTE *s_c );
WORD extract_word( VOID );
WORD gen_d( SS_STR *ss );
WORD do_sc( WORD type );
WORD finish_sc( VOID );
LONG init_nea( VOID );
WORD gen_dea( VOID );
WORD min_dfa( VOID );
WORD delete( VOID );