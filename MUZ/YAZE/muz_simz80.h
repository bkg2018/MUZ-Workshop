//
//  muz_simz80.h
//  MUZ-Workshop
//
// This header is a rewritten YAZE/simz80.h adapted to MUZ MemoryMgr.
//
//  Created by Francis Pierot on 28/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef muz_simz80_h
#define muz_simz80_h

// define memory manager and Yaze types
#include "MUZ-Computer/Computer.h"
#include "muz_mmu.h"
#include "simz80.h"

// include MUZ stuff

// redefine I/O calls in terms of MUZ Computer port manager
#undef Input
#undef Output
#define Input(port)         m_portmgr.In(port)
#define Output(port, value) m_portmgr.Out(port,value)

/*// define the structure for 16-bit general registers
struct ddregs {
	WORD bc;
	WORD de;
	WORD hl;
};*/



// encapsulate registers and simulator function into a MUZ Computer
class YazeZ80 : public MUZ::Computer {

	// registers can be accessed from anywhere
public:
	/* two sets of accumulator / flags */
	WORD		af[2];
	int			af_sel;

	/* two sets of 16-bit registers */
	ddregs		regs[2];
	int			regs_sel;
	
	WORD		ir;
	WORD		ix;
	WORD		iy;
	WORD		sp;
	WORD		pc;
	WORD		IFF;
	
public:
	void InitRegisters() {
		af[0] = 0;
		af[1] = 0;
		regs[0].bc = 0;
		regs[0].de = 0;
		regs[0].hl = 0;
		regs[1].bc = 0;
		regs[1].de = 0;
		regs[1].hl = 0;
		ir = 0;
		ix = 0;
		iy = 0;
		regs_sel = 0;
		af_sel = 0;
		pc = 0;
		sp = 0;
		IFF = 0;
	};
	
	FASTWORK simz80(FASTREG PC, bool step) ;
	
};
#endif /* muz_simz80_h */
