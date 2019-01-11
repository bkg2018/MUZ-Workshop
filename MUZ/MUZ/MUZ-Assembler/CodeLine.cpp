//
//  CodeLine.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 28/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "CodeLine.h"
#include "Assembler.h"

namespace MUZ {
	
	/** Resets the token exploration system to the first token after the instruction. */
	void CodeLine::ResetInstruction(int start) {
		instructiontoken = start;
		curtoken = instructiontoken + 1;
	}
	
	/** Set machine cycles. */
	void CodeLine::SetCycles(int mintime, int maxtime) {
		cyclesmin = mintime;
		cyclesmax = maxtime > 0 ? maxtime : mintime;
	}
	
	/** Pushes codes. */
	void CodeLine::ResetCode() {
		code.clear();
	}
	void CodeLine::AddCode(DATATYPE b0) {
		code.push_back(b0 & DATAMASK);
	}
	void CodeLine::AddCode(DATATYPE b0, DATATYPE b1) {
		AddCode(b0);
		AddCode(b1);
	}
	void CodeLine::AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2) {
		AddCode(b0);
		AddCode(b1);
		AddCode(b2);
	}
	void CodeLine::AddCode(DATATYPE b0, DATATYPE b1, DATATYPE b2, DATATYPE b3) {
		AddCode(b0);
		AddCode(b1);
		AddCode(b2);
		AddCode(b3);
	}
} // namespace
