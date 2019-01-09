//
//  DefSymbol.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 01/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef DefSymbol_h
#define DefSymbol_h

#include <unordered_map>
#include "AddressZone.h"
#include "Name.h"
#include "Instruction.h"

namespace MUZ {
	
	/** Structure for a #DEFINE symbol. */
	class DefSymbol : public Instruction
	{
	public:
		//LineReference line; 	// file and line reference for this #DEFINE
		class CodeLine* codeline;
		std::string value;		// value of this symbol if it is defined by a string value
		bool singledefine = false; // true if this symbol is just defined with no value
	};
	
	typedef std::unordered_map<std::string, DefSymbol*> DefSymbolsMap; // Note: 'DefSymbolMap' (no 's' before map)  makes a strange bug in XCode: values are not added to the map
	
} // namespace MUZ
#endif /* DefSymbol_h */
