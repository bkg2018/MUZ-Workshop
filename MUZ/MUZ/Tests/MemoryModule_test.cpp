/*
 * MemoryModule_test.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */

#include "MUZ-Computer/MemoryModule.h"

static std::string RomHexFilePath = "/Users/bkg2018/Desktop/IntelHex.hex";
#define XCTAssertEqual(x, y) assert( (x) == (y) );
#define XCTAssertNotEqual(x, y) assert( (x) != (y) );

void testMemoryModule()
{
	MUZ::MemoryModule mem;
	mem.SetROM(RomHexFilePath);
	
	MUZ::BYTE b0 = mem[0];
	mem[0] = ~b0;// no effect in ROM
	XCTAssertEqual(b0, mem[0]);
	mem[1] = mem[2];
	XCTAssertNotEqual(mem[1], mem[2]);
	
	mem.SetRAM(RomHexFilePath);
	MUZ::BYTE b1 = mem[0];
	mem[0] = ~b1;
	XCTAssertNotEqual(b1, mem[0]);
	mem[1] = mem[2];
	XCTAssertEqual(mem[1], mem[2]);
}
