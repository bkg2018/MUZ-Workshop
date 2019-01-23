//
//  MUZ_WorkshopTests.m
//  MUZ-WorkshopTests
//
//  Created by Francis Pierot on 27/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <QuartzCore/QuartzCore.h>
#include <chrono>
#include <thread>
// emulation
#include "MUZ-Computer/ROMPagingPort.h"
#include "muz_simz80.h"

// WIP
//#include "MUZ-Assembler/AddressZone.h"

// assembling and parsing
#include "MUZ-Assembler/Parser.h"
#include "MUZ-Assembler/Assembler.h"
#include "MUZ-Assembler/Expression.h"
#include "MUZ-Assembler/Z80-Operands.h"
#include "MUZ-Common/StrUtils.h"

#include <iostream>
using std::cout;

@interface MUZ_WorkshopTests : XCTestCase

@end

@implementation MUZ_WorkshopTests

const std::string SourcesRootDir = "/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/MUZ-GIT/MUZ/TestSources/";
std::string RomHexFilePath ;
std::string SourceFilePath;

- (void)setUp {
	
	RomHexFilePath = "/Users/bkg2018/Desktop/IntelHex.hex";
	SourceFilePath = "/Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm";
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testMemoryModule {
	MUZ::MemoryModule mem;
	mem.SetROM(RomHexFilePath);
	
	MUZ::BYTE b0 = mem[0];
	mem[0] = ~b0;// no effect in ROM
	XCTAssert(b0 == mem[0]);
	mem[1] = mem[2];
	XCTAssert(mem[1] != mem[2]);
	
	mem.SetRAM(RomHexFilePath);
	MUZ::BYTE b1 = mem[0];
	mem[0] = ~b1;
	XCTAssert(b1 != mem[0]);
	mem[1] = mem[2];
	XCTAssert(mem[1] == mem[2]);

}

- (void)testMemoryManager {
	MUZ::MemoryMgr mmgr;
	// prepare 64KB RAM
	mmgr.SetMaxRAM();
	// Load SC Monitor + Basic + CP/M loader
	mmgr.SetROM(RomHexFilePath);
	
	// we now have:
	// ROM from 0000 to 3FFF
	// RAM from 4000 to FFFF
	MUZ::DATATYPE value;

	// test ROM
	value = mmgr[0]; // should be C3
	XCTAssertEqual(value, 0xC3);
	mmgr[0] = 0; // should not write
	value = mmgr[0]; // should be C3
	XCTAssertEqual(value, 0xC3);

	// test RAM
	mmgr[0x4000] = 0xAA;
	XCTAssertEqual(mmgr[0x4000], 0xAA);
	mmgr[0x4000] = ~mmgr[0x4000]; // should become 0x55
	XCTAssertEqual(mmgr[0x4000], 0x55);
	
	// test ROM paging out
	mmgr.PageROMout();

	value = mmgr[0]; // should be 00
	XCTAssertEqual(value, 0x00);
	mmgr[0] = 0x55;
	value = mmgr[0]; // should be C3
	XCTAssertEqual(value, 0x55);

}

/** unused
- (void) testAddressZone {
	
	MUZ::AddressZone azone;
	azone.Reset(0x2000, 0x800);
	
	MUZ::AddressZone az(0x2000,0x800);
	// Check the out of range exceptions
	bool exceptionok = false;
	try {
		az[0x1FFF] = 0;
	} catch (std::exception& e) {
		exceptionok = true;
	}
	XCTAssertTrue(exceptionok);
	exceptionok = false;
	try {
		az[0x2800] = 0;
	} catch (std::exception& e) {
		exceptionok = true;
	}
	XCTAssertTrue(exceptionok);

	
	// Case 1: new zone embed existing
	az.Reset(0x2000,0x800);		// 2000-2800
	az.Mark(0x1000,0x2000);		// 1000-3000, overwrites 2000-2800
	XCTAssertEqual(az[0x1FFF], 1); // in new zone but not previous
	XCTAssertEqual(az[0x2000], 2); // in both zones
	XCTAssertEqual(az[0x27ff], 2); // in both zones, last byte of previous
	XCTAssertEqual(az[0x2800], 1); // in new zone only

	// Case 2: new zone starts before and partially overwrite
	az.Reset(0x2000,0x800);		// 2000-2800
	az.Mark(0x1800,0x0A00);		// 1800-2200
	XCTAssertEqual(az[0x1fff], 1);
	XCTAssertEqual(az[0x2000], 2);
	XCTAssertEqual(az[0x21ff], 2);
	XCTAssertEqual(az[0x2200], 1);

	// Case 3: new zone starts and end before
	az.Reset(0x2000,0x800);		// 2000-2800
	az.Mark(0x1000,0x800); 		// 1000-1800
	XCTAssertEqual(az[0x11ff], 1);
	XCTAssertEqual(az[0x1800], 0);
	XCTAssertEqual(az[0x1fff], 0);
	XCTAssertEqual(az[0x2000], 1);

	// Case 4: new zone entirely in existing
	az.Reset(0x2000,0x800);		// 2000-2800
	az.Mark(0x2400,0x200);		// 2400-2600
	XCTAssertEqual(az[0x23ff], 1);
	XCTAssertEqual(az[0x2400], 2);
	XCTAssertEqual(az[0x25ff], 2);
	XCTAssertEqual(az[0x2600], 1);
	
	// Case 5: new zone starts in existing and ends after
	az.Reset(0x2000,0x800);		// 2000-2800
	az.Mark(0x2400,0x1000);		// 2400-3400
	XCTAssertEqual(az[0x23ff], 1);
	XCTAssertEqual(az[0x2400], 2);
	XCTAssertEqual(az[0x27ff], 2);
	XCTAssertEqual(az[0x2800], 1);

	// Case 6: new zone starts and ends after
	az.Reset(0x2000,0x800);		// 2000-2800
	az.Mark(0x3000,0x800);		// 3000-3800
	XCTAssertEqual(az[0x27FF], 1);
	XCTAssertEqual(az[0x2800], 0);
	XCTAssertEqual(az[0x2FFF], 0);
	XCTAssertEqual(az[0x3000], 1);
}
*/
- (void)testComputer {
	
	// the virtual computer
	MUZ::Computer computer;

	// Equip with 64KB RAM
	computer.SetMaxRAM();

	// Load SC Monitor + Basic + CP/M loader in ROM (this file fills 16KB at addresses $0000->$3FFF)
	computer.SetROM(RomHexFilePath);
	
	// create a switching paging port
	MUZ::ROMPagingPort pagingport(&computer, true);
	// assign it to port 38
	computer.Assign(0x38, &pagingport);
	
	// we now have:
	// ROM from 0000 to 3FFF
	// RAM from 4000 to FFFF
	
	// test ROM
	XCTAssertEqual(computer[0], 0xC3);
	computer[0] = 0; // should not write
	XCTAssertEqual(computer[0], 0xC3);
	
	// test RAM
	computer[0x4000] = 0xAA;
	XCTAssertEqual(computer[0x4000], 0xAA);
	computer[0x4000] = ~computer[0x4000]; // should become 0x55
	XCTAssertEqual(computer[0x4000], 0x55);
	
	// test ROM paging out by sending 0 (switching mode)
	computer.Out(0x38, 0);
	XCTAssertEqual(computer[0], 0x00);
	computer[0] = 0x55;
	XCTAssertEqual(computer[0], 0x55);
	
	// test ROM paging in
	computer.Out(0x38, 0);
	XCTAssertEqual(computer[0], 0xC3);
	computer[0] = 0x55;
	XCTAssertEqual(computer[0], 0xC3);
	
	// change to non switching mode
	pagingport.SetSwitchMode(false);
	
	// test ROM paging out by sending 1 to port
	computer.Out(0x38, 1);
	XCTAssertEqual(computer[0], 0x55);
	computer[0] = ~computer[0];
	XCTAssertEqual(computer[0], 0xAA);
	
	// test ROM paging in
	computer.Out(0x38, 0);
	XCTAssertEqual(computer[0], 0xC3);
	computer[0] = 0x55;
	XCTAssertEqual(computer[0], 0xC3);
}

- (void)testZ80 {
	
	// the virtual computer
	YazeZ80 computer;
	computer.InitRegisters();
	computer.pc = 0;
	computer.SetMaxRAM();
	computer.SetROM(RomHexFilePath);
	MUZ::ROMPagingPort pagingport(&computer, true);
	computer.Assign(0x38, &pagingport);
	// dummy ports for other modules
	MUZ::PortModule port;
	computer.Assign(0, &port);		// IO
	computer.Assign(0x80, &port);	// SIO2
	computer.Assign(0x81, &port);
	computer.Assign(0x82, &port);
	computer.Assign(0x83, &port);

	// we now have:
	// ROM from 0000 to 3FFF
	// RAM from 4000 to FFFF

	// go for it ?
	computer.simz80(0, true);
	
}

- (void)testConditionnalsAssembler {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testConditionnalsAssembler.LST");
	as.AssembleFile(SourcesRootDir + "Conditionnals.asm", msg);
	// dump warnings?
	for (MUZ::ErrorMessage& m : msg) {
		if (m.type == MUZ::errorTypeWARNING) {
			printf("%s(%d): %s\n", as.GetFileName(m.file).c_str(), (int)m.line, msg.GetMessage(m.kind).c_str());
		}
	}
}

- (void)testExpressionsAssembler {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testExpressionsAssembler.LST");
	as.AssembleFile(SourcesRootDir + "Expressions.asm", msg);
}

-(void) testInclude {
	MUZ::Assembler as;
	MUZ::CodeLine codeline;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testInclude.LST");
	codeline = as.AssembleLine("#INCLUDE    /Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/Hardware\\Custom\\Config_R0.asm", msg);
}

-(void) testEquate {
	MUZ::Assembler as;
	MUZ::CodeLine codeline;
	MUZ::ErrorList msg;
	MUZ::Label* label;
	codeline = as.AssembleLine("NotLabel: .EQU $1234", msg);
	label = as.GetLabel("NotLabel");
	XCTAssertNotEqual(label, nullptr);
	if (label) {
		XCTAssertTrue(label->equate);
		XCTAssertEqual(label->AddressFrom(0), 0x1234);
	}
}

-(void) testDB {
	MUZ::Assembler as;
	MUZ::CodeLine codeline;
	MUZ::ErrorList msg;
	codeline = as.AssembleLine(".DB  0xDB,0xFF,0x44,0x23,0x03", msg);
}

-(void) testParseUnitaryExpressions {
	MUZ::Assembler as;
	MUZ::CodeLine codeline;
	MUZ::ErrorList msg;
	MUZ::ParseToken result;
	MUZ::ExpressionEvaluator eval;
	int lasttoken = -1;

	codeline = as.AssembleLine("0xFFFF & 01010b", msg);
	lasttoken = -1;
	result = eval.Evaluate(codeline.tokens, 0, lasttoken);
	XCTAssertEqual(result.source, "10"); // 1010b
	XCTAssertEqual(result.type, MUZ::tokenTypeDECNUMBER);

	codeline = as.AssembleLine("0xAAAA | 5555h", msg);
	lasttoken = -1;
	result = eval.Evaluate(codeline.tokens, 0, lasttoken);
	XCTAssertEqual(result.source, "65535");
	XCTAssertEqual(result.type, MUZ::tokenTypeDECNUMBER);

	codeline = as.AssembleLine("0xAAAA ^ FFFFh", msg);
	lasttoken = -1;
	result = eval.Evaluate(codeline.tokens, 0, lasttoken);
	XCTAssertEqual(result.source, "21845"); // 5555h
	XCTAssertEqual(result.type, MUZ::tokenTypeDECNUMBER);

	codeline = as.AssembleLine("(1=0) && (1=1)", msg);
	lasttoken = -1;
	result = eval.Evaluate(codeline.tokens, 0, lasttoken);
	XCTAssertEqual(result.source, ""); // false  
	XCTAssertEqual(result.type, MUZ::tokenTypeBOOL);

	codeline = as.AssembleLine("(1=0) || (1=1)", msg);
	lasttoken = -1;
	result = eval.Evaluate(codeline.tokens, 0, lasttoken);
	XCTAssertEqual(result.source, "t"); // true
	XCTAssertEqual(result.type, MUZ::tokenTypeBOOL);
	
	
	codeline = as.AssembleLine("0xFFFF", msg);
	XCTAssertEqual(codeline.tokens.size(), 1);
	XCTAssertEqual(codeline.tokens[0].source, "65535");
	codeline = as.AssembleLine("0b10101010",  msg);
	XCTAssertEqual(codeline.tokens.size(), 1);
	XCTAssertEqual(codeline.tokens[0].source, "170"); // 0xAA
	codeline = as.AssembleLine("65536",  msg);
	XCTAssertEqual(codeline.tokens.size(), 1);
	XCTAssertEqual(codeline.tokens[0].source, "65536");
	codeline = as.AssembleLine("-1",  msg);
	XCTAssertEqual(codeline.tokens.size(), 2);
	XCTAssertEqual(codeline.tokens[0].type, MUZ::tokenTypeOP_MINUS);
	XCTAssertEqual(codeline.tokens[1].type, MUZ::tokenTypeDECNUMBER);
	codeline = as.AssembleLine("1 + 2 * 4",  msg);
	XCTAssertEqual(codeline.tokens.size(), 5);
	XCTAssertEqual(codeline.tokens[0].type, MUZ::tokenTypeDECNUMBER);
	XCTAssertEqual(codeline.tokens[1].type, MUZ::tokenTypeOP_PLUS);
	XCTAssertEqual(codeline.tokens[2].type, MUZ::tokenTypeDECNUMBER);
	XCTAssertEqual(codeline.tokens[3].type, MUZ::tokenTypeOP_MUL);
	XCTAssertEqual(codeline.tokens[4].type, MUZ::tokenTypeDECNUMBER);

	
	
	codeline = as.AssembleLine(".UNKNOWN 'nothing', 0x7FFF",  msg);
	codeline = as.AssembleLine("Start: .EQU 0x8000",  msg);
	codeline = as.AssembleLine("Origin: .ORG 0x8000",  msg);

}

-(void) testEvaluateExpressions {

	MUZ::ExpressionEvaluator eval;
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	MUZ::Parser parser(as); // give reference of the assembler to the parser
	MUZ::CodeLine codeline;
	MUZ::ParseToken result;
	int lasttoken = -1;
	codeline.source = "! ( 1 + 2 * ( 3 + 4 )) + 5 * ( 6 + 7 )";
	parser.Split(codeline, msg);
	result = eval.Evaluate(codeline.tokens, 0, lasttoken);
	XCTAssertEqual(result.source, "49");
	
	// check parenthesis errors
	codeline.source = "0 ( 1 ( 2 ( 3 ) 2 ) 1 ) 0"; // ok
	parser.Split(codeline, msg);
	XCTAssertEqual( eval.CheckParenthesis(codeline.tokens ), 0);

	codeline.source = "0 ( 1 ( 2 ( 3 ) 2 ) 1 "; // ok
	parser.Split(codeline, msg);
	XCTAssertEqual(eval.CheckParenthesis(codeline.tokens ), +1);

	codeline.source = " 1 ( 2 ( 3 ) 2 ) 1 ) 0"; // ok
	parser.Split(codeline, msg);
	XCTAssertEqual(eval.CheckParenthesis(codeline.tokens ), -1);

		

}

/** Tests various minor stuff for a good code coverage. */
-(void) testCoverage {
	MUZ::Operator op;
	MUZ::ParseToken token, tok2;
	MUZ::ParseToken result;
	const std::string somestring("*** some random string ***");
	
	result.source = somestring;
	result = op.Exec(token, tok2); // does a nop!
	XCTAssertEqual(result.source, "");
	
}

/** Tests the operand types detection functions. */
-(void) testOperandTypes {

	MUZ::Assembler as;
	MUZ::ErrorList msg;
	MUZ::Parser parser(as); // give reference of the assembler to the parser
	MUZ::ParseToken token = {"", MUZ::tokenTypeLETTERS};
	MUZ::ParseToken tokPO ={"", MUZ::tokenTypePAROPEN};;
	MUZ::ParseToken tokPC ={"", MUZ::tokenTypePARCLOSE};;
	token.type = MUZ::tokenTypeLETTERS;
	MUZ::Z80::OperandType optype;
	int value;
	MUZ::Z80::OperandTools optools;
	// 8 bit registers tests
	{
		std::vector<std::string> reg8ok = {"A", "B", "C", "D", "E", "H", "L", "I", "R" };
		std::vector<std::string> reg8bad = {"J", "SP", "", "0", "(C)" };
		for (auto & reg: reg8ok) {
			token.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(token);
			MUZ::CodeLine codeline;
			codeline.source = std::string("LD A,") + reg;
			parser.Split(codeline, msg);
			codeline.curtoken = 3;
			MUZ::OperandError result = optools.GetReg8(codeline, optype);
			XCTAssertEqual(result, MUZ::operrOK);
		}
		for (auto & reg: reg8bad) {
			token.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(token);
			MUZ::CodeLine codeline;
			codeline.source = std::string("LD A,") + reg;
			parser.Split(codeline, msg);
			codeline.curtoken = 3;
			MUZ::OperandError result = optools.GetReg8(codeline, optype);
			XCTAssertNotEqual(result, MUZ::operrOK);
		}
	}
	// 16 bit registers tests
	{
		std::vector<std::string> reg16ok = {"AF", "AF'", "BC", "DE", "HL", "SP", "IX", "IY" };
		std::vector<std::string> reg16bad = {"J", "(SP)", "", "0", "(C)" };
		for (auto & reg: reg16ok) {
			token.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(token);
			int curtoken = 0;
			bool result = optools.reg16(&tokens, curtoken, optype);
			XCTAssertEqual(result, true);
		}
		for (auto & reg: reg16bad) {
			token.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(token);
			int curtoken = 0;
			bool result = optools.reg16(&tokens, curtoken, optype);
			XCTAssertEqual(result, false);
		}
	}
	// various indirect via register
	{
		{
			MUZ::ExpVector tokens;
			token.source = "C";
			tokens.push_back(tokPO);
			tokens.push_back(token);
			tokens.push_back(tokPC);
			int curtoken = 0;
			bool result = optools.indirectC(&tokens, curtoken, optype);
			XCTAssertEqual(result, true);
		}
		{
			token.source = "HL";
			MUZ::ExpVector tokens;
			tokens.push_back(tokPO);
			tokens.push_back(token);
			tokens.push_back(tokPC);
			int curtoken = 0;
			bool result = optools.indirectHL(&tokens, curtoken, optype);
			XCTAssertEqual(result, true);
		}
		{
			token.source = "SP";
			MUZ::ExpVector tokens;
			tokens.push_back(tokPO);
			tokens.push_back(token);
			tokens.push_back(tokPC);
			int curtoken = 0;
			bool result = optools.indirectSP(&tokens, curtoken, optype);
			XCTAssertEqual(result, true);
		}
		{
			// (IX+8)
			MUZ::ExpVector tokens;
			tokens.push_back(tokPO);
			token.source = "IX";
			tokens.push_back(token);
			token.source = "+";
			token.type = MUZ::tokenTypeOP_PLUS;
			tokens.push_back(token);
			token.source = "8";
			token.type = MUZ::tokenTypeDECNUMBER;
			tokens.push_back(token);
			tokens.push_back(tokPC);
			int curtoken = 0;
			MUZ::OperandError result = optools.indirectX(&tokens, curtoken, optype, value);
			XCTAssertEqual(result, MUZ::operrOK);
			XCTAssertEqual(optype, MUZ::Z80::regIX);
			XCTAssertEqual(value, 8);
		}
	}
	// bit number
	{
		MUZ::ExpVector tokens;
		token.source = "7";
		token.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(token);
		int curtoken = 0;
		MUZ::OperandError result = optools.bitnumber( &tokens, curtoken, optype);
		XCTAssertEqual(result, MUZ::operrOK);
		XCTAssertEqual(optype, MUZ::Z80::bit7);
	}
	// conditions
	{
		std::vector<std::string> condok = {"NC", "C", "NZ", "Z", "PE", "PO", "M", "P" };
		std::vector<std::string> condbad = {"CY", "PA", "", "0" };
		for (auto & reg: condok) {
			token.source = reg;
			token.type = MUZ::tokenTypeLETTERS;
			MUZ::ExpVector tokens;
			tokens.push_back(token);
			int curtoken = 0;
			MUZ::OperandError result = optools.condition(&tokens, curtoken, optype);
			XCTAssertEqual(result, MUZ::operrOK);
		}
		for (auto & reg: condbad) {
			token.source = reg;
			token.type = MUZ::tokenTypeLETTERS;
			MUZ::ExpVector tokens;
			tokens.push_back(token);
			int curtoken = 0;
			MUZ::OperandError result = optools.condition(&tokens, curtoken, optype);
			XCTAssertNotEqual(result, MUZ::operrOK);
		}
	}
	// 8-bit direct data
	{
		MUZ::ExpVector tokens;
		token.source = "127";
		token.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(token);
		int curtoken = 0;
		MUZ::OperandError result = optools.number8( &tokens, curtoken, value);
		XCTAssertEqual(result, MUZ::operrOK);
		XCTAssertEqual(value, 127);
	}
	// 16-bit direct data
	{
		MUZ::ExpVector tokens;
		token.source = "65535";
		token.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(token);
		int curtoken = 0;
		MUZ::OperandError result = optools.number16( &tokens, curtoken, value);
		XCTAssertEqual(result, MUZ::operrOK);
		XCTAssertEqual(value, 65535);
	}
	// indirect 16-bit address
	{
		MUZ::ExpVector tokens;
		token.source.clear();
		token.type = MUZ::tokenTypePAROPEN;
		tokens.push_back(token);
		token.source = "65535";
		token.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(token);
		token.source.clear();
		token.type = MUZ::tokenTypePARCLOSE;
		tokens.push_back(token);
		int curtoken = 0;
		int lasttoken = 0;
		bool result = (MUZ::operrOK==optools.indirect16( &tokens, curtoken, value, lasttoken));
		XCTAssertTrue(result);
		XCTAssertEqual(value, 65535);
	}
}

-(void) testInstructionCodes {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testInstructionsCodes.LST");
	as.SetMemoryFilename("testInstructionsCodesMemory.LST");
	as.SetIntelHexFilename("testInstructionsCodesIntelHex.HEX");
	as.AssembleFile(SourcesRootDir + "Instructions.asm", msg); // false = not included
}

-(void) testZ80InstructionCodes {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testZ80InstructionsCodes.LST");
	as.SetMemoryFilename("testZ80InstructionsCodesMemory.LST");
	as.SetIntelHexFilename("testZ80InstructionsCodesIntelHex.HEX");
	as.SetLogFilename("testZ80InstructionsCodes.LOG");
	as.EnableFullListing(true); // don't limit .DB/.DS/.DW sequences to 8 bytes
	as.AssembleFile(SourcesRootDir + "Z80-Instructions.asm", msg); // false = not included
}

-(void) testErrors {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testErrors.LST");
	as.SetMemoryFilename("testErrorsMemory.LST");
	as.SetIntelHexFilename("testErrorsIntelHex.HEX");
	as.SetLogFilename("testErrors.LOG");
	as.EnableFullListing(true); // don't limit .DB/.DS/.DW sequences to 8 bytes
	as.AssembleFile(SourcesRootDir + "Errors.asm", msg); // false = not included
}

- (void)testAssembler {

//	CFTimeInterval startTime = CACurrentMediaTime();
	std::chrono::high_resolution_clock Clock;
	auto startTime = Clock.now();

	int nbloops = 1;
	for (int i = 0 ; i < nbloops ; i++) {
		//printf("%d : start time %f\n", i+1, CACurrentMediaTime()-startTime);
		MUZ::Assembler as;
		MUZ::ErrorList msg;
		as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
		as.SetListingFilename("testAssembler.LST");
		as.SetMemoryFilename("testAssemblerMemory.LST");
		as.SetIntelHexFilename("testAssemblerIntelHex.HEX");
		as.SetLogFilename("testAssembler.LOG");
		as.EnableFullListing(false); // limit .DB/.DS/.DW sequences to 8 bytes
		try {
			as.AssembleFile(SourceFilePath, msg);
		} catch (std::exception &e) {
			perror(e.what());
		}
	}
	// CFTimeInterval elapsedTime = CACurrentMediaTime() - startTime;
	double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Clock.now() - startTime).count() / 1000.0;
	printf("Time per assembly: %f\n", elapsedTime / nbloops);

}


@end
