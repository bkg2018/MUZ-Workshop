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
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionADD::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
		{
			return true;
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
			if (!codeline.enoughTokensLeft(3)) {
				//TODO: not enough tokens
				return false;
			}
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
					if (src == regA) {
						codeline.SetCode(0x8F);
					} else if (src == regB) {
						codeline.SetCode(0x88);
					} else if (src == regC) {
						codeline.SetCode(0x89);
					} else if (src == regD) {
						codeline.SetCode(0x8A);
					} else if (src == regE) {
						codeline.SetCode(0x8B);
					} else if (src == regH) {
						codeline.SetCode(0x8C);
					} else if (src == regL) {
						codeline.SetCode(0x8D);
					} else {
						//TODO: wrong register
						return false;
					}
					return true;
				}
				if (codeline.GetNextIndHL(src)) {
					codeline.SetCode(0x8E);
					return true;
				}
				if (codeline.GetNextIndX(src, value)) {
					if (src == regIX)
						codeline.SetCode(0xDD, 0x8E, (value & 0xFF));
					else
						codeline.SetCode(0xFD, 0x8E, (value & 0xFF));
					return true;
				}
				if (codeline.GetNextNum8(src, value)) {
					codeline.SetCode(0xCE, (value & 0xFF));
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
					if (src == regBC) {
						codeline.SetCode(0xED, 0x4A);
					} else if (src == regDE) {
						codeline.SetCode(0xED, 0x5A);
					} else if (src == regHL) {
						codeline.SetCode(0xED, 0x6A);
					} else if (src == regSP) {
						codeline.SetCode(0xED, 0x7A);
					} else {
						//TODO: wrong register
						return false;
					}
					return true;
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
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionAND::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		bool InstructionBIT::Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg)
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

