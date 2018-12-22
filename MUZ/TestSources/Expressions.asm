;
;  Expressions.asm
;  MUZ-Workshop
;  Tests string, boolean and numeric exxpressions
;
;  Created by Francis Pierot on 16/12/2018.
;  Copyright © 2018 Francis Pierot. All rights reserved.
;
#define BUILD R0
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
