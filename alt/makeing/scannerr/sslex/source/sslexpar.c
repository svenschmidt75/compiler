/*
LR-Parser-Generator V1.00û
(C) 1997,1998 by Sven Schmidt

Parsertyp     : SLR 
Datum         : 27.03.1998
Zeit          : 12:05
Debug-Version : Ja
Komprimiert   : Nein
*/


/************/
/* Includes */
/************/

#include <portab.h>
#include <stdio.h>
#include <stdlib.h>
#include <tos.h>
#include "SSLEX.H"



/****************/
/* Definitionen */
/****************/

/* Grîsse des Stacks */
#define STACK_SIZE	100


/* Anzahl der Terminale */
#define ANZ_TERM	26
#define OK			0
#define ERROR		1


/* Ausgabemedien */
#define DEST_SCREEN	0/* Ausgabe auf Bildschirm */
#define DEST_FILE	1/* Ausgabe in Datei */
#define DEST_PRN	2/* Ausgabe auf Drucker */



/**************/
/* Prototypen */
/**************/

VOID shift( LONG shift_state, LONG old_lahead, LONG lahead );
VOID output_prod( LONG prod_index );
VOID reduce( LONG prod_index );
VOID push( LONG value );
WORD do_parse( VOID );
WORD error( VOID );
LONG pop( VOID );



/*****************/
/* Deklarationen */
/*****************/

/* Lookahead */
WORD	lookahead;

/* Aktueller Status */
LONG	state;

/* Stackpointer */
LONG	stackpointer = -1;

/* Stack */
LONG	stack[STACK_SIZE];

/* Anzahl der Fehlermeldungen */
WORD	anz_error = 0;

/* Flag fÅr Fehlermeldungen */
WORD	error_flg = 0;

/* Altes Lookahead */
WORD	old_lookahead = - 1;

/* Alter Status */
LONG	old_state;

/* Ausgabemedium festlegen */
WORD	dest_state = DEST_FILE;

/* fÅr Log */
FILE	*output_file;

/* Der Stack fÅr die semmantischen Aktionen */
LR_TYPE	lr_sem_stack[100];

/* Stack-Conter */
WORD	lr_sem_count = 0;

/* Variable, die vom Scanner zurueckgeliefert wird */
LR_TYPE	scan_val;

/* fÅr die interne Verwaltung */
LR_TYPE	lr_val;

/* Attribut-Stack */
LONG	a_stack[10];

/* Keine semmantischen Aktionen im Fehlerfall */
WORD	sem_flg = 0;


#include "SSLEXPAR.H"


/****************/
/* SLR -Tabelle */
/****************/

