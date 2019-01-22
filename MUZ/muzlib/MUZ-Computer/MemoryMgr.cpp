/*
 * MemoryMgr.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */
#include "pch.h"
#include "MUZ-Computer/MemoryMgr.h"

namespace MUZ {

	MemoryMgr::MemoryMgr() {
		m_rompagedout = false;
	}
	MemoryMgr::~MemoryMgr() {
	}

	/** Sets ram to a start address and a size.
	 *  @see MemoryModule::SetRAM()
	 */
	void MemoryMgr::SetRAM(ADDRESSTYPE start, ADDRESSSIZETYPE size) {
		m_ram.SetRAM(start, size);
	}

	/** Sets the maximum amount of RAM. */
	void MemoryMgr::SetMaxRAM() {
		m_ram.SetRAM(0, MEMMAXSIZE);
	}

	/** Sets ROM with a content from a Intel hex file.
	 *	@see MemoryModule::SetROM()
	 */
	void MemoryMgr::SetROM(std::string hexfile) {
		m_rom.SetROM(hexfile);
	}

	/** Pages the ROM out.
	 *	This disables the ROM module: all memory accesses will go to the RAM module.
	 */
	void MemoryMgr::PageROMout() {
		m_rompagedout = true;
	}

	/** Pages the ROM in.
	 *	This enables the ROM module: memory accesses will go to the ROM module when the address is in ROM range,
	 *	Other addresses will go to the RAM module.
	 */
	void MemoryMgr::PageROMin() {
		m_rompagedout = false;
	}

	/** Switches the ROM paging, RC2014 pageable ROM mode.
	 */
	void MemoryMgr::PageROMswitch() {
		m_rompagedout = !m_rompagedout;
	}


	/** Relocates the ROM module to another address. The ROM always hides RAm at the same addresses when it is aged in.
	 */
	void MemoryMgr::Relocate( ADDRESSTYPE address ) {
		m_rom.Relocate(address);
	}

	/** Returns the right module for an adress. */
	MemoryModule& MemoryMgr::GetModuleFor(ADDRESSTYPE address) {
		if (!m_rompagedout && m_rom.isInRange(address)) return m_rom;
		if (m_ram.isInRange(address)) return m_ram;
		return m_empty;
	}

	/** Read/Write access to an address. This operator returns an instance of the MemoryReference
	 * 	class which allows to use operators = and [] on the manager in expressions:
	 *		manager[address] = value
	 *		manager[address] = othermanager[otheraddress]
	 *		value = manager[address]
	 *  Can throw a memory access exception if out of range or unassigned.
	 *  @param address the address, which must be within range
	 *	@return a memory reference to be used with operators = and [] in expressions.
	 */
	MemoryModule::MemoryReference MemoryMgr::operator[](ADDRESSTYPE address) {
		MemoryModule& mm = GetModuleFor(address);
		return mm[address];
	}

	/** Displays on a given peripheral. */
	void MemoryMgr::DisplayOn(Peripheral* /*peripheral*/) {}

} // namespace
