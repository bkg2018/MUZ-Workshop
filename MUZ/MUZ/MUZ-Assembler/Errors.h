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
		errorTypeABOUTFILE,	// Warning or error about a filename
		errorTypeERROR,		// doesn't generate code but can continue assembly with possibly wrong addresses
		errorTypeFATAL		// breaks assembly
	};
	
	struct ErrorMessage {
		ErrorType type;		// (see enum above): info, warning, about a file, serious error or fatal error
		std::string text;	// message to display
		class CodeLine* codeline;	// line number where it occured
	};
	
	class ErrorList : public std::vector<ErrorMessage>
	{

	};
	
} // namespace MUZ

#endif /* Errors_h */
