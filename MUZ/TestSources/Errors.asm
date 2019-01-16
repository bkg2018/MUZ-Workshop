This source contains mistakes to check the error/warning system

#define
#define 456(abc
#define wrong,label
#define wrong&label

equate1: equ 100000
equate2: equ -100000

JP undefinedlabel

JR toofarlabel
.DS 256
toofarlabel:

LD wrongregister, $0
LD A,wrongregister
LD BC,HL
PUSH A
POP F
ADD HL,(IX+1)
ADD A,(HL+1)
ADD A,DE

