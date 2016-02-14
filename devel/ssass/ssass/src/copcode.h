/***************************************************************************
 *   Copyright (C) 2005 by Sven Schmidt   *
 *   s.schmidt@lboro.ac.uk   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef COPCODE_H
#define COPCODE_H


/*
	Dies ist die Basis-Klasse für die Opcodes.
	Es gibt Opcodes, die keinen Operanden haben wie z.B. nop, die
	in CNoOperandOpcode.
	Solche, die einen haben, wie z.B. bra label, sind in COneOperandOpcode
	implementiert. Schliesslich sind die mit zweien in CTwoOperandOpcode
	implementiert.
*/


/**
@author Sven Schmidt
*/
class COpcode
{
public:

	//Opcodes
	enum _opcode { _BCC, _ASL, _ASR, _BCS, _BEQ, _BGE, _BGT, _BHI, _BLE, _BLS, _BLT,
				_BMI, _BNE, _BPL, _BRA, _BSR, _BVC, _BVS, _CLR, _CMP, _EXTB,
				_LABEL_DEF, _MOVE, _EXT, _LINK, _OR, _JMP, _JSR, _NEGX, _MULS,
				_MULU, _LEA, _ADDX, _LSL, _LSR, _SUBX, _NEG, _NOP, _NOT, _UNLK,
				_PEA, _SCC, _SCS, _RTS, _SEQ, _SGE, _SGT, _SHI, _SLE, _SLS, _SLT,
				_SMI, _SNE, _SPL, _SSF, _SST, _SUB, _DIVS, _DIVU, _SVC, _SVS,
				_TST, _ADD, _XOR, _AND };

	//wird für die Identifizierung der jeweiligen (abgeleiteten) Klasse benötigt
	enum _typeId { _COPCODE, _CNO, _CONE, _CTWO };

	//Typ für die Extensions .b, .w, .l, .s, .d
	enum _extension { _NOEXT, _BYTE, _WORD, _LONG, _SINGLE, _DOUBLE };

	//Kon- und Destruktor
	COpcode( _opcode );
	virtual ~COpcode();

	/*
		Der Zuweisungsoperator muss hier überladen werden, weil COpcode ein const-Element
		hat, welches bei der Erzeugung von COpcode initialisiert werden muss.
		Wenn man nun z.B. vector<COpcode> benutzen möchte, wird der Standard-Assignment-Konstruktor
		benutzt, der aber von dem const-Element nichts weiss.
	*/
	COpcode &operator=( const COpcode & );

	//Extension setzen
	void setExt( _extension );

	//Zeile eintragen
	void setLine( unsigned long );

	//Zeile zurückliefern
	unsigned long getLine( void ) const;

	//wer bin ich
	virtual _typeId getInternType( void ) const;

	//Adresse des Opcodes zurückliefern
	virtual long getAdress( void ) const;

	//Adresse des Opcodes setzen
	virtual void setAdress( long );

	//codierte Größe in Bytes dieses Befehls
	virtual short getSize( void ) const;


private:

	//.b, .w, .l, .s, .d
	unsigned char			_ext;


protected:

	//Opcode-Nummer
	const _opcode			_op;

	//Zeile des Befehls
	unsigned long			_line;

	//Adresse, an der der assemblierte Befehl steht
	long					_adr;
};

#endif
