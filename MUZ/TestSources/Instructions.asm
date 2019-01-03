BIT 7,(HL)
BIT 0,B
BIT 0,C
BIT 0,D
BIT 0,E
BIT 0,H
BIT 0,L
BIT 0,(HL)
BIT 0,A
BIT 7,B
BIT 7,C
BIT 7,D
BIT 7,E
BIT 7,H
BIT 7,L
;
BIT 7,A

LD (DE),A
LD (BC),A
LD (HL),A

.ORG $2000
JP NotSoFar
.DS 0x100
NotSoFar: .DB 0


kWrongHex: .EQU 0x0B

kNull: .EQU 0
kNewLine: .EQU 13
szDevices:  .DB  "Devices detected:",kNewLine,kNull

 kVersMajor: .EQU 1
.ORG 2000h
.DB  '0'+kVersMajor,'.'

.DB  '0','.'
store: .DB 1,2,3,4
LD (store), HL

kData		.EQU 1
kSPUsr:     .EQU kData+0x00C0
kInputSize: .EQU 128
;0000:               0052  kSPUsr:     .EQU kData+0x00C0   ;Top of stack for user program
;0000:               0053  kSPSys:     .EQU kData+0x0100   ;Top of stack for system
;0000:               0054  kInputBuff: .EQU kData+0x0100   ;Line input buffer start    (to +0x017F);
;0000:               0055  kInputSize: .EQU 128            ;Size of input buffer
;0000:               0056  kStrBuffer: .EQU kData+0x0180   ;String buffer              (to +0x01FF)
;0000:               0057  kStrSize:   .EQU 128            ;Size of string buffer
;0000:               0058  kJumpTab:   .EQU kData+0x0200   ;Redirection jump table     (to +0x025F)
 .ORG $2000
Start: .DB 1
End: .DB 2
LD HL,(End-Start)

.ORG $2000
Start:		.DS 16
		JR Start


LD (HL),A    ;      77            0045              LD   (HL),A
LD A,E       ;00C4: 7B            0032              LD   A,E            ;First byte to write to LEDs = 0x01
LD HL,-8000  ;00C7: 21 C0 E1      0034              LD   HL,-8000       ;Set delay time


//
//  Instructions.asm
//  MUZ-Workshop
//
//  Created by Francis Pierot on 24/12/2018.
//  Copyright © 2018 Francis Pierot. All rights reserved.
//

// ADC ok
ADC A, B
ADC A, C
ADC A, D
ADC A, E
ADC A, H
ADC A, L
ADC A, (HL)
ADC A, A
ADC A, 0x55
ADC A, (IX+0xAA)
ADC A, (IY+0xAA)
ADC HL, BC
ADC HL, DE
ADC HL, HL
ADC HL, SP
// ADC fail
ADC B, C
ADC A
ADC HL, (HL)
ADC A, HL
ADC HL, IX
ADC A, 0x100
ADC IX,IY
ADC HL,IX
// ADD ok
ADD A, B
ADD A, C
ADD A, D
ADD A, E
ADD A, H
ADD A, L
ADD A, (HL)
ADD A, A
ADD A, 0x55
ADD A, (IX+0xAA)
ADD A, (IY+0xAA)
ADD HL, BC
ADD HL, DE
ADD HL, HL
ADD HL, SP
ADD IX, BC
ADD IX, DE
ADD IX, IX
ADD IX, SP
ADD IY, BC
ADD IY, DE
ADD IY, IY
ADD IY, SP
// ADD fail
ADD B, C
ADD A
ADD HL, (HL)
ADD A, HL
ADD HL, IX
ADD A, 0x100
ADD IX,IY
ADD HL,IX
// AND ok

AND B
AND C
AND D
AND E
AND H
AND L
AND (HL)
AND A
AND 0x55
AND (IX+0xAA)
AND (IY+0xAA)

// not extensive
BIT 0,B
BIT 0,C
BIT 0,D
BIT 0,E
BIT 0,H
BIT 0,L
BIT 0,(HL)
BIT 0,A
BIT 0,(IX+0xAA)
BIT 0,(IY+0xAA)
BIT 7,B
BIT 7,C
BIT 7,D
BIT 7,E
BIT 7,H
BIT 7,L
BIT 7,(HL)
BIT 7,A
BIT 7,(IX+0xAA)
BIT 7,(IY+0xAA)

CALL 0x1234
CALL NC, 0x1234
CALL C, 0x1234
CALL NZ, 0x1234
CALL Z, 0x1234
CALL PO, 0x1234
CALL PE, 0x1234
CALL M, 0x1234
CALL P, 0x1234

CP B
CP C
CP D
CP E
CP H
CP L
CP (HL)
CP A
CP 0x55
CP (IX+0xAA)
CP (IY+0xAA)

CPD
CPDR
CPI
CPIR

CPL
DAA

DEC B
DEC C
DEC D
DEC E
DEC H
DEC L
DEC (HL)
DEC A
DEC (IX+0xAA)
DEC (IY+0xAA)
DEC BC
DEC DE
DEC HL
DEC SP
DEC IX
DEC IY

DI

DJNZ 0x1234

EI
