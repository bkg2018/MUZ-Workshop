//
//  muz_mmu.h
//  MUZ-Workshop
//
// This header is a rewritten YAZE/mem_mmu.h adapted to MUZ MemoryMgr.
//
//  Created by Francis Pierot on 28/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef muz_mmu_h
#define muz_mmu_h

// Include original YAZE header to define all macros
#undef MMU
#include "mem_mmu.h"

// Redefine the Yaze ram access macros to use the MUZ Computer memory manager
#undef RAM
#undef MRAM
#undef RAM_pp
#undef MRAM_pp
#undef RAM_mm
#undef MRAM_mm
#undef mm_RAM
#undef mm_MRAM

#define RAM(a)          m_memorymgr[(a)&0xffff]
#define MRAM(xmmu,a)    m_memorymgr[(a)&0xffff]
#define RAM_pp(a)       m_memorymgr[(a++)&0xffff]
#define MRAM_pp(xmmu,a) m_memorymgr[(a++)&0xffff]
#define RAM_mm(a)       m_memorymgr[(a--)&0xffff]
#define MRAM_mm(xmmu,a) m_memorymgr[(a--)&0xffff]
#define mm_RAM(a)       m_memorymgr[(--a)&0xffff]
#define mm_MRAM(xmmu,a) m_memorymgr[(--a)&0xffff]

// Below are *untouched*  macros which were indirectly referencing the ram[] array in mem_mmu.h
// we redefine them to make sure they use the new RAM family macros above.
#undef GetBYTE
#undef GetBYTE_pp
#undef GetBYTE_mm
#undef mm_GetBYTE
#undef PutBYTE
#undef PutBYTE_pp
#undef PutBYTE_mm
#undef GetWORD
#undef PutWORD
#undef memcpy_get_z
#undef memcpy_M_get_z
#undef memcpy_put_z
#undef memcpy_M_put_z
#undef memset_M_z
#undef memset_z
// and redefine them below, this is identical to what's written in mem_mmu.h


/* Some important macros. They are the interface between an access from
 the simz80-/yaze-Modules and the method of the memory access: */

#define GetBYTE(a)      RAM(a)
#define GetBYTE_pp(a)   RAM_pp(a)
#define GetBYTE_mm(a)   RAM_mm(a)
#define mm_GetBYTE(a)   mm_RAM(a)
#define PutBYTE(a, v)   RAM(a) = v
#define PutBYTE_pp(a,v) RAM_pp(a) = v
#define PutBYTE_mm(a,v)	RAM_mm(a) = v
#define GetWORD(a)      (RAM(a) | (RAM((a)+1) << 8))

/* don't work: #define GetWORD_pppp(a)	(RAM_pp(a) + (RAM_pp(a) << 8)) */
/* make once more a try at 18.10.1999/21:45 ... with the following macro:  */
/* works also not #define GetWORD_pppp(a) (RAM_pp(a) | (RAM_pp(a) << 8))  */
/* I dont know what the optimizer do with such macro.
 If someone knows about it - I'am very interessed to that knowledge.
 */

#define PutWORD(a, v)							\
do { RAM(a) = (BYTE)(v);						\
RAM((a)+1) = (v) >> 8;						\
} while (0)

/*------------------- Some macros for manipulating Z80-memory : -------*/

#define memcpy_get_z(d,s,n)						\
do { size_t len = n;						\
size_t source = s;						\
BYTE *p1 = d;							\
while (len--) *p1++ = GetBYTE_pp(source);			\
} while (0)


#define memcpy_M_get_z(xm,d,s,n)					\
do { size_t len = n;						\
size_t source = s;						\
BYTE *p1 = d;							\
while (len--) *p1++ = MRAM_pp(xm,source);			\
} while (0)


#define memcpy_put_z(d,s,n)						\
do { size_t len = n;						\
size_t dest = d;						\
BYTE *p1 = s;							\
while (len--) PutBYTE_pp(dest,*p1++);				\
} while (0)


#define memcpy_M_put_z(xm,d,s,n)					\
do { size_t len = n;						\
size_t dest = d;						\
BYTE *p1 = s;							\
while (len--) MRAM_pp(xm,dest) = *p1++;			\
} while (0)


#define memset_M_z(xm,p,v,n)						\
do { size_t len = n;						\
while (len--) MRAM_pp(xm,p) = v;				\
} while (0)


#define memset_z(p,v,n)							\
do { size_t len = n;						\
while (len--) PutBYTE_pp(p,v);					\
} while (0)

#endif
