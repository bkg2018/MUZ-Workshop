//
//  ParseToken.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "StrUtils.h"
#include "Parser.h"
#include "Expression.h"
#include "CodeLine.h"
#include "Assembler.h"

#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

namespace MUZ {


	//MARK: private functions
	
	/** Checks if current and next characters can combine into the given operator and if so, store it */
	bool Parser::find2CharsOperator(std::string token, TokenType tokentype) {
		if (c == token.at(0) && hasNext && nextc == token.at(1)) {
			StoreToken();
			type = tokentype;
			word = token;
			StoreToken();
			pos += 1; // skip the second character
			return true;
		}
		return false;
	}
	
	/** Checks if current character match the given operator and if so, store it */
	bool Parser::findOperator(char token, TokenType tokentype) {
		if (c == token) {
			StoreToken();
			type = tokentype;
			word = token;
			StoreToken();
			return true;
		}
		return false;
	}
	
	/** Checks if current and next characters are hex digits only until space or 'h' suffix. Set skip to ignore current character and some more if needed. */
	bool Parser::findHexNumberSkip(int skip ) {
		int nextpos = pos + skip - 1; // incremented at first loop
		bool onlyhex = true;
		char nextc=0;
		while (onlyhex && (nextpos < source->size()-1)) {
			nextpos += 1;
			nextc = source->at(nextpos);
			char nextupperc = upperchar(c);
			if (nextc >= '0' && nextc<= '9') continue;
			if (nextupperc >= 'A' && nextupperc <= 'F') continue;
			if (nextupperc == 'H') return true; // hexa suffix
			return false; // not an hexa digit or suffix
		}
		return onlyhex; // all hexx digits and no sufffix
	}
	
	/** Stores a new token given a string and a type.
	 In some cases no token will be added, e.g. a number with empty content.
	 In any cases, the parsing status is prepared for next token by reseting
	 status and clearing the given string.
	 */
	void Parser::StoreToken() {
		if (status != inSpace) {
			// unspecified digits sequence must be decimal
			if (type==tokenTypeDIGITS)
				type = tokenTypeDECNUMBER;
			// some types don't go with empty content
			if (word.empty()) {
				if (type==tokenTypeUNKNOWN) return;
				if (type==tokenTypeLETTERS) return;
				if (type==tokenTypeCOMMENT) return;
				if (type==tokenTypeDIRECTIVE) return;
				if (type==tokenTypeSTRING) return;
				if (type==tokenTypeHEXNUMBER) return;
				if (type==tokenTypeBINNUMBER) return;
				if (type==tokenTypeDECNUMBER) return;
			}
			
			// normalize file name for #INCLUDE
			if (type == tokenTypeFILENAME) {
				string filepath, filename;
				splitpath(word, filepath, filename);
				word = filepath + NORMAL_DIR_SEPARATOR + filename;
			}
			
			// Store the token, use upper case for directives
			if (type == tokenTypeDIRECTIVE) {
				word = to_upper(word);
			}
			ParseToken token;
			token.source = word;
			token.type = type;
			result->push_back(token);// store current value
			
			// set result flags for some directives and handle special #INCLUDE case
			if (token.type == tokenTypeDIRECTIVE) {
				resultFlag = hasNOTHING;
				lastDirective = as->GetDirective(token.source);
				if (token.source == "#IF" || token.source == "#IFDEF" || token.source == "#IFNDEF") resultFlag = hasIF;
				else if (token.source == "#ELSE") resultFlag = hasELSE;
				else if (token.source == "#ENDIF") resultFlag = hasENDIF;
				else if (token.source == "#INCLUDE") {
					resultFlag = hasINCLUDE;
					// special case: the rest of source is a filename even if no quotes surrounds it
					// skip white space
					size_t len = source->length();
					while (pos < len && (source->at(pos) == ' ' || source->at(pos) == '\t')) {
						pos++;
					}
					// handle quotes
					if (source->at(pos) == '"' || source->at(pos) == '\'') {
						status = inSpace;
						pos -= 1;
						word.clear();
						type = tokenTypeUNKNOWN;
						return;
					}
					// setup filename parsing
					status = inFilename;
					word = source->at(pos);
					type = tokenTypeFILENAME;
					return;
				}
				else if (token.source == "#INSERTHEX") resultFlag = hasINSERTHEX;
			}
		}
		word.clear();
		type = tokenTypeUNKNOWN;
		status = inNothing;
	}
	
	
	
