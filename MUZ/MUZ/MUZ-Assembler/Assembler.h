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
		/** Definition for one source file. */
		struct SourceFile
		{
			bool	included = false; 		// true for #INCLUDEd files
			int		parentfile = -1;		// if included, parent file reference
			int		parentline = -1;		// if included, line number in parent file
			std::string fileprefix;			// Windows specific prefixes
			std::string filepath;			// path part of the file
			std::string filename;			// filename part of the file
			std::vector<CodeLine> lines;	// parsed/assembled content, matches the source file lines
			
			/** set from the given filename and include / parentfile/line specs. */
			bool Set(std::string file, SourceFile* parent);
		};

	private:
		
		// Map tables for the instruction set and directives
		InstructionsMap				m_instructions;
		DirectivesMap				m_directives;
		
		// Tables for the assembly
		DefSymbolsMap				m_defsymbols;
		LabelMap					m_labels;
		std::vector<SourceFile*>	m_files;
		std::vector<AddressZone*>	m_zones;
		
		// Stack for imbricated conditionnal modes
		ParsingModeStack			m_modes;	// stack of all imbricated levels, root being the first and never popped
		size_t						m_curlevel; // should be 1 when at root
		
		/** Current assembly process status, */
		struct AssemblyStatus {
			ADDRESSTYPE curaddress;	// current assembling address
			Name*		curname;	// current symbol / label / equate
		} ;
		AssemblyStatus				m_status;
		
		/** Output directory and file names. */
		std::string					m_outputdir;
		std::string					m_binfilename;
		std::string 				m_hexfilename;
		std::string					m_listingfilename;
		std::string					m_memoryfilename;
		std::string					m_symbolsfilename;
		FILE*						m_listingfile = nullptr;

		
		/** Assemble a prepared code line. */
		bool Assemble(CodeLine& codeline, ErrorList& msg);
		

		/** Prepare listing file, close previous if any. */
		void PrepareListing(ErrorList& msg);
		/** Generate a listing for an assembled codeline. */
		void GenerateListing(CodeLine& codeline, ErrorList& msg);
		
		// following are mainly for use by Parser class and/or Assembler internals
		//friend class Parser;

		/** Scan and create the labels on the current line/ */
		Label* ScanLabel(CodeLine& codeline, ErrorList& msg);
		/** Create a label at current address. */
		Label* CreateLabel(std::string name, int file, int pline);

		// Conditionnal Parsing modes
		
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
		
		// Public API
	public:
		
		/** Initialize directives and instructions. */
		Assembler();
		virtual ~Assembler();
		
		/** Reset the assembly. */
		void Reset();
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
		/** Sets the current assembling address. */
		void SetAddress(ADDRESSTYPE address);
		/** Returns the current address. */
		ADDRESSTYPE GetAddress();

		/** Assembles a main or included source file. */
		bool Assemble(std::string file, ErrorList& msg, bool included, CodeLine& codeline);
		/** Assemble a single line and return a codeline */
		CodeLine Assemble(std::string sourceline, ErrorList& msg);
		
		
		/** Try to find a directive in the # and . directives array. */
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
