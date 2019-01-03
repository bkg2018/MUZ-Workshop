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
#include "Types.h"
#include "TokenType.h"

namespace MUZ {
	
	/** Class for a token. */
	struct ParseToken
	{
		std::string source;
		TokenType 	type;
		bool 		unsolved=false;// original source for unsolved labels at pass 1
		
		ADDRESSTYPE getAsAddress() {
			if (unsolved) return 0;
			return (ADDRESSTYPE)(atol(source.c_str()) & ADDRESSMASK);
		}
	};
} // namespace


#endif /* ParseToken_h */
