//
//  ParseToken.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 11/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef ParseToken_h
#define ParseToken_h

#include <string>
#include "MUZ-Common/Types.h"
#include "MUZ-Common/StrUtils.h"
#include "TokenType.h"

namespace MUZ {
	
	/** Class for a token. */
	struct ParseToken
	{
		std::string source;
		TokenType 	type = tokenTypeUNKNOWN;
		bool 		unsolved=false;// original source for unsolved labels at pass 1
		
		bool isIncludingDirective() {
			return (type == tokenTypeDIRECTIVE) && ((source == "#INCLUDE") || (source == "#INSERTHEX") || (source == "#INSERTBIN"));
		}

		/** Convert to a number depending on type and value.
		 	Empty and non-convertible tokens will return 0.
		 	Character will return its first character code.
		 	Hexa, decimal, binary and octal numbers will return their value
		 	String will return the appropriate values for accepptable prefixes (0x, 0b, 0) and suffixes (H, B)
		 */
		ADDRESSTYPE asNumber() {
			if (source.empty()) return 0;
			if (type <= tokenTypeFIRSTCONVERTIBLE || type >= tokenTypeLASTCONVERTIBLE) return 0;
			switch (type) {
				case tokenTypeHEXNUMBER:	return hex_to_unsigned(source);
				case tokenTypeBINNUMBER:	return bin_to_unsigned(source);
				case tokenTypeOCTNUMBER:	return oct_to_unsigned(source);
				case tokenTypeDECNUMBER:	return dec_to_unsigned(source);
				case tokenTypeBOOL:			return source.empty() ? -1 : 0;
				case tokenTypeCHAR:			return source[0];
				default:
					// 0x or 0b prefix?
					if (source.size() > 2 && source[0]=='0') {
						if (source[1]=='x' ||source[1]=='X') return hex_to_unsigned(source.substr(2));
						if (source[1]=='b' || source[1]=='B') return bin_to_unsigned(source.substr(2));
					}
					// 0octal?
					if (source.size() > 1 && source[0]=='0') {
						if (isOctal(source.substr(1))) return oct_to_unsigned(source.substr(1));
					}
					if (source.size() <= 1) return dec_to_unsigned(source);
					std::string start = source.substr(0, source.length() - 1);
					char lastchar = source[source.length()-1];
					// H suffix?
					if ((lastchar=='h'|| lastchar=='H') && isHexa(start)) 	return hex_to_unsigned(start);
					// B suffix?
					if ((lastchar=='b'|| lastchar=='B') && isBinary(start)) return bin_to_unsigned(start);
			};
			// default for anything else
			return dec_to_unsigned(source);
		}
	};
} // namespace


#endif /* ParseToken_h */
