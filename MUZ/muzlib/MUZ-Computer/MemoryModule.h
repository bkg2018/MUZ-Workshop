/*
 * MemoryModule.h- specialized RAM or ROM module
 *
 * A memory module offer memory services:
 *
 *		return a byte at an address
 *      write a byte at an address
 *      automatically manage read-only
 *
 * It can be "programmed":
 *
 *      be assigned an address range (start address and size)
 *      receive a Read Only status
 *      receive a HEX file content to store
 *		be relocated in the available address range
 *
 * For the disassembler use, a shadow image of memory is kept with content type flags:
 *
 *      b0 = Code if 1, data if 0
 *      b1 = Visited if 1
 *      b2 = Text if 1
 *      b3 = Sprite if 1
 *      b4 = Address if 1
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_MEMORYMODULE_H_
#define SRC_MUZ_MEMORYMODULE_H_

#include <stdlib.h>
#include <string>

#include "MUZ-Common/Exceptions.h"
#include "MUZ-Common/Types.h"
#include "Module.h"

namespace MUZ {

	
class MemoryModule: public Module {

	enum MemoryCodeFlag {
		codeFlagCODE    = 1 << 0,
		codeFlagDATA    = 1 << 0,	// same as code but used with reversed logic
		codeFlagVISITED = 1 << 1,
		codeFlagTEXT    = 1 << 2,
		codeFlagSPRITE  = 1 << 3,
		codeFlagADDRESS = 1 << 4
	};
	
	ADDRESSTYPE		m_start;	// Starting address of content in 0000-FFFF address space (for 16-bits)
	ADDRESSTYPE		m_end;		// last possible address (m_start + m_size - 1)
	ADDRESSSIZETYPE	m_size;		// Size in bytes, presumably multiple of 1024
	bool			m_rw;		// True for RAM, false for ROM
	DATATYPE*		m_content;	// Memory content
	BYTE*			m_codeflag;	// flags for each byte content type

public:
	MemoryModule() {
		m_start = 0;
		m_size = 0;
		m_end = 0;
		m_rw = true;
		m_content = nullptr;
		m_codeflag = nullptr;
	}
	virtual ~MemoryModule() {
		if (m_content != nullptr) free(m_content);
		if (m_codeflag != nullptr) free(m_codeflag);
	}
	
	/** tells if an address is in range. */
	bool isInRange(ADDRESSTYPE address) const {
		if (address > m_end) return false;
		if (address < m_start) return false;
		return true;
	}

	/** Checks if an address is valid in this module.
	 @param address the address to check
	 @throw MemoryRangeException: the address is invalid in the range managed by this module
	 @throw MemoryUnassignedException: this module has no assigned memory yet
	 @return true if address is valid, else an exception is thrown
	 */
	bool CheckAddress(ADDRESSTYPE address) const {
		if (!isInRange(address)) throw MemoryRangeException();
		if (m_content == nullptr) throw MemoryUnassignedException();
		return true;
	} ;

	/** Checks if an address range is valid in this module.
	 @param address the address to check
	 @param size the number of bytes
	 @throw MemoryRangeException: the address is invalid in the range managed by this module
	 @throw MemoryUnassignedException: this module has no assigned memory yet
	 @return true if address range is valid, else an exception is thrown
	 */
	bool CheckAddressRange(ADDRESSTYPE address, ADDRESSTYPE size) const {
		if (!isInRange(address)) throw MemoryRangeException();
		if (!isInRange(address + size)) throw MemoryRangeException();
		if (m_content == nullptr) throw MemoryUnassignedException();
		return true;
	} ;
	
	/** Module virtual: displays on a given peripheral. */
	virtual void DisplayOn(Peripheral* /*peripheral*/) {}

	/** Programs a content from an Intel HEX file and sets as ROM.
	 *  @param hexfile the HEX file containing the code to copy in ROM
	 * 	@throw MemoryRangeException: size is too big and would make the range exceed $FFFF
	 *  @throw NoFileException: HEX file not found
	 * 	@return true if assignement was OK
	 */
	bool SetROM(std::string hexfile);

	/** Programs a content from an Intel HEX file and sets as RAM.
	 *	@param hexfile the HEX file containing the code to copy in ROM
	 *	@throw MemoryRangeException: size is too big and would make the range exceed $FFFF
	 *  @throw NoFileException: HEX file not found
	 *	@return true if assignement was OK
	 */
	bool SetRAM(std::string hexfile);
	
	/** Assigns a RAM range to this module. RAM is initialized to 0.
	 * @param start the start address in 0000-FFFF range
	 * @param size the number of bytes
	 * @throw MemoryRangeException: size is too big and would make the range exceed $FFFF
	 * @return true if assignement was OK
	 */
	bool SetRAM(ADDRESSTYPE start, ADDRESSSIZETYPE size);

	/** Reference to a memory address for read/write access. This is returned by the [] operator
	    to differentiate between RAM and ROM memory modules: writing to ROM module is ignored
	 	in the = operator.
	 */
	class MemoryReference
	{
		DATATYPE* m_address;
		bool m_rw;
	public:
		MemoryReference(DATATYPE* address, bool rw) { m_address = address; m_rw = rw; }
		
		// this[address] = value
		MemoryReference& operator=( DATATYPE b ) {
			if (m_rw) *m_address = b;// no write if ROM
			return *this;
		}
		// this[address] = other[address]
		MemoryReference& operator=( const MemoryReference& other ) {
			if (m_rw) *m_address = *other.m_address;
			return *this;
		}
		// value = this[address]
		operator DATATYPE() const {
			return *m_address;
		}
	};
	
	/** Read/Write access to an address. This operator returns an instance of the MemoryReference
	 * 	class which allows to use operators = and [] on tyhe module in expressions:
	 *		module[address] = value
	 *		module[address] = othermodule[otheraddress]
	 *		value = module[address]
	 *  Can throw a memory access exception if out of range or unassigned.
	 *  @param address the address, which must be within range
	 *  @throw MemoryRangeException: the address is not contained in this memory module
	 *	@return a memory reference to be used with operators = and [] in expressions.
	 */
	MemoryReference operator[](ADDRESSTYPE address) {
		CheckAddress(address);
		return MemoryReference(m_content + address - m_start, m_rw);
	}
	
	/** Classical write.
	 *		module[address] = value                        : module.Set(address, value)
	 *		module[address] = othermodule[otheraddress]    : module.Set(address, othermodule.Get(otheraddress))
	 *		value = module[address]                        : value = module.Get(address)
	 *  @param address the address, which must be within range of this module
	 *  @param value the value
	 *  @throw MemoryRangeException: the address is not contained in this memory module
	 */
	void Set(ADDRESSTYPE address, DATATYPE value) {
		CheckAddress(address);
		if (m_rw) m_content[address - m_start] = value;
	}
	/** Classical read.
	 *		module[address] = value                        : module.Set(address, value)
	 *		module[address] = othermodule[otheraddress]    : module.Set(address, othermodule.Get(otheraddress))
	 *		value = module[address]                        : value = module.Get(address)
	 *  @param address the address, which must be within range of this module
	 *  @return the value stored at the address
	 *  @throw MemoryRangeException: the address is not contained in this memory module
	 */
	DATATYPE Get(ADDRESSTYPE address) {
		CheckAddress(address);
		return m_content[address - m_start];
	}
	
	/** Sets the code flag for a range of addresses.
	 *  @param address the starting address
	 *  @param size the number of bytes to mark with code flag
	 *	@param codeflags the flags to set
	 */
	void SetCodeFlags(ADDRESSTYPE address, ADDRESSTYPE size, BYTE codeflags) {
		CheckAddressRange(address, size);
		for (ADDRESSTYPE a = address-m_start ; a <= address-m_start + size ; a++) {
			m_codeflag[a] = codeflags;
		}
	}
	
