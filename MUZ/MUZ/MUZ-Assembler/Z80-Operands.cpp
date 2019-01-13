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
#include "Expression.h"
#include "CodeLine.h"
#include "Assembler.h"
#include "All-Directives.h"

#include <string>
#include <unordered_map>
#include <map>

namespace MUZ {
	
	namespace Z80 {
	
		//MARK: - Maps for register names and sub codes
		
		// Maps of names
		std::unordered_map<std::string, OperandType> registers8;    // list of acceptable 8-bit registers
		std::unordered_map<std::string, OperandType> registers16;	// list of acceptable 16-bit registers
		std::unordered_map<std::string, OperandType> conditions;	// list of acceptable conditions
		
		// Maps of sub-encoding for addressing
		std::map<OperandType, int> regsubcode;
		std::map<OperandType, int> regprefix;

		// Initialize registers map
		void InitRegisterMap() {
			
			// offsets for registers in some encodings
			regsubcode[regB] = 0;
			regsubcode[regC] = 1;
			regsubcode[regD] = 2;
			regsubcode[regE] = 3;
			regsubcode[regH] = 4;
			regsubcode[regL] = 5;
			regsubcode[regF] = 6;
			regsubcode[indHL] = 6;
			regsubcode[regA] = 7;
			//undocumented
			regsubcode[regIXH] = 4;
			regsubcode[regIXL] = 5;
			regsubcode[regIYH] = 4;
			regsubcode[regIYL] = 5;
			regprefix[regIXH] = 0xDD;
			regprefix[regIYH] = 0xFD;
			regprefix[regIXL] = 0xDD;
			regprefix[regIYL] = 0xFD;

			regsubcode[regI] = 0x07;
			regsubcode[regR] = 0x0F;
			
			regsubcode[regBC] = 0x00;
			regsubcode[regDE] = 0x10;
			regsubcode[regHL] = 0x20;
			regsubcode[regSP] = 0x30;
			regsubcode[regAF] = 0x30; // push,pop

			regsubcode[regIX] = 0x20;//hl
			regsubcode[regIY] = 0x20;//hl

			regprefix[regIX] = 0xDD;
			regprefix[regIY] = 0xFD;
			
			regsubcode[bit0] = 0x00;
			regsubcode[bit1] = 0x08;
			regsubcode[bit2] = 0x10;
			regsubcode[bit3] = 0x18;
			regsubcode[bit4] = 0x20;
			regsubcode[bit5] = 0x28;
			regsubcode[bit6] = 0x30;
			regsubcode[bit7] = 0x38;
			
			regsubcode[condNZ] = 0x00;
			regsubcode[condZ]  = 0x08;
			regsubcode[condNC] = 0x10;
			regsubcode[condC]  = 0x18;
			regsubcode[condPO] = 0x20;
			regsubcode[condPE] = 0x28;
			regsubcode[condP]  = 0x30;
			regsubcode[condM]  = 0x38;
		
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
			registers8["F"] = regF;
			// undocumented
			registers8["IXH"] = regIXH;
			registers8["IXL"] = regIXL;
			registers8["IYH"] = regIYH;
			registers8["IYL"] = regIYL;

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
		
		// singleton to launch register maps init at run-time
		struct _auto_init_register_map {
			_auto_init_register_map() {
				InitRegisterMap();
			}
		} _runtime_auto_init_register_map;
		
		//MARK: - Low level tokens analysis for operand types
		
		/** Returns the subcode for a register code. Used for instructions accepting a reg8 spec or a reg16 spec.
			Returns a 0 for any invalid register or addressing code.
		 */
		int getsubcode( OperandType reg )
		{
			if (regsubcode.count(reg))
				return regsubcode[reg];
			return 0;
		}
		/** Returns the prefix for a register code. Used for instructions accepting IX, IY and undocumented forms.
		 Returns a 0 for any invalid register or addressing code.
		 */
		int getprefix( OperandType reg )
		{
			if (regprefix.count(reg))
				return regprefix[reg];
			return 0;
		}

		/** Parses current token and return the code for a 8-bit register regA to regH, regI or regR and undocumented. */
		bool reg8( ExpVector* tokens, int& curtoken, OperandType& reg8)
		{
			ParseToken& token =tokens->at(curtoken);
			if (token.type != tokenTypeLETTERS) return false;
			std::string source = std::to_upper(token.source);
			if (registers8.count(source)) {
				reg8 = registers8[source];
				curtoken += 1;
				return true;
			}
			return false;
		}

		/** Parses current token and return the code for a 16-bit register regAF, regAFp regBC regDE regHL regSP IX or regIY. */
		bool reg16( ExpVector* tokens, int& curtoken, OperandType& reg16 )
		{
			ParseToken& token =tokens->at(curtoken);
			if (token.type != tokenTypeLETTERS) return false;
			std::string source = std::to_upper(token.source);
			if (registers16.count(source)) {
				reg16 = registers16[source];
				curtoken += 1;
				return true;
			}
			return false;
		}

		/** Parses current token and return the code for an indirect access via(C): indC. */
		bool indirectC( ExpVector* tokens, int& curtoken, OperandType& reg )
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
			curtoken += 3;
			return true;
		}

