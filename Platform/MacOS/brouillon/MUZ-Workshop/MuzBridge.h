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

/** Structure to hold the Z-80 registers. */
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

/** Structure to return a file specification. */
@interface MuzSourceFile : NSObject
	@property (nonatomic) bool		    included; 		// true for #INCLUDEd files
	@property (nonatomic) uint16_t		parentfile;		// if included, parent file reference
	@property (nonatomic) uint16_t		parentline;		// if included, line number in parent file
	@property (nonnull) NSString*		filepath;		// path part of the file
	@property (nonnull) NSString*		filename;		// filename part of the file
// std::vector<CodeLine> lines;	// parsed/assembled content, matches the source file lines
// LabelMap	labels;				// local labels
@end


/** Structure to hold a recursive files list.
@interface MuzFiles = NSObject
	@property NSString name;
	@property MuzFiles content; //$$ must return an array-> how to do this?
@end
*/

/** The bridge interface for swift calls. */
@interface MuzBridge : NSObject


// MARK: - MUZ-Assembler interface

/** Sets the output directory. */
-(void) setOutputDirectory:(nonnull NSString*)directory;

/** Sets the listing filename. */
-(void) setListingFilename:(nonnull NSString*)filename fullListing:(Boolean)fullListing trace:(Boolean)trace;

/** Assembles a main source. */
// "/Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm"
-(void) assemble:(nonnull NSString*)mainSource;

/** Gets the number of source files after assembling. */
-(int) filesNumber;

/** Gets the specification for one source file after assembling. */
-(nonnull MuzSourceFile*) getSourceFile: (int)index;


// MARK: - MUZ-Computer interface

// store pointers to the C++ instance of the computer module
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
