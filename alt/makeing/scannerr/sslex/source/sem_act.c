/*
	Projekt		:	Scanner-Generator
	Modul		:	SEM_ACT.C
	Sprache		:	ANSI-C
	Compiler	:	PureC V1.1

	Ab			:	25.07.1997
	Bis			:	xx.xx.xxxx
	von Sven Schmidt
*/





/************/
/* Includes */
/************/

/* Include-Datei des Scanner-Generators */
#include "sslex.h"
#include "sslexpar.h"



/**************/
/* Prototypen */
/**************/

LR_TYPE do_sem_act( LONG prod, WORD lr_sem_count, LR_TYPE *lr_val, LR_TYPE *lr_sem_stack );


/*****************/
/* Deklarationen */
/*****************/



/****************/
/* Definitionen */
/****************/

/* Zeiger auf Array mit den NEA_STR */
NEA_STR		**nea_str_arr;

/* Anzahl der NEA_STR in nea_str_arr */
LONG		nea_str_arr_count = 0;


/* Zeichenklasse */
CCL_STR		ccl_str;


/* Speicher fÅr einen String */
BYTE		st[10][256];

/* ZÑhler */
WORD		st1_count = 0, st2_count;


/* Flag fÅr die RÅckgabe LINEFEED */
WORD		analyze = 0;


/* Der Index eines NEAs */
LONG		nea_index;



/***********/
/* Imports */
/***********/

MGLOBAL NEA_STR	*nea_str;
MGLOBAL CCL_STR	all_bchar;
MGLOBAL LONG	zeile;
MGLOBAL BYTE	*temp_grammar;
MGLOBAL BYTE	*grammar;



