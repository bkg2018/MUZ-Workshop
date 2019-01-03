//
//  Expression.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 11/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "Expression.h"
#include <map>
#include "StrUtils.h"
#include "All-Operators.h"

namespace MUZ {
	
	/** Constructor in debug mode checks that the allOps array has the right operator for each index. This
	 	is not necessary for release code where the array will have been validated. */
	ExpressionEvaluator::ExpressionEvaluator()
	{
#ifdef DEBUG
		// make sure the operators are assigned in the right order
		for (TokenType type = tokenTypeUNKNOWN ; type < tokenTypeLAST ; type = (TokenType)(type + 1) ) {
			if (allOps[type].type != type) {
				assert("WRONG TYPE IN ALLOPS ARRAY");
			}
		}
#endif
	}


	
	/** Evaluates a non-parenthesed expression. There must be no parenthesis in the given array range tokens.
	 	Unary operators are managed first, then 2-operands operators by decreasing priority.
	 
	 	The tokens array range is reduced to one token holding the result, which is also returned.
	 */
	ParseToken ExpressionEvaluator::EvaluateExpression(ExpVector& tokens, int start, int& end)
	{
		ParseToken result;
		int i = start;
		if (end < 0) end = (int)tokens.size() - 1;
		while (i <= end) {
			TokenType type = tokens[i].type;
			// 5a-1) solve unary operators
			if (type == tokenTypeOP_NOT) {
				result = allOps[type].op->Exec(tokens[i+1], tokens[i+1]);// 5a-1a) compute op(i) with arg(i+1)
				tokens.erase(i+1, 1);// 5a-1b) delete argument
				end -= 1;
				tokens[i] = result;// 5a-1c) replace operator by result
			} else if (type == tokenTypeOP_MINUS) {
				ParseToken zero = {"0", tokenTypeDECNUMBER};
				result = allOps[type].op->Exec(zero, tokens[i+1]);
				tokens.erase(i+1, 1);
				end -= 1;
				tokens[i] = result;
			} else {
				// 5a-2) find next prioritary operator
				int found = -1;
				int priority = 9999;
				int index = i;
				while (index <= end) {
					type = tokens[index].type;
					if (allOps[type].priority < priority) {
						found = index;
						priority = allOps[type].priority;
					}
					index ++;
				}
				// did we find any operator?
				if (found < i)
					break;// finished, no more operators
				// 5a-2a) compute op(found) with arg(found-1) and arg(found+1)
				result = allOps[tokens[found].type].op->Exec(tokens[found-1], tokens[found+1]);
				// 5a-2b) delete op(found) and argument(found+1)
				tokens.erase(found, 2);
				end -= 2;
				// 5a-2c) replace arg(found-1) by result
				tokens[found-1] = result;
			}
			// 5a-3) loop until reduced to start = end
		} ;
		
		// the result should be the token at starting position
		return tokens[start];
	}
	
	/** Reduces all parentheses sub-expressions to their result. The tokens array is reduced to arguments and operators which
	 	form an expression with no parenthesis left. The end index is adjusted accrodingly.
	 */
	ParseToken ExpressionEvaluator::ReduceParenthesis(ExpVector& tokens, int start, int& end)
	{
		ParseToken result;
		
		int i = start;
		// 4a) look for PAROPEN

		// 4b) if no more tokens, finished
		while (i <= end) {
			
			// 4c) if paropen at [i], look for PARCLOSE starting at [i+1]
			if (tokens[i].type == tokenTypePAROPEN) {
				
				int j = i + 1;
				while (j <= end) {
					// 4c-1) if we find a PAROPEN we recurse starting at current index
					if (tokens[j].type == tokenTypePAROPEN) {
						result = ReduceParenthesis(tokens, j, end);
					}
					// 4c-3) if PARCLOSE, evaluate the expression between previous PAROPEN and this PARCLOSE
					else if (tokens[j].type == tokenTypePARCLOSE) {
				
						// 4c-4) compute expession from i+1 to j-1
						int last = j-1;
						int prevlast = last;
						result = EvaluateExpression(tokens, i+1, last);
						// update end to match what expression may have deleted
						end -= (prevlast - last);
						// 4c-5) delete i+1 (argument after paropen) to last (parclose)
						int todelete = last-i+1;
						tokens.erase(i+1, todelete);
						end -= todelete;
						// 4c-6) replace paropen by result
						tokens[i] = result;
						// 4c-7) continue PAROPEN loop
						break;
					}
					j += 1; // next parclose lookup
				} // while j <= end (and parclose not found)
			}
			i += 1; // next paropen lookup
		}
		
		return result;
	}
	
	/** Evaluates a series of tokens as an expression
	 @param tokens the vector holding all tokens from a line
	 @param start the first token to use for evaluation
	 @return a token containing the result
	 */
	ParseToken ExpressionEvaluator::Evaluate(ExpVector& tokens, int start, int& end)
	{
		// reset stack
		stack.clear();
		
		// 1c) build working copy, stops at first non expression token (comma ...)
		bool done = false;
		int curtoken = start;
		if (end == -1) {
			end = (int)tokens.size() - 1;
		}
		do {
			ParseToken& token = tokens.at(curtoken);
			switch( token.type ) {
					
				case tokenTypePAROPEN:
				case tokenTypeBOOL:			// <- bool should never happen, this is just in case
				case tokenTypeSTRING:		// explicitely enquoted strings
				case tokenTypeCHAR:			// 'c'
				case tokenTypeLETTERS: 		// <- letters asked for evaluation are considered as string
				case tokenTypeDECNUMBER:
				case tokenTypePARCLOSE:
				case tokenTypeOP_LSHIFT:
				case tokenTypeOP_RSHIFT:
				case tokenTypeOP_DIFF:
				case tokenTypeOP_EQUAL:
				case tokenTypeOP_LT:
				case tokenTypeOP_GT:
				case tokenTypeOP_LTE:
				case tokenTypeOP_GTE:
				case tokenTypeOP_OR:
				case tokenTypeOP_AND:
				case tokenTypeOP_BINOR:
				case tokenTypeOP_BINAND:
				case tokenTypeOP_BINXOR:
				case tokenTypeOP_NOT:
				case tokenTypeOP_PLUS:
				case tokenTypeOP_MINUS:
				case tokenTypeOP_MUL:
				case tokenTypeOP_DIV:
				case tokenTypeOP_MOD:
					stack.push_back(token);
					break;
					
				default:
					// non recognized token types put an end to expression
					done = true;
					break;
					
			};
			// next token
			if (!done) curtoken += 1;
			
		} while (!done && (curtoken <= end));
		
		
		// 1) check parenthesiss levels
		if (stack.size() > 0 && CheckParenthesis(stack)) {
		
			//2a) reduce parenthesis
			int stackend = (int)stack.size() - 1;
			ReduceParenthesis(stack, 0, stackend);

			// 2b) compute expression
			stackend = -1;
			ParseToken result = EvaluateExpression(stack, 0, stackend);
			
			// compute the next token index
			end = curtoken;
			return result;
		}
		
		//TODO: parenthesis error
		return nop;
	}

	/** Check parenthesis levels are paired. */
	bool ExpressionEvaluator::CheckParenthesis(ExpVector& tokens)
	{
		int level = 0;
		for (auto token : tokens) {
			if (token.type == tokenTypePAROPEN) {
				level += 1;
			} else if (token.type == tokenTypePARCLOSE) {
				level -= 1;
			}
			if (level < 0) break;// checks if one ')' too much
		}
		return level == 0; // checks '(' or ')' too much
	}
	
}// namespace
