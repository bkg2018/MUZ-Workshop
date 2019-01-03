//
//  All-Directives.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 03/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef All_Directives_h
#define All_Directives_h

#include "Directive.h"
#include "CodeLine.h"
#include "Assembler.h"
#include "StrUtils.h"

namespace MUZ {
	
	// Preprocessor directives (#xxx)
	
	class DirectiveDEFINE : public Directive {
		// Defines the new symbol or returns false.
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	
	class DirectiveUNDEFINE : public Directive {
		// Undefines the existing symbol or returns false.
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveIF : public Directive {
		// evaluate expression and return true if non null
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveIFDEF : public Directive {
		// Returns true if the symbol exists
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveIFNDEF : public Directive {
		// returns true if the symbol doesn't exist
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveELSE : public Directive {
		
	};
	class DirectiveENDIF : public Directive {
		
	};
	class DirectiveINCLUDE : public Directive {
		// returns true if a file must be included
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveINSERTHEX : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};

	// Assembler directives (.xxx)
	
	
	/** The .PROC drective only checks that its followed by Z80. */
	class DirectivePROC : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};

	class DirectiveCODE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};

	class DirectiveDATA : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveORG : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveHEXBYTES : public Directive {
		
	};
	class DirectiveEQU : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveBYTE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveWORD : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	class DirectiveSPACE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};


}
#endif /* All_Directives_h */
