//
//  Z80-Operands.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 23/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Z80_Operands_h
#define Z80_Operands_h

#include "ExpVector.h"
#include <map>

namespace MUZ {
	
	/** Enumeration for the possible Z-80 operand types.

	The same goes for 16-bits registers BC DE HL SP which have values to help their encoding:

	*/
	
	enum OperandType {
		
		// 8 bits registers, returned by reg8() function
		regA, regB, regC, regD, regE, regH, regL,
		regI, regR, regF,
		
		// 16 bits registers, returned by reg16() function
		regAF, regAFp, regBC, regDE, regHL, regSP, regIX, regIY,
		
		// indirect via 16 bits registers
		indHL,		// (HL), generally can be used at same places as A B C D E H L
		indBC,      // (BC)
		indDE,		// (DE)
		indSP,		// (SP)
		indC,		// (C)
		indIXd,		// (IX+d)
		indIYd,		// (IY+d)
		
		// bit number 0 to 7
		bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7,
		
		// conditions
		condNZ, condZ, condNC, condC, condPO, condPE, condP, condM,
		
		// numeric operands
		num8, num16,
		
		// indirect via 16 bits number
		ind16,
	};
	
	/** This must be called before any register can be recognized. */
	void initRegisterMap();
	
	/** Functions for each family of operand type. */
	bool reg8( ExpVector* tokens, int curtoken, OperandType& reg8, int& value );			// regA to regR
	bool reg16( ExpVector* tokens, int curtoken, OperandType& reg16, int& value );			// regAF to regIY
	bool indirectC( ExpVector* tokens, int curtoken, OperandType& regC, int& value );		// indC
	bool indirectHL( ExpVector* tokens, int curtoken, OperandType& regHL, int& value );		// indHL
	bool indirectBC( ExpVector* tokens, int curtoken, OperandType& regBC, int& value );		// indHL
	bool indirectDE( ExpVector* tokens, int curtoken, OperandType& regDE, int& value );		// indHL
	bool indirectSP( ExpVector* tokens, int curtoken, OperandType& regSP, int& value );		// indSP
	bool indirectX( ExpVector* tokens, int curtoken, OperandType& regX, int& value ); 		// indIXd, indIYd
	bool bitnumber( ExpVector* tokens, int curtoken, OperandType& bit, int& value );		// bit0 to bit7
	bool condition( ExpVector* tokens, int curtoken, OperandType& cond, int& value );		// condNZ to condP
	bool number8( ExpVector* tokens, int curtoken, OperandType& number8, int& value );		// num8
	bool number16( ExpVector* tokens, int curtoken, OperandType& number16, int& value );	// num16
	bool indirect16( ExpVector* tokens, int curtoken, OperandType& number16, int& value );	// ind16
	
	// get the subcode for a register code
	int getsubcode(OperandType reg);
}
#endif /* Z80_Operands_h */
