//
//  Instruction.hpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Instruction_h
#define Instruction_h

#include <vector>
#include <string>
#include <unordered_map>
#include "Parser.h"

namespace MUZ {
	
	/** Base class for each possible CPU instructiopn name.
	 Each derived instruction class will implement its argument parsing and use.
	 There may be a lot of different functioning modes for an instruction, but there is only
	 one derived class for each instruction name, so the instruction class will have to manage
	 any possible sub-deriving to delegate its work to different internal derived classes.
	*/
	class Instruction
	{
	public:
		/** Does the parsing and returns false in case of error.
		 @param as the assembler which stores symbols and assembly
		 @param parser the parser with current status for tokens and current token
		 @param label the last or current label if any, or nullptr
		 @param msg the message list which will receive any warning or error information
		 */
		virtual bool Parse(class Assembler& as,
						   class Parser& parser,
						   CodeLine& codeline,						// line number in file, 1-based
						   class Label* label,
						   ErrorList& msg) {
			ErrorMessage m = { errorTypeERROR, "Non derived instruction class used", "", 0};
			msg.push_back(m);
			return true;
		};
		
	};
	
	typedef std::unordered_map<std::string, Instruction*> InstructionsMap;
	
} // namespace MUZ
#endif /* Instruction_h */