protected:
	/** Tests if bits are set in a byte at an address.
	 * @param address the address
	 * @param bits the byte with bits ON for the bits to tests
	 * @return true if all bits were set at the address, false if any bit was not set
	 */
	bool testBit(ADDRESSTYPE address, BYTE bits) const {
		CheckAddress(address);
		return (m_codeflag[address-m_start] & bits) == bits;
	}
public:
	/** tests if the content at an address is marked as code. */
	bool isCode(ADDRESSTYPE address) const   { return  testBit(address, codeFlagCODE); }
	/** tests if the content at an address is marked as data. */
	bool isData(ADDRESSTYPE address) const   { return !testBit(address, codeFlagDATA); }
	/** tests if the content at an address is marked as text. */
	bool isText(ADDRESSTYPE address) const   { return  testBit(address, codeFlagTEXT); }
	/** tests if the content at an address is marked as sprite. */
	bool isSprite(ADDRESSTYPE address) const { return  testBit(address, codeFlagSPRITE); }
	/** tests if the content at an address is marked as address. */
	bool isAddr(ADDRESSTYPE address) const   { return  testBit(address, codeFlagADDRESS); }
	/** tests if the content at an address is marked as visited. */
	bool isVisited(ADDRESSTYPE address) const {  return testBit(address, codeFlagVISITED);  }
	
