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
		
		// undocumented 8-bit
		regIXH, regIXL, regIYH, regIYL,
		
		// indirect via 16 bits registers
		indHL,		// (HL), generally can be used at same places as A B C D E H L, and is used for (IX+d) / (IY+d) forms
		indBC,      // (BC) only in LD with A
		indDE,		// (DE) only in LD with A
		indSP,		// (SP) only in EX (SP),HL and (X+d) forms
		indC,		// (C), only in IN and OUT
		

		// bit number 0 to 7
		bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7,
		
		// conditions
		condNZ, condZ, condNC, condC, condPO, condPE, condP, condM,
		
		// numeric operands
		num8, num16,
		
		// indirect via 16 bits number
		ind16,
	};
	
	enum OperandError {
		
		operrOK,
		operrMISSINGPAROPEN,		// missing opened parenthesis
		operrMISSINGPARCLOSE,		// missing closed parenthesis
		operrMISSINGCOMMA,			// missing comma between arguments
		operrTOKENNUMBER,			// wrong token number
		operrREGISTERNAME,			// unknown register name
		operrWRONGREGISTER,			// wrong register used, like LD H,L
		operrWRONGOP,				// wrong operator, like (IX-2)
		operrNOTNUMBER,				// a number was needed
	};
	
	/** Functions for each family of operand type. */
	bool reg8( ExpVector* tokens, int& curtoken, OperandType& reg8 );			// regA to regR
	bool reg16( ExpVector* tokens, int& curtoken, OperandType& reg16 );			// regAF to regIY
	bool indirectC( ExpVector* tokens, int& curtoken, OperandType& regC );		// indC
	bool indirectHL( ExpVector* tokens, int& curtoken, OperandType& regHL );		// indHL
	bool indirectBC( ExpVector* tokens, int& curtoken, OperandType& regBC );		// indHL
	bool indirectDE( ExpVector* tokens, int& curtoken, OperandType& regDE );		// indHL
	bool indirectSP( ExpVector* tokens, int& curtoken, OperandType& regSP );		// indSP
	OperandError indirectX( ExpVector* tokens, int& curtoken, OperandType& regX, int& value ); 		// indIXd, indIYd
	bool bitnumber( ExpVector* tokens, int& curtoken, OperandType& bit );		// bit0 to bit7
	bool condition( ExpVector* tokens, int& curtoken, OperandType& cond );		// condNZ to condP
	bool number8( ExpVector* tokens, int& curtoken, int& value );		// num8
	bool number16( ExpVector* tokens, int& curtoken, int& value );	// num16
	OperandError indirect16( ExpVector* tokens, int curtoken, int& value, int& lasttoken );	// ind16
	
	// get the subcode for a register code
	int getsubcode(OperandType reg);
	int getprefix(OperandType reg);

	// High level functions for analyzing a CodeLine operands
	
	// register flags must be in the same order as OperandType registers:
	//	regA, regB, regC, regD, regE, regH, regL,
	//	regI, regR, regF,
	//	regAF, regAFp, regBC, regDE, regHL, regSP, regIX, regIY,
	//  regIXH, regIXL, regIYH, regIYL,
	
	enum REGFLAGS {
		// 8 bits registers, returned by reg8() function
		A		= 1 << 0,
		B		= 1 << 1,
		C		= 1 << 2,
		D		= 1 << 3,
		E		= 1 << 4,
		H		= 1 << 5,
		L		= 1 << 6,
		I		= 1 << 7,
		R		= 1 << 8,
		F		= 1 << 9,
		// 16 bits registers, returned by reg16() function
		AF		= 1 << 10,
		AFp		= 1 << 11,
		BC		= 1 << 12,
		DE		= 1 << 13,
		HL		= 1 << 14,
		SP		= 1 << 15,
		IX		= 1 << 16,
		IY		= 1 << 17,
		// undocumented 8-bits
		IXH 	= 1 << 18,
		IXL		= 1 << 19,
		IYH 	= 1 << 20,
		IYL		= 1 << 21,

		ABCDEHL = A | B | C | D | E | H | L,
		UNDOC = IXH | IXL | IYH | IYL,
		BCDESP = BC | DE | SP,
		BCDEHLSP = BC | DE | HL | SP,
		XY = IX | IY,
		HLXY = HL | IX | IY,
	};
	
	// helpers for instruction assembling
	bool RegAccept(int flags, OperandType reg) ;
	
	/** Returns true if the tokens array has at least the resquested number of tokens available starting at curtoken. */
	bool EnoughTokensLeft(class CodeLine& codeline, int number) ;
	
	/** Returns true if current token is a comma, and go next token. */
	bool GetComma(class CodeLine& codeline) ;
	
	/** Returns true if current token is recognized as an 8-bit register, and go next token. */
	bool GetReg8(class CodeLine& codeline, OperandType& reg, unsigned int regs = 0xFFFFFFFF ) ;
	
	/** Returns true if current token is recognized as an 16-bit register, and go next token. */
	bool GetReg16(class CodeLine& codeline, OperandType& reg, unsigned int regs = 0xFFFFFFFF ) ;
	
	/** Returns true if current token is recognized as (C), and go next token. */
	bool GetIndC( class CodeLine& codeline ) ;
	
	/** Returns true if current token is recognized as (HL), and go next token. */
	bool GetIndHL( class CodeLine& codeline ) ;
	
	/** Returns true if current token is recognized as (HL), and go next token. */
	bool GetIndBC( class CodeLine& codeline ) ;
	
	/** Returns true if current token is recognized as (HL), and go next token. */
	bool GetIndDE( class CodeLine& codeline );
	
	/** Returns true if current token is recognized as (SP), and go next token. */
	bool GetIndSP( class CodeLine& codeline );
	
	/** Returns true if current token is recognized as (IX+d) or (IY+d), and go next token. */
	bool GetIndX( class CodeLine& codeline, OperandType& regX, int& value );
	
	/** Returns true if current token is recognized as a bit number (0-7), and go next token. */
	bool GetBitNumber( class CodeLine& codeline, OperandType& bit );
	
	/** Returns true if current token is recognized as a condition, and go next token. */
	bool GetCond( class CodeLine& codeline, OperandType& cond ) ;
	
	/** Returns true if current token is recognized as an 8-bit number, and go next token. */
	bool GetNum8( class CodeLine& codeline, int& value );
	
	/** Returns true if current token is recognized as an 8-bit number, and go next token. */
	bool GetNum16( class CodeLine& codeline, int& value );
	
	/** Returns true if current token is recognized as an (16-bit) indirect addressing, and go next token. */
	bool GetInd16( class CodeLine& codeline, int& value );

}
#endif /* Z80_Operands_h */
