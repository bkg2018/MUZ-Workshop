/*
 * MemoryMgr_test.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#include "MUZ-Computer/MemoryMgr.h"

#define XCTAssertEqual(x, y) assert( (x) == (y) );

void testMemoryManager();

void testMemoryManager()
{
	MUZ::MemoryMgr mmgr;
	// prepare 64KB RAM
	mmgr.SetMaxRAM();
	// Load SC Monitor + Basic + CP/M loader
	mmgr.SetROM("/Users/bkg2018/Desktop/IntelHex.hex");
	
	// we now have:
	// ROM from 0000 to 3FFF
	// RAM from 4000 to FFFF
	MUZ::DATATYPE value;
	
	// test ROM
	value = mmgr[0]; // should be C3
	XCTAssertEqual(value, 0xC3);
	mmgr[0] = 0; // should not write
	value = mmgr[0]; // should be C3
	XCTAssertEqual(value, 0xC3);
	
	// test RAM
	mmgr[0x4000] = 0xAA;
	XCTAssertEqual(mmgr[0x4000], 0xAA);
	mmgr[0x4000] = ~mmgr[0x4000]; // should become 0x55
	XCTAssertEqual(mmgr[0x4000], 0x55);
	
	// test ROM paging out
	mmgr.PageROMout();
	
	value = mmgr[0]; // should be 00
	XCTAssertEqual(value, 0x00);
	mmgr[0] = 0x55;
	value = mmgr[0]; // should be C3
	XCTAssertEqual(value, 0x55);
	
}
