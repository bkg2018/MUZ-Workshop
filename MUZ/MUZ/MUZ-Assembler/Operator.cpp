//
//  Operator.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 12/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "Operator.h"

namespace MUZ {
	
	Operator nop;
	
	/** Execute the operation with the given arguments and return result. Argument type must have been checked first. */
	ParseToken Operator::Exec(ParseToken& arg1, ParseToken& arg2)
	{
		return nop;
	}
}
