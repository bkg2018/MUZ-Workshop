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
		ADDRESSTYPE			address = 0;				// starting address for this code
		CodeType			codetype = codeTypeUNKNOWN;	// type of data for this code
		int					includefile = 0;			// > 0 when this line includes another sourcefile
		class Label*		label = nullptr;			// label if there is one on this line
		
		// helpers for instructions assembling
		
		/** Returns true if the tokens array has at least the resquested number of tokens available starting at curtoken. */
		bool enoughTokensLeft(int number) {
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
		bool GetNextComma() {
			if (tokens.at(curtoken).type == tokenTypeCOMMA) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an 8-bit register, and go next token. */
		bool GetNextReg8( OperandType& reg ) {
			int value;
			if (!enoughTokensLeft(1)) return false;
			if (reg8(&tokens, curtoken, reg, value)) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an 16-bit register, and go next token. */
		bool GetNextReg16( OperandType& reg ) {
			int value;
			if (!enoughTokensLeft(1)) return false;
			if (reg16(&tokens, curtoken, reg, value)) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (C), and go next token. */
		bool GetNextIndC( OperandType& regC ) {
			int value;
			if (!enoughTokensLeft(3)) return false;
			if (indirectC(&tokens, curtoken, regC, value)) {
				curtoken += 3;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetNextIndHL( OperandType& regHL ) {
			int value;
			if (!enoughTokensLeft(3)) return false;
			if (indirectHL(&tokens, curtoken, regHL, value)) {
				curtoken += 3;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (SP), and go next token. */
		bool GetNextIndSP( OperandType& regSP ) {
			int value;
			if (!enoughTokensLeft(3)) return false;
			if (indirectSP(&tokens, curtoken, regSP, value)) {
				curtoken += 3;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (IX+d) or (IY+d), and go next token. */
		bool GetNextIndX( OperandType& regX, int& value ) {
			if (!enoughTokensLeft(5)) return false;
			if (indirectX(&tokens, curtoken, regX, value)) {
				curtoken += 5;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as a bit nuùber (0-7), and go next token. */
		bool GetNextBitNumber( OperandType& bit ) {
			int value;
			if (!enoughTokensLeft(1)) return false;
			if (bitnumber(&tokens, curtoken, bit, value)) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as a condition, and go next token. */
		bool GetNextCond( OperandType& cond ) {
			int value;
			if (!enoughTokensLeft(1)) return false;
			if (condition(&tokens, curtoken, cond, value)) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an 8-bit number, and go next token. */
		bool GetNextNum8( OperandType& num8, int& value ) {
			if (!enoughTokensLeft(1)) return false;
			if (number8(&tokens, curtoken, num8, value)) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an 8-bit number, and go next token. */
		bool GetNextNum16( OperandType& num16, int& value ) {
			if (!enoughTokensLeft(1)) return false;
			if (number16(&tokens, curtoken, num16, value)) {
				curtoken += 1;
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as an (16-bit) indirect addressing, and go next token. */
		bool GetNextInd16( OperandType& num16, int& value ) {
			if (!enoughTokensLeft(3)) return false;
			if (indirect16(&tokens, curtoken, num16, value)) {
				curtoken += 3;
				return true;
			}
			return false;
		}

		/** Pushes codes. */
		void SetCode(DATATYPE b0) {
			code.push_back(b0);
		}
		void SetCode(DATATYPE b0, DATATYPE b1) {
			code.push_back(b0);
			code.push_back(b1);
		}
		void SetCode(DATATYPE b0, DATATYPE b1, DATATYPE b2) {
			code.push_back(b0);
			code.push_back(b1);
			code.push_back(b2);
		}
		void SetCode(DATATYPE b0, DATATYPE b1, DATATYPE b2, DATATYPE b3) {
			code.push_back(b0);
			code.push_back(b1);
			code.push_back(b2);
			code.push_back(b3);
		}
	};

}
#endif /* CodeLine_h */
