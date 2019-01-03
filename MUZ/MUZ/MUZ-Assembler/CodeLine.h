//
//  CodeLine.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 18/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef CodeLine_h
#define CodeLine_h

#include <string>
#include <vector>

#include "Types.h"
#include "ExpVector.h"
#include "Z80-Operands.h"

namespace MUZ {
	
	/** Type of code content.*/
	enum CodeType
	{
		codeTypeUNKNOWN,
		codeTypeCODE,
		codeTypeDATA,
		codeTypeSTRING
	};

	// register flags must be in the same order as OperandType registers:
//	regA, regB, regC, regD, regE, regH, regL,
//	regI, regR, regF,
//	regAF, regAFp, regBC, regDE, regHL, regSP, regIX, regIY,

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

		ABCDEHL = A | B | C | D | E | H | L,
		BCDESP = BC | DE | SP,
		BCDEHLSP = BC | DE | HL | SP,
		XY = IX | IY,
		HLXY = HL | IX | IY,
	};
	

	/** Source and assembled content for one line of source code. */
	struct CodeLine
	{
		// source
		int					file;						// source file reference in the assembler
		long				offset;						// position in the source file
		int 				size;						// number of bytes in this source line
		int					line;						// duplicates the CodeLine index in SourceFile array
		std::string 		source;						// TODO: replace with file direct access
		
		// parsed tokens
		ExpVector			tokens;						// Tokens as prepared by the parser
		int					instructiontoken;			// The instruction token index
		int					curtoken;					// current position for Reset/Next functions
		
		// assembled code
		bool				assembled = false; 			// true when assembled
		std::vector<BYTE>	code;						// assembled code
		int					cyclesmin;					// machine cycles for this code
		int					cyclesmax;					// machine cycles for this code
		ADDRESSTYPE			address = 0;				// starting address for this code
		CodeType			codetype = codeTypeUNKNOWN;	// type of data for this code
		int					includefile = 0;			// file reference when this line includes another sourcefile
		class Label*		label = nullptr;			// label if there is one on this line
		class Assembler*	as = nullptr;				// referenc for pass check
		
		// helpers for instructions assembling
		bool RegAccept(int flags, OperandType reg) {
			int f = 1 << (int)reg;
			return ((f & flags) == f);
		}
		
		/** Returns true if the tokens array has at least the resquested number of tokens available starting at curtoken. */
		bool EnoughTokensLeft(int number) {
			// if curtoken is 'i' and we request 3 tokens,
			// then the tokens size must be 'i' + 3 or more
			return (tokens.size() >= curtoken + number) ;
		}
		
		/** Resets the token exploration system to the first token after the instruction. */
		void Reset(int start) {
			instructiontoken = start;
			curtoken = instructiontoken + 1;
		}
		
		/** Returns true if current token is a comma, and go next token. */
		bool GetComma() {
			if (!EnoughTokensLeft(1)) return false;
			if (tokens.at(curtoken).type == tokenTypeCOMMA) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an 8-bit register, and go next token. */
		bool GetReg8( OperandType& reg, unsigned int regs = 0xFFFFFFFF ) {
			int value;
			if (!EnoughTokensLeft(1)) return false;
			int worktoken = curtoken;
			if (reg8(&tokens, worktoken, reg, value)) {
				if (RegAccept(regs, reg)) {
					curtoken = worktoken;
					return true;
				}
			}
			return false;
		}
		/** Returns true if current token is recognized as an 16-bit register, and go next token. */
		bool GetReg16( OperandType& reg, unsigned int regs = 0xFFFFFFFF ) {
			int value;
			if (!EnoughTokensLeft(1)) return false;
			int worktoken = curtoken;
			if (reg16(&tokens, worktoken, reg, value)) {
				if (RegAccept(regs, reg)) {
					curtoken = worktoken;
					return true;
				}
				
			}
			return false;
		}
		/** Returns true if current token is recognized as (C), and go next token. */
		bool GetIndC(  ) {
			int value;
			if (!EnoughTokensLeft(3)) return false;
			OperandType regC;
			if (indirectC(&tokens, curtoken, regC, value)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetIndHL(  ) {
			int value;
			if (!EnoughTokensLeft(3)) return false;
			OperandType regHL;
			if (indirectHL(&tokens, curtoken, regHL, value)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetIndBC(  ) {
			int value;
			if (!EnoughTokensLeft(3)) return false;
			OperandType regBC;
			if (indirectBC(&tokens, curtoken, regBC, value)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetIndDE(  ) {
			int value;
			if (!EnoughTokensLeft(3)) return false;
			OperandType regDE;
			if (indirectDE(&tokens, curtoken, regDE, value)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (SP), and go next token. */
		bool GetIndSP(  ) {
			int value;
			if (!EnoughTokensLeft(3)) return false;
			OperandType regSP;
			if (indirectSP(&tokens, curtoken, regSP, value)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (IX+d) or (IY+d), and go next token. */
		bool GetIndX( OperandType& regX, int& value );
		
		/** Returns true if current token is recognized as a bit number (0-7), and go next token. */
		bool GetBitNumber( OperandType& bit );
		
		/** Returns true if current token is recognized as a condition, and go next token. */
		bool GetCond( OperandType& cond ) {
			int value;
			if (!EnoughTokensLeft(1)) return false;
			if (condition(&tokens, curtoken, cond, value)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an 8-bit number, and go next token. */
		bool GetNum8( int& value );
		
		/** Returns true if current token is recognized as an 8-bit number, and go next token. */
		bool GetNum16( int& value );
		
		/** Returns true if current token is recognized as an (16-bit) indirect addressing, and go next token. */
		bool GetInd16( int& value );
		
		/** Set machine cycles. */
		void SetCycles(int mintime, int maxtime = -1) {
			cyclesmin = mintime;
			cyclesmax = maxtime > 0 ? maxtime : mintime;
		}
		
		/** Pushes codes. */
		void ResetCode() {
			code.clear();
		}
		void AddCode(DATATYPE b0) {
			code.push_back(b0 & DATAMASK);
		}
		void AddCode(DATATYPE b0, DATATYPE b1) {
			AddCode(b0);
			AddCode(b1);
		}
		void AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2) {
			AddCode(b0);
			AddCode(b1);
			AddCode(b2);
		}
		void AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2, DATATYPE b3) {
			AddCode(b0);
			AddCode(b1);
			AddCode(b2);
			AddCode(b3);
		}
	};

}
#endif /* CodeLine_h */
