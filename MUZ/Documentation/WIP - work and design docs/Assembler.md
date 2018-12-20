#  MUZ Z-80 Assembler

# ** WORK IN PROGRESS **

**Syntax is not final**

Francis Piérot, 2018 December 1st.

MUZ assembler is a Z-80 assembler programmed in standard C++. It is focused on assembling files for the RC2014 pageable ROM, and does not feature macros or bank-switching mechanisms. It manages one $0000-$FFFF address space. It will track the portions in that space  accessed during assembly, and will only generate code for these portions in the corresponding HEX file, thus allowing different HEX files to fill an adress range without mutally overwriting.

MUZ assembler takes a main file and assembles it into a single HEX file. Include files are supported. The assembler issues warnings in case of memory space overwriting, but the output is still generated using the last assembled code.

The syntax of MUZ assembler has been made compatible with Steve Cousins's SC Monitor Workshop. The same assember source files should generate the same Hex and listing files.

## Assembler Preprocessing

The preprocessing takes place before assembling and uses a separate name space from the assembler. Preprocessing directives are useful for configuration, to adapt the generated code to a specific computer setting. (e.g. RAM size, ROM starting address, output ports for a given device.)

The general use is to define symbols depending on your configuration and, in source files, enable or disable portions of ccode depending on the symbol value, or include external assembler source files depending on the symbol values.

Remark: the symbols created by `#define` have no numeric value and are made to be used as strings in preprocessor directives like `#IF`, `#IFDEF` or `#INCLUDE`. They merely replace a string (the symbol name) by another (its string value) and as such, can be used in assembler code or directives. If their content represent a number, they can also be used in expressions. However, if an assembler symbol or label is defined with the same name, it will have precedence and hide the preprocessor symbol. The Assembler will issue a warning when this happens.

|Syntax|Example|Details|
|---|---|---|
|`#DEFINE <defsymbol>`|`#DEFINE RC2014_Z80`|Defines `defsymbol` for the rest of assembling. A defsymbol has no value, it's just defined or not. It is not defined in lines before the `#define` directive.|
|`#UNDEF <defsymbol>`|`#UNDEF RC2014_Z80`|Undefines `defsymbol` for the rest of assembling.|
|`#IFDEF <defsymbole>`|`#IFDEF IncludeFDOS`|if the `defsymbol` is defined, the lines following the `#IF` will be assembled, until an `#ELSE` or `#ENDIF` is meet. When an `#ELSE` is found, the following lines are ignored until an `#ENDIF` is found. If it is not defined, the assembler jumps over following lines until it finds an `#ELSE` or `#ENDIF`|
|`#IF <expression>`|`#IF BUILD = "W*"`|Evaluates the expression and if the result is not null, the lines following the `#IF` will be assembled, until an `#ELSE` or `#ENDIF` is meet. When an `#ELSE` is found, the following lines are ignored until an `#ENDIF` is found. If the result is null, the assembler jumps over following lines until it finds an `#ELSE` or `#ENDIF`|
|`#ELSE`||Defines the lines which will be assembled if the condition is false or null in the previous `#IF` or if the symbol is not defined in the previous `#ifdef`directive.|
|`#ENDIF`  ||Ends an `#IF/#ELSE` sequence.|
|`#INCLUDE`|`#INCLUDE    RomFS.asm`  `#INCLUDE    Hardware\Workshop\!Manager.asm`|Includes another source file. The current defsymbols, labels, address are all active during the assembly of the included file. Absolute and relative pathes can be used. Both Windows `\`and UNIX `/` are path separators. Multiples separators are considered as one: `///` and `\\\`are the same as `/`.|
|`#INSERTHEX <hexfile>`||Inserts an Intel Hex File in code. Each Intel HEX record has its own fixed address and will update the current assembling address counter. MUZ Assembler only processes the type-0 (16-bit address and up to 255 bytes) and type-1 (end of file) records. Overwriting is not checked.|

### Preprocessor <`defsymbol`>

The preprocessor symbols can use:

* letters `A` to `Z`, `a` to `z`
* digits `0` to `9`
* the underscore `_`

Notice in particular that white space, tabulations, accentuated characters,  `:`or arithmetic operators `+`  `-`  `*`  `/`are forbidden.

### Preprocessor Expressions

The `#if`directive can compare if a `defsymbol` is identical to a character strings expression with the syntax `#IF defsymbol = <stringexpression>`.


|Syntax|Example|Details|
|---|---|---|
|`<digit>
|`"<character>[[<character>]...]"`| `#define BUILD "R0"`| A character string is a sequence of characters between double quotes. Double quotes can not be stored in symbol values.|
|`<stringexpression> [[+ <stringexpression>] ...]`|`#define BUILDDIR "CONFIG_" + BUILD`|String expressions can be concatenated with the `+`operator. The symbols already defined can be used together with string constants.|


The `*` character is a joker which ends the comparison. If the symbol characters were identical to the string expression until this joker, the result is true.

