;
;  Expressions.asm
;  MUZ-Workshop
;  Tests string, boolean and numeric exxpressions
;
;  Created by Francis Pierot on 16/12/2018.
;  Copyright © 2018 Francis Pierot. All rights reserved.
;
;kDelete:    .EQU 127            ;Delete character (0x7F)
;kData:      .EQU 0xFC00         ;Typically 0xFC00 (to 0xFFFF)
;kPeriod:    .EQU 46             ;Period character (0x2E)                  -> 002E:
kColon:     .EQU 58             ;Colon character (0x3A)                   -> 0005:
kSemicolon: .EQU 59             ;Semicolon character (0x3B)               -> 0005:
kDelete:    .EQU 127            ;Delete character (0x7F)#define BUILD R0  -> 007F:
#DEFINE MACHINE "RC2014"
#define VERSION 0
#if MACHINE + "-" + VERSION = "RC2014-0"
#define ASSEMBLED
#endif
#if MACHINE + VERSION = "R*"
#define ASSEMBLED
#endif

;IF expression tests
#if ! ( 3 * ( 1 + 2 * (3 + 4)))
#define ASSEMBLED
#endif
