/*
 * types.h
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_TYPES_H_
#define SRC_MUZ_TYPES_H_

#include <limits.h>
#include <string>
#include <vector>

#include "Config.h"

namespace MUZ {

	// Types for memory and content
	typedef unsigned char BYTE;
	typedef unsigned short WORD;	// should be 16-bits
	
#if UINT_MAX == 65535
	typedef unsigned long DWORD;	// long should be 32-bits
#elif UINT_MAX == 4294967295
	typedef unsigned int DWORD;	// int is 32 bits
#else
#error "WHAT?"
#endif
	
#ifdef MODE_A16_D8
	typedef WORD ADDRESSTYPE; // 16-bits
	const int ADDRESSMASK = 0xFFFF;
	typedef unsigned int ADDRESSSIZETYPE;
	typedef BYTE DATATYPE;
	const int DATAMASK = 0xFF;
#elif MODE_A32_D16
	typedef DWORD ADDRESSTYPE; // 32-bits
	const int ADDRESSMASK = 0xFFFFffff;
	typedef unsigned int ADDRESSSIZETYPE;
	typedef WORD DATATYPE;
	const int DATAMASK = 0xFFFF;
#else
#error An addressing/data mode must be choosen in Config.h
#endif
	
	const ADDRESSSIZETYPE MEMMAXSIZE = (ADDRESSSIZETYPE)ADDRESSMASK + 1;
	
	typedef std::vector<std::string> StringVector;

	/** Definition for a code or data section. */
	struct Section
	{
	private:
		ADDRESSTYPE m_start = ADDRESSMASK;			// starting address
		ADDRESSTYPE m_curaddress = 0;		// current address for storage
		ADDRESSTYPE m_end = 0;			// last address, updated by each write
		bool orgset = false;			// no starting address yet
		std::string m_name;				// name for this section

	public:
		bool saved = false;				// false -> contains volatile storage, not to be written in HEX output

		enum MERGINGTYPE {
			mergingNOT,			// new range totally apart, NOT MERGED
			mergingBEFORE,			// new range right before
			mergingAFTER,			// new range right after
			mergingOVERWRITE,		// new range partially or totally overwriting
		};
		
		ADDRESSTYPE start() const { return m_start; }
		ADDRESSTYPE end() const { return m_end; }
		ADDRESSTYPE curaddress() const { return m_curaddress; }
		std::string name() const { return m_name; }

		/** Clear status. */
		void Reset() {
			m_curaddress = m_start;
			orgset = false;
		}
		
		/** Sets the name, can only be done once. */
		void SetName(std::string name) {
			if (m_name.empty()) {
				m_name = name;
			}
		}
		
		/** Sets the starting address. */
		void SetOrg(ADDRESSTYPE address) {
			// first creation?
			if (orgset) {
				// no, keep lowest starting address
				m_start = std::min<ADDRESSTYPE>(m_start, address);
			} else {
				m_start = address;
			}
			m_curaddress = address;
			m_end = std::max<ADDRESSTYPE>(m_end, address);
			orgset = true;
		}
		
		/** Sets the current address. */
		void SetAddress(ADDRESSTYPE address) {
			if (!orgset) {
				//TODO: WHAT? section cannot receive an address if org no set
			} else {
				m_curaddress = address;
				m_start = std::min<ADDRESSTYPE>(m_start, address);
				m_end = std::max<ADDRESSTYPE>(m_end, address);
			}
		}
		
		/** Tells if an address range is contained in this section. */
		bool contains(ADDRESSTYPE s, ADDRESSTYPE e) {
			if (m_start <= s && e <= m_end) return true;
			return false;
		}
		
		/** Expands the section using an address range, unless this range doesn't touch this section.
		 	Returns a merging code from above. mergingNOT means the address range could not be
		 	merged with this section, other codes tell the range has been merged.
		 */
		MERGINGTYPE extendedBy( ADDRESSTYPE s, ADDRESSTYPE e) {
			if (s == m_end + 1) {
				m_end = e;
				return mergingAFTER;
			}
			if (e == m_start - 1) {
				m_start = s;
				return mergingBEFORE;
			}
			if (e < m_start || m_end < s) {
				return mergingNOT;
			}
			// partial or total overwrite
			m_start = std::min<int>(m_start, s);
			m_end = std::max<int>(m_end, e);
			return mergingOVERWRITE;
		}
	};
	

}; // namespace MUZ

#endif /* SRC_MUZ_TYPES_H_ */
