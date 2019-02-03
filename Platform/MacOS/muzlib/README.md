# `muzlib` Static Library

The `muzlib` static library is a C++ set of sources and headers containing MUZ-Assembler, MUZ-Computer and related parts.

* MUZ-Assember takes a main ASM source file and generates an HEX file and optional listings
* MUZ-Computer allows to build a virtual computer using various modules like IO Ports or Memory modules and a virtual CPU using an emulator. 

Currently, MUZ-Assembler assembles Z-80 assembler files into an HEX file, while MUZ-Computer emulates a virtual RC2014 computer with Pageable ROM, 64K RAM and Digital I/O virtual modules, and can load hex files in the ROM or RAM modules.

MUZ-Assembler is 100% compatible with Steve Cousin's Small Computer Workshop assembler and will generate the same HEX output file from Small Computer Monitor source files.
