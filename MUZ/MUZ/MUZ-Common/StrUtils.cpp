//
//  StrUtils.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 04/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include "StrUtils.h"

using std::string;

/** Extensions to std namespace. */
namespace std {
	/** returns a string with decimal address */
	string to_string(MUZ::ADDRESSTYPE address)
	{
		unsigned int value = address;
		string result;
		while (value > 0) {
			result = to_string(value % 10) + result;
			value = value / 10;
		};
		return result;
	}

	/** returns uppercased string .*/
	string to_upper(string s)
	{
		string result;
		for (auto c : s) {
			result += upperchar(c);
		}
		return result;
	}
} // namespace std

/** Converts a base N number to unsigned int, */
unsigned int base_to_unsigned(string s, int base)
{
	unsigned int value = 0;
	if (base > 10) {
		char cmaxupper = 'A' + base - 10;
		char cminupper = 'A' - 10;
		char cmax = 'a' + base - 10;
		char cmin = 'a' - 10;
		for (unsigned char c: s) {
			if (c >= 'a' && c<= cmax) value = (c - cmin) + base * value;
			else if (c >= 'A' && c<= cmaxupper) value = (c - cminupper) + base * value;
			else value = (c - '0') + base * value;
		}
		return value;
	}
	for (unsigned char c: s) {
		value = (c - '0') + base * value;
	}
	return value;
}

/** Converts an hexadecimal number to unsigned int, */
unsigned int hex_to_unsigned(string s)
{
	unsigned int value = 0;
	for (unsigned char c: s) {
		if (c >= 'a' && c<= 'f') value = (c - 'a' + 10) + 16 * value;
		else if (c >= 'A' && c<= 'F') value = (c - 'A' + 10) + 16 * value;
		else value = (c - '0') + 16 * value;
	}
	return value;
}

/** Converts a binary number to unsigned int, */
unsigned int bin_to_unsigned(string s)
{
	unsigned int value = 0;
	for (unsigned char c: s) {
		value = (c - '0') + 2 * value;
	}
	return value;
}

/** Converts an octal number to unsigned int, */
unsigned int oct_to_unsigned(string s)
{
	unsigned int value = 0;
	for (unsigned char c: s) {
		value = (c - '0') + 8 * value;
	}
	return value;
}

/** Converts a decimal number string to unsigned int. */
unsigned int dec_to_unsigned(string s)
{
	unsigned int value = 0;
	for (unsigned char c: s) {
		value = (c - '0') + 10 * value;
	}
	return value;
}

