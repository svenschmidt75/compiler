/***************************************************************************
                          sslrtermdef.h  -  description
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

#ifndef SSLRTERMDEF_H
#define SSLRTERMDEF_H


/**
	@author SSLR V0.5a
	(c) 2002-2005 by Sven Schmidt
	eMail: s.schmidt@lboro.ac.uk
  */

/*
	Von CAdressingMode genügt hier eine Forward-Deklaration, da
	in SSLR_TYPE nur ein Zeiger auf diese Klasse benötigt werden.
	Sobald auf Methoden oder Member zugegriffen wird, muss inkludiert werden!
*/
class CAdressingMode;

#include "copcode.h"

//Terminals returned by the scanner
#define BCC	0
#define ASL	1
#define ASR	2
#define BCS	3
#define BEQ	4
#define BGE	5
#define BGT	6
#define BHI	7
#define BLE	8
#define BLS	9
#define BLT	10
#define BMI	11
#define EXPORT	12
#define BNE	13
#define BPL	14
#define BRA	15
#define BSR	16
#define BVC	17
#define BVS	18
#define CLR	19
#define CMP	20
#define BYTE	21
#define EXTB	22
#define PLUS	23
#define LOCAL	24
#define STRING	25
#define SINGLE	26
#define LABEL_DEF	27
#define MOVE	28
#define DC	29
#define EXT	30
#define LINK	31
#define OR	32
#define PC	33
#define SR	34
#define NAME	35
#define KLCLOSE	36
#define COMMA	37
#define JMP	38
#define JSR	39
#define NEGX	40
#define MULS	41
#define MULU	42
#define LEA	43
#define MINUS	44
#define ADDX	45
#define LONG	46
#define LSL	47
#define LSR	48
#define SUBX	49
#define ADRESS_REG	50
#define NUMBER	51
#define NEG	52
#define NOP	53
#define NOT	54
#define UNLK	55
#define DATA_REG	56
#define PEA	57
#define DATA_SEG	58
#define STRING_CODE	59
#define SCC	60
#define SCS	61
#define RTS	62
#define SHARP	63
#define SEQ	64
#define SGE	65
#define SGT	66
#define SHI	67
#define SLE	68
#define SLS	69
#define SLT	70
#define SMI	71
#define SNE	72
#define SPL	73
#define SSF	74
#define SST	75
#define SUB	76
#define DIVS	77
#define DIVU	78
#define SVC	79
#define SVS	80
#define DOUBLE	81
#define TST	82
#define KLOPEN	83
#define STACK	84
#define EPSILON	85
#define IMPORT	86
#define ADD	87
#define XOR	88
#define TEXT_SEG	89
#define AND	90
#define WORD	91
#define ACCEPT	92



//Type for semantic actions
typedef union
{

	//Nummer eines Registers
	unsigned long			_integer;

	//ganze Zahlen
	unsigned long			_number;

	//Bezeichner
	char					*_name;

	//Adressierungsart
	CAdressingMode			*_adrMode;

	//Extension
	COpcode::_extension		_size;

}SSLR_TYPE;


#endif

