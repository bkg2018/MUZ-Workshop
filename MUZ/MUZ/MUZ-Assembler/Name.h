//
//  Name.hpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 03/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Name_hpp
#define Name_hpp

#include <string>

namespace MUZ {
	
	// Class to represent an abstract name in source file:
	// - #define symbols
	// - labels defined with :
	// - equates defined with .EQU
	// Derived classes do the actual work
	class Name {
	protected:
		// the class is empty, only derived classes are meaningfull
		virtual void dummy() {};
	};
	
} // namespace MUZ

#endif /* Name_hpp */