/** Converts an address to an hexa string. */
const char hexchar[17] = "0123456789ABCDEF";
string address_to_hex(MUZ::ADDRESSTYPE address)
{
	unsigned int value = address;
	char result[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char* last = result + sizeof(result) - 2; // points to last digit before ending 0
	while (value > 0) {
		*last = hexchar[value % 16];
		value = value / 16;
		last -= 1;
	};
	string sresult = string(last);
	return sresult;
}

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
string unescape(string s)
{
	string result;
	int i = 0;
	int len = (int)s.length();
	bool inescape = false;
	for (i = 0 ; i < len ; i++) {
		char c = s[i];
		if (inescape) {
			// direct characters
			if (c =='t') {
				result += "\x09";
				inescape = false;
			} else if (c =='n') {
				result += "\x0D";
				inescape = false;
			} else if (c =='r') {
				result += "\x0A";
				inescape = false;
			} else if (c =='\\') {
				result += "\\";
				inescape = false;
			} else if (c =='h') {
				result += "\x08";
				inescape = false;
			} else if (c =='\'') {
				result += "'";
				inescape = false;
			} else if (c =='"') {
				result += "\"";
				inescape = false;
			} else if (c =='0') {
				// start octal sequence
				string number;
				i = i + 1;
				while (i < len) {
					c = s[i];
					if (c >= '0' && c <= '7')
						number += c;
					else
						break;
				}
				char s[16];
				snprintf(s, 16, "%c", oct_to_unsigned(number) & 0xFF);
				result += s;
				inescape = false;
			} else if (c =='x') {
				// start hexa sequence
				string number;
				i = i + 1;
				while (i < len) {
					c = s[i];
					if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
						number += c;
					else
						break;
				}
				char s[16];
				snprintf(s, 16, "%c", hex_to_unsigned(number) & 0xFF);
				result += s;
				inescape = false;
			} else if (c <='1' && c <= '9') {
				// start decimal sequence
				string number;
				i = i + 1;
				while (i < len) {
					c = s[i];
					if (c >= '0' && c <= '9')
						number += c;
					else
						break;
				}
				char s[16];
				snprintf(s, 16, "%c", dec_to_unsigned(number) & 0xFF);
				result += s;
				inescape = false;
			} else {
				// unknown escape, insert a ?
				result += "?";
				inescape = false;
			}
		} else if (c == '\\') {
			inescape = true;
		} else {
			result += c;
		}
	}
	return result;
}

/** Read a line of file until a null, a \r or a \r\n is found. */
bool fgetline(MUZ::BYTE** buffer, int *length, FILE* f)
{
	if (feof(f)) return false;
	MUZ::BYTE c;
	long offset = ftell(f);
	int size = 0;
	while (fread(&c, 1, 1, f) == 1) {
		if (c == 0) break; // ending null
		if (c == '\n') break; // CR = UNIX end of line
		if (c == '\r') { // LF: end of line on old macs, but maybe followed by CR
			offset = ftell(f);
			if (fread(&c, 1, 1, f) != 1) break; // EOF
			if (c == '\n') break; // CR+LF
			// '\r': alone: back to prev character and EOL
			fseek(f, offset, SEEK_SET);
			break;
		}
		// other characater: store it
		size += 1;
		*buffer = (MUZ::BYTE*)realloc(*buffer, size + 1); // reserve place for ending null
		*(*buffer + size - 1) = c;
	};
	// add ending null
	*(*buffer + size) = 0; // ok: size+1 allocated
	*length = size;
	return true;
}

#ifdef WIN32
const char NORMAL_DIR_SEPARATOR = '\\';
const char OTHER_DIR_SEPARATOR = '/';
const char ALTERNATE_ROOTDIR = '\0' ;	// no home directory on windows
#else
const char NORMAL_DIR_SEPARATOR = '/';
const char OTHER_DIR_SEPARATOR = '\\';
const char ALTERNATE_ROOTDIR = '~';		// home directory
#endif

/** Split a file path into path and filename parts. */
bool splitpath(string filepath, string& path, string& name)
{
	// replace alternative path separators and remember last separator position
	string fp = filepath;
	int lastslashpos = -1;
	for (size_t i = 0 ; i < fp.length() ; i++) {
		if (fp[i] == OTHER_DIR_SEPARATOR) fp[i] = NORMAL_DIR_SEPARATOR;
		if (fp[i] == NORMAL_DIR_SEPARATOR) {
			lastslashpos = (int)i;
		}
	}
	// no path?
	if (lastslashpos <= 0) {
		path.clear();
		name = fp;
	} else {
		path = fp.substr(0, lastslashpos); 	// exclude last slash
		name = fp.substr(lastslashpos + 1);
	}
	return true;
}

/** Returns the uppercase of a character if it is a letter. */
char upperchar(char c)
{
	return (c >= 'a' && c <= 'z') ? (c + 'A') - 'a' : c;
}

/** Trim spaces at the end */
void strtrimright(std::string& s)
{
	int pos = ((int)s.length())-1;
	while ((pos >= 0) && (s[pos] == ' ' || s[pos] == '\t')) {
		pos -= 1;
	}
	s = s.substr(0,pos+1);
}

/** returns a string with a number of spaces */
std::string spaces(int number)
{
	string result;
	for (int i = 0 ; i < number ; i++) {
		result += " ";
	}
	return result;
}
