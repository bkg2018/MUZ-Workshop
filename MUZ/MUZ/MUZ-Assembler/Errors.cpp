//
//  Errors.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 14/01/2019.
//  Copyright © 2019 Francis Pierot. All rights reserved.
//

#include "Errors.h"
#include "CodeLine.h"
#include "Assembler.h"

namespace MUZ {
	using MUZ::ErrorKind;
	using MUZ::ErrorList;

	std::map<MUZ::ErrorKind,std::string> MUZ::ErrorList::messageText = {
		{ errorOK, "no error"},
		{ errorUnknown, "unknown error"},
		{ errorNonDerivedInstruction, "SHOULD NOT OCCUR: Non derived Instruction class used (fatal)"},
		{ errorNonDerivedDirective, "SHOULD NOT OCCUR: Non derived Directive class used (fatal)"},
		{ errorWritingListing, "Cannot write listing file (about file) "},
		{ errorOpeningSource, "Cannot open source file: asm, hex or binary file not found"},
		{ errorElseNoIf, "#ELSE without corresponding #IF/#IFDEF/#IFNDEF"},
		{ errorEndifNoIf, "#ENDIF without #ELSE or #IF"},
		{ errorLabelExists,	 "A label already exists with the same name"},
		{ errorUnknownSyntax, "line does not start with a label, a directive or an instruction"},
		{ errorUnknownDirective, "directive starting with '.' or '#' is unknown"},
		{ errorUknownInstruction, "an instruction should have been found, probable wrong syntax"},
		{ errorMUZNoSection, "SHOULD NOT OCCUR: assembled code has no section"},
		{ warningMisplacedChar, "a '.' or '#' was found in an unsusual place"},
		{ errorMissingComma, "a ',' is missing in instruction operands"},
		{ errorWrongOperand1, "first operand is wrong type"},
		{ errorWrongOperand2, "second operand is wrong type"},
		{ errorWrongOperand3, "third operand is wrong type"},
		{ errorWrongRegister, "register name is not valid"},
		{ errorMissingParenthesisClose, "a ')' is missing"},
		{ errorWrongCondition, "A condition is invalid (e.g. JR PO,nn)"},
		{ errorNotRegister, "expected register name was not found"},
		{ errorWrongComma, "unexpected comma"},
		{ errorDefine, "#DEFINE could not define a symbol"},
		{ errorInvalidSymbol,	 "invalid symbol name after DEFINE"},
		{ errorFileSyntax, "invalid syntax for file name"},
		{ errorProcessor, "unsupported processor in .PROC"},
		{ warningUnsolvedExpression, "a symbol was unsolved in an expression"},
		{ errorEquate, ".EQU could not create label or assign value"},
		{ errorTooBigValue, "number too big for accepted values"},
		{ errorTooBigBit,	 "number too big for a bit number (0-7)"},
		{ warningTooBig8, "number too big for 8 bits"},
		{ warningTooBig16, "number too big for 16 bits"},
		{ warningTooFar, "DJNZ or JR target is too far"},
	};

	void ErrorList::Clear() {
		clear();
	}
	std::string ErrorList::GetMessage( ErrorKind kind ) {
		return messageText[kind];
	}
	bool TestPass(CodeLine& codeline, int pass)
	{
		if (codeline.as == nullptr) return false;
		return pass == 1 ? codeline.as->IsFirstPass() : ! codeline.as->IsFirstPass();

	}
	void ErrorList::Info( ErrorKind kind, class CodeLine& codeline, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeINFO, kind, codeline.file, codeline.line, ""});
	}
	void ErrorList::Warning( ErrorKind kind, class CodeLine& codeline, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeWARNING, kind, codeline.file, codeline.line, ""});
	}
	void ErrorList::ForceWarning( ErrorKind kind, class CodeLine& codeline) {
		push_back({ errorTypeWARNING, kind, codeline.file, codeline.line, ""});
	}
	void ErrorList::AboutFile( ErrorKind kind, class CodeLine& codeline, std::string file, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeABOUTFILE, kind, codeline.file, codeline.line, file});
	}
	bool ErrorList::Error( ErrorKind kind, class CodeLine& codeline, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeERROR, kind, codeline.file, codeline.line, ""});
		return false;
	}
	bool ErrorList::Error( ErrorKind kind, class CodeLine& codeline, std::string file, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeERROR, kind, codeline.file, codeline.line, file});
		return false;
	}
	bool ErrorList::Fatal( ErrorKind kind, class CodeLine& codeline, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeFATAL, kind, codeline.file, codeline.line, ""});
		return false;
	}
	bool ErrorList::Fatal( ErrorKind kind, class CodeLine& codeline, std::string file, int pass) {
		if (TestPass(codeline,pass))
			push_back({ errorTypeFATAL, kind, codeline.file, codeline.line, file});
		return false;
	}
} // namespace
