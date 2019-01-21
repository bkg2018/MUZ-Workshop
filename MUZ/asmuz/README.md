#  MUZ-Assembler Command Line Utility - asmuz

The `asmuz` tool is a simple command line utility to compile assembler source files from a shell.

Syntax :

    asmuz [command [...]] [--input|-f] inputfile

Parameters for this utility are:

| option | Details | C++ equivalent |
|---------|--------|-------------------|
| `--outputdir <path>` or `-od <path>`| Sets the output directory where files will be written. | as.SetOutputDirectory("/Users/bkg2018/Desktop/RC2014/MUZ-Workshop/Output");
| `--listing <filename>` or `-l <path>` | Sets the file name for the assembly listing |  as.SetListingFilename("testErrors.LST");
| `--memory <filename>` or `-m <path>` | Sets the file name for the memory dump | as.SetMemoryFilename("testErrorsMemory.LST");
| `--hex <filename>` or `-h <path>` | Sets the file name for the Intel HEX output | as.SetIntelHexFilename("testErrorsIntelHex.HEX");
| `--log <filename>` | Sets the file name for the warnings/errors log | as.SetLogFilename("testErrors.LOG");
| `--allbytes` or | Enables full byte sequences in listing, by default byte sequences are limited to 7 bytes (on 2 lines) with ellipsis "..." | as.EnableFullListing(true);
| `[--input|-f] <inputfile>`| Sets the file path of the main input source file | as.AssembleFile(SourcesRootDir + "Errors.asm", msg); 


