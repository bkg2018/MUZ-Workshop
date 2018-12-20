//
//  Z80-Instructions.h - Derived instruction classes for the Z-80.
//  MUZ-Workshop
//
//  Created by Francis Pierot on 02/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

#ifndef Z80_Instructions_h
#define Z80_Instructions_h

#include "Instruction.h"

namespace MUZ {

/** instruction names will lead to singleton instances of theese derived classes. */
	class InstructionLD : public Instruction {
	};
		
	class InstructionPUSH : public Instruction {
	};
	
	class InstructionPOP : public Instruction {
	};
	
	class InstructionEXX : public Instruction {
	};
	
	class InstructionEX : public Instruction {
	};
	
	class InstructionLDI : public Instruction {
	};
	
	class InstructionLDIR : public Instruction {
	};
	
	class InstructionLDD : public Instruction {
	};
	
	class InstructionLDDR : public Instruction {
	};
	
	class InstructionCPI : public Instruction {
	};
	
	class InstructionCPIR : public Instruction {
	};
	
	class InstructionCPD : public Instruction {
	};
	
	class InstructionCPDR : public Instruction {
	};
	
	class InstructionADD : public Instruction {
	};
	
	class InstructionADC : public Instruction {
	};
	
	class InstructionSUB : public Instruction {
	};
	
	class InstructionSBC : public Instruction {
	};
	
	class InstructionAND : public Instruction {
	};
	
	class InstructionOR : public Instruction {
	};
	
	class InstructionXOR : public Instruction {
	};
	
	class InstructionCP : public Instruction {
	};
	
	class InstructionINC : public Instruction {
	};
	
	class InstructionDEC : public Instruction {
	};
	
	class InstructionDAA : public Instruction {
	};
	
	class InstructionCPL : public Instruction {
	};
	
	class InstructionNEG : public Instruction {
	};
	
	class InstructionCCF : public Instruction {
	};
	
	class InstructionSCF : public Instruction {
	};
	
	class InstructionNOP : public Instruction {
	};
	
	class InstructionHALT : public Instruction {
	};
	
	class InstructionDI : public Instruction {
	};
	
	class InstructionEI : public Instruction {
	};
	
	class InstructionIM : public Instruction {
	};
		
	class InstructionRLCA : public Instruction {
	};
	
	class InstructionRLA : public Instruction {
	};
	
	class InstructionRRCA : public Instruction {
	};
	
	class InstructionRRA : public Instruction {
	};
	
	class InstructionRLC : public Instruction {
	};
	
	class InstructionRL : public Instruction {
	};
	
	class InstructionRRC : public Instruction {
	};
	
	class InstructionRR : public Instruction {
	};
	
	class InstructionSLA : public Instruction {
	};
	
	class InstructionSRA : public Instruction {
	};
	
	class InstructionSRL : public Instruction {
	};
	
	class InstructionRLD : public Instruction {
	};
	
	class InstructionRRD : public Instruction {
	};
	
	class InstructionBIT : public Instruction {
	};
	
	class InstructionSET : public Instruction {
	};
	
	class InstructionRES : public Instruction {
	};
	
	class InstructionJP : public Instruction {
	};
	
	class InstructionJR : public Instruction {
	};
	
	class InstructionDJNZ : public Instruction {
	};
	
	class InstructionCALL : public Instruction {
	};
	
	class InstructionRET : public Instruction {
	};
	
	class InstructionRETI : public Instruction {
	};
	
	class InstructionRETN : public Instruction {
	};
	
	class InstructionRST : public Instruction {
	};
	
	class InstructionIN : public Instruction {
	};
	
	class InstructionINI : public Instruction {
	};
	
	class InstructionINR : public Instruction {
	};
	
	class InstructionIND : public Instruction {
	};
	
	class InstructionINDR : public Instruction {
	};
	
	class InstructionOUT : public Instruction {
	};
	
	class InstructionOUTI : public Instruction {
	};
	
	class InstructionOTIR : public Instruction {
	};
	
	class InstructionOUTD : public Instruction {
	};
	
	class InstructionOTDR : public Instruction {
	};
	
} // namespace MUZ
#endif /* Z80_Instructions_h */
