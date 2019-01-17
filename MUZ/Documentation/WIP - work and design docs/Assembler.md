#  MUZ-Assembler for Z-80

### WORK IN PROGRESS

Francis Piérot, 2018 December 1st.

Updated 2019 January 4th.

MUZ-assembler is a Z-80 assembler programmed in standard C++ 11. It is designed to assemble files for the **RC2014** computer and has been programmed to be compatible with [*Steve Cousins's SC Monitor Workshop*](https://smallcomputercentral.wordpress.com/projects/small-computer-workshop), which is the reference development environment for [Small Computer Monitor](https://smallcomputercentral.wordpress.com/projects/small-computer-monitor), the hearth of the RC2014 computer ROM. MUZ-Assembler will compile the SCMonitor source files on an Apple Mac computer into the same Intel HEX format files as SCWorkshop on Windows.

MUZ-Assembler is also designed to be part of a RC2014 development environnement including a source editor, a debugger and a computer emulator for Apple Mac computers, similar to Steve Cousin's Workshop for Windows. (This is work in progress.)

The main features of MUZ assembler are:

* **64KB Address Space**: MUZ Assembler manages one $0000-$FFFF address space
* **Z-80 Processor**: MUZ Assembler assembles all documented Z-80 instructions with official timings, but also undocumented instructions and registers addressing modes
* **Code and Data Sections**: the address space can be split into separate code and data sections, each section having a current address. Multiple named sections can be created for both code or data, and sections can have separate address ranges. Data sections can optionally be put in hex and binary files
* **Two Passes Assembly**: the assembler does a first pass to determine the address and code size of each assembled line and label, then a second pass is done to generate the actual code using the first pass results. It allows instructions to use labels which will be defined later in the source files
* **HEX, Listing and Binary Output**: MUZ-assembler takes a main file and assembles it into HEX or binary files
* **ASM, HEX and Binary Include**: the main file can include other ASM files, as well as Intel HEX Files and binary files. Included code or data is assembled at current address in current section
* **Widely Compatible Syntax**: The assembler is 100% compatible with Steve Cousin's Workshop assembler, and features directives and syntaxes from other vintage assemblers
* **Debugging features**: The assembler issues a lot of warnings for syntax or consistency problems in the source files, but also for address range or code section overwriting, improbable JR or DJNZ jumps, and other issues which could reveal programming errors. These warnings can be suppressed in the source files with directives
* **GUI oriented functions**: The Assembler C++ class features functionalities which are helpful for a GUI and/or debugger, such as returning the file, line, section, and possibly label for an address

The syntax of MUZ assembler has been made compatible with Steve Cousins's SC Monitor Workshop. The same assembler source files generate the same Hex and binary files. Listing files are very similar with minor differences. Unnamed Code and Data sections works identically.

***Syntax in This Document***

This documentation uses a simple syntax in source code samples and syntax descriptions.

* A pair of square brackets `[]` surrounds an optional element and is not part of the element.

* A pair of angle brackets `<>`surrounds an element name and is not part of the name.

* Eveything else must be typed as is.

As an example, the notation `[;[<comment>]]` means that a comment is optional (as showed by the outmost '[]' pair), but if you intend to put a comment in the source, you *must* type a `;`, optionaly followed by your `comment` text.

## Assembler Syntax

In this part we will define the syntax for the different parts of the source code : instructions and operands, numeric and string constants, expressions, labels.

The assembler expects one directive or instruction by line at most. Empty lines or full comment lines are ignored.

The general format for a source line is one of the following forms:

* `[label name[:]]  [<directive> [<directive operands>]] [;[<comment>]]`
* `[label name[:]]  [<instruction> [<instruction operands>]] [;[<comment>]]`

The directives can be preprocessor directives like `#INCLUDE` or `#DEFINE` (see above in **Preprocessor Directives**), or assembler directives like `.EQU` or `.DB`, which are descripted below in **Assembler Directives**. Instructions are Z-80 instruction mnemonics like `LD` or `CALL`. 

Directives and instructions names are not case sensitive (`ld` is the same as `LD`), but the labels are.

The assembler recognizes the following elements, which will be detailed in this documentation:

* Preprocessing directives
* Preprocessor symbols
* Labels
* Instructions
* Comments
* Registers
* Numeric constants
* Character constants
* String constants
* Expressions
* Assembler directives

There is also some contexts in which the assembler adopts a specific behaviour and the documentation will xxplain them.


### Preprocessing Directives


The preprocessing takes place before actual assembling and code generation. It acts on the files and lines of source code which will be assembled or not, and do not generate code by itself.

Preprocessing directives are useful for configuration, to adapt the assembled code to a specific computer setting like RAM size or ROM starting address. The symbols defined by the `#DEFINE` directives are replaced by their string or numeric value before assembling takes place, and can be tested in directives like `#IF` for specific values and decide if a portion of code will be assembled or not.

The preprocessor has been made compatible with *Steve Cousins Workshop* assembler and has a few extended features. All the preprocessor directives starts with a `#`character.

**Remark:** the symbols created by `#define` may have a value or just be defined. If they don't hold a value, they are considered as defined and can be used as an empty string or a true boolean value in expressions. All symbols are considered as true boolean values in boolean expressions.

|Syntax                               |Example                                             |Details|
|-------------------------------------|----------------------------------------------------|------------------------------------------------|
|`#DEFINE <defsymbol> [expression]`   |`#DEFINE TARGET RC2014`                             |Defines `defsymbol` for the rest of assembling. If this symbol has no particular value, it is still considered as true when used in `#IF`. The optional expression can be a string or numerical expression. (See Expressions later in this document.) |
|`#UNDEF <defsymbol>`                 |`#UNDEF TARGET`                                     |Undefines `defsymbol` for the rest of assembling.|
|`#IFDEF <defsymbol>`                 |`#IFDEF IncludeFDOS`                                |if the `defsymbol` is defined, the lines following the `#IF` will be assembled, until an `#ELSE` or `#ENDIF` is meet. When an `#ELSE` is found, the following lines are ignored until an `#ENDIF` is found. If it is not defined, the assembler jumps over following lines until it finds an `#ELSE` or `#ENDIF`|
|`#IF <expression>`                   |`#IF BUILD = "W*"`                                  |Evaluates the expression and if the result is true or not null, the lines following the `#IF` will be assembled, until an `#ELSE` or `#ENDIF` is meet. When an `#ELSE` is found, the following lines are ignored until an `#ENDIF` is found. If the result is null, the assembler jumps over following lines until it finds an `#ELSE` or `#ENDIF`.  (See **Expressions** later in this document.)|
|`#ELSE`                              |`#ELSE`                                             |Defines the lines which will be assembled if the condition is false or null in the previous `#IF` or if the symbol is not defined in the previous `#ifdef`directive.|
|`#ENDIF`                             |`#ENDIF`                                            |Ends an `#IF/#ELSE` sequence.|
|`#INCLUDE <filepath>`                |`#INCLUDE    Hardware\Workshop\!Manager.asm`        |Includes another assembler source file. The current defsymbols, labels, address and sections are all active during the assembly of the included file. Absolute and relative paths can be used. Both Windows `\` and UNIX `/` are path separators. Multiples separators are considered as one: `///` and `\\\` are the same as `/`. Relative paths are relative to the main source path. File names with no path are searched in the parent path first, and if not found, in the main source directory.|
|`#INSERTHEX <hexfile>`               |                                                    |Inserts an Intel Hex File in code. Each Intel HEX record has its own fixed address and will update the current assembling address counter. MUZ Assembler only processes the type-0 (16-bit address and up to 255 bytes) and type-1 (end of file) records. The file path follows the same rules as for `#INCLUDE` directive (see above.)|
|`#INSERTBIN <binfile>`               |                                                    |Inserts the content of a file byte by byte at current address of current section. The file path follows the same rules as for `#INCLUDE` directive (see above.)|
|`#NOLIST` ||Disables listing from now on, until `#LIST` is used. 
|`#LIST [ON|OFF]`||Enables or disables listing from now on. `#LIST` alone acts as `#LIST ON`.

**Remark: ** `COND` is a synonym to `#IF` and `ENDC` is a synonym of  `#ENDIF`. 

### Preprocessor Symbols <`defsymbol`>

The preprocessor symbols can use any character except space, tabulation, single quote and double quote. However for compatibility it's better to restrict to the following characters:

* letters `A` to `Z`, `a` to `z`
* digits `0` to `9`
* the underscore `_`

In particular, accentuated characters, `:` or arithmetic operators like `+`  `-`  `*`  `/` may have side effects and generate assembly bugs.

The symbols can be defined with no specific value.

Any string of numeric expression can also be used to set a symbol value, e.g. `#DEFINE CONFIG "R" + "4"`. See **Expressions** later in this document.

### Labels

A label may appear first on a line of code, followed by a  `:`. It takes the value of the assembling address when this code is assembled. It can be used in expressions and will be replaced by its address. Instead of code, a label can be followed by an `.EQU` directive to define its value.

There are two types of labels.

* *Global* labels starts with a character other than `@` and are optionaly followed by `:`, a directive, an instruction or a comment. The global labels are unique for the whole assembling process, so if you redefine a global label in a source file it will only remember the new address value. The Assembler will issue a warning if a global label is defined more than once.

* *Local* labels starts with a `@`. They are always relative to the *last* previous global label. Instructions trying to use a local label will not find it further than the next global label.

Local labels are useful to put targets in sub-routines or near code. For example, a label named `@Loop` can be used in every sub-routine starting with a global label, and each local `@Loop` will be unique to its sub-routine. Redefining a local label later is not forbidden but *only the last one will be remembered* for the final assembly. To use more than one local labels, a common practice is to add a numeric suffix, e.g. `@Loop1` and `@Loop2`. 

#### Label Syntax

A label can use any character except the single quote `'`, the double quote `"`, the normal instructions separators like space, tabulation, `,`, the operators like `-`, `+`, `*` or `|` and special characters  like `;`. By convention programmers tend to restrict to uppercase and lowercase alphabetical characters, digits and the underscore `_`.

Labels are case sensitives: an uppercase character and its corresponding lowercase are two different characters.

MUZ-Assembler will accept any UTF-8 character in label names. 

#### Default Label Target

A label preceding the first `.ORG` in a section will be assigned the value `0`. If no section is defined yet, the unnamed `CODE` section, with the current address `0` will be used for this label.

### Comments

Comments start with the `;`character. Everything from the `;` to the end of line is ignored by the assembler.

### Instructions

Instruction names are not case sensitive: `LD` and `ld` are accepted.

Instruction names cannot be used as labels and EQU symbols names.

MUZ-Assembler recognises documented and undocumented instructions like `SLL` and three operands `SET` and `RES` instructions.

### Registers

Register names are not case sensitive: `BC` and `bc` are accepted.

MUZ-Assembler recognises undocumented Z-80 registers like `IXH`.


### Numeric Constants

Positive integer numbers from 0 to 65535 can be used and combined in expressions using the following formats:

* decimal digits (0-9), eg: 32769
* binary digits (0-1) with the `0b` prefix, eg: `0b1010`
* binary digits (0-1) with the `b` suffix, eg: `1010b`
* octal digits (0-7) with the `0` prefix, eg: `0755`
* hexadecimal digits (0-F) with `0x` prefix, eg: `0x7FFF`
* hexadecimal digits (0-F) with `$` prefix, eg: `$7FFF`
* hexadecimal digits (0-F) with `h` suffix, eg: `7FFFh`

All numbers are masked out by the 0xFFFF value to fit in 16 bits. Expressions can exceed this limit during calculations, but the end result will be masked out to 16 bits. If a string is used where a number is expected, it will be converted to a numeric value, eg. `"0x7FFF"`will be converted to `32767`.

To use a negative number, put the unary '-' operator in front of the number. All numbers are positive in MUZ-Assembler, eg: `0xFFFF` will be `65535` and not `-1`.

### Character Constants

Characters can be defined between single quotes, eg. `'H'`. Although you can use any character between code 0 and code 255, it is common practice to restrict to ASCII codes between 32 (space) and 127, as other characters are always encoding dependent. If you use other characters, you must be sure that your program will run in an environment with a font that will interpret the characters in the same way you intend in your source assembler files.

A character is always a *single* character between two single quotes.

Special characters can be *escaped* with the `\` backslash character, in which case the characters following the backslash define another character. The table below explains each escape sequence.

| sequence | value                                                                                                               |
|----------|---------------------------------------------------------------------------------------------------------------------|
| \t       | ASCII HT horizontal tabulation (code 0x09)                                                                          |
| \n       | ASCII CR carriage return (code 0x0D)                                                                                |
| \r       | ASCII LF line feed (code 0x0A)                                                                                      |
| \\       | backslash                                                                                                           |
| \h       | ASCII BS backspace (code 0x08)                                                                                      |
| \'       | single quote (allow its use in character constant)                                                                  |
| \"       | double quote (allow its use in string constant)                                                                     |
| \0*nnn*  | character with the octal code *nnn* (NB: backslash is followed by a zero, not the letter o)                         |
| \x*hh*   | character with the hexadecimal code *hh*                                                                            |
| \*ddd*   | character with the decimal code *ddd*, which must not start with a zero (or it is considered an octal number)       |


**Remark**: Any `\` backslash character not conforming to the sequences defined in this table will be replaced by a `?` character.

### String Constants

Character strings can be entered between `"` double quote characters. The double quotes themselves are not part of the string. 

All the escape sequence for character constants can be used in strings. (See table above.)

Strings can be concatenated using the `+` operator, see **Expressions** below.

Symbols defined with `#DEFINE` can have string values and be used in expressions. 

**Remark**: Character strings with only one character are used as *character* constants.


### Expressions

Expressions may use classical C/C++ operators and any parenthesis levels.

Expressions can mix:

* symbols defined with EQU
* labels
* binary, octal, decimal or hexadecimal constants
* characters between `'` single quotes
* character strings between `"` double quotes
* the special `$` character for current address of current section

**Remark:** preprocessor symbols defined with `#DEFINE` are replaced by their numeric, string or boolean value before expression evaluation.

**Remark**: a single character surrounded by double quotes is considered as a character constant, which means the `+` operator will only concatenate if the second operand is a string. See the `+`operator in the table below.

Operators in expressions follow the hierarchy from the following table, similar to hierarchy from the C / C++ language. The top operator has the highest priority, which is 0.

|Operator |Priority<BR>(0 = highest priority)|number of arguments|Arguments type|result type|Remark and example
|---------|------------|---|---------|---------|-----------------------------------------------------|
|  !      | 0          |1  | number  | number  | binary NOT: `! 0xFFFF` computes `0`
|  !      | 0          |1  | boolean | boolean | boolean NOT, useable on expressions returning a boolean: `! (a && b)` is true if a or b is false.
|  *      | 1          | 2 | number  | number  | multiplication: `2 * 3` computes as `6`
|  /      | 1          | 2 | number  | number  | integer division only: `49 / 3` computes `16`
|  \      | 1          | 2 | number  | number  | integer division only: `49 \ 3`  computes `16`
|  %      | 1          | 2 | number  | number  | modulo: `255 % 8`  computes `7`
|  +      | 2          | 2 | number  | number  | addition: `2 + 5` computes `7`
|  +      | 2          | 2 | string and number  | number  | adds the number to each character code in the string or character: `'A' + 1` will be evaluated to `66`, and `"BC" + 2` will compute as `"DE"`. 
|  +      | 2          | 2 | string  | string  | string concatenation: `"R" + "4"` is `"R4"`
|  -      | 2          | 1 | number  | number  | unary sign change: `-2`
|  -      | 2          | 2 | number  | number  | subtraction: `256 - 0x80` computes as `128`
|  <<     | 3          | 2 | number  | number  | binary left shift: `1 << 4` is `16`
|  >>     | 3          | 2 | number  | number  | binary right shift: `256 >> 3` is `32`
|  <      | 4          | 2 | number  | boolean | less than: `1 < 2` is true
|  <      | 4          | 2 | string  | boolean | less than: `"PLANE" < "QUESTION"` is true. Lowercase characters are inferior to uppercase characters.
|  >      | 4          | 2 | number  | boolean | greater than: `256 > 128` is true
|  >      | 4          | 2 | string  | boolean | greater than: `"v" > "V"` is false (equality)
|  <=     | 4          | 2 | number  | boolean | less than or equal: `127 <= 0x7F` is true (equal)
|  <=     | 4          | 2 | string  | boolean | less than or equal: `"alpha" <= "beta"` is true
|  >=     | 4          | 2 | number  | boolean | greater than or equal: `0x200 >= 0x1FF` is true
|  >=     | 4          | 2 | string  | boolean | greater than or equal: ` "" >= ""` is true
|  !=  <> | 5          | 2 | number  | boolean | different than: `3 != 5` is true
|  !=  <> | 5          | 2 | string  | boolean | different than: ` "R0" != "R1"` is true
|  = ==   | 5          | 2 | number  | boolean | equal: `128 = 0x80` is true
|  = ==   | 5          | 2 | string  | boolean | equal: `"alpha" = "alpha"` is true
|  &      | 6          | 2 | number  | number  | binary AND: `0x5746 & 0x7FFF` is `22342` (which is 0x5746)
|  ^      | 7          | 2 | number  | number  | binary XOR:`0xAA ^ 0xFF` is `85` (which is 0x55)
|  \|     | 8          | 2 | number  | number  | binary OR: `0x44 | 0x8F` is `207` (which is 0xCF)
|  &&     | 9          | 2 | boolean | boolean | boolean AND: `(1 < 2) && (3 < 4)` is true
|  \|\|   | 10         | 2 | boolean | boolean | boolean OR: `('A'='a') || ('3'='3')` is true


### Assembler Directives

The assembler has directives which are not generating CPU instructions but rather impact the assembling process itself. A few directives also allow insertion of bytes or characters sequences at current address.

The assembler directives start with a dot `.` character, except for the comment `;` directive.

Before we see the syntax of all directives we will present code and data sections.

#### CODE and DATA Sections

The assembler allows the programmer to place parts of its source files at different places in the processor address space. On a Z-80, this simply cuts the 0000-FFFF address space in different parts, under the programmer control. There is no actual difference between a code and data section other than its current address and address range: it's mostly a programmer's choice. On more recent processors, sections types have an actual impact on what you can do with code or data, for example the program won't be allowed to write into a code section or to transfer control into a data section. 

Most of the time, a CODE and a DATA section are sufficient because 8-bit processors were thought as mono-task processors and don't need to share memory with other programs, except maybe the operating system or BASIC interpreter.

As their name imply, the CODE section will be used for the space where the program instructions resides, while the DATA section will be placed elsewhere where the program stores its internal data or the user data when he or she's using the program.

In MUZ-Assembler, CODE and DATA sections can have names, so the programmer can create more than one section of both. This can be used to separate program datas from user datas for example, or various parts of the program code itself.

By default, MUZ-Assembler is compatible with Small Computer Workshop and will have the "CODE" and "DATA" sections that you can switch using the `.CODE` and `.DATA` directives. Once you issue these directives, all the following instructions or directives will be addressing the relevant section current address.

The interesting point in having named sections is that MUZ-Assembler will list the address spaces used by each section at the end of assembly, and will issue warnings for overwriting cases.

**Remark:** if no section is used in source files, all the assembled code will be stored in a unique CODE section.

#### Output in Hex File

By default, MUZ-Assembler will write each CODE section in the HEX output file, but *not the DATA sections*.

This behaviour is compatible with Small Computer Workshop.

MUZ-Assembler accepts an optional `SAVE` attribute for DATA sections. When the `.DATA` directive is used for the first time to start a data section, it can be added `,SAVE` to tell the assembler that this data section must be saved in output HEX files just as the code sections. 

#### Directives

The assembler directives all start with a `'.'`character.

|Syntax|Example|Details|
|---|---|---|
|`;<comment>`  ||Ends the current source line with a comment. Everything including and following the `;` is ignored by the assembler|
|`.CODE [name]`|| Starts a new code section or switches to it. The name is optional. After this directive, all the instructions and directives will be applied to the section current address. Notice that the CODE section are mostly supposed to receive instruction code, but can also contain `.DB`-like storage directives: this is a programmer's choice. 
|`.DATA [name] [,SAVE]`|| Starts a new data section or switch to it. The name is optional. After this directive, all the instructions and directives will be applied to the section current address. Notice that the DATA section is supposed to receive temporary or work storage areas with `.DB`-like directives. The content of a data section is only saved in HEX output if the `,SAVE` attribute is added to the *first* `.DATA` directive for this section.
|`.ORG <expression>` | `.ORG 0x2000`   `.ORG CodeORG`|Changes the current address for assembly in the current section. The `.ORG` directive starts a new assembly sequence at a new address. This can be used to generate different portions of code and data at different memory address ranges.
|`<symbol>: .EQU <expression>` |`kData: .EQU 0xFC00` `LF .EQU 0AH`|Defines a label with a given numeric or string value. The symbol will be replaced by its value during further assembling. Notice that *the symbols defined with `.EQU` don't have an address* and are normally not interacting with the current code or data section. However, you can use the `$` special symbol to refer to the current section current address in the expression.|
|`[<label>:] .DB <expression> [[, <expression>] ...]` or `[<label>:] .BYTE <expression> [[, <expression>] ...]`|`szStartup: .DB "Custom",kNull` `iHwFlags: .DB 0x00`|Inserts a sequence of 8-Bit numbers in the assembled code at the current address in the current section. This is generally preceded by a label or other DB lines. Strings can be used and will generate one byte for each character. Strings and numbers can be separated with a comma. If a label is provided it will represent the address where this directive stores the data.| 
|`[<label>:] .DW <expression> [[, <expression>] ...]` or `[<label>:] .WORD <expression> [[, <expression>] ...]`|`.DW 0xAA55`  `.DW EndOfMonitor-StartOfMonitor`|Insert a sequence of 16-Bit numbers in the assembled code at the current address in the current section. This is generally preceded by a label or other DW lines. Strings can be used but for each character they will generate an 8-bit zero followed by the byte value of each character. Strings and numbers can be separated with a comma. If a label is provided it will represent the address where this directive stores the data.|   
|`.PROC <processor code>`|`.PROC Z80`|Defines the processor instruction set to use. Ignored for now, should be defined as `Z80` for SCWorkshop compatibility.|
|`.HEXBYTES <expression>`||Ignored|


### EQU Symbols

Symbols defined with an `.EQU` directive are replaced by the result of the expression in their `.EQU` directive. 
Example:

In the following lines, the symbol `WRKSPC` is defined with the value being the value defined by another symbol, `kWRKSPC`. The next line defines `USR` as being the sum of `WRKSPC` and 3, which in this context means that the `USR` symbol represents the address 3 bytes after the address represented by `WRKSPC`. 

    WRKSPC      .EQU    kWRKSPC     ; BASIC Work space    <SCC> original 2045H  
    USR         .EQU    WRKSPC+3H   ; "USR (x)" jump

Remark: because the assembler does 2 passes on source files, the `.EQU` symbols can be used in expressions even before they appear in their `.EQU` directive. Their actual value will be known on the second pass, when code is actually generated.

## Special Contexts

There are contexts where MUZ-Assembler adopt a specific behaviour. Here are some details.

### Parenthesis and Indirect Addressing

Parenthesis can be used anywhere in expressions, however there is a special case where they surround something else than a numerical expression: it's when an indirect address is expected.

Indirect addressing is a mode where a number is not used as a direct value, but rather as an address where the actual data will be read or written.

Example: 

    store: .DW 0
    ...
    LD (store), HL
    
In this example, the label `store` is defined at current address and will reserve 2 bytes of space. Later, the `LD (store), HL` will take the content of the `HL` register and put it in the space pointed at by the `store` address.

The out parenthesis can contain any numerical expression to form the indirect address. The only rule is that the expression will be evaluated to a constant number at the assembling time. You can use `#DEFINE` symbols or label names computed by string expressions to change the result of an indirect address expression, but you cannot use the content of registers in expressions because they are run-time dependant and only available through the processor instructions.

There are two other forms of indirect addressing which uses registers instead of expressions:

* `(BC)`, `(DE)`, `(HL)` and sometimes `(SP)` can be used in some instructions to use the content of the register as an indirect address
* `(IX+d)`and `(IY+d)` can generally be used where `(HL)` can, and will add the content of `IX` or `IY` register and a decimal constant `d` from 0 to 255 to form an indirect address (any numerical expression can be used for `d`)

**Remark:** As a general rule, register names in the middle of numerical expressions will be considered as unknown symbols and have the value `0`.

**Remark:** Opening parenthesis will activate indirect addressing where it can be expected, so if a direct value is wanted, surrounding parenthesis should not be used. E.g. `LD HL, 8 * 1024` will load the value `8192` in the HL register, while `LD HL, (8 * 1024)` will load the 16-bit value stored at address 8192.


### String Constants and Expressions

Strings can be written between double quotes, and can be concatenated with the `+` operator. However, if one of the operands is a number, this number will be added to the code of each character in the string operand: concatenation only works when the two operands are strings.

Symbols defined with #DEFINE can be used in expressions and will be replaced by numbers or strings depending on the expression type where they appear. Be sure to check the resulting value in the assembly listing.

Strings can be compared using all comparison operators for equality or inequality: the ASCII character order is used in comparisons, so uppercase characters are *inferior* to lowercase characters.

#### Joker `*` Character

In equality and difference comparisons, the `*` character acts as a joker which ends the comparison. If the characters are identical to the string expression until this joker, the result is true.

This joker is only for equality and difference: in all other expressions, the `*` character is left unchanged.

The actual `*` character can be forced and the joker disabled by escaping it with a backslash character: `"\*"` represents the `"*"` in all contexts.

Examples:

* `#IF BUILD = "T*"` is true if the defsymbol `BUILD` has been defined as `T1`  or `T4` but not if it is `R0`.
* `#IF BUILD = "T" + "*"` is identical to the previous test above.
* `#IF BUILD = "R0"` is true if the defsymbol `BUILD` has been defined as `R0`. 
* `#IF BUILD = "T\*"` is true if the defsymbol `BUILD` has been defined as `T\*`

**Remark:** the joker is only available in `#IF` directives, and only for the `=`/`==`and `!=`/`<>` comparisons.

#### Concatenation Addition and Subtraction

A special feature allows the programmer to add or subtract numerical values to character codes. 

When a string is being added or subtracted a number in an expression, the effect is to add or subtract the numerical value to each of the characters in the string. If `+` is used on two strings, it concatenates them.

This only works with the `+` and `-` operators, and only for the `string <operator> number` expressions.

For example the expression `"AB" + 1` adds 1 to each character, resulting in the string `"BC"`. But the expression `"AB" + "1"` works on two strings and results in the string `"AB1"`.


## Assembler Output

MUZ-Assembler outputs a number of files on demand. Using C++ calls, the programmer can set an output directory and various file names. An empty file name will disable the output.

### Output Directory

The assembler must be set up with an output directory. It will write each file in this directory.

### Listing File

The *listing file* is a text file which shows the result of the assembly process, along with the original source lines of code and various informations.

Each line of a source file is listed, along with address, value, line number and source:

    00C1: 11 01 00      0031  Selftest:   LD   DE,1           ;Prepared for delay loop
    00C4: 7B            0032              LD   A,E            ;First byte to write to LEDs = 0x01
    
In this part of the listing, the lines defining a label or an EQU symbol show the address or value for that symbol at the beginning of the line:

    0052:               0022  kConfMajor: .EQU 'R'            ;Config: Letter = official, number = user
    ...
    0223:               0630  OutputMessage:

The `#INCLUDE`, `#INSERTBIN` and `#INSERTHEX` directives show a break in the listing with the listing of the included file, and show the return to the original source when the listing of the included file is finished:

                        0077  #IF         BUILD = "R0"
                        0078  #INCLUDE    Hardware\Custom\Config_R0.asm

                        Hardware/Custom/Config_R0.asm

                        0001  ; **********************************************************************
                        0002  ; **  Config: R0 = RC2014                       by Stephen C Cousins  **
                        ...
                        0095  
                        0096  

                        /Users/bkg2018/Desktop/SCWorkshop019_SCMonitor100_20181027/SCMonitor/Source/!Main.asm
                        0079  #ELSE


The lines which are not assembled following an `#IF` or `#IFDEF` test directive or an `#ELSE` are listed with no address and code and do not follow the include or insert directives:

                        0326  #IFDEF      IncludeScripting
                        0327              CALL ScrInitialise  ;Initialise script language
                        0328  #ENDIF
                        
The lines which are assembled following a test directive are listed with address and code:                      
                    
                        0329  #IFDEF      IncludeRomFS
    012A: CD E3 1A      0330              CALL RomInitialise  ;Initialise ROM filing system
                        0331  #ENDIF

After all lines have been listed, the file receives a list of each section with their name and all the separate address ranges it contains:

    Sections:
    -----------------------------------------------------------------------
    DATA: [FE00-FEB0]
    CODE: [0000-1E7F] [2000-3E21] [3F80-3FFF]


Then the assembler lists all the symbols defined by `#DEFINE` with their value if they have one:

    Defines:
    ------------------------------------------------------------------------
    BUILD                          :R0
    IncludeAPI                     :
    IncludeAssembler               :
    ...

Then the labels set to a value with `.EQU` are listed with their value:

    Equates:
    ------------------------------------------------------------------------
    BasicCodeW                     :2003
    CmdReset                       :0000
    ...

Finaly the listing lists all the global labels on two columns: labels are sorted by name in the first column and by address in the second:

    Global labels:
    --- By Name -------------------------------|---By Address --------------
    	APIHandler                     :04FD   |   0000: ColdStart
    	APITable                       :050F   |   0003: WarmStart
    	AsmSkipOverDelimiter           :0CE4   |   0014: WStrt
    	...


**Remarks:** Some labels may not appear in the second column because only one label is listed for each address and some labels can point to the same address.


### Memory Listing File

The memory listing file shows the list of sections, and then the binary content of each section address range, both in hecadecimal and ASCII form. Characters with a code greater than 127 (0x7F) are displayed as a simple dot `.`.

    Sections:
    	DATA: [FE00-FEB0]
     	CODE: [0000-1E7F] [2000-3E21] [3F80-3FFF]
    
    [0000-1E7F]:CODE
    0000:  C3 BD 00 18  0F C3 64 05  C3 03 FE 00  C3 BD 00 00   ......d.........
    0010:  C3 06 FE 00  C3 4B 01 00  C3 09 FE 00  00 00 00 00   .....K..........
    0020:  C3 0C FE 00  00 00 00 00  C3 0F FE 00  00 00 00 00   ................
    ...
    3E10:  65 70 61 72  65 64 20 43  46 20 63 61  72 64 29 0D   epared CF card).
    3E20:  0A 00                                                ..              
    
    [3F80-3FFF]:CODE
    3F80:  55 AA 43 50  4D 20 20 20  20 20 03 00  DE 3D 44 00   U.CPM     ...=D.
    3F90:  55 AA 43 50  4D 20 20 20  20 20 41 80  60 3C B4 00   U.CPM     A.`<..
    ...

### Intel HEX Output

The HEX output file contains the result of the assembly in a form which can be directly used to set up a computer memory content in various contexts.

    :10000000C3BD00180FC36405C303FE00C3BD0000D9
    :10001000C306FE00C34B0100C309FE000000000040
    ...
    :103FE00055AA424153494320202001000020601C73
    :103FF00055AA4D6F6E69746F722002000000801E1A
    :00000001FF

Here are some of the contexts where an HEX file can be used:

* EPROM burners can generaly use HEX Intel files to program an EPROM
* On the RC2014 computer, the SC Monitor program can receive and interpret the HEX file contents and put the binary content directly at the right address. Some BASIC programs exists which accepts HEX files as well.
* In MUZ-Computer, the MemoryModule class can load an HEX file.


