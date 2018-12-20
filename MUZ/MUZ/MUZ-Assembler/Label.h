//
//  Label.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 01/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Label_h
#define Label_h

#include <unordered_map>
#include "AddressZone.h"
#include "Name.h"

namespace MUZ {
	
	/** Structure to store a label from source file. */
	class Label : public Name
	{
	public:
		LineReference line;		// file and line numbers
		ADDRESSTYPE	address;	// address off this label
		bool equate;			// this label is set by a .EQU
		
		void SetAddress( unsigned int integer ) {
			this->address = (ADDRESSTYPE)(integer & ADDRESSMASK);;
		}
		
	};

	typedef std::unordered_map<std::string, Label*> LabelMap;
} // namespace MUZ
#endif /* Label_h */
