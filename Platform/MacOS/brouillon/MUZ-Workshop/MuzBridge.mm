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
}

-(MuzRegisters*) registers
{
	MuzRegisters* regs = [[MuzRegisters alloc] init];
	YazeZ80* computer = (YazeZ80*)self.computer;
	regs.af = computer->af[0];
	regs.afprime = computer->af[1];
	regs.bc = computer->regs[0].bc;
	regs.de = computer->regs[0].de;
	regs.hl = computer->regs[0].hl;
	regs.bcprime = computer->regs[1].bc;
	regs.deprime = computer->regs[1].de;
	regs.hlprime = computer->regs[1].hl;
	regs.ix = computer->ix;
	regs.iy = computer->iy;
	regs.sp = computer->sp;
	regs.pc = computer->pc;
	regs.ir = computer->ir;
	regs.iff = computer->IFF;
	return regs;
}

-(void) assemble
{
	MUZ::Assembler as;
	MUZ::ErrorList msg;
	as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
	as.SetListingFilename("testAssembler.LST");
	as.EnableFullListing(true);
	as.EnableTrace(false);
	try {
		as.AssembleFile("/Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm", msg);
	} catch (std::exception &e) {
		perror(e.what());
	}

}
@end