		/** Parses current token and return the code for an indirect access via (HL): indHL. */
		bool indirectHL( ExpVector* tokens, int& curtoken, OperandType& reg )
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
			curtoken += 3;
			return true;
		}
		
		/** Parses current token and return the code for an indirect access via (BC): indBC. */
		bool indirectBC( ExpVector* tokens, int& curtoken, OperandType& reg )
		{
			if (curtoken + 2 >= tokens->size() ) return false;
			ParseToken* token = &tokens->at(curtoken);
			if (token->type != tokenTypePAROPEN) return false;
			token = &tokens->at(curtoken + 1);
			if (token->type != tokenTypeLETTERS) return false;
			if (token->source != "BC") return false;
			token = &tokens->at(curtoken + 2);
			if (token->type != tokenTypePARCLOSE) return false;
			reg = indBC;
			curtoken += 3;
			return true;
		}
		
		/** Parses current token and return the code for an indirect access via (DE): indDE. */
		bool indirectDE( ExpVector* tokens, int& curtoken, OperandType& reg )
		{
			if (curtoken + 2 >= tokens->size() ) return false;
			ParseToken* token = &tokens->at(curtoken);
			if (token->type != tokenTypePAROPEN) return false;
			token = &tokens->at(curtoken + 1);
			if (token->type != tokenTypeLETTERS) return false;
			if (token->source != "DE") return false;
			token = &tokens->at(curtoken + 2);
			if (token->type != tokenTypePARCLOSE) return false;
			reg = indDE;
			curtoken += 3;
			return true;
		}