	// publics
	/** Resolve symbol, equates and labels starting in a given token. */
	void Parser::ResolveSymbolAt(int index)
	{
#ifdef DEBUG
		if (!curtoken || !result) throw PARSERNotInitialized();
		if (index < 0 || index > result->size()) throw PARSERInvalidTokenIndex();
#endif
		auto &token = result->at(index);
		// This only occurs on the LETTER type tokens
		if (token.type == tokenTypeLETTERS) {
			if (as->ReplaceDefSymbol(token))
				return;
			if (as->ReplaceLabel(token.source)) {
				token.type = tokenTypeDECNUMBER;
				return;
			}
		}
		// replace "$" current address if found
		if (token.type == tokenTypeDOLLAR) {
			token.source = std::to_string(as->GetAddress());
			token.type = tokenTypeDECNUMBER;
			return;
		}
		// translate hex numbers
		if (token.type == tokenTypeHEXNUMBER) {
			unsigned int uint = hex_to_unsigned(token.source);
			token.source = std::to_string(uint);
			token.type = tokenTypeDECNUMBER;
			return;
		}
		// translate binary numbers
		if (token.type == tokenTypeBINNUMBER) {
			unsigned int uint = bin_to_unsigned(token.source);
			token.source = std::to_string(uint);
			token.type = tokenTypeDECNUMBER;
			return;
		}
		// translate octal numbers
		if (token.type == tokenTypeOCTNUMBER) {
			unsigned int uint = oct_to_unsigned(token.source);
			token.source = std::to_string(uint);
			token.type = tokenTypeDECNUMBER;
			return;
		}
		// translate escape sequences in strings
		if (token.type == tokenTypeSTRING) {
			token.source = unescape(token.source);
		}
	}
	
	/** Resolve symbols, equates and labels starting at given token index. if no start index is given, will start at current token */
	void Parser::ResolveSymbols(int start /* = -1 */)
	{
		// next token if no start
		if (start == -1) start = *curtoken;
		// replace existing symbols, EQU values, labels by their value
		for (int i = start ; i < result->size() ; i++) {
			ResolveSymbolAt(i);
		}
	}
	
