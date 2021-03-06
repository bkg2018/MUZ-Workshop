/*
 * MemoryMgr.h
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MEMORYMGR_H_
#define SRC_MEMORYMGR_H_

#include <vector>
using std::vector;

#include "MUZ-Computer/MemoryModule.h"

namespace MUZ {

class MemoryMgr: public Module {
	
	MemoryModule	m_empty;		// voluntary unassigned module, will be used for out of ranges adresses
	MemoryModule	m_ram;			// up to 64KB of RAM
	MemoryModule	m_rom;			// up to 64KB of ROM, can be paged in/out to mask RAM
	bool			m_rompagedout;	// true if m_rom is paged out // it's paged in at start

public:
	MemoryMgr();
	virtual ~MemoryMgr();

	/** Sets ram to a start address and a size.
	 *  @see MemoryModule::SetRAM()
	 */
	void SetRAM(ADDRESSTYPE start, ADDRESSSIZETYPE size);
	
	/** Sets the maximum amount of RAM. */
	void SetMaxRAM();
	
	/** Sets ROM with a content from a Intel hex file.
	 *	@see MemoryModule::SetROM()
	 */
	void SetROM(std::string hexfile);
	
	/** Pages the ROM out.
	 *	This disables the ROM module: all memory accesses will go to the RAM module.
	 */
	void PageROMout();
	
	/** Pages the ROM in.
	 *	This enables the ROM module: memory accesses will go to the ROM module when the address is in ROM range,
	 *	Other addresses will go to the RAM module.
	 */
	void PageROMin();
	
	/** Switches the ROM paging, RC2014 pageable ROM mode.
	 */
	void PageROMswitch();


	/** Relocates the ROM module to another address. The ROM always hides RAm at the same addresses when it is aged in.
	 */
	void Relocate( ADDRESSTYPE address );
	
	/** Returns the right module for an adress. */
	MemoryModule& GetModuleFor(ADDRESSTYPE address);
	
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

	/** Displays on a given peripheral. */
	virtual void DisplayOn(Peripheral* /*peripheral*/);
};

};
#endif /* SRC_MEMORYMGR_H_ */
