//
//  Instructions.asm
//  MUZ-Workshop
//
//  Created by Francis Pierot on 24/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

// Should be assembled ok
ADC A, A
ADC A, B
ADC A, C
ADC A, D
ADC A, E
ADC A, H
ADC A, L
ADC A, 0x55
ADC A, (HL)
ADC A, (IX+0xAA)
ADC A, (IY+0xAA)
ADC HL, BC
ADC HL, DE
ADC HL, HL
ADC HL, SP

// should fail
ADC B, C
ADC A
ADC HL, (HL)
ADC A, HL
ADC HL, IX
ADC A, 0x100