LONG	lr_tab[66][ANZ_TERM] = {
/*  0 */ {       init_sslex,       init_sslex,       init_sslex,       init_sslex
,       init_sslex,       init_sslex,       init_sslex,       init_sslex
,       init_sslex,       init_sslex,       init_sslex,       init_sslex
,       init_sslex,       init_sslex,       init_sslex,       init_sslex
,       init_sslex,       init_sslex,       init_sslex,       init_sslex
,       init_sslex,       init_sslex,       init_sslex,       init_sslex
,       init_sslex,       init_sslex },
/*  1 */ {            start,            start,            start,            start
,            start,            start,            start,            start
,            start,            start,            start,            start
,            start,            start,            start,            start
,            start,            start,            start,            start
,            start,            start,            start,            start
,            start,            start },
/*  2 */ {                0,                0,               -6,               -8
,               -9,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/*  3 */ {             goal,             goal,             goal,             goal
,             goal,             goal,             goal,             goal
,             goal,             goal,             goal,             goal
,             goal,             goal,             goal,             goal
,             goal,             goal,             goal,             goal
,             goal,             goal,             goal,             goal
,             goal,             goal },
/*  4 */ {                0,                0,              -22,              -24
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/*  5 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/*  6 */ {      sslexrule_3,      sslexrule_3,      sslexrule_3,      sslexrule_3
,      sslexrule_3,      sslexrule_3,      sslexrule_3,      sslexrule_3
,      sslexrule_3,      sslexrule_3,      sslexrule_3,      sslexrule_3
,      sslexrule_3,      sslexrule_3,      sslexrule_3,      sslexrule_3
,      sslexrule_3,      sslexrule_3,      sslexrule_3,      sslexrule_3
,      sslexrule_3,      sslexrule_3,      sslexrule_3,      sslexrule_3
,      sslexrule_3,      sslexrule_3 },
/*  7 */ {      sslexrule_5,      sslexrule_5,      sslexrule_5,      sslexrule_5
,      sslexrule_5,      sslexrule_5,      sslexrule_5,      sslexrule_5
,      sslexrule_5,      sslexrule_5,      sslexrule_5,      sslexrule_5
,      sslexrule_5,      sslexrule_5,      sslexrule_5,      sslexrule_5
,      sslexrule_5,      sslexrule_5,      sslexrule_5,      sslexrule_5
,      sslexrule_5,      sslexrule_5,      sslexrule_5,      sslexrule_5
,      sslexrule_5,      sslexrule_5 },
/*  8 */ {  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1 },
/*  9 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/* 10 */ {           rule_3,           rule_3,           rule_3,           rule_3
,           rule_3,           rule_3,           rule_3,              -29
,              -31,              -30,           rule_3,           rule_3
,           rule_3,           rule_3,           rule_3,           rule_3
,           rule_3,           rule_3,           rule_3,           rule_3
,           rule_3,           rule_3,           rule_3,           rule_3
,           rule_3,           rule_3 },
/* 11 */ {             re_1,             re_1,             re_1,             re_1
,             re_1,             re_1,             re_1,             re_1
,             re_1,             re_1,             re_1,             re_1
,             re_1,              -14,             re_1,             re_1
,              -15,              -17,              -18,              -19
,             re_1,              -20,              -21,             re_1
,             re_1,             re_1 },
/* 12 */ {         series_1,         series_1,         series_1,         series_1
,         series_1,         series_1,         series_1,         series_1
,         series_1,         series_1,              -33,              -34
,              -35,              -36,         series_1,         series_1
,         series_1,         series_1,         series_1,         series_1
,         series_1,         series_1,         series_1,         series_1
,         series_1,         series_1 },
/* 13 */ {  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1 },
/* 14 */ {      singleton_8,      singleton_8,      singleton_8,      singleton_8
,      singleton_8,      singleton_8,      singleton_8,      singleton_8
,      singleton_8,      singleton_8,      singleton_8,      singleton_8
,      singleton_8,      singleton_8,      singleton_8,      singleton_8
,      singleton_8,      singleton_8,      singleton_8,      singleton_8
,      singleton_8,      singleton_8,      singleton_8,      singleton_8
,      singleton_8,      singleton_8 },
/* 15 */ {      singleton_9,      singleton_9,      singleton_9,      singleton_9
,      singleton_9,      singleton_9,      singleton_9,      singleton_9
,      singleton_9,      singleton_9,      singleton_9,      singleton_9
,      singleton_9,      singleton_9,      singleton_9,      singleton_9
,      singleton_9,      singleton_9,      singleton_9,      singleton_9
,      singleton_9,      singleton_9,      singleton_9,      singleton_9
,      singleton_9,      singleton_9 },
/* 16 */ {     singleton_10,     singleton_10,     singleton_10,     singleton_10
,     singleton_10,     singleton_10,     singleton_10,     singleton_10
,     singleton_10,     singleton_10,     singleton_10,     singleton_10
,     singleton_10,     singleton_10,     singleton_10,     singleton_10
,     singleton_10,     singleton_10,     singleton_10,     singleton_10
,     singleton_10,     singleton_10,     singleton_10,     singleton_10
,     singleton_10,     singleton_10 },
/* 17 */ {         string_1,         string_1,         string_1,         string_1
,         string_1,         string_1,         string_1,         string_1
,         string_1,         string_1,         string_1,         string_1
,         string_1,         string_1,         string_1,         string_1
,         string_1,         string_1,         string_1,         string_1
,         string_1,         string_1,         string_1,         string_1
,         string_1,         string_1 },
/* 18 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/* 19 */ {     singleton_13,     singleton_13,     singleton_13,     singleton_13
,     singleton_13,     singleton_13,     singleton_13,     singleton_13
,     singleton_13,     singleton_13,     singleton_13,     singleton_13
,     singleton_13,     singleton_13,     singleton_13,     singleton_13
,     singleton_13,     singleton_13,     singleton_13,     singleton_13
,     singleton_13,     singleton_13,     singleton_13,     singleton_13
,     singleton_13,     singleton_13 },
/* 20 */ {            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,              -41,            ccl_2 },
/* 21 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/* 22 */ {      sslexrule_1,      sslexrule_1,      sslexrule_1,      sslexrule_1
,      sslexrule_1,      sslexrule_1,      sslexrule_1,      sslexrule_1
,      sslexrule_1,      sslexrule_1,      sslexrule_1,      sslexrule_1
,      sslexrule_1,      sslexrule_1,      sslexrule_1,      sslexrule_1
,      sslexrule_1,      sslexrule_1,      sslexrule_1,      sslexrule_1
,      sslexrule_1,      sslexrule_1,      sslexrule_1,      sslexrule_1
,      sslexrule_1,      sslexrule_1 },
/* 23 */ {      sslexrule_4,      sslexrule_4,      sslexrule_4,      sslexrule_4
,      sslexrule_4,      sslexrule_4,      sslexrule_4,      sslexrule_4
,      sslexrule_4,      sslexrule_4,      sslexrule_4,      sslexrule_4
,      sslexrule_4,      sslexrule_4,      sslexrule_4,      sslexrule_4
,      sslexrule_4,      sslexrule_4,      sslexrule_4,      sslexrule_4
,      sslexrule_4,      sslexrule_4,      sslexrule_4,      sslexrule_4
,      sslexrule_4,      sslexrule_4 },
/* 24 */ {      sslexrule_2,      sslexrule_2,      sslexrule_2,      sslexrule_2
,      sslexrule_2,      sslexrule_2,      sslexrule_2,      sslexrule_2
,      sslexrule_2,      sslexrule_2,      sslexrule_2,      sslexrule_2
,      sslexrule_2,      sslexrule_2,      sslexrule_2,      sslexrule_2
,      sslexrule_2,      sslexrule_2,      sslexrule_2,      sslexrule_2
,      sslexrule_2,      sslexrule_2,      sslexrule_2,      sslexrule_2
,      sslexrule_2,      sslexrule_2 },
/* 25 */ {                0,                0,                0,                0
,                0,              -43,              -44,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0 },
/* 26 */ {      namelist2_1,      namelist2_1,      namelist2_1,      namelist2_1
,      namelist2_1,      namelist2_1,      namelist2_1,      namelist2_1
,      namelist2_1,      namelist2_1,      namelist2_1,      namelist2_1
,      namelist2_1,      namelist2_1,      namelist2_1,      namelist2_1
,      namelist2_1,      namelist2_1,      namelist2_1,      namelist2_1
,      namelist2_1,              -45,      namelist2_1,      namelist2_1
,      namelist2_1,      namelist2_1 },
/* 27 */ {           rule_0,           rule_0,           rule_0,           rule_0
,           rule_0,           rule_0,           rule_0,              -46
,              -31,           rule_0,           rule_0,           rule_0
,           rule_0,           rule_0,           rule_0,           rule_0
,           rule_0,           rule_0,           rule_0,           rule_0
,           rule_0,           rule_0,           rule_0,           rule_0
,           rule_0,           rule_0 },
/* 28 */ {           rule_2,           rule_2,           rule_2,           rule_2
,           rule_2,           rule_2,           rule_2,           rule_2
,           rule_2,           rule_2,           rule_2,           rule_2
,           rule_2,           rule_2,           rule_2,           rule_2
,           rule_2,           rule_2,           rule_2,           rule_2
,           rule_2,           rule_2,           rule_2,           rule_2
,           rule_2,           rule_2 },
/* 29 */ {              re2,              re2,              re2,              re2
,              re2,              re2,              re2,              re2
,              re2,              re2,              re2,              re2
,              re2,              re2,              re2,              re2
,              re2,              re2,              re2,              re2
,              re2,              re2,              re2,              re2
,              re2,              re2 },
/* 30 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -14,                0,                0
,              -15,              -17,              -18,              -19
,                0,              -20,              -21,                0
,                0,                0 },
/* 31 */ {         series_0,         series_0,         series_0,         series_0
,         series_0,         series_0,         series_0,         series_0
,         series_0,         series_0,              -33,              -34
,              -35,              -36,         series_0,         series_0
,         series_0,         series_0,         series_0,         series_0
,         series_0,         series_0,         series_0,         series_0
,         series_0,         series_0 },
/* 32 */ {      singleton_0,      singleton_0,      singleton_0,      singleton_0
,      singleton_0,      singleton_0,      singleton_0,      singleton_0
,      singleton_0,      singleton_0,      singleton_0,      singleton_0
,      singleton_0,      singleton_0,      singleton_0,      singleton_0
,      singleton_0,      singleton_0,      singleton_0,      singleton_0
,      singleton_0,      singleton_0,      singleton_0,      singleton_0
,      singleton_0,      singleton_0 },
/* 33 */ {      singleton_1,      singleton_1,      singleton_1,      singleton_1
,      singleton_1,      singleton_1,      singleton_1,      singleton_1
,      singleton_1,      singleton_1,      singleton_1,      singleton_1
,      singleton_1,      singleton_1,      singleton_1,      singleton_1
,      singleton_1,      singleton_1,      singleton_1,      singleton_1
,      singleton_1,      singleton_1,      singleton_1,      singleton_1
,      singleton_1,      singleton_1 },
/* 34 */ {      singleton_2,      singleton_2,      singleton_2,      singleton_2
,      singleton_2,      singleton_2,      singleton_2,      singleton_2
,      singleton_2,      singleton_2,      singleton_2,      singleton_2
,      singleton_2,      singleton_2,      singleton_2,      singleton_2
,      singleton_2,      singleton_2,      singleton_2,      singleton_2
,      singleton_2,      singleton_2,      singleton_2,      singleton_2
,      singleton_2,      singleton_2 },
/* 35 */ {  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,              -48,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1 },
/* 36 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,              -50
,                0,                0,                0,                0
,                0,              -45,                0,                0
,                0,                0 },
/* 37 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,              -51,                0
,                0,              -52,                0,                0
,                0,                0 },
/* 38 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,              -31,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,              -53,                0,                0,                0
,                0,                0 },
/* 39 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -55,                0,              -54
,                0,                0 },
/* 40 */ {            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2,            ccl_2,            ccl_2
,            ccl_2,            ccl_2 },
/* 41 */ {      sslexrule_0,      sslexrule_0,      sslexrule_0,      sslexrule_0
,      sslexrule_0,      sslexrule_0,      sslexrule_0,      sslexrule_0
,      sslexrule_0,      sslexrule_0,      sslexrule_0,      sslexrule_0
,      sslexrule_0,      sslexrule_0,      sslexrule_0,      sslexrule_0
,      sslexrule_0,      sslexrule_0,      sslexrule_0,      sslexrule_0
,      sslexrule_0,      sslexrule_0,      sslexrule_0,      sslexrule_0
,      sslexrule_0,      sslexrule_0 },
/* 42 */ {               sc,               sc,               sc,               sc
,               sc,               sc,               sc,               sc
,               sc,               sc,               sc,               sc
,               sc,               sc,               sc,               sc
,               sc,               sc,               sc,               sc
,               sc,               sc,               sc,               sc
,               sc,               sc },
/* 43 */ {  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1,  predef_string_1,  predef_string_1
,  predef_string_1,  predef_string_1 },
/* 44 */ {  predef_string_0,  predef_string_0,  predef_string_0,  predef_string_0
,  predef_string_0,  predef_string_0,  predef_string_0,  predef_string_0
,  predef_string_0,  predef_string_0,  predef_string_0,  predef_string_0
,  predef_string_0,  predef_string_0,  predef_string_0,  predef_string_0
,  predef_string_0,  predef_string_0,  predef_string_0,  predef_string_0
,  predef_string_0,  predef_string_0,  predef_string_0,  predef_string_0
,  predef_string_0,  predef_string_0 },
/* 45 */ {           rule_1,           rule_1,           rule_1,           rule_1
,           rule_1,           rule_1,           rule_1,           rule_1
,           rule_1,           rule_1,           rule_1,           rule_1
,           rule_1,           rule_1,           rule_1,           rule_1
,           rule_1,           rule_1,           rule_1,           rule_1
,           rule_1,           rule_1,           rule_1,           rule_1
,           rule_1,           rule_1 },
/* 46 */ {             re_0,             re_0,             re_0,             re_0
,             re_0,             re_0,             re_0,             re_0
,             re_0,             re_0,             re_0,             re_0
,             re_0,              -14,             re_0,             re_0
,              -15,              -17,              -18,              -19
,             re_0,              -20,              -21,             re_0
,             re_0,             re_0 },
/* 47 */ {                0,                0,                0,                0
,                0,                0,              -58,                0
,                0,                0,                0,                0
,                0,                0,                0,              -59
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0 },
/* 48 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,              -60
,                0,                0,                0,                0
,                0,              -45,                0,                0
,                0,                0 },
/* 49 */ {      singleton_7,      singleton_7,      singleton_7,      singleton_7
,      singleton_7,      singleton_7,      singleton_7,      singleton_7
,      singleton_7,      singleton_7,      singleton_7,      singleton_7
,      singleton_7,      singleton_7,      singleton_7,      singleton_7
,      singleton_7,      singleton_7,      singleton_7,      singleton_7
,      singleton_7,      singleton_7,      singleton_7,      singleton_7
,      singleton_7,      singleton_7 },
/* 50 */ {     singleton_11,     singleton_11,     singleton_11,     singleton_11
,     singleton_11,     singleton_11,     singleton_11,     singleton_11
,     singleton_11,     singleton_11,     singleton_11,     singleton_11
,     singleton_11,     singleton_11,     singleton_11,     singleton_11
,     singleton_11,     singleton_11,     singleton_11,     singleton_11
,     singleton_11,     singleton_11,     singleton_11,     singleton_11
,     singleton_11,     singleton_11 },
/* 51 */ {         string_0,         string_0,         string_0,         string_0
,         string_0,         string_0,         string_0,         string_0
,         string_0,         string_0,         string_0,         string_0
,         string_0,         string_0,         string_0,         string_0
,         string_0,         string_0,         string_0,         string_0
,         string_0,         string_0,         string_0,         string_0
,         string_0,         string_0 },
/* 52 */ {     singleton_12,     singleton_12,     singleton_12,     singleton_12
,     singleton_12,     singleton_12,     singleton_12,     singleton_12
,     singleton_12,     singleton_12,     singleton_12,     singleton_12
,     singleton_12,     singleton_12,     singleton_12,     singleton_12
,     singleton_12,     singleton_12,     singleton_12,     singleton_12
,     singleton_12,     singleton_12,     singleton_12,     singleton_12
,     singleton_12,     singleton_12 },
/* 53 */ {        fullccl_0,        fullccl_0,        fullccl_0,        fullccl_0
,        fullccl_0,        fullccl_0,        fullccl_0,        fullccl_0
,        fullccl_0,        fullccl_0,        fullccl_0,        fullccl_0
,        fullccl_0,        fullccl_0,        fullccl_0,        fullccl_0
,        fullccl_0,        fullccl_0,        fullccl_0,        fullccl_0
,        fullccl_0,        fullccl_0,        fullccl_0,        fullccl_0
,        fullccl_0,        fullccl_0 },
/* 54 */ {            ccl_1,            ccl_1,            ccl_1,            ccl_1
,            ccl_1,            ccl_1,            ccl_1,            ccl_1
,            ccl_1,            ccl_1,            ccl_1,            ccl_1
,            ccl_1,            ccl_1,            ccl_1,            ccl_1
,            ccl_1,            ccl_1,            ccl_1,            ccl_1
,            ccl_1,            ccl_1,            ccl_1,            ccl_1
,            ccl_1,              -61 },
/* 55 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -55,                0,              -62
,                0,                0 },
/* 56 */ {      namelist2_0,      namelist2_0,      namelist2_0,      namelist2_0
,      namelist2_0,      namelist2_0,      namelist2_0,      namelist2_0
,      namelist2_0,      namelist2_0,      namelist2_0,      namelist2_0
,      namelist2_0,      namelist2_0,      namelist2_0,      namelist2_0
,      namelist2_0,      namelist2_0,      namelist2_0,      namelist2_0
,      namelist2_0,              -45,      namelist2_0,      namelist2_0
,      namelist2_0,      namelist2_0 },
/* 57 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,              -63,              -64
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0 },
/* 58 */ {      singleton_5,      singleton_5,      singleton_5,      singleton_5
,      singleton_5,      singleton_5,      singleton_5,      singleton_5
,      singleton_5,      singleton_5,      singleton_5,      singleton_5
,      singleton_5,      singleton_5,      singleton_5,      singleton_5
,      singleton_5,      singleton_5,      singleton_5,      singleton_5
,      singleton_5,      singleton_5,      singleton_5,      singleton_5
,      singleton_5,      singleton_5 },
/* 59 */ {      singleton_6,      singleton_6,      singleton_6,      singleton_6
,      singleton_6,      singleton_6,      singleton_6,      singleton_6
,      singleton_6,      singleton_6,      singleton_6,      singleton_6
,      singleton_6,      singleton_6,      singleton_6,      singleton_6
,      singleton_6,      singleton_6,      singleton_6,      singleton_6
,      singleton_6,      singleton_6,      singleton_6,      singleton_6
,      singleton_6,      singleton_6 },
/* 60 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,              -65,                0,                0
,                0,                0 },
/* 61 */ {        fullccl_1,        fullccl_1,        fullccl_1,        fullccl_1
,        fullccl_1,        fullccl_1,        fullccl_1,        fullccl_1
,        fullccl_1,        fullccl_1,        fullccl_1,        fullccl_1
,        fullccl_1,        fullccl_1,        fullccl_1,        fullccl_1
,        fullccl_1,        fullccl_1,        fullccl_1,        fullccl_1
,        fullccl_1,        fullccl_1,        fullccl_1,        fullccl_1
,        fullccl_1,        fullccl_1 },
/* 62 */ {                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0,                0,              -66
,                0,                0,                0,                0
,                0,                0,                0,                0
,                0,                0 },
/* 63 */ {      singleton_4,      singleton_4,      singleton_4,      singleton_4
,      singleton_4,      singleton_4,      singleton_4,      singleton_4
,      singleton_4,      singleton_4,      singleton_4,      singleton_4
,      singleton_4,      singleton_4,      singleton_4,      singleton_4
,      singleton_4,      singleton_4,      singleton_4,      singleton_4
,      singleton_4,      singleton_4,      singleton_4,      singleton_4
,      singleton_4,      singleton_4 },
/* 64 */ {            ccl_0,            ccl_0,            ccl_0,            ccl_0
,            ccl_0,            ccl_0,            ccl_0,            ccl_0
,            ccl_0,            ccl_0,            ccl_0,            ccl_0
,            ccl_0,            ccl_0,            ccl_0,            ccl_0
,            ccl_0,            ccl_0,            ccl_0,            ccl_0
,            ccl_0,            ccl_0,            ccl_0,            ccl_0
,            ccl_0,            ccl_0 },
/* 65 */ {      singleton_3,      singleton_3,      singleton_3,      singleton_3
,      singleton_3,      singleton_3,      singleton_3,      singleton_3
,      singleton_3,      singleton_3,      singleton_3,      singleton_3
,      singleton_3,      singleton_3,      singleton_3,      singleton_3
,      singleton_3,      singleton_3,      singleton_3,      singleton_3
,      singleton_3,      singleton_3,      singleton_3,      singleton_3
,      singleton_3,      singleton_3 }
};


LONG	jump_tab[66][14] = {
/*  0 */ {  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/*  1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/*  2 */ {  0,  0,  3,  4,  0,  6, 10,  9, 11, 12, 15,  0,  0,  0 },
/*  3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/*  4 */ {  0,  0,  0,  0,  0, 22, 10,  9, 11, 12, 15,  0,  0,  0 },
/*  5 */ {  0,  0,  0,  0,  0, 24, 10,  9, 11, 12, 15,  0,  0,  0 },
/*  6 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/*  7 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/*  8 */ {  0,  0,  0,  0, 25,  0,  0,  0,  0,  0,  0,  0,  0, 26 },
/*  9 */ {  0,  0,  0,  0,  0,  0, 27,  0, 11, 12, 15,  0,  0,  0 },
/* 10 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 11 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 15,  0,  0,  0 },
/* 12 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 13 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 36 },
/* 14 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 15 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 16 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 17 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 37,  0 },
/* 18 */ {  0,  0,  0,  0,  0,  0, 38,  0, 11, 12, 15,  0,  0,  0 },
/* 19 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 20 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 39,  0,  0 },
/* 21 */ {  0,  0,  0,  0,  0, 41, 10,  9, 11, 12, 15,  0,  0,  0 },
/* 22 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 23 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 24 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 25 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 26 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 27 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 28 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 29 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 30 */ {  0,  0,  0,  0,  0,  0,  0,  0, 46, 12, 15,  0,  0,  0 },
/* 31 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 32 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 33 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 34 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 35 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48 },
/* 36 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 37 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 38 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 39 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 40 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 55,  0,  0 },
/* 41 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 42 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 43 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 56 },
/* 44 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 45 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 46 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 15,  0,  0,  0 },
/* 47 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 48 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 49 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 50 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 51 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 52 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 53 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 54 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 55 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 56 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 57 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 58 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 59 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 60 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 61 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 62 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 63 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 64 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
/* 65 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

LONG	goal_follow[] = { ACCEPT };
LONG	init_sslex_follow[] = { BEG_NAME_LIST, BEG_LINE, BROPEN
, ANY_CHAR, AROPEN, PREVCCL, STRING_SYM, KLOPEN, CHARACTER, 
EOF_OP };
LONG	sslexrule_follow[] = { ACCEPT };
LONG	sc_follow[] = { BEG_LINE, BROPEN, ANY_CHAR, AROPEN, PREVCCL
, STRING_SYM, KLOPEN, CHARACTER, EOF_OP };
LONG	namelist2_follow[] = { END_NAME_LIST, COMMA };
LONG	rule_follow[] = { ACCEPT };
LONG	re_follow[] = { END_SYM, OR, LOOKAHEAD_SYM, KLCLOSE, ACCEPT
 };
LONG	re2_follow[] = { BROPEN, ANY_CHAR, AROPEN, PREVCCL, STRING_SYM
, KLOPEN, CHARACTER };
LONG	series_follow[] = { BROPEN, ANY_CHAR, AROPEN, PREVCCL, 
STRING_SYM, KLOPEN, CHARACTER, END_SYM, OR, LOOKAHEAD_SYM, KLCLOSE
, ACCEPT };
LONG	singleton_follow[] = { MORE, ONE_OR_MORE, ONE_OR_ZERO, 
BROPEN, ANY_CHAR, AROPEN, PREVCCL, STRING_SYM, KLOPEN, CHARACTER
, END_SYM, OR, LOOKAHEAD_SYM, KLCLOSE, ACCEPT };
LONG	fullccl_follow[] = { MORE, ONE_OR_MORE, ONE_OR_ZERO, BROPEN
, ANY_CHAR, AROPEN, PREVCCL, STRING_SYM, KLOPEN, CHARACTER, 
END_SYM, OR, LOOKAHEAD_SYM, KLCLOSE, ACCEPT };
LONG	ccl_follow[] = { ARCLOSE, CHARACTER };
LONG	string_follow[] = { STRING_SYM, CHARACTER };
LONG	predef_string_follow[] = { BRCLOSE, CHARACTER, END_NAME_LIST
, COMMA };

typedef struct
{
	/* FOLLOW-Menge gilt ab Alternative begin */
	ULONG	beginn;

	/* FOLLOW-Menge gilt bis Alternative end */
	ULONG	end;

	/* Anzahl der FOLLOW-Elemente */
	ULONG	anz_follow_elem;

	/* FOLLOW-Menge */
	LONG	*follow;

}FOLLOW;

FOLLOW	follow[] =
{
{ 1, 1, 1, goal_follow },
{ 2, 2, 10, init_sslex_follow },
{ 3, 8, 1, sslexrule_follow },
{ 9, 9, 9, sc_follow },
{ 10, 11, 2, namelist2_follow },
{ 12, 15, 1, rule_follow },
{ 16, 17, 5, re_follow },
{ 18, 18, 7, re2_follow },
{ 19, 20, 12, series_follow },
{ 21, 34, 15, singleton_follow },
{ 35, 36, 15, fullccl_follow },
{ 37, 39, 2, ccl_follow },
{ 40, 41, 2, string_follow },
{ 42, 43, 4, predef_string_follow },
};


/* Die Ziel-Tabelle */
LONG	ziel[66] = { INIT_SSLEX, 0, 0, GOAL, 0, 0, SSLEXRULE, SSLEXRULE, PREDEF_STRING
, 0, RULE, RE, SERIES, PREDEF_STRING, SINGLETON, SINGLETON, 
SINGLETON, STRING, 0, SINGLETON, CCL, 0, SSLEXRULE, SSLEXRULE
, SSLEXRULE, 0, NAMELIST2, RULE, RULE, RE2, 0, SERIES, SINGLETON
, SINGLETON, SINGLETON, PREDEF_STRING, 0, 0, 0, 0, CCL, SSLEXRULE
, SC, PREDEF_STRING, PREDEF_STRING, RULE, RE, 0, 0, SINGLETON
, SINGLETON, STRING, SINGLETON, FULLCCL, CCL, 0, NAMELIST2, 
0, SINGLETON, SINGLETON, 0, FULLCCL, 0, SINGLETON, CCL, SINGLETON
 };


/* Die rechte Seite aller Produktionen */
BYTE	*start_ri[2] = { "goal", "\'ACCEPT\'" };
BYTE	*goal_ri[2] = { "init_sslex", "sslexrule" };
BYTE	*init_sslex_ri[1] = { "\'EPSILON\'" };
BYTE	*sslexrule_0_ri[3] = { "sc", "\'BEG_LINE\'", "rule" };
BYTE	*sslexrule_1_ri[2] = { "sc", "rule" };
BYTE	*sslexrule_2_ri[2] = { "\'BEG_LINE\'", "rule" };
BYTE	*sslexrule_3_ri[1] = { "rule" };
BYTE	*sslexrule_4_ri[2] = { "sc", "\'EOF_OP\'" };
BYTE	*sslexrule_5_ri[1] = { "\'EOF_OP\'" };
BYTE	*sc_ri[3] = { "\'BEG_NAME_LIST\'", "namelist2", "\'END_NAME_LIST\'" };
BYTE	*namelist2_0_ri[3] = { "namelist2", "\'COMMA\'", "predef_string" };
BYTE	*namelist2_1_ri[1] = { "predef_string" };
BYTE	*rule_0_ri[2] = { "re2", "re" };
BYTE	*rule_1_ri[3] = { "re2", "re", "\'END_SYM\'" };
BYTE	*rule_2_ri[2] = { "re", "\'END_SYM\'" };
BYTE	*rule_3_ri[1] = { "re" };
BYTE	*re_0_ri[3] = { "re", "\'OR\'", "series" };
BYTE	*re_1_ri[1] = { "series" };
BYTE	*re2_ri[2] = { "re", "\'LOOKAHEAD_SYM\'" };
BYTE	*series_0_ri[2] = { "series", "singleton" };
BYTE	*series_1_ri[1] = { "singleton" };
BYTE	*singleton_0_ri[2] = { "singleton", "\'MORE\'" };
BYTE	*singleton_1_ri[2] = { "singleton", "\'ONE_OR_MORE\'" };
BYTE	*singleton_2_ri[2] = { "singleton", "\'ONE_OR_ZERO\'" };
BYTE	*singleton_3_ri[6] = { "singleton", "\'BROPEN\'", "\'NUMBER\'", "\'COMMA\'", "\'NUMBER\'", "\'BRCLOSE\'" };
BYTE	*singleton_4_ri[5] = { "singleton", "\'BROPEN\'", "\'NUMBER\'", "\'COMMA\'", "\'BRCLOSE\'" };
BYTE	*singleton_5_ri[4] = { "singleton", "\'BROPEN\'", "\'NUMBER\'", "\'BRCLOSE\'" };
BYTE	*singleton_6_ri[4] = { "singleton", "\'BROPEN\'", "predef_string", "\'BRCLOSE\'" };
BYTE	*singleton_7_ri[3] = { "\'BROPEN\'", "predef_string", "\'BRCLOSE\'" };
BYTE	*singleton_8_ri[1] = { "\'ANY_CHAR\'" };
BYTE	*singleton_9_ri[1] = { "fullccl" };
BYTE	*singleton_10_ri[1] = { "\'PREVCCL\'" };
BYTE	*singleton_11_ri[3] = { "\'STRING_SYM\'", "string", "\'STRING_SYM\'" };
BYTE	*singleton_12_ri[3] = { "\'KLOPEN\'", "re", "\'KLCLOSE\'" };
BYTE	*singleton_13_ri[1] = { "\'CHARACTER\'" };
BYTE	*fullccl_0_ri[3] = { "\'AROPEN\'", "ccl", "\'ARCLOSE\'" };
BYTE	*fullccl_1_ri[4] = { "\'AROPEN\'", "\'EXP\'", "ccl", "\'ARCLOSE\'" };
BYTE	*ccl_0_ri[4] = { "ccl", "\'CHARACTER\'", "\'TO\'", "\'CHARACTER\'" };
BYTE	*ccl_1_ri[2] = { "ccl", "\'CHARACTER\'" };
BYTE	*ccl_2_ri[1] = { "\'EPSILON\'" };
BYTE	*string_0_ri[2] = { "string", "\'CHARACTER\'" };
BYTE	*string_1_ri[1] = { "\'EPSILON\'" };
BYTE	*predef_string_0_ri[2] = { "predef_string", "\'CHARACTER\'" };
BYTE	*predef_string_1_ri[1] = { "\'EPSILON\'" };


typedef struct
{
	/* Name der Produktion */
	BYTE	*prod_name;

	/* Anzahl der Elemente auf der rechten Seite */
	WORD	anz_ri;

	/* Zeiger auf rechte Seite */
	BYTE	**ri;

}NTERM;

NTERM	nterm[] = {

{ "start", 2, start_ri }, /* goal 'ACCEPT' */
{ "goal", 2, goal_ri }, /* init_sslex sslexrule */
{ "init_sslex", 0, init_sslex_ri }, /* 'EPSILON' */
{ "sslexrule_0", 3, sslexrule_0_ri }, /* sc 'BEG_LINE' rule */
{ "sslexrule_1", 2, sslexrule_1_ri }, /* sc rule */
{ "sslexrule_2", 2, sslexrule_2_ri }, /* 'BEG_LINE' rule */
{ "sslexrule_3", 1, sslexrule_3_ri }, /* rule */
{ "sslexrule_4", 2, sslexrule_4_ri }, /* sc 'EOF_OP' */
{ "sslexrule_5", 1, sslexrule_5_ri }, /* 'EOF_OP' */
{ "sc", 3, sc_ri }, /* 'BEG_NAME_LIST' namelist2 'END_NAME_LIST' */
{ "namelist2_0", 3, namelist2_0_ri }, /* namelist2 'COMMA' predef_string */
{ "namelist2_1", 1, namelist2_1_ri }, /* predef_string */
{ "rule_0", 2, rule_0_ri }, /* re2 re */
{ "rule_1", 3, rule_1_ri }, /* re2 re 'END_SYM' */
{ "rule_2", 2, rule_2_ri }, /* re 'END_SYM' */
{ "rule_3", 1, rule_3_ri }, /* re */
{ "re_0", 3, re_0_ri }, /* re 'OR' series */
{ "re_1", 1, re_1_ri }, /* series */
{ "re2", 2, re2_ri }, /* re 'LOOKAHEAD_SYM' */
{ "series_0", 2, series_0_ri }, /* series singleton */
{ "series_1", 1, series_1_ri }, /* singleton */
{ "singleton_0", 2, singleton_0_ri }, /* singleton 'MORE' */
{ "singleton_1", 2, singleton_1_ri }, /* singleton 'ONE_OR_MORE' */
{ "singleton_2", 2, singleton_2_ri }, /* singleton 'ONE_OR_ZERO' */
{ "singleton_3", 6, singleton_3_ri }, /* singleton 'BROPEN' 'NUMBER' 'COMMA' 'NUMBER' 'BRCLOSE' */
{ "singleton_4", 5, singleton_4_ri }, /* singleton 'BROPEN' 'NUMBER' 'COMMA' 'BRCLOSE' */
{ "singleton_5", 4, singleton_5_ri }, /* singleton 'BROPEN' 'NUMBER' 'BRCLOSE' */
{ "singleton_6", 4, singleton_6_ri }, /* singleton 'BROPEN' predef_string 'BRCLOSE' */
{ "singleton_7", 3, singleton_7_ri }, /* 'BROPEN' predef_string 'BRCLOSE' */
{ "singleton_8", 1, singleton_8_ri }, /* 'ANY_CHAR' */
{ "singleton_9", 1, singleton_9_ri }, /* fullccl */
{ "singleton_10", 1, singleton_10_ri }, /* 'PREVCCL' */
{ "singleton_11", 3, singleton_11_ri }, /* 'STRING_SYM' string 'STRING_SYM' */
{ "singleton_12", 3, singleton_12_ri }, /* 'KLOPEN' re 'KLCLOSE' */
{ "singleton_13", 1, singleton_13_ri }, /* 'CHARACTER' */
{ "fullccl_0", 3, fullccl_0_ri }, /* 'AROPEN' ccl 'ARCLOSE' */
{ "fullccl_1", 4, fullccl_1_ri }, /* 'AROPEN' 'EXP' ccl 'ARCLOSE' */
{ "ccl_0", 4, ccl_0_ri }, /* ccl 'CHARACTER' 'TO' 'CHARACTER' */
{ "ccl_1", 2, ccl_1_ri }, /* ccl 'CHARACTER' */
{ "ccl_2", 0, ccl_2_ri }, /* 'EPSILON' */
{ "string_0", 2, string_0_ri }, /* string 'CHARACTER' */
{ "string_1", 0, string_1_ri }, /* 'EPSILON' */
{ "predef_string_0", 2, predef_string_0_ri }, /* predef_string 'CHARACTER' */
{ "predef_string_1", 0, predef_string_1_ri } /* 'EPSILON' */
};

/* Die Terminale */
BYTE	*term_name[ANZ_TERM] = { "EPSILON", "ACCEPT", "BEG_LINE", "EOF_OP", "BEG_NAME_LIST", 
"END_NAME_LIST", "COMMA", "END_SYM", "OR", "LOOKAHEAD_SYM", 
"MORE", "ONE_OR_MORE", "ONE_OR_ZERO", "BROPEN", "NUMBER", "BRCLOSE"
, "ANY_CHAR", "PREVCCL", "STRING_SYM", "KLOPEN", "KLCLOSE", 
"CHARACTER", "AROPEN", "ARCLOSE", "EXP", "TO" };


WORD error( VOID )
{
	/* Hier werden die Fehler behandlet. */
	REG WORD	i, j, p;
	LONG		shift_state;

	/* Fehlermeldung ausgeben ? */
	if( !error_flg )
	{
		/* Fehlerflag setzen */
		anz_error++;

		/* Fehlerlawine verhindern */
		error_flg++;

		/* Keine semmantischen Aktionen ausfuehren */
		sem_flg++;
	}

	do
	{
		/* Alle Terminale */
		for( i = 0; i < ANZ_TERM; i++ )
		{
			/* Shift von i mîglich ? */
			if( ( shift_state = lr_tab[state][i] ) < 0 )
			{
				/* Shift von lookahead mîglich ? */
				if( lr_tab[~shift_state][lookahead] < 0 )
				{
				/* Wert vom Scanner auf den Stack legen */
				lr_sem_stack[++lr_sem_count] = scan_val;

					/* i schieben */
					shift( lr_tab[state][i], old_lookahead, i );

					/* Geschobenes Terminal merken */
					old_lookahead = i;

					/* Alles OK */
					return( 0 );
				}

				/* Reduziere-Zustand ? */
				else if( lr_tab[~shift_state][lookahead] > 0 )
				{
					/* Reduziere-Zustand */

					/* Ist lookahead in FOLLOW( lr_tab[~shift_state][i] ) ? */
					j = 0;

					/* Die passende FOLLOW-Struktur holen */
					do
					{
						/* Ist es die richtige ? */
						if( ( follow[j].beginn <= ( lr_tab[~shift_state][i] - 1 ) ) && ( follow[j].end >= ( lr_tab[~shift_state][i] - 1 ) ) )
							/* Ja */
							break;

						else
							/* Nein */
							j++;

					}while( 1 );

					/* Alle FOLLOW-Elemente */
					for( p = 0; p < follow[j].anz_follow_elem; p++ )
					{
						/* Ist es lookahead ? */
						if( follow[j].follow[p] == lookahead )
						{
							/* Wert vom Scanner auf den Stack legen */
							lr_sem_stack[++lr_sem_count] = scan_val;

							/* i schieben */
							shift( lr_tab[state][i], old_lookahead, i );

							/* Geschobenes Terminal merken */
							old_lookahead = i;

							/* Alles OK */
							return( 0 );
						}
					}
				}
			}
		}

		/* Konnte ein Terminal eingeschoben werden ? */
		if( i == ANZ_TERM )
		{
			/* Ist es ACCEPT ? */
			if( lookahead == ACCEPT )
				/* Ja, Parsing abbrechen */
				return( -1 );

			/* Nein, Meldung ausgeben */
			fprintf( output_file, "\n\nLîsche Terminal %s !", term_name[lookahead] );

			/* Lookahead lîschen */
			lookahead = scanner();
		}

	}while( 1 );
}





WORD parse( VOID )
{
	WORD	state;

	/* Ausgabemedium festlegen */
	switch( dest_state )
	{
		case DEST_PRN:
		{
			/* Ausgabe auf Drucker */
			output_file = stdprn;
			break;
		}

		case DEST_SCREEN:
		{
			/* Ausgabe auf Bildschirm */
			output_file = stdout;
			break;
		}

		case DEST_FILE:
		{
			/* Ausgabe in Datei */

			/* Datei zum Schreiben îffnen */
			output_file = freopen( "PARSER.LOG", "w", stdout );
		}
	}

	/* Stack initialisieren */
	push( 0 );

	/* Eingabesymbol holen */
	lookahead = scanner();

	/* Parsen */
	state = do_parse();

	/* Ausgabe auf Datei ? */
	if( dest_state == DEST_FILE )
		/* Datei schliessen */
		fclose( output_file );

	/* Alles OK */
	return( state );
}


VOID push( LONG value )
{
	/* Stackpointer erhîhen */
	stackpointer++;

	/* Ueberlauf ? */
	if( stackpointer > STACK_SIZE )
	{
		fprintf( output_file, "\n\nStackoverflow !" );
		exit( 1 );
	}

	/* Wert auf Stack schieben */
	stack[stackpointer] = value;
}


LONG pop( VOID )
{
	/* Stackpointer dekrementieren */
	stackpointer--;

	/* "Unterlauf" ? */
	if( stackpointer < -1 )
	{
		fprintf( output_file, "\n\nStackunderflow !" );
		exit( 1 );
	}

	/* Wert zurueckliefern */
	return( stack[stackpointer + 1] );
}


WORD do_parse( VOID )
{
	LONG taste, symbol, i;

	/* Endlosschleife */
	while( 1 )
	{
		/* Aktuellen Zustand holen */
		state = stack[stackpointer];

		/* Ist die Eingabe geparst ? */
		if( state == 1 )
		{
			/* Ist das aktuelle Terminal nicht ACCEPT ? */
			if( lookahead != ACCEPT )
			{
				/* Nein, Meldung ausgeben */
				fprintf( output_file, "\n\nLetztes Terminal ist nicht ACCEPT !" );
				anz_error++;
			}

			/* Gab es Fehler ? */
			if( anz_error )
				/* Ja */
				fprintf( output_file, "\n\nEs gab %d Fehler !", anz_error );

			else
				/* Alles OK */
				fprintf( output_file, "\n\nEingabe war OK\n" );

			/* Zurueck zum Aufrufer */
			return( anz_error );
		}

		/* Schieben ? */
		else if( lr_tab[state][lookahead] < 0 )
		{
				/* Wert vom Scanner auf den Stack legen */
				lr_sem_stack[++lr_sem_count] = scan_val;

			/* Schieben */
			shift( lr_tab[state][lookahead], old_lookahead, lookahead );

			/* Altes lookahead merken */
			old_lookahead = lookahead;

			/* Neues Eingabesymbol holen */
			lookahead = scanner();
		}

		/* Reduzieren */
		else if( lr_tab[state][lookahead] > 0 )
		{
			lr_val = do_sem_act( lr_tab[state][lookahead], lr_sem_count, &lr_val, lr_sem_stack );

				/* Stack korregieren */
				lr_sem_count -= nterm[lr_tab[state][lookahead] - 1].anz_ri;
				lr_sem_stack[++lr_sem_count] = lr_val;

			/* Zustand nach Reduktion holen */
			symbol = ziel[state];

			/* Reduzieren */
			reduce( lr_tab[state][lookahead] );

			/* Alten Zustand merken */
			old_state = state;

			/* Neuen Zustand holen */
			state = stack[stackpointer];

			/* Meldung ausgeben */
			fprintf( output_file, "\n\nReduziere mit : " );

			/* Die Produktion ausgeben */
			output_prod( lr_tab[old_state][lookahead] - 1 );

			fprintf( output_file, "\nNeuer Status  : %ld", jump_tab[state][symbol - 1] );
			fprintf( output_file, "\nAlter Status  : %ld", old_state );
			fprintf( output_file, "\nNeues Terminal: %s", term_name[lookahead] );

			/* Haben wir ein altes Terminal ? */
			if( old_lookahead >= 0 )
				/* Wird haben ein altes Terminal */
				fprintf( output_file, "\nAltes Terminal: %s", term_name[old_lookahead] );

			/* Zustand nach Reduktion herstellen */
			if( jump_tab[state][symbol - 1] != 0 )
				/* Wert auf Stack schieben */
				push( jump_tab[state][symbol - 1] );

			else
			{
				/* Fehler beim Springen */
				fprintf( output_file, "\n\nFehler beim Springen !"\
						"\nAktueller Status  : %ld"\
						"\nAktuelles Terminal: %s", old_state, term_name[lookahead] );

				/* Ende */
				exit( 1 );
			}
		}

		/* Syntaxfehler ? */
		else if( lr_tab[state][lookahead] == 0 )
		{
			/* Fehler */
			if( error() < 0 )
				/* Parsing beenden */
				return( -1 );
		}

		/* Ausgabe auf Bildschirm ? */
		if( dest_state == DEST_SCREEN )
		{
			/* Auf Tastendruck warten */
			taste = Cnecin();

			/* Stack ausgeben ? */
			if( ( taste & 0x000000ff ) == 0x73 )
			{
				/* Stack ausgeben */
				fprintf( output_file, "\n\nStack\n-----\n" );

				/* Alle EintrÑge im Stack ausgeben */
				for( i = 0; i < stackpointer; i++ )
					/* Eintrag ausgeben */
					fprintf( output_file, "%ld\n", stack[i] );
			}
		}

		else
		{
			/* Auf dem Drucker/Datei immer den Stack ausgeben */

			/* Stack ausgeben */
			fprintf( output_file, "\n\nStack\n-----\n" );

			/* Alle EintrÑge im Stack ausgeben */
			for( i = 0; i < stackpointer; i++ )
				/* Eintrag ausgeben */
				fprintf( output_file, "%ld\n", stack[i] );
		}
	}
}

VOID shift( LONG shift_state, LONG old_lahead, LONG lahead )
{
	/* Hier wird ein Eintrag aus der Shift-Tabelle auf den Stack geschoben. */

	/* Eintrag auf Stack schieben */
	push( ~shift_state );

	/* Meldung ausgeben */
	fprintf( output_file, "\n\nSchiebe       : %s", term_name[lahead] );
	fprintf( output_file, "\nNeuer Status  : %ld", ~shift_state );
	fprintf( output_file, "\nAlter Status  : %ld", state );
	fprintf( output_file, "\nNeues Terminal: %s", term_name[lahead] );

	/* Haben wir ein altes Terminal ? */
	if( old_lahead >= 0 )
		/* Wird haben ein altes Terminal */
		fprintf( output_file, "\nAltes Terminal: %s", term_name[old_lahead] );
}

VOID reduce( LONG nterm_index )
{
	/* Hier wird reduziert. */
	REG WORD i;

	/* Elemente vom Stack lîschen */
	for( i = 0; i < nterm[nterm_index - 1].anz_ri; i++ )
		/* Eintrag vom Stack lîschen */
		pop();
}

VOID output_prod( LONG prod_index )
{
	/* Hier wird eine Produktion ausgegeben. */
	REG ULONG	i;
	BYTE		**ri;

	/* Zeiger auf rechte Seite holen */
	ri = nterm[prod_index].ri;

	/* Die Produktion ausgeben */
	fprintf( output_file, "%s -> ", nterm[prod_index].prod_name );

	/* Ist es eine EPSILON-Produktion ? */
	if( !nterm[prod_index].anz_ri )
		/* Ja, nur EPSILON ausgeben */
		fprintf( output_file, "EPSILON" );

	else
	{
		/* Rechte Seite ausgeben */
		for( i = 0; i < nterm[prod_index].anz_ri; i++ )
			/* String ausgeben */
			fprintf( output_file, "%s ", ri[i] );
	}
}