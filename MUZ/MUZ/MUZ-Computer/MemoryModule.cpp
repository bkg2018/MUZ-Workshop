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

namespace MUZ {

	// numeric value of a char. only '0'-9'  'A'-'F' and 'a'-'f' have a value
	static BYTE asciinum[256] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00 - 0f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10 - 1f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20 - 2f
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, // 30 - 3f
			0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 40 - 4f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 50 - 5f
			0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60 - 6f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 70 - 7f
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	// helper function to convert a 2 chars hex at an address to a byte value
	inline BYTE hex2byte(const BYTE* p) {
		int value = asciinum[(int)*(p+1)] + (asciinum[(int)*p] << 4);
		return (BYTE)(value & 0xFF);
	}

	// practical consts for HEX intel format, these are byte offset of fields starting with ':' at offset 0
	const int HEXOFS_SIZE = 1;		// offset to the 2-digits byte size
	const int HEXOFS_ADDRESS = 3;	// offset of the 4-digits address
	const int HEXOFS_TYPE = 7;		// offset of the 1-digit record type
	const int HEXOFS_CONTENT = 9;	// offset of the content

	// returns the type of HEX record - returns -1 if not a valid HEX
	inline int hexType(const BYTE* hexline) {
		if (*hexline != ':') return -1;
		return (int)hex2byte(hexline + HEXOFS_TYPE);
	}

	// returns number of bytes in an Intel HEX record - returns 0 if not a type 0 record
	inline int hexNbBytes(const BYTE* hexline) {
		if (*hexline != ':') return 0;
		int type = hex2byte(hexline + HEXOFS_TYPE);
		if (type == 0) return hex2byte(hexline + HEXOFS_SIZE);
		return 0;
	}

	// tells if the record is an end of file
	inline bool hexEOF(const BYTE* hexline) {
		if (*hexline != ':') return false;
		int type = hex2byte(hexline + HEXOFS_TYPE);
		return (type == 1);
	}

	// returns the address in an hex record - returns 0 if not a type 0 record
	inline ADDRESSTYPE hexAddress(const BYTE* hexline) {
		if (*hexline != ':') return 0;
		int type = hex2byte(hexline + HEXOFS_TYPE);
		if (type == 0) {
			BYTE h = hex2byte(hexline + HEXOFS_ADDRESS);
			BYTE l = hex2byte(hexline + HEXOFS_ADDRESS + 2);
			return ((int)h << 8) + (int)l;
		}
		return 0;
	}

	// stores the hex content in a given buffer
	inline void hexStore(const BYTE* hexline, DATATYPE* buffer) {

		BYTE* psrc = (BYTE*)hexline + HEXOFS_CONTENT ;
		DATATYPE* pdest = buffer;
		int nbbytes = hexNbBytes(hexline);
		for (psrc = (BYTE*)(hexline + HEXOFS_CONTENT) ; nbbytes > 0 ; nbbytes --) {
			*pdest = (DATATYPE)hex2byte(psrc);
			psrc += 2;
			pdest += 1;
		}
	}

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
