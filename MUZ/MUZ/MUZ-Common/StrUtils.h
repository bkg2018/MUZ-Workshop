//
//  StrUtils.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 04/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef StrUtils_h
#define StrUtils_h

#include "Types.h"
#include <string>

/** add to_string() on MUZ addresses, extension to std namespace */
namespace std {
	string to_string(MUZ::ADDRESSTYPE address);	
	string to_upper(string);
} // namespace std

/** Convert a base N number to unsigned int, */
unsigned int base_to_unsigned(std::string s, int base);

/** Convert an hexadecimal number to unsigned int, */
unsigned int hex_to_unsigned(std::string s);

/** Convert a binary number to unsigned int, */
unsigned int bin_to_unsigned(std::string s);

/** Convert an octal number to unsigned int, */
unsigned int oct_to_unsigned(std::string s);

/** Convert a decimal number string to unsigned int, */
unsigned int dec_to_unsigned(std::string s);

/** Convert an address to an hexa string. */
std::string address_to_hex(MUZ::ADDRESSTYPE address);

/** Unescapes the escape sequences in a character string.
 Escape sequences begin with a \ backshlash and are followed by:
 '\t' : replaced by the $09 tabulation character
 '\n' : replaced by 0x0D carriage return
 '\r' : replaced by 0x0A line feed
 '\\' : replaced by a single backslash
 '\h' : replaced by 0x08 del character
 '\NNN': replaced by the character with decimal code NNN if doesn't start with a 0 (max 255)
 '\xHH': replaced by the character with hexadecimal code 0xHH (max 0xFF)
 '\0NNN' : replaced by the character with octal code 0NNN (max 0377)
 */
std::string unescape(std::string s);

/** Trim spaces at the end */
void strtrimright(std::string& s);

/** returns a string with a number of spaces */
std::string spaces(int number);

/** Read a line of file until a null, a \r or a \r\n is found. */
bool fgetline(MUZ::BYTE** buffer, int *length, FILE* f);

/** Split a file path into path and filename parts. */
bool splitpath(std::string filepath, std::string& path, std::string& name);

/** Returns the uppercase of a character if it is a letter. Returns unchanged character for anything else. */
char upperchar(char c);

extern const char NORMAL_DIR_SEPARATOR; // Windows backslash and UNIXes slash
extern const char OTHER_DIR_SEPARATOR;	// slash allowed on Windows, and backslash will be changed to slash on UNIXes
extern const char ALTERNATE_ROOTDIR; 	// '~' on UNIXes, '\0' elsewhere

#endif /* StrUtils_h */