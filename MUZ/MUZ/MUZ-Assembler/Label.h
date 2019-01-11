//
//  Label.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 01/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Label_h
#define Label_h

#include <unordered_map>
#include <vector>
#include <limits.h>
#include "Name.h"

namespace MUZ {
	
	/** Structure to store a label from source file. */
	class Label : public Name
	{
	public:
		class CodeLine* 				codeline;		// code line deffining this label
		std::vector<ADDRESSTYPE>		addresses;		// addresses of this label, unique for global label, multiple for local
		bool							equate=false;	// this label is set by a .EQU
		std::vector<class CodeLine*>	referencers;	// Code lines where it is used
		bool							multiple=false;	// true to authorize more than one address
		
		void ClearAddresses() {
			addresses.clear();
		}
		
		void SetCodeLine(class CodeLine* cl) {
			codeline = cl;
		}
		
		/** Sets a label address value from a .EQU directive. This sets the label with a unique value and equate mode. */
		void Equate(unsigned int integer) {
			equate = true;
			multiple = false;
			ClearAddresses();
			addresses.push_back((ADDRESSTYPE)(integer & ADDRESSMASK));
		}
		
		/** Sets an address into the label. If the label accepts multiple values, it is added to eisting values, else it replaces the current value. */
		void SetAddress( unsigned int integer ) {
			if (equate) Equate(integer);
			if (!multiple) ClearAddresses();
			addresses.push_back((ADDRESSTYPE)(integer & ADDRESSMASK));
		}
		
		/** Tells if the label has assigned addresses. */
		bool empty() {
			return addresses.empty();
		}
		
		/** Adds a referencing code line to the list of referencers. */
		void AddReferencer(class CodeLine* codeline) {
			referencers.push_back(codeline);
		}
		
		/** Computes the 2-complement displacement value from a given address to the nearest available addresses.
		 	This value is used for JR and DJNZ relative jumps */
		int DeltaFrom(ADDRESSTYPE from) {
			// compute delta
			int delta = AddressFrom(from) - from;
			// then 2-complement
			return ((((int)MEMMAXSIZE) - delta) & ADDRESSMASK);
		}
		
		/** Returns the nearest available address. It returns INT_MAX if no address is assigned. */
		int AddressFrom(ADDRESSTYPE from) {
			// find closest address
			int closest = INT_MAX;
			for (auto addr: addresses) {
				if (abs(addr - from) <= abs(closest - from)) {
					closest = addr;
				}
			}
			return closest;
		}
	};

	typedef std::unordered_map<std::string, Label*> LabelMap;
} // namespace MUZ
#endif /* Label_h */
