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

	struct AddressRange {
		ADDRESSTYPE start = ADDRESSMASK;	// starting address
		ADDRESSTYPE end = 0;				// last address, updated by each write
	};
	
	/** Definition for a code or data section. */
	class Section
	{
	public:
		std::vector<AddressRange>	m_ranges;			// All the address ranges in this section
		ADDRESSTYPE 				m_curaddress = 0;	// current address
		int							m_currange = 0;		// current address range
		std::string					m_name;				// name of this section

	public:
		bool saved = false;					// false -> contains volatile storage, not to be written in HEX output

		enum MERGINGTYPE {
			mergingNOT,						// new range totally apart, NOT MERGED
			mergingBEFORE,					// new range right before
			mergingAFTER,					// new range right after
			mergingOVERWRITE,				// new range partially or totally overwriting
		};

		/** Tests if an address merges to the left of a range.*/
		bool mergesLeft(ADDRESSTYPE address, int rangeindex) {
			return (address + 1 == m_ranges[rangeindex].start);
		}
		/** Tests if an address merges to the right of a range.*/
		bool mergesRight( ADDRESSTYPE address, int rangeindex) {
			return (m_ranges[rangeindex].end + 1 == address);
		}
		/** Tests if an address is inside a range. */
		bool isInside(ADDRESSTYPE address, int rangeindex) {
			return ((m_ranges[rangeindex].start <= address) && (address <= m_ranges[rangeindex].end));
		}
		
		/** Tests if this section contains an address range in its ranges and returns its index, returns -1 if not. */
		int FindRange(ADDRESSTYPE s, ADDRESSTYPE e) {
			for (int i = 0 ; i < m_ranges.size() ; i++) {
				if (m_ranges[i].start <= s && e <= m_ranges[i].end) return i;
			}
			return -1;
		}

		/** Enters an address into an xxisting or new address range. Merges two existing ranges when address links them. */
		void SetAddress(ADDRESSTYPE address) {
	
			// All pathes set the current address
			m_curaddress = address;
			
			// try all ranges
			for (int i  = 0 ; i < m_ranges.size() ; i++) {
				
				// inside this range?
				if (isInside(address, i)) {
					m_currange = i;
					return;
				}
				// merges to the left of [i]?
				if (mergesLeft(address, i)) {
					// check if it links with the end of another range
					for (int j = 0 ; j < m_ranges.size() ; j++) {
						if (i == j) continue;// skip self test
						if (mergesRight(address, j)) {
							// [j].end - a - [i].start
							if (i < j) {
								// merge in range[i] and delete [j]
								m_ranges[i].start = m_ranges[j].end;
								m_ranges.erase(m_ranges.begin() + j);
								m_currange = i;
								return;
							}
							// merge in range[j] and delete [i]
							m_ranges[j].end = m_ranges[i].end;
							m_ranges.erase(m_ranges.begin() + i);
							m_currange = j;
							return;
						}
					}
					// No link between two ranges: simply expand to the left of range[i]
					m_ranges[i].start = address;
					m_currange = i;
					return;
				} // merge left (i)
				// merges to the right of [i]?
				if (mergesRight(address, i)) {
					// check if it links with the start of another range
					for (int j = 0 ; j < m_ranges.size() ; j++) {
						if (i == j) continue;// skip self test
						if (mergesLeft(address, j)) {
							// [i].end - a - [j].start
							if (i < j) {
								// merge in range[i] and delete [j]
								m_ranges[i].end = m_ranges[j].end;
								m_ranges.erase(m_ranges.begin() + j);
								m_currange = i;
								return;
							}
							// merge in range[j] and delete [i]
							m_ranges[j].start = m_ranges[i].start;
							m_ranges.erase(m_ranges.begin() + i);
							m_currange = j;
							return;
						}
					}
					// No link between two ranges: simply expand to the right of range[i]
					m_ranges[i].end = address;
					m_currange = i;
					return;
				} // merge right(i)
			} // for all ranges (i)
			
			// no range found, create a new one for this address
			AddressRange range;
			range.start = address;
			range.end = address;
			m_ranges.push_back(range);
			m_currange = (int)m_ranges.size() - 1;
		}
		
		/** Returns the lowest starting address of all ranges. */
		ADDRESSTYPE absoluteStart() {
			if (m_ranges.size()==0) return 0;
			ADDRESSTYPE start = ADDRESSMASK;
			for (auto &range : m_ranges) {
				if (range.start < start) start = range.start;
			}
			return start;
		}
		
		/** Returns the highest ending address of all ranges. */
		ADDRESSTYPE absoluteEnd() {
			ADDRESSTYPE end = 0;
			for (auto &range : m_ranges) {
				if (range.end > end) end = range.end;
			}
			return end;
		}

		/** Returns starting address for current range. */
		ADDRESSTYPE start() const {
			if (m_ranges.size()==0) return 0;
			return m_ranges[m_currange].start;
		}
		/** Returns ending address for current range. */
		ADDRESSTYPE end() const {
			if (m_ranges.size()==0) return 0;
			return m_ranges[m_currange].end;
		}
		/** Returns current address for current range. */
		ADDRESSTYPE curaddress() const { return m_curaddress; }
		/** Returns this section name. */
		std::string name() const { return m_name; }

		/** Clear status. */
		void Reset() {
			m_currange = INT_MAX;
			m_curaddress = ADDRESSMASK;
			for (int i = 0 ; i < m_ranges.size() ; i++) {
				if (m_ranges[i].start < m_curaddress) {
					m_currange = i;
					m_curaddress = m_ranges[i].start;
				}
			}
		}
		
		/** Sets the name, can only be done once. */
		void SetName(std::string name) {
			if (m_name.empty()) {
				m_name = name;
			}
		}
		
		/** Sets the current address. */
		void SetOrg(ADDRESSTYPE address) {
			m_curaddress = address; // do NOT touch m_ranges
		}
		
	};
	

}; // namespace MUZ

#endif /* SRC_MUZ_TYPES_H_ */
