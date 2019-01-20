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
#include "Section.h"

namespace MUZ {
	
	/** Returns true if current token is a comma, and go next token. */
	bool GetComma(CodeLine& codeline) {
		if (!EnoughTokensLeft(codeline,1)) return false;
		if (codeline.tokens.at(codeline.curtoken).type == tokenTypeCOMMA) {
			codeline.curtoken += 1;
			return true;
		}
		return false;
	}
	
	/** Returns true if the tokens array has at least the resquested number of tokens available starting at curtoken. */
	bool EnoughTokensLeft(CodeLine& codeline, int number) {
		// if curtoken is 'i' and we request 3 tokens,
		// then the tokens size must be 'i' + 3 or more
		return (codeline.tokens.size() >= codeline.curtoken + number) ;
	}
	

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
			return msg.Error(errorInvalidSymbol, codeline);
		}
		// skip directive
		parser.JumpTokens(1);
		// resolve expression parts
		parser.ResolveNextSymbols(false); // skips next symbol before resolving
		
		// undefine the symbol if it exists
		as.DeleteDefSymbol(symbol.source);
		// create the #DEFINE symbol
		std::string value;
		if (parser.ExistMoreToken(1)) {
			parser.JumpNextToken();// skip symbol
			// convert letters to strings
			try { parser.EvaluateString(value); }
			catch (const std::exception & e) {
				return msg.Error(errorInvalidExpression, codeline);
			}
		}
		DefSymbol* defsymbol = as.CreateDefSymbol(symbol.source, value);
		if (defsymbol) {
			defsymbol->codeline = &codeline;
			return true;
		}
		return msg.Error(errorDefine, codeline);
	}

	/** #UNDEF <symbol>
	 	returns true if an existing symbol has been undefined
	 */
	bool DirectiveUNDEFINE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		ParseToken& symbol = parser.NextToken();
		if (symbol.type != tokenTypeLETTERS) {
			return msg.Error(errorInvalidSymbol, codeline);
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
		try { parser.EvaluateString(symbol); }
		catch (const std::exception & e) {
			return msg.Error(errorInvalidExpression, codeline);
		}
		return as.ExistSymbol(symbol);
	}

	/** #IFNDEF <symbol>
	 	returns true if the symbol is not defined
	 */
	bool DirectiveIFNDEF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		std::string symbol;
		parser.JumpNextToken();
		try { parser.EvaluateString(symbol); }
		catch (const std::exception & e) {
			return msg.Error(errorInvalidExpression, codeline);
		}
		return ! as.ExistSymbol(symbol);
	}

	/** #IF <expression>
	 returns true if the expression is true (or not "" and not 0)
	 */
	bool DirectiveIF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if ( ! parser.ExistMoreToken(1) ) return false;
		parser.ResolveNextSymbols(true); // use '*' joker
		parser.JumpTokens(1);
		bool b = false;
		try { parser.EvaluateBoolean(b); }
		catch (const std::exception & e) {
			return msg.Error(errorInvalidExpression, codeline);
		}
		return b;
	}
	
	/** #INCLUDE <file>
	 	returns true if a file must be included
	 */
	bool DirectiveINCLUDE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols(false);
		ParseToken& filetoken = parser.NextToken();
		// and include new file
		if ((filetoken.type != tokenTypeSTRING) && (filetoken.type != tokenTypeFILENAME)) {
			return msg.Error(errorInvalidSymbol, codeline);
		}
		// trim spaces at the end of filename
		strtrimright(filetoken.source);
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
		parser.ResolveNextSymbols(false);
		ParseToken& filetoken = parser.NextToken();
		// and include new file
		if ((filetoken.type != tokenTypeSTRING) && (filetoken.type != tokenTypeFILENAME)) {
			return msg.Error(errorFileSyntax, codeline);
		}
		// trim spaces at the end of filename
		strtrimright(filetoken.source);
		if (as.IsFirstPass()) {
			codeline.includefile = (int)as.m_files.size();
		}
		if (as.AssembleHexFile(filetoken.source, codeline, msg)) {
		}
		
		// tells the parser that the file must be included
		parser.JumpTokens(1);
		return true;
	}
	/** #INSERTBIN <file>
	 */
	bool DirectiveINSERTBIN::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols(false);
		ParseToken& filetoken = parser.NextToken();
		// and include new file
		if ((filetoken.type != tokenTypeSTRING) && (filetoken.type != tokenTypeFILENAME)) {
			return msg.Error(errorFileSyntax, codeline);
		}
		// trim spaces at the end of filename
		strtrimright(filetoken.source);
		if (as.IsFirstPass()) {
			codeline.includefile = (int)as.m_files.size();
		}
		if (as.AssembleBinFile(filetoken.source, codeline, msg)) {
		}
		
		// tells the parser that the file must be included
		parser.JumpTokens(1);
		return true;
	}

	/** #NOLIST
	 */
	bool DirectiveLISTOFF::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		as.EnableListing(false);
		return true;
	}
	/** #LIST [ON|OFF]
	 */
	bool DirectiveLIST::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		bool enable = true;
		if (parser.ExistMoreToken(1)) {
			std::string value;
			parser.JumpNextToken();
			try { parser.EvaluateString(value); }
			catch (const std::exception & e) {
				return msg.Error(errorInvalidExpression, codeline);
			}
			enable = (std::to_upper(value) == "OFF") ? false : true;
		}
		as.EnableListing(enable);
		// always list this line if it is not #LIST OFF
		if (enable) codeline.listing = true;
		return true;
	}

	//MARK: - Assembler directives (.)
	
	/** .PROC Z80
	 	Sets the instructions set for the Z80 processor.
	 	returns true if Z80 has been specified
	 */
	bool DirectivePROC::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols(false);// allow string expressions
		ParseToken& proc = parser.NextToken();
		if (proc.source != "Z80") {
			return msg.Error(errorProcessor, codeline);
			}
		// sets the instructions set into the assembler
		as.SetInstructions(proc.source);
		parser.JumpTokens(1);
		return true;
	}
	
	/** [label:] .CODE [name]
	 */
	bool DirectiveCODE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg) {
		std::string name;
		if (parser.ExistMoreToken(1)) {
			parser.JumpNextToken();
			try { parser.EvaluateString(name); }
			catch (const std::exception & e) {
				return msg.Error(errorInvalidExpression, codeline);
			}
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
				try { parser.EvaluateString(name); }
				catch (const std::exception & e) {
					return msg.Error(errorInvalidExpression, codeline);
				}
			}
			if (GetComma(codeline)) {
				std::string param;
				try { parser.EvaluateString(param); }
				catch (const std::exception & e) {
					return msg.Error(errorInvalidExpression, codeline);
				}
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
		std::vector<int> unsolved = parser.ResolveNextSymbols(false);
		ADDRESSTYPE address = 0;
		parser.JumpTokens(1); // skip after .EQU
		try { parser.EvaluateAddress(address); }
		catch (const std::exception & e) {
			return msg.Error(errorInvalidExpression, codeline);
		}
		if (label == nullptr)
			label = codeline.label;
		if (label && as.IsFirstPass()) {
			label->SetAddress(address);
			codeline.label = label;
		}
		Section* section= as.GetSection();
		if (section) {
			section->SetOrg(address);
		}
		return true;
	}
	
	/** label[:]  [.]EQU <expression>
	 	returns true if the label has been created with the value as a decimal number
	 */
	bool DirectiveEQU::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		std::vector<int> unsolved = parser.ResolveNextSymbols(false);
		ADDRESSTYPE address = 0;
		if (unsolved.size() > 0 && ! as.IsFirstPass()) {
			msg.ForceWarning(warningUnsolvedExpression, codeline);
		}
		// compute address, unsolved symbols have been replaced by "0"
		parser.JumpTokens(1); // skip after .EQU
		try { parser.EvaluateAddress(address); }
		catch (const std::exception & e) {
			return msg.Error(errorInvalidExpression, codeline);
		}
		// use previous label if none on this line
		if (label == nullptr)
			label = codeline.label;
		// set label address/value
		if (label) {
			label->Equate(address);
			codeline.label = label;
			return true;
		}
		return msg.Error(errorEquate, codeline);
	}

	/** Identifies a source string as self. */
	bool DirectiveEQU::Identify( std::string source )
	{
		std::string upper = std::to_upper(source);
		if (upper==".EQU") return true;
		if (upper=="EQU") return true;
		return false;
	}
	
	/** .DB <num8> [, <num8> [...]]
	 	encode the given bytes into the codeline.
	 */
	bool DirectiveBYTE::Parse(class Assembler& as, Parser& parser, CodeLine& codeline, class Label* label, ErrorList& msg)
	{
		if (!parser.ExistMoreToken(1)) return false;
		parser.ResolveNextSymbols(false);

		while (parser.ExistMoreToken(1)) {
			ParseToken& token = parser.JumpNextToken();
			if (token.type == tokenTypeSTRING) {
				std::string result;
				try { parser.EvaluateString(result); }
				catch (const std::exception & e) {
					return msg.Error(errorInvalidExpression, codeline);
				}
				// store each byte off the string
				for (auto c: result) {
					codeline.AddCode(c);
				}
			} else if (token.type == tokenTypeDECNUMBER) {
				ADDRESSTYPE address;
				try { parser.EvaluateAddress(address); }
				catch (const std::exception & e) {
					return msg.Error(errorInvalidExpression, codeline);
				}
				if ((address > 255)  && ! as.IsFirstPass()) {
					msg.ForceWarning(warningTooBig8, codeline);
				}
				codeline.AddCode(address);
			} else if (token.type == tokenTypeLETTERS && token.unsolved) {
				codeline.AddCode(0);
				if ( ! as.IsFirstPass()) {
					msg.ForceWarning(warningUnsolvedExpression, codeline);
				}
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
		parser.ResolveNextSymbols(false);
		
		while (parser.ExistMoreToken(1)) {
			ParseToken& token = parser.JumpNextToken();
			if (token.type == tokenTypeSTRING) {
				std::string result;
				try { parser.EvaluateString(result); }
				catch (const std::exception & e) {
					return msg.Error(errorInvalidExpression, codeline);
				}
				// store each byte off the string
				for (auto c: result) {
					codeline.AddCode(c);
				}
			} else if (token.type == tokenTypeDECNUMBER) {
				ADDRESSTYPE address;
				try { parser.EvaluateAddress(address); }
				catch (const std::exception & e) {
					return msg.Error(errorInvalidExpression, codeline);
				}
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
		parser.ResolveNextSymbols(false);
		ParseToken& token = parser.NextToken();
		int size = token.asNumber();
		for (int i = 0 ; i < size ; i++) {
			codeline.AddCode(0xFF);
		}
		return true;
	}

} // namespace MUZ
