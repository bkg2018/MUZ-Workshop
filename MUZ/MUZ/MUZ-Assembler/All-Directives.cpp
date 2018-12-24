//
//  All-Directives.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 03/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "All-Directives.h"
#include "Assembler.h"
#include "Errors.h"
#include "Expression.h"
#include "Parser.h"

namespace MUZ {
	
	/** #DEFINE <symbol> <stringexpression>
	 	and
	 	#DEFINE <symbol>
	 
	 	returns true if the new symbol has been defined
	 */
	bool DirectiveDEFINE::Parse( Assembler& as, Parser& parser, CodeLine& codeline, Label* label, ErrorList& msg) {
		//TODO: string expressions for argument starting at curtoken +2
		if (!parser.ExistMoreToken(1)) return false;
		ParseToken& symbol = parser.NextToken();
		if (symbol.type != tokenTypeLETTERS) {
			//TODO: error, invalid name for symbol
			return false;
		}
		// skip directive
		parser.JumpTokens(1);
		// resolve expression parts
		parser.ResolveNextSymbols(); // skips symbol before resolving
		
		// undefine the symbol if it exists
		as.DeleteDefSymbol(symbol.source);
		// create the #DEFINE symbol
		std::string value;
		if (parser.ExistMoreToken(1)) {
			parser.JumpNextToken();// skip symbol
			value = parser.EvaluateString();
		}
		DefSymbol* defsymbol = as.CreateDefSymbol(symbol.source, value);
		if (defsymbol) {
			defsymbol->line.file = codeline.file;
			defsymbol->line.line = codeline.line;
			return true;
		}
		msg.push_back({ errorTypeERROR, "couldn't create #DEFINE symbol", "", codeline.line});
		return false;
	}

	/** #UNDEF <symbol>
	 	returns true if an existing symbol has been undefined
	 */
	bool DirectiveUNDEFINE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		ParseToken& symbol = parser.NextToken();
		if (symbol.type != tokenTypeLETTERS) {
			//TODO: error, invalid name for symbol
			return false;
		}
		// Delete the symbol : no symbol resolving or the symbol to undefine would be replaced by its value
		as.DeleteDefSymbol(symbol.source);
		parser.JumpTokens(1);
		return true;
	}
	
	/** #IFDEF <symbol>
	 	returns true if the symbol is defined
	 */
	bool DirectiveIFDEF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		// and now check existence of the symbol
		if (!parser.ExistMoreToken(1)) return false;
		ParseToken& symbol = parser.NextToken();
		if (symbol.type != tokenTypeLETTERS) {
				//TODO: error, invalid name for symbol
				return false;
			}
		parser.JumpTokens(1);
		//TODO: resolve symbols and evaluate as string?
		return as.ExistSymbol(symbol.source);
	}

	/** #IFNDEF <symbol>
	 	returns true if the symbol is not defined
	 */
	bool DirectiveIFNDEF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		ParseToken& symbol = parser.NextToken();
		if (symbol.type != tokenTypeLETTERS) {
			//TODO: error, invalid name for symbol
			return false;
		}
		parser.JumpTokens(1);
		//TODO: resolve symbols and evaluate as string?
		return ! as.ExistSymbol(symbol.source);
	}

	/** #IF <expression>
	 returns true if the expression is true (or not "" and not 0)
	 */
	bool DirectiveIF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if ( ! parser.ExistMoreToken(1) ) return false;
		parser.ResolveNextSymbols();
		parser.JumpTokens(1);
		bool b = parser.EvaluateBoolean();
		parser.JumpTokens(1);
		return b;
	}
	
	/** #INCLUDE <file>
	 	returns true if a file must be included
	 */
	bool DirectiveINCLUDE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();
		ParseToken& filetoken = parser.NextToken();
		// and include new file
		if ((filetoken.type != tokenTypeSTRING) && (filetoken.type != tokenTypeFILENAME)) {
			//TODO: invalid name for an include file
			return false;
		}
		// trim spaces at the end of filename
		strtrimright(filetoken.source);
		if (as.AssembleFile(filetoken.source, true, codeline, msg)) {
			//TODO: anything special to do if assembly replied ok?
		}
		
		// tells the parser that the file must be included
		parser.JumpTokens(1);
		return true;
	}

	/** .PROC Z80
	 	returns true if Z80 has been specified
	 */
	bool DirectivePROC::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();
		ParseToken& proc = parser.NextToken();
		if (proc.source != "Z80") {
			//errors.push_back(".PROC only handles Z80");
			return false;
		}
		parser.JumpTokens(1);
		return true;
	}
	
	/** [label:] .ORG <value>
	 	returns true if the assembling address must be changed
	 */
	bool DirectiveORG::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();
		ParseToken& address = parser.NextToken();
		parser.JumpTokens(1);
		if (address.type == tokenTypeDECNUMBER) {
			as.SetAddress( address.getAsAddress() );
			return true;
		}
		// TODO: return error, parsing problem, not a decimal number
		return false;
	}
	
	/** label[:]  .EQU <expression>
	 	returns true if the label has been created with the value as a decimal number
	 */
	bool DirectiveEQU::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();
		parser.JumpTokens(1); // skip after .EQU
		ADDRESSTYPE address = parser.EvaluateAddress();
		if (label) {
			label->SetAddress(address);
			label->equate = true;
			parser.JumpTokens(1);
			codeline.label = label;
			return true;
		}
		// TODO: return error
		return false;
	}
	
} // namespace MUZ
