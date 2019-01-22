/*
 * Computer.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */
#include "pch.h"
#include "MUZ-Computer/Computer.h"

namespace MUZ {

	Computer::Computer() {
	}

	Computer::~Computer() {
	}

	/** Displays on a given peripheral. */
	void Computer::DisplayOn(Peripheral* /*peripheral*/)
	{
	}

	/** Set the ROM from an Intel Hex File. */
	void Computer::SetROM( std::string hexfile )
	{
		m_memorymgr.SetROM(hexfile);
	}

	/** Allocate maximum RAM size. A part of it maybe overloaded by ROM. */
	void Computer::SetMaxRAM()
	{
		m_memorymgr.SetMaxRAM();
	}

	/** Page the ROM out, allowing access to the RAM below if available. */
	void Computer::PageROMout()
	{
		m_memorymgr.PageROMout();
	}

	/** Page the ROM in, possibly hiding the RAM below. */
	void Computer::PageROMin()
	{
		m_memorymgr.PageROMin();
	}

	/** Switches the ROM paging, RC2014 pageable ROM mode.
	 */
	void Computer::PageROMswitch()
	{
		m_memorymgr.PageROMswitch();
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
	MemoryModule::MemoryReference Computer::operator[](ADDRESSTYPE address)
	{
		return m_memorymgr[address];
	}

	/** Assigns a port module to a port address. If nullptr is given, the port address releases the port it is assigned. */
	void Computer::Assign(int address, PortModule* module)
	{
		m_portmgr.Assign(address, module);
	}

	/** Generic input: returns a data. */
	DATATYPE Computer::In(int address)
	{
		return m_portmgr.In(address);
	}

	/** Generic output: sends a data. */
	void Computer::Out(int address, DATATYPE data)
	{
		m_portmgr.Out(address,data);
	}
} /* namespace MUZ */
