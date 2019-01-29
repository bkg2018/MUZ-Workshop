# Small Computer Workshop Assembler Differences


Here is a summary of the differences between MUZ-Assembler and the Assembler in Small Computer Workshop.

## Directives

The `.DATA` and `.CODE` directives accept names to create more than one section of each kind.

By default, data sections are not saved in HEX output files: this can be changed using the `,SAVE` attribute in `.DATA` directive.

Most `.`dotted directives have synonyms without the dot.

Directive names cannot be used as labels.

## Instructions

Instruction names cannot be used as labels.

The `SLL` undocumented instruction is valid, as well as `IN (C),0`, `SET` and `RES` through registers and undocumented registers `IXH`, `IXL`, `IYH` and `IYL`.

## Expressions

In almost all the places where a number or a string is awaited, MUZ-Assembler accepts expressions with operators and parenthesis, including outmost parenthesis for indirect addressing.

**Operations:**

A string expression can concatenate two strings, or *add a number to each character of a string*.

A boolean expression can use other boolean operands with logical operators, or compare strings and numbers sub expressions.

A numerical expression can use binary, octal, decimal or hexadecimal numbers with arithmetical and binary operators.

**Comparisons**

Equality and Inequality between strings can use the `*` character as a joker ending comparison. If the strings match until this character is met, there are considered equal. *This joker can be disabled using the `\` backslah escape character*.

**Expressions in Directives**

Expressions are valid in `#DEFINE` values on the right of the symbol, as well as in `#IF` family directives, provided the expression can be evaluated at the time the `#DEFINE` is assembled. It implies that the expression do not used undefined symbols.

Undefined symbols do not stop assembly, they issue a warning and are treated as boolean false, empty string or 0 number depending on the context.

Please see [Assembler.md]() for more details about string, integer or  boolean constants and expressions.


## Labels

Labels can use any number of characters. The ending semi-colon is optional.

The characters which can be identified as operators or separators should be avoided: colon, semi colon, comma, parenthesis, dash etc.

A label cannot use the name of a directive or instruction: it would be identified as such and not as a label.

A label must not start with a digit.

Avoid single and double quotes in label names as they are used to surround strings and characters.

A label starting with the `@` character is local to the code between the last global label and the next global label.

A label alone on its line will take the current assembling address of the current section, unless the next assembled line is an .ORG directive which will reprogramm the label address.

## Listing

### Tables

MUZ-Assembler ends the listing with some tables.

**Sections**: sections are listed with their address ranges. The ranges only contain addresses written by code or .DB/.DW directives or reserved with directive .DS. The HEX output file will also only contain the same ranges.

    Sections:
    ------------------------------------------------------------------------------------
	DATA: [FE00-FEB0]
	CODE: [0000-1E7F] [2000-3E21] [3F80-3FFF]

**Defined symbols**: Every sybmol defined with the `#DEFINE` directive during assembly is listed. If the symbol has a value it is listed as well. The symbols are sorted alphabetically with upper cases before lower cases.

    Defines:
    ------------------------------------------------------------------------------------
	BUILD                          :R0
	IncludeAPI                     :
	IncludeAssembler               :
	...


**Equates symbols**: Every symbol defined with the `.EQU` directive during assembly is listed with its value. The symbols are sorted alphabetically with upper cases before lower cases.

    Equates:
    ------------------------------------------------------------------------------------
	BasicCodeW                     :2003
	CmdReset                       :0000
	Reset                          :0000
	...
	
**Global Labels**: Every global label created during assembly is listed with its address. two columns are listed : the left column has the symbols sorted alphabetically with upper cases first, the right colum has the symbols sorted by address. Some addresses are targetted by more than one symbol: if it happens, the right column only shows the last symbol and the right column will be shorter..

    Global labels:
    --- By Name -------------------------------|---By Address --------------------------
	    APIHandler                     :04FD   |   0000: ColdStart
	    APITable                       :050F   |   0003: WarmStart
	    AsmSkipOverDelimiter           :0CE4   |   0014: WStrt
	    AsmSkipToDelimiter             :0CF4   |   0040: kaConDev
	    ...


### Inclusions

When a #INCLUDE, #INSERTBIN or #INSERTHEX includes another source file, the listing show the path of the oncluded file, and lists back the current file when inclusion is finished.

SCWorkshop :

                        0198  ; Core OS functions
                        0199  #INCLUDE    Alpha.asm           ;This must be the first #include
                        0001  ; **********************************************************************
                        0002  ; **  Alpha module                              by Stephen C Cousins  **
                        ...
                        0754  
                        0755  
                        0200  #INCLUDE    Console.asm         ;Console support


MUZ-Assembler:

                        0198  ; Core OS functions
                        0199  #INCLUDE    Alpha.asm             ;This must be the first #include

                        /Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/Alpha.asm

                        0001  ; **********************************************************************
                        0002  ; **  Alpha module                              by Stephen C Cousins  **
                        ...
                        0755  
                        0756  

                        /Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm
                        
                        0200  #INCLUDE    Console.asm           ;Console support



### Equates

The value of labels defined by EQU is put at the beginning of listing instead of current address.

SCWorkshop:

    4000:               0056  kStrBuffer:
                        0056  kStrBuffer: .EQU kData+0x0180   ;String buffer              (to +0x01FF)
    0080:               0057  kStrSize:   .EQU 128            ;Size of string buffer

MUZ-Assembler:

    FD80:               0056  kStrBuffer: .EQU kData+0x0180     ;String buffer              (to +0x01FF)
    0080:               0057  kStrSize:   .EQU 128              ;Size of string buffer

### Labels longs

Labels appear on only one line.

 SCWorkshop: 

    08EC:               0077  szAsmError:
    08EC: 53 79 6E 74   0077  szAsmError: .DB  "Syntax error",kNewLine,kNull
    08F0: 61 78 ...     3214  
    
MUZ-Assembler:    

    08EC: 53 79 6E 74   0077  szAsmError: .DB  "Syntax error",kNewLine,kNull
    08F0: 61 78 20 ...        

### Séquences DB

Code sequences with more than 4 bytes only show the source line number on first line. 

SCWorkshop :

    1859: 88 F8 31 23   0133  	.DB  0x88,0xF8,0x31,0x23,0x07 ;Opcode: 0x88 - ADC  A   ,r2  
    185D: 07            6090  
    185E: CE FF 31 23   0134  	.DB  0xCE,0xFF,0x31,0x23,0x02 ;Opcode: 0xCE - ADC  A   ,n   
    1862: 02            6091  

MUZ-Assembler :

    1859: 88 F8 31 23   0133  	.DB  0x88,0xF8,0x31,0x23,0x07    ;Opcode: 0x88 - ADC  A   ,r2  
    185D: 07                  
    185E: CE FF 31 23   0134  	.DB  0xCE,0xFF,0x31,0x23,0x02    ;Opcode: 0xCE - ADC  A   ,n   
    1862: 02                  

The listing can be set to display all bytes of the code sequence instead of the default mode which displays up to 7 bytes with an ellipsis "...".

