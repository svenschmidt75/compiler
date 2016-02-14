	/* Includedatei des SLR-Parsers */


/* Includes */
#include <portab.h>


/* Deklarationen */


/* Copyright-Meldung */
#define	INFO						"\nSLR-Parser-Generator V0.00û"\
									"\nAb 12.01.1995"\
									"\n(C) 1995 by Sven Schmidt"


/* Fehlermeldungen */
#define	FILE_NOT_FOUND				0
#define	CANNOT_OPEN_FILE			1
#define	NOT_ENOUGH_FILE_MEMORY		2
#define	CANNOT_READ_FILE			3
#define	CANNOT_CLOSE_FILE			4
#define	INVALID_CHAR				5
#define	NOT_ENOUGH_GR_HASH_MEMORY	6
#define	FALSE_PROD_SEPERATOR		7
#define REDEFINED_NONTERM			8
#define	NOT_ENOUGH_MEMORY			9
#define NOT_DEFINED_NONTERM			10
#define DOUBLE_ALT					11
#define EPSILON_ERR					12
#define NO_START_PROD				13
#define ASSOC_ERR					14
#define	CANNOT_OPEN_INF_FILE		15
#define	CANNOT_READ_INF_FILE		16
#define	CANNOT_CLOSE_INF_FILE		17
#define	CANNOT_OPEN_INCLUDE_FILE	18
#define	CANNOT_CLOSE_INCLUDE_FILE	19
#define	UNKNOWN_TERM				20
#define	INF_FILE_ERR				21
#define	INF_ERR						22
#define NO_GRFILE_NAME				23


/* Default-Grîûe diverser Strukturen */
#define DEFAULT_PROD_INFO_LENGTH	80
#define DEFAULT_TERM_LIST_LENGTH	80
#define DEFAULT_HULLE_LENGTH		80


/* Dient zum Initialisieren von zweidim. Arrays */
#define	PTR_ULONG					0
#define	PTR_HULLE					1
#define	PTR_SLR_TAB					2


/* FÅr OUTPUT.C */
#define _FIRST						0
#define _FOLLOW						1


/* FÅr insert_hash_word */
#define	TERM						0	/* Terminal eintragen */
#define	NON_TERM					1	/* Nichtterminal eintragen */


/* PrioritÑt und AssoziativitÑt */
#define	LEFT						1	/* Linksassoziativ */
#define	RIGHT						2	/* Rechtsassoziativ */


/* FÅr die SLR-Tabelle */
#define ERROR						0
#define	SHIFT						1
#define REDUCE						2


/* FÅr die Info-Datei */
#define ON							0
#define OFF							1
#define DEST_SCREEN					0
#define DEST_FILE					1
#define DEST_PRN					2


/* Faktor, mit dem die Nummer eines Items multipliziert wird */
#define ITEM_LIST_FAC				1024



/* Definitionen */

typedef struct
{
	/*	0: Error, 1: Shift; 2: Reduce */
	WORD	state;

	/*
		Zu schiebendes Terminal, bei Reduktion Anzahl der vom Stack zu
		lîschenden Terminale.
	*/
	ULONG	value;

}SLR_TAB;


typedef struct
{
	/* Anzahl der max. Elemente */
	ULONG	max_anz_elem;

	/* Anzahl der aktuellen Elemente */
	ULONG	anz_elem;

	/* Array der Terminale */
	ULONG	*arr;

}FIRST;


typedef struct
{
	/* Nummer der Produktion ( von der HASH-Funktion ermittelt ) */
	LONG		prod_nr;

	/* Index der Produktion ( fÅr PROD_INFO ) */
	ULONG		prod_list_nr;

	/* Nummer des Items */
	ULONG		item_list_nr;

	/* Position des Punktes im Items */
	WORD		pkt_pos;

}PROD_ITEM;


typedef struct
{
	/* Anzahl der max. Items */
	ULONG		anz_max_items;

	/* Anzahl der Kerne im Item */
	ULONG		anz_kitems;

	/* Anzahl der Items */
	ULONG		anz_items;

	/* Array, daû die Items nÑher beschreibt. */
	PROD_ITEM	**items_array;

	/* Nichtterminal/Terminal -> GOTO( ..., Nterm/Term ) */
	ULONG		nterm;

	/* Index der HÅlle -> GOTO( Ix, ... ) */
	ULONG		index;

}HULLE;


typedef struct
{
	/* Anzahl der max. HÅllen-Strukturen */
	ULONG		anz_max_hulle;

	/* Array, daû die Items nÑher beschreibt. */
	HULLE		**hulle;

}OBER_HULLE;



