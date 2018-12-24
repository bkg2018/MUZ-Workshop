//
//  Z80-Instructions.h - Derived instruction classes for the Z-80.
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Z80_Instructions_h
#define Z80_Instructions_h

#include "Instruction.h"

namespace MUZ {

/** instruction names will lead to singleton instances of theese derived classes. */
	class InstructionLD : public Instruction {
	public:
		/** Assemble LD instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
		
	class InstructionPUSH : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionPOP : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionEXX : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionEX : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionLDI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionLDIR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionLDD : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionLDDR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCPI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCPIR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCPD : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCPDR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionADD : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionADC : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSUB : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSBC : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionAND : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionOR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionXOR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCP : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionINC : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionDEC : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionDAA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCPL : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionNEG : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCCF : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSCF : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionNOP : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionHALT : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionDI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionEI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionIM : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
		
	class InstructionRLCA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRLA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRRCA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRRA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRLC : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRL : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRRC : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSLA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSRA : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSRL : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRLD : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRRD : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionBIT : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionSET : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRES : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionJP : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionJR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionDJNZ : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionCALL : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRET : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRETI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRETN : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionRST : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionIN : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionINI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionINR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionIND : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionINDR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionOUT : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionOUTI : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionOTIR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionOUTD : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class InstructionOTDR : public Instruction {
	public:
		/** Assemble instruction at current token, returns false if error
		 @param as the assembler which stores symbols and assembly
		 @param codeline the code line in which assembled codes will be stored
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(class Assembler& as, CodeLine& codeline, class Label* label, ErrorList& msg);
};
	
} // namespace MUZ
#endif /* Z80_Instructions_h */
