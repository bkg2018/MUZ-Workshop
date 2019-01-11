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
#include "Z80-Operands.h"

namespace MUZ {
	
	/** Source and assembled content for one line of source code. */
	struct CodeLine
	{
		// source
		int					file;						// source file reference (index in Assembler SourceFile array)
		int					line;						// duplicates the CodeLine
		std::string 		source;						// TODO: replace with file direct access?
		//long				offset;						// position in the source file
		//int 				size;						// number of bytes in this source line

		// parsed tokens
		ExpVector			tokens;						// Tokens as prepared by the parser
		int					instructiontoken;			// The instruction token index
		int					curtoken;					// current position for Reset/Next functions
		
		// assembled code
		bool				assembled = false; 			// true when assembled
		std::vector<BYTE>	code;						// assembled code
		int					cyclesmin;					// machine cycles for this code
		int					cyclesmax;					// machine cycles for this code
		ADDRESSTYPE			address = 0;				// starting address for this code
		class Section*		section = nullptr;
		int					includefile = 0;			// file reference when this line includes another sourcefile
		class Label*		label = nullptr;			// label if there is one on this line
		class Assembler*	as = nullptr;				// referenc for pass check
		
		
		/** Resets the token exploration system to the first token after the instruction. */
		void ResetInstruction(int start);
		
		/** Set machine cycles. */
		void SetCycles(int mintime, int maxtime = -1);
		
		/** Pushes codes. */
		void ResetCode();
		void AddCode(DATATYPE b0);
		void AddCode(DATATYPE b0, DATATYPE b1);
		void AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2) ;
		void AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2, DATATYPE b3);
	};

}
#endif /* CodeLine_h */
