;
;  Expressions.asm
;  MUZ-Workshop
;  Tests string, boolean and numeric exxpressions
;
;  Created by Francis Pierot on 16/12/2018.
;  Copyright © 2018 Francis Pierot. All rights reserved.
;

#DEFINE MACHINE "RC2014"
#define VERSION 0
#if MACHINE + VERSION = "R0"
#define ASSEMBLED
#endif
#if MACHINE + VERSION = "R*"
#define ASSEMBLED
#endif

