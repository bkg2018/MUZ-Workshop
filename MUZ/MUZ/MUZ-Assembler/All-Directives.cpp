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
	
	
	//MARK: - Preprocessor directives (#)
	
	/** #DEFINE <symbol> <stringexpression>
	 	and
	 	#DEFINE <symbol>
	 	returns true if the new symbol has been defined
	 */
	bool DirectiveDEFINE::Parse( Assembler& as, Parser& parser, CodeLine& codeline, Label* label, ErrorList& msg) {

		if (!parser.ExistMoreToken(1)) return false;
		ParseToken& symbol = parser.NextToken();
		if (symbol.type != tokenTypeLETTERS) {
			msg.push_back({ errorTypeERROR, errorInvalidSymbol, &codeline});
			return false;
		}
		// skip directive
		parser.JumpTokens(1);
		// resolve expression parts
		parser.ResolveNextSymbols(); // skips next symbol before resolving
		
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
			defsymbol->codeline = &codeline;
			return true;
		}
		msg.push_back({ errorTypeERROR, errorDefine, &codeline});
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
		std::string symbol;
		parser.JumpNextToken();
		symbol = parser.EvaluateString(); // do not resolve symbols here!
		return as.ExistSymbol(symbol);
	}

	/** #IFNDEF <symbol>
	 	returns true if the symbol is not defined
	 */
	bool DirectiveIFNDEF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		std::string symbol;
		parser.JumpNextToken();
		symbol = parser.EvaluateString(); // // do not resolve symbols here!
		return ! as.ExistSymbol(symbol);
	}

	/** #IF <expression>
	 returns true if the expression is true (or not "" and not 0)
	 */
	bool DirectiveIF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if ( ! parser.ExistMoreToken(1) ) return false;
		parser.ResolveNextSymbols();
		parser.JumpTokens(1);
		bool b = parser.EvaluateBoolean();
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
		//TODO: anything special to do if assembly returned ok or not ok?
		if (as.IsFirstPass()) {
			codeline.includefile = (int)as.m_files.size();
			if (as.AssembleIncludedFilePassOne(filetoken.source, codeline, msg)) {
			}
		} else {
			if (as.AssembleIncludedFilePassTwo(filetoken.source, codeline, msg)) {
			}
		}
		
		// tells the parser that the file must be included
		parser.JumpTokens(1);
		return true;
	}

	/** #INSERTHEX <file>
	  */
	bool DirectiveINSERTHEX::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
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
		if (as.IsFirstPass()) {
			codeline.includefile = (int)as.m_files.size();
		}
		if (as.AssembleHexFile(filetoken.source, codeline, msg)) {
			//TODO: anything special to do if assembly replied ok?
		}
		
		// tells the parser that the file must be included
		parser.JumpTokens(1);
		return true;
	}
	/** #INSERTBIN <file>
	 */
	bool DirectiveINSERTBIN::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
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
		if (as.IsFirstPass()) {
			codeline.includefile = (int)as.m_files.size();
		}
		if (as.AssembleBinFile(filetoken.source, codeline, msg)) {
			//TODO: anything special to do if assembly replied ok?
		}
		
		// tells the parser that the file must be included
		parser.JumpTokens(1);
		return true;
	}
	
	//MARK: - Assembler directives (.)
	
	/** .PROC Z80
	 	returns true if Z80 has been specified
	 */
	bool DirectivePROC::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();// allow string expressions
		ParseToken& proc = parser.NextToken();
		if (proc.source != "Z80") {
			//errors.push_back(".PROC only handles Z80");
			return false;
		}
		parser.JumpTokens(1);
		return true;
	}
	
	/** [label:] .CODE [name]
	 */
	bool DirectiveCODE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		std::string name;
		if (parser.ExistMoreToken(1)) {
			parser.JumpNextToken();
			name = parser.EvaluateString();
		}
		as.SetCodeSection( name );
		return true;
	}
	
	/** [label:] .DATA [name] [,SAVE]
	 */
	bool DirectiveDATA::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		std::string name;
		bool save = false;
		if (parser.ExistMoreToken(1)) {
			ParseToken& token = parser.JumpNextToken();
			if (token.type != tokenTypeCOMMA) {
				name = parser.EvaluateString();
			}
			if (GetComma(codeline)) {
				std::string param = parser.EvaluateString();
				param = std::to_upper(param);
				if (param == "SAVE") {
					save = true;
				}
			}
		}
		as.SetDataSection( name, save );
		return true;
	}
	
	/** Sets the current address in the current section.
	 [label:] .ORG <value>
	 	returns true if the assembling address must be changed
	 */
	bool DirectiveORG::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		std::vector<int> unsolved = parser.ResolveNextSymbols();
		ADDRESSTYPE address = 0;
		parser.JumpTokens(1); // skip after .EQU
		address = parser.EvaluateAddress();
		Section* section= as.GetSection();
		if (section) {
			section->SetOrg(address);
		}
		as.SetAddress( address );
		return true;
	}
	
	/** label[:]  [.]EQU <expression>
	 	returns true if the label has been created with the value as a decimal number
	 */
	bool DirectiveEQU::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		std::vector<int> unsolved = parser.ResolveNextSymbols();
		ADDRESSTYPE address = 0;
		if (unsolved.size() > 0) {
			// TODO: warning?
		}
		// compute address, unsolved symbols have been replaced by "0"
		parser.JumpTokens(1); // skip after .EQU
		address = parser.EvaluateAddress();
		// use previous label if none on this line
		if (label == nullptr)
			label = codeline.label;
		// set label address/value
		if (label) {
			label->Equate(address);
			codeline.label = label;
			return true;
		}
		// TODO: return error
		return false;
	}
	
	/** .DB <num8> [, <num8> [...]]
	 	encode the given bytes into the codeline.
	 */
	bool DirectiveBYTE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();

		while (parser.ExistMoreToken(1)) {
			ParseToken& token = parser.JumpNextToken();
			if (token.type == tokenTypeSTRING) {
				std::string result = parser.EvaluateString();
				// store each byte off the string
				for (auto c: result) {
					codeline.AddCode(c);
				}
			} else if (token.type == tokenTypeDECNUMBER) {
				ADDRESSTYPE address = parser.EvaluateAddress();// will only use 8-bit in fact
				codeline.AddCode(address);
			} else if (token.type == tokenTypeLETTERS && token.unsolved) {
				codeline.AddCode(0);
			}
			if (parser.ExistMoreToken(1)) {
				ParseToken& tokencomma = parser.NextToken(0);// (0) = current token 
				if (tokencomma.type == tokenTypeCOMMA) {
					continue;
				}
			}
			break;			
		}

		
		return true;
	}
	
	bool DirectiveWORD::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();
		
		while (parser.ExistMoreToken(1)) {
			ParseToken& token = parser.JumpNextToken();
			if (token.type == tokenTypeSTRING) {
				std::string result = parser.EvaluateString();
				// store each byte off the string
				for (auto c: result) {
					codeline.AddCode(c);
				}
			} else if (token.type == tokenTypeDECNUMBER) {
				ADDRESSTYPE address = parser.EvaluateAddress();
				codeline.AddCode(address & 0xFF, address >> 8);
			} else if (token.type == tokenTypeLETTERS && token.unsolved) {
				codeline.AddCode(0,0);
			}
			if (parser.ExistMoreToken(1)) {
				ParseToken& tokencomma = parser.NextToken(0);// (0) = current token
				if (tokencomma.type == tokenTypeCOMMA) {
					continue;
				}
			}
			break;
		}

		
		return true;
	}
	
	
	bool DirectiveSPACE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols();
		ParseToken& token = parser.NextToken();
		int size = token.asNumber();
		for (int i = 0 ; i < size ; i++) {
			codeline.AddCode(0xFF);
		}
		return true;
	}

} // namespace MUZ
