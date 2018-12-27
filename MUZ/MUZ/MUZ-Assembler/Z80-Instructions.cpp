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
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionADC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest, src;
		int d;
		// ADC A, 8 bit
		if (codeline.GetNextReg8(dest,REGFLAGS::A)) {
			if (!codeline.GetNextComma()) {
				//TODO: syntax error after ADC A
				return false;
			}
			// ADC A,r
			if (codeline.GetNextReg8(src, REGFLAGS::ABCDEHL)) {
				codeline.AddCode(0x88 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}
			// ADC  A,(HL)
			if (codeline.GetNextIndHL(src)) {
				codeline.AddCode(0x8E);
				codeline.SetCycles(7);
				return true;
			}
			// ADC A,(X+d)
			if (codeline.GetNextIndX(src, d)) {
				codeline.AddCode(getsubcode(src), 0x8E, d);
				codeline.SetCycles(19);
				return true;
			}
			// ADC A,n
			if (codeline.GetNextNum8(src, d)) {
				codeline.AddCode(0xCE, d);
				codeline.SetCycles(7);
				return true;
			}
			// TODO: ADC A,IXL IXH IYL IYH
			return false;
		}
		// ADC 16 bits
		if (codeline.GetNextReg16(dest,REGFLAGS::HL)) {
			if (!codeline.GetNextComma()) {
				return false;//TODO: syntax after ADC HL
			}
			// ADC HL, BC DE HL SP
			if (codeline.GetNextReg16(src,REGFLAGS::BCDESP | REGFLAGS::HL)) {
				codeline.AddCode(0xED, 0x4A + getsubcode(src));
				codeline.SetCycles(15);
				return true;
			}
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
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionADD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest, src;
		int d;
		
		// ADD A,8bit
		if (codeline.GetNextReg8(dest,REGFLAGS::A)) {
			if (!codeline.GetNextComma()) {
				//TODO: syntax error after ADC A
				return false;
			}
			// ADD A,r
			if (codeline.GetNextReg8(src,REGFLAGS::ABCDEHL)) {
				codeline.AddCode(0x80 + getsubcode(src));
				codeline.SetCycles(4);
				return true;
			}
			// ADD A,(HL)
			if (codeline.GetNextIndHL(src)) {
				codeline.AddCode(0x86);
				codeline.SetCycles(7);
				return true;
			}
			// ADD A,(X+d)
			if (codeline.GetNextIndX(src, d)) {
				codeline.AddCode(getsubcode(src), 0x86, d);
				codeline.SetCycles(19);
				return true;
			}
			// ADD A,n
			if (codeline.GetNextNum8(src, d)) {
				codeline.AddCode(0xC6, d);
				codeline.SetCycles(7);
				return true;
			}
			// TODO: error after ADD A,
			return false;
		}
		// ADD 16 bits
		if (codeline.GetNextReg16(dest,REGFLAGS::HLXY)) {
			if (!codeline.GetNextComma()) {
				return false;//TODO: syntax after ADD rp
			}
			// ADD HL, BC DE HL SP
			// ADD IX, BC DE IX SP
			// ADD IY, BC DE IY SP
			if (codeline.GetNextReg16(src,REGFLAGS::BCDESP | REGFLAGS::HLXY)) {
				int cycles = 11;
				// HL/IX/IY as source: only accept HL,HL - IX,IX - IY,IY
				if ( codeline.regaccept(REGFLAGS::HLXY, src)) {
					if (src != dest) return false;
					src = regHL; // mimic ADD HL opcode
				}
				// indexed prefix?
				if ( codeline.regaccept(REGFLAGS::XY, dest)) {
					codeline.AddCode(getsubcode(dest)); // prefix for IX and IY
					cycles = 15;
				}
				// ADD HL opcode
				codeline.AddCode(0x09 + getsubcode(src));
				codeline.SetCycles(cycles);
				return true;
			}
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 AND A/B/C/D/E/H/L
	 AND n
	 AND (HL)/(IX+d)/(IY+d)
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionAND::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType src;
		int d;
		if (codeline.GetNextReg8(src, REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xA0 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		if (codeline.GetNextIndHL(src)) {
			codeline.AddCode(0xA6);
			codeline.SetCycles(7);
			return true;
		}
		if (codeline.GetNextIndX(src, d)) {
			codeline.AddCode(getsubcode(src), 0xA6, d);
			codeline.SetCycles(19);
			return true;
		}
		if (codeline.GetNextNum8(src, d)) {
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
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionBIT::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		int d=0;
		OperandType src, bit;
		if (!codeline.GetNextBitNumber(bit)) return false;
		if (!codeline.GetNextComma()) return false;
		if (codeline.GetNextReg8(src,REGFLAGS::ABCDEHL)) {
			codeline.SetCycles(8);
		} else if (codeline.GetNextIndHL(src)) {
			codeline.SetCycles(12);
		} else if (codeline.GetNextIndX(src, d)) {
			codeline.SetCycles(20);
		} else return false;
		// (IX+d) and (IY+d) are prefixed then use (HL) encoding
		if (codeline.regaccept(REGFLAGS::XY, src)) {
			codeline.AddCode(getsubcode(src), 0xCB, d, 0x40 + getsubcode(bit) + getsubcode(indHL));
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
	 
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCALL::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType cond, dummy;
		int addr;
		if (codeline.GetNextCond(cond)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextNum16(dummy, addr)) {
					codeline.AddCode(0xC4 + getsubcode(cond), (addr & 0xFF), (addr >> 8));
					codeline.SetCycles(10,17);
					return true;
				}
			}
			return false;
		}
		if (codeline.GetNextNum16(cond, addr)) {
			codeline.AddCode(0xCD, (addr & 0xFF), (addr >> 8));
			codeline.SetCycles(17);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 CCF
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCCF::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0x3F);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 CP A/B/C/D/E/H/L
	 CP n
	 CP (HL)/(IX+d)/(IY+d)
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType src;
		int d ;
		if (codeline.GetNextReg8(src,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0xB8 + getsubcode(src));
			codeline.SetCycles(4);
			return true;
		}
		if (codeline.GetNextIndHL(src)) {
			codeline.AddCode(0xBE);
			codeline.SetCycles(7);
			return true;
		}
		if (codeline.GetNextIndX(src, d)) {
			codeline.AddCode(getsubcode(src), 0xBE, d);
			codeline.SetCycles(19);
			return true;
		}
		if (codeline.GetNextNum8(src, d)) {
			codeline.AddCode(0xFE, d);
			codeline.SetCycles(7);
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA9);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPDR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB9);
		codeline.SetCycles(16,21);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA1);
		codeline.SetCycles(16);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPIR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xB1);
		codeline.SetCycles(16,21); // 5 if repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionCPL::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0x2F);
		codeline.SetCycles(4); // 5 if repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDAA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDEC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest;
		int d;
		if (codeline.GetNextReg8(dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0x05 + 8 * getsubcode(dest));
			codeline.SetCycles(4);
			return true;
		}
		if (codeline.GetNextReg16(dest)) {
			if (codeline.regaccept(REGFLAGS::BCDEHLSP, dest)) {
				codeline.AddCode(0x0B + getsubcode(dest));
				codeline.SetCycles(6);
				return true;
			}
			if (codeline.regaccept(REGFLAGS::XY, dest)) {
				codeline.AddCode(getsubcode(dest), 0x0B + getsubcode(regHL));
				codeline.SetCycles(10);
				return true;
			}
		}
		if (codeline.GetNextIndHL(dest)) {
			codeline.AddCode(0x35);
			codeline.SetCycles(11);
			return true;
		}
		if (codeline.GetNextIndX(dest, d)) {
			codeline.AddCode(getsubcode(dest), 0x35, d);
			codeline.SetCycles(23);
			return true;
		}
		return false;
	}

	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xF3);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionDJNZ::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		int d;
		OperandType dummy;
		if (codeline.GetNextNum8(dummy, d)) {
			codeline.AddCode(0x10, d);
			codeline.SetCycles(8,13); // 3 when looping, 2 when finished
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
	 
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEX::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest, src;
		if (codeline.GetNextReg16(dest,REGFLAGS::AF | REGFLAGS::DE)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextReg16(src, REGFLAGS::AFp | REGFLAGS::HL)) {
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
		if (codeline.GetNextIndSP(dest)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextReg16(src, REGFLAGS::HLXY)) {
					if (src == regHL) {
						codeline.AddCode(0xE3);
						codeline.SetCycles(19);
						return true;
					}
					// ADD (SP),IX/IY
					codeline.AddCode(getsubcode(src), 0xE3);
					codeline.SetCycles(23);
					return true;
				}
			}
		}
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEXX::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xD9);
		codeline.SetCycles(4);
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionHALT::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0x76);
		codeline.SetCycles(4);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionIM::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		int m;
		OperandType dummy;
		if (codeline.GetNextNum8(dummy, m)) {
			if (m >= 0 && m <= 2) {
				codeline.AddCode(0xED, 0x46 + m * 0x10);
				codeline.SetCycles(8);
				return true;
			}
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 IN A,(num8)
	 IN r,(C)  ->  A B C D E F H L
	 
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionIN::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest, src;
		int n;
		if (codeline.GetNextReg8(dest,REGFLAGS::ABCDEHL|REGFLAGS::F)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextIndC(src)) {
					codeline.AddCode(0xED, 0x40 + getsubcode(dest) * 8);
					codeline.SetCycles(12);
					return true;
				}
				if (dest == regA && codeline.GetNextInd16(src, n)) {
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
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest;
		int d;
		if (codeline.GetNextReg8(dest,REGFLAGS::ABCDEHL)) {
			codeline.AddCode(0x04 + 8 * getsubcode(dest));
			codeline.SetCycles(4);
			return true;
		}
		if (codeline.GetNextReg16(dest,REGFLAGS::BCDEHLSP | REGFLAGS::XY)) {
			if (codeline.regaccept(REGFLAGS::XY, dest)) {
				codeline.AddCode(getsubcode(dest), 0x03 + getsubcode(regHL));
				codeline.SetCycles(10);
				return true;
			}
			codeline.AddCode(0x03 + getsubcode(dest));
			codeline.SetCycles(6);
			return true;
		}
		if (codeline.GetNextIndHL(dest)) {
			codeline.AddCode(0x34);
			codeline.SetCycles(11);
			return true;
		}
		if (codeline.GetNextIndX(dest, d)) {
			codeline.AddCode(getsubcode(dest), 0x34, d);
			codeline.SetCycles(23);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionIND::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xAA);
		codeline.SetCycles(16);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINDR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xBA);
		codeline.SetCycles(16,21); // 5 when repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		codeline.AddCode(0xED, 0xA2);
		codeline.SetCycles(16); // 5 when repeat, 4 when finished
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionINIR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionJP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType cond, reg;
		int addr;
		if (codeline.GetNextCond(cond)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextNum16(reg, addr)) {
					codeline.AddCode(0xC2 + getsubcode(cond), (addr & 0xFF), (addr >> 8));
					codeline.SetCycles(10);
					return true;
				}
			}
			return false;
		}
		if (codeline.GetNextIndHL(reg)) {
			codeline.AddCode(0xE9);
			codeline.SetCycles(4);
			return true;
		}
		if (codeline.GetNextIndX(reg, addr)) {
			codeline.AddCode(getsubcode(reg), 0xE9);
			codeline.SetCycles(8);
			return true;
		}
		if (codeline.GetNextNum16(cond, addr)) {
			codeline.AddCode(0xC3, (addr & 0xFF), (addr >> 8));
			codeline.SetCycles(10);
			return true;
		}
		return false;
	}
	
	/** Assemble instruction at current token, returns false if error
	 
	 JR cond,d
	 JR d
	 
	 condNZ, condZ, condNC, condC,
	 20      28     30      38
	 
	 invalid: condPO, condPE, condP, condM

	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionJR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType cond, reg;
		int d;
		if (codeline.GetNextCond(cond)) {
			if (cond == condZ || cond == condNZ || cond == condC || cond == condNC) {
				if (codeline.GetNextComma()) {
					if (codeline.GetNextNum8(reg, d)) {
						codeline.AddCode(0x20 + getsubcode(cond), d & 0xFF);
						codeline.SetCycles(7,12);
						return true;
					}
				}
			}
		}
		if (codeline.GetNextNum8(reg, d)) {
			codeline.AddCode(0x18, d & 0xFF);
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
	 
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		return true;
	}

	//---------------------------------------------------------------------------------------------------------------
	
	
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionPUSH::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionPOP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

	/** Assemble instruction at current token, returns false if error
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionLDI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		return true;
	}
	
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionLDIR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionLDD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionLDDR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}



		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSUB::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSBC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionOR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionXOR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}


		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionNEG::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSCF::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionNOP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}



		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRLCA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRLA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRRCA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRRA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRLC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRL::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRRC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSLA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}


		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSRA::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSRL::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRLD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRRD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}


		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionSET::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRES::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}


		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRET::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRETI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRETN::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionRST::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}




		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionOUT::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionOUTI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionOTIR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionOUTD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionOTDR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

} // namespace MUZ

