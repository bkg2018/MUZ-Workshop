//
//  TokenType.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 11/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef TokenType_h
#define TokenType_h

namespace MUZ {

	/** ParseToken type for parser. */
	enum TokenType {
		// transitionnal parsing token types
		tokenTypeUNKNOWN,	/// No token of this type can be stored
		tokenTypeLETTERS,	/// during parsing of a characters string but not knowing what king of token it will be
		tokenTypeDIGITS,	/// during parsing of a character string containing digits
		tokenTypeHEXNUMBER,	/// a "0x" prefixed, "$" prefixed or "h" suffixed number (- )before decimal translation)
		tokenTypeBINNUMBER,	/// a "b" suffixed binary number (- )before decimal translation)
		tokenTypeOCTNUMBER,	/// a "0" prefixed octal number - followed by at least one 0-7 digit (before decimal translation)
		tokenTypeFILENAME,	/// a filename after an #INCLUDE, #INSERTHEX, #INSERTBIN directive

		// final stored tokens
		tokenTypeCOMMENT,	/// ';' and all that follows on line
		tokenTypeDIRECTIVE,	/// a '.' or '#' directive
		tokenTypeSTRING,	/// a " delimited string
		tokenTypeCHAR,		/// a ' delimited character
		tokenTypeDECNUMBER,	/// a non prefixed and non suffixed decimal number
		tokenTypeCOMMA,		/// a ','
		tokenTypePAROPEN,	/// (
		tokenTypePARCLOSE,	/// )
		tokenTypeDOLLAR,	/// $ alone (not an hex number prefix)
		tokenTypeCOLON,		/// ':' ending a label definition
		tokenTypeBOOL,		/// for boolean values - this can only be written by expression evaluator
		
		// stored operators
		tokenTypeOP_LSHIFT,	/// <<
		tokenTypeOP_RSHIFT,	/// >>
		tokenTypeOP_DIFF,	/// <> and !=
		tokenTypeOP_EQUAL,	/// =
		tokenTypeOP_LT,		/// <
		tokenTypeOP_GT,		/// >
		tokenTypeOP_LTE,	/// <=
		tokenTypeOP_GTE,	/// >=
		tokenTypeOP_OR,		/// ||
		tokenTypeOP_AND,	/// &&
		tokenTypeOP_BINOR,	/// |
		tokenTypeOP_BINAND,	/// &
		tokenTypeOP_BINXOR,	/// ^
		tokenTypeOP_NOT,	/// !
		tokenTypeOP_PLUS,	/// +
		tokenTypeOP_MINUS,	/// -
		tokenTypeOP_MUL,	/// *
		tokenTypeOP_DIV,	/// \ /  
		tokenTypeOP_MOD,	/// %
		
		tokenTypeIGNORE,	/// used by parser to neutralize colons in label definitions
		
		tokenTypeLAST		/// used for priority array size
	};

}
#endif /* TokenType_h */
