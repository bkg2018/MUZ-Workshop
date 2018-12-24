//
//  MUZ_WorkshopTests.m
//  MUZ-WorkshopTests
//
//  Created by Francis Pierot on 27/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#import <XCTest/XCTest.h>

// emulation
#include "MUZ-Computer/ROMPagingPort.h"
#include "muz_simz80.h"

// assembling and parsing
#include "Parser.h"
#include "Assembler.h"
#include "Expression.h"
#include "Z80-Operands.h"

#include <iostream>
using std::cout;

@interface MUZ_WorkshopTests : XCTestCase

@end

@implementation MUZ_WorkshopTests

const std::string SourcesRootDir = "/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/MUZ-GIT/MUZ/TestSources/";
std::string RomHexFilePath ;
std::string SourceFilePath;
std::string ConditionnalsSourcePath;
std::string ExpressionsSourcePath;
std::string InstructionsSourcePath;

- (void)setUp {
	
	RomHexFilePath = "/Users/bkg2018/Desktop/IntelHex.hex";
	SourceFilePath = "/Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm";
	ConditionnalsSourcePath = SourcesRootDir + "Conditionnals.asm";
	ExpressionsSourcePath = SourcesRootDir + "Expressions.asm";
	InstructionsSourcePath = SourcesRootDir + "Instructions.asm";
	MUZ::initRegisterMap();
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

- (void)testAssembler {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	MUZ::CodeLine codeline;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testAssembler.LST");
	try {
		as.AssembleFile(SourceFilePath, false, codeline, msg); // false = not included
	} catch (std::exception &e) {
		perror(e.what());
	}
}

- (void)testConditionnalsAssembler {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	MUZ::CodeLine codeline;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testConditionnalsAssembler.LST");
	as.AssembleFile(ConditionnalsSourcePath, false, codeline, msg); // false = not included
}

- (void)testExpressionsAssembler {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	MUZ::CodeLine codeline;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testExpressionsAssembler.LST");
	as.AssembleFile(ExpressionsSourcePath, false, codeline, msg); // false = not included
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
		XCTAssertEqual(label->address, 0x1234);
	}
}

-(void) testParseUnitaryExpressions {
	MUZ::Assembler as;
	MUZ::CodeLine codeline;
	MUZ::ErrorList msg;
	MUZ::ParseToken result;
	MUZ::ExpressionEvaluator eval;

	codeline = as.AssembleLine("0xFFFF & 01010b", msg);
	result = eval.Evaluate(codeline.tokens, 0);
	XCTAssertEqual(result.source, "10"); // 1010b
	XCTAssertEqual(result.type, MUZ::tokenTypeDECNUMBER);

	codeline = as.AssembleLine("0xAAAA | 5555h", msg);
	result = eval.Evaluate(codeline.tokens, 0);
	XCTAssertEqual(result.source, "65535");
	XCTAssertEqual(result.type, MUZ::tokenTypeDECNUMBER);

	codeline = as.AssembleLine("0xAAAA ^ FFFFh", msg);
	result = eval.Evaluate(codeline.tokens, 0);
	XCTAssertEqual(result.source, "21845"); // 5555h
	XCTAssertEqual(result.type, MUZ::tokenTypeDECNUMBER);

	codeline = as.AssembleLine("(1=0) && (1=1)", msg);
	result = eval.Evaluate(codeline.tokens, 0);
	XCTAssertEqual(result.source, ""); // false  
	XCTAssertEqual(result.type, MUZ::tokenTypeBOOL);

	codeline = as.AssembleLine("(1=0) || (1=1)", msg);
	result = eval.Evaluate(codeline.tokens, 0);
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
	MUZ::ExpVector& tokens = codeline.tokens;
	int curtoken = 0;
	parser.Init(tokens, curtoken); // give references of this codeline array and index to the parser
	
	codeline.source = "! ( 1 + 2 * ( 3 + 4 )) + 5 * ( 6 + 7 )"; 
	parser.Split(codeline.source, msg);
	result = eval.Evaluate(codeline.tokens, 0);
	XCTAssertEqual(result.source, "49");
	
	// check parenthesis errors
	codeline.source = "0 ( 1 ( 2 ( 3 ) 2 ) 1 ) 0"; // ok
	parser.Split(codeline.source, msg);
	XCTAssertTrue( eval.CheckParenthesis(codeline.tokens ));

	codeline.source = "0 ( 1 ( 2 ( 3 ) 2 ) 1 "; // ok
	parser.Split(codeline.source, msg);
	XCTAssertFalse( eval.CheckParenthesis(codeline.tokens ));

	codeline.source = " 1 ( 2 ( 3 ) 2 ) 1 ) 0"; // ok
	parser.Split(codeline.source, msg);
	XCTAssertFalse( eval.CheckParenthesis(codeline.tokens ));

		

}

/** Tests various minor stuff for a good code coverage. */
-(void) testCoverage {
	MUZ::Operator op;
	MUZ::ParseToken tok1, tok2;
	MUZ::ParseToken result;
	const std::string somestring("*** some random string ***");
	
	result.source = somestring;
	result = op.Exec(tok1, tok2); // does a nop!
	XCTAssertEqual(result.source, "");
	
}

