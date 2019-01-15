# MUZ-Workshop

***NOTICE: this is preliminary work ***

MUZ is a Z-80 development workshop for MacOS and aimed at all OS-es, inspired by the [Small Computer Monitor Workshop from Steven Cousins](https://smallcomputercentral.wordpress.com/projects/small-computer-workshop/).

Its heart is a C++ 11 conformant library featuring an internal C++ **Z-80 assembler** and a **computer model**. For the initial Z-80 version, MUZ computer model is integrating the [YAZE Z-80 emulator](http://www.mathematik.uni-ulm.de/users/ag/yaze-ag) as its **virtual CPU** and implements classes for the RC2014 computer.

MUZ Computer has been designed to emulate the RC2014 backplane Pro with Z-80 CPU, Clock, Pageable ROM, 64KB RAM and SIO/2 Serial modules.

MUZ Assembler is designed to be compatible with Small Computer Workshop outputs, and will compile the SC Monitor sources with no modification.

The project features a MacOS workshop, which in turn links with the C++ MUZ library into a multi editor / debugguer / assembler development system, trying to bring to the Mac the same services as Steve Cousins SCWorkshop on Windows.

Main directories:

* `MUZ` : This is the main directory for the C++ code base and associated files
  * `Documentation` : holds various documents in Markdown for other formats, this is currently only work docs and should not be considered as strict specifications
  * `MUZ` : main C++ source directory
      * ` MUZ-Assembler` : source files related to the `Assembler` class
      * `MUZ-Common` : source files common to all parts of MUZ
      * `MUZ-Computer`: source files related to the computer model classes (memory modules, port modules, computer, cpu ...)
      * `Tests` : source files for testing programs, tghey all feature a main which must be linked to MUZ
  * `TestSources` : assembler source files for tests
  * `YAZE`: YAZE emulator integration and source files
      
* `Platform` : Main directory for OS specific files
      * `MacOS` : directory for MacOS specific files
        * `MUZ-Workshop` : project directory for the MacOS workshop application 
        * `MUZ-WorkshopTests`: directory for the MUZ-Workshop unit tests
        * `MUZ-Workshop.xcodeproj` : XCode 10 project file for the Workshop application 

There is no makefile or other OS development system project yet, you need XCode 10 to build the MUZ-Workshop project as of now. Apart from the MacOS UI and Objective-C Bridging specifics, anything in the `MUZ` root directory tries to stay standard C++ 11 which should be compilable on a lot of systems - as opposed to what's in `Platform` directory.

December 2018

Francis Piérot

