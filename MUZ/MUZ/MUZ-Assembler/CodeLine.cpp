//
//  CodeLine.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 28/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "CodeLine.h"
#include "Assembler.h"

namespace MUZ {
	
	/** Returns true if current token is recognized as (IX+d) or (IY+d), and go next token. */
	bool CodeLine::GetIndX( OperandType& regX, int& value ) {
		if (!EnoughTokensLeft(5)) return false;
		OperandError operr = indirectX(&tokens, curtoken, regX, value);
		if (operr == operrOK) {
			return true;
		}
		if (as->IsFirstPass() && operr == operrNOTNUMBER) {
			// probably unresolved label, simulate success with neutral value
			value = 0;
			return true;
		}
		return false;
	}

	/** Returns true if current token is recognized as a bit number (0-7), and go next token. */
	bool CodeLine::GetBitNumber( OperandType& bit ) {
		int value;
		if (!EnoughTokensLeft(1)) return false;
		int worktoken = curtoken;
		if (reg8(&tokens, worktoken, bit, value)) return false;//TODO: return explicit error (register name for 8-bit value)
		if (reg16(&tokens, worktoken, bit, value)) return false;//TODO: return explicit error (register name for 8-bit value)
		if (bitnumber(&tokens, curtoken, bit, value)) {
			return true;
		}
		if (as->IsFirstPass()) {
			// not number: probably unresolved label, simulate success with neutral value
			value = 0;
			return true;
		}
		return false;
	}

	/** Returns true if current token is recognized as an 8-bit number, and go next token. */
	bool CodeLine::GetNum8( int& value ) {
		if (!EnoughTokensLeft(1)) return false;
		OperandType num8;
		// forbid register names
		int worktoken = curtoken;
		if (reg8(&tokens, worktoken, num8, value)) return false;//TODO: return explicit error (register name for 8-bit value)
		if (reg16(&tokens, worktoken, num8, value)) return false;//TODO: return explicit error (register name for 8-bit value)
		// now only numbers or labels
		if (number8(&tokens, curtoken, num8, value)) {
			return true;
		}
		if (as->IsFirstPass()) {
			// not number: probably unresolved label, simulate success with neutral value
			value = 0;
			return true;
		}
		return false;
	}
	
	/** Returns true if current token is recognized as an 8-bit number expression, and go next token. */
	bool CodeLine::GetNum16( int& value ) {
		if (!EnoughTokensLeft(1)) return false;
		OperandType num16;
		int worktoken = curtoken;
		if (reg8(&tokens, worktoken, num16, value)) return false;//TODO: return explicit error (register name for 16-bit value)
		if (reg16(&tokens, worktoken, num16, value)) return false;//TODO: return explicit error (register name for 16-bit value)
		if (number16(&tokens, curtoken, num16, value)) {
			// value *= mutliplier;
			return true;
		}
		if (as->IsFirstPass()) {
			// not number: probably unresolved label, simulate success with neutral value
			value = 0;
			return true;
		}
		return false;
	}

	/** Returns true if current token is recognized as an (16-bit) indirect addressing, and go next token. */
	bool CodeLine::GetInd16( int& value ) {
		if (!EnoughTokensLeft(3)) return false;
		OperandType num16;
		int lasttoken;
		OperandError operr = indirect16(&tokens, curtoken, num16, value, lasttoken);
		if (operr==operrOK) {
			curtoken = lasttoken;
			return true;
		}
		if (operr==operrNOTNUMBER && as->IsFirstPass()) {
			// not number: probably unresolved label, simulate success with neutral value
			value = 0;
			curtoken = lasttoken;
			return true;
		}
		// other errors do not update curtoken
		return false;
	}

} // namespace
