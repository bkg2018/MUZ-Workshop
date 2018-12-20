//
//  Asm-Exceptions.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 13/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Asm_Exceptions_h
#define Asm_Exceptions_h

#include <exception>

namespace MUZ {
	// wrong type for an operator in an expression
	class ASMOperandTypeException: public std::exception {
		virtual const char* what() const _NOEXCEPT{ return "ASM: wrong operand type for operator"; }
	};
	
	class PARSERNotInitialized: public std::exception {
		virtual const char *what() const _NOEXCEPT{ return "PARSER not initialized"; }
	};
	
	class PARSERInvalidTokenIndex: public std::exception {
		virtual const char *what() const _NOEXCEPT{ return "PARSER invalid token index"; }
	};

	class PARSERDivisionByZero: public std::exception {
		virtual const char *what() const _NOEXCEPT{ return "PARSER division by zero"; }
	};
}
#endif /* Asm_Exceptions_h */
