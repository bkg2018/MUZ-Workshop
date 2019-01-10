//
//  MUZ-Assembler.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 01/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <set>
#include "StrUtils.h"
#include "FileUtils.h"
#include "Exceptions.h"
#include "Assembler.h"
#include "Parser.h"

#include "All-Directives.h"
#include "Z80-Instructions.h"
#include "Z80-Operands.h"

using MUZ::BYTE;
using MUZ::ADDRESSTYPE;
using std::vector;
using std::string;

#include <sstream>
#include <iostream>
#include <stdio.h>

namespace MUZ {
	
	//MARK: - Listing helper functions
	
	// Build listing line helper
	// places the various components aligned on template
	//
	// 0044: 00 00 00 00   0234              .DW  0,0            ;0x0044  Not used
	// ADDR: XX XX XX XX   NNNN  <source>
	// 012345678901234567890123456
	//
	// ADDR: at pos 0                       : 4 bytes hexa address, blank for non assembled
	// XX    at pos 6/7 9/10 12/13 15/16    : up to 4 bytes of code, can be continued on next line for long data directives (.DB etc)
	// NNNNN at pos 20-23                   : line number with 4 digits, left 0-padded
	// Source at pos 26                     : source text
	
	// Returns the 12 characters part of byte codes
	string buildCodes(vector<BYTE> code, int firstcode, int nbcodes)
	{
		// 0 codes: all spaces
		if (nbcodes == 0) return spaces(12);
		// 1 to 4 codes
		string result;
		int lastindex = std::min<int>(firstcode + nbcodes - 1, (int)code.size() - 1);
		// (0,4) -> codes 0 to 3
		// (4,4) -> codes 4 to 7
		// (0,2) -> codes 0 to 1 then "   "
		// (4,3) -> codes 4 to 6 then ".. "
		for (size_t index = firstcode ; index <= firstcode + 3 ; index++) {
			if (index <= lastindex) {
				result += data_to_hex(code[index]) + " "; 	// normal code display
			} else if (index < code.size()) {
				result += "...";									// show code presence but not value
			} else {
				result += spaces(3);							// no more code available, all space
			}
		} // 4*3 characters added
		return result;
	}
	
	/** Builds one listing line from a code index.
	 @param address the starting address to put first
	 @param code the vector of code containing all code
	 @param firstcode the index of the first code to display
	 @param nbcodes the number of codes to display, frfom 0 to 4 maximum
	 @param label a label containing a value to display instead of 'address', or nullptr to ignore
	 @param file the file number of the code source line, -1 to ignore
	 @param line the line number of the code source line, -1 to ignore
	 @param source the source code to display after address, code and line number
	 @return the string containing the listing line
	 */
	string buildOneListingLine(ADDRESSTYPE address, vector<BYTE> code, int firstcode = 0, int nbcodes = 4, Label* label = nullptr, int file = -1, int line = -1, string source = "")
	{
		string thisline;
		if (code.empty() && label != nullptr && label->codeline && label->codeline->file == file && label->codeline->line == line) {
			thisline = address_to_hex(label->AddressFrom(address)) + ": ";
		} else {
			thisline = (code.empty()) ? spaces(6) : address_to_hex(address) + ": ";
		}
		
		//prepare line number
		string linenum = "";
		if (line <0) {
			linenum = spaces(4);
		} else {
			linenum = std::to_string(line);
			while (linenum.length() < 4) {
				linenum = "0" + linenum;
			}
		}
		
		// 0 byte : one line, all space instead of code, then line number and source
		// 02EE:               0033  ConInitialise:
		if (code.empty()) {
			return thisline + buildCodes(code, 0, 0) + spaces(2) + linenum + spaces(2) + source;
		}
		
		// 1 to 4 bytes: one line with all elements and codes 0..3/2/1/0
		// 1 to 8 bytes: one line with all elements and codes 0..3, one line with codes 4..7 and no line number nor source
		// 	02CB: 31 2E         0716              .DB  '0'+kVersMajor,'.'
		//	02C3: 56 65 72 73   0715              .DB  "Version "
		//	02C7: 69 6F 6E 20   1089
		// 1 to 9 bytes: one line with all elements and codes 0..3, one line with codes 4..5 and ".." and no line nor source
		
		// get the code part
		//int nbcodes = std::min<int>(4, (int)code.size() - firstcode);
		// 0 to 4 bytes -> 0 to 3
		// 5 to 8 bytes -> 4 if starting at 0, 0 to 3 if starting at 4 ...
		thisline += buildCodes(code, firstcode, nbcodes);;
		
		if (firstcode == 0) {
			// add line number
			thisline += spaces(2) + linenum;
			thisline += spaces(2) + source;
		}
		
		return thisline;
	}
	
	/** Build one or more lines of listing for the codes given.
	 @param codeline the assembled code line containing code to list.
	 @param all true to list all bytes , false to limit to 2 lines of listing, with a "..." ellipsis for code not listed
	 @return a vector of strings to display/list in order
	 */
	vector<string> buildListingLines(CodeLine& codeline, bool all)
	{
		vector<string> result;
		vector<BYTE>& code = codeline.code;
		int codesize = (int)code.size();
		
		// first line complete with 0 to 4 bytes of code
		result.push_back(buildOneListingLine(codeline.address, code, 0, std::min<int>(4, codesize), codeline.label, codeline.file, codeline.line, codeline.source));
		
		// second line depend on the number of codes
		if (codesize >= 5 && codesize <= 8) {
			// second line with 1 to 4 bytes of code
			result.push_back(buildOneListingLine(codeline.address + 4, code, 4, codesize - 4));
		} else if (codeline.code.size() >= 9) {
			// rest of listing
			if (all) {
				// each packet of 4 code bytes
				for (int start = 4 ; start < codeline.code.size() ; start += 4) {
					result.push_back(buildOneListingLine(codeline.address + start, code, start, 4));
				}
			} else {
				// one line only with 1 to 3 bytes of code bytes 4 to 7, then "..." if there's more
				result.push_back(buildOneListingLine(codeline.address + 4, code, 4, 3));
			}
		}
		return result;
	}
	
	
	//MARK: - Assembler::SourceFile structure
	
	/** Gets the root up the whole parent SourceFile tree. Should return the structure for the main source file. */
	Assembler::SourceFile* Assembler::SourceFile::Root()
	{
		SourceFile* root = this->parent;
		while (root && root->parent)
			root = root->parent;
		return root;
	}
	
	/** Lets a SourceFile set itself from a given file path and a parent specification. */
	bool Assembler::SourceFile::Set(std::string file, SourceFile *parentfile) {
		// included files may be relative to the path of their parent, check this
		included = (parentfile != nullptr);
		if (included) {
			bool absolute = false;
#ifdef WIN32
			// Separate the possible prefixes on Windows
			
			// UNC long names on Windows
			if (file.substr(0,4) == "\\\\?\\")) {
				fileprefix = "\\\\?\\";
				file = file.substr(4); // cut UNC prefix
			}
			// Windows drive unit letter followed by a ':'
			if ((file[1] == ':') && ((file[0] >= 'a' && file[0] <= 'z') || (file[0] >= 'A' && file[0] <= 'Z'))) {
				fileprefix += file.substr(0,2);
				file = file.substr(2); // cut drive letter
			}
			// Windows server pathes starting with \\ are absolute and not prefixed
#else
			// UNIX-like pathes start with / or ~ when absolute
#endif
			// split into non-prefixed absolute or relative path and name
			splitpath(file, filepath, filename);
			parent = parentfile;
			SourceFile* root = parent->Root();
			if (filepath.size() == 0) {
				// no path 1: try to find in parent directory
				if (ExistFile(parent->filepath + NORMAL_DIR_SEPARATOR + filename)) {
					filepath = parent->filepath;
				} else {
					// no path 2: try to find file in root
					if (ExistFile(root->filepath + NORMAL_DIR_SEPARATOR + filename)) {
						filepath = root->filepath;
					} else {
						// not found, will generate an error
						filepath = parent->filepath;
					}
				}
			} else {
				// path not empty: is it an absolute path?
				absolute = (filepath[0] == NORMAL_DIR_SEPARATOR) || (filepath[0] == ALTERNATE_ROOTDIR);
				// if absolute, keep it this way
				if (!absolute) {
					
					// not absolute: try relative to parent
					if (ExistFile(parent->filepath + NORMAL_DIR_SEPARATOR + filepath + NORMAL_DIR_SEPARATOR + filename)) {
						filepath = parent->filepath + NORMAL_DIR_SEPARATOR + filepath;
					} else {
						if (ExistFile(root->filepath + NORMAL_DIR_SEPARATOR + filepath + NORMAL_DIR_SEPARATOR + filename)) {
							filepath = root->filepath + NORMAL_DIR_SEPARATOR + filepath;
						} else {
							// not found, will generate an error
							filepath = parent->filepath;
						}
					}
				}
			}
		} else {
			// main file, simply cut into parts
			splitpath(file, filepath, filename);
		}
		
