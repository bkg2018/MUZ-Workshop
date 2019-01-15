//
//  MUZ-Assembler.h
//  MUZ-Workshop
//
//  Created by Francis Pierot on 01/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Assembler_h
#define Assembler_h

#include <stdio.h>
#include <vector>
#include <unordered_map>

#include "Types.h"
#include "ParsingMode.h"
#include "Errors.h"
#include "Label.h"
#include "DefSymbol.h"
#include "Directive.h"
#include "Exceptions.h"
#include "ExpVector.h"
#include "CodeLine.h"

namespace MUZ {
	
	class Assembler
	{
		//MARK: - Private management structures
		/** Definition for one source file. */
		struct SourceFile
		{
			bool	included = false; 		// true for #INCLUDEd files
			int		parentfile = -1;		// if included, parent file reference
			int		parentline = -1;		// if included, line number in parent file
			SourceFile* parent = nullptr;	// if included, parent source file
			std::string fileprefix;			// Windows specific prefixes
			std::string filepath;			// path part of the file
			std::string filename;			// filename part of the file
			std::vector<CodeLine> lines;	// parsed/assembled content, matches the source file lines
			LabelMap labels;				// local labels
			
			/** Gets the root parent of this SourceFile. */
			SourceFile* Root();
			
			/** Sets this SourceFile from the given filename and parent. Set parent to NULL to set a main source file. */
			bool Set(std::string file, SourceFile* parent);
		};
		
		// The #INCLUDE directive uses Assembler::AssembleFile private function
		friend class DirectiveINCLUDE;
		// The #INSERTHEX directive uses Assembler::AssembleHexFile private function
		friend class DirectiveINSERTHEX;
		// The #INSERTBIN directive uses Assembler::AssembleBinFile private function
		friend class DirectiveINSERTBIN;

		// Map tables for the instruction set and directives
		InstructionsMap				m_instructions;
		DirectivesMap				m_directives;
		
		// Tables for the assembly
		DefSymbolsMap				m_defsymbols;
		LabelMap					labels;
		std::vector<SourceFile*>	m_files;
		std::unordered_map<std::string, Section*> m_sections;
		
		// Stack for imbricated conditionnal modes
		ParsingModeStack			m_modes;	// stack of all imbricated levels, root being the first and never popped
		size_t						m_curlevel = 0; // should be 1 when at root
		
		/** Current assembly status singleton. */
		struct AssemblyStatus {
			Section*	cursection = nullptr;	// current code or data section
			bool		firstpass  = true;		// true for first ass, false for second
			int			curfile		= 0;		// current file number in m_files, used for local labels
			std::string	lastlabel;				// last global label, used as a prefix for local labels starting with '@'
			bool		trace		= false;	// true to activate debug trace on standard output
			bool 		allcodelisting=false;	// true to list all bytes of a line, false to limit to 2 lines of listing
		} m_status;
		
		//MARK: - Private Output directory and file names
		/// root output directory for all files
		std::string					m_outputdir;
		/// file name for the binary output
		std::string					m_binfilename;
		/// file name for the Intel HEX output
		std::string 				m_hexfilename;
		/// file name for the listing output
		std::string					m_listingfilename;
		/// file name for the memory output
		std::string					m_memoryfilename;
		/// file name for the symbols output
		std::string					m_symbolsfilename;
		/// file name for errors/warnings log
		std::string					m_logfilename;
		/// current opened listing file
		FILE*						m_listingfile = nullptr;

		//MARK: - Private Assembler functions
		/** Assembles a prepared code line. */
		bool AssembleCodeLine(CodeLine& codeline, ErrorList& msg);
		/** Initializes listing file, closes previous if any. */
		void PrepareListing(CodeLine& codeline, ErrorList& msg);
		/** Generates a listing line for an assembled codeline. */
		void GenerateListing(CodeLine& codeline, ErrorList& msg);
		/** Initializes memory listing file, close previous if any. */
		void GenerateMemoryListing(DATATYPE* memory, Section& section, ErrorList& msg);
		/** Generate Intel HEX output. */
		void GenerateIntelHex(DATATYPE* memory, Section& section, ErrorList& msg);
		/** Fill a memory image and list of sections from an assembled source file. */
		void FillFromFile(int file, DATATYPE* memory, Section& section, ErrorList& msg);
		/** Generate warning/error file. */
		void GenerateLog(ErrorList& msg);
		
		//MARK: - Private Sections management
		/** Gets or create a named section. */
		Section* GetSection(std::string name);
		/** Tells if a section exists. */
		bool ExistSection(std::string name);
		
		//MARK: - Private Labels management
		/** Scans a code line for a global or local label, creating it if needed. */
		Label* ScanLabel(CodeLine& codeline, ErrorList& msg);
		/** Creates a label at current address. */
		Label* CreateLabel(std::string name, CodeLine& codeline, ErrorList& msg);
		/** Sets the last global label name. */
		void SetLastLabelName(std::string name);
		/** Returns the last global label name. */
		std::string GetLastLabelName(void);
		
