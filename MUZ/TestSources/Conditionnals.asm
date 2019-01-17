
;  Conditionnals.asm
;  MUZ-Workshop
;  Tests the IF/ELSE/ENDIF behaviours
;
;  Created by Francis Pierot on 07/12/2018.
;  Copyright © 2018 Francis Pierot. All rights reserved.
;
#IFDEF DEFINED
Label1: .ORG 0x0000 ; no
#ELSE
Label2: .ORG 0x1000 ; yes
#ENDIF

#LIST OFF

#IFNDEF NOT_DEFINED
Label3: .ORG 0x2000 ; yes
#ELSE
Label4: .ORG 0x3000 ; no
#ENDIF

; define some symbols
#DEFINE BUILD R0
#DEFINE DEFINED_EMPTY

#LIST ON

; escaped joker
#if BUILD = "R\*"
ok equ 1 ; no
#else
ok equ 0 ; yes
#endif

; normal joker
#if BUILD = "R*"
ok equ 1 ; yes
#else
ok equ 0 ; no
#endif

#IF DEFINED_EMPTY ; ->DOTOELSE
ok equ 1 ; yes
#ELSE ; -> SKIPTOEND
ok equ 0 ; no
#ELSE ; WARNING ignored
ok equ -1 ; no
#ENDIF

#IF NOT_DEFINED ; ->SKIPTOELSE
ok equ 2 ; no
#ELSE ; -> DOTOEND
ok equ 3 ; yes
#ELSE ; WARNING
ok equ 4
#ENDIF

; level 2
#IFDEF DEFINED_EMPTY ; -> DOTOELSE

	#IFDEF DEFINED_TOO
	Label5: .ORG 0x4000 ; yes
	#ELSE
	Label6: .ORG 0x5000 ; no
	#ENDIF

	#IFDEF UNDEFINED
	Label5: .ORG 0x6000 ; no
	#ELSE
	Label6: .ORG 0x7000 ; yes
	#ENDIF

#ELSE ; ->SKIPTOEND

	Label7: .ORG 0x8000; no

#ENDIF ;-> level 1

; level 2
#IFDEF NOTDEFINED ; -> SKIPTOELSE

Label8: .ORG 0x9000; no

#ELSE ; ->DOTOEND

#IFDEF DEFINED_TOO
	Label9: .ORG 0xa000 ; yes
#ELSE
	Label10: .ORG 0xb000 ; no
#ENDIF

#IFDEF UNDEFINED
	Label1: .ORG 0xc000 ; no
#ELSE
	Label12: .ORG 0xd000 ; yes
#ENDIF

#ENDIF ;-> level 1
