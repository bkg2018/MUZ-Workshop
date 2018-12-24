//
//  AddressZone.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 01/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef AddressZone_h
#define AddressZone_h

#include "Types.h"
#include <stdexcept>

namespace MUZ {

	/** Structure to reference a line in a source file. */
	struct LineReference
	{
		int				file;	// file reference
		int				line;	// line number in file
	};
	
	/** Structure for an address zone. */
	struct AddressZone
	{
		ADDRESSTYPE		start;	// starting address
		ADDRESSTYPE		size;	// zone size
		std::vector<LineReference> lines;	// references of each line included in this zone
		DATATYPE*		zone = nullptr;
		
		// low level increment a zone relative to current start and size - beware, no check
		void _inc(ADDRESSTYPE a, ADDRESSTYPE s) {
			if (zone == nullptr) return;
			for (ADDRESSTYPE addr = a-start ; s > 0 ; addr++, s--) {
				zone[addr]++;
			}
		}
		
		AddressZone() : start(0), size(0), zone(nullptr) {}
		
		AddressZone(ADDRESSTYPE a, ADDRESSTYPE s) { Mark(a,s); }
		
		DATATYPE& operator[](ADDRESSTYPE a) {
			if (zone == nullptr) throw std::out_of_range("AddressZone[] has no range");
			if (a < start) throw std::out_of_range("AddressZone[] address is too low");
			if (a >= start + size) throw std::out_of_range("AddressZone[] address is too high");
			return zone[a - start];
		}
		
		void Reset(ADDRESSTYPE a, ADDRESSTYPE s) {
			if (zone == nullptr) return;
			free(zone);zone = nullptr;
			Mark(a,s);
		}
		
		void Mark(ADDRESSTYPE a, ADDRESSTYPE s) {
			if (zone == nullptr) {
				zone = (DATATYPE*)calloc(s, sizeof(DATATYPE));
				start = a;
				size = s;
				_inc(a,s);
				return;
			}
			ADDRESSTYPE end = start + size - 1;
			if (a < start) {
				// new lower starting address, we must allocate a new buffer and copy back old content
				if (a + s > end) {
					// Case 1: new zone larger
					//  ++++++*********++++++  a, s
					//  ......*********        start, size
					DATATYPE* newzone = (DATATYPE*)calloc(s, sizeof(DATATYPE));	// allocate new buffer all set to 0
					memmove(newzone + start - a, zone, size);		// copy previous data
					free(zone);zone = newzone;						// replace
					start = a; size = s;							// new start and size
				} else {
					// Case 2: partial overwriting before
					//  ++++++****             a, s
					//  ......****+++++        start, size
					// Case 3: start before
					//  ++++                   a, s
					//  ......+++++++++        start, size
					DATATYPE* newzone = (DATATYPE*)calloc(end - a + 1, sizeof(DATATYPE));	// allocate new buffer all set to 0
					memmove(newzone + start - a, zone, size);		// copy previous data
					free(zone);zone = newzone;						// replace
					start = a; size = end - a + 1;					// new start and size
				}
			} else {
				// simply reallocate existing buffer  if needed
				if (a + s > end) {
					//           ******+++     a, s
					//        +++******...     start, size

					//                  ++++   a, s
					//        +++++++++.....   start, size
					zone = (DATATYPE*)realloc(zone, a + s - start);	// allocate new buffer all set to 0
					memset((char*)(zone + size), 0, a + s - (start + size));	// set new alloc to zero
					size = a + s - start;							// new size
				} else {
					//          ****           a, s
					//        ++****+++        start, size
					// nothing to do
				}
			}
			_inc(a,s);
		}
		
	};
	
	
} // namespace MUZ

#endif /* AddressZone_h */
