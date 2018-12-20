//
//  CodeLine.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 18/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef CodeLine_h
#define CodeLine_h

#include <string>
#include <vector>

#include "Types.h"
#include "ExpVector.h"

namespace MUZ {
	
	/** Type of code content.*/
	enum CodeType
	{
		codeTypeUNKNOWN,
		codeTypeCODE,
		codeTypeDATA,
		codeTypeSTRING
	};

	/** Source and assembled content for one line of source code. */
	struct CodeLine
	{
		// source
		int					file;						// source file reference in the assembler
		long				offset;						// position in the source file
		int 				size;						// number of bytes in this source line
		int					line;						// duplicates the CodeLine index in SourceFile array
		std::string 		source;						// TODO: replace with file direct access
		
		// parsed tokens
		ExpVector			tokens;
		
		// assembled code
		bool				assembled = false; 			// true when assembled
		std::vector<BYTE>	code;						// assembled code
		ADDRESSTYPE			address = 0;				// starting address for this code
		CodeType			codetype = codeTypeUNKNOWN;	// type of data for this code
		int					includefile = 0;			// > 0 when this line includes another sourcefile
	};

}
#endif /* CodeLine_h */
