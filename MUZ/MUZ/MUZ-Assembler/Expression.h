//
//  Expression.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 11/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Expression_h
#define Expression_h

#include <vector>
#include <map>

#include "TokenType.h"
#include "ExpVector.h"
#include "All-Operators.h"


namespace MUZ {

	/** Class for the expression evaluator. */
	class ExpressionEvaluator
	{
		/** Stack for arguments and operators. */
		ExpVector stack;
		
		/** Evaluates a non-parenthesed expression starting at a given token index. Updates the end index depending on the tokens erased and replaced by intermediate results. Tokens before the starting index are untouched. */
		ParseToken EvaluateExpression(ExpVector& tokens, int start, int& end);

		/** Reduces all parentheses sub-expressions to their result. Updates the end index depending on the tokens erased and replaced by intermediate results. Tokens before the starting index are untouched. */
		ParseToken ReduceParenthesis(ExpVector& tokens, int start, int& end);

		
	public:
		ExpressionEvaluator();
		
		/** Check parenthesis levels are paired. */
		bool CheckParenthesis(ExpVector& tokens);
		
		/** Evaluates a sub expression starting at a given token until end of tokens or invalid token type.
		 Returns a result and the last token used.
		 */
		ParseToken Evaluate(ExpVector& tokens, int start);
	};
	
} // namespace
#endif /* Expression_hpp */
