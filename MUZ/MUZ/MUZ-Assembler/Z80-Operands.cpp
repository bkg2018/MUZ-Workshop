//
//  Z80-Operands.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 23/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "Z80-Operands.h"
#include "ParseToken.h"
#include "StrUtils.h"

#include <string>
#include <unordered_map>
#include <map>

namespace MUZ {
	
	/** Functions for each family of operand type. */

	
	// Maps of names
	std::unordered_map<std::string, OperandType> registers8;    // list of acceptable 8-bit registers
	std::unordered_map<std::string, OperandType> registers16;	// list of acceptable 16-bit registers
	std::unordered_map<std::string, OperandType> conditions;	// list of acceptable conditions
	
	// Maps off sub-encoding for addressing
	std::map<OperandType, int> regsubcode;

	// Initialize registers map
	void initRegisterMap() {
		
		// offsets for some registers in some encodings
		regsubcode[regB] = 0;
		regsubcode[regC] = 1;
		regsubcode[regD] = 2;
		regsubcode[regE] = 3;
		regsubcode[regH] = 4;
		regsubcode[regL] = 5;
		regsubcode[indHL] = 6;
		regsubcode[regA] = 7;
		regsubcode[regBC] = 0x00;
		regsubcode[regDE] = 0x10;
		regsubcode[regHL] = 0x20;
		regsubcode[regSP] = 0x30;
		regsubcode[regIX] = 0xDD;
		regsubcode[regIY] = 0xFD;
		regsubcode[bit0] = 0x00;
		regsubcode[bit1] = 0x08;
		regsubcode[bit2] = 0x10;
		regsubcode[bit3] = 0x18;
		regsubcode[bit4] = 0x20;
		regsubcode[bit5] = 0x28;
		regsubcode[bit6] = 0x30;
		regsubcode[bit7] = 0x38;

		// authorized registers
		registers8["A"] = regA;
		registers8["B"] = regB;
		registers8["C"] = regC;
		registers8["D"] = regD;
		registers8["E"] = regE;
		registers8["H"] = regH;
		registers8["L"] = regL;
		registers8["I"] = regI;
		registers8["R"] = regR;
		
		registers16["AF"] = regAF;
		registers16["AF'"] = regAFp;
		registers16["BC"] = regBC;
		registers16["DE"] = regDE;
		registers16["HL"] = regHL;
		registers16["SP"] = regSP;
		registers16["IX"] = regIX;
		registers16["IY"] = regIY;
		
		conditions["NC"] = condNC;
		conditions["C"] = condC;
		conditions["NZ"] = condNZ;
		conditions["Z"] = condZ;
		conditions["PE"] = condPE;
		conditions["PO"] = condPO;
		conditions["P"] = condP;
		conditions["M"] = condM;

	}
	
	/** Returns the subcode for a register code. Used for instructions accepting a reg8 spec or a reg16 spec.
	 	Returns a 0 for any invalid register or addressing code.
	 */
	int getsubcode( OperandType reg )
	{
		if (regsubcode.count(reg))
			return regsubcode[reg];
		return 0;
	}
	
	// regA to regR
	bool reg8( ExpVector* tokens, int curtoken, OperandType& reg8, int& value )
	{
		ParseToken& token =tokens->at(curtoken);
		if (token.type != tokenTypeLETTERS) return false;
		if (registers8.count(token.source)) {
			reg8 = registers8[token.source];
			return true;
		}
		return false;
	}

	// regAF to regIY
	bool reg16( ExpVector* tokens, int curtoken, OperandType& reg16, int& value )
	{
		ParseToken& token =tokens->at(curtoken);
		if (token.type != tokenTypeLETTERS) return false;
		if (registers16.count(token.source)) {
			reg16 = registers16[token.source];
			return true;
		}
		return false;
	}

	// indC
	bool indirectC( ExpVector* tokens, int curtoken, OperandType& reg, int& value )
	{
		if (curtoken + 2 >= tokens->size() ) return false;
		ParseToken* token = &tokens->at(curtoken);
		if (token->type != tokenTypePAROPEN) return false;
		token = &tokens->at(curtoken + 1);
		if (token->type != tokenTypeLETTERS) return false;
		if (token->source != "C") return false;
		token = &tokens->at(curtoken + 2);
		if (token->type != tokenTypePARCLOSE) return false;
		reg = indC;
		return true;
	}

