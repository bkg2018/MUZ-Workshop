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
	@property (nonatomic) int16_t		af;
	@property (nonatomic) int16_t		afprime;
	@property (nonatomic) int16_t 		bc;
	@property (nonatomic) int16_t 		de;
	@property (nonatomic) int16_t 		hl;
	@property (nonatomic) int16_t 		bcprime;
	@property (nonatomic) int16_t 		deprime;
	@property (nonatomic) int16_t 		hlprime;
	@property (nonatomic) int16_t		ix;
	@property (nonatomic) int16_t		iy;
	@property (nonatomic) int16_t		sp;
	@property (nonatomic) int16_t		pc;
	@property (nonatomic) int16_t		ir;
	@property (nonatomic) int16_t		iff;
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

// get current values for registers
-(nonnull MuzRegisters*) registers;

@end

#endif /* MuzBridge.h */