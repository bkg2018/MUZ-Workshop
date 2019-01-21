/*
 * Computer_test.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#include "MUZ-Computer/ROMPagingPort.h"

#define XCTAssertEqual(x, y) assert( (x) == (y) );
void testComputer();

void testComputer()
{
	// the virtual computer
	MUZ::Computer computer;

	// Equip with 64KB RAM
	computer.SetMaxRAM();

	// Load SC Monitor + Basic + CP/M loader in ROM (this file fills 16KB at addresses $0000->$3FFF)
	computer.SetROM("/Users/bkg2018/Desktop/IntelHex.hex");

	// create a switching paging port
	MUZ::ROMPagingPort pagingport(&computer, true);
	// assign it to port 38
	computer.Assign(0x38, &pagingport);

	// we now have:
	// ROM from 0000 to 3FFF
	// RAM from 4000 to FFFF

	// test ROM
	XCTAssertEqual(computer[0], 0xC3);
	computer[0] = 0; // should not write
	XCTAssertEqual(computer[0], 0xC3);

	// test RAM
	computer[0x4000] = 0xAA;
	XCTAssertEqual(computer[0x4000], 0xAA);
	computer[0x4000] = ~computer[0x4000]; // should become 0x55
	XCTAssertEqual(computer[0x4000], 0x55);

	// test ROM paging out by sending 0 (switching mode)
	computer.Out(0x38, 0);
	XCTAssertEqual(computer[0], 0x00);
	computer[0] = 0x55;
	XCTAssertEqual(computer[0], 0x55);

	// test ROM paging in
	computer.Out(0x38, 0);
	XCTAssertEqual(computer[0], 0xC3);
	computer[0] = 0x55;
	XCTAssertEqual(computer[0], 0xC3);

	// change to non switching mode
	pagingport.SetSwitchMode(false);

	// test ROM paging out by sending 1 to port
	computer.Out(0x38, 1);
	XCTAssertEqual(computer[0], 0x55);
	computer[0] = ~computer[0];
	XCTAssertEqual(computer[0], 0xAA);

	// test ROM paging in
	computer.Out(0x38, 0);
	XCTAssertEqual(computer[0], 0xC3);
	computer[0] = 0x55;
	XCTAssertEqual(computer[0], 0xC3);
}