	// indHL
	bool indirectHL( ExpVector* tokens, int curtoken, OperandType& reg, int& value )
	{
		if (curtoken + 2 >= tokens->size() ) return false;
		ParseToken* token = &tokens->at(curtoken);
		if (token->type != tokenTypePAROPEN) return false;
		token = &tokens->at(curtoken + 1);
		if (token->type != tokenTypeLETTERS) return false;
		if (token->source != "HL") return false;
		token = &tokens->at(curtoken + 2);
		if (token->type != tokenTypePARCLOSE) return false;
		reg = indHL;
		return true;
	}

	// indSP
	bool indirectSP( ExpVector* tokens, int curtoken, OperandType& reg, int& value )
	{
		if (curtoken + 2 >= tokens->size() ) return false;
		ParseToken* token = &tokens->at(curtoken);
		if (token->type != tokenTypePAROPEN) return false;
		token = &tokens->at(curtoken + 1);
		if (token->type != tokenTypeLETTERS) return false;
		if (token->source != "SP") return false;
		token = &tokens->at(curtoken + 2);
		if (token->type != tokenTypePARCLOSE) return false;
		reg = indSP;
		return true;
	}

	// indIXd, indIYd
	bool indirectX( ExpVector* tokens, int curtoken, OperandType& regX, int& value )
	{
		if (curtoken + 4 >= tokens->size() ) return false;
		ParseToken* token = &tokens->at(curtoken);
		if (token->type != tokenTypePAROPEN) return false;
		if (! reg16(tokens, curtoken+1, regX, value)) return false;
		if (regX != regIX && regX != regIY) return false;
		token = &tokens->at(curtoken + 2);
		if (token->type != tokenTypeOP_PLUS) return false;
		token = &tokens->at(curtoken + 3);
		if (token->type != tokenTypeDECNUMBER) return false;
		value = dec_to_unsigned(token->source);
		token = &tokens->at(curtoken + 4);
		if (token->type != tokenTypePARCLOSE) return false;
		return true;
	}

	// bit0 to bit7
	bool bitnumber( ExpVector* tokens, int curtoken, OperandType& bit, int& value )
	{
		ParseToken* token = &tokens->at(curtoken);
		if (token->type != tokenTypeDECNUMBER) return false;
		value = dec_to_unsigned(token->source);
		if (value < 0 || value > 7) return false;
		if (value == 0) bit = bit0;
		else if (value == 1) bit = bit1;
		else if (value == 2) bit = bit2;
		else if (value == 3) bit = bit3;
		else if (value == 4) bit = bit4;
		else if (value == 5) bit = bit5;
		else if (value == 6) bit = bit6;
		else bit = bit7;
		return true;
	}

	// condNZ to condP
	bool condition( ExpVector* tokens, int curtoken, OperandType& cond, int& value )
	{
		ParseToken& token =tokens->at(curtoken);
		if (token.type != tokenTypeLETTERS) return false;
		if (conditions.count(token.source)) {
			cond = conditions[token.source];
			return true;
		}
		return false;
	}

	// num8
	bool number8( ExpVector* tokens, int curtoken, OperandType& number8, int& value )
	{
		ParseToken& token =tokens->at(curtoken);
		if (token.type != tokenTypeDECNUMBER) return false;
		value = dec_to_unsigned(token.source);
		if (value > 255) return false;
		number8 = num8;
		return true;
	}

	// num16
	bool number16( ExpVector* tokens, int curtoken, OperandType& number16, int& value )
	{
		ParseToken& token =tokens->at(curtoken);
		if (token.type != tokenTypeDECNUMBER) return false;
		value = dec_to_unsigned(token.source);
		if (value > 65535) return false;
		number16 = num16;
		return true;
	}

	// ind16
	bool indirect16( ExpVector* tokens, int curtoken, OperandType& number16, int& value )
	{
		if (curtoken + 2 >= tokens->size() ) return false;
		ParseToken* token = &tokens->at(curtoken);
		if (token->type != tokenTypePAROPEN) return false;
		token = &tokens->at(curtoken + 1);
		if (token->type != tokenTypeDECNUMBER) return false;
		value = dec_to_unsigned(token->source);
		token = &tokens->at(curtoken + 2);
		if (token->type != tokenTypePARCLOSE) return false;
		number16 = ind16;
		return true;
	}


} // namespace
