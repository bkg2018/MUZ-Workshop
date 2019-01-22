/*
 * Computer.h
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_COMPUTER_H_
#define SRC_MUZ_COMPUTER_H_

#include "MUZ-Computer/MemoryMgr.h"
#include "MUZ-Computer/Module.h"
#include "MUZ-Computer/PortMgr.h"

namespace MUZ {

class Computer: public Module {

protected:

	MemoryMgr		m_memorymgr;
	PortMgr			m_portmgr;

public:
	Computer();
	virtual ~Computer();

	/** Displays on a given peripheral. */
	virtual void DisplayOn(Peripheral* /*peripheral*/) ;

	/** Set the ROM from an Intel Hex File. */
	void SetROM( std::string hexfile );
	
	/** Allocate maximum RAM size. A part of it maybe overloaded by ROM. */
	void SetMaxRAM();
	
	/** Page the ROM out, allowing access to the RAM below if available. */
	void PageROMout();

	/** Page the ROM in, possibly hiding the RAM below. */
	void PageROMin() ;
	
	/** Switches the ROM paging, RC2014 pageable ROM mode.
	 */
	void PageROMswitch() ;

	/** Read/Write access to an address. This operator returns an instance of the MemoryReference
	 * 	class which allows to use operators = and [] on the manager in expressions:
	 *		manager[address] = value
	 *		manager[address] = othermanager[otheraddress]
	 *		value = manager[address]
	 *  Can throw a memory access exception if out of range or unassigned.
	 *  @param address the address, which must be within range
	 *	@return a memory reference to be used with operators = and [] in expressions.
	 */
	MemoryModule::MemoryReference operator[](ADDRESSTYPE address);
	
	/** Assigns a port module to a port address. If nullptr is given, the port address releases the port it is assigned. */
	void Assign(int address, PortModule* module);
	
	/** Generic input: returns a data. */
	virtual DATATYPE In(int address);
	
	/** Generic output: sends a data. */
	virtual void Out(int address, DATATYPE data) ;


};

} /* namespace MUZ */

#endif /* SRC_MUZ_COMPUTER_H_ */
