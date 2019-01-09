//
//  Z80-Instructions.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "Assembler.h"
#include "Z80-Operands.h"
#include "Z80-Instructions.h"

namespace MUZ {
	

	
	/** Assemble instruction at current token, returns false if error.
	 tok index 0   1   2   3
	 ADC A   ,   A/B/C/D/E/H/L
	 ADC A   ,   n
	 ADC A   ,   (HL)/(IX+d)/(IY+d)
	 ADC HL  ,   BC/DE/HL/SP
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionADC::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest, src;
		int d;
		// save current position
		int curtoken = codeline.curtoken;
		// ADC A, 8 bit
		if (GetReg8(codeline, dest,REGFLAGS::A)) {
			if (!GetComma(codeline)) {
				// ADC A
				codeline.AddCode(0x88 + getsubcode(regA));
				codeline.SetCycles(4);
				return true;
			}
			// ADC A,r
			if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
				codeline.AddCode(0x88 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}
			if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
				codeline.AddCode(getprefix(src), 0x88 + getsubcode(src));
				codeline.SetCycles(8);
				return true;
			}
			// ADC  A,(HL)
			if (GetIndHL(codeline)) {
				codeline.AddCode(0x8E);
				codeline.SetCycles(7);
				return true;
			}
			// ADC A,(X+d)
			if (GetIndX(codeline, src, d)) {
				codeline.AddCode(getprefix(src), 0x8E, d);
				codeline.SetCycles(19);
				return true;
			}
			// ADC A,n
			if (GetNum8(codeline, d)) {
				codeline.AddCode(0xCE, d);
				codeline.SetCycles(7);
				return true;
			}
			// TODO: ADC A,IXL IXH IYL IYH
			return false;
		}
		// ADC 16 bits
		if (GetReg16(codeline, dest,REGFLAGS::HL)) {
			if (!GetComma(codeline)) {
				return false;//TODO: syntax after ADC HL
			}
			// ADC HL, BC DE HL SP
			if (GetReg16(codeline, src,REGFLAGS::BCDESP | REGFLAGS::HL)) {
				codeline.AddCode(0xED, 0x4A + getsubcode(src));
				codeline.SetCycles(15);
				return true;
			}
			return false;
		}
		// restore initial position
		codeline.curtoken = curtoken; //TODO:: not needed?
		// ADC r
		if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x88 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0x88 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// ADC  (HL)
		if (GetIndHL(codeline)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x8E);
			codeline.SetCycles(7);
			return true;
		}
		// ADC (X+d)
		if (GetIndX(codeline, src, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(getprefix(src), 0x8E, d);
			codeline.SetCycles(19);
			return true;
		}
		// ADC n
		if (GetNum8(codeline, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0xCE, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 tok index 0   1   2   3
	 ADD A   ,   A/B/C/D/E/H/L
	 ADD A   ,   n
	 ADD A   ,   (HL)/(IX+d)/(IY+d)
	 ADD HL  ,   BC/DE/HL/SP
	 ADD IX  ,   BC/DE/IX/SP
	 ADD IY  ,   BC/DE/IY/SP
	 allowed:
	 ADD A/B/C/D/E/H/L
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionADD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest, src;
		int d;
		// save current position
		int curtoken = codeline.curtoken;
		// ADD A,8bit
		if (GetReg8(codeline, dest,REGFLAGS::A)) {
			if (!GetComma(codeline)) {
				// ADD A
				codeline.AddCode(0x87);
				codeline.SetCycles(4);
				return true;
			}
			// ADD A,r
			if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
				codeline.AddCode(0x80 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}
			// ADD r,undoc
			if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
				codeline.AddCode(getprefix(src), 0x80 + getsubcode(src));
				codeline.SetCycles(8);
				return true;
			}
			// ADD A,(HL)
			if (GetIndHL(codeline)) {
				codeline.AddCode(0x86);
				codeline.SetCycles(7);
				return true;
			}
			// ADD A,(X+d)
			if (GetIndX(codeline, src, d)) {
				codeline.AddCode(getprefix(src), 0x86, d);
				codeline.SetCycles(19);
				return true;
			}
			// ADD A,n
			if (GetNum8(codeline, d)) {
				codeline.AddCode(0xC6, d);
				codeline.SetCycles(7);
				return true;
			}
			// TODO: error after ADD A,
			return false;
		}
		// ADD 16 bits
		if (GetReg16(codeline, dest,REGFLAGS::HLXY)) {
			if (!GetComma(codeline)) {
				return false;//TODO: syntax after ADD rp
			}
			// ADD HL, BC DE HL SP
			// ADD IX, BC DE IX SP
			// ADD IY, BC DE IY SP
			if (GetReg16(codeline, src,REGFLAGS::BCDESP | REGFLAGS::HLXY)) {
				int cycles = 11;
				// HL/IX/IY as source: only accept HL,HL - IX,IX - IY,IY
				if ( RegAccept(REGFLAGS::HLXY, src)) {
					if (src != dest) return false;
					src = regHL; // mimic ADD HL opcode
				}
				// indexed prefix?
				if ( RegAccept(REGFLAGS::XY, dest)) {
					codeline.AddCode(getprefix(dest));
					cycles = 15;
				}
				// ADD HL opcode
				codeline.AddCode(0x09 + getsubcode(src));
				codeline.SetCycles(cycles);
				return true;
			}
			return false;
		}
		// all checked, maybe shortcut form ADD A / ADD B etc
		// error if there is a comma after next operand
		// restore initial position
		codeline.curtoken = curtoken;//TODO: normaly not needed?

		// ADD r
		if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x80 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		// ADD undoc
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0x80 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// ADD (HL)
		if (GetIndHL(codeline)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x86);
			codeline.SetCycles(7);
			return true;
		}
		// ADD (X+d)
		if (GetIndX(codeline, src, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(getprefix(src), 0x86, d);
			codeline.SetCycles(19);
			return true;
		}
		// ADD n
		if (GetNum8(codeline, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0xC6, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 AND A/B/C/D/E/H/L
	 AND n
	 AND (HL)/(IX+d)/(IY+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionAND::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src;
		int d;
		// save current position
		int curtoken = codeline.curtoken;
		if (GetReg8(codeline, src, REGFLAGS::A)) {
			if (GetComma(codeline)) {
				// AND A,r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0xA0 + getsubcode(src));
					codeline.SetCycles(4);
					return true;
				}
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(src), 0xA0 + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// AND A,(HL)
				if (GetIndHL(codeline)) {
					codeline.AddCode(0xA6);
					codeline.SetCycles(7);
					return true;
				}
				// AND A,(X+d)
				if (GetIndX(codeline, src, d)) {
					codeline.AddCode(getprefix(src), 0xA6, d);
					codeline.SetCycles(19);
					return true;
				}
				// AND A,n
				if (GetNum8(codeline, d)) {
					codeline.AddCode(0xE6, d);
					codeline.SetCycles(7);
					return true;
				}
				return false;
			}
			// restore initial position
			codeline.curtoken = curtoken;
		}
		// AND r
		if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xA0 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0xA0 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// AND (HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xA6);
			codeline.SetCycles(7);
			return true;
		}
		// AND (X+d)
		if (GetIndX(codeline, src, d)) {
			codeline.AddCode(getprefix(src), 0xA6, d);
			codeline.SetCycles(19);
			return true;
		}
		// AND n
		if (GetNum8(codeline, d)) {
			codeline.AddCode(0xE6, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 BIT b,r
	 b = 0 to 7
	 r = A B C D E H L (HL) (IX+d) (IY+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionBIT::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		int d=0;
		OperandType src, bit;
		if (!GetBitNumber(codeline, bit)) return false;
		if (!GetComma(codeline)) return false;
		if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
			codeline.SetCycles(8);
		} else if (GetIndHL(codeline)) {
			codeline.SetCycles(12);
			src = indHL;
		} else if (GetIndX(codeline, src, d)) {
			codeline.SetCycles(20);
		} else return false;
		// (IX+d) and (IY+d) are prefixed then use (HL) encoding
		if (RegAccept(REGFLAGS::XY, src)) {
			codeline.AddCode(getprefix(src), 0xCB, d, 0x40 + getsubcode(bit) + getsubcode(indHL));
		} else {
			codeline.AddCode(0xCB, 0x40 + getsubcode(bit) + getsubcode(src));
		}
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 
	 CALL cond, num16
	 CALL num16
	 
	 num16 stored low byte first
	 
	 condNZ, condZ, condNC, condC, condPO, condPE, condP, condM,
	 c4      cc     d4      dc     e4      ec      f4     fc
	 
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCALL::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType cond;
		int addr;
		if (GetCond(codeline, cond)) {
			if (GetComma(codeline)) {
				if (GetNum16(codeline, addr)) {
					codeline.AddCode(0xC4 + getsubcode(cond), (addr & 0xFF), (addr >> 8));
					codeline.SetCycles(10,17);
					return true;
				}
			}
			return false;
		}
		if (GetNum16(codeline, addr)) {
			codeline.AddCode(0xCD, (addr & 0xFF), (addr >> 8));
			codeline.SetCycles(17);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 CCF
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCCF::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x3F);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 CP A/B/C/D/E/H/L
	 CP n
	 CP (HL)/(IX+d)/(IY+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCP::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src;
		int d ;
		if (GetReg8(codeline, src,REGFLAGS::A)) {
			if (!GetComma(codeline)) {
				// CP A
				codeline.AddCode(0xB8 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}

			// CP A,r
			if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
				codeline.AddCode(0xB8 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}
			if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
				codeline.AddCode(getprefix(src), 0xB8 + getsubcode(src));
				codeline.SetCycles(8);
				return true;
			}
			// CP A,(HL)
			if (GetIndHL(codeline)) {
				codeline.AddCode(0xBE);
				codeline.SetCycles(7);
				return true;
			}
			// CP A,(X+d)
			if (GetIndX(codeline, src, d)) {
				codeline.AddCode(getprefix(src), 0xBE, d);
				codeline.SetCycles(19);
				return true;
			}
			// CP A,n
			if (GetNum8(codeline, d)) {
				codeline.AddCode(0xFE, d);
				codeline.SetCycles(7);
				return true;
			}
			return false;
		}
		// CP r
		if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xB8 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0xB8 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// CP (HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xBE);
			codeline.SetCycles(7);
			return true;
		}
		// CP (X+d)
		if (GetIndX(codeline, src, d)) {
			codeline.AddCode(getprefix(src), 0xBE, d);
			codeline.SetCycles(19);
			return true;
		}
		// CP n
		if (GetNum8(codeline, d)) {
			codeline.AddCode(0xFE, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA9);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPDR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB9);
		codeline.SetCycles(16,21);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA1);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPIR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB1);
		codeline.SetCycles(16,21); // 5 if repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPL::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x2F);
		codeline.SetCycles(4); // 5 if repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDAA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x27);
		codeline.SetCycles(4); // 5 if repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 DEC B  05     5 + 8 * 0
	 DEC C  0D     5 + 8 * 1
	 DEC D  15     5 + 8 * 2
	 DEC E  1D     5 + 8 * 3
	 DEC H  25     5 + 8 * 4
	 DEC L  2D     5 + 8 * 5
	 DEC (HL) 35   5 + 8 * 6
	 DEC A  3D     5 + 8 * 7
	 DEC (IX+0xAA) dd 35 d
	 DEC (IY+0xAA) fd 35 d
	 DEC BC        0B
	 DEC DE        1B
	 DEC HL        2B
	 DEC SP        3B
	 DEC IX        dd 2B
	 DEC IY        fd 2B
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDEC::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest;
		int d;
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0x05 + 8 * getsubcode(dest));
			codeline.SetCycles(4);
			return true;
		}
		if (GetReg8(codeline, dest, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(dest), 0x05 + 8 * getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		if (GetReg16(codeline, dest)) {
			if (RegAccept(REGFLAGS::BCDEHLSP, dest)) {
				codeline.AddCode(0x0B + getsubcode(dest));
				codeline.SetCycles(6);
				return true;
			}
			if (RegAccept(REGFLAGS::XY, dest)) {
				codeline.AddCode(getprefix(dest), 0x0B + getsubcode(regHL));
				codeline.SetCycles(10);
				return true;
			}
		}
		if (GetIndHL(codeline)) {
			codeline.AddCode(0x35);
			codeline.SetCycles(11);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			codeline.AddCode(getprefix(dest), 0x35, d);
			codeline.SetCycles(23);
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xF3);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDJNZ::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		int d;
		if (GetNum16(codeline, d)) {
			int depl = codeline.as->GetAddress() + 2 - d;
			if ( depl < -126 || depl > +129) {
				// TODO: issue warning for target too far
			}
			depl = 0x100 - depl;
			codeline.AddCode(0x10, depl);
			codeline.SetCycles(8,13); // 3 when looping, 2 when finished
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xFB);
		codeline.SetCycles(4);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 EX AF,AF'  08
	 EX DE,HL   EB
	 EX (SP),HL E3
	 EX (SP),IX dd E3
	 EX (SP),IY fd E3
	 
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEX::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest, src;
		if (GetReg16(codeline, dest,REGFLAGS::AF | REGFLAGS::DE)) {
			if (GetComma(codeline)) {
				if (GetReg16(codeline, src, REGFLAGS::AFp | REGFLAGS::HL)) {
					if (src == regAFp && dest == regAF) {
						codeline.AddCode(0x08);
						codeline.SetCycles(4);
						return true;
					}
					if (src == regHL && dest == regDE) {
						codeline.AddCode(0xEB);
						codeline.SetCycles(4);
						return true;
					}
				}
			}
			return false;
		}
		if (GetIndSP(codeline)) {
			if (GetComma(codeline)) {
				if (GetReg16(codeline, src, REGFLAGS::HLXY)) {
					if (src == regHL) {
						codeline.AddCode(0xE3);
						codeline.SetCycles(19);
						return true;
					}
					// EX (SP),IX/IY
					codeline.AddCode(getprefix(src), 0xE3);
					codeline.SetCycles(23);
					return true;
				}
			}
		}
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEXX::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xD9);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionHALT::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x76);
		codeline.SetCycles(4);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 IM 0 ED46
	 IM 1 ED56
	 IM 2 ED5E

	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionIM::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		int m;
		if (GetNum8(codeline, m)) {
			if (m >= 0 && m <= 2) {
				const int code[3] = { 0x46, 0x56, 0x5E };
				codeline.AddCode(0xED, code[m]);
				codeline.SetCycles(8);
				return true;
			}
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 IN A,(num8)
	 IN r,(C)  ->  A B C D E F H L
	 
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionIN::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest;
		int n;
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL|REGFLAGS::F)) {
			if (GetComma(codeline)) {
				if (GetIndC(codeline)) {
					codeline.AddCode(0xED, 0x40 + getsubcode(dest) * 8);
					codeline.SetCycles(12);
					return true;
				}
				if (dest == regA && GetInd16(codeline, n)) {
					codeline.AddCode(0xDB, n);
					codeline.SetCycles(11);
					return true;
				}
			}
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 INC B  05     4 + 8 * 0
	 INC C  0D     4 + 8 * 1
	 INC D  15     4 + 8 * 2
	 INC E  1D     4 + 8 * 3
	 INC H  25     4 + 8 * 4
	 INC L  2D     4 + 8 * 5
	 INC (HL) 35   4 + 8 * 6
	 INC A  3D     4 + 8 * 7
	 INC (IX+0xAA) dd 34 d
	 INC (IY+0xAA) fd 34 d
	 INC BC        03
	 INC DE        13
	 INC HL        23
	 INC SP        33
	 INC IX        dd 23
	 INC IY        fd 23
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINC::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest;
		int d;
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0x04 + 8 * getsubcode(dest));
			codeline.SetCycles(4);
			return true;
		}
		if (GetReg8(codeline, dest, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(dest), 0x04 + 8 * getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		if (GetReg16(codeline, dest,REGFLAGS::BCDEHLSP | REGFLAGS::XY)) {
			if (RegAccept(REGFLAGS::XY, dest)) {
				codeline.AddCode(getprefix(dest), 0x03 + getsubcode(regHL));
				codeline.SetCycles(10);
				return true;
			}
			codeline.AddCode(0x03 + getsubcode(dest));
			codeline.SetCycles(6);
			return true;
		}
		if (GetIndHL(codeline)) {
			codeline.AddCode(0x34);
			codeline.SetCycles(11);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			codeline.AddCode(getprefix(dest), 0x34, d);
			codeline.SetCycles(23);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionIND::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xAA);
		codeline.SetCycles(16);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINDR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xBA);
		codeline.SetCycles(16,21); // 5 when repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA2);
		codeline.SetCycles(16); // 5 when repeat, 4 when finished
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINIR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB2);
		codeline.SetCycles(21); // 5 when repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 JP cond, num16
	 JP num16
	 JP (HL) (IX) (IY)
	 
	 num16 is stored low byte first
	 
	 condNZ, condZ, condNC, condC, condPO, condPE, condP, condM,
	 c2      ca     d2      da     e2      ea      f2     fa
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionJP::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType cond, reg;
		int addr;
		if (GetCond(codeline, cond)) {
			if (GetComma(codeline)) {
				if (GetNum16(codeline, addr)) {
					codeline.AddCode(0xC2 + getsubcode(cond), (addr & 0xFF), (addr >> 8));
					codeline.SetCycles(10);
					return true;
				}
			}
			return false;
		}
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xE9);
			codeline.SetCycles(4);
			return true;
		}
		if (GetIndX(codeline, reg, addr)) {
			codeline.AddCode(getprefix(reg), 0xE9);
			codeline.SetCycles(8);
			return true;
		}
		// JP (IX) or (IY)
		if (codeline.tokens[codeline.curtoken].type == tokenTypePAROPEN) {
			codeline.curtoken += 1;
			if (GetReg16(codeline, reg, REGFLAGS::XY)) {
				if (codeline.curtoken < codeline.tokens.size()) {
					if (codeline.tokens[codeline.curtoken].type == tokenTypePARCLOSE) {
						codeline.AddCode(getprefix(reg), 0xE9);
						codeline.SetCycles(8);
						return true;
					}
				}
			}
		}
		if (GetNum16(codeline, addr)) {
			codeline.AddCode(0xC3, (addr & 0xFF), (addr >> 8));
			codeline.SetCycles(10);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 
	 JR cond,d
	 JR d
	 
	 d: 2 complement of $ + 2 - dest address
	 
	 condNZ, condZ, condNC, condC,
	 20      28     30      38
	 
	 invalid: condPO, condPE, condP, condM

	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionJR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType cond;
		int d;
		if (GetCond(codeline, cond)) {
			if (cond == condZ || cond == condNZ || cond == condC || cond == condNC) {
				if (GetComma(codeline)) {
					if (GetNum16(codeline, d)) {
						int depl = codeline.as->GetAddress() + 2 - d;
						if ( depl < -126 || depl > +129) {
							// TODO: issue warning for target too far
						}
						depl = 0x100 - depl;
						codeline.AddCode(0x20 + getsubcode(cond), depl);
						codeline.SetCycles(7,12);
						return true;
					}
				}
			}
		}
		if (GetNum16(codeline, d)) {
			int depl = codeline.as->GetAddress() + 2 - d;
			depl = 0x100 - depl; // 2-complement
			if (depl < -126 || depl > +129) {
				// TODO: issue warning for target too far
			}
			codeline.AddCode(0x18, depl);
			codeline.SetCycles(12);
			return true;
		}
		return false;
	}

	/** Assemble LD instruction at current token, returns false if error
	 
	 LD rd,rs           B C D E H L (HL) A    $40 + rd << 3 + rs
	                    A,I                   $ED $57
	                    A,R                   $ED $5F
	                    I,A                   $ED $47
						R,A                   $ED $4F

	 LD rd,(ix)         (HL) (IX+d) (IY+d)	  ($DD,$FD) $46 + rd << 3
	 LD (ix),rs         (HL) (IX+d) (IY+d)	  ($DD,$FD) $70 + rs
	 
	 LD rd,num8         B C D E H L (HL) A    $70 + rd << 3
	 
	 LD A,(num8)                              $3E n
	 
	 LD A,(BC)                                $0A
	 LD A,(DE)                                $1A
	 LD (BC),A                                $02
	 LD (DE),A                                $12
	 
	 LD rp,num16
	 LD IX,num16
	 LD IY,num16
	 
	 LD rp,(addr)       BC DE HL SP
	 LD IX,(addr)
	 LD IY,(addr)
	 LD HL,(addr)
	 
	 LD (HL),n                                $36 n
	 
	 LD (addr),A
	 LD (addr),BC
	 LD (addr),DE
	 LD (addr),HL
	 LD (addr),IX
	 LD (addr),IY
	 LD (addr),SP

	 LD SP,HL
	 LD SP,IX
	 LD SP,IY
	 
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src, dest;
		int d;
		// LD I/R,A
		if (GetReg8(codeline, dest, REGFLAGS::I | REGFLAGS::R)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src, REGFLAGS::A)) {
					codeline.AddCode(0xED, 0x40 + getsubcode(src));
					codeline.SetCycles(9);
					return true;
				}
			}
			return false;
		}
		// LD SP,(nn) HL IX IY nn
		if (GetReg16(codeline, dest, REGFLAGS::SP)) {
			if (GetComma(codeline)) {
				// LD SP, HL IX IY
				if (GetReg16(codeline, src, REGFLAGS::HLXY)) {
					int cycles = 6;
					if (src != regHL) {
						codeline.AddCode(getprefix(src));
						cycles = 10;
					}
					codeline.AddCode(0xF9);
					codeline.SetCycles(cycles);
					return true;
				}
				// LD SP,(nn)
				if (GetInd16(codeline, d)) {
					codeline.AddCode(0xED, 0x7B, d & 0xFF, d >> 8);
					codeline.SetCycles(20);
					return true;
				}
				// LD SP,nn
				if (GetNum16(codeline, d)) {
					codeline.AddCode(0x31, d & 0xFF, d >> 8);
					codeline.SetCycles(10);
					return true;
				}
			}
			return false;
		}
		// LD BC DE HL IX IY, (nn) nn
		if (GetReg16(codeline, dest, REGFLAGS::BC | REGFLAGS::DE | REGFLAGS::HLXY)) {
			if (GetComma(codeline)) {
				if (GetInd16(codeline, d)) {
					// LD BC,(nn)  ED 4B nn nn
					// LD DE,(nn)  ED 5B nn nn 
					// LD HL,(nn)  2A nn nn
					// LD IX,(nn)  DD 2A nn nn
					// LD IY,(nn)  FD 2A nn nn
					if (dest == regBC) {
						codeline.AddCode(0xED, 0x4B, d & 0xFF, d >> 8);
						codeline.SetCycles(20);
						return true;
					} else if (dest == regDE) {
						codeline.AddCode(0xED, 0x5B, d & 0xFF, d >> 8);
						codeline.SetCycles(20);
						return true;
					}
					int cycles  = 16;
					if (dest != regHL) {
						codeline.AddCode(getprefix(dest));
						cycles = 20;
					}
					codeline.AddCode(0x2A, d & 0xFF, d >> 8);
					codeline.SetCycles(cycles);
					return true;
				}
				if (GetNum16(codeline, d)) {
					// LD BC,nn  01 nn nn
					// LD DE,nn  11 nn nn
					// LD HL,nn  21 nn nn
					// LD IX,nn  DD 21 nn nn
					// LD IY,nn  FD 21 nn nn
					if (dest == regBC) {
						codeline.AddCode(0x01, d & 0xFF, d >> 8);
						codeline.SetCycles(10);
						return true;
					} else if (dest == regDE) {
						codeline.AddCode(0x11, d & 0xFF, d >> 8);
						codeline.SetCycles(10);
						return true;
					}
					int cycles  = 10;
					if (dest != regHL) {
						codeline.AddCode(getprefix(dest));
						cycles = 14;
					}
					codeline.AddCode(0x21, d & 0xFF, d >> 8);
					codeline.SetCycles(cycles);
					return true;
				}
			}
			return false;
		}
		// LD (HL) , n r
		if (GetIndHL(codeline)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0x70 + getsubcode(src));
					codeline.SetCycles(7);
					return true;
				}
				if (GetNum8(codeline, d)) {
					codeline.AddCode(0x36, d);
					codeline.SetCycles(10);
					return true;
				}
			}
			return false;
		}
		// LD (IX+d) (IY+d), n r
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(getprefix(dest), 0x70 + getsubcode(src), d);
					codeline.SetCycles(7);
					return true;
				}
				if (GetNum8(codeline, d)) {
					codeline.AddCode(getprefix(dest), 0x36, d);
					codeline.SetCycles(10);
					return true;
				}
			}
			return false;
		}
		// LD (BC),A
		if (GetIndBC(codeline)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src, REGFLAGS::A)) {
					codeline.AddCode(0x02);
					codeline.SetCycles(7);
					return true;
				}
			}
			return false;
		}
		// LD (DE),A
		if (GetIndDE(codeline)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src, REGFLAGS::A)) {
					codeline.AddCode(0x12);
					codeline.SetCycles(7);
					return true;
				}
			}
			return false;
		}
		// LD (nn), A BC DE HL IX IY SP
		if (GetInd16(codeline, d)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src, REGFLAGS::A)) {
					codeline.AddCode(0x32, d & 0xFF, d >> 8);
					codeline.SetCycles(13);
					return true;
				}
				if (GetReg16(codeline, src, REGFLAGS::BCDEHLSP | REGFLAGS::XY)) {
					if (src == regBC) {
						codeline.AddCode(0xED, 0x43, d & 0xFF, d >> 8);
						codeline.SetCycles(20);
						return true;
					}
					if (src == regDE) {
						codeline.AddCode(0xED, 0x53, d & 0xFF, d >> 8);
						codeline.SetCycles(20);
						return true;
					}
					if (src == regSP) {
						codeline.AddCode(0xED, 0x73, d & 0xFF, d >> 8);
						codeline.SetCycles(20);
						return true;
					}
					// HL, IX, IY
					int cycles = 16;
					if (src == regIX || src == regIY) {
						codeline.AddCode(getprefix(src));
						cycles = 20;
					}
					codeline.AddCode(0x22, d & 0xFF, d >> 8);
					codeline.SetCycles(cycles);
					return true;
				}
			}
			return false;
		}
		// LD A, (BC) (DE) (HL) (IX+d) (IY+d) n A B C D E H L I R
		if (GetReg8(codeline, dest, REGFLAGS::A)) {
			if (GetComma(codeline)) {
				// LD A,I R
				if (GetReg8(codeline, src, REGFLAGS::I | REGFLAGS::R)) {
					codeline.AddCode(0xED, 0x50 + getsubcode(src));
					codeline.SetCycles(9);
					return true;
				}
				// LD A, r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0x78 + getsubcode(src));
					codeline.SetCycles(4);
					return true;
				}
				// LD A,undoc
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(src), 0x78 + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// LD A,(HL)
				if (GetIndHL(codeline)) {
					codeline.AddCode(0x7E);
					codeline.SetCycles(7);
					return true;
				}
				// LD A,(BC)
				if (GetIndBC(codeline)) {
					codeline.AddCode(0x0A);
					codeline.SetCycles(7);
					return true;
				}
				// LD A,(DE)
				if (GetIndDE(codeline)) {
					codeline.AddCode(0x1A);
					codeline.SetCycles(7);
					return true;
				}
				// LD A,(X+d)
				if (GetIndX(codeline, src, d)) {
					codeline.AddCode(getprefix(src), 0x7E, d);
					codeline.SetCycles(19);
					return true;
				}
				// LD A,(nn)
				if (GetInd16(codeline, d)) {
					codeline.AddCode(0x3A, d & 0xFF, d >> 8);
					codeline.SetCycles(13);
					return true;
				}
				// LD A,n
				if (GetNum8(codeline, d)) {
					codeline.AddCode(0x3E, d);
					codeline.SetCycles(7);
					return true;
				}
			}
			return false;
		}
		// LD r,r (HL) (X+d) n
		if (GetReg8(codeline, dest, REGFLAGS::ABCDEHL)) {
			if (GetComma(codeline)) {
				// LD r,r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0x40 + 8 * getsubcode(dest) + getsubcode(src));
					codeline.SetCycles(4);
					return true;
				}
				// LD r,undoc
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(src), 0x40 + 8 * getsubcode(dest) + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// LD r,(HL)
				if (GetIndHL(codeline)) {
					codeline.AddCode(0x46 + 8 * getsubcode(dest));
					codeline.SetCycles(7);
					return true;
				}
				// LD r,(x+d)
				if (GetIndX(codeline, src, d)) {
					codeline.AddCode(getprefix(src), 0x46 + 8 * getsubcode(dest), d );
					codeline.SetCycles(19);
					return true;
				}
				// LD r,n
				if (GetNum8(codeline, d)) {
					codeline.AddCode(0x06 + 8 * getsubcode(dest), d);
					codeline.SetCycles(7);
					return true;
				}
			}
			return false;
		}
		// LD undoc,r n
		if (GetReg8(codeline, dest, REGFLAGS::UNDOC)) {
			if (GetComma(codeline)) {
				// LD undoc,r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(getprefix(dest), 0x40 + 8 * getsubcode(dest) + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// LD undoc,undoc
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(dest), 0x40 + 8 * getsubcode(dest) + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// LD undoc,n
				if (GetNum8(codeline, d)) {
					codeline.AddCode(getprefix(dest), 0x06 + 8 * getsubcode(dest), d);
					codeline.SetCycles(11);
					return true;
				}
			}
			return false;
		}

		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLDD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA8);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLDDR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB8);
		codeline.SetCycles(16,21);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLDI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA0);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLDIR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB0);
		codeline.SetCycles(16,21);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionNEG::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0x44);
		codeline.SetCycles(8);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionNOP::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x00);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 AND A/B/C/D/E/H/L
	 AND n
	 AND (HL)/(IX+d)/(IY+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionOR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src;
		int d;
		// save current position
		int curtoken = codeline.curtoken;
		if (GetReg8(codeline, src, REGFLAGS::A)) {
			if (GetComma(codeline)) {
				// OR A,r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0xB0 + getsubcode(src));
					codeline.SetCycles(4);
					return true;
				}
				// OR A,undoc
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(src), 0xB0 + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// OR A,(HL)
				if (GetIndHL(codeline)) {
					codeline.AddCode(0xB6);
					codeline.SetCycles(7);
					return true;
				}
				// OR A,(X+d)
				if (GetIndX(codeline, src, d)) {
					codeline.AddCode(getprefix(src), 0xB6, d);
					codeline.SetCycles(19);
					return true;
				}
				// OR A,n
				if (GetNum8(codeline, d)) {
					codeline.AddCode(0xF6, d);
					codeline.SetCycles(7);
					return true;
				}
				return false;
			}
			// restore initial position
			codeline.curtoken = curtoken;
		}
		// OR r
		if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xB0 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		// OR undoc
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0xB0 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// OR (HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xB6);
			codeline.SetCycles(7);
			return true;
		}
		// OR (X+d)
		if (GetIndX(codeline, src, d)) {
			codeline.AddCode(getprefix(src), 0xB6, d);
			codeline.SetCycles(19);
			return true;
		}
		// OR n
		if (GetNum8(codeline, d)) {
			codeline.AddCode(0xF6, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionOTDR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xBB);
		codeline.SetCycles(16,21);
		return true;
	}
	

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionOTIR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB3);
		codeline.SetCycles(16,21);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 OUT (num8),A
	 OUT (C),r  ->  A B C D E   H L
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionOUT::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src;
		int n;
		if (GetIndC(codeline)) {
			if (GetComma(codeline)) {
				// OUT (C),r
				if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0xED, 0x41 + getsubcode(src) * 8);
					codeline.SetCycles(12);
					return true;
				}
				// OUT (C),0 (undoc)
				if (GetNum8(codeline, n)) {
					if (n == 0) {
						codeline.AddCode(0xED, 0x71);
						codeline.SetCycles(12);
					}
				}
			}
			return false;
		}
		if (GetInd16(codeline, n)) {
			if (GetComma(codeline)) {
				if (GetReg8(codeline, src,REGFLAGS::A)) {
					codeline.AddCode(0xD3, n);
					codeline.SetCycles(11);
					return true;
				}
			}
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionOUTD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xAB);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionOUTI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA3);
		codeline.SetCycles(16);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionPOP::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest;
		if (GetReg16(codeline, dest,REGFLAGS::AF|REGFLAGS::BC|REGFLAGS::DE|REGFLAGS::HLXY)) {
			codeline.SetCycles(10);
			if (dest == regIX || dest == regIY) {
				codeline.AddCode(getprefix(dest));
				dest = regHL;
				codeline.SetCycles(14);
			}
			codeline.AddCode(0xC1 + getsubcode(dest));
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionPUSH::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest;
		if (GetReg16(codeline, dest,REGFLAGS::AF|REGFLAGS::BC|REGFLAGS::DE|REGFLAGS::HLXY)) {
			codeline.SetCycles(11);
			if (dest == regIX || dest == regIY) {
				codeline.AddCode(getprefix(dest));
				dest = regHL;
				codeline.SetCycles(15);
			}
			codeline.AddCode(0xC5 + getsubcode(dest));
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRES::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		int d=0;
		OperandType src, bit, reg;
		if (!GetBitNumber(codeline, bit)) return false;
		if (!GetComma(codeline)) return false;
		if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
			codeline.SetCycles(8);
		} else if (GetIndHL(codeline)) {
			codeline.SetCycles(15);
			src = indHL;
		} else if (GetIndX(codeline, src, d)) {
			codeline.SetCycles(23);
			if (GetComma(codeline)) {
				// RES bit,(X+d),reg  (undoc)
				if (GetReg8(codeline, reg, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(getprefix(src), 0xCB, d, 0x80 + getsubcode(bit) + getsubcode(reg));
					return true;
				}
			}
		} else return false;
		// (IX+d) and (IY+d) are prefixed then use (HL) encoding
		if (RegAccept(REGFLAGS::XY, src)) {
			codeline.AddCode(getprefix(src), 0xCB, d, 0x80 + getsubcode(bit) + getsubcode(indHL));
		} else {
			codeline.AddCode(0xCB, 0x80 + getsubcode(bit) + getsubcode(src));
		}
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRET::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType cond;
		if (GetCond(codeline, cond)) {
			codeline.AddCode(0xC0 + getsubcode(cond));
			codeline.SetCycles(5,11);
			return true;
		}
		codeline.AddCode(0xC9);
		codeline.SetCycles(10);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRETI::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0x4D);
		codeline.SetCycles(14);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRETN::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0x45);
		codeline.SetCycles(14);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 RL r
	 RL (HL) (X+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRL::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src,dest;
		int d;
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x16);
			codeline.SetCycles(15);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// RL (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x10 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x16);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x10 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRLA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x17);
		codeline.SetCycles(4);
		return true;
	}
	/** Assemble instruction at current token, returns false if error
	 RLC r
	 RLC (HL) (X+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRLC::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src,dest;
		int d;
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x06);
			codeline.SetCycles(15);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// RLC (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x00 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x06);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x00 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRLCA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x07);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRLD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0x6F);
		codeline.SetCycles(18);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src,dest;
		int d;
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x1E);
			codeline.SetCycles(15);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// RR (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x18 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x1E);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x18 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRRA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x1F);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRRC::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src, dest;
		int d;
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x0E);
			codeline.SetCycles(15);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// RRC (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x08 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x0E);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x08 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRRCA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x0F);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRRD::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0x67);
		codeline.SetCycles(18);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionRST::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		int addr;
		if (GetNum8(codeline, addr)) {
			if (addr <= 0x38 && addr % 8 == 0) {
				codeline.AddCode(0xC7 + addr);
				codeline.SetCycles(11);
				return true;
			}
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 SBC A   ,   A/B/C/D/E/H/L
	 SBC A   ,   n
	 SBC A   ,   (HL)/(IX+d)/(IY+d)
	 SBC HL  ,   BC/DE/HL/SP
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSBC::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType dest, src;
		int d;
		// save current position
		int curtoken = codeline.curtoken;
		// SBC A, 8 bit
		if (GetReg8(codeline, dest,REGFLAGS::A)) {
			if (!GetComma(codeline)) {
				// SBC A
				codeline.AddCode(0x98 + getsubcode(regA));
				codeline.SetCycles(4);
				return true;
			}
			// SBC A,r
			if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
				codeline.AddCode(0x98 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}
			// SBC A,undoc
			if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
				codeline.AddCode(getprefix(src), 0x98 + getsubcode(src));
				codeline.SetCycles(8);
				return true;
			}
			// SBC A,(HL)
			if (GetIndHL(codeline)) {
				codeline.AddCode(0x9E);
				codeline.SetCycles(7);
				return true;
			}
			// SBC A,(X+d)
			if (GetIndX(codeline, src, d)) {
				codeline.AddCode(getprefix(src), 0x9E, d);
				codeline.SetCycles(19);
				return true;
			}
			// SBC A,n
			if (GetNum8(codeline, d)) {
				codeline.AddCode(0xDE, d);
				codeline.SetCycles(7);
				return true;
			}
			// TODO: ADD A,IXL IXH IYL IYH
			return false;
		}
		// SBC 16 bits
		if (GetReg16(codeline, dest,REGFLAGS::HL)) {
			if (!GetComma(codeline)) {
				return false;//TODO: syntax after SBC HL
			}
			// SBC HL, BC DE HL SP
			if (GetReg16(codeline, src,REGFLAGS::BCDESP | REGFLAGS::HL)) {
				codeline.AddCode(0xED, 0x42 + getsubcode(src));
				codeline.SetCycles(15);
				return true;
			}
			return false;
		}
		// restore initial position
		codeline.curtoken = curtoken;//TODO: not needed?
		// SBC r
		if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x98 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		// SBC undoc
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0x98 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// SBC (HL)
		if (GetIndHL(codeline)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x9E);
			codeline.SetCycles(7);
			return true;
		}
		// SBC (X+d)
		if (GetIndX(codeline, src, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(getprefix(src), 0x9E, d);
			codeline.SetCycles(19);
			return true;
		}
		// SBC n
		if (GetNum8(codeline, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0xDE, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSCF::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		codeline.AddCode(0x37);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 SET b,r
	 b = 0 to 7
	 r = A B C D E H L (HL) (IX+d) (IY+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSET::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		int d=0;
		OperandType src, bit, reg;
		if (!GetBitNumber(codeline, bit)) return false;
		if (!GetComma(codeline)) return false;
		if (GetReg8(codeline, src,REGFLAGS::ABCDEHL)) {
			codeline.SetCycles(8);
		} else if (GetIndHL(codeline)) {
			codeline.SetCycles(15);
			src = indHL;
		} else if (GetIndX(codeline, src, d)) {
			codeline.SetCycles(23);
			if (GetComma(codeline)) {
				// SET bit,(X+d),reg  (undoc)
				if (GetReg8(codeline, reg, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(getprefix(src), 0xCB, d, 0xC0 + getsubcode(bit) + getsubcode(reg));
					return true;
				}
			}
		} else return false;
		// (IX+d) and (IY+d) are prefixed then use (HL) encoding
		if (RegAccept(REGFLAGS::XY, src)) {
			codeline.AddCode(getprefix(src), 0xCB, d, 0xC0 + getsubcode(bit) + getsubcode(indHL));
		} else {
			codeline.AddCode(0xCB, 0xC0 + getsubcode(bit) + getsubcode(src));
		}
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 SLA r
	 SLA (HL) (X+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSLA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src, dest;
		int d;
		// SLA (HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x26);
			codeline.SetCycles(15);
			return true;
		}
		// SLA (X+d)
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// SLA (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x20 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x26);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x20 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 SLL r
	 SLL (HL) (X+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSLL::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src, dest;
		int d;
		// SLL(HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x36);
			codeline.SetCycles(15);
			return true;
		}
		// SLL (X+d)
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// SLL (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x30 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x36);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x30 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	/** Assemble instruction at current token, returns false if error
	 SRA r
	 SRA (HL) (X+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSRA::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src,dest;
		int d;
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x2E);
			codeline.SetCycles(15);
			return true;
		}
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// SRA (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x28 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x2E);
			codeline.SetCycles(23);
			return true;
		}
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x28 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 SRL r
	 SRL (HL) (X+d)
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSRL::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src,dest;
		int d;
		// SRL (HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xCB, 0x3E);
			codeline.SetCycles(15);
			return true;
		}
		// SRL (X+d)
		if (GetIndX(codeline, dest, d)) {
			if (GetComma(codeline)) {
				// SRL (X+d),r (undoc)
				if (GetReg8(codeline, src)) {
					codeline.AddCode(getprefix(dest), 0xCB, d, 0x38 + getsubcode(src));
					codeline.SetCycles(23);
					return true;
				}
			}
			codeline.AddCode(getprefix(dest), 0xCB, d, 0x3E);
			codeline.SetCycles(23);
			return true;
		}
		// SRL r
		if (GetReg8(codeline, dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xCB, 0x38 + getsubcode(dest));
			codeline.SetCycles(8);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionSUB::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src;
		int d;
		// SUB (HL)
		if (GetIndHL(codeline)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0x96);
			codeline.SetCycles(7);
			return true;
		}
		// SUB (X+d)
		if (GetIndX(codeline, src, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(getprefix(src), 0x96, d);
			codeline.SetCycles(19);
			return true;
		}
		// save current position
		int curtoken = codeline.curtoken;
		// SUB A, ...
		if (GetReg8(codeline, src,REGFLAGS::A)) {
			if (GetComma(codeline)) {
				// SUB A,r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0x90 + getsubcode(src));
					codeline.SetCycles(4);
					return true;
				}
				// SUB A,undoc
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(src), 0x90 + getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// SUB A,(HL)
				if (GetIndHL(codeline)) {
					codeline.AddCode(0x96);
					codeline.SetCycles(7);
					return true;
				}
				// SUB A, (X+d)
				if (GetIndX(codeline, src, d)) {
					codeline.AddCode(getprefix(src), 0x96, d);
					codeline.SetCycles(19);
					return true;
				}
				// SUB A, n
				if (GetNum8(codeline, d)) {
					if (GetComma(codeline)) return false;//TODO: return an error
					codeline.AddCode(0xD6, d);
					codeline.SetCycles(7);
					return true;
				}
				return false;
			}
			// restore initial position
			codeline.curtoken = curtoken;
		}
		// SUB r
		if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0x90 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		// SUB undoc
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0x90 + getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// SUB n
		if (GetNum8(codeline, d)) {
			if (GetComma(codeline)) return false;//TODO: return an error
			codeline.AddCode(0xD6, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}
	

	/** Assemble instruction at current token, returns false if error
	 @param codeline the code line in which assembled codes will be stored
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionXOR::Assemble(CodeLine& codeline, ErrorList& msg)
	{
		OperandType src;
		int d;
		// save current position
		int curtoken = codeline.curtoken;
		if (GetReg8(codeline, src,REGFLAGS::A)) {
			if (GetComma(codeline)) {
				// XOR A,r
				if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
					codeline.AddCode(0xA8 + getsubcode(src));
					codeline.SetCycles(4);
					return true;
				}
				// XOR A,undoc
				if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
					codeline.AddCode(getprefix(src), 0xA8+ getsubcode(src));
					codeline.SetCycles(8);
					return true;
				}
				// XOR A,(HL)
				if (GetIndHL(codeline)) {
					codeline.AddCode(0xAE);
					codeline.SetCycles(7);
					return true;
				}
				// XOR A,(X+d)
				if (GetIndX(codeline, src, d)) {
					codeline.AddCode(getprefix(src), 0xAE, d);
					codeline.SetCycles(19);
					return true;
				}
				// XOR A,n
				if (GetNum8(codeline, d)) {
					codeline.AddCode(0xEE, d);
					codeline.SetCycles(7);
					return true;
				}
				return false;
			}
			// restore initial position
			codeline.curtoken = curtoken;
		}
		// XOR r
		if (GetReg8(codeline, src, REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xA8 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		// XOR undoc
		if (GetReg8(codeline, src, REGFLAGS::UNDOC)) {
			codeline.AddCode(getprefix(src), 0xA8+ getsubcode(src));
			codeline.SetCycles(8);
			return true;
		}
		// XOR A,(HL)
		if (GetIndHL(codeline)) {
			codeline.AddCode(0xAE);
			codeline.SetCycles(7);
			return true;
		}
		// XOR A,(X+d)
		if (GetIndX(codeline, src, d)) {
			codeline.AddCode(getprefix(src), 0xAE, d);
			codeline.SetCycles(19);
			return true;
		}
		// XOR A,n
		if (GetNum8(codeline, d)) {
			codeline.AddCode(0xEE, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}
	
} // namespace MUZ

