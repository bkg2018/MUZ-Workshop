//
//  MuzBridge.mm
//  MUZ-Workshop
//
//  Created by Francis Pierot on 26/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#import <Foundation/Foundation.h>

// Bridging Objective-C class
#import "MuzBridge.h"

// MUZ
#include "YAZE/muz_simz80.h"
#include "MUZ-Computer/ROMPagingPort.h"
#include "MUZ-Assembler/Assembler.h"

@implementation MuzRegisters

@end

@implementation MuzSourceFile

@end

@implementation MuzBridge:NSObject

-(id)init
{
	self = [super init];
	
	// Create the derived muz computer
	YazeZ80* computer = new YazeZ80;
	
	// store as a void*, because swift cannot  accept a .H using a C++ class pointer
	self.computer = computer;
	
	// create a memory paging port and store as void* for later
	MUZ::ROMPagingPort* pagingport = new MUZ::ROMPagingPort(computer, true);
	self.pagingport = pagingport;
	
	// initialize the RC2014 computer
	computer->SetMaxRAM();
	computer->SetROM("/Users/bkg2018/Desktop/IntelHex.hex");
	computer->Assign(0x38, pagingport);
	
	// Assign dummy ports on other used port addresses
	MUZ::PortModule* port = new MUZ::PortModule;
	computer->Assign(0, port);		// I/O module
	computer->Assign(0x80, port);	// SIO2 SIOA-Control
	computer->Assign(0x81, port);	// SIO2 SIOA-Data
	computer->Assign(0x82, port);	// SIO2 SIOB-Control
	computer->Assign(0x83, port);	// SIO2 SIOB-Data

	// Create the registers copy
	self.registers = [[ MuzRegisters alloc] init];

	return self;
}

-(void)dealloc
{
	MUZ::Computer* computer = (MUZ::Computer*)self.computer;
	MUZ::ROMPagingPort* pagingport = (MUZ::ROMPagingPort*)self.pagingport;
	if (pagingport) {
		computer->Assign(0x38, nullptr);
		delete (pagingport);
	}
	if (computer) delete computer;
}

/* Example of MUZ use in the bridge, callable from swift with:
 
   let bridge = MuzBridge()
   bridge.test()
*/
-(void) start
{
	self.PC = 0;
	YazeZ80* computer = (YazeZ80*)self.computer;
	computer->InitRegisters();
	computer->pc = self.PC;
}

-(void) step
{
	YazeZ80* computer = (YazeZ80*)self.computer;
	computer->simz80(self.PC, true); // true -> step
	self.PC = computer->pc;
	[self getRegisters];// copy from computer
}

-(MuzRegisters*) getRegisters
{
	YazeZ80* computer = (YazeZ80*)self.computer;
	self.registers.af = computer->af[0];
	self.registers.afprime = computer->af[1];
	self.registers.bc = computer->regs[0].bc;
	self.registers.de = computer->regs[0].de;
	self.registers.hl = computer->regs[0].hl;
	self.registers.bcprime = computer->regs[1].bc;
	self.registers.deprime = computer->regs[1].de;
	self.registers.hlprime = computer->regs[1].hl;
	self.registers.ix = computer->ix;
	self.registers.iy = computer->iy;
	self.registers.sp = computer->sp;
	self.registers.pc = computer->pc;
	self.registers.ir = computer->ir;
	self.registers.iff = computer->IFF;
	return self.registers;
}



/** Assembler. The bridge has a global Assembler instance which does all work. The functions are only
  interfaces to the Assembler class API. */
MUZ::Assembler as;

/** Global errors and warnings list. This is filled by the Assembler::Assemble() APIs. */
MUZ::ErrorList msg;

/** Sets the output directory. */
-(void) setOutputDirectory:(NSString*)directory
{
	as.SetOutputDirectory([directory cStringUsingEncoding:NSUnicodeStringEncoding]);
}

/** Sets the listing filename. */
-(void) setListingFilename:(NSString*)filename fullListing:(Boolean)fullListing trace:(Boolean)trace
{
	as.SetListingFilename([filename cStringUsingEncoding:NSUnicodeStringEncoding]);
	as.EnableFullListing(fullListing);
	as.EnableTrace(trace);
}

// "/Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm"
-(void) assemble:(NSString*)mainSource
{
//	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
//	as.SetListingFilename("testAssembler.LST");
//	as.EnableFullListing(true);
//	as.EnableTrace(false);
	as.Reset();
	msg.Clear();
	try {
		as.AssembleFile([mainSource cStringUsingEncoding:NSUnicodeStringEncoding], msg);
	} catch (std::exception &e) {
		perror(e.what());
	}
}


// Get the number of source files
-(int) filesNumber
{
	return (int)as.m_files.size();
}

// Get specification for one source file
-(nonnull MuzSourceFile*) getSourceFile:(int)index
{
	MuzSourceFile* sourcefile = [[MuzSourceFile alloc] init];
	size_t nbfiles = as.m_files.size();
	if (index < nbfiles) {
		MUZ::Assembler::SourceFile* sf = as.m_files.at(index);
		sourcefile.filename = [NSString stringWithCString:sf->filename.c_str() encoding:NSUnicodeStringEncoding];
		sourcefile.filepath = [NSString stringWithCString:sf->filepath.c_str() encoding:NSUnicodeStringEncoding];
		sourcefile.parentfile = sf->parentfile;
		sourcefile.parentline = sf->parentline;
	}
	return sourcefile;
}


@end