LR_TYPE do_sem_act( LONG prod, WORD lr_sem_count, LR_TYPE *lr_val, LR_TYPE *lr_sem_stack )
{
	/* Hier werden die semmantischen Aktionen ausgefuehrt. */
	MLOCAL LONG	sc_arr[10], sc_arr_count = 0;
	MLOCAL LONG		nea_ind;
	LONG			i;
	LONG			sc_ind;

	switch( ( WORD ) prod )
	{
		case goal:
		{
			/* init_sslex sslexrule */
			nea_index = lr_sem_stack[lr_sem_count].nea_ind;

			/* Den RE extrahieren */
			if( insert_re( lr_val->nea_ind, temp_grammar, grammar ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			output_nea( &nea_str[lr_val->nea_ind] );
			break;
		}

		case init_sslex:
		{
			/* 'EPSILON' */

			/* ZÑhler fÅr das SC-Array */
			sc_arr_count = 0;
			st1_count = 0;

			break;
		}
/*
		case do_sect1_:
		{
			/*  sect1_string 'NAME_SEP' series */

			/* Den erzeugten NEA in die PREDEF_NEA-Struktur eintragen */
			if( ( insert_predef_nea( st[lr_sem_stack[lr_sem_count- 2].w], lr_sem_stack[lr_sem_count].nea_ind ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			/* ZÑhler zurÅcksetzen */
			st1_count--;

			break;
		}


		case sect2_0:
		{
			/* sect2_0 -> sect2 initforrule flexrule 'LINEFEED' */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			/* Flag zurÅcksetzen */
			analyze = 0;
			break;
		}
/*
		case initforrule:
		{
			/* initforrule -> 'EPSILON' */
			lr_val->nea_ind = new_nea();
			break;
		}
*/
*/
		case sslexrule_0 :
		{
			/* sc 'BEG_LINE' rule */

			/* Den NEA in die SCs eintragen, die im sc_arr sind */
			for( i = 0; i < sc_arr_count; i++ )
				/* NEA in SC eintragen */
				insert_nea_sc( lr_sem_stack[lr_sem_count].nea_ind, sc_arr[i], 0, 0 );

			/* NEA darf nur am Zeilenanfang stehen */
			nea_str[lr_sem_stack[lr_sem_count].nea_ind].beg_line++;

			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;
			break;
		}

		case sslexrule_1 :
		{
			/* sc rule */

			/* Den NEA in die SCs eintragen, die im sc_arr sind */
			for( i = 0; i < sc_arr_count; i++ )
				/* NEA in SC eintragen */
				insert_nea_sc( lr_sem_stack[lr_sem_count].nea_ind, sc_arr[i], 0, 0 );

			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;
			break;
		}

		case sslexrule_2 :
		{
			/* 'BEG_LINE' rule */

			/* Den NEA in alle inclusive-SCs eintragen */
			insert_nea_sc( lr_sem_stack[lr_sem_count].nea_ind, 0L, INCLUSIVE, 1 );

			/* NEA darf nur am Zeilenanfang stehen */
			nea_str[lr_sem_stack[lr_sem_count].nea_ind].beg_line++;

			/* Vererbung */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;
			break;
		}

		case sslexrule_3 :
		{
			/* rule */

			/* Den NEA in alle inclusive-SCs eintragen */
			insert_nea_sc( lr_sem_stack[lr_sem_count].nea_ind, 0L, INCLUSIVE, 1 );

			/* Vererbung */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;
			break;
		}

		case sslexrule_4 :
		{
			/* sc 'EOF_OP' */
			break;
		}

		case sslexrule_5 :
		{
			/* 'EOF_OP' */
			break;
		}

		case sc:
		{
			/* sc	->	'BEG_NAME_LIST' namelist2 'END_NAME_LIST' */
			break;
		}

		case namelist2_0 :
		{
			/* namelist2 'COMMA' predef_string */

			/* Die SC in die SS_STR eintragen */
			if( ( sc_ind = get_sc_ind( st[lr_sem_stack[lr_sem_count].w] ) ) < 0 )
				/* Undefinierte SC */
				fprintf( stderr, "\nZeile %lu: Startcondition %s nicht definiert !", zeile, st[lr_sem_stack[lr_sem_count].w] );

			else
				/* Die SC ins SC-Array eintragen */
				sc_arr[sc_arr_count++] = sc_ind;

			break;
		}

		case namelist2_1 :
		{
			/* predef_string */

			/* Die SC in die SS_STR eintragen */
			if( ( sc_ind = get_sc_ind( st[lr_sem_stack[lr_sem_count].w] ) ) < 0 )
				/* Undefinierte SC */
				fprintf( stderr, "\nZeile %lu: Startcondition %s nicht definiert !", zeile, st[lr_sem_stack[lr_sem_count].w] );

			else
				/* Die SC ins SC-Array eintragen */
				sc_arr[sc_arr_count++] = sc_ind;

			break;
		}

		case rule_0 :
		{
			/* rule_0	->	re2 re */

			/* Die beiden NEAen verknuepfen */
			if( ( lr_val->nea_ind = link_nea( lr_sem_stack[lr_sem_count - 1].nea_ind, lr_sem_stack[lr_sem_count].nea_ind ) ) < 0 )
				/* Fehler */
				lr_val->l = -1L;

			break;
		}

		case rule_1 :
		{
			/* rule_1	->	re2 re 'END_SYM' */

			/* Die beiden NEAen verknuepfen */
			if( ( lr_val->nea_ind = link_nea( lr_sem_stack[lr_sem_count - 1].nea_ind, lr_sem_stack[lr_sem_count].nea_ind ) ) < 0 )
				/* Fehler */
				lr_val->l = -1L;

			/* Zeichenklasse initialisieren */
			memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );

			/* Zeichen in die Zeichenklasse eintragen */
			insert_ccl( &ccl_str, '\n', '\n' );

			/* An den NEA ein ZustandsÅbergang zu '\n' errichten */
			if( nea_bra( lr_val->nea_ind, FINIT_STATE, NEW_FINIT_STATE, ccl_str.ccl, 0, 0, _EPSILON ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			break;
		}

		case rule_2 :
		{
			/* rule_2	->	re 'END_SYM' */

			/* Zeichenklasse initialisieren */
			memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );

			/* Zeichen in die Zeichenklasse eintragen */
			insert_ccl( &ccl_str, '\n', '\n' );

			/* An den NEA ein ZustandsÅbergang zu '\n' errichten */
			if( nea_bra( lr_sem_stack[lr_sem_count - 1].nea_ind, FINIT_STATE, NEW_FINIT_STATE, ccl_str.ccl, 0, 0, _EPSILON ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			break;
		}

		case rule_3 :
		{
			/* rule_3	->	re */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;
			break;
		}

		case re2 :
		{
			/* re2	->	re 'LOOKAHEAD_SYM' */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			break;
		}

		case re_1 :
		{
			/* re_1	->	series */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;
			break;
		}

		case re_0 :
		{
			/* re_0	->	re 'OR' series */

			/* Zwei NEAen verodern */
			if( ( lr_val->nea_ind = or_nea( lr_sem_stack[lr_sem_count - 2].nea_ind, lr_sem_stack[lr_sem_count].nea_ind ) ) < 0 )
				/* Fehler */
				lr_val->l = -1L;

			break;
		}

		case series_0 :
		{
			/* series_0	->	series singleton */

			/* Die beiden NEAen zu einem verbinden */
			if( ( lr_val->nea_ind = link_nea( lr_sem_stack[lr_sem_count - 1].nea_ind, lr_sem_stack[lr_sem_count].nea_ind ) ) < 0 )
				/* Fehler */
				lr_val->l = -1L;

			break;
		}

		case series_1 :
		{
			/* series_1	->	singleton */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;

			break;
		}

		case singleton_0:
		{
			/* singleton_0	->	singleton 'MORE' */

			/* NEA ist optional */
			if( make_nea_optional( lr_sem_stack[lr_sem_count - 1].nea_ind ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			break;
		}

		case singleton_1:
		{
			/* singleton_1	->	singleton 'ONE_OR_MORE' */

			/* NEA muss mind. einmal durchlaufen werden */
			if( make_nea_once( lr_sem_stack[lr_sem_count - 1].nea_ind ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			break;
		}

		case singleton_2:
		{
			/* singleton_2	->	singleton 'ONE_OR_ZERO' */

			/* NEA muss einmal oder nullmal durchlaufen werden. */
			if( make_nea_once_or_never( lr_sem_stack[lr_sem_count - 1].nea_ind ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			break;
		}

		case singleton_3:
		{
			/* singleton_3	->	singleton 'BROPEN' 'NUMBER' 'COMMA' 'NUMBER' 'BRCLOSE' */
			LONG	first = lr_sem_stack[lr_sem_count - 3].l, secound = lr_sem_stack[lr_sem_count - 1].l;

			/* Fehler ? */
			if( ( first < 0 ) || ( secound < 0 ) || ( first > secound ) || ( secound < first ) )
				/* Fehler */
				fprintf( stderr, "\nZeile %lu: Zahlen sind unsinnig !" );

			/* NEA kopieren */
			if( ( lr_val->nea_ind = rep_nea( lr_sem_stack[lr_sem_count - 5].nea_ind, lr_sem_stack[lr_sem_count - 3].l, lr_sem_stack[lr_sem_count - 1].l ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			break;
		}

		case singleton_4:
		{
			/* singleton_4	->	singleton 'BROPEN' 'NUMBER' 'COMMA' 'BRCLOSE' */

			/* Fehler ? */
			if( lr_sem_stack[lr_sem_count - 1].l < 0 )
				/* Fehler */
				fprintf( stderr, "\nZeile %lu: Wiederholungs-Wert muû >= 0 sein !" );

			/* NEA kopieren */
			if( ( lr_val->nea_ind = rep_nea( lr_sem_stack[lr_sem_count - 4].nea_ind, lr_sem_stack[lr_sem_count - 1].l, -1 ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			break;
		}

		case singleton_5:
		{
			/* singleton_5	->	singleton 'BROPEN' 'NUMBER' 'BRCLOSE' */

			/* Fehler ? */
			if( lr_sem_stack[lr_sem_count - 1].l < 0 )
				/* Fehler */
				fprintf( stderr, "\nZeile %lu: Wiederholungs-Wert muû >= 0 sein !" );

			/* NEA kopieren */
			if( ( lr_val->nea_ind = rep_nea( lr_sem_stack[lr_sem_count - 3].nea_ind, lr_sem_stack[lr_sem_count - 1].l, 0 ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			break;
		}

		case singleton_6:
		{
			/*  singleton 'BROPEN' sect1_string 'BRCLOSE' */

			/* Den NEA suchen, und dessen nea_ind zurÅckliefern */
			if( ( nea_ind = get_predef_nea( st[lr_sem_stack[lr_sem_count - 1].w] ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
			{
				/* Den NEA kopieren */
				 if( ( nea_ind = copy_nea( nea_ind ) ) < 0 )
					/* Fehler */
					lr_val->l = -1;

				/* Die beiden NEAen zu einem verbinden */
				if( ( lr_val->nea_ind = link_nea( lr_sem_stack[lr_sem_count - 3].nea_ind, nea_ind ) ) < 0 )
					/* Fehler */
					lr_val->l = -1L;
			}

			/* Analysiere RE */
			analyze++;
			break;
		}

		case singleton_7:
		{
			/*  'BROPEN' string 'BRCLOSE' */

			/* Den NEA suchen, und dessen nea_ind zurÅckliefern */
			if( ( nea_ind = get_predef_nea( st[lr_sem_stack[lr_sem_count - 1].w] ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
			{
				/* Den NEA kopieren */
				 if( ( lr_val->nea_ind = copy_nea( nea_ind ) ) < 0 )
					/* Fehler */
					lr_val->l = -1;
			}

			/* Analysiere RE */
			analyze++;
			break;
		}

		case singleton_8:
		{
			/* 'ANY_CHAR' */

			/* Einen neuen NEA erzeugen */
			nea_ind = init_nea();

			/* Ein ZustandsÅbergang erzeugen */
			if( ( nea_bra( nea_ind, FINIT_STATE, NEW_FINIT_STATE, all_bchar.ccl, 0, 1, 0 ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = nea_ind;

			/* Analysiere RE */
			analyze++;
			break;
		}

		case singleton_9:
		{
			/* fullccl */

			/* Vererbung */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count].nea_ind;

			/* Analysiere RE */
			analyze++;
			break;
		}

		case singleton_10:
		{
			/* 'PREVCCL' */

			/* Analysiere RE */
			analyze++;
			break;
		}

		case singleton_11:
		{
            /* 'BEG_STRING' string 'END_STRING' */ 
 
            /* Vererbung */ 
            lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind; 

			/* Analysiere RE */
			analyze++;
            break; 
		}

		case singleton_12:
		{
			/* 'KLOPEN' re 'KLCLOSE' */
			lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			/* Analysiere RE */
			analyze++;
			break;
		}

		case singleton_13:
		{
			/* 'CHARACTER' */

			/* Einen neuen NEA erzeugen */
			nea_ind = init_nea();

			/* Zeichenklasse initialisieren */
			memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );

			/* Zeichen in die Zeichenklasse eintragen */
			insert_ccl( &ccl_str, lr_sem_stack[lr_sem_count].b, lr_sem_stack[lr_sem_count].b );

			/* CCL ÅberprÅfen */
			check_ccl( &ccl_str );

			/* Ein ZustandsÅbergang erzeugen */
			if( ( nea_bra( nea_ind, FINIT_STATE, NEW_FINIT_STATE, ccl_str.ccl, 0, 0, 0 ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = nea_ind;

			/* Analysiere RE */
			analyze++;
			break;
		}

		case fullccl_0 :
		{
			/* fullccl_0	->	'AROPEN' ccl 'ARCLOSE' */

			/* Einen neuen NEA erzeugen */
			nea_ind = init_nea();

			/* CCL ÅberprÅfen */
			check_ccl( &ccl_str );

			/* Ein ZustandsÅbergang erzeugen */
			if( ( nea_bra( nea_ind, FINIT_STATE, NEW_FINIT_STATE, lr_sem_stack[lr_sem_count - 1].ccl_str->ccl, 0, 0, 0 ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = nea_ind;

			break;
		}

		case fullccl_1 :
		{
			/* fullccl_1	->	'AROPEN' 'EXP' ccl 'ARCLOSE' */

			/* Zeichenklasse negieren */
			negate_ccl( lr_sem_stack[lr_sem_count - 1].ccl_str );

			/* CCL ÅberprÅfen */
			check_ccl( &ccl_str );

			/* Einen neuen NEA erzeugen */
			if( ( nea_ind = init_nea() ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
			{
				/* Ein ZustandsÅbergang erzeugen */
				if( ( nea_bra( nea_ind, FINIT_STATE, NEW_FINIT_STATE, lr_sem_stack[lr_sem_count - 1].ccl_str->ccl, 1, 0, 0 ) ) < 0 )
					/* Fehler */
					lr_val->l = -1;

				else
					/* Vererbung */
					lr_val->nea_ind = nea_ind;
			}

			break;
		}

		case ccl_0 :
		{
			/* ccl_0	->	ccl 'CHARACTER' 'TO' 'CHARACTER' */

			/* Eine Zeichenklasse erzeugen */
			insert_ccl( &ccl_str, lr_sem_stack[lr_sem_count - 2].b, lr_sem_stack[lr_sem_count].b );

			/* Vererbung */
			lr_val->ccl_str = &ccl_str;
			break;
		}

		case ccl_1 :
		{
			/* ccl_1	->	ccl 'CHARACTER' */

			/* Eine Zeichenklasse mit einem Zeichen erzeugen */
			insert_ccl( &ccl_str, lr_sem_stack[lr_sem_count].b, lr_sem_stack[lr_sem_count].b );

			/* Vererbung */
			lr_val->ccl_str = &ccl_str;
			break;
		}

		case ccl_2 :
		{
			/* ccl_2	->	'EPSILON' */

			/* Zeichenklasse initialisieren */
			memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );

			/* Vererbung */
			lr_val->ccl_str = &ccl_str;

			/* Analysiere RE */
			analyze++;
			break;
		}

		case string_0 :
		{
			/* string_0	->	string 'CHARACTER' */

			/* Zeichenklasse initialisieren */
			memset( ( VOID * ) ccl_str.ccl, 0, CCL_WIDTH );

			/* Eine Zeichenklasse mit einem Zeichen erzeugen */
			insert_ccl( &ccl_str, lr_sem_stack[lr_sem_count].b, lr_sem_stack[lr_sem_count].b );

			/* CCL ÅberprÅfen */
			check_ccl( &ccl_str );

			/* Ein ZustandsÅbergang erzeugen */
			if( ( nea_bra( lr_sem_stack[lr_sem_count - 1].nea_ind, FINIT_STATE, NEW_FINIT_STATE, ccl_str.ccl, 0, 0, 0 ) ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
				/* Vererbung */
				lr_val->nea_ind = lr_sem_stack[lr_sem_count - 1].nea_ind;

			/* Analysiere RE */
			analyze++;

			break;
		}

		case string_1 :
		{
			/* string_1	->	'EPSILON' */

			/* Einen neuen NEA erzeugen */
			if( ( nea_ind = init_nea() ) < 0 )
				/* Fehler */
				lr_val->l = -1;

			else
			{
				/* Ein ZustandsÅbergang erzeugen */
				if( ( nea_bra( nea_ind, FINIT_STATE, NEW_FINIT_STATE, 0L, 0, 0, _EPSILON ) ) < 0 )
					/* Fehler */
					lr_val->l = -1;

				else
					/* Vererbung */
					lr_val->nea_ind = nea_ind;
			}

			/* Analysiere RE */
			analyze++;
			break;
		}

		case predef_string_0:
		{
			/*  predef_string 'CHARACTER' */

			/* Zeichen merken */
			st[lr_sem_stack[lr_sem_count - 1].w][st2_count++] = lr_sem_stack[lr_sem_count].b;
			break;
		}

		case predef_string_1:
		{
			/* EPSILON */

			/* Weitergeben */
			st2_count = 0;
			lr_val->w = st1_count++;

			if( st1_count >= 10 )
				fprintf( stderr, "\nsem_act.c: st1_count ist zu groû !" );

			break;
		}

		default:
		{
			/* HierfÅr gibt es keine semmantische Aktion */
			fprintf( stderr, "\n\nKeine semmantische Aktion fÅr %ld!", prod );
		}
	}

	/* Alles OK */
	return( *lr_val );
}																																