/** Tests the operand types detection functions. */
-(void) testOperandTypes {

	MUZ::ParseToken tok1, tok2;
	tok1.type = MUZ::tokenTypeLETTERS;
	MUZ::OperandType optype;
	int value;
	
	// 8 bit registers tests
	{
		std::vector<std::string> reg8ok = {"A", "B", "C", "D", "E", "H", "L", "I", "R" };
		std::vector<std::string> reg8bad = {"J", "SP", "", "0", "(C)" };
		for (auto & reg: reg8ok) {
			tok1.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = reg8(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
		}
		for (auto & reg: reg8bad) {
			tok1.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = reg8(&tokens, 0, optype, value);
			XCTAssertEqual(result, false);
		}
	}
	// 16 bit registers tests
	{
		std::vector<std::string> reg16ok = {"AF", "AF'", "BC", "DE", "HL", "SP", "IX", "IY" };
		std::vector<std::string> reg16bad = {"J", "(SP)", "", "0", "(C)" };
		for (auto & reg: reg16ok) {
			tok1.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = reg16(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
		}
		for (auto & reg: reg16bad) {
			tok1.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = reg16(&tokens, 0, optype, value);
			XCTAssertEqual(result, false);
		}
	}
	// various indirect via register
	{
		{
			tok1.source = "(C)";
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = indirectC(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
		}
		{
			tok1.source = "(HL)";
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = indirectHL(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
		}
		{
			tok1.source = "(SP)";
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = indirectSP(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
		}
		{
			// (IX+8)
			MUZ::ExpVector tokens;
			tok1.source.clear();
			tok1.type = MUZ::tokenTypePAROPEN;
			tokens.push_back(tok1);
			tok1.source = "IX";
			tok1.type = MUZ::tokenTypeLETTERS;
			tokens.push_back(tok1);
			tok1.source = "+";
			tok1.type = MUZ::tokenTypeOP_PLUS;
			tokens.push_back(tok1);
			tok1.source = "8";
			tok1.type = MUZ::tokenTypeDECNUMBER;
			tokens.push_back(tok1);
			tok1.source.clear();
			tok1.type = MUZ::tokenTypePARCLOSE;
			tokens.push_back(tok1);
			bool result = indirectX(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
			XCTAssertEqual(optype, MUZ::indIXd);
			XCTAssertEqual(value, 8);
		}
	}
	// bit number
	{
		MUZ::ExpVector tokens;
		tok1.source = "7";
		tok1.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(tok1);
		bool result = bitnumber( &tokens, 0, optype, value);
		XCTAssertTrue(result);
		XCTAssertEqual(optype, MUZ::bit7);
	}
	// conditions
	{
		std::vector<std::string> condok = {"NC", "C'", "NZ", "Z", "PE", "PO", "M", "P" };
		std::vector<std::string> condbad = {"CY", "PA", "", "0" };
		for (auto & reg: condok) {
			tok1.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = condition(&tokens, 0, optype, value);
			XCTAssertEqual(result, true);
		}
		for (auto & reg: condbad) {
			tok1.source = reg;
			MUZ::ExpVector tokens;
			tokens.push_back(tok1);
			bool result = condition(&tokens, 0, optype, value);
			XCTAssertEqual(result, false);
		}
	}
	// 8-bit direct data
	{
		MUZ::ExpVector tokens;
		tok1.source = "127";
		tok1.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(tok1);
		bool result = number8( &tokens, 0, optype, value);
		XCTAssertTrue(result);
		XCTAssertEqual(optype, MUZ::num8);
		XCTAssertEqual(value, 127);
	}
	// 16-bit direct data
	{
		MUZ::ExpVector tokens;
		tok1.source = "65535";
		tok1.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(tok1);
		bool result = number16( &tokens, 0, optype, value);
		XCTAssertTrue(result);
		XCTAssertEqual(optype, MUZ::num16);
		XCTAssertEqual(value, 65535);
	}
	// indirect 16-bit address
	{
		MUZ::ExpVector tokens;
		tok1.source.clear();
		tok1.type = MUZ::tokenTypePAROPEN;
		tokens.push_back(tok1);
		tok1.source = "65535";
		tok1.type = MUZ::tokenTypeDECNUMBER;
		tokens.push_back(tok1);
		tok1.source.clear();
		tok1.type = MUZ::tokenTypePARCLOSE;
		tokens.push_back(tok1);
		bool result = indirect16( &tokens, 0, optype, value);
		XCTAssertTrue(result);
		XCTAssertEqual(optype, MUZ::ind16);
		XCTAssertEqual(value, 65535);
	}
}

-(void) testInstructionCodes {
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	MUZ::CodeLine codeline;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testInstructionsCodes.LST");
	as.AssembleFile(InstructionsSourcePath, false, codeline, msg); // false = not included
}
/*
- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}
*/
@end
