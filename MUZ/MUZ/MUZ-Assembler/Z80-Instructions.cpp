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
		int value;
		if (codeline.GetNextReg8(dest)) {
			if (dest != regA) {
				//TODO: wrong register ADC only works on A
				return false;
			}
			if (!codeline.GetNextComma()) {
				//TODO: syntax error after ADC A
				return false;
			}
			// ADC A,?
			if (codeline.GetNextReg8(src)) {
				if (src == regI || src == regR) return false;
				codeline.AddCode(0x88 + getsubcode(src));
				codeline.SetCycles(1);
				return true;
			}
			if (codeline.GetNextIndHL(src)) {
				codeline.AddCode(0x8E);
				codeline.SetCycles(2);
				return true;
			}
			if (codeline.GetNextIndX(src, value)) {
				codeline.AddCode(getsubcode(src), 0x8E, (value & 0xFF));
				codeline.SetCycles(5);
				return true;
			}
			if (codeline.GetNextNum8(src, value)) {
				codeline.AddCode(0xCE, (value & 0xFF));
				codeline.SetCycles(2);
				return true;
			}
			// TODO: error after ADD A,
			return false;
		}
		if (codeline.GetNextReg16(dest)) {
			if (dest != regHL) {
				// TODO: error, ADC on another 16-bit register than HL
				return false;
			}
			if (!codeline.GetNextComma()) {
				return false;//TODO: syntax after ADC HL
			}
			if (codeline.GetNextReg16(src)) {
				if (src == regBC || src == regDE || src == regHL || src == regSP) {
					codeline.AddCode(0xED, 0x4A + getsubcode(src));
					codeline.SetCycles(2);
					return true;
				}
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
		int value;
		if (codeline.GetNextReg8(dest)) {
			if (dest != regA) {
				//TODO: wrong register ADC only works on A
				return false;
			}
			if (!codeline.GetNextComma()) {
				//TODO: syntax error after ADC A
				return false;
			}
			// ADD A,?
			if (codeline.GetNextReg8(src)) {
				if (src == regI || src == regR) return false;
				codeline.AddCode(0x80 + getsubcode(src));
				codeline.SetCycles(1);
				return true;
			}
			if (codeline.GetNextIndHL(src)) {
				codeline.AddCode(0x86);
				codeline.SetCycles(2);
				return true;
			}
			if (codeline.GetNextIndX(src, value)) {
				codeline.AddCode(getsubcode(src), 0x86, (value & 0xFF));
				codeline.SetCycles(5);
				return true;
			}
			if (codeline.GetNextNum8(src, value)) {
				codeline.AddCode(0xC6, (value & 0xFF));
				codeline.SetCycles(2);
				return true;
			}
			// TODO: error after ADD A,
			return false;
		}
		if (codeline.GetNextReg16(dest)) {
			if (!codeline.GetNextComma()) {
				return false;//TODO: syntax after ADD rp
			}
			if (codeline.GetNextReg16(src)) {
				int cycles = 3;
				if (src == regIX || src == regIY || src == regHL) {
					if (src != dest) return false; // only accept HL,HL - IX,IX - IY,IY
					src = regHL;
				}
				if (dest == regIX || dest == regIY) {
					codeline.AddCode(getsubcode(dest)); // prefix for IX and IY
					cycles = 4;
				}
				if (src == regBC || src == regDE || src == regHL || src == regSP) {
					codeline.AddCode(0x09 + getsubcode(src));
					codeline.SetCycles(cycles);
					return true;
				}
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
		int value;
		if (codeline.GetNextReg8(src)) {
			// AND r
			if (src == regI || src == regR) {
				return false;
			}
			codeline.AddCode(0xA0 + getsubcode(src));
			codeline.SetCycles(1);
			return true;
		}
		if (codeline.GetNextIndHL(src)) {
			codeline.AddCode(0xA6);
			codeline.SetCycles(2);
			return true;
		}
		if (codeline.GetNextIndX(src, value)) {
			codeline.AddCode(getsubcode(src), 0xA6, (value & 0xFF));
			codeline.SetCycles(5);
			return true;
		}
		if (codeline.GetNextNum8(src, value)) {
			codeline.AddCode(0xE6, (value & 0xFF));
			codeline.SetCycles(2);
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
		if (codeline.GetNextReg8(src)) {
			if (src == regI || src == regR) return false;
			codeline.SetCycles(2);
		} else if (codeline.GetNextIndHL(src)) {
			codeline.SetCycles(3);
		} else if (codeline.GetNextIndX(src, d)) {
			codeline.SetCycles(5);
		} else return false;
		// (IX+d) and (IY+d) are prefixed then use (HL) encoding
		if (src == regIX || src == regIY) {
			codeline.AddCode(getsubcode(src), 0xCB, 0x40 + getsubcode(bit) + getsubcode(indHL), (d & 0xFF));
		} else {
			codeline.AddCode(0xCB, 0x40 + getsubcode(bit) + getsubcode(src));
		}
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 
	 CALL cond, num16
	 CALL num16
	 
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
					codeline.AddCode(0xC4 + getsubcode(cond), (addr >> 8), (addr & 0xFF));
					codeline.SetCycles(5); // 5 if cond is true, 3 if false
					return true;
				}
			}
			return false;
		}
		if (codeline.GetNextNum16(cond, addr)) {
			codeline.AddCode(0xCD, (addr >> 8), (addr & 0xFF));
			codeline.SetCycles(3);
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
		codeline.SetCycles(1);
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
		int value;
		if (codeline.GetNextReg8(src)) {
			// AND r
			if (src == regI || src == regR) {
				return false;
			}
			codeline.AddCode(0xB8 + getsubcode(src));
			codeline.SetCycles(1);
			return true;
		}
		if (codeline.GetNextIndHL(src)) {
			codeline.AddCode(0xBE);
			codeline.SetCycles(2);
			return true;
		}
		if (codeline.GetNextIndX(src, value)) {
			codeline.AddCode(getsubcode(src), 0xBE, (value & 0xFF));
			codeline.SetCycles(5);
			return true;
		}
		if (codeline.GetNextNum8(src, value)) {
			codeline.AddCode(0xFE, (value & 0xFF));
			codeline.SetCycles(2);
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
		codeline.SetCycles(4);
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
		codeline.SetCycles(5); // 5 if repeat, 4 when finished
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
		codeline.SetCycles(4);
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
		codeline.SetCycles(5); // 5 if repeat, 4 when finished
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
		codeline.SetCycles(1); // 5 if repeat, 4 when finished
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
		codeline.SetCycles(1); // 5 if repeat, 4 when finished
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
		int value;
		if (codeline.GetNextReg8(dest)) {
			if (dest == regI || dest == regR) {
				return false;
			}
			codeline.AddCode(0x05 + 8 * getsubcode(dest));
			codeline.SetCycles(1);
			return true;
		}
		if (codeline.GetNextReg16(dest)) {
			if (dest == regBC || dest == regDE || dest == regHL || dest == regSP) {
				codeline.AddCode(0x0B + getsubcode(dest));
				codeline.SetCycles(1);
				return true;
			}
			if (dest == regIX || dest == regIY) {
				codeline.AddCode(getsubcode(dest), 0x0B + getsubcode(regHL));
				codeline.SetCycles(2);
				return true;
			}
		}
		if (codeline.GetNextIndHL(dest)) {
			codeline.AddCode(0x35);
			codeline.SetCycles(3);
			return true;
		}
		if (codeline.GetNextIndX(dest, value)) {
			codeline.AddCode(getsubcode(dest), 0x35, (value & 0xFF));
			codeline.SetCycles(6);
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
		codeline.SetCycles(1);
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
			codeline.AddCode(0x10, (d & 0xFF));
			codeline.SetCycles(3); // 3 when looping, 2 when finished
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
		codeline.SetCycles(1);
		return true;
	}

	/** Assemble instruction at current token, returns false if error
	 EX AF,AF'  08    1
	 EX DE,HL   EB    1
	 EX (SP),HL E3    5
	 EX (SP),IX dd E3 6
	 EX (SP),IY fd E3 6
	 
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionEX::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType dest, src;
		if (codeline.GetNextReg16(dest)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextReg16(src)) {
					if (src == regAFp && dest == regAF) {
						codeline.AddCode(0x08);
						codeline.SetCycles(1);
						return true;
					}
					if (src == regHL && dest == regDE) {
						codeline.AddCode(0xEB);
						codeline.SetCycles(1);
						return true;
					}
				}
			}
			return false;
		}
		if (codeline.GetNextIndSP(dest)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextReg16(src)) {
					if (src == regHL) {
						codeline.AddCode(0xE3);
						codeline.SetCycles(5);
						return true;
					}
					if (src == regIX || src == regIY) {
						codeline.AddCode(getsubcode(src), 0xE3);
						codeline.SetCycles(6);
						return true;
					}
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
		codeline.SetCycles(1);
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
		codeline.SetCycles(1);
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
				codeline.SetCycles(2);
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
		if (codeline.GetNextReg8(dest)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextIndC(src)) {
					if (dest == regA || dest == regB || dest == regC || dest == regD || dest == regE || dest == regF || dest == regH || dest == regL){
						codeline.AddCode(0xED, 0x40 + getsubcode(dest) * 8);
						codeline.SetCycles(3);
						return true;
					}
					return false;
				}
				if (dest == regA && codeline.GetNextInd16(src, n)) {
					codeline.AddCode(0xDB, (n & 0xFF));
					codeline.SetCycles(3);
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
		int value;
		if (codeline.GetNextReg8(dest)) {
			if (dest == regI || dest == regR) {
				return false;
			}
			codeline.AddCode(0x04 + 8 * getsubcode(dest));
			codeline.SetCycles(1);
			return true;
		}
		if (codeline.GetNextReg16(dest)) {
			if (dest == regBC || dest == regDE || dest == regHL || dest == regSP) {
				codeline.AddCode(0x03 + getsubcode(dest));
				codeline.SetCycles(1);
				return true;
			}
			if (dest == regIX || dest == regIY) {
				codeline.AddCode(getsubcode(dest), 0x03 + getsubcode(regHL));
				codeline.SetCycles(2);
				return true;
			}
		}
		if (codeline.GetNextIndHL(dest)) {
			codeline.AddCode(0x34);
			codeline.SetCycles(3);
			return true;
		}
		if (codeline.GetNextIndX(dest, value)) {
			codeline.AddCode(getsubcode(dest), 0x34, (value & 0xFF));
			codeline.SetCycles(6);
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
		codeline.SetCycles(4);
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
		codeline.SetCycles(5); // 5 when repeat, 4 when finished
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
		codeline.SetCycles(5); // 5 when repeat, 4 when finished
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
		codeline.SetCycles(5); // 5 when repeat, 4 when finished
		return true;
	}
	
	/** Assemble instruction at current token, returns false if error
	 JP cond, num16
	 JP num16
	 JP (HL) (IX) (IY)
	 
	 condNZ, condZ, condNC, condC, condPO, condPE, condP, condM,
	 c2      ca     d2      da     e2      ea      f2     fa
	 @param as the assembler which stores symbols and assembly
	 @param codeline the code line in which assembled codes will be stored
	 @param label the last or current label if any, or nullptr
	 @param msg the message list which will receive any warning or error information
	 */
	bool InstructionJP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType cond, dummy;
		int addr;
		if (codeline.GetNextCond(cond)) {
			if (codeline.GetNextComma()) {
				if (codeline.GetNextNum16(dummy, addr)) {
					codeline.AddCode(0xC2 + getsubcode(cond), (addr >> 8), (addr & 0xFF));
					codeline.SetCycles(3);
					return true;
				}
			}
			return false;
		}
		if (codeline.GetNextIndHL(dummy)) {
			codeline.AddCode(0xE9);
			codeline.SetCycles(1);
			return true;
		}
		if (codeline.GetNextIndX(dummy, addr)) {
			codeline.AddCode(getsubcode(dummy), 0xE9);
			codeline.SetCycles(2);
			return true;
		}
		if (codeline.GetNextNum16(cond, addr)) {
			codeline.AddCode(0xC3, (addr >> 8), (addr & 0xFF));
			codeline.SetCycles(3);
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
	bool InstructionJR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		OperandType cond, reg;
		int e;
		if (codeline.GetNextCond(cond)) {
			if (cond == condZ || cond == condNZ || cond == condC || cond == condNC) {
				if (codeline.GetNextComma()) {
					if (codeline.GetNextNum8(reg, e)) {
						codeline.AddCode(0x20 + getsubcode(cond), e & 0xFF);
						codeline.SetCycles(3); // 3 if condition true, 2 if false
						return true;
					}
				}
			}
		}
		if (codeline.GetNextNum8(reg, e)) {
			codeline.AddCode(0x18, e & 0xFF);
			codeline.SetCycles(3);
			return true;
		}
		return false;
	}

	/** Assemble LD instruction at current token, returns false if error
	 
	 LD rd,rs           B C D E H L (HL) A    0x40 + rd << 3 + rs
	 LD rd,(is)         (HL) (IX+d) (IY+d)	   0x46 + rd << 3
	 LD (id),rs         (HL) (IX+d) (IY+d)	   0x46 + r3
	 LD rd,num8         B C D E H L (HL) A    0x70 + rd << 3
	 LD A,(addr)
	 LD A,(BC)
	 LD A,(DE)
	 LD (addr),A
	 LD (BC),A
	 LD (DE),A
	 LD A,I
	 LD A,R
	 LD I,A
	 LD R,A
	 LD rp,num16
	 LD IX,num16
	 LD IY,num16
	 LD rp,(addr)       BC DE HL SP
	 LD IX,(addr)
	 LD IY,(addr)
	 LD HL,(addr)
	 LD (addr),rp
	 LD (addr),IX
	 LD (addr),IY
	 LD (addr),HL
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

