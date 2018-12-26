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
	
		/** Assemble LD instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionLD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}
	
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
		bool InstructionEXX::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionEX::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
		bool InstructionCPI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionCPD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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
			return true;
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
					return true;
				}
				if (codeline.GetNextIndHL(src)) {
					codeline.AddCode(0x86);
					return true;
				}
				if (codeline.GetNextIndX(src, value)) {
					codeline.AddCode(getsubcode(src), 0x86, (value & 0xFF));
					return true;
				}
				if (codeline.GetNextNum8(src, value)) {
					codeline.AddCode(0xC6, (value & 0xFF));
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
					if (src == regIX || src == regIY || src == regHL) {
						if (src != dest) return false; // only accept HL,HL - IX,IX - IY,IY
						src = regHL;
					}
					if (dest == regIX || dest == regIY) {
						codeline.AddCode(getsubcode(dest)); // prefix for IX and IY
					}
					if (src == regBC || src == regDE || src == regHL || src == regSP) {
						codeline.AddCode(0x09 + getsubcode(src));
						return true;
					}
				}
			}
			return false;
		}

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
					return true;
				}
				if (codeline.GetNextIndHL(src)) {
					codeline.AddCode(0x8E);
					return true;
				}
				if (codeline.GetNextIndX(src, value)) {
					codeline.AddCode(getsubcode(src), 0x8E, (value & 0xFF));
					return true;
				}
				if (codeline.GetNextNum8(src, value)) {
					codeline.AddCode(0xCE, (value & 0xFF));
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
						return true;
					}
				}
			}
			return false;
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
				return true;
			}
			if (codeline.GetNextIndHL(src)) {
				codeline.AddCode(0xA6);
				return true;
			}
			if (codeline.GetNextIndX(src, value)) {
				codeline.AddCode(getsubcode(src), 0xA6, (value & 0xFF));
				return true;
			}
			if (codeline.GetNextNum8(src, value)) {
				codeline.AddCode(0xE6, (value & 0xFF));
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
		bool InstructionCP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionINC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionDEC::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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
		bool InstructionCCF::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
		bool InstructionHALT::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionDI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionEI::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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
		 	bit b,r
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
			} else if (codeline.GetNextIndHL(src)) {
			} else if (codeline.GetNextIndX(src, d)) {
			} else return false;
			// IX and IY are prefixxed then using (HL) encoding
			if (src == regIX || src == regIY) {
				codeline.AddCode(getsubcode(src), 0xCB, 0x40 + getsubcode(bit) + getsubcode(regHL), (d & 0xFF));
			} else {
				codeline.AddCode(0xCB, 0x40 + getsubcode(bit) + getsubcode(src));
			}
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
		bool InstructionJP::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionJR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionCALL::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
		bool InstructionIN::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionINR::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
		}

		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionIND::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
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

