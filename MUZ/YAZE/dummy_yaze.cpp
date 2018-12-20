/*
 * dummy_yaze.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */



#include "ytypes.h"
#include "mem_mmu.h"
#include "simz80.h"
// YAZE variables
/* Z80 registers */
WORD af[2];			/* accumulator and flags (2 banks) */
int af_sel;			/* bank select for af */

struct ddregs regs[2];		/* bc,de,hl */
int regs_sel;			/* bank select for ddregs */

WORD ir;			/* other Z80 registers */
WORD ix;
WORD iy;
WORD sp;
WORD pc;
WORD IFF;


int in(unsigned int /*port*/)
{

	return 0;
}
/*}}}*/
/* out */ /*{{{*/
void out(unsigned int /*port*/, unsigned char /*byte*/)
{

}
