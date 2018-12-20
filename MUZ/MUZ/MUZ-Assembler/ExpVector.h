//
//  ExpVector.hpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 12/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef ExpVector_h
#define ExpVector_h

#include <vector>
#include "ParseToken.h"

namespace MUZ {
	
	/** Specialized vector to store expression tokens and operators. It has a few extended functions useffull for 2 operands operators. */
	class ExpVector : public std::vector<ParseToken>
	{
		static ParseToken dummyToken;// = { "", tokenTypeUNKNOWN };
		// direct references for fast access
		ParseToken* prevBackToken = &dummyToken;
		ParseToken* backToken = &dummyToken;
		size_t		mysize = 0;		// shortcut
	public:
		/** Returns the last token on stack or an UNKNOWN type token if nothing on stack. */
		virtual ParseToken& back() { return *backToken; }
		/** Returns the token previous to last token on stack or an UNKNOWN type token if only one token on stack. */
		virtual ParseToken& prevBack() { return *prevBackToken; }
		/** Returns the number of tokens on stack before the given type. */
		virtual int tokensBefore(TokenType type) {
			int number = 0;
			for (int index = (int)mysize - 1 ; (index >= 0) && (at(index).type != type); index -= 1) {
				number ++;
			}
			return number;
		}
		/** Updates cached values */
		void update() {
			mysize = size();
			backToken = (mysize > 0) ? &at(mysize - 1) : &dummyToken;
			prevBackToken = (mysize > 1) ? &at(mysize - 2) : &dummyToken;
		}
		/** Pushes a token on stack. */
		virtual void push_back(ParseToken& token) {
			std::vector<ParseToken>::push_back(token);
			update();
		}
		/** Pops and returns the last token on stack. Returns an UNKNOWN type token if the stack is empty. */
		ParseToken pop_back() {
			ParseToken token = { "", tokenTypeUNKNOWN };
			if (mysize > 0) {
				token = back();
				std::vector<ParseToken>::pop_back();
				update();
			}
			return token;
		}
		/** Delete tokens from the stack. */
		bool erase(int start, int number) {
			if (start < 0) return false;
			int end = start + number;
			if (end > mysize) return false;
			std::vector<ParseToken>::erase(begin() + start, begin() + end);
			update();
			return true;
		}
	};

} // namespace
#endif /* ExpVector_h */
