//
//  MuzBridge.h - header included in bridging.
//  MUZ-Workshop
//
//  Created by Francis Pierot on 26/11/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef ObjcMemoryModule_h
#define ObjcMemoryModule_h

#import <Foundation/Foundation.h>

// structure to hold registers
@interface MuzRegisters  : NSObject
	@property (nonatomic) uint16_t		af;
	@property (nonatomic) uint16_t		afprime;
	@property (nonatomic) uint16_t 		bc;
	@property (nonatomic) uint16_t 		de;
	@property (nonatomic) uint16_t 		hl;
	@property (nonatomic) uint16_t 		bcprime;
	@property (nonatomic) uint16_t 		deprime;
	@property (nonatomic) uint16_t 		hlprime;
	@property (nonatomic) uint16_t		ix;
	@property (nonatomic) uint16_t		iy;
	@property (nonatomic) uint16_t		sp;
	@property (nonatomic) uint16_t		pc;
	@property (nonatomic) uint16_t		ir;
	@property (nonatomic) uint16_t		iff;
@end

@interface MuzBridge : NSObject

// store a pointer to the C++ instance of a memory module
@property (nonatomic) void* _Nullable computer;
@property (nonatomic) void* _Nullable pagingport;

// current program counter
@property (nonatomic) int PC;

// initialize the computer
-(void) start;

// do a step from current program counter
-(void) step;

// Assemble a source file
-(void) assemble;

// get current values for registers
-(nonnull MuzRegisters*) registers;

@end

#endif /* MuzBridge.h */
