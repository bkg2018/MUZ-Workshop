//
//  Directive.hpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 03/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Directive_h
#define Directive_h

#include "CodeLine.h"
#include "Instruction.h"

namespace MUZ {

	/** Directives are special instructions which do not generate assembled code, but rather
    act on the assembler work process. There are preprocessor directives, which define and test
    abstract symbols, and assembler directives which change the code address or compute labels and values
    which can be used as constants in instruction arguments.
 
 	Directives are derived from Instruction because they share the same argument parsing.
 */

	class Directive : public Instruction
	{
	public:
		
		virtual bool Parse(class Assembler& as,				// assembler woth current address
						   class Parser& parser,			// parser status
						   CodeLine& codeline,
						   class Label* label,				// nullptr or label found in line
						   ErrorList& msg) {
			return Instruction::Parse(as, parser, codeline, label, msg);
		}
	};
	
	typedef std::unordered_map<std::string, Directive*> DirectivesMap;
	
} // namespace MUZ

#endif /* Directive_h */