		/** Parses current token and return the code for an indirect access via (SP): indSP. */
		bool indirectSP( ExpVector* tokens, int& curtoken, OperandType& reg )
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
			curtoken += 3;
			return true;
		}

		/** Parses current token and return the code for an indirect access via (IX+d) and (IY+d): indIX, indIY. */
		OperandError indirectX( ExpVector* tokens, int& curtoken, OperandType& regX, int& value )
		{
			if (curtoken + 4 >= tokens->size() ) return operrTOKENNUMBER;
			ParseToken* token = &tokens->at(curtoken);
			if (token->type != tokenTypePAROPEN) return operrMISSINGPAROPEN;
			int indextoken = curtoken + 1;
			if (! reg16(tokens, indextoken, regX )) return operrREGISTERNAME;
			if (regX != regIX && regX != regIY) return operrWRONGREGISTER;
			token = &tokens->at(curtoken + 2);
			if (token->type != tokenTypeOP_PLUS) return operrWRONGOP;
			// find closing parenthesis
			indextoken = curtoken + 3;// skip '(' regX '+'
			int parlevel = 1;
			for ( ; indextoken < tokens->size() ; indextoken++) {
				token = &tokens->at(indextoken);
				if (token->type == tokenTypePAROPEN) {
					parlevel += 1;
				} else if (token->type == tokenTypePARCLOSE) {
					parlevel -= 1;
					if (parlevel == 0) break;
				}
			}
			
			// evaluate the value after "+" and before closing parenthesis
			indextoken = indextoken - 1;
			ExpressionEvaluator eval;
			ParseToken evaluated = eval.Evaluate(*tokens, curtoken + 3, indextoken);
			if (evaluated.unsolved) {
				value = 0;
			}
			value = evaluated.asNumber();
			curtoken = indextoken + 1;// skips after closing parenthesis
			return operrOK;
		}

		/** Parses current token and return the code for a bit nunmber: bit0 to bit7. */
		bool bitnumber( ExpVector* tokens, int& curtoken, OperandType& bit )
		{
			ExpressionEvaluator eval;
			int lasttoken = -1;
			ParseToken evaluated = eval.Evaluate(*tokens, curtoken, lasttoken);
			if (evaluated.unsolved) {
				bit = bit0;
				curtoken = lasttoken;
				return true;
			}
			if ((evaluated.type == tokenTypeSTRING) || (evaluated.type == tokenTypeDECNUMBER)) {
				int value = evaluated.asNumber();
				if (value < 0 || value > 7) return false;
				if (value == 0) bit = bit0;
				else if (value == 1) bit = bit1;
				else if (value == 2) bit = bit2;
				else if (value == 3) bit = bit3;
				else if (value == 4) bit = bit4;
				else if (value == 5) bit = bit5;
				else if (value == 6) bit = bit6;
				else bit = bit7;
				curtoken = lasttoken ;
				return true;
			}
			return false;
		}

		/** Parses current token and return the code for a condition name: condNZ to condM. */
		bool condition( ExpVector* tokens, int& curtoken, OperandType& cond )
		{
			ParseToken& token =tokens->at(curtoken);
			if (token.type != tokenTypeLETTERS) return false;
			if (conditions.count(token.source)) {
				cond = conditions[token.source];
				curtoken += 1;
				return true;
			}
			return false;
		}

		/** Parses current token and return the value for an 8-bit number. */
		bool number8( ExpVector* tokens, int& curtoken, int& value )
		{
			ExpressionEvaluator eval;
			int lasttoken = -1;
			ParseToken evaluated = eval.Evaluate(*tokens, curtoken, lasttoken);
			if (evaluated.unsolved) {
				value = 0;
				curtoken = lasttoken + 1;
				return true;
			}
			if ((evaluated.type == tokenTypeSTRING) || (evaluated.type == tokenTypeDECNUMBER)) {
				value = evaluated.asNumber();
				if (value > 255) return false;
				curtoken = lasttoken + 1;
				return true;
			}
			return false;
		}

		/** Parses current token and return the value for a 16-bit number. */
		bool number16( ExpVector* tokens, int& curtoken, int& value )
		{
			ExpressionEvaluator eval;
			int lasttoken = -1;
			ParseToken evaluated = eval.Evaluate(*tokens, curtoken, lasttoken);
			if (evaluated.unsolved) {
				value = 0;
				curtoken = lasttoken + 1;
				return true;
			}
			if ((evaluated.type == tokenTypeSTRING) || (evaluated.type == tokenTypeDECNUMBER)) {
				value = evaluated.asNumber();
				if (value > 65535) return false;
				curtoken = lasttoken + 1;
				return true;
			}
			return false;
		}

		/** Compute a 16-bit value from a numeric expression between parenthesis. If parenthesis or a value cannot be found,
		 returns an error code. The last used token index is returned even if the expression doesn't compute a number but
		 have correct parenthesis. */
		OperandError indirect16( ExpVector* tokens, int curtoken, int& value, int& lasttoken )
		{
			if (curtoken + 2 >= tokens->size() ) return operrTOKENNUMBER;
			ParseToken* token = &tokens->at(curtoken);
			if (token->type != tokenTypePAROPEN) return operrMISSINGPAROPEN;
			token = &tokens->at(curtoken + 1);
			// find closing parenthesis
			lasttoken = curtoken + 2;
			int parlevel = 1;
			for ( ; lasttoken < tokens->size() ; lasttoken++) {
				token = &tokens->at(lasttoken);
				if (token->type == tokenTypePAROPEN) {
					parlevel += 1;
				} else if (token->type == tokenTypePARCLOSE) {
					parlevel -= 1;
					if (parlevel == 0) break;
				}
			}
			if (token->type != tokenTypePARCLOSE) return operrMISSINGPARCLOSE;
			// evaluate the tokens between parenthesis
			ExpressionEvaluator eval;
			lasttoken -= 1; // back from parenthesis close
			ParseToken evaluated = eval.Evaluate(*tokens, curtoken + 1, lasttoken );
			if (evaluated.unsolved) {
				value = 0;
				lasttoken = lasttoken + 1;
				return operrOK;
			}
			if ((evaluated.type == tokenTypeSTRING) || (evaluated.type == tokenTypeDECNUMBER)) {
				value = evaluated.asNumber();
				lasttoken = lasttoken + 1;// skips  closing parenthesis
				return operrOK;
			}
			lasttoken = lasttoken + 1;// skips  closing parenthesis
			return operrNOTNUMBER;
		}
		
		//MARK: - High level functions for CodeLine operands analysis

		// helpers for instruction assembling
		bool RegAccept(int flags, OperandType reg) {
			int f = 1 << (int)reg;
			return ((f & flags) == f);
		}
		
		/** Returns true if current token is recognized as an 8-bit register, and go next token. */
		bool GetReg8(CodeLine& codeline, OperandType& reg, unsigned int regs ) {
			if (!EnoughTokensLeft(codeline, 1)) return false;
			int worktoken = codeline.curtoken;
			if (reg8(&codeline.tokens, worktoken, reg)) {
				if (RegAccept(regs, reg)) {
					codeline.curtoken = worktoken;
					return true;
				}
			}
			return false;
		}
		/** Returns true if current token is recognized as an 16-bit register, and go next token. */
		bool GetReg16(CodeLine& codeline, OperandType& reg, unsigned int regs  ) {
			if (!EnoughTokensLeft(codeline, 1)) return false;
			int worktoken = codeline.curtoken;
			if (reg16(&codeline.tokens, worktoken, reg)) {
				if (RegAccept(regs, reg)) {
					codeline.curtoken = worktoken;
					return true;
				}
				
			}
			return false;
		}
		/** Returns true if current token is recognized as (C), and go next token. */
		bool GetIndC( CodeLine& codeline ) {
			if (!EnoughTokensLeft(codeline, 3)) return false;
			OperandType regC;
			if (indirectC(&codeline.tokens, codeline.curtoken, regC)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetIndHL( CodeLine& codeline ) {
			if (!EnoughTokensLeft(codeline, 3)) return false;
			OperandType regHL;
			if (indirectHL(&codeline.tokens, codeline.curtoken, regHL)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetIndBC( CodeLine& codeline ) {
			if (!EnoughTokensLeft(codeline, 3)) return false;
			OperandType regBC;
			if (indirectBC(&codeline.tokens, codeline.curtoken, regBC)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (HL), and go next token. */
		bool GetIndDE( CodeLine& codeline ) {
			if (!EnoughTokensLeft(codeline, 3)) return false;
			OperandType regDE;
			if (indirectDE(&codeline.tokens, codeline.curtoken, regDE)) {
				return true;
			}
			return false;
		}
		/** Returns true if current token is recognized as (SP), and go next token. */
		bool GetIndSP( CodeLine& codeline ) {
			if (!EnoughTokensLeft(codeline, 3)) return false;
			OperandType regSP;
			if (indirectSP(&codeline.tokens, codeline.curtoken, regSP)) {
				return true;
			}
			return false;
		}
		
		/** Returns true if current token is recognized as (IX+d) or (IY+d), and go next token. */
		bool GetIndX(CodeLine& codeline, OperandType& regX, int& value ) {
			if (!EnoughTokensLeft(codeline,5)) return false;
			OperandError operr = indirectX(&codeline.tokens, codeline.curtoken, regX, value);
			if (operr == operrOK) {
				return true;
			}
			if (codeline.as->IsFirstPass() && operr == operrNOTNUMBER) {
				// probably unresolved label, simulate success with neutral value
				value = 0;
				return true;
			}
			return false;
		}
		
		/** Returns true if current token is recognized as a bit number (0-7), and go next token. */
		bool GetBitNumber(CodeLine& codeline, OperandType& bit ) {
			if (!EnoughTokensLeft(codeline,1)) return false;
			int worktoken = codeline.curtoken;
			if (reg8(&codeline.tokens, worktoken, bit)) return false;//TODO: return explicit error (register name for 8-bit value)
			if (reg16(&codeline.tokens, worktoken, bit)) return false;//TODO: return explicit error (register name for 8-bit value)
			if (bitnumber(&codeline.tokens, codeline.curtoken, bit)) {
				return true;
			}
			if (codeline.as->IsFirstPass()) {
				// not number: probably unresolved label, simulate success with neutral value
				bit = bit0;
				return true;
			}
			return false;
		}
		
		/** Returns true if current token is recognized as a condition, and go next token. */
		bool GetCond( CodeLine& codeline, OperandType& cond ) {
			if (!EnoughTokensLeft(codeline, 1)) return false;
			if (condition(&codeline.tokens, codeline.curtoken, cond)) {
				return true;
			}
			return false;
		}
		
		/** Returns true if current token is recognized as an 8-bit number, and go next token. */
		bool GetNum8(CodeLine& codeline, int& value ) {
			if (!EnoughTokensLeft(codeline,1)) return false;
			OperandType num8;
			// forbid register names
			int worktoken = codeline.curtoken;
			if (reg8(&codeline.tokens, worktoken, num8)) return false;//TODO: return explicit error (register name for 8-bit value)
			if (reg16(&codeline.tokens, worktoken, num8)) return false;//TODO: return explicit error (register name for 8-bit value)
			// now only numbers or labels
			if (number8(&codeline.tokens, codeline.curtoken, value)) {
				return true;
			}
			if (codeline.as->IsFirstPass()) {
				// not number: probably unresolved label, simulate success with neutral value
				value = 0;
				return true;
			}
			return false;
		}
		
		/** Returns true if current token is recognized as an 8-bit number expression, and go next token. */
		bool GetNum16(CodeLine& codeline, int& value ) {
			if (!EnoughTokensLeft(codeline,1)) return false;
			OperandType num16;
			int worktoken = codeline.curtoken;
			if (reg8(&codeline.tokens, worktoken, num16)) return false;//TODO: return explicit error (register name for 16-bit value)
			if (reg16(&codeline.tokens, worktoken, num16)) return false;//TODO: return explicit error (register name for 16-bit value)
			if (number16(&codeline.tokens, codeline.curtoken, value)) {
				// value *= mutliplier;
				return true;
			}
			if (codeline.as->IsFirstPass()) {
				// not number: probably unresolved label, simulate success with neutral value
				value = 0;
				return true;
			}
			return false;
		}
		
		/** Returns true if current token is recognized as an (16-bit) indirect addressing, and go next token. */
		bool GetInd16(CodeLine& codeline, int& value ) {
			if (!EnoughTokensLeft(codeline,3)) return false;
			int lasttoken;
			OperandError operr = indirect16(&codeline.tokens, codeline.curtoken, value, lasttoken);
			if (operr==operrOK) {
				codeline.curtoken = lasttoken;
				return true;
			}
			if (operr==operrNOTNUMBER && codeline.as->IsFirstPass()) {
				// not number: probably unresolved label, simulate success with neutral value
				value = 0;
				codeline.curtoken = lasttoken;
				return true;
			}
			// other errors do not update curtoken
			return false;
		}

	} // namespace Z80

} // namespace MUZ
