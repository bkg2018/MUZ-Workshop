//
//  Z180-Instructions.cpp
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//
#include "pch.h"
#include "Assembler.h"
#include "All-Directives.h"
#include "Z80-Operands.h"
#include "Z180-Instructions.h"

using namespace MUZ::Z80;

namespace MUZ {
namespace Z180 {

/* added to the Z-80 instruction set :

	Mnemo			bytes	cycles	states

	MLT ww			2		13		17      ED / 01 ww1 100 (4C/5C/6C/7C)
	MULT ww			idem MLT ww

	OTDM			2		6		14		ED 8B
	OTDMR			2		8		16		ED 9B
	OTIM			2		6		14		ED 83
	OTIMR			2		8		16		ED 93
	IN0 g',(m)		3		4		12		ED / 00 g 000 (00, 08, 10, 18, 20, 28, 30, 38) / m
	OUT0 (m),g''	3		5		13		ED / 00 g 001 (01, 09, 11, 19, 29, 28, 39, 38) / m
	SLP				2		2		8		ED 	76
	TST g			2		3		7		ED / 00 g 100 (04, 0C, 14, 1C, 24, 2C, 34, 3C)
	TST A,g 		idem TST g
	TST m			3		3		9		ED 64 m
	TST A,m			idem TST m
	TSTIO m			3		4		12		ED 74 m
	TST (C),m		idem TSTIO m

	g   : B(000) C(001) D(010) E(011) H(100) L(101) (HL)(110) A(111)
	g'  : B(000) C(001) D(010) E(011) H(100) L(101) F (110)   A(111)
	g'' : B(000) C(001) D(010) E(011) H(100) L(101)           A(111)
	ww  : BC(00) DE(01) HL(10) SP(11)
	m   : 8-bit data

 Synonyms:
 TSTIO 		= TST (C)
 MLT 		= MULT
 TST g 		= TST A,g
 TST m 		= TST A,m
 TST (HL) 	= TST A,(HL)
 */

/** Assemble instruction at current token, returns false if error
 MLT ww			2		13		17      ED / 01 ww1 100 (4C/5C/6C/7C)
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionMLT::Assemble(CodeLine& codeline, ErrorList& msg)
{
	OperandType dest;
	OperandError operr = optools.GetReg16(codeline, dest, REGFLAGS::BC|REGFLAGS::DE|REGFLAGS::HL|REGFLAGS::SP);
	if (operr == operrOK) {
		codeline.SetCycles(13);
		codeline.AddCode(0xED, 0x4C + optools.GetSubCode(dest));
		return true;
	}
	return msg.Error(errorWrongOperand1, codeline);
}
/** Assemble instruction at current token, returns false if error
 OTIMR			2		8		16		ED 93
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionOTIMR::Assemble(CodeLine& codeline, ErrorList& )
{
	codeline.AddCode(0xED, 0x93);
	codeline.SetCycles(8);
	return true;
}
/** Assemble instruction at current token, returns false if error
 OTDMR			2		8		16		ED 9B
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionOTDMR::Assemble(CodeLine& codeline, ErrorList& )
{
	codeline.AddCode(0xED, 0x9B);
	codeline.SetCycles(8);
	return true;
}

/** Assemble instruction at current token, returns false if error
 OTIM			2		6		14		ED 83
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionOTIM::Assemble(CodeLine& codeline, ErrorList& )
{
	codeline.AddCode(0xED, 0x83);
	codeline.SetCycles(6);
	return true;
}
/** Assemble instruction at current token, returns false if error
 OTDM			2		6		14		ED 8B
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionOTDM::Assemble(CodeLine& codeline, ErrorList& )
{
	codeline.AddCode(0xED, 0x8B);
	codeline.SetCycles(6);
	return true;
}

/** Assemble instruction at current token, returns false if error
 IN0 A B C D E F H L, (num8)
 IN0 g',(m)		3		4		12		ED / 00 g 000 (00, 08, 10, 18, 20, 28, 30, 38) / m
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionIN0::Assemble(CodeLine& codeline, ErrorList& msg)
{
	OperandType src;
	int n;
	OperandError operr = optools.GetReg8(codeline, src, REGFLAGS::ABCDEHL | REGFLAGS::F);
	if (operr == operrOK) {
		if (GetComma(codeline)) {
			operr = optools.GetInd16(codeline, n);
			if (operr == operrOK) {
				codeline.AddCode(0xED, 0 + (optools.GetSubCode(src) << 3), n);
				codeline.SetCycles(4);
				return true;
			}
			return msg.Error(errorWrongOperand2, codeline);
		}
		return msg.Error(errorMissingComma, codeline);
	}
	return msg.Error(errorWrongOperand1, codeline);
}

/** Assemble instruction at current token, returns false if error
 OUT0 (num8), A B C D E   H L
 OUT0 (m),g''	3		5		13		ED / 00 g 001 (01, 09, 11, 19, 21, 29, 31, 39) / m
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionOUT0::Assemble(CodeLine& codeline, ErrorList& msg)
{
	OperandType src;
	int n;
	OperandError operr = optools.GetInd16(codeline, n);
	if (operr == operrOK) {
		if (GetComma(codeline)) {
			operr = optools.GetReg8(codeline, src, REGFLAGS::ABCDEHL);
			if (operr == operrOK) {
				codeline.AddCode(0xED, 1 + (optools.GetSubCode(src) << 3), n);
				codeline.SetCycles(5);
				return true;
			}
			return msg.Error(errorWrongOperand2, codeline);
		}
		return msg.Error(errorMissingComma, codeline);
	}
	return msg.Error(errorWrongOperand1, codeline);
}

/** Assemble instruction at current token, returns false if error
 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionSLP::Assemble(CodeLine& codeline, ErrorList& )
{
	codeline.AddCode(0xED, 0x76);
	codeline.SetCycles(2);
	return true;
}

/** Assemble instruction at current token, returns false if error
 tok index 0   1   2   3
 TST A   ,   B/C/D/E/H/L/(HL)/A   ED 04/0C/14/1C/24/2C/34/3C
 TST A   ,   m                    ED 64 m
 TST A   ,   (HL)                 ED 34
 TST (C) ,   m                    ED 74 m

 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionTST::Assemble(CodeLine& codeline, ErrorList& msg)
{
	OperandType dest, src;
	int d;
	// save current position
	size_t curtoken = codeline.curtoken;
	// TST A[,8bit]
	OperandError operr = optools.GetReg8(codeline, dest,REGFLAGS::A);
	if (operr == operrOK) {
		if (!GetComma(codeline)) {
			// TST A
			codeline.AddCode(0xED, 0x3C);
			codeline.SetCycles(3);
			return true;
		}
		// TST A,g
		operr = optools.GetReg8(codeline, src,REGFLAGS::ABCDEHL);
		if (operr == operrOK) {
			codeline.AddCode(0xED, 0x04 + (optools.GetSubCode(src) << 3));
			codeline.SetCycles(3);
			return true;
		}
		// TST A,(HL)
		operr = optools.GetIndHL(codeline);
		if (operr == operrOK) {
			codeline.AddCode(0xED, 0x34);
			codeline.SetCycles(4);
			return true;
		}
		// TST A,n
		operr = optools.GetNum8(codeline, d);
		if (operr == operrOK) {
			codeline.AddCode(0xED, 0x64, d);
			codeline.SetCycles(3);
			return true;
		}
		return msg.Error(errorWrongOperand2, codeline);;
	}
	// all checked, maybe shortcut form ADD A / ADD B etc
	// error if there is a comma after next operand
	// restore initial position
	codeline.curtoken = curtoken;// normaly not needed?

	// TST g
	operr = optools.GetReg8(codeline, src,REGFLAGS::ABCDEHL);
	if (operr == operrOK) {
		codeline.AddCode(0xED, 0x04 + (optools.GetSubCode(src) << 3));
		codeline.SetCycles(3);
		return true;
	}
	// TST (HL)
	operr = optools.GetIndHL(codeline);
	if (operr == operrOK) {
		codeline.AddCode(0xED, 0x34);
		codeline.SetCycles(4);
		return true;
	}
	// TST (C),n
	operr = optools.GetIndC(codeline);
	if (operr == operrOK) {
		operr = optools.GetNum8(codeline, d);
		if (operr == operrOK) {
			codeline.AddCode(0xED, 0x74, d);
			codeline.SetCycles(4);
			return true;
		}
		return msg.Error(errorWrongOperand2, codeline);;
	}
	// TST n
	operr = optools.GetNum8(codeline, d);
	if (operr == operrOK) {
		codeline.AddCode(0xED, 0x64, d);
		codeline.SetCycles(3);
		return true;
	}
	return msg.Error(errorWrongOperand1, codeline);
}

/** Assemble instruction at current token, returns false if error
 tok index 0   1   2   3
 TSTIO   m                    ED 74 m

 @param codeline the code line in which assembled codes will be stored
 @param msg the message list which will receive any warning or error information
 */
bool InstructionTSTIO::Assemble(CodeLine& codeline, ErrorList& msg)
{
	int d;
	// save current position
	OperandError operr = optools.GetNum8(codeline, d);
	if (operr == operrOK) {
		codeline.AddCode(0xED, 0x74, d);
		codeline.SetCycles(4);
		return true;
	}
	return msg.Error(errorWrongOperand1, codeline);;
}

} // namespace Z180
} // namespace MUZ