	/** Cuts a string into a vector of typed tokens. Each token has a type but is not interpreted, so
	 symbols, labels or equates are not replaced by their values. Numbers can be decimal, octal, binary:
	 they don't have any prefix or suffix, only a type.
	 Spaces and tabs are not stored in tokens.
	 A vector of warnings and errors is returned.
	 */
	void Parser::Split(std::string s, ErrorList& msg)
	{
		source = &s;
		pos = -1;								// current parsing position in string
		const int len = (int)s.length();		// explicitely signed because unsigned would fail the test (pos < len)
		word.empty();							// current parsed word
		type = tokenTypeUNKNOWN;				// current token type
		
		msg.clear();							// clear warnings
		
		result->clear();						// clear results
		*curtoken = 0;
		
		// Parse the string, concatenating characters in 'word' until a token can be stored.
		while (pos + 1 < len) {
			
			// Get next character and keep an upper-case equivalent for letters.
			pos += 1;
			c = s[pos];
			upperc = upperchar(c);
			
			// NOTICE: each of the following tests ends either with a break if the string parsing is finished,
			// either with a continue to check the next character. Each test can be thought as either a try to detect
			// a new token, either as a way to continue to store its string content character by character.
			// Spaces and tabulations are parsed too, but they will not be stored.
			// Each test which starts a new token first does a StoreToken(word, type) to store
			// the previous token if there was one being parsed
			// some ambiguous specific cases are handle either in StoreToken(), either in the parsing loop:
			//		- #INCLUDE accepts a non quoted string
			//      - hexa numbers can use letter digits followed by a 'h' suffix
			
			// include filenames have absolute priority over anything else
			if (status == inFilename) {
				word += c;
				continue;
			}

			// comment?
			if (c == ';') {
				StoreToken();
				word = s.substr(pos); // add the rest of line as a comment
				type = tokenTypeCOMMENT;
				StoreToken();
				break; // finished!
			}
			// end of label?
			if (c == ':') {
				StoreToken();
				type = tokenTypeCOLON;
				StoreToken();
				continue;
			}
			
			// Directive prefix?
			if (c == '.' || c == '#') {
				if (word.empty()) {
					// nothing stored yet, start a directive
					status = inDirective;
					word = c;
					type = tokenTypeDIRECTIVE;
					continue;
				}
				// we're not  supposed to reach here: would mean a '.' or '#' contained in a word
				// so just keep going and emit a warning
				word += c;
				//TODO: warnings.push_back(std::string("A dubious '") + c + "' was found in a sequence of characters");
				continue;
			}
			// double quote running?
			if (status == inDoubleQuotes) {
				if (c == '"') {
					StoreToken();
					continue; // finished the double quoted string
				}
				// else continue to add
				word += c;
				continue;
			}
			// single quote running?
			if (status == inSingleQuotes) {
				if (c == '\'') {
					StoreToken();
					continue; // finished the single quoted string
				}
				// else continue to add
				word += c;
				continue;
			}
			// start double quotes?
			if (c == '"') {
				StoreToken();
				status = inDoubleQuotes;
				type = tokenTypeSTRING;
				continue;
			}
			// start single quotes?
			if (c == '\'') {
				StoreToken();
				status = inSingleQuotes;
				type = tokenTypeSTRING;
				continue;
			}
			hasNext = (pos + 1 < s.length());
			nextc = hasNext ? s[pos + 1] : '\0';
			uppernextc = upperchar(nextc);
			// "//" comments?
			if ((c == '/') && hasNext && (nextc == '/')) {
				StoreToken();
				word = s.substr(pos); // add the rest of line as a comment
				type = tokenTypeCOMMENT;
				StoreToken();
				continue;
			}
			// "0x" prefix of hex numbers?
			if ((status != inDigits) && (c == '0') && hasNext && (uppernextc == 'X')) {
				// start an hex number
				StoreToken();
				status = inDigits;
				type = tokenTypeHEXNUMBER;
				pos += 1; // skip "x"
				continue;
			}
			// "0b" prefix of binary numbers?
			if ((status != inDigits) && (c == '0') && (uppernextc == 'B')) {
				// start a binary number
				StoreToken();
				status = inDigits;
				type = tokenTypeBINNUMBER;
				pos += 1; // skip "B"
				continue;
			}
			// "0" prefix on octal numbers
			if ((status != inDigits) && (c == '0') && (nextc >= '0') && (nextc <= '7')) {
				StoreToken();
				status = inDigits ;
				type = tokenTypeOCTNUMBER;
				continue;
			}

			// H suffix after digits?
			if ((status == inDigits) && (upperc == 'H')) {
				type = tokenTypeHEXNUMBER;
				StoreToken();
				continue;
			}
			// B suffix after digits?
			if ((status == inDigits) && (upperc == 'B')) {
				type = tokenTypeBINNUMBER;
				StoreToken();
				continue;
			}

			// continue digit sequence?
			bool decdigit = (c >='0') && (c <= '9');
			bool hexdigit = (upperc >= 'A') && (upperc <= 'F');
			if ((status == inDigits) && (decdigit || hexdigit)) {
				word += c;
				continue;
			}
			
			// "$" prefix of hex numbers?
			if (word == "$") {
				if (findHexNumberSkip(0)) {
					// forget the "$" and start an hex number
					word = c;
					status = inDigits;
					type = tokenTypeHEXNUMBER;
					continue;
				}
				// $ not followed by hex = current assembling address, store a dollar token
				word.clear();
				type = tokenTypeDOLLAR;
				StoreToken();
				//TODO: what happens to the current character after the "$" ?
				word = c;
				type = tokenTypeLETTERS; // default type
				continue;
			}
			
			// digits? always starts with a 0-9 digit and can be followed by dec/ex digits
			if (word.empty() && (decdigit || ((status == inDigits) && hexdigit))) {
				// start new digits sequence?
				if (status != inDigits) {
					StoreToken();
					type = tokenTypeDIGITS;
					status = inDigits;
				}
				word += c;
				continue;
			}
			
			// Special case: we found an hex letter-digit first, but no prefix: check if the following are only hex digits and 'h' prefix
			if (word.empty() && (status != inDigits) && hexdigit) {
				if (findHexNumberSkip(1)) {
					StoreToken();
					word = c;
					status = inDigits;
					type = tokenTypeHEXNUMBER;
					continue;
				}
			}
			
			// space?
			if (c == ' ' || c == '\t') {
				if (status == inSpace) continue;
				StoreToken();
				if (status != inFilename)
					status = inSpace;
				continue;
			}
			
			// parenthesis?
			if (c == '(') {
				StoreToken();
 				word = c;
				type = tokenTypePAROPEN;
				StoreToken();
				continue;
			}
			if (c == ')') {
				StoreToken();
				word = c;
				type = tokenTypePARCLOSE;
				StoreToken();
				continue;
			}
			// comma?
			if (c == ',') {
				StoreToken();
				word = c;
				type = tokenTypeCOMMA;
				StoreToken();
				continue;
			}
			
			// 2 characters operators?
			if (find2CharsOperator("<<", tokenTypeOP_LSHIFT))
				continue;
			if (find2CharsOperator(">>", tokenTypeOP_RSHIFT))
				continue;
			if (find2CharsOperator("<>", tokenTypeOP_DIFF))
				continue;
			if (find2CharsOperator("!=", tokenTypeOP_DIFF))
				continue;
			if (find2CharsOperator("==", tokenTypeOP_EQUAL))
				continue;
			if (find2CharsOperator("<=", tokenTypeOP_LTE))
				continue;
			if (find2CharsOperator(">=", tokenTypeOP_GTE))
				continue;
			if (find2CharsOperator("&&", tokenTypeOP_AND))
				continue;
			if (find2CharsOperator("||", tokenTypeOP_OR))
				continue;

			// single character operators?
			if (findOperator('<', tokenTypeOP_LT))
				continue;
			if (findOperator('>', tokenTypeOP_GT))
				continue;
			if (findOperator('=', tokenTypeOP_EQUAL))
				continue;
			if (findOperator('+', tokenTypeOP_PLUS))
				continue;
			if (findOperator('-', tokenTypeOP_MINUS))
				continue;
			if (findOperator('*', tokenTypeOP_MUL))
				continue;
			if (findOperator('/', tokenTypeOP_DIV))
				continue;
			if (findOperator('%', tokenTypeOP_MOD))
				continue;
			if (findOperator('!', tokenTypeOP_NOT))
				continue;
			if (findOperator('&', tokenTypeOP_BINAND))
				continue;
			if (findOperator('|', tokenTypeOP_BINOR))
				continue;
			if (findOperator('^', tokenTypeOP_BINXOR))
				continue;
			
			// nothing particular has been found, it must be a running sequence or a new text sequence
			
			// decimal digits?
			//TODO: not sure of this
			if ((status == inDigits) && !decdigit) {
				// hex and binary suffixes have been checked already, so current token should have been a decimal number
				type = tokenTypeDECNUMBER;
				StoreToken();
				// start a letters sequence
				word = c;
				type = tokenTypeLETTERS;
				continue;
			}
			
			// continue running sequence
			if (status == inLetters || status == inDirective) {
				word += c;
				continue;
			}
			
			// start a new text sequence
			StoreToken();
			type = tokenTypeLETTERS;
			word = c;
			status = inLetters;
		}
		
		// store last current token if any
		StoreToken();
	}