/*
	Beinhaltet die rechte Seite einer Produktion.
	Bietet fÅr max. 10 Terminale bzw. Nichtterminale Platz.
*/
typedef struct
{
	/* Nummer der Produktion ( von der HASH-Funktion ermittelt ) */
	LONG		prod_nr;

	/* PrioritÑt der Produktion */
	ULONG		priori;

	/* AssoziativitÑt der Produktion */
	ULONG		assoc;

	/*
		Rechte Seite der Produktion.
		( Nummer des Nichtterminales bzw. Terminals )
	*/
	ULONG		prod_ri[10];

	/* Anzahl der Nichtterminale und Terminale auf der rechten Seite */
	WORD		prod_ri_anz;

	/*
		Anzahl der Alternativen der Produktion.
		z.B. factor : '(' expr ')'
					| '0'..'9'
		Produktionen, die den gleichen Namen haben, liegen sequentiell in
		dem Array von dieser Struktur !
	*/
	WORD		prod_fl;

}PROD_INFO;


/*
	Die Nummer der Nichtterminale wird Åber eine HASH-Funktion erzeugt.
	Es werden nur die Nichtterminale in die HASH-Tab aufgenommen, die
	auf der linken Seite einer Produktion stehen.
*/
typedef struct
{
	/* Index der Produktion ( fÅr PROD_INFO ) */
	ULONG		prod_list_nr;

	/* 0: Nichtterminal / 1: Terminal */
	WORD		prod_type;

	/* PrioritÑt des Terminals */
	ULONG		priori;

	/*
		AssoziativitÑt des Terminals
		LEFT : %left
		RIGHT: %right
	*/
	ULONG		assoc;

	/*
		Terminal     : 1: Terminal wird auf der rechten Seite einer
					      Produktion benutzt.

		Nichtterminal:    Beugt Endlosschleifen bei der Generierung
					      der FOLLOW-Mengen vor.
	*/
	WORD		defined;

	/* Beinhaltet den String des Nichtterminales/Terminals */
	BYTE		*prod_name;

	/* Fehlernummer fÅr automatische Fehlerbehandlung */
	ULONG		err_nr;

	/* Nummer der Produktion ohne Alternativen */
	ULONG		index;

	/* FÅr die FOLLOW-Mengen */
	FIRST		follow;

}PROD_HASH;


/* Struktur fÅr den binÑren Baum */

typedef struct bin_tree
{
	/* Nummer des Nichtterminals( von der HASH-Funktion ) */
	LONG			prod_nr;

	/* Zeiger auf linken Ast des Baums */
	struct bin_tree	*left;

	/* Zeiger auf linken Ast des Baums */
	struct bin_tree	*right;

}BIN_TREE;


/* Struktur fÅr die Info-Datei */

typedef struct
{
	/* Anzeigen auf Bildschirm bzw. Datei */
	BYTE	dest;

	/* Anzeigen der FOLLOW-Mengen */
	BYTE	follow;

	/* Anzeigen der FIRST-Mengen */
	BYTE	first;

	/* Anzeigen der LR(0)-Items */
	BYTE	slr_items;

	/* Analyse-Prozess-Meldungen ausgeben */
	BYTE	analyze;

	/* Ausgabe der Tabelleninformationen */
	BYTE	slr_tab_1;

	/* Ausgabe der SLR-Tabelle */
	BYTE	slr_tab_2;

	/* DEBUG-Version des SLR-Parser erzeugen */
	BYTE	debug;

	/* Speicher fÅr Grammatik-Hash-Tabelle */
	LONG	gr_hash_tab_size;

	/* Speicher fÅr Fehlermeldungen-Hash-Tabelle */
	LONG	err_msg_hash_tab_size;

	/* Max. Anzahl der Zeichen pro Zeile */
	WORD	max_char;

}SLR_INFO;



/* Prototypen */
WORD analyze_grammar( VOID );
WORD gen_items( VOID );
WORD error( WORD err_no, WORD err );
WORD add_prod( HULLE *hulle );
WORD add_item( HULLE *_hulle, LONG prod_nr, ULONG prod_list_nr, WORD pkt_pos );
WORD cmp_hulle( HULLE *h_1, HULLE *h_2 );
WORD make_tab( VOID );
WORD init_twodim( VOID ***pointer, WORD flg, ULONG x_anz, ULONG y_anz );
ULONG _goto( HULLE *_hulle, LONG n_term, ULONG h_count, WORD state );
WORD gen_first( FIRST *fst, LONG nterm, WORD *state );
WORD get_all_term_nr( BIN_TREE *blat );
WORD gen_output( BYTE *file_name );
WORD insert_first( LONG nterm, FIRST *fst );