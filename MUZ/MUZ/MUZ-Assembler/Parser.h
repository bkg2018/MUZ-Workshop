//
//  Parser.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Token_hpp
#define Token_hpp

#include "Types.h"
#include "Errors.h"
#include "ParseToken.h"
#include "ExpVector.h"
#include "CodeLine.h"
#include "Asm-Exceptions.h"

namespace MUZ {
	

	
	
	/** shortcuts for interesting directives. */
	enum ResultFlag {
		hasNOTHING			= 0,
		hasIF  				,			// there is an #IF #IFDEF or #IFNDEF directive
		hasELSE 			,			// there is an #ELSE directive
		hasENDIF 			,			// there is an #ENDIF directive
		hasINCLUDE			,			// there is an #INCLUDE directive
		hasINSERTHEX		,			// there is an INSERTHEX directive
	} ;
	
	/** Current parsing status. */
	enum ParsingStatus {
		inNothing			= 0,		// current pos is in nothing special
		inLetters			,			// in an alphabetical sequence
		inDigits			,			// in a sequence of binary, decimal, octal or hexa digits
		inDoubleQuotes		,			// in a double quoted string
		inSingleQuotes		,			// in a single quoted string
		inSpace				,			// in a sequence of white space
		inDirective			,			// in a '.' or "#' directive name
		inFilename			,			// in a filename following an #INCLUDE directive
	} ;

	
	/** Structure for the code line parser.  It works on one line at a time. */
	class Parser {
		
		// parsing status variables for current code line
		int pos = 0;									/// current position in source
		char c = '\0';									/// current character
		char upperc = '\0';								/// current character uppercase (identical if not a letter)
		bool hasNext = true;							/// tell if there is a next character (false at the last character)
		char nextc = '\0';								/// next character in string - if hasNext is true
		char uppernextc = '\0';							/// next character in string in uppercase - if hasNext is true
		ParsingStatus status = inNothing;				/// main parsing status
		ResultFlag resultFlag = hasNOTHING;				/// shortcut flag for the most significant directives
		std::string word;								/// cumulated characters for current token string
		TokenType type = tokenTypeUNKNOWN;				/// current token type
		bool doubleQuoted = false;						/// true while parsing a string between double quotes, also works for filenames
		
		// shortcuts to usefull objects
		class Directive* lastDirective = nullptr;		/// Direct directive access for conditionnal and including directives
		class Assembler* as = nullptr;					/// Direct assembler access
		
		/** Points to the parsing result. Each parsed token will be pushed in this result. */
		ExpVector* result = nullptr;
		/** Points to the current token variable from caller. */
		int* curtoken = nullptr;
		/** Points to the original source string. */
		std::string* source;


		/** Checks if current and next characters can combine into the given operator and if so, store it as token and goes forward. */
		bool find2CharsOperator(std::string token, TokenType tokentype);
		
		/** Checks if current character match the given operator and if so, stores it as token and goes forward. */
		bool findOperator(char token, TokenType tokentype);
		
		/** Checks if current and next characters are hex digits only until space.
		 @param skip number of character to ignore including the ciurrent one
		 @return true if a possible hex number has been detected
		 */
		bool findHexNumberNoSuffixSkip(int skip );
		/** Checks if current and next characters are hex digits only until 'h' suffix.
		 @param skip number of character to ignore including the current one
		 @return true if a possible hex number has been detected
		 */
		bool findHexNumberWithSuffixSkip(int skip );
		/** Checks if current and next characters are decimal digits ('0'-'9') only until space. Set skip to ignore current character and some more if needed. */
		bool findDecimalNumberSkip(int skip );
		/** Checks if current and next characters are octal digits ('0'-'7') only until space. Set skip to ignore current character and some more if needed. */
		bool findOctalNumberSkip(int skip );
		/** Checks if current and next characters are binary digits ('0'-'1') only until space. Set skip to ignore current character and some more if needed. */
		bool findBinaryNumberSkip(int skip );

		/** Stores a new token given current string and type.
		 In some cases no token will be added, e.g. a number with empty content.
		 In any case, the parsing status is prepared for next token by reseting
		 status and clearing the given string.
		 */
		void StoreToken();
	
	public:
		
		Parser(class Assembler& assembler) {
			result = nullptr;
			curtoken = nullptr;
			as = &assembler;
		}
		
		/** Init to work on a vector of tokens.*/
		void Init(ExpVector& tokens, int& starttoken) {
			result = &tokens;
			curtoken = &starttoken;
			*curtoken = 0;
			tokens.clear();
			resultFlag = hasNOTHING;
		}
		
		/** Test particular results (directives). */
		bool Test(ResultFlag f) { return resultFlag == f; }
		
		/** Tells if there are more tokens after the current one. */
		bool ExistMoreToken(int howmany) {
			return (*curtoken + howmany < result->size());
		}
		
		/** Returns the next token. */
		ParseToken& NextToken(int increment = 1) {
			return (*result)[*curtoken + increment];
		}

		/** Jumps over next tokens, done by the directives or assembler after arguments have been treated. */
		void JumpTokens(int increment) {
			*curtoken += increment;
		}
		
		/** Jumps to next token and returns it. */
		ParseToken& JumpNextToken() {
			*curtoken += 1;
			return (*result)[*curtoken];
		}
		
		/** Returns the current token index. Notice that this token is external to the parser and was set by Init() call. */
		int GetCurrentTokenIndex() {
			return *curtoken;
		}
		
		/** Resolve symbol, equates and labels starting in a given token. */
		bool ResolveSymbolAt(int index);
		
		/** Resolve symbols, equates and labels starting at given token index. If no start index is given, will start at current token.
		 Returns a list of unsolved label tokens.
		 */
		std::vector<int> ResolveSymbols(int start = -1);
		
		/** Resolve symbols, equates and labels starting at next token. */
		std::vector<int> ResolveNextSymbols() {
			return ResolveSymbols( *curtoken + 1 );
		}
		
		/** Evaluate next tokens to produce a boolean result. */
		bool EvaluateBoolean();

		/** Evaluate next tokens to produce a string result. Operands must be strings, but automatic conversion will happen on
		 	decimal numbers and booleans.
		 */
		std::string EvaluateString();
		
		/** Evaluate nexxt tokens to produce an integer number masked by the address size.
		 */
		ADDRESSTYPE EvaluateAddress();

		/** Cuts a string into a vector of tokens.
		 Spaces and tabs are not stored in tokens, only the significant parts are stored.
		 A vector of warnings can be returned.
		 */
		void Split(std::string s, ErrorList& msg);
		
		/** Execute the last directive and returns its result: this is used by IF directives called from CodeLine.Assemble() to choose
		 the parsing mode. */
		bool LastDirective(CodeLine& codeline, ErrorList& msg);
		
	}; // class Parser
}//namespace MUZ

#endif /* Token_h */
