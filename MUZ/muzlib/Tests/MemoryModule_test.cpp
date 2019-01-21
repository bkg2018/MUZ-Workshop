/*
 * MemoryModule_test.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */

#include "MUZ-Computer/MemoryModule.h"

#define XCTAssertEqual(x, y) assert( (x) == (y) );
#define XCTAssertNotEqual(x, y) assert( (x) != (y) );

// avoid warning for no prev prototype
void testMemoryModule();


void testMemoryModule()
{
	MUZ::MemoryModule mem;
	mem.SetROM("/Users/bkg2018/Desktop/IntelHex.hex");
	
	MUZ::BYTE b0 = mem[0];
	mem[0] = ~b0;// no effect in ROM
	XCTAssertEqual(b0, mem[0]);
	mem[1] = mem[2];
	XCTAssertNotEqual(mem[1], mem[2]);
	
	mem.SetRAM("/Users/bkg2018/Desktop/IntelHex.hex");
	MUZ::BYTE b1 = mem[0];
	mem[0] = ~b1;
	XCTAssertNotEqual(b1, mem[0]);
	mem[1] = mem[2];
	XCTAssertEqual(mem[1], mem[2]);
}
