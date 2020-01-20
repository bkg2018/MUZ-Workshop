//
//  Z180-Instructions.h
//  muzlib
//
//  Created by Francis Pierot on 14/01/2020.
//  Copyright © 2020 Francis Pierot. All rights reserved.
//

#ifndef Z180_Instructions_h
#define Z180_Instructions_h

#include "Z80-Instructions.h"

namespace MUZ {
namespace Z180 {

	class InstructionTST : public Instruction {
	public:
		/** Assemble TST instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionTSTIO : public Instruction {
	public:
		/** Assemble TSTIO instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionSLP : public Instruction {
	public:
		/** Assemble SLP instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionOUT0 : public Instruction {
	public:
		/** Assemble OUT0 instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionIN0 : public Instruction {
	public:
		/** Assemble IN0 instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionOTDM : public Instruction {
	public:
		/** Assemble OTDM instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionOTIM : public Instruction {
	public:
		/** Assemble OTIM instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionOTDMR : public Instruction {
	public:
		/** Assemble OTDMR instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionOTIMR : public Instruction {
	public:
		/** Assemble OTIMR instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
	class InstructionMLT : public Instruction {
	public:
		/** Assemble MLT instruction at current token, returns false if error
		 @param codeline the code line in which assembled codes will be stored
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Assemble(CodeLine& codeline, ErrorList& msg);
	};
} // namespace Z180
} // namespace MUZ
#endif /* Z180_Instructions_h */
