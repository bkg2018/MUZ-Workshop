//
//  Errors.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 04/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Errors_h
#define Errors_h

#include <vector>
#include <string>

namespace MUZ {
	enum ErrorType {
		errorTypeINFO,		// warmless informations
		errorTypeWARNING,	// doesn't break assembly
		errorTypeABOUTFILE,	// Warning or error about a filename (#include, #insertbin, #inserthex)
		errorTypeERROR,		// doesn't generate code but can continue assembly with possibly wrong addresses
		errorTypeFATAL		// breaks assembly
	};
	
	enum ErrorKind {
		errorOK,						// no error
		errorUnknown,					// unknown error
		errorInvalidSymbol,				// invalid symbol name after DEFINE
		errorDefine,					// #DEFINE could not define a symbol
		errorNonDerivedInstruction,		// SHOULD NOT OCCUR: Non derived Instruction class used (fatal)
		errorNonDerivedDirective,		// SHOULD NOT OCCUR: Non derived Directive class used (fatal)
		errorWritingListing,			// Cannot write listing file (about file)
		errorOpeningSource,				// "cannot open source file" asm, hex or binary file not found
	};
	
	struct ErrorMessage {
		ErrorType type=errorTypeINFO;			// (see enum above): info, warning, about a file, serious error or fatal error
		ErrorKind kind=errorUnknown;			// (see enum above): what kind of error it is
		class CodeLine* codeline=nullptr;		// code line where it occured
		std::string file="";					// file name for errorTypeABOUTFILE errors
	};
	
	class ErrorList : public std::vector<ErrorMessage>
	{

	};
	
} // namespace MUZ

#endif /* Errors_h */