		return true;
	}
	
	
	//MARK: - Private Assembler functions

	/** Assemble a prepared code line. The code line must have its file and source set, and the assembler will
	 fill the rest. Notice that running conditionnal directive conditions can make the line to be unassembled
	 and ignored. In this case, the "assembled" flag is not set.
	 */
	bool Assembler::AssembleCodeLine(CodeLine& codeline, ErrorList& msg)
	{
		// cut the source line into a vector of tokens
		Parser parser(*this); // give reference of the assembbler to the parser
		parser.Init(codeline.tokens, codeline.curtoken); // give references of this codeline token array to the parser
		parser.Split(codeline.source, msg);
		
		// Handle conditionnal assembling for IF/ELSE/ENDIF directives
		ParsingMode curmode = m_modes.top();
		if (parser.Test(hasIF)) {
			if ((curmode == parsingModeSKIPTOEND) || (curmode == parsingModeSKIPTOELSE)) {
				EnterMode(parsingModeSKIPTOEND);
				return false;
			}
			// Compute the result of the IF directive
			EnterMode(parser.LastDirective(codeline, msg) ? parsingModeDOTOELSE : parsingModeSKIPTOELSE);
			// Create label if there is one
			ScanLabel(codeline, msg);
			return true;
		}
		if (parser.Test(hasELSE)) {
			switch (curmode) {
					
				case parsingModeROOT:
					return false; //TODO: error, ELSE without an IF
				case parsingModeSKIPTOEND:
					return false; // ignore
				case parsingModeDOTOEND:
					return false; //TODO: error, ELSE without an IF
					
				case parsingModeSKIPTOELSE:
					ExitMode(curmode);
					EnterMode(parsingModeDOTOEND);
					break;
					
				case parsingModeDOTOELSE:
					ExitMode(curmode);
					EnterMode(parsingModeSKIPTOEND);
					ScanLabel(codeline, msg);
					break;
			}
			return true;
		}
		else if (parser.Test(hasENDIF)) {
			if (curmode == parsingModeROOT) {
				return false;//TODO: error, ELSE without an IF
			}
			ExitMode(curmode);
			ScanLabel(codeline, msg);
			return true;
		}
		
		// IF/ELSE/ENDIF already returned, here we handle conditionnal skipping modes
		if ((curmode == parsingModeSKIPTOEND) || (curmode == parsingModeSKIPTOELSE)) {
			return false; // ignore everything in SKIP modes
		}
		
		// If we reach here, #IF/ELSE/ENDIF conditions have all been managed
		
		// Scan for a possible label
		Label* label = ScanLabel(codeline, msg);
		
		// If there is a label it can be assigned the current address. Local labels
		// can store more than one address, while global labels store only one address
		//TODO: warning if already existing global label ?
		if (label) {
			label->SetAddress(GetAddress());
		}
		
		// Handle the last directives (#INCLUDE, #INSERTHEX, #INSERTBIN) as well as '.' directives
		// Skip label if any
		for (codeline.curtoken = (label != nullptr) ? 1 : 0 ; codeline.curtoken < codeline.tokens.size() ; codeline.curtoken++) {
			ParseToken& token = codeline.tokens[codeline.curtoken];
			if (token.type == tokenTypeDIRECTIVE) {
				Directive* directive = GetDirective(token.source);
				if (!directive) {
					//TODO: msg.push_back("Unkown directive: " + token.source );
					return false;
				}
				// Let the directive do further parsing and symbols resolving
				bool result = directive->Parse(*this, parser, codeline, label, msg);
				// replace last code line label
				codeline.label = label;
				return result;
				
				// anything else can be an instruction
			} else if (token.type == tokenTypeLETTERS) {
				
				//TODO:USELESS NOW?
				// ignore if followed by a colon or a directive, next token will handle it
				if (codeline.curtoken + 1 < codeline.tokens.size()) {
					ParseToken& nexttoken = codeline.tokens[codeline.curtoken + 1];
					if (nexttoken.type == tokenTypeCOLON) continue;
					if (nexttoken.type == tokenTypeDIRECTIVE) continue;
				}
				// Should be an instruction
				Instruction* instruction = GetInstruction(token.source);
				if (instruction) {
					// resolve any symbols, and prepare the token index for assembling
					vector<int> unsolved = parser.ResolveSymbols(codeline.curtoken + 1, false);
					codeline.ResetInstruction( codeline.curtoken );
					codeline.as = this;
					// let the instruction set the assembled code in the coode line
					if (instruction->Assemble(codeline, msg)) {
					} else {
						// TODO: signal an assembling error
					}
				} else {
					// TODO: should be an instruction, probably syntax error?
				}
			}
			// assume it is something to convert, like HEXNUMBER
			else {
				parser.ResolveSymbolAt(codeline.curtoken, false);
			}
		}
		// replace last code line label
		codeline.label = label;
		return true;
	}

	/** Initializes listing file, close previous if any.
	 */
	void Assembler::PrepareListing(ErrorList& msg)
	{
		if ( ! m_outputdir.empty() && ! m_listingfilename.empty()) {
			string filename = m_outputdir + NORMAL_DIR_SEPARATOR + m_listingfilename;
			if (m_listingfile) {
				fclose(m_listingfile);
			}
			m_listingfile = fopen(filename.c_str(), "w");
			if (m_listingfile == nullptr) {
				msg.push_back({errorTypeABOUTFILE, errorWritingListing, nullptr, filename});
			}
		}
	}
	
	/** Generate a listing for an assembled code line. The listing file must have been initialized first.
	 @param codeline the assembled code line containing code to list
	 @param msg the error and warning list returned by assembler
	 */
	void Assembler::GenerateListing(CodeLine& codeline, ErrorList& msg)
	{
		if (! m_listingfile) return;
		vector<string> lines ;
		
		if (codeline.assembled) {
			lines = buildListingLines(codeline, m_status.allcodelisting);
		} else {
			vector<BYTE> emptycode;
			lines.push_back(buildOneListingLine(codeline.address, emptycode,  0, 0, nullptr, codeline.file, codeline.line, codeline.source));
		}
		
		// write to listing file and trace on standard output
		for ( auto thisline: lines) {
			fprintf(m_listingfile, "%s\n", thisline.c_str());
			fflush(m_listingfile);
			if (m_status.trace) printf("%s\n", thisline.c_str());
		}
	}
	
	/** Initializes memory listing file, close previous if any.
	 */
	void Assembler::GenerateMemoryListing(DATATYPE* memory, Section& section, ErrorList& mergingMsg)
	{
		FILE* memoryfile = nullptr;
		if ( ! m_outputdir.empty() && ! m_memoryfilename.empty()) {
			string filename = m_outputdir + NORMAL_DIR_SEPARATOR + m_memoryfilename;
			memoryfile = fopen(filename.c_str(), "w");
		}
		if (memoryfile == nullptr) {
			mergingMsg.push_back({errorTypeABOUTFILE, errorWritingListing, nullptr, m_memoryfilename});
			return;
		}
		// output listing
		// format:
		// 0080:  01 C3 54 01  C3 54 01 C3  54 01 C3 54  01 C3 54 01   ..T..T..T..T..T.
		// partial zone with unwritten bytes:
		// 3E20:  0A 00 .. ..  .. .. .. ..  .. .. .. ..  .. .. .. ..   ................
		
		// list sections
		if (m_sections.size()) {
			std::string s = "\nSections:\n";
			fprintf(memoryfile, "%s", s.c_str());
			for (auto section: m_sections) {
				s = string("\t") + section.first + ":";
				if (section.second->m_ranges.size() == 0) {
					s += "<empty>";
				} else {
					// sort ranges by starting addresses
					std::sort(section.second->m_ranges.begin(), section.second->m_ranges.end(), []( AddressRange const& a, AddressRange const& b) {
						return a.start < b.start;
					});
					for (auto range: section.second->m_ranges) {
						s += " [" + address_to_hex(range.start) + "-" + address_to_hex(range.end) + "]";
					}
				}
				fprintf(memoryfile, "%s\n", s.c_str());
			}
			fprintf(memoryfile, "%s\n", "");
		}
		
		for (auto &range: section.m_ranges) {
			
			string line = "[" + address_to_hex(range.start) + "-" + address_to_hex(range.end) + "]:";
			Section* namedsection = FindSection(range.start, range.end);
			if (namedsection) {
				line = line + namedsection->name();
			}
			fprintf(memoryfile, "%s\n", line.c_str());
			
			ADDRESSTYPE startdumpaddress = (range.start >> 4) << 4; // zero last 4 bits so we dump 16 bytes
			ADDRESSTYPE enddumpaddress = (range.end >> 4) << 4;
			for (ADDRESSTYPE dumpaddress = startdumpaddress ; dumpaddress <= enddumpaddress ; dumpaddress += 16) {
				// Address
				string line = address_to_hex(dumpaddress) + ":" + spaces(2);
				// 16-bytes hex dump
				for (ADDRESSTYPE address = dumpaddress ; address < dumpaddress + 16 ; address += 1) {
					if (address < range.start || address > range.end)
						line = line + "  " + " ";
					else
						line = line + data_to_hex(memory[address]) + " ";
					// 4 bytes separator
					if (address % 4 == 3)
						line = line + " ";
				}
				line = line + " ";
				// 16-bytes ASCII dump
				for (ADDRESSTYPE address = dumpaddress ; address < dumpaddress + 16 ; address += 1) {
					if (address < range.start || address > range.end) {
						line = line + " ";
					} else {
						char c = memory[address];
						if (c < 32 || c > 127) c = '.';
						line = line + c;
					}
				}
				// write this line
				fprintf(memoryfile, "%s\n", line.c_str());
			} // next dumpaddress
			fprintf(memoryfile, "%s\n", "");
		} // next zone
		fclose(memoryfile);
	}

	/** Generate Intel HEX output. */
	void Assembler::GenerateIntelHex(DATATYPE* memory, Section& section, ErrorList& msg)
	{
		FILE* hexfile = nullptr;
		if ( ! m_outputdir.empty() && ! m_hexfilename.empty()) {
			string filename = m_outputdir + NORMAL_DIR_SEPARATOR + m_hexfilename;
			hexfile = fopen(filename.c_str(), "w");
		}
		if (hexfile == nullptr) {
			msg.push_back({errorTypeABOUTFILE, errorWritingListing, nullptr, m_hexfilename});
			return;
		}
		// output listing
		// format:
		// :1019A0004CC101A0FF4DC101B0FF4EC10144FF4F2A
		// ':'
		// '10'   - nbbytes
		// '19A0' - address
		// '00'   - Intel record type = 0
		// '4CC101A0FF4DC101B0FF4EC10144FF4F' - up to nbbytes data
		// '2A' - control byte
		
		for (auto &range: section.m_ranges) {
			
			for (ADDRESSTYPE dumpaddress = range.start ; dumpaddress <= range.end ; dumpaddress += 16) {
				
				int nbbytes = 16;
				if (dumpaddress + 15 > range.end) {
					nbbytes = range.end - dumpaddress;
				}
				// check sections
				Section* section = FindSection(dumpaddress, dumpaddress + nbbytes - 1);
				if (!section) {
					//TODO: IMPOSSIBLE generated code with no section!
				} else if ( ! section->saved ) {
					continue;
				}
				// code comes from a section with save attribute
				string line = ":";
				line = line + data_to_hex(nbbytes) + address_to_hex(dumpaddress) + "00"; // nbbytes, address, record type
				int sum = nbbytes + (dumpaddress >> 8) + (dumpaddress & 0xFF); // bytes values after ':'
				for (ADDRESSTYPE address = dumpaddress ; address < dumpaddress + nbbytes ; address += 1) {
					line = line + data_to_hex(memory[address]);
					sum = sum + memory[address];
				}
				// compute 2's complement and keep LSB
				sum = ((~sum) + 1) & 0xFF;
				line = line + data_to_hex(sum);
				// write this line
				fprintf(hexfile, "%s\n", line.c_str());
			} // next dumpaddress
		} // next zone
		// write end record
		fprintf(hexfile, "%s\n", ":00000001FF");
		fclose(hexfile);
	}

	/** Fill a memory image and list of sections from an assembled source file. */
	void Assembler::FillFromFile(int file, DATATYPE* memory, Section& section, ErrorList& msg)
	{
		SourceFile* sourcefile = m_files.at(file);
		for (auto & codeline : sourcefile->lines) {
			
			// #include lines will recursively call this function
			if (codeline.includefile > codeline.file) {
				FillFromFile(codeline.includefile, memory, section, msg);
			} else if (codeline.code.size() > 0) {
				// copy this line code in memory image
				ADDRESSTYPE address = codeline.address;
				for (auto c: codeline.code) {
					memory[address] = c;
					section.SetAddress(address);
					address += 1;
				}
			}
			
		}
	}
	
	//MARK: - Private Sections management
	
	Section* Assembler::GetSection(std::string name)
	{
		if (m_sections.count(name) == 0) {
			Section* section = new Section;
			m_sections[name] = section;
		}
		return m_sections[name];
	}
	
	/** Tells if a section exists. */
	bool Assembler::ExistSection(std::string name)
	{
		return (m_sections.count(name) != 0);
	}
	
	//MARK: - Private Labels management

	/** Creates a label if the code line starts with a label.  A Label can be followed by ':' or by a directive. */
	Label* Assembler::ScanLabel(CodeLine& codeline, ErrorList& msg)
	{
		const int nbtokens = (const int)codeline.tokens.size();
		// empty line
		if (nbtokens == 0) {
			return nullptr;
		}
		// <letters> ?
		if (nbtokens == 1) {
			if (codeline.tokens[0].type == tokenTypeLETTERS) {
				// <directive> ? (ex: .CODE)
				if (GetDirective(codeline.tokens[0].source)) return nullptr;
				// <instruction> ? (ex: RET)
				if (GetInstruction(codeline.tokens[0].source)) return nullptr;
				// must be 	 <labelname>
				if (codeline.tokens[0].source[0] != '@' && codeline.tokens[1].source != ".EQU" && codeline.tokens[1].source != "EQU") {
					SetLastLabelName(codeline.tokens[0].source) ;
				}
				return CreateLabel(codeline.tokens[0].source, &codeline);
			}
			///TODO: return error, only one token and unknown case
			return nullptr;
		}
		// <letters> <token> ... ?
		if (codeline.tokens[0].type == tokenTypeLETTERS) {
			// <letters> <:> ... ?
			if (codeline.tokens[1].type == tokenTypeCOLON) {
				// neutralize colon for parser
				codeline.tokens[1].type = tokenTypeIGNORE;
				// store as last global label unless it is an .EQU or local label
				if (codeline.tokens[0].source[0] != '@' && ((nbtokens <= 2) || ((codeline.tokens[2].source != ".EQU") && (codeline.tokens[2].source != "EQU")))) {
					SetLastLabelName(codeline.tokens[0].source) ;
				}
				return CreateLabel(codeline.tokens[0].source, &codeline);
			}
			// <letters> <directive> ?
			if (GetDirective(codeline.tokens[1].source)) {
				// store as last global label unless it is an .EQU or local label
				if (codeline.tokens[0].source[0] != '@' && codeline.tokens[1].source != ".EQU" && codeline.tokens[1].source != "EQU") {
					SetLastLabelName(codeline.tokens[0].source) ;
				}
				return CreateLabel(codeline.tokens[0].source, &codeline);
			}
			// <letters> <instruction> ?
			if (GetInstruction(codeline.tokens[1].source)) {
				// store as last global label
				SetLastLabelName(codeline.tokens[0].source) ;
				return CreateLabel(codeline.tokens[0].source, &codeline);
			}
		}
		return nullptr;
	}

	/** Create a label at current address. If the label name starts with a '@', a local label is created for current file and is prefixed with the last
	 global label name, else the label is global. */
	Label* Assembler::CreateLabel(std::string name, CodeLine* codeline)
	{
		if (name.empty()) return nullptr;
		Label* label = GetLabel(name);
		if (label == nullptr) {
			label = new Label();
			if (!label) throw OutOfMemoryException();
			if (name[0] == '@') {
				string fullname = GetLastLabelName() + name;
				m_files[m_status.curfile]->labels[fullname] = label;
				label->multiple = true;
			} else {
				labels[name] = label;
				label->multiple = false;
			}
		}
		label->codeline = codeline;
		return label;
	}

	/** Sets the last global label name. */
	void Assembler::SetLastLabelName(std::string name)
	{
		m_status.lastlabel = name;
	}
	
	/** Returns the last global label name. */
	std::string Assembler::GetLastLabelName(void)
	{
		return m_status.lastlabel;
	}
	
	//MARK: - Private Conditionnal Parsing modes
	
	/** Tells if current mode is root (no conditionnal running) */
	bool Assembler::isRoot()
	{
		return m_modes.isRoot();
	}
	
	/** Tells mode stack level. */
	size_t Assembler::modeLevel()
	{
		return m_modes.size();
	}
	
	/** Tells current imbrication level. */
	size_t Assembler::currentLevel()
	{
		return m_curlevel;
	}
	
	/** Tells the current parsing mode. */
	ParsingMode Assembler::currentMode()
	{
		return m_modes.top();
	}
	
	/** Increment the imbrication level. */
	void Assembler::IncLevel()
	{
		m_curlevel += 1;
	}
	
	/** Decrement the imbrication level. */
	void Assembler::DecLevel()
	{
		m_curlevel -= 1;
	}
	
	/** Enter a new conditional mode. */
	void Assembler::EnterMode(ParsingMode p)
	{
		m_modes.push(p);
		m_curlevel += 1;
	}
	
	/** Closes one conditionnal mode. The expected mode must be given. */
	bool Assembler::ExitMode(ParsingMode p)
	{
		if (p == m_modes.top()) {
			m_modes.pop();
			m_curlevel -= 1;
			return true;
		}
		return false;
	}
	
	//MARK: - Private ASM, HEX and binary files including
	
	/** Assembles a source file as a main file.
	 
	 The first pass is for labels generation:
	 - doesn't store the assembled codelines
	 - doesn't generate listing
	 - follows directives like .ORG or #DEFINE or #INCLUDE
	 - updates current address so labels can be created correctly
	 
	 The second pass is for code generation.
	 
	 A source file name is split into three parts:
	 - a prefix, which may include UNC "\\?\" and/or "<unit>:" on Windows, can be empty for relative pathes
	 - an absolute path starting with a / or \, can be empty for relative pathes
	 - a filename.ext part, cannot be empty
	 
	 Absolute pathes for included files are prepended with their parent file path.
	 
	 @param file the file path to the source to assemble, can be relative to parent file path if included
	 @param msg the stack of error and warnings returned by the assembler
	 
	 @return true if assembly was correctly done
	 
	 @throw MUZ::NoFileExceptionif file not found
	 */
	bool Assembler::AssembleMainFilePassOne(string file, ErrorList& msg)
	{
		// basic security
		if (file.size() < 2) return false;

		// clear sections
		for (auto & section: m_sections) {
			delete section.second;
		}
		m_status.cursection = nullptr;
		
		// Prepare the path and name for file
		SourceFile* sourcefile = new SourceFile;
		if (sourcefile == nullptr) throw MUZ::OutOfMemoryException();
		if (!sourcefile->Set(file, nullptr)) return false;
		
		// try to open the source file
		file = sourcefile->fileprefix + sourcefile->filepath + NORMAL_DIR_SEPARATOR + sourcefile->filename;
		FILE* f = fopen(file.c_str(), "r");
		if (!f) {
			msg.push_back({errorTypeFATAL, errorOpeningSource, nullptr, file});
			throw MUZ::NoFileException();
		}
		
		// Store this file definition
		size_t filenum = m_files.size();
		m_files.push_back(sourcefile);
		m_status.curfile = (int)filenum;
		
		// now explore the file line by line
		BYTE* buffer = nullptr;
		int linesize = 0;
		long offset = ftell(f);
		Label* lastLabel = nullptr;
		while (fgetline(&buffer, &linesize, f)) {
			
			// debug
			if (m_status.trace) printf("%04X: [%4d] %s\n", GetAddress(),(int)sourcefile->lines.size()  + 1, buffer);
			
			// prepare the codeline to assemble
			CodeLine cl;
			cl.address = GetAddress();
			cl.section = GetSection();
			cl.assembled = false;
			cl.file = (int)filenum;
			cl.offset = offset;
			cl.line = (int)sourcefile->lines.size()  + 1;
			cl.size = linesize;
			cl.source = string((char*)buffer);
			cl.label = lastLabel;	// send previous label so a possible .EQU directive will change its value
			// Assemble this line, will include another file if #INCLUDE is met
			cl.assembled = AssembleCodeLine(cl, msg);
			if (cl.assembled) {
				cl.address = GetAddress();// useless?
				cl.section = GetSection();
				lastLabel = cl.label;
			}
			// Store assembly result
			sourcefile->lines.push_back(cl);
			// update current address and file position
			offset = ftell(f);
			AdvanceAddress(cl.code.size());
		}
		
		// close main source and release IO buffer
		fclose(f);
		free(buffer);

		return true;
	}

	bool Assembler::AssembleMainFilePassTwo(string file, ErrorList& msg)
	{
		// initialize listing
		PrepareListing(msg);
		
		// reset sections
		for (auto & section: m_sections) {
			section.second->Reset();
		}
		m_status.cursection = nullptr;

		
		// Execute pass 2
		CodeLine codeline;
		codeline.includefile = 0;
		codeline.file = 0;
		AssembleIncludedFilePassTwo(file, codeline, msg);
		
		// Close the listing file if finished main source
		if (m_listingfile) {
			
			// list sections
			if (m_sections.size()) {
				std::string s = "\nSections:\n------------------------------------------------------------------------------------\n";
				fprintf(m_listingfile, "%s", s.c_str());
				if (m_status.trace) printf("%s", s.c_str());
				for (auto section: m_sections) {
					s = string("\t") + section.first + ":";
					if (section.second->m_ranges.size() == 0) {
						s += "<empty>";
					} else {
						// sort ranges by starting addresses
						std::sort(section.second->m_ranges.begin(), section.second->m_ranges.end(), []( AddressRange const& a, AddressRange const& b) {
							return a.start < b.start;
						});
						for (auto range: section.second->m_ranges) {
							s += " [" + address_to_hex(range.start) + "-" + address_to_hex(range.end) + "]";
						}
					}
					fprintf(m_listingfile, "%s\n", s.c_str());
				}
			}
			
			// list DEFINEs
			if (m_defsymbols.size()) {
				std::string s = "\nDefines:\n------------------------------------------------------------------------------------\n";
				fprintf(m_listingfile, "%s", s.c_str());
				if (m_status.trace) printf("%s", s.c_str());
				std::map<string,DefSymbol*> sortedSymbols;
				for (auto symbol: m_defsymbols) {
					sortedSymbols[symbol.first] = symbol.second;
				}
				for (auto defsymbol: sortedSymbols) {
					string sleft;
					sleft = defsymbol.first.substr(0,29);
					sleft += spaces(30 - (int)sleft.length());
					sleft += " :" + defsymbol.second->value;
					s = string("\t") + sleft + "\n";
					fprintf(m_listingfile, "%s", s.c_str());
					if (m_status.trace) printf("%s", s.c_str());
				}
			}
			
			// build sorted map of equate and labels
			std::map<string,Label*> sortedEquates;		// map, ordered by name
			std::map<string,ADDRESSTYPE> nameSortedLabels;	//map ordered by name
			std::map<ADDRESSTYPE, string> addressSortedLabels;	// map ordered by address
			for (auto label: labels) {
				if ( ! label.second->equate) {
					nameSortedLabels[label.first] = label.second->addresses[0];
					addressSortedLabels[label.second->addresses[0]] = label.first;
				} else {
					sortedEquates[label.first] = label.second;
				}
			}
			
			// list equates
			if (sortedEquates.size()) {
				std::string s = "\nEquates:\n------------------------------------------------------------------------------------\n";
				fprintf(m_listingfile, "%s", s.c_str());
				if (m_status.trace) printf("%s", s.c_str());
				for (auto equate: sortedEquates) {
					string sleft;
					sleft = equate.first.substr(0,29);
					sleft += spaces(30 - (int)sleft.length());
					sleft += " :" + address_to_hex(equate.second->addresses[0]);
					s = string("\t") + sleft + "\n";
					fprintf(m_listingfile, "%s", s.c_str());
					if (m_status.trace) printf("%s", s.c_str());
				}
			}
			
			// list global labels sorted by value then name
			if (nameSortedLabels.size() || addressSortedLabels.size()) {
				std::string s = "\nGlobal labels:\n--- By Name -------------------------------|---By Address --------------------------\n";
				fprintf(m_listingfile, "%s", s.c_str());
				if (m_status.trace) printf("%s", s.c_str());
				bool nameFinished = false, addressFinished = false;
				auto iterName = nameSortedLabels.begin();
				auto iterAddress = addressSortedLabels.begin();
				if (iterName == nameSortedLabels.end()) nameFinished = true;
				if (iterAddress == addressSortedLabels.end()) addressFinished = true;
				do {
					string sleft, sright;
					if (nameFinished) {
						sleft = spaces(30) + "  " + spaces(4);
					} else {
						sleft = iterName->first.substr(0,29);
						sleft += spaces(30 - (int)sleft.length());
						sleft += " :" + address_to_hex(iterName->second);
						iterName++;
						if (iterName == nameSortedLabels.end()) nameFinished = true;
					}
					if (!addressFinished) {
						sright = address_to_hex(iterAddress->first) + ": ";
						sright += iterAddress->second.substr(0,30);
						iterAddress++;
						if (iterAddress == addressSortedLabels.end()) addressFinished = true;
					}
					s = string("\t") + sleft + spaces(3) + "|" + spaces(3) + sright + "\n";
					fprintf(m_listingfile, "%s", s.c_str());
					if (m_status.trace) printf("%s", s.c_str());
					
				} while (!nameFinished || !addressFinished);
			}
			fclose(m_listingfile);
			m_listingfile = nullptr;
		}
		return true;
	}
	
	bool Assembler::AssembleIncludedFilePassOne(string file, CodeLine& codeline, ErrorList& msg)
	{
		// basic security
		if (file.size() < 2) return false;
		if (codeline.file >= (int)m_files.size()) return false;
		
		// Prepare the path and name for file
		SourceFile* sourcefile = new SourceFile;
		if (sourcefile == nullptr) throw MUZ::OutOfMemoryException();
		if (!sourcefile->Set(file, (codeline.file >= 0) ? m_files[codeline.file] : nullptr)) return false;
		
		// try to open the source file
		file = sourcefile->fileprefix + sourcefile->filepath + NORMAL_DIR_SEPARATOR + sourcefile->filename;
		FILE* f = fopen(file.c_str(), "r");
		if (!f) {
			msg.push_back({errorTypeFATAL, errorOpeningSource, &codeline, file});
			throw MUZ::NoFileException();
		}
		
		// Store this file definition
		size_t filenum = m_files.size();
		m_files.push_back(sourcefile);
		m_status.curfile = (int)filenum;
		
		sourcefile->parentfile = codeline.file;
		sourcefile->parentline = codeline.line;
		sourcefile->included = (sourcefile->parentfile >= 0);
		
		// now explore the file line by line
		BYTE* buffer = nullptr;
		int linesize = 0;
		long offset = ftell(f);
		Label* lastLabel = nullptr;
		while (fgetline(&buffer, &linesize, f)) {
			
			// debug
			if (m_status.trace) printf("%04X: [%4d] %s\n", GetAddress(),(int)sourcefile->lines.size()  + 1, buffer);
			
			// prepare the codeline to assemble
			CodeLine cl;
			cl.address = GetAddress();
			cl.section = GetSection();
			cl.assembled = false;
			cl.file = (int)filenum;
			cl.offset = offset;
			cl.line = (int)sourcefile->lines.size()  + 1;
			cl.size = linesize;
			cl.source = string((char*)buffer);
			cl.label = lastLabel;	// send previous label so a possible .EQU directive will change its value
			// Assemble this line, will include another file if #INCLUDE is met
			cl.assembled = AssembleCodeLine(cl, msg);
			if (cl.assembled) {
				cl.address = GetAddress();// useless?
				cl.section = GetSection();
				lastLabel = cl.label;
			}
			// Store assembly result
			sourcefile->lines.push_back(cl);
			// update current address and file position
			offset = ftell(f);
			AdvanceAddress(cl.code.size());
		}
		
		// close main source and release IO buffer
		fclose(f);
		free(buffer);
		return true;
	}
	
	bool Assembler::AssembleIncludedFilePassTwo(string file, CodeLine& codeline, ErrorList& msg)
	{
		// basic security
		if (file.size() < 2) return false;
		if (codeline.file >= (int)m_files.size()) return false;
		
		// For included files, put the #include in listing here before the included source is listed
		if (codeline.file != codeline.includefile) GenerateListing(codeline, msg);

		SourceFile* sourcefile = m_files.at(codeline.includefile);
		m_status.curfile = codeline.includefile;
	
		// print the full file path in listing
		if (m_listingfile) {
			fprintf(m_listingfile, "\n                    %s\n\n", file.c_str()); // 20 spaces before file path
		}
		
		// now reassemble line by line
		for (CodeLine& cl : sourcefile->lines) {
			
			cl.code.clear();
			cl.assembled = AssembleCodeLine(cl, msg);
			if (cl.assembled) {
				cl.address = GetAddress();
				cl.section = GetSection();
			}
			// Add the assembled line to listing, or display parent filename if it was an #INCLUDE directive
			if (cl.tokens.size() > 0) {
				ParseToken& token = cl.tokens.at(0);
				if (cl.assembled && token.isIncludingDirective()) {
					// list current parent file to show that include if finished
					fprintf(m_listingfile, "\n                    %s\n", file.c_str()); // 20 spaces before file path
				} else {
					// not assembled, or not an include directive: show normal listing
					GenerateListing( cl, msg);
				}
			} else {
				// no token: shoud be empty line, list it
				GenerateListing( cl, msg);
			}
			AdvanceAddress(cl.code.size()) ;
		}
		return true;
	}
	
	/** Assembled an HEX included file.
	 @param file the file path for the HEX file to include
	 @param msg the list of message and warnings returned by the assembler
	 @return true if the file was included
	 */
	bool Assembler::AssembleHexFile(std::string file, CodeLine& codeline, ErrorList& msg)
	{
		// basic security
		if (file.size() < 2) return false;
		if (codeline.file >= (int)m_files.size()) return false;
		
		// first pass?
		if (m_status.firstpass) {
			
			// Prepare the path and name for file
			SourceFile* sourcefile = new SourceFile;
			if (sourcefile == nullptr) throw MUZ::OutOfMemoryException();
			if (!sourcefile->Set(file, codeline.file >= 0 ? m_files[codeline.file] : nullptr)) return false;
			
			// try to open the source file
			file = sourcefile->fileprefix + sourcefile->filepath + NORMAL_DIR_SEPARATOR + sourcefile->filename;
			FILE* f = fopen(file.c_str(), "r");
			if (!f) {
				msg.push_back({errorTypeFATAL, errorOpeningSource, &codeline, file});
				throw MUZ::NoFileException();
			}
			
			// Store this file definition
			size_t filenum = m_files.size();
			m_files.push_back(sourcefile);
			m_status.curfile = (int)filenum;
			sourcefile->parentfile = codeline.file;
			sourcefile->parentline = codeline.line;
			sourcefile->included = (sourcefile->parentfile >= 0);
			
			// now explore the file line by line
			BYTE* buffer = nullptr;
			int linesize = 0;
			long offset = ftell(f);
			Label* lastLabel = nullptr;
			BYTE* binbuffer = (BYTE*)calloc(1024,1);// should be suffficient, lines can only store 256 bytes definitions
			while (fgetline(&buffer, &linesize, f)) {
				
				// debug
				if (m_status.trace) printf("%04X: [%4d] %s\n", GetAddress(),(int)sourcefile->lines.size()  + 1, buffer);
				// translate the hex line into .DB source line
				int nbbytes = hexNbBytes(buffer);
				if (nbbytes) {
					hexStore(buffer, binbuffer);
					string source="\t.DB ";
					int b = 0;
					for ( ; b < nbbytes ; b++) {
						source += "$" + data_to_hex(binbuffer[b]) + ",";
					}

					// prepare the codeline to assemble
					CodeLine cl;
					cl.address = GetAddress();
					cl.section = GetSection();
					cl.assembled = false;
					cl.file = (int)filenum;
					cl.offset = offset;
					cl.line = (int)sourcefile->lines.size()  + 1;
					cl.size = linesize;
					cl.source = source;
					cl.label = lastLabel;	// send previous label so a possible .EQU directive will change its value
					// Assemble this line, will include another file if #INCLUDE is met
					cl.assembled = AssembleCodeLine(cl, msg);
					if (cl.assembled) {
						cl.address = GetAddress();// useless?
						cl.section = GetSection();
						lastLabel = cl.label;
					}
					// Store assembly result
					sourcefile->lines.push_back(cl);
					// update current address and file position
					offset = ftell(f);
					AdvanceAddress(cl.code.size());
				}
			}
			
			// close main source and release IO buffer
			fclose(f);
			free(buffer);
			free(binbuffer);
		} else {
			AssembleIncludedFilePassTwo(file, codeline, msg);
		}
		return true;
	}
	
	/** Assembled an BIN included file.
	 */
	bool Assembler::AssembleBinFile(std::string file, CodeLine& codeline, ErrorList& msg)
	{
		// basic security
		if (file.size() < 2) return false;
		if (codeline.file >= (int)m_files.size()) return false;
		
		// first pass?
		if (m_status.firstpass) {
			
			// Prepare the path and name for file
			SourceFile* sourcefile = new SourceFile;
			if (sourcefile == nullptr) throw MUZ::OutOfMemoryException();
			if (!sourcefile->Set(file, codeline.file >= 0 ? m_files[codeline.file] : nullptr)) return false;
			
			// try to open the source file
			file = sourcefile->fileprefix + sourcefile->filepath + NORMAL_DIR_SEPARATOR + sourcefile->filename;
			FILE* f = fopen(file.c_str(), "r");
			if (!f) {
				msg.push_back({errorTypeFATAL, errorOpeningSource, &codeline, file});
				throw MUZ::NoFileException();
			}
			
			// Store this file definition
			size_t filenum = m_files.size();
			m_files.push_back(sourcefile);
			m_status.curfile = (int)filenum;
			sourcefile->parentfile = codeline.file;
			sourcefile->parentline = codeline.line;
			sourcefile->included = (sourcefile->parentfile >= 0);
			
			// now explore the file up to 16 bytes at a time
			BYTE* binbuffer = (BYTE*)calloc(16,1);
			int nbbytes = (int)fread(binbuffer, 1, 16, f);
			while (nbbytes > 0)   {
				
				// translate the bytes into .DB source line
				string source="\t.DB ";
				int b = 0;
				for ( ; b < nbbytes ; b++) {
					source += "$" + data_to_hex(binbuffer[b]) + ",";
				}
				
				// prepare the codeline to assemble
				CodeLine cl;
				cl.address = GetAddress();
				cl.section = GetSection();
				cl.assembled = false;
				cl.file = (int)filenum;
				cl.offset = ftell(f);;
				cl.line = (int)sourcefile->lines.size()  + 1;
				cl.size = nbbytes;
				cl.source = source;
				cl.label = nullptr;
				// Assemble this line, will include another file if #INCLUDE is met
				cl.assembled = AssembleCodeLine(cl, msg);
				if (cl.assembled) {
					cl.address = GetAddress();// useless?
					cl.section = GetSection();
				}
				// Store assembly result
				sourcefile->lines.push_back(cl);
				// mark section and advance address to next position for code
				AdvanceAddress(cl.code.size());
				// advance in binary file
				nbbytes = (int)fread(binbuffer, 1, 16, f);
			}
			
			// close main source and release IO buffer
			fclose(f);
			free(binbuffer);
		} else {
			AssembleIncludedFilePassTwo(file, codeline, msg);
		}
		return true;
	}
	
	//MARK: - PUBLIC API
	
	//MARK: - Constructor and destructor
	
	Assembler::Assembler()
	{
		Reset();
		// Instructions
		m_instructions["LD"] = new InstructionLD();
		m_instructions["PUSH"] = new InstructionPUSH();
		m_instructions["POP"] = new InstructionPOP();
		m_instructions["EXX"] = new InstructionEXX();
		m_instructions["EX"] = new InstructionEX();
		m_instructions["LDI"] = new InstructionLDI();
		m_instructions["LDIR"] = new InstructionLDIR();
		m_instructions["LDD"] = new InstructionLDD();
		m_instructions["LDDR"] = new InstructionLDDR();
		m_instructions["CPI"] = new InstructionCPI();
		m_instructions["CPIR"] = new InstructionCPIR();
		m_instructions["CPD"] = new InstructionCPD();
		m_instructions["CPDR"] = new InstructionCPDR();
		m_instructions["ADD"] = new InstructionADD();
		m_instructions["ADC"] = new InstructionADC();
		m_instructions["SUB"] = new InstructionSUB();
		m_instructions["SBC"] = new InstructionSBC();
		m_instructions["AND"] = new InstructionAND();
		m_instructions["OR"] = new InstructionOR();
		m_instructions["XOR"] = new InstructionXOR();
		m_instructions["CP"] = new InstructionCP();
		m_instructions["INC"] = new InstructionINC();
		m_instructions["DEC"] = new InstructionDEC();
		m_instructions["DAA"] = new InstructionDAA();
		m_instructions["CPL"] = new InstructionCPL();
		m_instructions["NEG"] = new InstructionNEG();
		m_instructions["CCF"] = new InstructionCCF();
		m_instructions["SCF"] = new InstructionSCF();
		m_instructions["NOP"] = new InstructionNOP();
		m_instructions["HALT"] = new InstructionHALT();
		m_instructions["DI"] = new InstructionDI();
		m_instructions["EI"] = new InstructionEI();
		m_instructions["IM"] = new InstructionIM();
		m_instructions["RLCA"] = new InstructionRLCA();
		m_instructions["RLA"] = new InstructionRLA();
		m_instructions["RRCA"] = new InstructionRRCA();
		m_instructions["RRA"] = new InstructionRRA();
		m_instructions["RLC"] = new InstructionRLC();
		m_instructions["RL"] = new InstructionRL();
		m_instructions["RRC"] = new InstructionRRC();
		m_instructions["RR"] = new InstructionRR();
		m_instructions["SLA"] = new InstructionSLA();
		m_instructions["SLL"] = new InstructionSLL();//undoc
		m_instructions["SRA"] = new InstructionSRA();
		m_instructions["SRL"] = new InstructionSRL();
		m_instructions["RLD"] = new InstructionRLD();
		m_instructions["RRD"] = new InstructionRRD();
		m_instructions["BIT"] = new InstructionBIT();
		m_instructions["SET"] = new InstructionSET();
		m_instructions["RES"] = new InstructionRES();
		m_instructions["JP"] = new InstructionJP();
		m_instructions["JR"] = new InstructionJR();
		m_instructions["DJNZ"] = new InstructionDJNZ();
		m_instructions["CALL"] = new InstructionCALL();
		m_instructions["RET"] = new InstructionRET();
		m_instructions["RETI"] = new InstructionRETI();
		m_instructions["RETN"] = new InstructionRETN();
		m_instructions["RST"] = new InstructionRST();
		m_instructions["IN"] = new InstructionIN();
		m_instructions["INI"] = new InstructionINI();
		m_instructions["INIR"] = new InstructionINIR();
		m_instructions["IND"] = new InstructionIND();
		m_instructions["INDR"] = new InstructionINDR();
		m_instructions["OUT"] = new InstructionOUT();
		m_instructions["OUTI"] = new InstructionOUTI();
		m_instructions["OTIR"] = new InstructionOTIR();
		m_instructions["OUTD"] = new InstructionOUTD();
		m_instructions["OTDR"] = new InstructionOTDR();
		
		// Preprocessor directives
		m_directives["#DEFINE"] = new DirectiveDEFINE();
		m_directives["#UNDEF"] = new DirectiveUNDEFINE();
		m_directives["#IF"] = new DirectiveIF();
		m_directives["COND"] = new DirectiveIF();
		m_directives["#IFDEF"] = new DirectiveIFDEF();
		m_directives["#ELSE"] = new DirectiveELSE();
		m_directives["#IFNDEF"] = new DirectiveIFNDEF();
		m_directives["#ENDIF"] = new DirectiveENDIF();
		m_directives["#INCLUDE"] = new DirectiveINCLUDE();
		m_directives["#INSERTHEX"] = new DirectiveINSERTHEX();
		m_directives["#INSERTBIN"] = new DirectiveINSERTBIN();
		
		// Assembler directives
		m_directives[".PROC"] = new DirectivePROC();
		m_directives[".ORG"] = new DirectiveORG();
		m_directives["ORG"] = new DirectiveORG();
		m_directives[".DATA"] = new DirectiveDATA();
		m_directives[".CODE"] = new DirectiveCODE();
		m_directives[".EQU"] = new DirectiveEQU();
		m_directives["EQU"] = new DirectiveEQU();
		m_directives[".BYTE"] = new DirectiveBYTE();
		m_directives[".DB"] = new DirectiveBYTE();
		m_directives["DB"] = new DirectiveBYTE();
		m_directives["DEFB"] = new DirectiveBYTE();
		m_directives[".WORD"] = new DirectiveWORD();
		m_directives[".DW"] = new DirectiveWORD();
		m_directives["DW"] = new DirectiveWORD();
		m_directives["DEFW"] = new DirectiveWORD();
		m_directives[".DS"] = new DirectiveSPACE();
		m_directives["DS"] = new DirectiveSPACE();
		m_directives["DEFS"] = new DirectiveSPACE();
	}
	
	Assembler::~Assembler()
	{
		for (auto &i : m_instructions) {
			delete i.second;
		}
		for (auto &d : m_directives) {
			delete d.second;
		}
		for (auto &d : m_defsymbols) {
			delete d.second;
		}
		for (auto &d : labels) {
			delete d.second;
		}
		for (auto &f : m_files) {
			delete f;
		}
		for (auto &s : m_sections) {
			delete s.second;
		}
	}
	
	//MARK: - Initializer and setting output files
	
	/** Resets the assembler. */
	void Assembler::Reset() {
		m_defsymbols.clear();
		labels.clear();
		m_files.clear();
		for (auto &section : m_sections) delete section.second;
		m_sections.clear();
		m_status.cursection = nullptr;
		m_modes = ParsingModeStack();// resets
	}
	
	/** Enable/Disable all bytes listing or 2-lines listing. */
	void Assembler::EnableFullListing(bool yes)
	{
		m_status.allcodelisting = yes;
	}
	
	/** Enable/Disable trace on standard output. */
	void Assembler::EnableTrace(bool yes)
	{
		m_status.trace = yes;
	}

	/** Sets the directory where MUZ places the output files and listings. */
	void Assembler::SetOutputDirectory(std::string directory)
	{
		m_outputdir = directory;
	}
	
	/** Sets the listing filename. */
	void Assembler::SetListingFilename(std::string filename)
	{
		m_listingfilename = filename;
	}
	
	/** Sets the binary filename. */
	void Assembler::SetBinaryFilename(std::string filename)
	{
		m_binfilename = filename;
	}
	
	/** Sets the IntelHex filename. */
	void Assembler::SetIntelHexFilename(std::string filename)
	{
		m_hexfilename = filename;
	}
	
	/** Sets the Memory listing filename. */
	void Assembler::SetMemoryFilename(std::string filename)
	{
		m_memoryfilename = filename;
	}
	
	/** Sets the Symbols filename. */
	void Assembler::SetSymbolsFilename(std::string filename)
	{
		m_symbolsfilename = filename;
	}

	//MARK: - Sections and current address management
	
	/** Sets current section to code section.*/
	void Assembler::SetCodeSection(std::string name)
	{
		std::string fullname = "CODE" + (name.empty() ? "" : "@"+name);
		bool justcreated = !ExistSection(fullname);
		m_status.cursection = GetSection(fullname);
		if  (justcreated) {
			m_status.cursection->saved = true;
			m_status.cursection->SetName(fullname);
		}
	}
	
	/** Sets current section to data section. The attributes (save, name) are only set if the section is set for the first time. */
	void Assembler::SetDataSection(std::string name, bool save)
	{
		std::string fullname = "DATA" + (name.empty() ? "" : "@"+name);
		bool justcreated = !ExistSection(fullname);
		m_status.cursection = GetSection(fullname);
		if  (justcreated) {
			m_status.cursection->saved = save;
			m_status.cursection->SetName(fullname);
		}
	}
	
	/** Sets the current assembling address. */
	void Assembler::SetAddress(ADDRESSTYPE address)
	{
		if (m_status.cursection == nullptr) SetCodeSection();
		m_status.cursection->SetAddress( address );
	}
	
	/** Advance the current assembling address. Marks the section for addresses from current to just before the new position. */
	void Assembler::AdvanceAddress( ADDRESSTYPE advance )
	{
		if (advance) {
			// touch current address to just before current address
			ADDRESSTYPE start = GetAddress();
			for (int i = 0 ; i < advance ; i++)
				 SetAddress(start + i);
			// set new current address
			m_status.cursection->m_curaddress = start + advance;
		}
	}
	
	/** Returns the current address in current section. */
	ADDRESSTYPE Assembler::GetAddress()
	{
		if (m_status.cursection == nullptr) SetCodeSection();
		return m_status.cursection->curaddress();
	}
	
	/** Returns the current section. */
	Section* Assembler::GetSection()
	{
		return m_status.cursection;
	}
	
	/** Finds a section from an address range. */
	Section* Assembler::FindSection(ADDRESSTYPE s, ADDRESSTYPE e)
	{
		for (auto& section: m_sections) {
			int range = section.second->FindRange(s, e);
			if (range >= 0)
				return section.second;
		}
		return nullptr;
	}

	//MARK: - Pass 1 / 2 setting
	
	/** Set first or second pass. */
	void Assembler::SetFirstPass(bool yes)
	{
		m_status.firstpass = yes;
	}
	/** Check if running first pass. */
	bool Assembler::IsFirstPass()
	{
		return m_status.firstpass;
	}
	
	//MARK: - Assembling an individual code line or a main file

	/** Assemble a single line into a codeline,
	 */
	CodeLine Assembler::AssembleLine(std::string sourceline, ErrorList& msg)
	{
		//TODO: get the source line from its file
		CodeLine codeline;
		codeline.address = 0;
		codeline.assembled = false;
		codeline.file = -1;
		codeline.offset = 0;
		codeline.size = 0;
		codeline.source = sourceline;
		codeline.assembled = AssembleCodeLine(codeline, msg);
		return codeline;
	}

	/** Assembles a main file.
	 
	 @param file the file path to the source to assemble, can be relative to parent file path if included
	 @param msg the stack of error and warnings returned by the assembler
	 
	 @return true if assembly was correctly done
	 */
	bool Assembler::AssembleFile(string file, ErrorList& msg)
	{
		SetFirstPass(true);
		CodeLine codeline;
		if (m_status.trace)	printf("Pass 1: %s\n", file.c_str());
		if (AssembleMainFilePassOne(file, msg)) {
			SetFirstPass(false);
			if (m_status.trace) printf("Pass 2: %s\n", file.c_str());
			if (AssembleMainFilePassTwo(file, msg)) {
				
				// All ok, now output memory listing

				// first build memory image and list of written zones
				DATATYPE* memory = (DATATYPE*)calloc(ADDRESSMASK+1, 1);
				Section section;
				ErrorList mergingMsg;
				FillFromFile(0, memory, section, mergingMsg); // this handles recursive calls for included files
				// sort ranges by starting addresses
				std::sort(section.m_ranges.begin(), section.m_ranges.end(), []( AddressRange const& a, AddressRange const& b) {
					return a.start < b.start;
				});
				
				// dump this memory and zones in listing
				GenerateMemoryListing(memory, section, mergingMsg);
				
				// and Intel Hex format
				GenerateIntelHex(memory, section, mergingMsg);
			}
		}
		return false;
	}

	//MARK: - Interface to instructions, labels, directives, symbols
	
	/** Try to find a directive in the # and . directives array. */
	Directive* Assembler::GetDirective(std::string name)
	{
		name = std::to_upper(name);
		if (m_directives.count(name))
			return m_directives[name];
		return nullptr;
	}
	
	/** Try to find a n instruction in the instruction set. */
	Instruction* Assembler::GetInstruction(std::string name)
	{
		name = std::to_upper(name);
		if (m_instructions.count(name))
			return m_instructions[name];
		return nullptr;
	}
	
	/** Try to find a named label in local or global labels. */
	Label* Assembler::GetLabel(std::string name)
	{
		if (name.empty()) return nullptr;
		if (name[0] == '@') {
			string fullname = GetLastLabelName() + name;
			if (m_files[m_status.curfile]->labels.count(fullname)) {
				return m_files[m_status.curfile]->labels[fullname];
			}
		} else {
			if (labels.count(name)) {
				return labels[name];
			}
		}
		return nullptr;
	}
	
	/** Create a #DEFINE symbol, optionaly with a given string value.
	 	Sending an empty value will define the symbol as a virtually true value in a DefSymbol,
	 	while non empty values are stored as strings in a DefSymbol.
	 */
	DefSymbol* Assembler::CreateDefSymbol(std::string name, std::string value)
	{
		// Check if the name exists
		DefSymbol* defsymbol = nullptr;
		if (m_defsymbols.count(name)) {
			defsymbol = m_defsymbols[name];
		}
		// check if the value is empty and if so, define the symbol as an empty but true defsymbol
		if (!defsymbol) defsymbol = new DefSymbol();
		if (!defsymbol) throw OutOfMemoryException();
		if (value.empty()) {
			defsymbol->value.clear();
			defsymbol->singledefine = true;
		} else {
			defsymbol->value = value;
			defsymbol->singledefine = false;
		}
		// do the assignment
		m_defsymbols[name] = defsymbol;
		return defsymbol;
	}
	
	/** Delete a #DEFINE symbol. */
	bool Assembler::DeleteDefSymbol(std::string name)
	{
		if (m_defsymbols.count(name)) {
			m_defsymbols.erase(name);
			return true;
		}
		return false;
	}
	
	/** Check if a symbol is #DEFINEd.*/
	bool Assembler::ExistSymbol(std::string name)
	{
		if (m_defsymbols.count(name))
			return true;
		return false;
	}
	
	/** Try to replace a symbol from the #DEFINE table.
	 Symbols associated to nothing are considered as a boolean true, while the other symbols are replaced by their string value.
	 @param token [IN/OUT] the token to check, if its value exists as a symbol it will be replaced by the symbol value if it has a value or by a boolean set to true if the symbol is empty
	 @return true if the symbol has been found and the token value replaced, false is not
	 */
	bool Assembler::ReplaceDefSymbol(ParseToken& token)
	{
		if (m_defsymbols.count(token.source)) {
			auto defsymbol = m_defsymbols[token.source];
			if (defsymbol) {
				if (defsymbol->singledefine) {
					token.source = "t";
					token.type = tokenTypeBOOL;
				} else {
					token.source = defsymbol->value;
					token.type = tokenTypeSTRING;
				}
			}
			return true;
		}
		return false; // not found
	}
	

	/** Try to replace a symbol from the Label table. Returns closest address if this is a local label with more than one address. Returns false if the label
	 doesn't exist or has no address yet. */
	bool Assembler::ReplaceLabel(std::string& source)
	{
		// local label?
		if (source.empty()) return false;
		Label* label = GetLabel(source);
		if (label && !label->empty()) {
			source = std::to_string(label->AddressFrom(GetAddress())); // std::to_string() extension at the top of this file
			return true;
		}
		return false;
	}
	


} // namespace MUZ