protected:
	void   markBits(ADDRESSTYPE address, ADDRESSTYPE length = 1, BYTE bits = codeFlagVISITED) {
		CheckAddressRange(address, length);
		m_codeflag[address-m_start] |= bits;
	}
	void unmarkBits(ADDRESSTYPE address, ADDRESSTYPE length = 1, BYTE bits = codeFlagVISITED) {
		CheckAddressRange(address, length);
		m_codeflag[address-m_start] &= ~bits;
	}
	
public:
	void   markCode(ADDRESSTYPE address, ADDRESSTYPE length=1)    {   markBits(address,length,codeFlagCODE); }
	void   markData(ADDRESSTYPE address, ADDRESSTYPE length=1)    { unmarkBits(address,length,codeFlagDATA); }
	void   markVisited(ADDRESSTYPE address, ADDRESSTYPE length=1) {   markBits(address,length,codeFlagVISITED); }
	void unmarkVisited(ADDRESSTYPE address, ADDRESSTYPE length=1) { unmarkBits(address,length,codeFlagVISITED); }
	void   markText(ADDRESSTYPE address, ADDRESSTYPE length=1)    {   markBits(address,length,codeFlagTEXT); }
	void unmarkText(ADDRESSTYPE address, ADDRESSTYPE length=1)    { unmarkBits(address,length,codeFlagTEXT); }
	void   markSprite(ADDRESSTYPE address, ADDRESSTYPE length=1)  {   markBits(address,length,codeFlagSPRITE); }
	void unmarkSprite(ADDRESSTYPE address, ADDRESSTYPE length=1)  { unmarkBits(address,length,codeFlagSPRITE); }
	void   markAddr(ADDRESSTYPE address, ADDRESSTYPE length=2)	  {   markBits(address,length,codeFlagADDRESS); }
	void unmarkAddr(ADDRESSTYPE address, ADDRESSTYPE length=2)	  { unmarkBits(address,length,codeFlagADDRESS); }
	
	/** Relocates the module to another address.
	 *	If the new address plus the module size exceeds the possible addresses, a MemoryRangeException is thrown.
	 */
	void Relocate(ADDRESSTYPE address) {
		if (address + m_size > MEMMAXSIZE) throw MemoryRangeException();
		m_start = address;
		m_end = (ADDRESSTYPE)(m_start + m_size - 1);
	}
};

};
#endif /* SRC_MUZ_MEMORYMODULE_H_ */
