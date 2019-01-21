# MUZ-Workshop

***NOTICE: this is preliminary work ***

MUZ is a Z-80 development workshop for MacOS with sub components aimed at all OS-es, inspired by the [Small Computer Monitor Workshop from Steven Cousins](https://smallcomputercentral.wordpress.com/projects/small-computer-workshop/).

Its heart is a C++ 11 compliant library with an internal C++ **Z-80 assembler** and a **computer model**. For the initial Z-80 version, MUZ computer model is integrating the [YAZE Z-80 emulator](http://www.mathematik.uni-ulm.de/users/ag/yaze-ag) as its virtual CPU and implements classes for the [RC2014 computer](https://rc2014.co.uk/).

* *MUZ-Computer* has been designed to emulate the RC2014 built with a Backplane Pro and Z-80 CPU, Clock, Pageable ROM, 64KB RAM and SIO/2 Serial modules. *This is work in progress and is not available yet.*

* *MUZ-Assembler* is designed to be compatible with Small Computer Workshop outputs, and will compile the SC Monitor sources with no modification. MUZ-Assembler features a lot of helps for the programmer, like cross-references, label listings and warnings. See more details in [MUZ-Assembler documentation](MUZ/Documentation/MUZ-Assembler.md).

* *MUZ-Workshop* is an EDI project in Swift for MacOS XCode. It links with the C++ MUZ library into a multi editor / debugguer / assembler development system, trying to bring to the Mac the same services as Steve Cousins SCWorkshop on Windows. *This is work in progress and is not available yet.*

Main directories:

* `MUZ` : This is the main directory for the C++ code base and associated files
work docs and should not be considered as strict specifications
  * `asMUZ` : main source directory for `asmuz` command-line tool sources
  * `Documentation` : holds various documents in Markdown formats   
  * `muzlib` : main source directory for `muzlib` library 
      * `MUZ-Assembler` : source files related to the `Assembler` class
      * `MUZ-Common` : source files common to all parts of MUZ
      * `MUZ-Computer`: source files related to the computer model classes (memory modules, port modus, computer, cpu ...)
    * `Tests` : source files for testing programs, they feature a main which must be linked to `muzlib`
  * `TestSources` : Z-80 assembler source files for unit-tests
  * `YAZE`: YAZE emulator source files integrated for MUZ
      
* `Platform` : Main directory for OS specific files
  * `MacOS` : directory for MacOS specific files
      * `asmuz` : directory for the XCode project for command-line utility 
      * `MUZ-Workshop` : directory for the MacOS workshop sources
      * `MUZ-WorkshopTests`: directory for the MacOS unit tests
      * `muzlib` : directory for the MacOS prohect
      * `MUZ-Workshop.xcworkspace` : XCode 10 workspace for the Workshop application, the `muzlib` library and the `asmuz` command-line tool.

There is no makefile or other OS development system project yet, you need XCode 10 to build the MUZ-Workshop project as of now. Apart from the MacOS UI and Objective-C Bridging specifics, anything in the `MUZ` root directory tries to stay standard C++ 11 which should be compilable on a lot of systems - as opposed to what's in `Platform` directory.

December 2018

Francis Piérot

