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
		ExpVector			tokens;						// Tokens as prepared by the parser
		int					instructiontoken;			// The instruction token index
		int					curtoken;					// current position for Reset/Next functions
		
		// assembled code
		bool				assembled = false; 			// true when assembled
		std::vector<BYTE>	code;						// assembled code
		int					cyclesmin;					// machine cycles for this code
		int					cyclesmax;					// machine cycles for this code
		ADDRESSTYPE			address = 0;				// starting address for this code
		Section*			section = nullptr;
		CodeType			codetype = codeTypeUNKNOWN;	// type of data for this code
		int					includefile = 0;			// file reference when this line includes another sourcefile
		class Label*		label = nullptr;			// label if there is one on this line
		class Assembler*	as = nullptr;				// referenc for pass check
		
		
		/** Resets the token exploration system to the first token after the instruction. */
		void ResetInstruction(int start) {
			instructiontoken = start;
			curtoken = instructiontoken + 1;
		}
		
		/** Set machine cycles. */
		void SetCycles(int mintime, int maxtime = -1) {
			cyclesmin = mintime;
			cyclesmax = maxtime > 0 ? maxtime : mintime;
		}
		
		/** Pushes codes. */
		void ResetCode() {
			code.clear();
		}
		void AddCode(DATATYPE b0) {
			code.push_back(b0 & DATAMASK);
		}
		void AddCode(DATATYPE b0, DATATYPE b1) {
			AddCode(b0);
			AddCode(b1);
		}
		void AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2) {
			AddCode(b0);
			AddCode(b1);
			AddCode(b2);
		}
		void AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2, DATATYPE b3) {
			AddCode(b0);
			AddCode(b1);
			AddCode(b2);
			AddCode(b3);
		}
	};

}
#endif /* CodeLine_h */