Examples:

* `#IF BUILD = "T*"` is true if the defsymbol `BUILD` has been defined as `T1`  or `T4` but not if it is `R0`.
* `#IF BUILD = "T" + "*"` is identical to the previous test above.
* `#IF BUILD = "R0"` is true if the defsymbol `BUILD` has been defined as `R0`. 


## Assembler Directives

The assembler has directives which are not générating CPU instructions but rather control the assembling process itself. 

The directives always start with a dot `.`character, except for the comment `;` directive.

|Syntax|Example|Details|
|---|---|---|
|`;<comment>`  ||Everything following the `;` on the line is ignored by the assembler|
|`.ORG <expression>` | `.ORG 0x2000`   `.ORG CodeORG`|Changes the current address for assembly. Whatever was assembled before, the `.ORG` directive starts a new assembly sequence at a new address. This can be used to generate different portions of code and data in the same assembly HEX file at different memory ranges.
|`<symbole>: .EQU <expression>` |`kData: .EQU 0xFC00` `LF .EQU 0AH`|Defines a symbol with a given numeric or string value. The symbol will be replaced by its value during further assembling.|
|`.DB <expression> [[, <expression>] ...]` or `.BYTE <expression> [[, <expression>] ...]`|`szStartup: .DB "Custom",kNull` `iHwFlags: .DB 0x00`|Insert a sequence of 8-Bit numbers in the assembled code. This is generally preceeded by a label or other DB lines. Strings can be used and will generate one byte for each character.| 
|`.DW <expression> [[, <expression>] ...]` or `.WORD <expression> [[, <expression>] ...]`|`.DW 0xAA55`  `.DW EndOfMonitor-StartOfMonitor`|Insert a sequence of 16-Bit numbers in the assembled code. This is generally preceeded by a label or other DW lines. Strings can be used and will generate a zero followed by the byte value for each character.|   
|`.PROC <processorcode>`|`.PROC Z80`|Defines the processor instruction set to use. Ignored for now, should be defined as `Z80` for SCWorkshop compatibility.|
|`.HEXBYTES <expression>`||Ignored|

## Expressions

Expressions can mix symbols defined with EQU, labels, decimal or hexadecimal constants, characters, character strings and the special '$' character.

### EQU symbols

Symbols defined with an `.EQU` directive are replaced by the result of the expression in their `.EQU` directive. Like any symbol, the `.EQU` symbols do not exist before they appear in `.EQU` directive so they cannot be used before this directive has been assembled.

Example:

In the following lines, the symbol `WRKSPC`is defined with the value being the value defined by another symbol, `kWRKSPC`. The next line defines `USR`as being the sum of `WRKSPC`and 3, which in this context means that the `USR`symbol represents the address 3 bytes after the address represented by `WRKSPC`. 

    WRKSPC      .EQU    kWRKSPC     ; BASIC Work space    <SCC> original 2045H  
    USR         .EQU    WRKSPC+3H   ; "USR (x)" jump


### Labels

A label may appear first on a line of code, followed by a  `:`. It takes the value of the assembling address when this code is assembled. After that, it can be used in expressions. An error is generated is a label is used in an expression before it has been assembled. Instead of code, a label can be followed by an `.EQU` directive to define its value.

Notice that a label preceeding the first  `.ORG` can only be defined with `.EQU`, as there is no current assembling address before the first `.ORG`.

### Characters

|Syntax|Example|Details|
|---|---|---|
|`'<character>'`
|`"<string>"`

### Hexadecimal constants

|Syntax|Example|Details|
|---|---|---|
|`<hexdigit>[[<hexdigit>] ...]H`  |`0000H`  `80H`
|`$<hexdigit>[[<hexdigit>] ...]` |`$0000` `$A0`
|`0x<hexdigit>[[<hexdigit>] ...]`|`0xFFFF`  `0x4A00`

### Decimal constants

Notice that decimal constants must be positive. You can compute negative values with expressions but be sure to check the actual value, as converting a positive number to a negative is not simply setting the sign bit. For example `+127` is `01111111`, while `-127` is `10000001`.

|Syntax|Example|Details|
|---|---|---|
|`<decdigit>[[<decdigit>] ...]`|32768|

### Current assembling address

The `$` special symbol can be used as a prefix for hexadecimal characters. If no hexadecimal number is following it, it is considered as a symbol which values is the current assembling address. You can use this in expressions, or in assembler code to let the program knows its current address. for example by doing a `LD HL,$`: this will be converted to a `LD HL,$xxxx` instruction at assembly time, with `$xxxx` being the actual aseembly address of this very instruction.

|Syntax|Example|Details|
|---|---|---|
|`$`

### Operators

The arithmetic operators can be used in numeric expressions. In String expressions, the  `+` operator does concatenation.

|Syntax|Example|Details|
|---|---|---|
|`<expression>+<expression>`
|`<expression>-<expression>`
|`<expression>*<expression>`
|`<expression>/<expression>`

