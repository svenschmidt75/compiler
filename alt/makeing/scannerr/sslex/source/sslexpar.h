/*
LR-Parser-Generator V1.00ž
(C) 1997,1998 by Sven Schmidt

Debug-Version
*/


/*************/
/* Terminale */
/*************/


#define EPSILON			0
#define ACCEPT			1
#define BEG_LINE			2
#define EOF_OP			3
#define BEG_NAME_LIST			4
#define END_NAME_LIST			5
#define COMMA			6
#define END_SYM			7
#define OR			8
#define LOOKAHEAD_SYM			9
#define MORE			10
#define ONE_OR_MORE			11
#define ONE_OR_ZERO			12
#define BROPEN			13
#define NUMBER			14
#define BRCLOSE			15
#define ANY_CHAR			16
#define PREVCCL			17
#define STRING_SYM			18
#define KLOPEN			19
#define KLCLOSE			20
#define CHARACTER			21
#define AROPEN			22
#define ARCLOSE			23
#define EXP			24
#define TO			25


/****************/
/* Produktionen */
/****************/

#define start			1	/*  goal 'ACCEPT' */
#define goal			2	/*  init_sslex sslexrule */
#define init_sslex			3	/*  'EPSILON' */
#define sslexrule_0			4	/*  sc 'BEG_LINE' rule */
#define sslexrule_1			5	/*  sc rule */
#define sslexrule_2			6	/*  'BEG_LINE' rule */
#define sslexrule_3			7	/*  rule */
#define sslexrule_4			8	/*  sc 'EOF_OP' */
#define sslexrule_5			9	/*  'EOF_OP' */
#define sc			10	/*  'BEG_NAME_LIST' namelist2 'END_NAME_LIST' */
#define namelist2_0			11	/*  namelist2 'COMMA' predef_string */
#define namelist2_1			12	/*  predef_string */
#define rule_0			13	/*  re2 re */
#define rule_1			14	/*  re2 re 'END_SYM' */
#define rule_2			15	/*  re 'END_SYM' */
#define rule_3			16	/*  re */
#define re_0			17	/*  re 'OR' series */
#define re_1			18	/*  series */
#define re2			19	/*  re 'LOOKAHEAD_SYM' */
#define series_0			20	/*  series singleton */
#define series_1			21	/*  singleton */
#define singleton_0			22	/*  singleton 'MORE' */
#define singleton_1			23	/*  singleton 'ONE_OR_MORE' */
#define singleton_2			24	/*  singleton 'ONE_OR_ZERO' */
#define singleton_3			25	/*  singleton 'BROPEN' 'NUMBER' 'COMMA' 'NUMBER' 'BRCLOSE' */
#define singleton_4			26	/*  singleton 'BROPEN' 'NUMBER' 'COMMA' 'BRCLOSE' */
#define singleton_5			27	/*  singleton 'BROPEN' 'NUMBER' 'BRCLOSE' */
#define singleton_6			28	/*  singleton 'BROPEN' predef_string 'BRCLOSE' */
#define singleton_7			29	/*  'BROPEN' predef_string 'BRCLOSE' */
#define singleton_8			30	/*  'ANY_CHAR' */
#define singleton_9			31	/*  fullccl */
#define singleton_10			32	/*  'PREVCCL' */
#define singleton_11			33	/*  'STRING_SYM' string 'STRING_SYM' */
#define singleton_12			34	/*  'KLOPEN' re 'KLCLOSE' */
#define singleton_13			35	/*  'CHARACTER' */
#define fullccl_0			36	/*  'AROPEN' ccl 'ARCLOSE' */
#define fullccl_1			37	/*  'AROPEN' 'EXP' ccl 'ARCLOSE' */
#define ccl_0			38	/*  ccl 'CHARACTER' 'TO' 'CHARACTER' */
#define ccl_1			39	/*  ccl 'CHARACTER' */
#define ccl_2			40	/*  'EPSILON' */
#define string_0			41	/*  string 'CHARACTER' */
#define string_1			42	/*  'EPSILON' */
#define predef_string_0			43	/*  predef_string 'CHARACTER' */
#define predef_string_1			44	/*  'EPSILON' */



/**************************************/
/* Anlauf-Produktionen nach Reduktion */
/**************************************/


#define GOAL 1
#define INIT_SSLEX 2
#define SSLEXRULE 3
#define SC 4
#define NAMELIST2 5
#define RULE 6
#define RE 7
#define RE2 8
#define SERIES 9
#define SINGLETON 10
#define FULLCCL 11
#define CCL 12
#define STRING 13
#define PREDEF_STRING 14



/* Funktionsprototypen */
MGLOBAL WORD scanner( VOID );
MGLOBAL WORD parse( VOID );