		//MARK: - Private Conditionnal Parsing modes
		/** Tells if current mode is root (no conditionnal running) */
		bool isRoot() ;
		/** Tells mode stack level. */
		size_t modeLevel();
		/** Tells current imbrication level. */
		size_t currentLevel();
		/** Tells the current parsing mode. */
		ParsingMode currentMode();
		/** Increment the imbrication level. */
		void IncLevel();
		/** Decrement the imbrication level. */
		void DecLevel();
		/** Enter a new conditional mode. */
		void EnterMode(ParsingMode p);
		/** Closes one conditionnal mode. The expected mode must be given. */
		bool ExitMode(ParsingMode p);
	
		//MARK: - Private ASM, HEX and binary files including
		/** Assembles a main source file. */
		bool AssembleMainFilePassOne(std::string file, ErrorList& msg);
		bool AssembleMainFilePassTwo(std::string file, ErrorList& msg);
		/** Assembles an included source file. */
		bool AssembleIncludedFilePassOne(std::string file, CodeLine& codeline, ErrorList& msg);
		bool AssembleIncludedFilePassTwo(std::string file, CodeLine& codeline, ErrorList& msg);
		/** Assembles an HEX included file. */
		bool AssembleHexFile(std::string file, CodeLine& codeline, ErrorList& msg);
		/** Assembles a binary included file. */
		bool AssembleBinFile(std::string file, CodeLine& codeline, ErrorList& msg);

	public:
		//MARK: - PUBLIC API
		
		//MARK: - Constructor and destructor
		/** Initialize directives and instructions. */
		Assembler();
		/** Clean up memory. */
		virtual ~Assembler();
		
		//MARK: - Initializer and setting output files
		/** Reset the assembly. */
		void Reset();
		/** Enable/Disable all bytes listing or 2-lines listing. */
		void EnableFullListing(bool yes);
		/** Enable/Disable trace on standard output. */
		void EnableTrace(bool yes);
		/** Sets the directory where MUZ places the output files and listings. */
		void SetOutputDirectory(std::string directory);
		/** Sets the listing filename. */
		void SetListingFilename(std::string filename);
		/** Sets the binary filename. */
		void SetBinaryFilename(std::string filename);
		/** Sets the IntelHex filename. */
		void SetIntelHexFilename(std::string filename);
		/** Sets the Memory listing filename. */
		void SetMemoryFilename(std::string filename);
		/** Sets the Symbols filename. */
		void SetSymbolsFilename(std::string filename);
		/** Sets the erros/warnings log filename. */
		void SetLogFilename(std::string filename);
		
		//MARK: - Sections and current address management
		/** sets current section to code .*/
		void SetCodeSection(std::string name = "");
		/** sets current section to data .*/
		void SetDataSection(std::string name = "", bool save = false);
		/** Sets the current assembling address. */
		void SetAddress(ADDRESSTYPE address);
		/** Advances the current assembling address. */
		void AdvanceAddress( ADDRESSTYPE advance );
		/** Returns the current address. */
		ADDRESSTYPE GetAddress();
		/** Returns the current section. */
		Section* GetSection();
		/** Finds a section from an address range. */
		Section* FindSection(ADDRESSTYPE a, ADDRESSTYPE e);

		
		//MARK: - Pass 1 / 2 setting
		/** Sets first or second pass. */
		void SetFirstPass(bool yes);
		/** Checks if running first pass. */
		bool IsFirstPass();

		//MARK: - Assembling an individual code line or a main file
		/** Assemble a single line and return a codeline */
		CodeLine AssembleLine(std::string sourceline, ErrorList& msg);
		/** Assembles a main source file. */
		bool AssembleFile(std::string file, ErrorList& msg);
		/** Get the name of a file from its index. */
		std::string GetFileName(int index);

		//MARK: - Interface to instructions, labels, directives, symbols
		
		/** Sets the instruction and operand set. */
		void SetInstructions(std::string name);
		
		/** Tries to find a directive by name. Must include the '#' or '.' prefix. */
		Directive* GetDirective(std::string name);
		/** Try to find a n instruction in the instruction set. */
		Instruction* GetInstruction(std::string name);
		/** Try to find a named label in assembled labels. */
		Label* GetLabel(std::string name);
		/** Create a #DEFINE symbol, optionaly with a given string value. */
		DefSymbol* CreateDefSymbol(std::string name, std::string value);
		/** Delete a #DEFINE symbol. */
		bool DeleteDefSymbol(std::string name);
		/** Check if a symbol is #DEFINEd.*/
		bool ExistSymbol(std::string name);
		/** Try to replace a symbol from the #DEFINE table. */
		bool ReplaceDefSymbol(ParseToken& token);
		/** try to replace a symbol from the Label table */
		bool ReplaceLabel(std::string& source);

	};
}
#endif /* Assembler_h */
