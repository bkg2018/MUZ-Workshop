/*
 * MemoryModule.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */

#include "MemoryModule.h"

#include <stdio.h>
#include <stdlib.h>

#include "Types.h"
#include "StrUtils.h"

namespace MUZ {


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
			const ADDRESSTYPE nbbytes = hexNbBytes(readbuffer);
			// first record?
			if (first) {
				// yes: set starting address and size
				m_start = newaddress;
				m_size = nbbytes;
				first = false;
			} else {
				// compute the new last address
				const ADDRESSTYPE last = std::max<ADDRESSTYPE>(m_start + m_size, newaddress + nbbytes);
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
		m_end = m_start + m_size - 1;
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
		m_end = m_start + m_size - 1;
		return true;
	}

}; // namespace MUZ
