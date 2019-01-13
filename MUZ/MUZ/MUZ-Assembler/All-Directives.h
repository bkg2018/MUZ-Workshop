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
	
	/** Returns true if current token is a comma, and go next token. Don't change current token if not found.
	 @param codeline the code line to assemble, with curtoken set to the start token for analysis
	 @return true if a comma has been found
	 */
	bool GetComma(class MUZ::CodeLine& codeline) ;
	
	/** Returns true if the tokens array has at least the resquested number of tokens available starting at curtoken.
	 @param codeline the code line to assemble, with curtoken set to the start token for analysis
	 */
	bool EnoughTokensLeft(class CodeLine& codeline, int number) ;
	

	// Preprocessor directives (#xxx)
	
	/** #DEFINE directive. */
	class DirectiveDEFINE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #UNDEF */
	class DirectiveUNDEFINE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #IF */
	class DirectiveIF : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #IFDEF */
	class DirectiveIFDEF : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #IFNDEF */
	class DirectiveIFNDEF : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #ELSE - does nothing, handled by Assembler. */
	class DirectiveELSE : public Directive {
	};
	/** #ENDIF - does nothing, handled by Assembler. */
	class DirectiveENDIF : public Directive {
	};
	/** #INCLUDE */
	class DirectiveINCLUDE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #INSERTHEX */
	class DirectiveINSERTHEX : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** #INSERTBIN */
	class DirectiveINSERTBIN : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};

	// Assembler directives (.xxx)
	
	
	/** The .PROC drective only checks that its followed by Z80. */
	class DirectivePROC : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .CODE */
	class DirectiveCODE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .DATA */
	class DirectiveDATA : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .ORG */
	class DirectiveORG : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .EQU */
	class DirectiveEQU : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .DB */
	class DirectiveBYTE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .DW */
	class DirectiveWORD : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};
	/** .DS */
	class DirectiveSPACE : public Directive {
		virtual bool Parse(class Assembler& as, class Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg);
	};


}
#endif /* All_Directives_h */