	/** Execute the last directive and returns its result: this is used by IF directives called from CodeLine.Assemble() to choose
	 the parsing mode, or INCLUDE to asssemble a child source file. */
	bool Parser::LastDirective(CodeLine& codeline, ErrorList& msg)
	{
		if (!lastDirective) return false;
		// set curtoken on the directive token so the directive have access to its arguments
		*curtoken = 0;
		while (*curtoken < result->size()) {
			if ((*result)[*curtoken].type == tokenTypeDIRECTIVE)
				break;
			*curtoken += 1;
		};
		// and call the directive
		return lastDirective->Parse(*as, *this, codeline, nullptr, msg);
	}
	
	/** Evaluate next tokens to produce a boolean result. */
	bool Parser::EvaluateBoolean()
	{
		ExpressionEvaluator eval;
		ParseToken evaluated = eval.Evaluate(*result, *curtoken);
		if (evaluated.type == tokenTypeDECNUMBER) {
			return dec_to_unsigned(evaluated.source) != 0;
		}
		if ((evaluated.type == tokenTypeBOOL) || (evaluated.type == tokenTypeSTRING)) {
			return !evaluated.source.empty();
		}
		return false;
	}
	
	/** Evaluate next tokens to produce a string result. Operands must be strings, but automatic conversion will happen on
	 decimal numbers and booleans.
	 */
	std::string Parser::EvaluateString()
	{
		ExpressionEvaluator eval;
		ParseToken evaluated = eval.Evaluate(*result, *curtoken);
		if ((evaluated.type == tokenTypeSTRING) || (evaluated.type == tokenTypeDECNUMBER)) {
			return evaluated.source;
		}
		if (evaluated.type == tokenTypeBOOL) {
			return evaluated.source.empty() ? "" : "t";
		}
		return "";
	}
	
	/** Evaluate next tokens to produce an integer number masked by the address size.
	 */
	ADDRESSTYPE Parser::EvaluateAddress()
	{
		ExpressionEvaluator eval;
		ParseToken evaluated = eval.Evaluate(*result, *curtoken);
		if ((evaluated.type == tokenTypeSTRING) || (evaluated.type == tokenTypeDECNUMBER)) {
			return dec_to_unsigned(evaluated.source);
		}
		//TODO: generate an error when EvaluateAddress() doesn't get a string or number
		return 0;
	}
} // namespace MUZ
