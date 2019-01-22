/*
 * MemoryModule.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */
#include "pch.h"
#include "MemoryModule.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "MUZ-Common/Types.h"
#include "MUZ-Common/StrUtils.h"

namespace MUZ {


	MemoryModule::MemoryModule() {
		m_start = 0;
		m_size = 0;
		m_end = 0;
		m_rw = true;
		m_content = nullptr;
		m_codeflag = nullptr;
	}
	MemoryModule::~MemoryModule() {
		if (m_content != nullptr) free(m_content);
		if (m_codeflag != nullptr) free(m_codeflag);
	}

	/** tells if an address is in range. */
	bool MemoryModule::isInRange(ADDRESSTYPE address) const {
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
	bool MemoryModule::CheckAddress(ADDRESSTYPE address) const {
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
	bool MemoryModule::CheckAddressRange(ADDRESSTYPE address, ADDRESSTYPE size) const {
		if (!isInRange(address)) throw MemoryRangeException();
		if (!isInRange(address + size)) throw MemoryRangeException();
		if (m_content == nullptr) throw MemoryUnassignedException();
		return true;
	} ;

	/** Module virtual: displays on a given peripheral. */
	void MemoryModule::DisplayOn(Peripheral* /*peripheral*/) {}
	/** Programs a content from an Intel HEX file and sets as ROM.
	 *  @param hexfile the HEX file containing the code to copy in ROM
	 * 	@throw MemoryRangeException: size is too big and would make the range exceed $FFFF
	 *  @throw NoFileException: HEX file not found
	 * 	@return true if assignement was OK
	 */
	bool MemoryModule::SetROM(std::string hexfile) {

		// act as if we're in RAM
		if (SetRAM(hexfile)) {
			m_rw = false;// set as read-only
			return true;
		}
		return false;
	}

	/** Programs a content from an Intel HEX file and sets as RAM.
	 *	@param hexfile the HEX file containing the code to copy in ROM
	 *	@throw MemoryRangeException: size is too big and would make the range exceed $FFFF
	 *  @throw NoFileException: HEX file not found
	 *	@return true if assignement was OK
	 */
	bool MemoryModule::SetRAM(std::string hexfile) {
		
		// Setup a read buffer with enough place for a maximal sized HEX line
		const int maxreadlen = 256 * 2 + 11; // ':'->1 byte, len->2 bytes, addr->4 bytes, type->2 bytes, content->max 256*2 bytes, chk->2 bytes
		BYTE readbuffer[maxreadlen];
		
		// do a first pass on file to get the lowest address and max size
		FILE* f = fopen(hexfile.c_str(), "r");
		if (!f) throw NoFileException();
		m_start = 0;
		m_size = 0;
		bool first = true;
		// read next hex record
		while (fgets((char*)readbuffer, maxreadlen, f)) {
			// check end of records
			if (hexEOF(readbuffer)) break;
			// check type 0
			int type = hexType(readbuffer);
			if (type != 0) continue;
			// get address and size from record
			const ADDRESSTYPE newaddress = hexAddress(readbuffer);
			const ADDRESSSIZETYPE nbbytes = hexNbBytes(readbuffer);
			// first record?
			if (first) {
				// yes: set starting address and size
				m_start = newaddress;
				m_size = (ADDRESSSIZETYPE)nbbytes;
				first = false;
			} else {
				// compute the new last address
				const ADDRESSTYPE last = (ADDRESSTYPE)(std::max<unsigned int>(m_start + m_size, newaddress + nbbytes));
				// is the new starting address lower than current one?
				if (newaddress < m_start) {
					// yes: change it
					m_start = newaddress;
				}
				// and adjust size
				m_size = last - m_start;
			}
		}; // while
		
		// allocate the memory block
		if (m_content) free(m_content);
		m_content = (DATATYPE*)calloc(m_size,1);
		if (m_content == nullptr) throw OutOfMemoryException();
		
		// read again to store content
		fseek(f, 0, SEEK_SET);
		while (fgets((char*)readbuffer, maxreadlen, f)) {
			int type = hexType(readbuffer);
			if (type != 0) continue;
			if (hexEOF(readbuffer)) break;
			ADDRESSTYPE newaddress = hexAddress(readbuffer);
			// destination address in m_content is given address minus the starting address of this module
			hexStore(readbuffer, m_content + newaddress - m_start);
		}; // while
		fclose(f);
		m_end = (ADDRESSTYPE)(ADDRESSMASK & (m_start + m_size - 1));
		m_rw = true;
		return true;
	}

	/** Assigns a RAM range to this module. RAM is initialized to 0.
	 * @param start the start address in 0000-FFFF range
	 * @param size the number of bytes
	 * @throw MemoryRangeException: size is too big and would make the range exceed $FFFF
	 * @return true if assignement was OK
	 */
	bool MemoryModule::SetRAM(ADDRESSTYPE start, ADDRESSSIZETYPE size) {
		if (size > MEMMAXSIZE) throw MemoryRangeException();
		// allocate the memory block
		if (m_content) free(m_content);
		m_content = (DATATYPE*)calloc(size,1);
		if (m_content == nullptr) throw OutOfMemoryException();
		m_start = start;
		m_size = size;
		m_end = (ADDRESSTYPE)(ADDRESSMASK & (m_start + m_size - 1));
		return true;
	}

	//MARK: - MemoryReference sub class

	MemoryModule::MemoryReference::MemoryReference(DATATYPE* address, bool rw) { m_address = address; m_rw = rw; }

	// this[address] = value
	MemoryModule::MemoryReference& MemoryModule::MemoryReference::operator=( DATATYPE b ) {
		if (m_rw) *m_address = b;// no write if ROM
		return *this;
	}
	// this[address] = other[address]
	MemoryModule::MemoryReference& MemoryModule::MemoryReference::operator=( const MemoryReference& other ) {
		if (m_rw) *m_address = *other.m_address;
		return *this;
	}
	// value = this[address]
	MemoryModule::MemoryReference::operator DATATYPE() const {
		return *m_address;
	}

	//MARK: - Relays to MemoryReference services

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
	MemoryModule::MemoryReference MemoryModule::operator[](ADDRESSTYPE address) {
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
	void MemoryModule::Set(ADDRESSTYPE address, DATATYPE value) {
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
	DATATYPE MemoryModule::Get(ADDRESSTYPE address) {
		CheckAddress(address);
		return m_content[address - m_start];
	}

	/** Sets the code flag for a range of addresses.
	 *  @param address the starting address
	 *  @param size the number of bytes to mark with code flag
	 *	@param codeflags the flags to set
	 */
	void MemoryModule::SetCodeFlags(ADDRESSTYPE address, ADDRESSTYPE size, BYTE codeflags) {
		CheckAddressRange(address, size);
		for (ADDRESSTYPE a = address-m_start ; a <= address-m_start + size ; a++) {
			m_codeflag[a] = codeflags;
		}
	}

	//MARK: - Memory Attributes

	/** Tests if bits are set in a byte at an address.
	 * @param address the address
	 * @param bits the byte with bits ON for the bits to tests
	 * @return true if all bits were set at the address, false if any bit was not set
	 */
	bool MemoryModule::testBit(ADDRESSTYPE address, BYTE bits) const {
		CheckAddress(address);
		return (m_codeflag[address-m_start] & bits) == bits;
	}

	/** tests if the content at an address is marked as code. */
	bool MemoryModule::isCode(ADDRESSTYPE address) const   { return  testBit(address, codeFlagCODE); }
	/** tests if the content at an address is marked as data. */
	bool MemoryModule::isData(ADDRESSTYPE address) const   { return !testBit(address, codeFlagDATA); }
	/** tests if the content at an address is marked as text. */
	bool MemoryModule::isText(ADDRESSTYPE address) const   { return  testBit(address, codeFlagTEXT); }
	/** tests if the content at an address is marked as sprite. */
	bool MemoryModule::isSprite(ADDRESSTYPE address) const { return  testBit(address, codeFlagSPRITE); }
	/** tests if the content at an address is marked as address. */
	bool MemoryModule::isAddr(ADDRESSTYPE address) const   { return  testBit(address, codeFlagADDRESS); }
	/** tests if the content at an address is marked as visited. */
	bool MemoryModule::isVisited(ADDRESSTYPE address) const {  return testBit(address, codeFlagVISITED);  }

	void MemoryModule::markBits(ADDRESSTYPE address, ADDRESSTYPE length /*= 1*/, BYTE bits /*= codeFlagVISITED*/) {
		CheckAddressRange(address, length);
		m_codeflag[address-m_start] |= bits;
	}
	void MemoryModule::unmarkBits(ADDRESSTYPE address, ADDRESSTYPE length /*= 1*/, BYTE bits /*= codeFlagVISITED*/) {
		CheckAddressRange(address, length);
		m_codeflag[address-m_start] &= ~bits;
	}

	void MemoryModule::  markCode(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/)    {   markBits(address,length,codeFlagCODE); }
	void MemoryModule::  markData(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/)    { unmarkBits(address,length,codeFlagDATA); }
	void MemoryModule::  markVisited(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/) {   markBits(address,length,codeFlagVISITED); }
	void MemoryModule::unmarkVisited(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/) { unmarkBits(address,length,codeFlagVISITED); }
	void MemoryModule::  markText(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/)    {   markBits(address,length,codeFlagTEXT); }
	void MemoryModule::unmarkText(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/)    { unmarkBits(address,length,codeFlagTEXT); }
	void MemoryModule::  markSprite(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/)  {   markBits(address,length,codeFlagSPRITE); }
	void MemoryModule::unmarkSprite(ADDRESSTYPE address, ADDRESSTYPE length/*= 1*/)  { unmarkBits(address,length,codeFlagSPRITE); }
	void MemoryModule::  markAddr(ADDRESSTYPE address, ADDRESSTYPE length/*= 2*/)	  {   markBits(address,length,codeFlagADDRESS); }
	void MemoryModule::unmarkAddr(ADDRESSTYPE address, ADDRESSTYPE length/*= 2*/)	  { unmarkBits(address,length,codeFlagADDRESS); }

	/** Relocates the module to another address.
	 *	If the new address plus the module size exceeds the possible addresses, a MemoryRangeException is thrown.
	 */
	void MemoryModule::Relocate(ADDRESSTYPE address) {
		if (address + m_size > MEMMAXSIZE) throw MemoryRangeException();
		m_start = address;
		m_end = (ADDRESSTYPE)(m_start + m_size - 1);
	}


}; // namespace MUZ
