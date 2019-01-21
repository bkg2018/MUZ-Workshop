//
//  Section.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 11/01/2019.
//  Copyright © 2019 Francis Pierot. All rights reserved.
//

#ifndef Section_h
#define Section_h

#include "Types.h"

namespace MUZ {
	/** Definition for a code or data section. A section has a name and a current address set by calling SetAddress(). It can store any number of address ranges. The SetAddress function will find an existing appropriate address range or create a new one when needed. The SetOrg call can set the current address without computing address ranges, which is useful to set the section before it is actually receiving any code.
	 	The Section class DO NOT store any content, it stores addressing areas to group them under a name.
	 */
	class Section
	{
	public:
		/** Array of all the address ranges stored in the Section. */
		std::vector<AddressRange>	m_ranges;
		/** Current address for the section. */
		ADDRESSTYPE 				m_curaddress = 0;
		/** Index of the current address range for the current address. */
		size_t						m_currange = 0;
		/** Name of the section. This is the same as the m_sections index in Assembler. */
		std::string					m_name;
		/** HEX output attribute, false for DATA sections by default.*/
		bool						m_saved = false;
		
	public:
		/** Tests if an address merges to the left of a range.*/
		bool mergesLeft(ADDRESSTYPE address, size_t rangeindex);
		
		/** Tests if an address merges to the right of a range.*/
		bool mergesRight( ADDRESSTYPE address, size_t rangeindex);
		
		/** Tests if an address is inside a range. */
		bool isInside(ADDRESSTYPE address, size_t rangeindex);
		
		/** Tests if this section contains an address range in its ranges and returns its index, returns -1 if not. */
		int FindRange(ADDRESSTYPE s, ADDRESSTYPE e);
		
		/** Enters an address into an existing or new address range. Merges two existing ranges when address links them. */
		void SetAddress(ADDRESSTYPE address);
		
		/** Returns the lowest starting address of all ranges. */
		ADDRESSTYPE absoluteStart();
		
		/** Returns the highest ending address of all ranges. */
		ADDRESSTYPE absoluteEnd();
		
		/** Returns starting address for current range. */
		ADDRESSTYPE start() const ;
		
		/** Returns ending address for current range. */
		ADDRESSTYPE end() const ;
		
		/** Returns current address for current range. */
		ADDRESSTYPE curaddress() const;
		
		/** Returns this section name. */
		std::string name() const;
		
		/** Returs true if the section must be saved in HEX output. */
		bool save() const;
		
		/** Clear status. */
		void Reset();
		
		/** Sets the name, can only be done once. */
		void SetName(std::string name);
		
		/** Sets the current address. */
		void SetOrg(ADDRESSTYPE address);
		
		/** Sets the SAVE attribute to enable/disable the HEX output. */
		void SetSave(bool yes);
		
	};
}

#endif /* Section_h */
