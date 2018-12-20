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

namespace MUZ {

	#define MODE_A16_D8	// 8-bit data wth 16-bit addressing (Z-80, 6502, 6809...)
	// #define MODE_A32_D16	// 16-bit data with 32-bit addressing (8086, 68000,..)
	
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
#error An addressing/data mode must be choosen
#endif
	
	const ADDRESSSIZETYPE MEMMAXSIZE = (ADDRESSSIZETYPE)ADDRESSMASK + 1;
	
	typedef std::vector<std::string> StringVector;
	
}; // namespace MUZ

#endif /* SRC_MUZ_TYPES_H_ */
